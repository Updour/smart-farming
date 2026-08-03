#include "index.h" // Mengambil kode HTML dari file terpisah yang baru saja kita buat
#include <DHT.h>
#include <WebServer.h>
#include <WiFi.h>
#include <esp_now.h>

// --- KONFIGURASI DHT & TRAFFIC LIGHT ---
#define DHTPIN 4
#define DHTTYPE DHT11
#define RELAY1 25

#define LED_HIJAU 32
#define LED_KUNING 33
#define LED_MERAH 27

DHT dht(DHTPIN, DHTTYPE);

const float batasSuhu = 30.0;
const float suhuBahaya = 35.0;

// --- KONFIGURASI ESP-NOW (SOIL MOISTURE) ---
typedef struct struct_message {
  int8_t persen;
} struct_message;

struct_message myData;

// Variabel Data Sensor Global (agar bisa dibaca oleh Web Server)
volatile bool newDataReceived = false;
int8_t latestMoisturePercent = 100; // Default 100% (Basah/Aman)
unsigned long lastRecvTime = 0;

// Menyimpan data cuaca untuk Web Server
float lastSuhuC = 0.0;
float lastSuhuF = 0.0;
float lastKelembapanUdara = 0.0;
float lastHeatIndexC = 0.0;
float lastHeatIndexF = 0.0;
float lastDewPoint = 0.0;

// --- TIMER RELAY (MAKSIMAL 20 MENIT) ---
unsigned long relayStartTime = 0;
bool isRelayOn = false;
bool relayCooldown = false;
const unsigned long MAX_RELAY_TIME = 20UL * 60UL * 1000UL; // 20 Menit// 20 Menit

// --- VARIABEL KENDALI WEB MANUAL ---
bool isManualMode = false;
bool manualRelayState = false;

// --- WEB SERVER LOKAL ---
WebServer server(80);

// Callback saat data ESP-NOW diterima
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  latestMoisturePercent = myData.persen;
  newDataReceived = true;
  lastRecvTime = millis();
}
void esp_now_recv_info_t_wrapper(const esp_now_recv_info *info,
                                 const uint8_t *data, int len) {
  OnDataRecv(info->src_addr, data, len);
}

// ==== API WEB SERVER ====
void handleRoot() { server.send_P(200, "text/html", index_html); }

void handleData() {
  String bgClass = "aman";
  String textStatus = "Status: AMAN";
  if (lastSuhuC >= suhuBahaya || latestMoisturePercent < 25) {
    bgClass = "bahaya";
    textStatus = "BAHAYA (Sangat Kering / Panas!)";
  } else if (lastSuhuC > batasSuhu || latestMoisturePercent < 45) {
    bgClass = "peringatan";
    textStatus = "PERINGATAN (Mulai Kering / Hangat)";
  }

  int timeout = (millis() - lastRecvTime > 10000 && lastRecvTime != 0) ? 1 : 0;

  String json = "{";
  json += "\"suhuC\":\"" + String(lastSuhuC) + "\",";
  json += "\"suhuF\":\"" + String(lastSuhuF) + "\",";
  json += "\"hum\":\"" + String(lastKelembapanUdara) + "\",";
  json += "\"heatC\":\"" + String(lastHeatIndexC) + "\",";
  json += "\"heatF\":\"" + String(lastHeatIndexF) + "\",";
  json += "\"dew\":\"" + String(lastDewPoint) + "\",";
  json += "\"soil\":\"" + String(latestMoisturePercent) + "\",";
  json += "\"timeout\":" + String(timeout) + ",";
  json += "\"statusColor\":\"" + bgClass + "\",";
  json += "\"statusText\":\"" + textStatus + "\",";
  json += "\"isManual\":" + String(isManualMode ? 1 : 0) + ",";
  json += "\"relayOn\":" + String(isRelayOn ? 1 : 0) + ",";
  json += "\"cooldown\":" + String(relayCooldown ? 1 : 0);
  json += "}";

  server.send(200, "application/json", json);
}

void handleSetMode() {
  if (server.hasArg("m")) {
    if (server.arg("m") == "manual")
      isManualMode = true;
    else
      isManualMode = false;
  }
  server.send(200, "text/plain", "OK");
}

void handleToggleRelay() {
  if (server.hasArg("s")) {
    if (server.arg("s") == "on")
      manualRelayState = true;
    else
      manualRelayState = false;
  }
  server.send(200, "text/plain", "OK");
}
// ========================

// Timer untuk Serial Print
unsigned long lastSerialPrintTime = 0;

void setup() {
  Serial.begin(115200);

  // --- SETUP DHT & PIN ---
  dht.begin();
  pinMode(RELAY1, OUTPUT);
  digitalWrite(RELAY1, HIGH); // OFF

  pinMode(LED_HIJAU, OUTPUT);
  pinMode(LED_KUNING, OUTPUT);
  pinMode(LED_MERAH, OUTPUT);

  // --- SETUP WiFi AP & ESP-NOW ---
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("SmartFarm-ESP32", "12345678");

  Serial.println(F("========================================="));
  Serial.println(F("✅ JARINGAN WIFI LOKAL BERHASIL DIBUAT!"));
  Serial.println(F("Nama WiFi : SmartFarm-ESP32"));
  Serial.println(F("Password  : 12345678"));
  Serial.print(F("Buka Web  : http://"));
  Serial.println(WiFi.softAPIP());
  Serial.print(F("MAC ESP32 : "));
  Serial.println(WiFi.macAddress());
  Serial.println(F("========================================="));

  if (esp_now_init() != ESP_OK) {
    Serial.println(F("Gagal menginisialisasi ESP-NOW"));
    return;
  }
  esp_now_register_recv_cb(esp_now_recv_info_t_wrapper);

  // --- SETUP WEB SERVER ---
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.on("/setMode", handleSetMode);
  server.on("/toggleRelay", handleToggleRelay);
  server.begin();
}

void loop() {
  // 1. Jalankan Web Server
  server.handleClient();

  // 2. MENGEMBALIKAN LOG SUKSES/GAGAL ESP-NOW KE FORMAT ASLI
  if (newDataReceived) {
    Serial.println(F("-----------------------------------------"));
    Serial.print(F("[ESP-NOW] SUKSES terima data! Kelembapan Tanah: "));
    Serial.print(latestMoisturePercent);
    Serial.println(F("%"));
    Serial.println(F("-----------------------------------------"));
    newDataReceived = false;
  } else if (millis() - lastRecvTime > 10000 && lastRecvTime != 0) {
    Serial.println(F("-----------------------------------------"));
    Serial.println(F("[ESP-NOW] GAGAL/TIMEOUT: Tidak ada data dari ESP8266 "
                     "selama 10 detik! Cek daya/koneksi ESP8266."));
    Serial.println(F("-----------------------------------------"));
    lastRecvTime = 0;
  }

  // 3. Baca Sensor Suhu
  if (millis() - lastSerialPrintTime >= 2000) {
    lastSerialPrintTime = millis();

    float suhu = dht.readTemperature();
    float suhuF = dht.readTemperature(true);
    float kelembapan = dht.readHumidity();

    if (isnan(suhu) || isnan(suhuF) || isnan(kelembapan)) {
      Serial.println(F("Gagal baca sensor DHT11!"));
    } else {
      // Simpan ke variabel global untuk Web Server
      lastSuhuC = suhu;
      lastSuhuF = suhuF;
      lastKelembapanUdara = kelembapan;
      lastHeatIndexC = dht.computeHeatIndex(suhu, kelembapan, false);
      lastHeatIndexF = dht.computeHeatIndex(suhuF, kelembapan, true);

      float a = 17.271;
      float b = 237.7;
      float gamma = (a * suhu / (b + suhu)) + log(kelembapan / 100.0);
      lastDewPoint = (b * gamma) / (a - gamma);

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
      Serial.println(F("========================================="));

      digitalWrite(LED_HIJAU, LOW);
      digitalWrite(LED_KUNING, LOW);
      digitalWrite(LED_MERAH, LOW);

      Serial.print(F("[DHT11] Suhu Saat Ini: "));
      Serial.print(suhu);
      Serial.println(F(" °C"));

      bool butuhRelayON = false;

      if (suhu >= suhuBahaya || latestMoisturePercent < 25) {
        digitalWrite(LED_MERAH, HIGH);
        butuhRelayON = true;
        Serial.print(F(" => STATUS: BAHAYA (Suhu: "));
        Serial.print(suhu);
        Serial.print(F("°C, Tanah: "));
        Serial.print(latestMoisturePercent);
        Serial.print(F("%)"));
      } else if (suhu > batasSuhu || latestMoisturePercent < 45) {
        digitalWrite(LED_KUNING, HIGH);
        butuhRelayON = true;
        Serial.print(F(" => STATUS: PERINGATAN (Suhu: "));
        Serial.print(suhu);
        Serial.print(F("°C, Tanah: "));
        Serial.print(latestMoisturePercent);
        Serial.print(F("%)"));
      } else {
        digitalWrite(LED_HIJAU, HIGH);
        butuhRelayON = false;
        relayCooldown = false;
        Serial.print(F(" => STATUS: AMAN (Suhu: "));
        Serial.print(suhu);
        Serial.print(F("°C, Tanah: "));
        Serial.print(latestMoisturePercent);
        Serial.print(F("%)"));
      }

      // KONDISI RELAY DENGAN DUKUNGAN MANUAL OVERRIDE
      if (isManualMode) {
        if (manualRelayState) {
          digitalWrite(RELAY1, LOW); // NYALA
          digitalWrite(LED_KUNING, HIGH);
          digitalWrite(LED_HIJAU, LOQ);
          isRelayOn = true;
          Serial.println(F(" | [MODE MANUAL WEB] RELAY DI-ON-KAN OLEH USER!"));
        } else {
          digitalWrite(RELAY1, HIGH); // MATI
          digitalWrite(LED_HIJAU, HIGH);
          isRelayOn = false;
          Serial.println(F(" | [MODE MANUAL WEB] RELAY DIMATIKAN OLEH USER!"));
        }
      } else {
        // Mode Auto (Logika Sensor & Timer Maks 20 Menit)
        if (butuhRelayON) {
          if (relayCooldown) {
            digitalWrite(RELAY1, HIGH); // MATI
            isRelayOn = false;
            Serial.println(
                F(" | RELAY DIPAKSA OFF (Cooldown/Pendinginan Mesin!)"));
          } else {
            if (!isRelayOn) {
              isRelayOn = true;
              relayStartTime = millis();
              digitalWrite(RELAY1, LOW); // NYALA
              Serial.println(F(" | RELAY MULAI ON"));
            } else {
              unsigned long timeElapsed = millis() - relayStartTime;
              if (timeElapsed >= MAX_RELAY_TIME) {
                relayCooldown = true;
                isRelayOn = false;
                digitalWrite(RELAY1, HIGH); // MATI
                Serial.println(F(" | RELAY OFF SECARA PAKSA! (Batas 20 Menit Tercapai)"));
              } else {
                unsigned long timeLeft = (MAX_RELAY_TIME - timeElapsed) / 1000;
                digitalWrite(RELAY1, LOW); // NYALA
                Serial.print(F(" | RELAY ON (Sisa Waktu Nyala: "));
                Serial.print(timeLeft / 60);
                Serial.print(F("m "));
                Serial.print(timeLeft % 60);
                Serial.println(F("s)"));
              }
            }
          }
        } else {
          digitalWrite(RELAY1, HIGH); // MATI
          isRelayOn = false;
          Serial.println(F(" | RELAY OFF"));
        }
      }
    }
  }
}
