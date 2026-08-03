#include <ESP8266WiFi.h>
#include <espnow.h>
extern "C" {
#include <user_interface.h>
}

#define LED_BUILTIN_PIN 2 // Onboard LED ESP8266 (GPIO2)

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Struktur Payload 4-Byte (Harus Persis Sama dengan ESP32 Receiver)
typedef struct struct_message {
  int8_t persen;   // Persentase Kelembapan (0 - 100%)
  int8_t baterai;  // Estimasi Baterai Node Kebun (0 - 100%)
  uint16_t rawAdc; // Nilai Mentah Analog ADC (0 - 1024)
} struct_message;

struct_message myData;

volatile int lastSendStatus = -1; // -1: Transmisi, 0: Sukses, 1: Gagal

// CALLBACK ESP-NOW AMAN ANTI-CRASH
void OnDataSent(uint8_t *mac_addr, uint8_t status) { lastSendStatus = status; }

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN_PIN, OUTPUT);
  digitalWrite(LED_BUILTIN_PIN, HIGH); // Off (Active LOW)

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // DONGKRAK DAYA PANCAR RADIO ESP8266 KE MAKSIMAL 20.5 dBm
  WiFi.setOutputPower(20.5);
  wifi_set_phy_mode(
      PHY_MODE_11B);   // MODE 802.11B PERTEMBUS DINDING/POHON (UP TO 300 METER)
  wifi_set_channel(1); // PAKSA KANAL RADIO PHY KE KANAL 1 SAMA DENGAN ESP32!

  if (esp_now_init() != 0) {
    Serial.println(F("Error initializing ESP-NOW"));
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {
  // 1. SAMPLING FILTER (5x Read)
  long sumAdc = 0;
  for (int i = 0; i < 5; i++) {
    sumAdc += analogRead(A0);
    delay(2);
  }
  int sensorValue = sumAdc / 5;

  // 2. KALIBRASI AKURAT SESUAI HARDWARE TANAH ANDA:
  // ADC >= 480 = TANAH KERING (0%), ADC <= 200 = TERGENANG (100%)
  const int dryValue = 480;
  const int wetValue = 200;

  int persenLembab = map(sensorValue, dryValue, wetValue, 0, 100);
  persenLembab = constrain(persenLembab, 0, 100);

  myData.persen = (int8_t)persenLembab;
  myData.baterai = 100; // Daya Catu 3.3V Regulated
  myData.rawAdc = (uint16_t)sensorValue;

  // 3. INDIKATOR KEDIPAN FISIK LED LAPANGAN
  lastSendStatus = -1;
  digitalWrite(LED_BUILTIN_PIN, LOW); // LED Nyala
  esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
  delay(20);
  digitalWrite(LED_BUILTIN_PIN, HIGH); // LED Mati

  Serial.print(F("🌱 [ESP8266 SENDER AKURAT] Raw ADC: "));
  Serial.print(sensorValue);
  Serial.print(F(" | Soil: "));
  Serial.print(persenLembab);
  Serial.print(F("%"));

  if (lastSendStatus == 0) {
    Serial.println(F(" | Status Transmisi: ✅ SUKSES MENDARAT"));
  } else if (lastSendStatus > 0) {
    Serial.println(F(" | Status Transmisi: ❌ GAGAL"));
  } else {
    Serial.println(F(" | Status Transmisi: ⚡ MEMANCARKAN..."));
  }

  delay(980); // Total Delay 1000 ms (1 Detik)
}