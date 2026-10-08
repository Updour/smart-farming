#include "index.h"
#include <esp_idf_version.h>
#include <esp_task_wdt.h> // HARDWARE WATCHDOG TIMER (ANTI-HANG AUTO RESTART)
#define WDT_TIMEOUT_SECONDS 15 // Batas toleransi hang 15 detik sebelum auto-restart fisik
#include <DHT.h>
#include <LittleFS.h>
#include <WebServer.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h> // UNTUK LOCK CHANNEL RADIO PHY ESP32 & DONGKRAK POWER MAKSIMAL
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

#include <Preferences.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

Preferences preferences;
ThreeWire myWire(19, 18, 21); // DAT/IO, CLK/SCLK, RST/CE
RtcDS1302<ThreeWire> Rtc(myWire);

// --- KONFIGURASI LCD 16X2 I2C (SDA: PIN 23, SCL: PIN 22) ---
#define LCD_SDA_PIN 23 // SDA LCD I2C
#define LCD_SCL_PIN 22 // SCL LCD I2C

LiquidCrystal_I2C *lcd = nullptr;
bool isLcdAvailable = false;
unsigned long lastLcdUpdate = 0;
uint8_t lcdScreenPage = 0;
unsigned long lastLcdPageRotate = 0;

// 7 IKON GRAFIS KUSTOM 5x8 PIXEL (CGRAM)
const uint8_t iconTemp[8]   = { 0b00100, 0b01010, 0b01010, 0b01110, 0b01110, 0b11111, 0b11111, 0b01110 }; // 0: 🌡️ Suhu
const uint8_t iconDrop[8]   = { 0b00100, 0b00100, 0b01010, 0b01010, 0b10001, 0b10001, 0b10001, 0b01110 }; // 1: 💧 Kelembapan Udara
const uint8_t iconSoil[8]   = { 0b00100, 0b01110, 0b10101, 0b00100, 0b00100, 0b00100, 0b01110, 0b11111 }; // 2: 🌱 Kelembapan Tanah
const uint8_t iconPump[8]   = { 0b00100, 0b10101, 0b01110, 0b11011, 0b01110, 0b10101, 0b00100, 0b00000 }; // 3: ⚙️ Pompa Air
const uint8_t iconLamp[8]   = { 0b01110, 0b10001, 0b10001, 0b10001, 0b01110, 0b01110, 0b00100, 0b01110 }; // 4: 💡 Lampu Pemanas
const uint8_t iconClock[8]  = { 0b01110, 0b10101, 0b10101, 0b10111, 0b10001, 0b10001, 0b01110, 0b00000 }; // 5: 🕒 Jam RTC
const uint8_t iconSignal[8] = { 0b00001, 0b00001, 0b00101, 0b00101, 0b10101, 0b10101, 0b10101, 0b00000 }; // 6: 📶 Sinyal Radio

// NVS Persistent Configuration Variables
uint8_t cropMode = 0;
String cropName = "Cabai Rawit";
uint16_t cropAge = 14;     // Hari Setelah Tanam (HST)
String cropStage = "vegetatif";
uint8_t cropLeaves = 4;
String cropEnv = "bedengan";
uint16_t cropArea = 100;

uint16_t pumpLph = 1800;
uint16_t pumpWatt = 25;
uint16_t plnTariff = 415;

uint8_t sched1_en = 1;
uint8_t sched1_h = 6;
uint8_t sched1_m = 0;
uint8_t sched1_dur = 15;

uint8_t sched2_en = 1;
uint8_t sched2_h = 17;
uint8_t sched2_m = 0;
uint8_t sched2_dur = 10;

uint8_t lamp_sched_en = 0; // Default 0 (Mati saat mode Auto)
uint8_t lamp_on_h = 18;
uint8_t lamp_on_m = 0;
uint8_t lamp_dur = 12; // dalam jam



// --- KONFIGURASI DHT, RELAY & TRAFFIC LIGHT ---
#define DHTPIN 4
#define DHTTYPE DHT11

// RELAY AKTIVASI (ACTIVE LOW: LOW = ON, HIGH = OFF)
#define RELAY1 26 // Pin 26: Relay Pompa Air (Sanyo)
#define RELAY2 27 // Pin 27: Relay Lampu Penerangan / Pemanas (Grow Light)

// SETELAN ACTIVE LOW (LOW = ON, HIGH = OFF)
#define RELAY_ON_STATE LOW
#define RELAY_OFF_STATE HIGH

// LAMPU INDIKATOR TRAFFIC LIGHT (ACTIVE HIGH: HIGH = ON, LOW = OFF)
#define LED_HIJAU 32  // Pin 32: Hijau (Kondisi Aman)
#define LED_KUNING 33 // Pin 33: Kuning (Peringatan Menyiram)
#define LED_MERAH 25  // Pin 25: Merah (Bahaya Suhu / Hardware Fault)

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
int8_t latestMoisturePercent = -1; // Default -1 (Menunggu sensor, tidak memalsukan 100%)
int8_t esp8266Battery = 0;
uint16_t latestRawAdc = 0;
unsigned long lastRecvTime = 0;
int8_t esp8266Rssi = -99;

// Data Cuaca Lokal (DHT11) - Pure Hardware Telemetry (Zero Dummy Default)
bool isDhtValid = false;
float lastSuhuC = 0.0;
float lastSuhuF = 0.0;
float lastKelembapanUdara = 0.0;
float lastHeatIndexC = 0.0;
float lastHeatIndexF = 0.0;
float lastDewPoint = 0.0;

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
unsigned long waitingSecs = 0; // DETIK BERJALAN MENUNGGU SENSOR
uint8_t waitingPercent = 0; // COUNTER PERSEN MENUNGGU (0% - 100%)

bool isEsp8266Unplugged = false; // FLAG PERINGATAN ESP8266 DICABUT
bool isRelayUnplugged = false;   // FLAG PERINGATAN RELAY PIN 26 DICABUT

// --- FAILSAFE WATCHDOG SENSOR ESP8266 (DISINKRONKAN DENGAN INTERVAL SENDER 1 MENIT) ---
const unsigned long ESP_NOW_TIMEOUT_MS = 300000UL;         // 300 Detik (5 Menit): Toleransi stabil anti-false alarm
const unsigned long WATERING_SENSOR_TIMEOUT_MS = 300000UL; // 300 Detik (5 Menit): Watchdog saat menyiram disesuaikan siklus

// --- TIMER RELAY & PRO COUNTER NVS PERSISTENT ---
unsigned long relayStartTime = 0;
bool isRelayOn = false;
bool relayCooldown = false;
const unsigned long MAX_RELAY_TIME = 5UL * 60UL * 1000UL; // 5 Menit (Batas Maksimal Siram Darurat / Failsafe)

uint32_t pumpCount = 0;       // Total Frekuensi Pompa MENYALA (ON) - Tersimpan Permanen di NVS
uint32_t pumpOffCount = 0;    // Total Frekuensi Pompa MATI (OFF) - Tersimpan Permanen di NVS
unsigned long totalPumpSecs = 0; // Total Detik Aktif Kumulatif - Tersimpan Permanen di NVS
unsigned long lastPumpSecUpdate = 0;
unsigned long lastNvsPumpSecSave = 0;

// --- COUNTER UNTUK LOG DATABASE ---
uint16_t hourlyPumpCount = 0;
uint32_t hourlyPumpSecs = 0;

// --- KENDALI WEB MANUAL ---
bool isManualMode = false;
bool manualRelayState = false;
bool isRtcScheduleActive = false;
uint8_t ledState = 0; // 0=semua mati, 1=merah, 2=kuning, 3=hijau, 4=rtc-cycle

bool isLampOn = false;
bool isLampManualMode = false;
bool manualLampState = false;

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
  if (isStartupWaiting) {
    return "⏳ MENUNGGU SENSOR (" + String(waitingPercent) + "% - " + String(waitingSecs) + "s)";
  }
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
  if (isStartupWaiting) {
    return "⏳ [MEMUAT " + String(waitingPercent) + "%] " + String(waitingSecs) + "s / 75s - Menunggu sinyal ESP8266 (Lampu Kuning Berkedip)";
  }

  if (latestMoisturePercent >= 90) {
    return F("🛑 [INTERLOCK AKAR 90%] Tanah sangat basah (≥90%). Pompa dikunci "
             "OFF otomatis untuk mencegah pembusukan akar tanaman!");
  } else if (latestMoisturePercent >= 0 && latestMoisturePercent < batasTanah) {
    return "🍂 [PERINGATAN TANAH KERING] Kelembapan tanah (" +
           String(latestMoisturePercent) + "% < " + String(batasTanah) +
           "%). Pompa siap menyiram.";
  } else if (lastSuhuC >= suhuBahaya) {
    return "☀️ [SUHU UDARA EKSTREM] Suhu (" + String(lastSuhuC, 1) +
           "°C). Tanah tetap lembab (" + String(latestMoisturePercent) +
           "%). Pompa aman OFF.";
  } else if (lastSuhuC > batasSuhu) {
    return "☀️ [SUHU UDARA PANAS] Suhu (" + String(lastSuhuC, 1) +
           "°C). Tanah lembab optimal (" + String(latestMoisturePercent) +
           "%). Pompa aman OFF.";
  } else {
    return "🌿 [KONDISI IDEAL] Suhu adem (" + String(lastSuhuC, 1) +
           "°C) & kelembapan tanah (" + String(latestMoisturePercent) +
           "%) optimal. Pompa MATI.";
  }
}

// CALLBACK UNIVERSAL ESP-NOW DENGAN UPDATE INSTAN SAAT PAKET TERIMA
void OnDataRecvInternal(const uint8_t *incomingData, int len) {
  if (len >= 4) {
    memcpy(&myData, incomingData, sizeof(myData));
    latestMoisturePercent = myData.persen;
    esp8266Battery = myData.baterai;
    latestRawAdc = myData.rawAdc;
  } else if (len >= 1) {
    // Kompatibel dengan sender versi 1-byte (struct { int8_t persen; })
    latestMoisturePercent = (int8_t)incomingData[0];
    esp8266Battery = 100;
    latestRawAdc = 0;
  } else {
    return;
  }
  newDataReceived = true;
  lastRecvTime = millis();
  isStartupWaiting = false;
  isEsp8266Unplugged = false; // SINYAL HIDUP KEMBALI
  waitingPercent = 100;
  Serial.printf("📡 [ESP-NOW RECV] Paket Masuk (%d byte)! Kelembapan Tanah: %d%%\n", len, latestMoisturePercent);
}

#if defined(ESP_IDF_VERSION_MAJOR) && ESP_IDF_VERSION_MAJOR >= 5
void esp_now_recv_info_t_wrapper(const esp_now_recv_info *info,
                                 const uint8_t *data, int len) {
  if (info && info->rx_ctrl && info->rx_ctrl->rssi != 0)
    esp8266Rssi = info->rx_ctrl->rssi;
  else
    esp8266Rssi = -55;
  OnDataRecvInternal(data, len);
}
#else
void esp_now_recv_info_t_wrapper(const uint8_t *mac, const uint8_t *data,
                                 int len) {
  esp8266Rssi = -55;
  OnDataRecvInternal(data, len);
}
#endif

void writeLogHeader() {
  File file = LittleFS.open("/log.csv", FILE_WRITE);
  if (file) {
    file.println(
        F("Waktu,Suhu_Lokal(C),Kelembapan_Udara(%),Kelembapan_Tanah(%),"
          "Kategori_Tanah,Akumulasi_Pompa_Nyala,Akumulasi_Durasi_Detik,Raw_ADC,"
          "Suhu_Satelit(C),Kondisi_Satelit,Prediksi_Hujan,Status_Lampu"));
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
    String timeStampStr = "-";
    if (Rtc.GetIsRunning() && Rtc.IsDateTimeValid()) {
      RtcDateTime dtNow = Rtc.GetDateTime();
      char timeBuf[32];
      snprintf(timeBuf, sizeof(timeBuf), "%04u-%02u-%02u %02u:%02u:%02u",
        dtNow.Year(), dtNow.Month(), dtNow.Day(),
        dtNow.Hour(), dtNow.Minute(), dtNow.Second());
      timeStampStr = String(timeBuf);
    } else if (satTime != "-" && satTime.length() > 0) {
      timeStampStr = satTime;
    } else {
      unsigned long sec = millis() / 1000;
      char timeBuf[32];
      snprintf(timeBuf, sizeof(timeBuf), "Uptime %02lu:%02lu:%02lu",
        sec / 3600, (sec % 3600) / 60, sec % 60);
      timeStampStr = String(timeBuf);
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
    file.print(hourlyPumpSecs);
    file.print(',');
    file.print(latestRawAdc);
    file.print(',');
    file.print(satTemp);
    file.print(',');
    file.print(satDesc);
    file.print(',');
    file.print(satRainPred);
    file.print(',');
    file.println(isLampOn ? "1" : "0");
    file.close();

    hourlyPumpSecs = 0; // Reset counter durasi pompa untuk siklus jam berikutnya
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
      isDhtValid = true;
      lastSuhuC = suhu;
      lastSuhuF = suhuF;
      lastKelembapanUdara = kelembapan;
      lastHeatIndexC = dht.computeHeatIndex(suhu, kelembapan, false);
      lastHeatIndexF = dht.computeHeatIndex(suhuF, kelembapan, true);

      float a = 17.271, b = 237.7;
      float gamma = (a * suhu / (b + suhu)) + log(kelembapan / 100.0);
      lastDewPoint = (b * gamma) / (a - gamma);
    } else {
      isDhtValid = false;
    }
  }

  // 1. STATUS SINYAL ESP8266 (Watchdog Toleransi 8 Detik Real-Time):
  bool espNowTimeout = (lastRecvTime != 0 && (millis() - lastRecvTime > ESP_NOW_TIMEOUT_MS));

  // 2. STATUS INTEGRITAS HARDWARE RELAY (PIN 26 SELALU OUTPUT STABIL)
  isRelayUnplugged = false;

  if (espNowTimeout) {
    isEsp8266Unplugged = true;
    esp8266Rssi = -99;
    latestMoisturePercent = -1; // Reset data sensor usang agar tidak memicu pompa
    systemErrorMsg = F("⚠️ Sinyal ESP8266 Terputus (>5 Menit)");
    isSystemError = false;
    isStartupWaiting = false;
  } else if (lastRecvTime == 0) {
    // Saat baru boot dan menunggu paket pertama dari ESP8266 (Siklus 1 Menit):
    latestMoisturePercent = -1;
    waitingSecs = (millis() - startupWaitStartTime) / 1000;
    const unsigned long ESTIMASI_TUNGGU_SECS = 75; // Estimasi tunggu transmisi 1-menitan (75s)
    if (waitingSecs < ESTIMASI_TUNGGU_SECS) {
      isStartupWaiting = true;
      isEsp8266Unplugged = false;
      isSystemError = false;
      waitingPercent = (uint8_t)((waitingSecs * 100) / ESTIMASI_TUNGGU_SECS);
      systemErrorMsg = "Menunggu Sinyal Sensor: " + String(waitingPercent) + "% (" + String(waitingSecs) + "s / " + String(ESTIMASI_TUNGGU_SECS) + "s)";
    } else {
      // Melebihi 75 detik tanpa sinyal radio: ESP8266 dinyatakan BELUM AKTIF / MATI
      isStartupWaiting = false;
      isEsp8266Unplugged = true;
      isSystemError = false;
      waitingPercent = 100;
      esp8266Rssi = -99;
      systemErrorMsg = F("⚠️ Sinyal ESP8266 Tidak Terdeteksi (>75s). Cek Baterai/Power Node Sensor!");
    }
  } else {
    isEsp8266Unplugged = false;
    isSystemError = false;
    isStartupWaiting = false;
    waitingPercent = 100;
    waitingSecs = 0;
    systemErrorMsg = "";
  }
}

// ==== API WEB SERVER ====
void handleRoot() {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "0");
  server.send_P(200, "text/html", index_html);
}

void handleData() {
  String bgClass = F("aman");
  String textStatus = F("KONDISI: AMAN (Tanah Lembab & Ideal)");

  if (isStartupWaiting && latestMoisturePercent < 0) {
    bgClass = F("peringatan");
    textStatus = F("⏳ INISIALISASI: Menunggu Sinyal ESP8266...");
  } else if (isEsp8266Unplugged || latestMoisturePercent < 0) {
    bgClass = F("bahaya");
    textStatus = F("🚨 PERINGATAN: SINYAL ESP8266 TERPUTUS!");
  } else if (isRelayUnplugged) {
    bgClass = F("peringatan");
    textStatus = F("🚨 PERINGATAN: RELAY PIN 26 DICABUT!");
  } else if (latestMoisturePercent >= 0 && latestMoisturePercent < batasTanah) {
    // Fokus tanah: Kuning HANYA jika tanah kering butuh siram
    bgClass = F("peringatan");
    textStatus = F("PERINGATAN (Tanah Kering, Waktu Menyiram)");
  } else if (lastSuhuC >= suhuBahaya) {
    // Tanah lembab, tapi suhu ekstrem
    bgClass = F("aman");
    textStatus = F("AMAN (Tanah Lembab, Suhu Ekstrem Panas)");
  } else if (lastSuhuC > batasSuhu) {
    // Tanah lembab, suhu di atas batas normal
    bgClass = F("aman");
    textStatus = F("AMAN (Tanah Lembab, Suhu Udara Panas)");
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
      (lastRecvTime == 0 || millis() - lastRecvTime > ESP_NOW_TIMEOUT_MS) ? 0 : 100;

  float relayLifePercent = 100.0 - ((float)pumpCount / 1000.0);
  if (relayLifePercent < 0)
    relayLifePercent = 0;

  // Perhitungan Presisi Akumulasi Berdasarkan Kalibrasi NVS
  float waterLiters = ((float)totalPumpSecs / 3600.0f) * (float)pumpLph;
  float kWhUsed = ((float)totalPumpSecs / 3600.0f) * ((float)pumpWatt / 1000.0f);
  float costIdr = kWhUsed * (float)plnTariff;

  float tempDiff = 0.0;
  if (satTemp != "-") {
    tempDiff = lastSuhuC - satTemp.toFloat();
  }

  // Perhitungan Presisi VPD Termodinamika (kPa)
  float vpdVal = 0.0;
  if (isDhtValid && lastSuhuC > 0.0 && lastKelembapanUdara > 0.0) {
    float es = 0.61078f * expf((17.27f * lastSuhuC) / (lastSuhuC + 237.3f));
    float ea = es * (lastKelembapanUdara / 100.0f);
    vpdVal = (es > ea) ? (es - ea) : 0.0f;
  }
  unsigned long secSinceRecv = (lastRecvTime > 0) ? (millis() - lastRecvTime) / 1000 : 0;

  // Baca waktu DS1302 & cek validitas chip
  String rtcTimeStr = "-";
  uint8_t rtcValid = 0;
  uint8_t rtcRunning = 0;
  if (Rtc.GetIsRunning()) {
    rtcRunning = 1;
    RtcDateTime dtNow = Rtc.GetDateTime();
    if (Rtc.IsDateTimeValid()) {
      rtcValid = 1;
      char timeBuf[32];
      snprintf(timeBuf, sizeof(timeBuf),
        "%04u-%02u-%02u %02u:%02u:%02u",
        dtNow.Year(), dtNow.Month(), dtNow.Day(),
        dtNow.Hour(), dtNow.Minute(), dtNow.Second());
      rtcTimeStr = String(timeBuf);
    } else {
      rtcTimeStr = "DS1302: Waktu Tidak Valid";
    }
  } else {
    rtcTimeStr = "DS1302: Tidak Berjalan";
  }

  String json;
  json.reserve(3000);
  json = "{";
  if (isDhtValid) {
    json += "\"suhuC\":\"" + String(lastSuhuC, 1) + "\",";
    json += "\"temp\":\"" + String(lastSuhuC, 1) + "\",";
    json += "\"suhuF\":\"" + String(lastSuhuF, 1) + "\",";
    json += "\"hum\":\"" + String(lastKelembapanUdara, 1) + "\",";
    json += "\"heatC\":\"" + String(lastHeatIndexC, 1) + "\",";
    json += "\"heatF\":\"" + String(lastHeatIndexF, 1) + "\",";
    json += "\"dew\":\"" + (isnan(lastDewPoint) ? "--" : String(lastDewPoint, 1)) + "\",";
  } else {
    json += "\"suhuC\":\"--\",";
    json += "\"temp\":\"--\",";
    json += "\"suhuF\":\"--\",";
    json += "\"hum\":\"--\",";
    json += "\"heatC\":\"--\",";
    json += "\"heatF\":\"--\",";
    json += "\"dew\":\"--\",";
  }
  if (latestMoisturePercent < 0) {
    json += "\"soil\":\"--\",";
  } else {
    json += "\"soil\":\"" + String(latestMoisturePercent) + "\",";
  }
  json += "\"isEsp8266Unplugged\":" + String(isEsp8266Unplugged ? 1 : 0) + ",";
  json += "\"isSystemError\":" + String(isSystemError ? 1 : 0) + ",";
  json +=
      "\"soilCategory\":\"" + getSoilCategory(latestMoisturePercent) + "\",";
  json += "\"soilDepletion\":\"" + getSoilDepletionTime() + "\",";
  json += "\"plantSummary\":\"" + getPlantHealthSummary() + "\",";
  json += "\"rawAdc\":\"" + String(latestRawAdc) + "\",";
  json += "\"nodeBat\":\"" + String(esp8266Battery) + "\",";
  json += "\"battery\":" + String(esp8266Battery) + ",";
  json += "\"statusColor\":\"" + bgClass + "\",";
  json += "\"statusText\":\"" + textStatus + "\",";
  json += "\"errorMsg\":\"" + systemErrorMsg + "\",";
  json += "\"isWaiting\":" + String((isStartupWaiting && latestMoisturePercent < 0) ? 1 : 0) + ",";
  json += "\"waitingSecs\":" + String(waitingSecs) + ",";
  json += "\"waitingPercent\":" + String(waitingPercent) + ",";
  json += "\"rtcTime\":\"" + rtcTimeStr + "\",";
  json += "\"rtcValid\":" + String(rtcValid) + ",";
  json += "\"rtcRunning\":" + String(rtcRunning) + ",";
  json += "\"satTemp\":\"" + satTemp + "\",";
  json += "\"satDesc\":\"" + satDesc + "\",";
  json += "\"satRainPred\":\"" + satRainPred + "\",";
  json += "\"satTime\":\"" + satTime + "\",";
  json += "\"tempDiff\":\"" + String(tempDiff, 1) + "\",";
  json += "\"vpd\":\"" + String(vpdVal, 2) + "\",";
  json += "\"secSinceRecv\":" + String(secSinceRecv) + ",";
  json += "\"isManual\":" + String(isManualMode ? 1 : 0) + ",";
  json += "\"relayOn\":" + String(isRelayOn ? 1 : 0) + ",";
  json += "\"lampOn\":" + String(isLampOn ? 1 : 0) + ",";
  json += "\"lampManual\":" + String(isLampManualMode ? 1 : 0) + ",";
  json += "\"l_en\":" + String(lamp_sched_en) + ",";
  json += "\"l_h\":" + String(lamp_on_h) + ",";
  json += "\"l_m\":" + String(lamp_on_m) + ",";
  json += "\"l_dur\":" + String(lamp_dur) + ",";
  json += "\"sched1_en\":" + String(sched1_en) + ",";
  json += "\"sched1_h\":" + String(sched1_h) + ",";
  json += "\"sched1_m\":" + String(sched1_m) + ",";
  json += "\"sched1_dur\":" + String(sched1_dur) + ",";
  json += "\"sched2_en\":" + String(sched2_en) + ",";
  json += "\"sched2_h\":" + String(sched2_h) + ",";
  json += "\"sched2_m\":" + String(sched2_m) + ",";
  json += "\"sched2_dur\":" + String(sched2_dur) + ",";
  json += "\"cropMode\":" + String(cropMode) + ",";
  json += "\"cropName\":\"" + cropName + "\",";
  json += "\"cropAge\":" + String(cropAge) + ",";
  json += "\"cropStage\":\"" + cropStage + "\",";
  json += "\"cropLeaves\":" + String(cropLeaves) + ",";
  json += "\"cropEnv\":\"" + cropEnv + "\",";
  json += "\"cropArea\":" + String(cropArea) + ",";
  json += "\"batasTanah\":" + String(batasTanah) + ",";
  json += "\"batasSuhu\":\"" + String(batasSuhu, 1) + "\",";
  json += "\"pumpLph\":" + String(pumpLph) + ",";
  json += "\"pumpWatt\":" + String(pumpWatt) + ",";
  json += "\"plnTariff\":" + String(plnTariff) + ",";
  json += "\"rtcSchedule\":" + String(isRtcScheduleActive ? 1 : 0) + ",";
  json += "\"ledState\":" + String(ledState) + ",";
  json += "\"cooldown\":" + String(relayCooldown ? 1 : 0) + ",";

  json += "\"hourlyCount\":" + String(hourlyPumpCount) + ",";
  json += "\"hourlySecs\":" + String(hourlyPumpSecs) + ",";
  json += "\"uptime\":\"" + getUptime() + "\",";
  json += "\"freeHeap\":" +
          String(ESP.getHeapSize() > 0 ? (ESP.getFreeHeap() / 1024) : 210) +
          ",";
  json += "\"pumpCount\":" + String(pumpCount) + ",";
  json += "\"pumpOffCount\":" + String(pumpOffCount) + ",";
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

  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Access-Control-Allow-Origin", "*");
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
  pumpOffCount = 0;
  totalPumpSecs = 0;
  hourlyPumpCount = 0;
  hourlyPumpSecs = 0;
  preferences.putUInt("pumpCount", 0);
  preferences.putUInt("pumpOffCount", 0);
  preferences.putULong("pumpSecs", 0);
  Serial.println(F("💾 [LITTLEFS & NVS] Berkas Log & Counter Akumulasi Pompa Berhasil Direset Total!"));
  server.send(200, "text/plain", F("Log Storage & Counter Pompa Berhasil Direset Total!"));
}

void handleResetPumpStats() {
  pumpCount = 0;
  pumpOffCount = 0;
  totalPumpSecs = 0;
  hourlyPumpCount = 0;
  hourlyPumpSecs = 0;
  preferences.putUInt("pumpCount", 0);
  preferences.putUInt("pumpOffCount", 0);
  preferences.putULong("pumpSecs", 0);
  Serial.println(F("\n💾 [NVS PUMP STATS] Seluruh Statistik Akumulasi Pompa Berhasil Direset ke 0!\n"));
  server.send(200, "application/json", F("{\"status\":\"ok\",\"message\":\"Statistik pompa direset ke 0\"}"));
}

void handleSetThreshold() {
  if (server.hasArg("soil") && server.hasArg("temp")) {
    batasTanah = server.arg("soil").toInt();
    batasSuhu = server.arg("temp").toFloat();
    preferences.putInt("batasTanah", batasTanah);
    preferences.putFloat("batasSuhu", batasSuhu);
    Serial.printf("💾 [NVS PREFERENCES] Ambang Batas Berhasil Disimpan: Tanah < %d%% | Suhu > %.1f°C\n", batasTanah, batasSuhu);
    server.send(200, "text/plain", "Threshold Berhasil Disimpan di NVS!");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}


// ================= HANDLER GROW LIGHT =================
void handleSetLampMode() {
  if (server.hasArg("m")) {
    String m = server.arg("m");
    isLampManualMode = (m == "manual");
    if (!isLampManualMode) {
      updateLampState(); // Sinkronkan langsung ke status Auto
    }
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Missing arg m");
  }
}

void handleToggleLamp() {
  if (!isLampManualMode) {
    server.send(403, "text/plain", F("Ditolak: Mode Lampu sedang AUTO. Ubah ke Mode Manual terlebih dahulu!"));
    return;
  }
  if (server.hasArg("s")) {
    String s = server.arg("s");
    manualLampState = (s == "on");
  }
  digitalWrite(RELAY2, manualLampState ? LOW : HIGH); // Active Low: LOW = ON, HIGH = OFF
  isLampOn = manualLampState;
  Serial.print(F("💡 [WEB MANUAL] LAMPU PIN 27 -> "));
  Serial.println(manualLampState ? F("LOW (ON)") : F("HIGH (OFF)"));
  server.send(200, "text/plain", "OK");
}

void handleSetLampSchedule() {
  if (server.hasArg("en") && server.hasArg("h") && server.hasArg("m") && server.hasArg("dur")) {
    lamp_sched_en = server.arg("en").toInt();
    lamp_on_h = server.arg("h").toInt();
    lamp_on_m = server.arg("m").toInt();
    lamp_dur = server.arg("dur").toInt();
    
    preferences.putUChar("l_en", lamp_sched_en);
    preferences.putUChar("l_h", lamp_on_h);
    preferences.putUChar("l_m", lamp_on_m);
    preferences.putUChar("l_dur", lamp_dur);
    
    if (!isLampManualMode) {
      updateLampState();
    }
    Serial.println(F("\n💡 -----------------------------------------------------------"));
    Serial.println(F("💡 [NVS LAMPU] Jadwal Timer Lampu Grow Light Berhasil Disimpan!"));
    Serial.printf("💡 Jam Mulai : %02d:%02d WIB | Durasi: %d Jam | Status: %s\n",
                  lamp_on_h, lamp_on_m, lamp_dur, lamp_sched_en ? "AKTIF" : "NONAKTIF");
    Serial.println(F("💡 -----------------------------------------------------------\n"));
    server.send(200, "application/json", "{\"status\":\"ok\",\"message\":\"Jadwal Lampu Tersimpan\"}");
  } else {
    server.send(400, "text/plain", "Missing args");
  }
}

void updateLampState() {
  if (isLampManualMode) {
    digitalWrite(RELAY2, manualLampState ? LOW : HIGH);
    isLampOn = manualLampState;
  } else {
    // Mode Otomatis RTC
    if (lamp_sched_en && Rtc.GetIsRunning() && Rtc.IsDateTimeValid()) {
      RtcDateTime now = Rtc.GetDateTime();
      int current_hour = now.Hour();
      int current_min = now.Minute();
      
      int start_mins = (lamp_on_h * 60) + lamp_on_m;
      int end_mins = start_mins + (lamp_dur * 60);
      int now_mins = (current_hour * 60) + current_min;
      
      bool shouldBeOn = false;
      
      // Handle cross-midnight schedule (e.g. 18:00 to 06:00 = 12h duration)
      if (end_mins >= 1440) {
         int end_mins_next_day = end_mins - 1440;
         if (now_mins >= start_mins || now_mins < end_mins_next_day) {
           shouldBeOn = true;
         }
      } else {
         if (now_mins >= start_mins && now_mins < end_mins) {
           shouldBeOn = true;
         }
      }
      
      digitalWrite(RELAY2, shouldBeOn ? LOW : HIGH);
      isLampOn = shouldBeOn;
    } else {
      // Auto default: Lampu MATI (OFF)
      digitalWrite(RELAY2, HIGH); // OFF (Active Low)
      isLampOn = false;
    }
  }
}

void handleSetMode() {
  if (server.hasArg("m")) {
    isManualMode = (server.arg("m") == "manual");
    if (!isManualMode) {
      server.send(200, "text/plain", F("OK"));
      delay(15);
      updateRelayState(); // Sinkronkan kembali ke sensor Auto
      return;
    } else {
      // Saat pindah ke mode Manual, pastikan relay OFF dulu demi keselamatan
      manualRelayState = false;
      server.send(200, "text/plain", F("OK"));
      delay(15);
      digitalWrite(RELAY1, HIGH);
      isRelayOn = false;
      return;
    }
  }
  server.send(200, "text/plain", F("OK"));
}

void handleToggleRelay() {
  if (!isManualMode) {
    server.send(403, "text/plain", F("Ditolak: Mode Pompa sedang AUTO. Tombol manual terkunci!"));
    return;
  }
  // Mode manual adalah otoritas penuh petani: siram manual wajib nyala langsung
  if (server.hasArg("s")) {
    manualRelayState = (server.arg("s") == "on");
  }

  isRelayOn = manualRelayState;
  if (manualRelayState) {
    pumpCount++;
    hourlyPumpCount++;
    preferences.putUInt("pumpCount", pumpCount);
    relayStartTime = millis();
    Serial.printf("⚡ [WEB MANUAL] POMPA PIN 26 -> LOW (ON) | Siklus Nyala Ke: %u\n", pumpCount);
  } else {
    pumpOffCount++;
    preferences.putUInt("pumpOffCount", pumpOffCount);
    preferences.putULong("pumpSecs", totalPumpSecs);
    Serial.printf("⚡ [WEB MANUAL] POMPA PIN 26 -> HIGH (OFF) | Siklus Mati Ke: %u | Total: %lu Detik\n", pumpOffCount, totalPumpSecs);
  }

  server.send(200, "text/plain", F("OK"));
  delay(15);
  digitalWrite(RELAY1, manualRelayState ? LOW : HIGH); // LOW = ON, HIGH = OFF
}

void handleReboot() {
  server.send(200, "text/plain", F("Rebooting ESP32 System..."));
  delay(1000);
  ESP.restart();
}

// ==== UTAMA: LOGIKA KENDALI POMPA AIR (RELAY PIN 26) ====
void updateRelayState() {
  bool butuhON = false;
  isRtcScheduleActive = false;

  // AUTO SCHEDULER RTC LOGIC (Kalkulasi Menit Presisi)
  if (!isManualMode && Rtc.GetIsRunning() && Rtc.IsDateTimeValid()) {
    RtcDateTime now = Rtc.GetDateTime();
    uint16_t curMin = now.Hour() * 60 + now.Minute();
    
    // Check Slot 1
    uint16_t start1 = sched1_h * 60 + sched1_m;
    if (sched1_en && curMin >= start1 && curMin < (start1 + sched1_dur)) {
       isRtcScheduleActive = true;
    }
    // Check Slot 2
    uint16_t start2 = sched2_h * 60 + sched2_m;
    if (sched2_en && curMin >= start2 && curMin < (start2 + sched2_dur)) {
       isRtcScheduleActive = true;
    }
  }

  // JIKA MODE MANUAL: IKUTI PERINTAH SAKLAR MANUAL USER SECARA PENUH (WAJIB NYALA!)
  if (isManualMode) {
    if (manualRelayState) {
      if (!isRelayOn) {
        isRelayOn = true;
        pumpCount++;
        hourlyPumpCount++;
        preferences.putUInt("pumpCount", pumpCount);
        relayStartTime = millis();
        Serial.println(F("\n💧 =============================================================="));
        Serial.println(F("💧 [NOTIFIKASI UTAMA: POMPA SEDANG MENYIRAM LAHAN (MODE MANUAL)!]"));
        Serial.printf("💧 Kelembapan Tanah : %d%% | Status Pin 26: LOW (RELAY AKTIF / ON)\n", latestMoisturePercent);
      }
      // FAILSAFE PROTEKSI 5 MENIT PADA MODE MANUAL JIKA KONEKSI TERPUTUS / LUPA MEMATIKAN
      if (millis() - relayStartTime >= MAX_RELAY_TIME) {
        manualRelayState = false;
        isRelayOn = false;
        pumpOffCount++;
        preferences.putUInt("pumpOffCount", pumpOffCount);
        preferences.putULong("pumpSecs", totalPumpSecs);
        digitalWrite(RELAY1, HIGH); // OFF
        Serial.println(F("\n🚨 =============================================================="));
        Serial.println(F("🚨 [FAILSAFE MANUAL] 5 MENIT NONSTOP TERCAPAI -> POMPA OTOMATIS MATI!"));
        Serial.println(F("🚨 Mencegah banjir lahan & melindungi pompa jika koneksi terputus!"));
        Serial.println(F("🚨 ==============================================================\n"));
      } else {
        digitalWrite(RELAY1, LOW); // ON
      }
    } else {
      if (isRelayOn) {
        pumpOffCount++;
        preferences.putUInt("pumpOffCount", pumpOffCount);
        preferences.putULong("pumpSecs", totalPumpSecs);
        unsigned long runDuration = (millis() - relayStartTime) / 1000;
        float waterLitersEst = ((float)runDuration / 60.0) * ((float)pumpLph / 60.0);
        Serial.println(F("\n✅ =============================================================="));
        Serial.println(F("✅ [NOTIFIKASI: PENYIRAMAN MANUAL SELESAI / POMPA DIMATIKAN]"));
        Serial.printf("✅ Durasi Siram     : %lu Detik (~%.1f Liter Air Terdistribusi)\n", runDuration, waterLitersEst);
        Serial.printf("✅ Kelembapan Terkini: %d%% | Status Pin 26: HIGH (STANDBY / OFF)\n", latestMoisturePercent);
        Serial.println(F("✅ ==============================================================\n"));
      }
      isRelayOn = false;
      digitalWrite(RELAY1, HIGH); // OFF
    }
    return;
  }

  // =========================================================================
  // 🚨 FAILSAFE DARURAT 1: SENSOR TERPUTUS SAAT SEDANG MENYIRAM (MODE AUTO)
  // =========================================================================
  // Jika pompa sedang ON di mode AUTO dan transmisi ESP8266 terputus / berhenti (>8s),
  // pompa WAJIB LANGSUNG DIMATIKAN SEKETIKA demi mencegah banjir & kerusakan akar!
  if (isRelayOn) {
    bool isSensorLostWhileWatering = (lastRecvTime == 0 ||
                                     (millis() - lastRecvTime > WATERING_SENSOR_TIMEOUT_MS) ||
                                     isEsp8266Unplugged || isSystemError ||
                                     latestMoisturePercent < 0);
    if (isSensorLostWhileWatering) {
      isRelayOn = false;
      relayCooldown = false;
      pumpOffCount++;
      preferences.putUInt("pumpOffCount", pumpOffCount);
      preferences.putULong("pumpSecs", totalPumpSecs);
      digitalWrite(RELAY1, HIGH); // SAKLAR POMPA PIN 26 MATI SEKETIKA (ACTIVE LOW -> HIGH)
      isEsp8266Unplugged = true;
      latestMoisturePercent = -1;  // Invalidate data sensor usang
      esp8266Rssi = -99;
      systemErrorMsg = F("🚨 FAILSAFE: Sinyal ESP8266 Terputus Saat Menyiram! Pompa Dipaksa MATI!");
      Serial.println(F("\n🚨 ================= FAILSAFE DARURAT TERPICU ================="));
      Serial.println(F("🚨 [FAILSAFE AKTIF] SENSOR ESP8266 TERPUTUS SAAT SEDANG MENYIRAM!"));
      Serial.println(F("🚨 POMPA (PIN 26) DIMATIKAN SEKETIKA DEMI MENCEGAH BANJIR LAHAN!"));
      Serial.println(F("===============================================================\n"));
      return;
    }
  }

  // =========================================================================
  // 🚨 FAILSAFE 2: SENSOR TERPUTUS / STANDBY / BOOT / ERROR (POMPA TERKUNCI MATI)
  // =========================================================================
  // Pompa TIDAK BOLEH PERNAH NYALA jika sinyal sensor tidak ada atau belum terhubung!
  if (isSystemError || isEsp8266Unplugged || isRelayUnplugged ||
      latestMoisturePercent < 0 || isStartupWaiting || lastRecvTime == 0 ||
      (millis() - lastRecvTime > ESP_NOW_TIMEOUT_MS)) {
    isRelayOn = false;
    relayCooldown = false;
    digitalWrite(RELAY1, HIGH); // SAKLAR POMPA PIN 26 KUNCI MATI (HIGH / 3.3V)
    return;
  }

  // PROTEKSI KEJENUHAN AIR: Jika tanah sudah basah (>= 80%), JANGAN SIRAM demi cegah busuk akar!
  if (latestMoisturePercent >= 80) {
    butuhON = false;
    relayCooldown = false;
  } else if (isRtcScheduleActive) {
    // Jadwal RTC aktif dan tanah belum basah jenuh (< 80%) -> Siram!
    butuhON = true;
  } else {
    // KENDALI AUTO SENSOR: HANYA MENYIRAM JIKA TANAH KERING (< batasTanah)
    // Suhu panas terik TIDAK BOLEH memaksa menyiram jika tanah masih cukup lembab!
    if (latestMoisturePercent < batasTanah) {
      butuhON = true;
    } else {
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
      preferences.putUInt("pumpCount", pumpCount);
      relayStartTime = millis();
      digitalWrite(RELAY1, LOW);
      Serial.println(F("\n💧 =============================================================="));
      if (isRtcScheduleActive) {
        Serial.println(F("💧 [NOTIFIKASI UTAMA: POMPA SEDANG MENYIRAM LAHAN (JADWAL RTC)!]"));
      } else {
        Serial.println(F("💧 [NOTIFIKASI UTAMA: POMPA SEDANG MENYIRAM LAHAN (OTOMATIS TANAH)!]"));
      }
      Serial.printf("💧 Pemicu Siram     : Tanah Kering (%d%% < Batas %d%%)\n", latestMoisturePercent, batasTanah);
      Serial.printf("💧 Suhu Udara       : %.1f°C | Target Pemulihan: >= 80%%\n", lastSuhuC);
      Serial.println(F("💧 Status Relai     : PIN 26 AKTIF (LOW / ALIRAN AIR MENYALA)"));
      Serial.println(F("💧 ==============================================================\n"));
    } else {
      if (millis() - relayStartTime >= MAX_RELAY_TIME) {
        relayCooldown = true;
        isRelayOn = false;
        pumpOffCount++;
        preferences.putUInt("pumpOffCount", pumpOffCount);
        preferences.putULong("pumpSecs", totalPumpSecs);
        digitalWrite(RELAY1, HIGH);
        Serial.println(F("\n🚨 [RELAY] 5 MENIT NONSTOP TERCAPAI -> AUTO COOLDOWN LOCK!\n"));
      } else {
        digitalWrite(RELAY1, LOW);
      }
    }
  } else {
    if (isRelayOn) {
      pumpOffCount++;
      preferences.putUInt("pumpOffCount", pumpOffCount);
      preferences.putULong("pumpSecs", totalPumpSecs);
      unsigned long runDuration = (millis() - relayStartTime) / 1000;
      float waterLitersEst = ((float)runDuration / 60.0) * ((float)pumpLph / 60.0);
      Serial.println(F("\n✅ =============================================================="));
      Serial.println(F("✅ [NOTIFIKASI: PENYIRAMAN OTOMATIS SELESAI / POMPA DIMATIKAN]"));
      Serial.printf("✅ Kondisi Tanah    : Tercukupi (%d%% >= Batas Ideal)\n", latestMoisturePercent);
      Serial.printf("✅ Durasi Siram     : %lu Detik (~%.1f Liter Air Terdistribusi)\n", runDuration, waterLitersEst);
      Serial.println(F("✅ Status Relai     : PIN 26 STANDBY (HIGH / POMPA OFF)"));
      Serial.println(F("✅ ==============================================================\n"));
    }
    isRelayOn = false;
    digitalWrite(RELAY1, HIGH); // PIN 26 KUNCI MATI (HIGH / 3.3V)
  }
}

// ledState: 0=semua mati, 1=merah, 2=kuning, 3=hijau, 4=rtc-cycle
// Nilai ini mencerminkan sinyal GPIO AKTUAL yang diperintahkan firmware.

// ==== LOGIKA SEJATI INDIKATOR LAMPU TRAFFIC LIGHT (ACTIVE HIGH / COMMON GND)
// ====
void updateLEDState() {
  // 1. HARDWARE ERROR / CRITICAL FAULT / SENSOR TERPUTUS -> LAMPU MERAH BERKEDIP (250ms)!
  if (isSystemError || isEsp8266Unplugged) {
    digitalWrite(LED_HIJAU, LOW);  // OFF
    digitalWrite(LED_KUNING, LOW); // OFF
    digitalWrite(LED_MERAH, ((millis() / 250) % 2) ? HIGH : LOW);
    ledState = 1; // 1 = merah
    return;
  }

  // 2. STATUS UTAMA: POMPA SEDANG MENYIRAM LAHAN (3 LAMPU TRAFFIC LIGHT BERKEDIP AKTIF)!
  // Saat pompa sedang menyiram (isRelayOn == true), lampu Merah, Kuning, dan Hijau semuanya ikut berkedip dinamis bergantian (160ms cascade).
  if (isRelayOn) {
    uint8_t step = (millis() / 160) % 3;
    digitalWrite(LED_MERAH, step == 0 ? HIGH : LOW);
    digitalWrite(LED_KUNING, step == 1 ? HIGH : LOW);
    digitalWrite(LED_HIJAU, step == 2 ? HIGH : LOW);
    ledState = 5; // 5 = menyiram lahan (3 lampu aktif berkedip)
    return;
  }

  // 3. AWAL BOOT / MENUNGGU KONEKSI SENSOR KEBUN -> LAMPU KUNING BERKEDIP (500ms)!
  if (isStartupWaiting) {
    digitalWrite(LED_HIJAU, LOW);
    digitalWrite(LED_MERAH, LOW);
    digitalWrite(LED_KUNING, ((millis() / 500) % 2) ? HIGH : LOW);
    ledState = 2; // 2 = kuning
    return;
  }

  // 4. STATUS TANAH STANDBY:
  // JIKA TANAH KERING (latestMoisturePercent < batasTanah) -> LAMPU KUNING SOLID ON
  if (latestMoisturePercent >= 0 && latestMoisturePercent < batasTanah) {
    digitalWrite(LED_HIJAU, LOW);   // OFF
    digitalWrite(LED_MERAH, LOW);   // OFF
    digitalWrite(LED_KUNING, HIGH); // HIGH = NYALA SOLID (Hanya jika tanah kering)
    ledState = 2; // 2 = kuning
  }
  // JIKA TANAH LEMBAB (latestMoisturePercent >= batasTanah) -> LAMPU MUTLAK TETAP HIJAU SOLID!
  // Walaupun suhu udara di atas 30°C (panas), lampu TETAP HIJAU SOLID karena kelembapan tanah aman!
  else {
    digitalWrite(LED_KUNING, LOW); // OFF
    digitalWrite(LED_MERAH, LOW);  // OFF
    digitalWrite(LED_HIJAU, HIGH); // HIGH = TETAP HIJAU SOLID
    ledState = 3; // 3 = hijau
  }
}

unsigned long lastSerialPrintTime = 0;


void handleSetRtc() {
  if (server.hasArg("y") && server.hasArg("h")) {
    int y = server.arg("y").toInt();
    int m = server.arg("m").toInt();
    int d = server.arg("d").toInt();
    int h = server.arg("h").toInt();
    int min = server.arg("min").toInt();
    int s = server.arg("s").toInt();
    
    // Pastikan register write protect DS1302 dinonaktifkan
    if (Rtc.GetIsWriteProtected()) {
      Rtc.SetIsWriteProtected(false);
    }
    if (!Rtc.GetIsRunning()) {
      Rtc.SetIsRunning(true);
    }
    
    RtcDateTime dt(y, m, d, h, min, s);
    Rtc.SetDateTime(dt);
    
    char buf[32];
    snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d", y, m, d, h, min, s);
    Serial.println(F("\n⏰ -----------------------------------------------------------"));
    Serial.printf("⏰ [RTC HARDWARE SYNC] Jam DS1302 Berhasil Disinkronkan!\n");
    Serial.printf("⏰ Waktu Sekarang: %s WIB (Tersimpan di RTC)\n", buf);
    Serial.println(F("⏰ -----------------------------------------------------------\n"));
    
    server.send(200, "application/json", "{\"status\":\"ok\",\"rtcTime\":\"" + String(buf) + "\"}");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

void handleSetSchedule() {
  if (server.hasArg("slot") && server.hasArg("en")) {
    int slot = server.arg("slot").toInt();
    if (slot == 1) {
      sched1_en = server.arg("en").toInt();
      sched1_h = server.arg("h").toInt();
      sched1_m = server.arg("m").toInt();
      sched1_dur = server.arg("dur").toInt();
      preferences.putUChar("s1_en", sched1_en);
      preferences.putUChar("s1_h", sched1_h);
      preferences.putUChar("s1_m", sched1_m);
      preferences.putUChar("s1_dur", sched1_dur);
      Serial.println(F("\n💾 -----------------------------------------------------------"));
      Serial.println(F("💾 [NVS UPDATE] Jadwal Penyiraman Slot 1 (Pagi) Berhasil Disimpan!"));
      Serial.printf("💾 Jam Siram: %02d:%02d WIB | Durasi: %d Menit | Status: %s\n",
                    sched1_h, sched1_m, sched1_dur, sched1_en ? "AKTIF" : "NONAKTIF");
      Serial.println(F("💾 -----------------------------------------------------------\n"));
    } else if (slot == 2) {
      sched2_en = server.arg("en").toInt();
      sched2_h = server.arg("h").toInt();
      sched2_m = server.arg("m").toInt();
      sched2_dur = server.arg("dur").toInt();
      preferences.putUChar("s2_en", sched2_en);
      preferences.putUChar("s2_h", sched2_h);
      preferences.putUChar("s2_m", sched2_m);
      preferences.putUChar("s2_dur", sched2_dur);
      Serial.println(F("\n💾 -----------------------------------------------------------"));
      Serial.println(F("💾 [NVS UPDATE] Jadwal Penyiraman Slot 2 (Sore) Berhasil Disimpan!"));
      Serial.printf("💾 Jam Siram: %02d:%02d WIB | Durasi: %d Menit | Status: %s\n",
                    sched2_h, sched2_m, sched2_dur, sched2_en ? "AKTIF" : "NONAKTIF");
      Serial.println(F("💾 -----------------------------------------------------------\n"));
    }
    server.send(200, "application/json", "{\"status\":\"ok\"}");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

void handleSetPumpConfig() {
  if (server.hasArg("lph")) {
    pumpLph = server.arg("lph").toInt();
    pumpWatt = server.arg("watt").toInt();
    plnTariff = server.arg("tariff").toInt();
    preferences.putUShort("pumpLph", pumpLph);
    preferences.putUShort("pumpWatt", pumpWatt);
    preferences.putUShort("plnTariff", plnTariff);
    Serial.println(F("\n⚙️ -----------------------------------------------------------"));
    Serial.println(F("⚙️ [NVS UPDATE] Spesifikasi Kalibrasi Pompa & Listrik Disimpan!"));
    Serial.printf("⚙️ Debit Pompa: %d L/Jam | Daya: %d Watt | Tarif PLN: Rp %d/kWh\n", pumpLph, pumpWatt, plnTariff);
    Serial.println(F("⚙️ -----------------------------------------------------------\n"));
    server.send(200, "application/json", "{\"status\":\"ok\"}");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

void handleSetCropProfile() {
  if (server.hasArg("name") || server.hasArg("mode") || server.hasArg("age")) {
    if (server.hasArg("mode")) {
      cropMode = server.arg("mode").toInt();
      preferences.putUChar("cropMode", cropMode);
    }
    if (server.hasArg("name") && server.arg("name").length() > 0) {
      cropName = server.arg("name");
      preferences.putString("cropName", cropName);
    }
    if (server.hasArg("age")) {
      cropAge = server.arg("age").toInt();
      preferences.putUShort("cropAge", cropAge);
    }
    if (server.hasArg("stage")) {
      cropStage = server.arg("stage");
      preferences.putString("cropStage", cropStage);
    }
    if (server.hasArg("leaves")) {
      cropLeaves = server.arg("leaves").toInt();
      preferences.putUChar("cropLeaves", cropLeaves);
    }
    if (server.hasArg("env")) {
      cropEnv = server.arg("env");
      preferences.putString("cropEnv", cropEnv);
    }
    if (server.hasArg("area")) {
      cropArea = server.arg("area").toInt();
      preferences.putUShort("cropArea", cropArea);
    }
    Serial.println(F("\n🌾 -----------------------------------------------------------"));
    Serial.println(F("🌾 [NVS UPDATE] Profil Budidaya & HST Tersimpan Permanen di ESP32!"));
    Serial.printf("🌾 Komoditas: %s | Usia: %d HST | Fase: %s | Daun: %d | Luas: %dm2\n",
                  cropName.c_str(), cropAge, cropStage.c_str(), cropLeaves, cropArea);
    Serial.println(F("🌾 -----------------------------------------------------------\n"));
    server.send(200, "application/json", "{\"status\":\"ok\"}");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

// ================================================================
// JURNAL PERKEMBANGAN TANAMAN (HST) TERSIMPAN DI LITTLEFS ESP32
// ================================================================
void handleGetCropHistory() {
  if (LittleFS.exists("/crop_hist.json")) {
    File f = LittleFS.open("/crop_hist.json", FILE_READ);
    if (f) {
      server.streamFile(f, "application/json");
      f.close();
      return;
    }
  }
  server.send(200, "application/json", "[]");
}

void handleSaveCropHistory() {
  if (server.hasArg("plain") && server.arg("plain").length() > 0) {
    File f = LittleFS.open("/crop_hist.json", FILE_WRITE);
    if (f) {
      f.print(server.arg("plain"));
      f.close();
      server.send(200, "application/json", "{\"status\":\"ok\",\"msg\":\"Jurnal HST tersimpan di LittleFS ESP32\"}");
      Serial.println(F("🌾 [LITTLEFS] Jurnal Riwayat HST berhasil disimpan permanen di ESP32!"));
      return;
    }
  }
  server.send(400, "text/plain", "Data Kosong");
}

void handleResetCropHistory() {
  File f = LittleFS.open("/crop_hist.json", FILE_WRITE);
  if (f) {
    f.print("[]");
    f.close();
  }
  cropAge = 1;
  cropStage = "semai";
  preferences.putUShort("cropAge", 1);
  preferences.putString("cropStage", "semai");
  server.send(200, "application/json", "{\"status\":\"ok\"}");
  Serial.println(F("🌾 [LITTLEFS] Jurnal Riwayat HST direset ke awal (HST 1)."));
}

void handleDownloadCropHistory() {
  if (LittleFS.exists("/crop_hist.json")) {
    File f = LittleFS.open("/crop_hist.json", FILE_READ);
    if (f) {
      server.sendHeader("Content-Disposition", "attachment; filename=\"smartfarm_riwayat_hst.json\"");
      server.streamFile(f, "application/json");
      f.close();
      return;
    }
  }
  server.send(404, "text/plain", "Belum ada riwayat HST");
}

// ================================================================
// INISIALISASI LCD 16X2 DENGAN AUTO-SCAN I2C (0x27 / 0x3F)
// ================================================================
void initLcd16x2() {
  Wire.begin(LCD_SDA_PIN, LCD_SCL_PIN);
  
  uint8_t lcdAddr = 0;
  Wire.beginTransmission(0x27);
  if (Wire.endTransmission() == 0) {
    lcdAddr = 0x27;
  } else {
    Wire.beginTransmission(0x3F);
    if (Wire.endTransmission() == 0) {
      lcdAddr = 0x3F;
    }
  }

  if (lcdAddr != 0) {
    lcd = new LiquidCrystal_I2C(lcdAddr, 16, 2);
    lcd->init();
    lcd->backlight();
    
    // Daftarkan 7 custom icon ke CGRAM
    lcd->createChar(0, (uint8_t*)iconTemp);
    lcd->createChar(1, (uint8_t*)iconDrop);
    lcd->createChar(2, (uint8_t*)iconSoil);
    lcd->createChar(3, (uint8_t*)iconPump);
    lcd->createChar(4, (uint8_t*)iconLamp);
    lcd->createChar(5, (uint8_t*)iconClock);
    lcd->createChar(6, (uint8_t*)iconSignal);

    // Splash Screen Booting 2 Detik
    lcd->setCursor(0, 0);
    lcd->print(F("SMART FARM IOT  "));
    lcd->setCursor(0, 1);
    lcd->print(F("Booting ESP32..."));
    isLcdAvailable = true;
    Serial.printf("📺 [LCD 16x2] Berhasil Terdeteksi & Aktif pada Alamat I2C 0x%02X!\n", lcdAddr);
  } else {
    isLcdAvailable = false;
    Serial.println(F("ℹ️ [LCD 16x2] Modul I2C tidak terdeteksi di Pin SDA:23 / SCL:22. Sistem berjalan tanpa LCD."));
  }
}

// ================================================================
// REFRESH LAYAR LCD 16X2 (100% SERBA IKON & NON-BLOCKING MILLIS)
// ================================================================
void updateLcdDisplay() {
  if (!isLcdAvailable || lcd == nullptr) return;

  unsigned long now = millis();
  if (now - lastLcdUpdate < 1000) return; // Refresh tiap 1 detik
  lastLcdUpdate = now;

  // 1. PRIORITAS UTAMA: JIKA SEDANG MENYIRAM (KUNCI LAYAR, TIDAK PINDAH SAMPAI SELESAI)
  if (isRelayOn) {
    lastLcdPageRotate = now; // Kunci rotasi halaman selama menyiram
    unsigned int runSecs = (relayStartTime > 0 && now >= relayStartTime) ? ((now - relayStartTime) / 1000) : 0;
    
    bool blinkAnim = ((now / 500) % 2 == 0);
    lcd->setCursor(0, 0);
    lcd->write(blinkAnim ? byte(3) : byte(1)); // ⚙️ Pompa / 💧 Tetesan Air berkedip dinamis
    lcd->print(F(" SEDANG NYIRAM "));
    
    // Baris 2: 🕒 25s   🌱 34%  AUT (16 Kolom)
    lcd->setCursor(0, 1);
    lcd->write(byte(5)); // 🕒 Icon Jam
    char sBuf[7];
    snprintf(sBuf, sizeof(sBuf), " %3ds ", runSecs);
    lcd->print(sBuf);
    
    lcd->write(byte(2)); // 🌱 Icon Tanah
    if (latestMoisturePercent >= 0) {
      char tBuf[7];
      snprintf(tBuf, sizeof(tBuf), " %-3d%% ", latestMoisturePercent);
      lcd->print(tBuf);
    } else {
      lcd->print(F(" --%  "));
    }
    
    lcd->print(isManualMode ? F("MAN") : F("AUT"));
    return; // Tetap di layar ini sampai pompa selesai menyiram
  }

  // Rotasi halaman tiap 3.5 detik (jika tidak sedang darurat / menyiram)
  if (now - lastLcdPageRotate >= 3500) {
    lcdScreenPage = (lcdScreenPage + 1) % 3;
    lastLcdPageRotate = now;
  }

  // 2. STATUS SENSOR TERPUTUS (PRIORITAS DARURAT)
  if (isEsp8266Unplugged) {
    lcd->setCursor(0, 0);
    lcd->print(F("! SENSOR PUTUS !"));
    lcd->setCursor(0, 1);
    lcd->print(F("POMPA KUNCI OFF "));
    return;
  }

  // 3. STATUS SUHU EKSTREM BAHAYA (>= 35 C)
  if (isDhtValid && lastSuhuC >= suhuBahaya) {
    lcd->setCursor(0, 0);
    lcd->print(F("! BAHAYA  SUHU !"));
    lcd->setCursor(0, 1);
    char bufHot[17];
    snprintf(bufHot, sizeof(bufHot), "Suhu:%.1fC PANAS", lastSuhuC);
    lcd->print(bufHot);
    return;
  }

  // 4. STATUS AWAL BOOT / MENUNGGU SENSOR
  if (isStartupWaiting) {
    lcd->setCursor(0, 0);
    lcd->print(F("! MEMUAT SISTEM !"));
    lcd->setCursor(0, 1);
    char wBuf[17];
    snprintf(wBuf, sizeof(wBuf), "Tunggu:%3d%% %3lus", (int)waitingPercent, waitingSecs);
    lcd->print(wBuf);
    return;
  }

  // 5. STATUS PENDINGINAN POMPA (COOLDOWN)
  if (relayCooldown) {
    lcd->setCursor(0, 0);
    lcd->print(F("! COOLDOWN LOCK !"));
    lcd->setCursor(0, 1);
    lcd->print(F("Pendinginan Pompa"));
    return;
  }

  // ========================================================
  // LAYAR 1: TELEMETRI LENGKAP & STATUS KONDISI TANAH
  // ========================================================
  if (lcdScreenPage == 0) {
    // --- BARIS 1: SUHU, UDARA, & MODE (AUTO / MAN) ---
    lcd->setCursor(0, 0);
    lcd->write(byte(0)); // 🌡️ Icon Suhu
    if (isDhtValid && !isnan(lastSuhuC)) {
      char sBuf[6];
      snprintf(sBuf, sizeof(sBuf), "%4.1f", lastSuhuC);
      lcd->print(sBuf);
      lcd->print('C');
    } else {
      lcd->print(F(" --C "));
    }

    // Jika suhu panas (> batasSuhu), tampilkan status [PANAS] di LCD fisik
    if (isDhtValid && lastSuhuC > batasSuhu) {
      lcd->print(F(" PANAS "));
      lcd->print(isManualMode ? F("MAN") : F("AUT"));
    } else {
      lcd->print(' ');
      lcd->write(byte(1)); // 💧 Icon Udara
      if (isDhtValid && !isnan(lastKelembapanUdara)) {
        int hVal = (int)round(lastKelembapanUdara);
        if (hVal < 10) lcd->print(' ');
        lcd->print(hVal);
        lcd->print('%');
      } else {
        lcd->print(F("--%"));
      }

      lcd->print(F("  "));
      if (isManualMode) {
        lcd->print(F(" MAN"));
      } else {
        lcd->print(F("AUTO"));
      }
    }

    // --- BARIS 2: TANAH & STATUS KERING / AKTUATOR ---
    lcd->setCursor(0, 1);
    lcd->write(byte(2)); // 🌱 Icon Tunas/Tanah
    if (latestMoisturePercent >= 0) {
      char tBuf[5];
      snprintf(tBuf, sizeof(tBuf), "%-3d%%", latestMoisturePercent);
      lcd->print(tBuf);
    } else {
      lcd->print(F("--% "));
    }

    if (latestMoisturePercent >= 0 && latestMoisturePercent < 25) {
      // Kondisi Bahaya Sangat Kering (<25%)
      lcd->print(F(" !KRITIS!  "));
    } else if (latestMoisturePercent >= 0 && latestMoisturePercent < batasTanah) {
      // Kondisi Peringatan Kering (<45%)
      lcd->print(F(" [KERING]  "));
    } else {
      // Kondisi Normal: Tampilkan Pompa & Lampu
      lcd->print(' ');
      lcd->write(byte(3)); // ⚙️ Pompa
      lcd->print(F("OFF "));
      lcd->write(byte(4)); // 💡 Lampu
      lcd->print(isLampOn ? F("ON  ") : F("OFF "));
    }

  } else if (lcdScreenPage == 1) {
    // ========================================================
    // LAYAR 2: WAKTU DETIK RTC, SINYAL RADIO & IP AKSES WEB
    // Baris 1: 🕒 16:52:30 WIB AUTO (Pas 16 Kolom)
    // Baris 2: 📶-62dB  192.168.4.1 (Pas 16 Kolom)
    // ========================================================
    lcd->setCursor(0, 0);
    lcd->write(byte(5)); // 🕒 Icon Jam
    lcd->print(' ');
    if (Rtc.GetIsRunning() && Rtc.IsDateTimeValid()) {
      RtcDateTime dt = Rtc.GetDateTime();
      char tBuf[9];
      snprintf(tBuf, sizeof(tBuf), "%02d:%02d:%02d", dt.Hour(), dt.Minute(), dt.Second());
      lcd->print(tBuf);
    } else {
      lcd->print(F("--:--:--"));
    }
    lcd->print(F("  "));
    if (isManualMode) {
      lcd->print(F(" MAN"));
    } else {
      lcd->print(F("AUTO"));
    }

    lcd->setCursor(0, 1);
    lcd->write(byte(6)); // 📶 Icon Sinyal
    char rBuf[6];
    if (!isEsp8266Unplugged && latestMoisturePercent >= 0) {
      snprintf(rBuf, sizeof(rBuf), "%3ddB", esp8266Rssi);
    } else {
      snprintf(rBuf, sizeof(rBuf), " --dB");
    }
    lcd->print(rBuf);
    lcd->print(' ');
    // Baterai Node Sensor ESP8266
    if (esp8266Battery > 0 && !isEsp8266Unplugged) {
      char bBuf[8];
      snprintf(bBuf, sizeof(bBuf), "B:%3d%%", esp8266Battery);
      lcd->print(bBuf);
    } else {
      lcd->print(F("B: --%"));
    }
    lcd->print(F(" AP"));
  } else {
    // ========================================================
    // LAYAR 3: PROFIL TANAMAN & HST TERSIMPAN DI NVS ESP32
    // Baris 1: 🌱 Cabai Rawit (Maks 14 Karakter)
    // Baris 2: HST:14  VEG   AUTO (Huruf Besar Jelas Tanpa Angka 9)
    // ========================================================
    lcd->setCursor(0, 0);
    lcd->write(byte(2)); // 🌱 Icon Tunas
    lcd->print(' ');
    String dispName = cropName;
    if (dispName.length() > 14) dispName = dispName.substring(0, 14);
    lcd->print(dispName);
    for (int pad = dispName.length(); pad < 14; pad++) lcd->print(' ');

    lcd->setCursor(0, 1);
    char hstBuf[17];
    String shortStage = (cropStage == "semai") ? "SEMAI" : (cropStage == "pindah" ? "PNDH" : (cropStage == "generatif" ? "GENR" : "VEG"));
    snprintf(hstBuf, sizeof(hstBuf), "HST:%-3d %-5s %s", (int)cropAge, shortStage.c_str(), isManualMode ? "MAN " : "AUTO");
    lcd->print(hstBuf);
  }
}

void printNvsConfigToSerial() {
  Serial.println(F("\n=================================================================="));
  Serial.println(F("🌿 SMART FARM PRECISION AGRICULTURE - KONFIGURASI NVS TERSIMPAN"));
  Serial.println(F("=================================================================="));
  Serial.printf("  🌱 Batas Kelembapan Tanah  : < %d%% (Pemicu Pompa Siram Otomatis)\n", batasTanah);
  Serial.printf("  🌡️ Batas Suhu Panas        : > %.1f°C (Ambang Siram Darurat Panas)\n", batasSuhu);
  Serial.printf("  🕒 Jadwal Slot 1 (Pagi)    : %02d:%02d | Durasi: %d Menit | Status: %s\n",
                sched1_h, sched1_m, sched1_dur, sched1_en ? "AKTIF" : "NONAKTIF");
  Serial.printf("  🕒 Jadwal Slot 2 (Sore)    : %02d:%02d | Durasi: %d Menit | Status: %s\n",
                sched2_h, sched2_m, sched2_dur, sched2_en ? "AKTIF" : "NONAKTIF");
  Serial.printf("  💡 Jadwal Lampu Grow Light : %02d:%02d | Durasi: %d Jam   | Status: %s\n",
                lamp_on_h, lamp_on_m, lamp_dur, lamp_sched_en ? "AKTIF" : "NONAKTIF");
  Serial.printf("  ⚙️ Spesifikasi Pompa      : Debit %d L/Jam | Daya %d Watt | PLN Rp %d/kWh\n",
                pumpLph, pumpWatt, plnTariff);
  Serial.printf("  📊 Akumulasi Pompa (NVS)   : %u Nyala | %u Mati | %lu Detik Total (~%.1f L Air | Rp %.1f)\n",
                pumpCount, pumpOffCount, totalPumpSecs,
                ((float)totalPumpSecs / 3600.0f) * (float)pumpLph,
                (((float)totalPumpSecs / 3600.0f) * ((float)pumpWatt / 1000.0f)) * (float)plnTariff);
  Serial.printf("  🌾 Profil Budidaya         : %s | Usia: %d HST | Fase: %s | Daun: %d | Luas: %d m2\n",
                cropName.c_str(), cropAge, cropStage.c_str(), cropLeaves, cropArea);
  
  if (Rtc.GetIsRunning() && Rtc.IsDateTimeValid()) {
    RtcDateTime now = Rtc.GetDateTime();
    Serial.printf("  ⏰ Waktu Hardware RTC      : %04d-%02d-%02d %02d:%02d:%02d WIB (Valid & Berjalan)\n",
                  now.Year(), now.Month(), now.Day(), now.Hour(), now.Minute(), now.Second());
  } else {
    Serial.println(F("  ⏰ Waktu Hardware RTC      : BELUM SINKRON (Menunggu Sinkronisasi Browser)"));
  }
  Serial.println(F("==================================================================\n"));
}

void setup() {
#if defined(RTC_CNTL_BROWN_OUT_REG)
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // NONAKTIFKAN BROWNOUT DETECTOR (CEGAH REBOOT KARENA INDUKTIF POMPA MATI)
#endif
  Serial.begin(115200);
  initLcd16x2(); // Inisialisasi LCD 16x2 I2C Otomatis

  // Initialize NVS Preferences
  preferences.begin("smartfarm", false);
  pumpLph = preferences.getUShort("pumpLph", 1800);
  pumpWatt = preferences.getUShort("pumpWatt", 25);
  plnTariff = preferences.getUShort("plnTariff", 415);
  pumpCount = preferences.getUInt("pumpCount", 0);
  pumpOffCount = preferences.getUInt("pumpOffCount", 0);
  totalPumpSecs = preferences.getULong("pumpSecs", 0);
  cropMode = preferences.getUChar("cropMode", 0);
  cropName = preferences.getString("cropName", "Cabai Rawit");
  cropAge = preferences.getUShort("cropAge", 14);
  cropStage = preferences.getString("cropStage", "vegetatif");
  cropLeaves = preferences.getUChar("cropLeaves", 4);
  cropEnv = preferences.getString("cropEnv", "bedengan");
  cropArea = preferences.getUShort("cropArea", 100);
  batasTanah = preferences.getInt("batasTanah", 45);
  batasSuhu = preferences.getFloat("batasSuhu", 30.0);
  
  sched1_en = preferences.getUChar("s1_en", 1);
  sched1_h = preferences.getUChar("s1_h", 6);
  sched1_m = preferences.getUChar("s1_m", 0);
  sched1_dur = preferences.getUChar("s1_dur", 15);
  
  sched2_en = preferences.getUChar("s2_en", 1);
  sched2_h = preferences.getUChar("s2_h", 17);
  sched2_m = preferences.getUChar("s2_m", 0);
  sched2_dur = preferences.getUChar("s2_dur", 10);

  lamp_sched_en = preferences.getUChar("l_en", 0);
  lamp_on_h = preferences.getUChar("l_h", 18);
  lamp_on_m = preferences.getUChar("l_m", 0);
  lamp_dur = preferences.getUChar("l_dur", 12);

  // Initialize RTC DS1302
  Rtc.Begin();
  if (Rtc.GetIsWriteProtected()) {
    Rtc.SetIsWriteProtected(false);
  }
  if (!Rtc.GetIsRunning()) {
    Rtc.SetIsRunning(true);
  }
  
  startupWaitStartTime = millis(); // REKAM DETIK AWAL MASA TUNGGU SEJAK BOOT

  if (!LittleFS.begin(true)) {
    Serial.println(F("Gagal Inisialisasi LittleFS!"));
  } else {
    if (!LittleFS.exists("/log.csv"))
      writeLogHeader();
  }

  dht.begin();
  pinMode(RELAY1, OUTPUT);
  digitalWrite(RELAY1, HIGH); // SANGAT TEGAS INSIALISASI AWAL MATI (HIGH / 3.3V)

  pinMode(RELAY2, OUTPUT);
  digitalWrite(RELAY2, HIGH); // LAMPU JUGA MATI SAAT AWAL


  pinMode(LED_HIJAU, OUTPUT);
  pinMode(LED_KUNING, OUTPUT);
  pinMode(LED_MERAH, OUTPUT); // TRAFFIC LIGHT LEDS (PIN 32, 33, 27)

  // Inisialisasi awal lampu LED (SEMUA MATI SEJAK BOOT)
  digitalWrite(LED_HIJAU, LOW);
  digitalWrite(LED_KUNING, LOW);
  digitalWrite(LED_MERAH, LOW);

  WiFi.mode(WIFI_AP_STA);
  WiFi.setAutoReconnect(false); // Matikan pencarian router latar belakang agar kanal tidak hopping!
  WiFi.disconnect();            // Putuskan interface STA dari scanning jaringan luar
  WiFi.softAP("SmartFarm-ESP32", "12345678", 1); // Kunci Access Point mutlak di Kanal 1

  // DONGKRAK POWER MAKSIMAL RADIO ESP32 (84 * 0.25dBm = +21 dBm MAX POWER)
  esp_wifi_set_max_tx_power(84);

  // KUNCI KANAL RADIO PHY ESP32 KE KANAL 1 SAMA PERSIS DENGAN ESP8266
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

  // MATIKAN MODEM SLEEP AGAR RADIO TIDAK MENGALAMI PACKET DROP
  WiFi.setSleep(false);
  esp_wifi_set_ps(WIFI_PS_NONE);

  if (esp_now_init() != ESP_OK) {
    Serial.println(F("❌ [ESP-NOW] Gagal Inisialisasi ESP-NOW!"));
  } else {
    Serial.println(F("✅ [ESP-NOW] Inisialisasi Berhasil di Kanal 1"));
  }

#if defined(ESP_IDF_VERSION_MAJOR) && ESP_IDF_VERSION_MAJOR >= 5
  esp_now_register_recv_cb(esp_now_recv_info_t_wrapper);
#else
  esp_now_register_recv_cb((esp_now_recv_cb_t)esp_now_recv_info_t_wrapper);
#endif

  // Peer broadcast tidak diperlukan untuk receiver dan di-nonaktifkan agar tidak bentrok radio
  // esp_now_peer_info_t bPeer = {};
  // memset(&bPeer, 0, sizeof(bPeer));
  // memset(bPeer.peer_addr, 0xFF, 6);
  // bPeer.channel = 1;
  // bPeer.encrypt = false;
  // bPeer.ifidx = WIFI_IF_AP;
  // esp_now_add_peer(&bPeer);
  // bPeer.ifidx = WIFI_IF_STA;
  // esp_now_add_peer(&bPeer);

  Serial.print(F("📡 [ESP32 MAC AP]  : "));
  Serial.println(WiFi.softAPmacAddress());
  Serial.print(F("📡 [ESP32 MAC STA] : "));
  Serial.println(WiFi.macAddress());

  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.on("/pushWeather", HTTP_POST, handlePushWeather);
  server.on("/downloadLog", HTTP_GET, handleDownloadLog);
  server.on("/clearLogs", HTTP_POST, handleClearLogs);
  
  server.on("/setLampMode", handleSetLampMode);
  server.on("/toggleLamp", handleToggleLamp);
  server.on("/setLampSchedule", handleSetLampSchedule);
  server.on("/setMode", handleSetMode);
  server.on("/setThreshold", handleSetThreshold);
  server.on("/toggleRelay", handleToggleRelay);
  server.on("/reboot", HTTP_POST, handleReboot);
  server.on("/setRtc", handleSetRtc);
  server.on("/setSchedule", handleSetSchedule);
  server.on("/setPumpConfig", handleSetPumpConfig);
  server.on("/setCropProfile", handleSetCropProfile);
  server.on("/getCropHistory", HTTP_GET, handleGetCropHistory);
  server.on("/saveCropHistory", HTTP_POST, handleSaveCropHistory);
  server.on("/resetCropHistory", HTTP_POST, handleResetCropHistory);
  server.on("/downloadCropHistory", HTTP_GET, handleDownloadCropHistory);
  server.on("/resetStats", handleResetPumpStats);
  server.on("/resetPumpStats", handleResetPumpStats);
  server.begin();

  // ================= HARDWARE TASK WATCHDOG TIMER (WDT) =================
  // Proteksi Failsafe: Jika ESP32 mengalami crash/hang > 15 detik, otomatis auto-restart!
#if defined(ESP_IDF_VERSION_MAJOR) && ESP_IDF_VERSION_MAJOR >= 5
  esp_task_wdt_config_t wdt_config = {
      .timeout_ms = WDT_TIMEOUT_SECONDS * 1000,
      .idle_core_mask = 0,
      .trigger_panic = true
  };
  esp_task_wdt_reconfigure(&wdt_config);
  esp_task_wdt_add(NULL);
#else
  esp_task_wdt_init(WDT_TIMEOUT_SECONDS, true);
  esp_task_wdt_add(NULL);
#endif
  Serial.printf("🛡️ [WATCHDOG] Hardware WDT Aktif (%d detik). Anti-Hang Siap Beroperasi!\n", WDT_TIMEOUT_SECONDS);

  // Tampilkan ringkasan seluruh data konfigurasi NVS tersimpan ke Serial Monitor
  printNvsConfigToSerial();
}

void loop() {
  esp_task_wdt_reset(); // Memberi makan watchdog di setiap putaran loop

  server.handleClient();

  checkSystemStatus();

  updateRelayState();

  updateLampState(); // Evaluasi & Sinkronisasi Fisik Relay Lampu (Pin 27)

  updateLEDState();

  updateLcdDisplay(); // Refresh LCD 16x2 Real-Time & Non-Blocking

  if (isRelayOn) {
    if (millis() - lastPumpSecUpdate >= 1000) {
      lastPumpSecUpdate = millis();
      totalPumpSecs++;
      hourlyPumpSecs++;

      // Auto-save tiap 30 detik ke NVS saat menyiram agar aman jika listrik padam mendadak
      if (millis() - lastNvsPumpSecSave >= 30000) {
        lastNvsPumpSecSave = millis();
        preferences.putULong("pumpSecs", totalPumpSecs);
      }
    }
  }

  if (millis() - lastLogTime >= LOG_INTERVAL) {
    lastLogTime = millis();
    appendLogData();
  }

  if (newDataReceived) {
    Serial.println(F("------------------------------------------------------------------------------------------------------"));
    Serial.printf("📡 [ESP-NOW RECV] Paket Data Tanah Diterima! 🌱 Kelembapan: %d%% (%s) | 📊 Raw ADC: %u | 🔋 Baterai: %d%% | 📶 Sinyal: %d dBm\n",
                  latestMoisturePercent, getSoilCategory(latestMoisturePercent).c_str(), latestRawAdc, esp8266Battery, esp8266Rssi);
    Serial.println(F("------------------------------------------------------------------------------------------------------"));
    newDataReceived = false;
  }

  if (millis() - lastSerialPrintTime >= 1000) { // PRINT LOG REALTIME 1 DETIK!
    lastSerialPrintTime = millis();

    if (!isSystemError && !isEsp8266Unplugged && !isStartupWaiting && wasInErrorState) {
      Serial.println(F("\n✅ =========================================="));
      Serial.println(
          F("✅ SYSTEM RECOVERY: Sinyal Radio ESP8266 Terhubung Kembali!"));
      Serial.println(F("============================================="));
      wasInErrorState = false;
    }

    if (isSystemError || isEsp8266Unplugged) {
      wasInErrorState = true;
      Serial.print(F("🚨 [FAILSAFE WATCHDOG] "));
      Serial.print(systemErrorMsg);
      Serial.println(F(" | ⚙️ Pompa: KUNCI MATI (HIGH) | 🔴 Traffic: Merah Berkedip | Failsafe: AKTIF"));
    } else if (isStartupWaiting) {
      wasInErrorState = true;
      Serial.print(F("⏳ [STARTUP WAITING] Mencari sinyal ESP8266... "));
      Serial.print(waitingPercent);
      Serial.print(F("% ("));
      Serial.print(waitingSecs);
      Serial.println(F("s / 75s) | ⚙️ Pompa: Kunci OFF"));
    } else if (isRelayOn) {
      unsigned long runSecs = (millis() - relayStartTime) / 1000;
      unsigned long timeElapsed = millis() - relayStartTime;
      unsigned long timeLeft = (MAX_RELAY_TIME > timeElapsed) ? (MAX_RELAY_TIME - timeElapsed) / 1000 : 0;
      float waterEst = ((float)runSecs / 60.0) * ((float)pumpLph / 60.0);
      unsigned long secSinceRecv = (lastRecvTime > 0) ? (millis() - lastRecvTime) / 1000 : 0;
      String shortStage = (cropStage == "semai") ? "SEMAI" : (cropStage == "pindah" ? "PNDH" : (cropStage == "generatif" ? "GENR" : "VEG"));

      Serial.println(F("------------------------------------------------------------------------------------------------------------------------------------------------------"));
      Serial.printf("💧 [SEDANG MENYIRAM LAHAN (%s)] 🌾 %s (%d HST | %s) | Durasi: %lus (~%.1fL Air) | 🌱 Tanah: %d%% -> Target: >=80%% | Sisa Max: %lum %lus | ⚙️ Pin26: ON | ⏳ Paket: %lus lalu\n",
                    isManualMode ? "MANUAL" : (isRtcScheduleActive ? "AUTO-RTC" : "AUTO-SENSOR"),
                    cropName.c_str(), cropAge, shortStage.c_str(),
                    runSecs, waterEst, latestMoisturePercent, timeLeft / 60, timeLeft % 60, secSinceRecv);
      Serial.println(F("------------------------------------------------------------------------------------------------------------------------------------------------------"));
    } else {
      char timeBuf[12] = "--:--:--";
      if (Rtc.GetIsRunning() && Rtc.IsDateTimeValid()) {
        RtcDateTime dt = Rtc.GetDateTime();
        snprintf(timeBuf, sizeof(timeBuf), "%02d:%02d:%02d", dt.Hour(), dt.Minute(), dt.Second());
      }
      unsigned long secSinceRecv = (lastRecvTime > 0) ? (millis() - lastRecvTime) / 1000 : 0;
      float es = (lastSuhuC > 0) ? 0.61078f * expf((17.27f * lastSuhuC) / (lastSuhuC + 237.3f)) : 0.0f;
      float ea = es * (lastKelembapanUdara / 100.0f);
      float vpdVal = (es > ea) ? (es - ea) : 0.0f;
      String shortStage = (cropStage == "semai") ? "SEMAI" : (cropStage == "pindah" ? "PNDH" : (cropStage == "generatif" ? "GENR" : "VEG"));

      Serial.printf("[%s] 🌾 %s (%d HST | %s) | 🌱 Tanah: %d%% (%s | ADC:%u) | 🌡️ Udara: %.1f°C (%.0f%%) | 🍃 VPD: %.2fkPa | ⚙️ Pompa: %s (%s) | 💡 Lampu: %s | 📶 RSSI: %ddBm | 🔋 Bat: %d%% | ⏳ Paket: %lus lalu (~60s)\n",
                    timeBuf, cropName.c_str(), cropAge, shortStage.c_str(),
                    latestMoisturePercent, getSoilCategory(latestMoisturePercent).c_str(), latestRawAdc,
                    lastSuhuC, lastKelembapanUdara, vpdVal,
                    relayCooldown ? "COOLDOWN" : (isRelayOn ? "ON" : "OFF"),
                    isManualMode ? "MANUAL" : "AUTO",
                    isLampOn ? "ON" : "OFF",
                    esp8266Rssi, esp8266Battery,
                    secSinceRecv);
    }
  }
}
