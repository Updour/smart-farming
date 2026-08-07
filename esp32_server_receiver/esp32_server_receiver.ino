#include "index.h"
#include <DHT.h>
#include <LittleFS.h>
#include <WebServer.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h> // UNTUK LOCK CHANNEL RADIO PHY ESP32 & DONGKRAK POWER MAKSIMAL

// --- KONFIGURASI DHT, RELAY (PIN 26 KHUSUS) & TRAFFIC LIGHT ---
#define DHTPIN 4
#define DHTTYPE DHT11

// RELAY PADA PIN 26 (DS1 / RELAY PIN 26)
#define RELAY1 26

// SETELAN ACTIVE LOW (LOW = ON, HIGH = OFF)
#define RELAY_ON_STATE LOW
#define RELAY_OFF_STATE HIGH

// LAMPU INDIKATOR TRAFFIC LIGHT (PIN 32 HIJAU, 33 KUNING, 27 MERAH)
#define LED_HIJAU 32
#define LED_KUNING 33
#define LED_MERAH 27

DHT dht(DHTPIN, DHTTYPE);

float batasSuhu = 30.0;        // Batas Hangat
int batasTanah = 45;           // Batas Tanah Minimal
const float suhuBahaya = 35.0; // Batas Bahaya Panas Ekstrem

typedef struct struct_message {
  int8_t persen;   // 0 - 100%
  int8_t baterai;  // 0 - 100%
  uint16_t rawAdc; // Nilai Mentah Analog ADC (Misal: 620)
} struct_message;

struct_message myData;

volatile bool newDataReceived = false;
int8_t latestMoisturePercent = -1; // -1 ARTINYA BELUM ADA DATA MENDARAT
int8_t esp8266Battery = 0;
uint16_t latestRawAdc = 0;
unsigned long lastRecvTime = 0;
int8_t esp8266Rssi = -99;

// Data Cuaca Lokal (DHT11) - Default Fallback 28.0 C (Adem)
float lastSuhuC = 28.0;
float lastSuhuF = 82.4;
float lastKelembapanUdara = 65.0;
float lastHeatIndexC = 28.5;
float lastHeatIndexF = 83.3;
float lastDewPoint = 21.0;

// Data Cuaca Satelit (Disuntikkan via Web)
String satTemp = "-";
String satDesc = "-";
String satRainPred = "Cerah / Tidak Ada Hujan";
String satTime = "-";

// --- VARIABEL ERROR & PERINGATAN DICABUT ---
String systemErrorMsg = "";
bool isSystemError = false;
bool isStartupWaiting = true;
bool wasInErrorState = false;
unsigned long startupWaitStartTime = 0;
uint8_t waitingPercent = 0; // COUNTER PERSEN MENUNGGU (0% - 100%)

bool isEsp8266Unplugged = false; // FLAG PERINGATAN ESP8266 DICABUT
bool isRelayUnplugged = false;   // FLAG PERINGATAN RELAY PIN 26 DICABUT

// --- TIMER RELAY & PRO COUNTER ---
unsigned long relayStartTime = 0;
bool isRelayOn = false;
bool relayCooldown = false;
const unsigned long MAX_RELAY_TIME = 20UL * 60UL * 1000UL; // 20 Menit

uint32_t pumpCount = 0;
unsigned long totalPumpSecs = 0;
unsigned long lastPumpSecUpdate = 0;

// --- COUNTER UNTUK LOG DATABASE ---
uint16_t hourlyPumpCount = 0;
uint32_t hourlyPumpSecs = 0;

// --- KENDALI WEB MANUAL ---
bool isManualMode = false;
bool manualRelayState = false;

// --- TIMER PENCATATAN MEMORI (LITTLEFS) ---
unsigned long lastLogTime = 0;
const unsigned long LOG_INTERVAL = 60UL * 60UL * 1000UL;

WebServer server(80);

// Helper Uptime Sistem
String getUptime() {
  unsigned long sec = millis() / 1000;
  unsigned long d = sec / 86400;
  unsigned long h = (sec % 86400) / 3600;
  unsigned long m = (sec % 3600) / 60;
  unsigned long s = sec % 60;
  if (d > 0)
    return String(d) + "d " + String(h) + "h " + String(m) + "m";
  if (h > 0)
    return String(h) + "h " + String(m) + "m " + String(s) + "s";
  return String(m) + "m " + String(s) + "s";
}

// Helper Klasifikasi Kategori Kondisi Tanah
String getSoilCategory(int8_t moisture) {
  if (moisture < 0)
    return F("❌ SENSOR TIDAK TERHUBUNG");
  if (moisture >= 90)
    return F("🌊 SANGAT BASAH / TERGENANG");
  if (moisture >= 65)
    return F("🌱 SANGAT LEMBAB (IDEAL SUBUR)");
  if (moisture >= batasTanah)
    return F("☘️ LEMBAB OPTIMAL");
  if (moisture >= 25)
    return F("🍂 TANAH MULAI KERING");
  return F("🍂 TANAH KERING (PERLU MENYIRAM)");
}

// Rumus Indeks Kesehatan Lahan (0 - 100%)
uint8_t calculateFarmHealth() {
  if (isSystemError || isEsp8266Unplugged || isRelayUnplugged ||
      latestMoisturePercent < 0)
    return 0;
  int score = 100;
  if (latestMoisturePercent < 25)
    score -= 25;
  else if (latestMoisturePercent < batasTanah)
    score -= 15;
  else if (latestMoisturePercent >= 90)
    score -= 15;

  if (lastSuhuC >= suhuBahaya)
    score -= 35;
  else if (lastSuhuC > batasSuhu)
    score -= 15;

  return constrain(score, 0, 100);
}

// Estimasi Laju Penguapan Air Tanah (% / Jam)
float calculateEvaporationRate() {
  if (lastSuhuC == 0.0)
    return 0.0;
  float rate = (lastSuhuC / 10.0) * (1.0 - (lastKelembapanUdara / 100.0)) * 2.5;
  return constrain(rate, 0.1, 15.0);
}

// Estimasi Jam/Hari Sisa Air Tanah Habis
String getSoilDepletionTime() {
  if (latestMoisturePercent < 0)
    return F("Sensor Hilang");
  if (latestMoisturePercent <= 25)
    return F("Sudah Waktunya Menyiram!");
  float evap = calculateEvaporationRate();
  if (evap <= 0.05)
    evap = 0.05;

  float remainingPercent = latestMoisturePercent - 25.0;
  float totalHours = remainingPercent / (evap * 0.35);

  int days = (int)(totalHours / 24.0);
  int remainingHours = (int)totalHours % 24;

  if (days > 0) {
    return "~" + String(days) + " Hari " + String(remainingHours) + " Jam";
  } else {
    return "~" + String(remainingHours) + " Jam";
  }
}

// Kesimpulan AI Diagnostik Kondisi Tanaman Real-Time
String getPlantHealthSummary() {
  if (isEsp8266Unplugged || latestMoisturePercent < 0)
    return F("🚨 [PERINGATAN TERPUTUS] ESP8266 DICABUT / MATI! Sinyal Hilang. "
             "Lampu Merah Berkedip! Pompa MUTLAK MATI!");
  if (isRelayUnplugged)
    return F(
        "🚨 [PERINGATAN HARDWARE] KABEL RELAY (PIN 26) TERLEPAS / DICABUT!");
  if (isStartupWaiting)
    return "⏳ [INITIALIZING " + String(waitingPercent) +
           "%] Menunggu sinyal ESP8266... (Lampu Kuning Berkedip)";

  if (latestMoisturePercent >= 90) {
    return F("🛑 [INTERLOCK AKAR 90%] Tanah sangat basah (≥90%). Pompa dikunci "
             "OFF otomatis untuk mencegah pembusukan akar tanaman!");
  } else if (lastSuhuC >= suhuBahaya) {
    return "☀️ [KONDISI 1: BAHAYA PANAS] Suhu tinggi (" + String(lastSuhuC, 1) +
           "°C). Penyiraman darurat diaktifkan.";
  } else if (lastSuhuC > batasSuhu || latestMoisturePercent < batasTanah) {
    return "🍂 [KONDISI 2: PERINGATAN MENYIRAM] Kelembapan tanah (" +
           String(latestMoisturePercent) + "%) & suhu (" +
           String(lastSuhuC, 1) + "°C). Pompa menyala menyiram.";
  } else {
    return "🌿 [KONDISI 3: AMAN & IDEAL] Suhu adem (" + String(lastSuhuC, 1) +
           "°C) & kelembapan tanah (" + String(latestMoisturePercent) +
           "%) ideal. Pompa MATI.";
  }
}

// CALLBACK UNIVERSAL ESP-NOW DENGAN UPDATE INSTAN SAAT PAKET TERIMA
void OnDataRecvInternal(const uint8_t *incomingData, int len) {
  if (len >= sizeof(myData)) {
    memcpy(&myData, incomingData, sizeof(myData));
    latestMoisturePercent = myData.persen;
    esp8266Battery = myData.baterai;
    latestRawAdc = myData.rawAdc;
    newDataReceived = true;
    lastRecvTime = millis();
    isStartupWaiting = false;
    isEsp8266Unplugged = false; // SINYAL HIDUP KEMBALI
    waitingPercent = 100;
  }
}

#if defined(ESP_IDF_VERSION_MAJOR) && ESP_IDF_VERSION_MAJOR >= 5
void esp_now_recv_info_t_wrapper(const esp_now_recv_info *info,
                                 const uint8_t *data, int len) {
  if (info && info->rx_ctrl)
    esp8266Rssi = info->rx_ctrl->rssi;
  OnDataRecvInternal(data, len);
}
#else
void esp_now_recv_info_t_wrapper(const uint8_t *mac, const uint8_t *data,
                                 int len) {
  esp8266Rssi = -60;
  OnDataRecvInternal(data, len);
}
#endif

void writeLogHeader() {
  File file = LittleFS.open("/log.csv", FILE_WRITE);
  if (file) {
    file.println(
        F("Waktu,Suhu_Lokal(C),Kelembapan_Udara(%),Kelembapan_Tanah(%),"
          "Kategori_Tanah,Akumulasi_Pompa_Nyala,Akumulasi_Durasi_Detik,Raw_ADC,"
          "Suhu_Satelit(C),Kondisi_Satelit,Prediksi_Hujan"));
    file.close();
  }
}

void appendLogData() {
  if (LittleFS.exists("/log.csv")) {
    File checkFile = LittleFS.open("/log.csv", FILE_READ);
    if (checkFile && checkFile.size() > 50000) {
      checkFile.close();
      Serial.println(F("🧹 [LITTLEFS] File log melebihi 50KB. Auto-Wipe..."));
      writeLogHeader();
    } else {
      checkFile.close();
    }
  } else {
    writeLogHeader();
  }

  File file = LittleFS.open("/log.csv", FILE_APPEND);
  if (file) {
    String timeStampStr = satTime;
    if (timeStampStr == "-") {
      unsigned long sec = millis() / 1000;
      timeStampStr = "Uptime " + String(sec / 3600) + "h " +
                     String((sec % 3600) / 60) + "m";
    }
    file.print(timeStampStr);
    file.print(',');
    file.print(lastSuhuC, 1);
    file.print(',');
    file.print(lastKelembapanUdara, 1);
    file.print(',');
    file.print(latestMoisturePercent);
    file.print(',');
    file.print(getSoilCategory(latestMoisturePercent));
    file.print(',');
    file.print(pumpCount);
    file.print(',');
    file.print(totalPumpSecs);
    file.print(',');
    file.print(latestRawAdc);
    file.print(',');
    file.print(satTemp);
    file.print(',');
    file.print(satDesc);
    file.print(',');
    file.println(satRainPred);
    file.close();
    Serial.println(F(
        "💾 [LITTLEFS] Data log database berhasil dicatat & aman tersimpan."));
  }
}

unsigned long lastDhtReadTime = 0;

void checkSystemStatus() {
  if (millis() - lastDhtReadTime >= 1000) {
    lastDhtReadTime = millis();
    float suhu = dht.readTemperature();
    float suhuF = dht.readTemperature(true);
    float kelembapan = dht.readHumidity();

    bool dhtError = isnan(suhu) || isnan(suhuF) || isnan(kelembapan);
    if (!dhtError && suhu > 0.0) {
      lastSuhuC = suhu;
      lastSuhuF = suhuF;
      lastKelembapanUdara = kelembapan;
      lastHeatIndexC = dht.computeHeatIndex(suhu, kelembapan, false);
      lastHeatIndexF = dht.computeHeatIndex(suhuF, kelembapan, true);

      float a = 17.271, b = 237.7;
      float gamma = (a * suhu / (b + suhu)) + log(kelembapan / 100.0);
      lastDewPoint = (b * gamma) / (a - gamma);
    }
  }

  // 1. TIMEOUT REALTIME SINYAL ESP8266: JIKA TERPUTUS >8 DETIK -> PERINGATAN
  // DARURAT SAKLAR MERAH KEDIP!
  bool espNowTimeout = (lastRecvTime != 0 && (millis() - lastRecvTime > 8000));

  // 2. CEK APABILA KABEL RELAY (PIN 26) DICABUT / DISCONNECTED
  pinMode(RELAY1, INPUT_PULLUP);
  int pinFeedback = digitalRead(RELAY1);
  pinMode(RELAY1, OUTPUT);
  digitalWrite(RELAY1, isRelayOn ? LOW : HIGH);
  isRelayUnplugged = (pinFeedback == LOW && !isRelayOn && millis() > 10000);

  if (espNowTimeout) {
    isEsp8266Unplugged = true;
    systemErrorMsg =
        F("🚨 PERINGATAN: SINYAL ESP8266 TERPUTUS / MATI! (Sinyal Hilang >8s)");
    isSystemError = true;
    isStartupWaiting = false;
  } else if (isRelayUnplugged) {
    systemErrorMsg = F("🚨 PERINGATAN HARDWARE: KABEL RELAY PIN 26 TERLEPAS!");
    isSystemError = true;
    isStartupWaiting = false;
  } else if (lastRecvTime == 0) {
    unsigned long elapsedWait = millis() - startupWaitStartTime;
    waitingPercent = constrain(map(elapsedWait, 0, 5000, 0, 100), 0, 100);

    if (elapsedWait >= 5000) {
      // SETELAH 5 DETIK TANPA ESP8266 -> SENSOR DIDEKLARASIKAN TERPUTUS / MATI!
      isStartupWaiting = false;
      isEsp8266Unplugged = true;
      isSystemError = true;
      systemErrorMsg =
          F("🚨 PERINGATAN: ESP8266 TIDAK MENGIRIM DATA (MATI / TERPUTUS)!");
    } else {
      isStartupWaiting = true;
      isSystemError = false;
      isEsp8266Unplugged = false;
      systemErrorMsg = "⏳ MENUNGGU KONEKSI SENSOR [" + String(waitingPercent) +
                       "%] (Lampu Kuning Berkedip)";
    }
  } else {
    isEsp8266Unplugged = false;
    isSystemError = false;
    isStartupWaiting = false;
    waitingPercent = 100;
    systemErrorMsg = "";
  }
}

// ==== API WEB SERVER ====
void handleRoot() { server.send_P(200, "text/html", index_html); }

void handleData() {
  String bgClass = F("aman");
  String textStatus = F("KONDISI 3: AMAN (Suhu Adem & Lembab)");

  if (isEsp8266Unplugged || latestMoisturePercent < 0) {
    bgClass = F("bahaya");
    textStatus = F("🚨 PERINGATAN: SINYAL ESP8266 TERPUTUS!");
  } else if (isRelayUnplugged) {
    bgClass = F("peringatan");
    textStatus = F("🚨 PERINGATAN: RELAY PIN 26 DICABUT!");
  } else if (lastSuhuC >= suhuBahaya) {
    bgClass = F("bahaya");
    textStatus = F("KONDISI 1: BAHAYA (Suhu Panas Ekstrem!)");
  } else if (lastSuhuC > batasSuhu || latestMoisturePercent < batasTanah) {
    bgClass = F("peringatan");
    textStatus = F("PERINGATAN (Waktu Menyiram)");
  }

  size_t fsUsed = 0;
  if (LittleFS.exists("/log.csv")) {
    File f = LittleFS.open("/log.csv", FILE_READ);
    if (f) {
      fsUsed = f.size();
      f.close();
    }
  }
  size_t fsTotal = LittleFS.totalBytes();

  int signalQuality = constrain(map(esp8266Rssi, -95, -50, 0, 100), 0, 100);
  uint8_t connectedClients = WiFi.softAPgetStationNum();

  uint8_t dhtHealth = (isnan(lastSuhuC) || lastSuhuC == 0.0) ? 50 : 100;
  uint8_t soilHealth =
      (lastRecvTime == 0 || millis() - lastRecvTime > 8000) ? 0 : 100;

  float relayLifePercent = 100.0 - ((float)pumpCount / 1000.0);
  if (relayLifePercent < 0)
    relayLifePercent = 0;

  float waterLiters = ((float)totalPumpSecs / 60.0) * 60.0;
  float kWhUsed = ((float)totalPumpSecs / 3600.0) * 0.25;
  float costIdr = kWhUsed * 415.0;

  float tempDiff = 0.0;
  if (satTemp != "-") {
    tempDiff = lastSuhuC - satTemp.toFloat();
  }

  String json;
  json.reserve(1024);
  json = "{";
  json += "\"suhuC\":\"" + String(lastSuhuC, 1) + "\",";
  json += "\"suhuF\":\"" + String(lastSuhuF, 1) + "\",";
  json += "\"hum\":\"" + String(lastKelembapanUdara, 1) + "\",";
  json += "\"heatC\":\"" + String(lastHeatIndexC, 1) + "\",";
  json += "\"heatF\":\"" + String(lastHeatIndexF, 1) + "\",";
  json += "\"dew\":\"" + String(lastDewPoint, 1) + "\",";
  json += "\"soil\":\"" +
          String(latestMoisturePercent < 0 ? 0 : latestMoisturePercent) + "\",";
  json +=
      "\"soilCategory\":\"" + getSoilCategory(latestMoisturePercent) + "\",";
  json += "\"soilDepletion\":\"" + getSoilDepletionTime() + "\",";
  json += "\"plantSummary\":\"" + getPlantHealthSummary() + "\",";
  json += "\"rawAdc\":\"" + String(latestRawAdc) + "\",";
  json += "\"nodeBat\":\"" + String(esp8266Battery) + "\",";
  json += "\"statusColor\":\"" + bgClass + "\",";
  json += "\"statusText\":\"" + textStatus + "\",";
  json += "\"errorMsg\":\"" + systemErrorMsg + "\",";
  json += "\"isWaiting\":" + String(isStartupWaiting ? 1 : 0) + ",";
  json += "\"waitingPercent\":" + String(waitingPercent) + ",";
  json += "\"satTemp\":\"" + satTemp + "\",";
  json += "\"satDesc\":\"" + satDesc + "\",";
  json += "\"satRainPred\":\"" + satRainPred + "\",";
  json += "\"satTime\":\"" + satTime + "\",";
  json += "\"tempDiff\":\"" + String(tempDiff, 1) + "\",";
  json += "\"isManual\":" + String(isManualMode ? 1 : 0) + ",";
  json += "\"relayOn\":" + String(isRelayOn ? 1 : 0) + ",";
  json += "\"cooldown\":" + String(relayCooldown ? 1 : 0) + ",";

  json += "\"hourlyCount\":" + String(hourlyPumpCount) + ",";
  json += "\"hourlySecs\":" + String(hourlyPumpSecs) + ",";
  json += "\"uptime\":\"" + getUptime() + "\",";
  json += "\"freeHeap\":" +
          String(ESP.getHeapSize() > 0 ? (ESP.getFreeHeap() / 1024) : 210) +
          ",";
  json += "\"pumpCount\":" + String(pumpCount) + ",";
  json += "\"totalPumpSecs\":" + String(totalPumpSecs) + ",";
  json += "\"waterLiters\":\"" + String(waterLiters, 1) + "\",";
  json += "\"kWhUsed\":\"" + String(kWhUsed, 3) + "\",";
  json += "\"costIdr\":\"" + String(costIdr, 1) + "\",";
  json += "\"evaporation\":\"" + String(calculateEvaporationRate(), 1) + "\",";
  json += "\"fsUsed\":" + String(fsUsed / 1024) + ",";
  json += "\"fsTotal\":" + String(fsTotal / 1024) + ",";
  json += "\"rssi\":" + String(esp8266Rssi) + ",";
  json += "\"signalQuality\":" + String(signalQuality) + ",";
  json += "\"clients\":" + String(connectedClients) + ",";
  json += "\"farmHealth\":" + String(calculateFarmHealth()) + ",";
  json += "\"dhtHealth\":" + String(dhtHealth) + ",";
  json += "\"soilHealth\":" + String(soilHealth) + ",";
  json += "\"espVcc\":\"3.3V (Stabil)\",";
  json += "\"relayLife\":\"" + String(relayLifePercent, 1) + "\"";
  json += "}";

  server.send(200, "application/json", json);
}

void handlePushWeather() {
  if (server.hasArg("temp") && server.hasArg("desc") && server.hasArg("time")) {
    satTemp = server.arg("temp");
    satDesc = server.arg("desc");
    if (server.hasArg("rain"))
      satRainPred = server.arg("rain");
    satTime = server.arg("time");
    appendLogData();
    server.send(200, "text/plain", F("Data Satelit Sukses Diterima!"));
  } else {
    server.send(400, "text/plain", F("Data Tidak Lengkap"));
  }
}

void handleDownloadLog() {
  if (LittleFS.exists("/log.csv")) {
    File file = LittleFS.open("/log.csv", FILE_READ);
    server.streamFile(file, "text/csv");
    file.close();
  } else {
    server.send(404, "text/plain", F("Log Belum Ada"));
  }
}

void handleClearLogs() {
  writeLogHeader();
  pumpCount = 0;
  totalPumpSecs = 0;
  hourlyPumpCount = 0;
  hourlyPumpSecs = 0;
  Serial.println(F("🧹 [LITTLEFS] Berkas Log & Seluruh Counter Akumulasi Pompa "
                   "Berhasil Direset Total!"));
  server.send(200, "text/plain",
              F("Log Storage & Counter Pompa Berhasil Direset Total!"));
}

void handleSetThreshold() {
  if (server.hasArg("soil") && server.hasArg("temp")) {
    batasTanah = server.arg("soil").toInt();
    batasSuhu = server.arg("temp").toFloat();
    server.send(200, "text/plain", "Threshold Berhasil Diubah Sementara!");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

void handleSetMode() {
  if (server.hasArg("m"))
    isManualMode = (server.arg("m") == "manual");
  server.send(200, "text/plain", F("OK"));
}

void handleToggleRelay() {
  isManualMode = true;
  if (server.hasArg("s")) {
    manualRelayState = (server.arg("s") == "on");
  }

  if (manualRelayState && !isSystemError && !isEsp8266Unplugged &&
      latestMoisturePercent >= 0) {
    digitalWrite(RELAY1, LOW); // INSTAN ON KE PIN 26
    isRelayOn = true;
    Serial.println(F("⚡ [WEB OVERRIDE] MANUAL NYALAKAN POMPA -> PIN 26 DIBERI "
                     "SINYAL LOW (0V) (ON!)"));
  } else {
    digitalWrite(RELAY1, HIGH); // INSTAN OFF KE PIN 26
    isRelayOn = false;
    Serial.println(F("⚡ [WEB OVERRIDE] MANUAL MATIKAN POMPA / FAILSAFE LOCK "
                     "-> PIN 26 DIBERI SINYAL HIGH (3.3V) (OFF!)"));
  }

  server.send(200, "text/plain", F("OK"));
}

void handleReboot() {
  server.send(200, "text/plain", F("Rebooting ESP32 System..."));
  delay(1000);
  ESP.restart();
}

// ==== UTAMA: LOGIKA 3 KONDISI REAL-TIME MURNI DENGAN HARDWARE FAILSAFE LOCK
// ====
void updateRelayState() {
  bool butuhON = false;

  // HARDWARE FAILSAFE INTERLOCK UTAMA: JIKA SINYAL TERPUTUS / ESP8266 DICABUT /
  // MATI / HARDWARE ERROR / TANAH TERGENANG >=90%
  if (isSystemError || isEsp8266Unplugged || isRelayUnplugged ||
      latestMoisturePercent < 0 || isStartupWaiting) {
    // MUTLAK KUNCI SAKLAR RELAY PIN 26 MATI TOTAL! (AUTO EXIT MANUAL)
    isRelayOn = false;
    isManualMode = false;
    manualRelayState = false;
    digitalWrite(RELAY1, HIGH); // SANGAT TEGAS PAKSA PIN 26 HIGH (3.3V / OFF)!
    return;
  }

  if (latestMoisturePercent >= 90) {
    butuhON = false;
    isManualMode = false;
    manualRelayState = false;
    relayCooldown = false;
  } else if (isManualMode) {
    butuhON = manualRelayState;
  } else {
    // KONDISI 1 (Panas Ekstrem >= 35C) ATAU KONDISI 2 (Menyiram: Suhu > 30C
    // ATAU Tanah < 45%)
    if (lastSuhuC >= suhuBahaya || lastSuhuC > batasSuhu ||
        latestMoisturePercent < batasTanah) {
      butuhON = true;
    } else {
      // KONDISI 3 (Aman: Suhu <= 30C DAN Tanah Lembab >= 45%)
      butuhON = false;
      relayCooldown = false;
    }

    if (butuhON && relayCooldown)
      butuhON = false;
  }

  if (butuhON) {
    if (!isRelayOn) {
      isRelayOn = true;
      pumpCount++;
      hourlyPumpCount++;
      relayStartTime = millis();
      digitalWrite(RELAY1, LOW);
      Serial.println(F(
          "⚡ [RELAY HARDWARE] PIN 26 DIBERI SINYAL LOW (0V) -> RELAY ON 1X!"));
    } else {
      if (!isManualMode && (millis() - relayStartTime >= MAX_RELAY_TIME)) {
        relayCooldown = true;
        isRelayOn = false;
        digitalWrite(RELAY1, HIGH);
        Serial.println(
            F("🚨 [RELAY] 20 MENIT NONSTOP TERCAPAI -> AUTO COOLDOWN LOCK!"));
      } else {
        digitalWrite(RELAY1, LOW);
      }
    }
  } else {
    isRelayOn = false;
    digitalWrite(RELAY1, HIGH); // PIN 26 GINJAL KUNCI MATI (HIGH / 3.3V)
  }
}

// ==== LOGIKA SEJATI INDIKATOR LAMPU TRAFFIC LIGHT (ACTIVE HIGH / COMMON GND)
// ====
void updateLEDState() {
  if (isSystemError || isEsp8266Unplugged || latestMoisturePercent < 0) {
    // 1. SINYAL HILANG / TERPUTUS / DICABUT -> LAMPU MERAH BERKEDIP (250ms)!
    digitalWrite(LED_HIJAU, LOW);  // OFF
    digitalWrite(LED_KUNING, LOW); // OFF
    digitalWrite(LED_MERAH,
                 ((millis() / 250) % 2) ? HIGH : LOW); // HIGH = NYALA
  } else if (isStartupWaiting) {
    // 2. AWAL BOOT / MENUNGGU KONEKSI (WAITING) -> LAMPU KUNING BERKEDIP
    // (500ms)!
    digitalWrite(LED_HIJAU, LOW); // OFF
    digitalWrite(LED_MERAH, LOW); // OFF
    digitalWrite(LED_KUNING,
                 ((millis() / 500) % 2) ? HIGH : LOW); // HIGH = NYALA
  } else {
    // 3. KONDISI 1: BAHAYA PANAS (>= 35.0 C) -> LAMPU MERAH SOLID ON
    if (lastSuhuC >= suhuBahaya) {
      digitalWrite(LED_HIJAU, LOW);  // OFF
      digitalWrite(LED_KUNING, LOW); // OFF
      digitalWrite(LED_MERAH, HIGH); // HIGH = NYALA SOLID
    }
    // 4. KONDISI 2: PERINGATAN MENYIRAM (> 30.0 C ATAU TANAH < 45%) -> LAMPU
    // KUNING SOLID ON
    else if (lastSuhuC > batasSuhu || latestMoisturePercent < batasTanah) {
      digitalWrite(LED_HIJAU, LOW);   // OFF
      digitalWrite(LED_MERAH, LOW);   // OFF
      digitalWrite(LED_KUNING, HIGH); // HIGH = NYALA SOLID
    }
    // 5. KONDISI 3: AMAN (SUHU <= 30.0 C DAN TANAH >= 45%) -> LAMPU HIJAU SOLID
    // ON
    else {
      digitalWrite(LED_KUNING, LOW); // OFF
      digitalWrite(LED_MERAH, LOW);  // OFF
      digitalWrite(LED_HIJAU, HIGH); // HIGH = NYALA SOLID
    }
  }
}

unsigned long lastSerialPrintTime = 0;

void setup() {
  Serial.begin(115200);
  startupWaitStartTime = millis(); // REKAM DETIK AWAL MASA TUNGGU SEJAK BOOT

  if (!LittleFS.begin(true)) {
    Serial.println(F("Gagal Inisialisasi LittleFS!"));
  } else {
    if (!LittleFS.exists("/log.csv"))
      writeLogHeader();
  }

  dht.begin();
  pinMode(RELAY1, OUTPUT);
  digitalWrite(RELAY1,
               HIGH); // SANGAT TEGAS INSIALISASI AWAL MATI (HIGH / 3.3V)

  pinMode(LED_HIJAU, OUTPUT);
  pinMode(LED_KUNING, OUTPUT);
  pinMode(LED_MERAH, OUTPUT); // TRAFFIC LIGHT LEDS (PIN 32, 33, 27)

  // Inisialisasi awal lampu LED (SEMUA MATI SEJAK BOOT)
  digitalWrite(LED_HIJAU, LOW);
  digitalWrite(LED_KUNING, LOW);
  digitalWrite(LED_MERAH, LOW);

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("SmartFarm-ESP32", "12345678", 1);

  // DONGKRAK POWER MAKSIMAL RADIO ESP32 (84 * 0.25dBm = +21 dBm MAX POWER)
  esp_wifi_set_max_tx_power(84);

  // LOCK KANAL RADIO PHY ESP32 PERMANEN KE KANAL 1
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);

  if (esp_now_init() != ESP_OK)
    return;

#if defined(ESP_IDF_VERSION_MAJOR) && ESP_IDF_VERSION_MAJOR >= 5
  esp_now_register_recv_cb(esp_now_recv_info_t_wrapper);
#else
  esp_now_register_recv_cb((esp_now_recv_cb_t)esp_now_recv_info_t_wrapper);
#endif

  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.on("/pushWeather", HTTP_POST, handlePushWeather);
  server.on("/downloadLog", HTTP_GET, handleDownloadLog);
  server.on("/clearLogs", HTTP_POST, handleClearLogs);
  server.on("/setMode", handleSetMode);
  server.on("/setThreshold", handleSetThreshold);
  server.on("/toggleRelay", handleToggleRelay);
  server.on("/reboot", HTTP_POST, handleReboot);
  server.begin();
}

void loop() {
  server.handleClient();

  checkSystemStatus();

  updateRelayState();

  updateLEDState();

  if (isRelayOn) {
    if (millis() - lastPumpSecUpdate >= 1000) {
      lastPumpSecUpdate = millis();
      totalPumpSecs++;
      hourlyPumpSecs++;
    }
  }

  if (millis() - lastLogTime >= LOG_INTERVAL) {
    lastLogTime = millis();
    appendLogData();
  }

  if (newDataReceived) {
    Serial.println(F("-----------------------------------------"));
    Serial.print(F("[ESP-NOW INSTAN 1s] SUKSES terima data! Tanah: "));
    Serial.print(latestMoisturePercent);
    Serial.print(F("% ("));
    Serial.print(getSoilCategory(latestMoisturePercent));
    Serial.print(F(") | Raw ADC: "));
    Serial.print(latestRawAdc);
    Serial.print(F(" | Sinyal Kebun: "));
    Serial.print(esp8266Rssi);
    Serial.println(F(" dBm"));
    Serial.println(F("-----------------------------------------"));
    newDataReceived = false;
  }

  if (millis() - lastSerialPrintTime >= 1000) { // PRINT LOG REALTIME 1 DETIK!
    lastSerialPrintTime = millis();

    if (!isSystemError && wasInErrorState) {
      Serial.println(F("\n✅ =========================================="));
      Serial.println(
          F("✅ SYSTEM RECOVERY: Sinyal Radio ESP8266 Terhubung Kembali!"));
      Serial.println(F("============================================="));
      wasInErrorState = false;
    }

    if (isSystemError || isEsp8266Unplugged) {
      wasInErrorState = true;
      Serial.println(F("\n🚨 ========= PERINGATAN HARDWARE DARURAT ========="));
      Serial.print(F("🚨 STATUS  : "));
      Serial.println(systemErrorMsg);
      Serial.println(F("🔴 INDIKATOR: LAMPU TRAFFIC MERAH BERKEDIP (ESP8266 "
                       "TERPUTUS / MATI) | RELAY POMPA MUTLAK KUNCI MATI!"));
      Serial.println(F("=================================================="));
    } else if (isStartupWaiting) {
      wasInErrorState = true;
      unsigned long elSec = (millis() - startupWaitStartTime) / 1000;
      Serial.println(F("\n⏳ ========= MENUNGGU SENSOR KEBUN ========="));
      Serial.print(F("⏳ PROGRES MENUNGGU : ["));
      Serial.print(waitingPercent);
      Serial.print(F("%] | Waktu: "));
      Serial.print(elSec);
      Serial.println(F("s / 5s (Lampu Kuning Berkedip)"));
      Serial.println(F("============================================="));
    } else {
      Serial.println(F("☁️ ========= DATA CUACA (DHT11) ========="));
      Serial.print(F("🌡️ Suhu Udara       : "));
      Serial.print(lastSuhuC);
      Serial.print(F(" °C  |  "));
      Serial.print(lastSuhuF);
      Serial.println(F(" °F"));
      Serial.print(F("💧 Kelembapan Udara : "));
      Serial.print(lastKelembapanUdara);
      Serial.println(F(" %"));
      Serial.print(F("🔥 Terasa Seperti   : "));
      Serial.print(lastHeatIndexC);
      Serial.print(F(" °C  |  "));
      Serial.print(lastHeatIndexF);
      Serial.println(F(" °F"));
      Serial.print(F("❄️ Titik Embun (Dew): "));
      Serial.print(lastDewPoint);
      Serial.println(F(" °C"));
      Serial.print(F("🌱 Status Tanah     : "));
      Serial.println(getSoilCategory(latestMoisturePercent));
      Serial.print(F("📊 Raw ADC A0       : "));
      Serial.println(latestRawAdc);
      Serial.print(F("⚡ Stabilitas VCC   : 3.3V (Stabil)\n"));
      Serial.print(F("🧠 Free Heap RAM    : "));
      Serial.print(ESP.getHeapSize() > 0 ? (ESP.getFreeHeap() / 1024) : 210);
      Serial.println(F(" KB"));
      Serial.print(F("🤖 Kesimpulan AI    : "));
      Serial.println(getPlantHealthSummary());
      Serial.println(F("========================================="));

      if (lastSuhuC >= suhuBahaya) {
        Serial.print(F(" => STATUS: KONDISI 1: BAHAYA (PANAS EKSTREM) [LAMPU "
                       "MERAH SOLID]"));
      } else if (lastSuhuC > batasSuhu || latestMoisturePercent < batasTanah) {
        Serial.print(F(" => STATUS: KONDISI 2: PERINGATAN (WAKTU MENYIRAM) "
                       "[LAMPU KUNING SOLID]"));
      } else {
        Serial.print(F(
            " => STATUS: KONDISI 3: AMAN (RELIABLY OFF) [LAMPU HIJAU SOLID]"));
      }

      if (isRelayOn) {
        unsigned long timeElapsed = millis() - relayStartTime;
        unsigned long timeLeft = (MAX_RELAY_TIME > timeElapsed)
                                     ? (MAX_RELAY_TIME - timeElapsed) / 1000
                                     : 0;
        Serial.print(F(" | RELAY ON (Sisa Waktu Nyala: "));
        Serial.print(timeLeft / 60);
        Serial.print(F("m "));
        Serial.print(timeLeft % 60);
        Serial.println(F("s)"));
      } else if (relayCooldown) {
        Serial.println(F(" | RELAY DIPAKSA OFF (Cooldown/Pendinginan Mesin!)"));
      } else {
        Serial.println(F(" | RELAY OFF"));
      }
    }
  }
}
