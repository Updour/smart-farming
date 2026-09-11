/*
 * =========================================================================================
 * PROYEK: SMART IoT 8-CHANNEL LAMP SCHEDULER
 * MIKROKONTROLER : ESP32 Dev Module
 * MODUL REAL-TIME: RTC DS1302 (3-Wire Interface)
 * MODUL AKTUATOR : Relay 8 Channel Optocoupler (Jumper COM-HIGH: ACTIVE HIGH)
 * =========================================================================================
 * 
 * KONFIGURASI WIRING PINOUT ESP32:
 * 
 * 1. RTC DS1302:
 *    - VCC       -> 3.3V ESP32
 *    - GND       -> GND ESP32
 *    - DAT / IO  -> GPIO 14
 *    - CLK / SCLK-> GPIO 12
 *    - RST / CE  -> GPIO 13
 * 
 * 2. RELAY 8-CHANNEL (ACTIVE HIGH - Jumper dipasang pada COM & HIGH):
 *    - VCC / DC+ -> VIN (5V) ESP32 / Adaptor 5V eksternal
 *    - GND / DC- -> GND ESP32
 *    - IN1 (S1)  -> GPIO 25 (Lampu 1)
 *    - IN2 (S2)  -> GPIO 26 (Lampu 2)
 *    - IN3 (S3)  -> GPIO 32 (Lampu 3)
 *    - IN4 (S4)  -> GPIO 33 (Lampu 4)
 *    - IN5 (S5)  -> GPIO 27 (Lampu 5)
 *    - IN6 (S6)  -> GPIO 18 (Lampu 6)
 *    - IN7 (S7)  -> GPIO 19 (Lampu 7)
 *    - IN8 (S8)  -> GPIO 23 (Lampu 8)
 * 
 * KETENTUAN LOGIKA ACTIVE HIGH:
 * - HIGH (3.3V) = Relay ON  (Lampu Menyala)
 * - LOW  (0V)   = Relay OFF (Lampu Mati)
 * =========================================================================================
 */

#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include "web_dashboard.h"

// --- KONFIGURASI PIN HARDWARE ---
#define PIN_RTC_IO   14   // DAT
#define PIN_RTC_CLK  12   // CLK
#define PIN_RTC_CE   13   // RST / CE

#define NUM_CHANNELS 8

#define PIN_RELAY_1  25
#define PIN_RELAY_2  26
#define PIN_RELAY_3  32
#define PIN_RELAY_4  33
#define PIN_RELAY_5  27
#define PIN_RELAY_6  18
#define PIN_RELAY_7  19
#define PIN_RELAY_8  23

#define RELAY_ACTIVE_STATE   HIGH
#define RELAY_INACTIVE_STATE LOW

// --- INISIALISASI OBJEK RTC DS1302 ---
ThreeWire rtcWire(PIN_RTC_IO, PIN_RTC_CLK, PIN_RTC_CE);
RtcDS1302<ThreeWire> Rtc(rtcWire);

// --- WEB SERVER & NVS PREFERENCES ---
WebServer server(80);
Preferences prefs;

// --- STRUKTUR DATA CHANNEL RELAY ---
struct LampChannel {
  uint8_t pin;
  String name;
  uint8_t mode;      // 0 = MANUAL, 1 = AUTOMATIC (Jadwal RTC)
  uint8_t state;     // 0 = OFF, 1 = ON
  uint8_t on_hour;   // 0 - 23
  uint8_t on_min;    // 0 - 59
  uint8_t off_hour;  // 0 - 23
  uint8_t off_min;   // 0 - 59
};

LampChannel lamps[NUM_CHANNELS] = {
  { PIN_RELAY_1, "Lampu Teras",      1, 0, 18, 0,  6, 0 },
  { PIN_RELAY_2, "Lampu Taman",      1, 0, 18, 30, 5, 30 },
  { PIN_RELAY_3, "Lampu Garasi",     1, 0, 17, 45, 6, 15 },
  { PIN_RELAY_4, "Lampu Balkon",     1, 0, 18, 0,  6, 0 },
  { PIN_RELAY_5, "Lampu Ruang Tamu", 1, 0, 17, 30, 22, 30 },
  { PIN_RELAY_6, "Lampu Dapur",      1, 0, 18, 0,  23, 0 },
  { PIN_RELAY_7, "Lampu Koridor",    1, 0, 18, 0,  6, 0 },
  { PIN_RELAY_8, "Lampu Belakang",   1, 0, 18, 15, 5, 45 }
};

// Variable Waktu RTC Terkini
RtcDateTime nowTime;
unsigned long lastRtcReadMs = 0;
unsigned long lastSchedCheckMs = 0;

// =========================================================================================
// HELPER FUNCTION: PARSING JSON STRING RINGAN (TANPA DEPENDENSI EKSTERNAL)
// =========================================================================================
int getJsonInt(const String& json, const String& key, int defaultVal = 0) {
  int keyIdx = json.indexOf("\"" + key + "\"");
  if (keyIdx < 0) return defaultVal;
  int colonIdx = json.indexOf(':', keyIdx);
  if (colonIdx < 0) return defaultVal;
  int startIdx = colonIdx + 1;
  while (startIdx < json.length() && (json[startIdx] == ' ' || json[startIdx] == '\"')) startIdx++;
  int endIdx = startIdx;
  while (endIdx < json.length() && (isDigit(json[endIdx]) || json[endIdx] == '-')) endIdx++;
  if (startIdx == endIdx) return defaultVal;
  return json.substring(startIdx, endIdx).toInt();
}

String getJsonString(const String& json, const String& key, const String& defaultVal = "") {
  int keyIdx = json.indexOf("\"" + key + "\"");
  if (keyIdx < 0) return defaultVal;
  int colonIdx = json.indexOf(':', keyIdx);
  if (colonIdx < 0) return defaultVal;
  int firstQuote = json.indexOf('\"', colonIdx);
  if (firstQuote < 0) return defaultVal;
  int secondQuote = json.indexOf('\"', firstQuote + 1);
  if (secondQuote < 0) return defaultVal;
  return json.substring(firstQuote + 1, secondQuote);
}

// =========================================================================================
// PENYIMPANAN NVS PREFERENCES
// =========================================================================================
void loadPreferences() {
  prefs.begin("smart_lamp8", true); // Read-only mode

  for (int i = 0; i < NUM_CHANNELS; i++) {
    String pName = "nm" + String(i);
    String pMode = "md" + String(i);
    String pOnH  = "oh" + String(i);
    String pOnM  = "om" + String(i);
    String pOffH = "fh" + String(i);
    String pOffM = "fm" + String(i);

    if (prefs.isKey(pName.c_str())) lamps[i].name = prefs.getString(pName.c_str(), lamps[i].name);
    lamps[i].mode     = prefs.getUChar(pMode.c_str(), lamps[i].mode);
    lamps[i].on_hour  = prefs.getUChar(pOnH.c_str(), lamps[i].on_hour);
    lamps[i].on_min   = prefs.getUChar(pOnM.c_str(), lamps[i].on_min);
    lamps[i].off_hour = prefs.getUChar(pOffH.c_str(), lamps[i].off_hour);
    lamps[i].off_min  = prefs.getUChar(pOffM.c_str(), lamps[i].off_min);
  }

  prefs.end();
  Serial.println(F("[NVS] Konfigurasi 8 Channel berhasil dimuat."));
}

void saveChannelSchedule(int ch) {
  if (ch < 0 || ch >= NUM_CHANNELS) return;
  prefs.begin("smart_lamp8", false); // Read-write mode

  String pName = "nm" + String(ch);
  String pOnH  = "oh" + String(ch);
  String pOnM  = "om" + String(ch);
  String pOffH = "fh" + String(ch);
  String pOffM = "fm" + String(ch);

  prefs.putString(pName.c_str(), lamps[ch].name);
  prefs.putUChar(pOnH.c_str(), lamps[ch].on_hour);
  prefs.putUChar(pOnM.c_str(), lamps[ch].on_min);
  prefs.putUChar(pOffH.c_str(), lamps[ch].off_hour);
  prefs.putUChar(pOffM.c_str(), lamps[ch].off_min);

  prefs.end();
}

void saveChannelMode(int ch) {
  if (ch < 0 || ch >= NUM_CHANNELS) return;
  prefs.begin("smart_lamp8", false);
  String pMode = "md" + String(ch);
  prefs.putUChar(pMode.c_str(), lamps[ch].mode);
  prefs.end();
}

// =========================================================================================
// KONTROL FISIK RELAY (ACTIVE HIGH)
// =========================================================================================
void applyRelayHardware(int ch, uint8_t state) {
  if (ch < 0 || ch >= NUM_CHANNELS) return;
  lamps[ch].state = state;
  digitalWrite(lamps[ch].pin, (state == 1) ? RELAY_ACTIVE_STATE : RELAY_INACTIVE_STATE);
}

// =========================================================================================
// EVALUASI JADWAL OTOMATIS BERDASARKAN RTC DS1302
// =========================================================================================
void evaluateSchedules() {
  if (!Rtc.IsDateTimeValid()) {
    Serial.println(F("[RTC WARN] Waktu RTC tidak valid!"));
    return;
  }

  uint16_t curMinutes = (nowTime.Hour() * 60) + nowTime.Minute();

  for (int i = 0; i < NUM_CHANNELS; i++) {
    // Hanya proses channel yang berstatus OTOMATIS (mode == 1)
    if (lamps[i].mode != 1) continue;

    uint16_t onMinutes = (lamps[i].on_hour * 60) + lamps[i].on_min;
    uint16_t offMinutes = (lamps[i].off_hour * 60) + lamps[i].off_min;

    bool shouldBeOn = false;

    if (onMinutes == offMinutes) {
      shouldBeOn = false;
    } else if (onMinutes < offMinutes) {
      // Jadwal siang/satu hari (misal 06:00 s/d 18:00)
      shouldBeOn = (curMinutes >= onMinutes && curMinutes < offMinutes);
    } else {
      // Jadwal melewati tengah malam / overnight (misal 18:00 s/d 06:00)
      shouldBeOn = (curMinutes >= onMinutes || curMinutes < offMinutes);
    }

    uint8_t targetState = shouldBeOn ? 1 : 0;
    if (lamps[i].state != targetState) {
      applyRelayHardware(i, targetState);
      Serial.printf("[AUTO] Lampu %d (%s) diatur ke: %s (Jam %02d:%02d)\n", 
                    i + 1, lamps[i].name.c_str(), shouldBeOn ? "ON" : "OFF",
                    nowTime.Hour(), nowTime.Minute());
    }
  }
}

// =========================================================================================
// HANDLER ROUTE WEB SERVER
// =========================================================================================
void handleRoot() {
  server.send_P(200, "text/html", INDEX_HTML);
}

void handleStatus() {
  char timeBuf[16];
  char dateBuf[24];
  
  if (Rtc.IsDateTimeValid()) {
    snprintf(timeBuf, sizeof(timeBuf), "%02u:%02u:%02u", 
             nowTime.Hour(), nowTime.Minute(), nowTime.Second());
    snprintf(dateBuf, sizeof(dateBuf), "%02u/%02u/%04u", 
             nowTime.Day(), nowTime.Month(), nowTime.Year());
  } else {
    snprintf(timeBuf, sizeof(timeBuf), "--:--:--");
    snprintf(dateBuf, sizeof(dateBuf), "RTC Error");
  }

  String json = "{";
  json += "\"time\":\"" + String(timeBuf) + "\",";
  json += "\"date\":\"" + String(dateBuf) + "\",";
  json += "\"uptime\":" + String(millis() / 1000) + ",";
  json += "\"relays\":[";

  for (int i = 0; i < NUM_CHANNELS; i++) {
    json += "{";
    json += "\"pin\":" + String(lamps[i].pin) + ",";
    json += "\"name\":\"" + lamps[i].name + "\",";
    json += "\"mode\":" + String(lamps[i].mode) + ",";
    json += "\"state\":" + String(lamps[i].state) + ",";
    json += "\"on_h\":" + String(lamps[i].on_hour) + ",";
    json += "\"on_m\":" + String(lamps[i].on_min) + ",";
    json += "\"off_h\":" + String(lamps[i].off_hour) + ",";
    json += "\"off_m\":" + String(lamps[i].off_min);
    json += "}";
    if (i < NUM_CHANNELS - 1) json += ",";
  }

  json += "]}";
  server.send(200, "application/json", json);
}

void handleRelayControl() {
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"status\":\"error\",\"msg\":\"Bad Request\"}");
    return;
  }
  String body = server.arg("plain");
  int ch = getJsonInt(body, "ch", -1);

  if (ch < 0 || ch >= NUM_CHANNELS) {
    server.send(400, "application/json", "{\"status\":\"error\",\"msg\":\"Invalid Channel\"}");
    return;
  }

  if (body.indexOf("\"mode\"") >= 0) {
    int mode = getJsonInt(body, "mode", 0);
    lamps[ch].mode = (mode == 1) ? 1 : 0;
    saveChannelMode(ch);
  }

  if (body.indexOf("\"state\"") >= 0) {
    int state = getJsonInt(body, "state", 0);
    lamps[ch].mode = 0; // Manual
    saveChannelMode(ch);
    applyRelayHardware(ch, (state == 1) ? 1 : 0);
  }

  server.send(200, "application/json", "{\"status\":\"ok\"}");
}

void handleSaveSchedule() {
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"status\":\"error\",\"msg\":\"Bad Request\"}");
    return;
  }
  String body = server.arg("plain");
  int ch = getJsonInt(body, "ch", -1);

  if (ch < 0 || ch >= NUM_CHANNELS) {
    server.send(400, "application/json", "{\"status\":\"error\",\"msg\":\"Invalid Channel\"}");
    return;
  }

  String newName = getJsonString(body, "name", lamps[ch].name);
  if (newName.length() > 0) lamps[ch].name = newName;

  lamps[ch].on_hour  = constrain(getJsonInt(body, "on_h", lamps[ch].on_hour), 0, 23);
  lamps[ch].on_min   = constrain(getJsonInt(body, "on_m", lamps[ch].on_min), 0, 59);
  lamps[ch].off_hour = constrain(getJsonInt(body, "off_h", lamps[ch].off_hour), 0, 23);
  lamps[ch].off_min  = constrain(getJsonInt(body, "off_m", lamps[ch].off_min), 0, 59);

  saveChannelSchedule(ch);
  
  if (lamps[ch].mode == 1) {
    evaluateSchedules();
  }

  server.send(200, "application/json", "{\"status\":\"ok\"}");
}

void handleSyncTime() {
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"status\":\"error\",\"msg\":\"Bad Request\"}");
    return;
  }
  String body = server.arg("plain");

  int year  = getJsonInt(body, "year", 2026);
  int month = getJsonInt(body, "month", 1);
  int day   = getJsonInt(body, "day", 1);
  int hour  = getJsonInt(body, "hour", 0);
  int min   = getJsonInt(body, "min", 0);
  int sec   = getJsonInt(body, "sec", 0);

  RtcDateTime compiled = RtcDateTime(year, month, day, hour, min, sec);
  Rtc.SetDateTime(compiled);
  nowTime = compiled;

  Serial.printf("[RTC SYNC] Waktu RTC di-update ke: %04d-%02d-%02d %02d:%02d:%02d\n",
                year, month, day, hour, min, sec);

  evaluateSchedules();

  server.send(200, "application/json", "{\"status\":\"ok\"}");
}

void handleAllAction() {
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"status\":\"error\",\"msg\":\"Bad Request\"}");
    return;
  }
  String body = server.arg("plain");
  String action = getJsonString(body, "action", "");

  if (action == "on") {
    for (int i = 0; i < NUM_CHANNELS; i++) {
      lamps[i].mode = 0;
      saveChannelMode(i);
      applyRelayHardware(i, 1);
    }
  } else if (action == "off") {
    for (int i = 0; i < NUM_CHANNELS; i++) {
      lamps[i].mode = 0;
      saveChannelMode(i);
      applyRelayHardware(i, 0);
    }
  } else if (action == "auto") {
    for (int i = 0; i < NUM_CHANNELS; i++) {
      lamps[i].mode = 1;
      saveChannelMode(i);
    }
    evaluateSchedules();
  }

  server.send(200, "application/json", "{\"status\":\"ok\"}");
}

// =========================================================================================
// SETUP & INITIALIZATION
// =========================================================================================
void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println(F("\n=========================================="));
  Serial.println(F("  SMART IoT 8-CHANNEL LAMP CONTROLLER     "));
  Serial.println(F("=========================================="));

  // 1. Inisialisasi 8 Pin Relay (Active HIGH: Inisialisasi awal ke LOW = MATI)
  for (int i = 0; i < NUM_CHANNELS; i++) {
    pinMode(lamps[i].pin, OUTPUT);
    digitalWrite(lamps[i].pin, RELAY_INACTIVE_STATE);
    lamps[i].state = 0;
  }
  Serial.println(F("[RELAY] 8 Channel diinisialisasi MATI (LOW)."));

  // 2. Inisialisasi RTC DS1302
  Rtc.Begin();
  if (!Rtc.GetIsRunning()) {
    Serial.println(F("[RTC] Mengaktifkan osilator RTC DS1302..."));
    Rtc.SetIsRunning(true);
  }
  if (!Rtc.IsDateTimeValid()) {
    Serial.println(F("[RTC WARN] Jam belum valid, silakan klik 'Sync Jam HP' di Web Dashboard."));
  } else {
    nowTime = Rtc.GetDateTime();
    Serial.printf("[RTC] Waktu awal: %02d:%02d:%02d %02d/%02d/%04d\n",
                  nowTime.Hour(), nowTime.Minute(), nowTime.Second(),
                  nowTime.Day(), nowTime.Month(), nowTime.Year());
  }

  // 3. Muat konfigurasi 8 Channel dari Flash (NVS)
  loadPreferences();

  // 4. Inisialisasi WiFi (Mode Access Point bawaan)
  WiFi.mode(WIFI_AP);
  WiFi.softAP("Smart-Lamp-AP", "12345678");
  IPAddress myIP = WiFi.softAPIP();

  Serial.println(F("[WIFI] Access Point Aktif!"));
  Serial.print(F("       SSID : Smart-Lamp-AP\n"));
  Serial.print(F("       Pass : 12345678\n"));
  Serial.print(F("       Web  : http://"));
  Serial.println(myIP);

  // 5. Daftarkan Endpoint Web Server
  server.on("/", HTTP_GET, handleRoot);
  server.on("/api/status", HTTP_GET, handleStatus);
  server.on("/api/relay", HTTP_POST, handleRelayControl);
  server.on("/api/schedule", HTTP_POST, handleSaveSchedule);
  server.on("/api/sync_time", HTTP_POST, handleSyncTime);
  server.on("/api/all", HTTP_POST, handleAllAction);

  server.begin();
  Serial.println(F("[HTTP] Web Server aktif dan siap melayani permintaan."));

  // Evaluasi jadwal awal saat booting
  if (Rtc.IsDateTimeValid()) {
    evaluateSchedules();
  }
}

// =========================================================================================
// MAIN LOOP
// =========================================================================================
void loop() {
  server.handleClient();

  unsigned long currentMs = millis();

  // Baca RTC setiap 1 detik
  if (currentMs - lastRtcReadMs >= 1000) {
    lastRtcReadMs = currentMs;
    if (Rtc.IsDateTimeValid()) {
      nowTime = Rtc.GetDateTime();
    }
  }

  // Evaluasi Jadwal Otomatis setiap 5 detik
  if (currentMs - lastSchedCheckMs >= 5000) {
    lastSchedCheckMs = currentMs;
    evaluateSchedules();
  }
}
