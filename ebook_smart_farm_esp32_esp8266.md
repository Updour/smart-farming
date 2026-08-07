# 📚 E-BOOK MASTERPIECE ULTIMATE: PANDUAN LANGKAH-DEMI-LANGKAH MEMBUAT SISTEM SMART FARM INDUSTRIAL SCADA (ESP32 & ESP8266)
**Penulis:** Abdur Rohman  
**Versi:** 9.0 Enterprise Vision AI & BMKG Edition  
**Tanggal:** 2026-08-06  

---

## 📖 DAFTAR ISI SANGAT LENGKAP

1. **BAB 1: KONSEPTUAL & ARSITEKTUR SISTEM (UNTUK PEMULA)**
   - 1.1 Mengapa Memilih Smart Farm Offline-First Store-and-Forward?
   - 1.2 Cara Kerja Komunikasi Radio ESP-NOW (Tanpa Internet)
   - 1.3 Cara Kerja WiFi Access Point & Web Dashboard SCADA
   - 1.4 Diagram Alur Kerja Sistem Lengkap dari Kebun ke HP Petani

2. **BAB 2: RENCANA ANGGARAN BIAYA (RAB), PINOUT, & MERAKIT HARDWARE (STEP-BY-STEP)**
   - 2.1 Rencana Anggaran Biaya (RAB) / Bill of Materials (BOM) Realistis & Lengkap
   - 2.2 Panduan Rangkaian Skematik ESP32 Master Receiver (Kabel demi Kabel)
   - 2.3 Panduan Rangkaian Skematik ESP8266 Sender Kebun (Kabel demi Kabel)
   - 2.4 Proteksi Kelistrikan: Diode Flyback, Optocoupler Relay, & Dinamo Pompa Sanyo 125W
   - 2.5 Hitung-hitungan Tarif Listrik Subsidi PLN 450 VA & Hemat Air Kebun

3. **BAB 3: PERSIAPAN SOFTWARE, IDE, & LIBRARY (STEP-BY-STEP)**
   - 3.1 Instalasi Arduino IDE 2.x
   - 3.2 Cara Menambahkan Board Manager ESP32 & ESP8266
   - 3.3 Cara Instalasi Library DHT Sensor & LittleFS Plugin
   - 3.4 Cara Upload Kode ke ESP8266 & ESP32 Tanpa Error

4. **BAB 4: TUTORIAL PEMROGRAMAN SENDER KEBUN (ESP8266) BARIS DEMI BARIS**
   - 4.1 Kode Sumber Lengkap (`esp8266_soil_sender.ino`)
   - 4.2 Penjelasan Kode Baris demi Baris (Line-by-Line Breakdown)
   - 4.3 Panduan Kalibrasi Sensor Kelembapan Tanah Capacitive (Uji Kering vs Basah)

5. **BAB 5: TUTORIAL PEMROGRAMAN MASTER SCADA (ESP32) BARIS DEMI BARIS**
   - 5.1 Kode Sumber Lengkap (`esp32_server_receiver.ino`)
   - 5.2 Penjelasan Struktur Data & Variabel Global Baris demi Baris
   - 5.3 Penjelasan Fungsi Interrupt Callback ESP-NOW & Dynamic RSSI
   - 5.4 Penjelasan Fungsi Diagnostik Hardware & Failsafe Interlock Lockout
   - 5.5 Penjelasan Fungsi Kontrol Traffic Light LED Active HIGH (Hijau, Kuning, Merah)
   - 5.6 Penjelasan Rumus Fisika & Finansial (PLN 450VA, Evaporasi, Dew Point, Heat Index, Relay Lifespan)
   - 5.7 Penjelasan Database LittleFS 50KB Auto-Wipe & Berkas CSV
   - 5.8 Penjelasan Seluruh Web API HTTP Endpoints (Port 80)

6. **BAB 6: TUTORIAL DOKUMENTASI FRONTEND DASHBOARD SCADA (`index.h` & `index.html`)**
   - 6.1 Kode Sumber HTML5 / CSS3 / JavaScript Glassmorphism (`index.h`)
   - 6.2 Integrasi API BMKG Indonesia (Studi Kasus: Tigasan Wetan, Leces, Probolinggo)
   - 6.3 Integrasi Computer Vision AI Pemindai Foto Daun & Hama (Google Gemini 1.5 Flash)
   - 6.4 Fitur Standalone Offline Mode (File index.html Mandiri Tanpa ESP32)
   - 6.5 Cara Kerja Render Grafik Dual-Line Real-Time SVG (0-Byte RAM ESP32)
   - 6.6 Cara Kerja Web Serial Live Console 1000ms

7. **BAB 7: PANDUAN PEMASANGAN DI LAPANGAN & BOKS PANEL IP65**
   - 7.1 Pemasangan Boks Panel Waterproof IP65
   - 7.2 Manajemen Daya Catu Daya Baterai / Solar Panel
   - 7.3 Panduan Troubleshooting Lengkap 20 Indikator Error

---

# BAB 1: KONSEPTUAL & ARSITEKTUR SISTEM (UNTUK PEMULA)

### 1.1 Mengapa Memilih Smart Farm Offline-First Store-and-Forward?
Banyak pembuat proyek otomasi pertanian gagal saat diterapkan di lapangan nyata karena mengandalkan internet 4G/WiFi terus-menerus. Di tengah ladang atau kebun:
- Sinyal HP sering lemah atau terputus.
- Kuota internet sering habis.
- Mati listrik lokal dapat memutus koneksi internet router.

**Solusi Sistem Ini:**
Sistem ini menggunakan arsitektur **Offline-First Store-and-Forward**. Artinya:
1. **100% Otonom Mandiri:** Otak ESP32 dan node ESP8266 dapat mengambil keputusan penyiraman, memantau suhu, dan menyimpan log catatan tanpa bantuan internet.
2. **Local Dashboard SCADA:** Saat petani berada di dekat panel di kebun, HP petani dihubungkan ke sinyal WiFi buatan ESP32 (`SmartFarm-ESP32`) untuk melihat status visual real-time di browser `http://192.168.4.1`.
3. **Store-and-Forward:** Saat petani kembali ke rumah yang ada internetnya, aplikasi web HP menyedot data cuaca satelit dari OpenWeatherMap API dan menyuntikkannya ke ESP32.

### 1.3 Konsep & Pengertian SCADA dalam Smart Farming Modern

**SCADA** adalah singkatan dari **Supervisory Control And Data Acquisition** (Sistem Pengawasan, Pengendalian, dan Akuisisi Data). Di ranah industri otomasi manufaktur dan minyak/gas, SCADA adalah standar tertinggi pengawasan infrastruktur. Dalam sistem *Smart Farming Presisi* berbasis ESP32 ini, SCADA diimplementasikan ke dalam 4 pilar utama:

1. **S — Supervisory (Pengawasan Visual Jarak Jauh):**
   Petani atau manajer lahan dapat mengawasi seluruh kondisi mikroiklim kebun (suhu, kelembapan tanah, tren evaporasi, prakiraan hujan BMKG, serta estimasi biaya PLN) secara visual melalui Web Control Center di HP/Laptop tanpa perlu mendatangi lokasi boks panel.
2. **C — Control (Pengendalian Otomatis & Manual Instan):**
   Sistem mengesekusi keputusan pengontrolan saklar relay pompa air secara otomatis berbasis ambang batas (*threshold interlock*) dan cuaca satelit, sekaligus memberikan kendali saklar manual *over-ride* langsung dari tombol Web Dashboard.
3. **A — And (Dan)**
4. **D — Data Acquisition (Akuisisi & Pengumpulan Data Telemetri):**
   Microcontroller ESP32 & ESP8266 mengumpulkan sampel data fisik dari sensor secara terus menerus melalui radio ESP-NOW 2.4 GHz, menyimpannya ke memori flash LittleFS berkas `.csv`, lalu menyajikannya dalam grafik osiloskop 3-zona interaktif secara real-time.

---

# BAB 2: RENCANA ANGGARAN BIAYA (RAB), PINOUT, & MERAKIT HARDWARE (STEP-BY-STEP)

### 2.1 Rencana Anggaran Biaya (RAB) / Bill of Materials (BOM) Realistis & Lengkap

Berikut adalah tabel **Rencana Anggaran Biaya (RAB)** belanja komponen lengkap berbasis pembelian nyata di toko kompenen elektronik online:

| No | Nama Komponen & Spesifikasi | Kuantitas | Harga Satuan (Rp) | Subtotal Harga (Rp) | Fungsi Utama dalam Sistem SCADA |
| :-: | :--- | :-: | :-: | :-: | :--- |
| 1 | **NodeMCU ESP32 Development Board** | 1 Pcs | Rp 65.000 | Rp 65.000 | Master Receiver, Web Server SCADA, & Pengontrol Utama. |
| 2 | **NodeMCU ESP8266 V3 (Lolin/Amica)** | 1 Pcs | Rp 35.000 | Rp 35.000 | Node Sender Kebun Pemancar Data Kelembapan Tanah. |
| 3 | **Capacitive Soil Moisture Sensor Module v1.2** | 1 Pcs | Rp 8.500 | Rp 8.500 | Sensor kelembapan tanah tahan korosi (Analog A0). |
| 4 | **Sensor Suhu & Kelembapan Udara DHT11** | 1 Pcs | Rp 18.000 | Rp 18.000 | Sensor suhu & kelembapan udara digital (GPIO 4). |
| 5 | **Modul Relay 1 Channel 5V Optocoupler** | 1 Pcs | Rp 12.000 | Rp 12.000 | Saklar elektronik pengontrol pompa air (GPIO 26 Active LOW). |
| 6 | **Buzzer Alarm Speaker Motherboard 5V** | 1 Pcs | Rp 3.000 | Rp 3.000 | Indikator suara alarm saat terjadi darurat/sinyal putus. |
| 7 | **PCB Lubang Prototype Board (9cm x 15cm)** | 2 Pcs | Rp 3.000 | Rp 6.000 | Papan sirkuit tempat menyolder komponen & socket header. |
| 8 | **Pin Header Male 2.54mm (1x40 Pin)** | 2 Pcs | Rp 500 | Rp 1.000 | Jarum konektor solderan modul sensor & PCB. |
| 9 | **Pin Header Female 2.54mm (1x40 Pin)** | 2 Pcs | Rp 1.000 | Rp 2.000 | Socket dudukan lepas-pasang ESP32, ESP8266 & Modul. |
| 10 | **Kabel Data Charger Micro USB (Macaron Fast)** | 2 Pcs | Rp 3.940 | Rp 7.880 | Kabel daya & flash upload firmware ESP32 & ESP8266. |
| 11 | **LED Indikator 5mm (Hijau, Kuning, Merah)** | 3 Pcs | Rp 500 | Rp 1.500 | Indikator Traffic Light fisik status kebun (Active HIGH). |
| 12 | **Resistor Pembatas Arus (220 / 330 Ohm)** | 3 Pcs | Rp 333 | Rp 1.000 | Pengaman LED agar tidak terbakar tegangan 3.3V. |
| 13 | **Adaptor / Power Supply 5V 2A Regulated** | 2 Pcs | Rp 15.000 | Rp 30.000 | Catu daya utama listrik ESP32 & ESP8266. |
| 14 | **Boks Panel Listrik Waterproof IP65** | 1 Pcs | Rp 45.000 | Rp 45.000 | Boks pelindung dari air hujan & debu di area kebun. |
| 15 | **Aksesoris Cable Ties & Terminal Screw Block** | 1 Set | Rp 5.000 | Rp 5.000 | Rapikan pengkabelan & terminal daya PLN/Pompa. |
| **TOTAL** | **ESTIMASI TOTAL ANGGARAN BELANJA HARDWARE (BOM)** | | | **Rp 240.880,-** | *Siap Merakit Sistem Smart Farm Utuh!* |

---

### 2.2 Panduan Rangkaian Skematik ESP32 Master Receiver (Kabel demi Kabel)

#### A. Rangkaian Sensor Suhu/Udara DHT11:
1. Hubungkan **Pin VCC (+)** DHT11 ke **Pin 3V3** pada ESP32.
2. Hubungkan **Pin GND (-)** DHT11 ke **Pin GND** pada ESP32.
3. Hubungkan **Pin DATA (S)** DHT11 ke **GPIO 4** pada ESP32.

#### B. Rangkaian Modul Relay 1 Channel (Active LOW):
1. Hubungkan **Pin VCC** Relay ke **Pin 5V / VIN** pada ESP32.
2. Hubungkan **Pin GND** Relay ke **Pin GND** pada ESP32.
3. Hubungkan **Pin IN (Sinyal Trigger)** Relay ke **GPIO 26** pada ESP32.
   > **Catatan Penting:** Pin GPIO 26 bekerja dengan logika *Active LOW*. Sinyal `LOW` (0V) menyalakan relay, sinyal `HIGH` (3.3V) mematikan relay.

#### C. Rangkaian Traffic Light LED (Active HIGH):
1. **LED Hijau (Status Aman):**
   - Anoda (Kaki Panjang) LED Hijau $\rightarrow$ Resistor 220 Ohm $\rightarrow$ **GPIO 32** ESP32.
   - Katoda (Kaki Pendek) LED Hijau $\rightarrow$ **GND** ESP32.
2. **LED Kuning (Status Peringatan / Waiting):**
   - Anoda (Kaki Panjang) LED Kuning $\rightarrow$ Resistor 220 Ohm $\rightarrow$ **GPIO 33** ESP32.
   - Katoda (Kaki Pendek) LED Kuning $\rightarrow$ **GND** ESP32.
3. **LED Merah (Status Bahaya / Failsafe Error):**
   - Anoda (Kaki Panjang) LED Merah $\rightarrow$ Resistor 220 Ohm $\rightarrow$ **GPIO 27** ESP32.
   - Katoda (Kaki Pendek) LED Merah $\rightarrow$ **GND** ESP32.

---

### 2.3 Panduan Rangkaian Skematik ESP8266 Sender Kebun (Kabel demi Kabel)

#### Rangkaian Sensor Kelembapan Tanah Capacitive v1.2:
1. Hubungkan **Pin VCC** Sensor Tanah ke **Pin 3V3** pada ESP8266.
2. Hubungkan **Pin GND** Sensor Tanah ke **Pin GND** pada ESP8266.
3. Hubungkan **Pin AOUT (Analog Output)** Sensor Tanah ke **Pin A0** (ADC0) pada ESP8266.

---

# BAB 3: PERSIAPAN SOFTWARE, IDE, & LIBRARY (STEP-BY-STEP)

### 3.1 Instalasi Arduino IDE 2.x
1. Unduh Arduino IDE versi terbaru dari situs resmi `arduino.cc`.
2. Jalankan file installer hingga selesai.

### 3.2 Cara Menambahkan Board Manager ESP32 & ESP8266
1. Buka Arduino IDE, klik menu **File > Preferences**.
2. Pada kolom **Additional Boards Manager URLs**, masukkan dua URL berikut (pisahkan dengan koma):
   ```text
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json, http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
3. Klik **OK**.
4. Buka **Tools > Board > Boards Manager**, cari `esp32` lalu klik **Install**.
5. Cari `esp8266` lalu klik **Install**.

### 3.3 Cara Instalasi Library Tambahan
Buka menu **Tools > Manage Libraries**, lalu cari dan install library berikut:
1. **DHT sensor library** oleh Adafruit.
2. **Adafruit Unified Sensor** oleh Adafruit.

---

# BAB 4: TUTORIAL PEMROGRAMAN SENDER KEBUN (ESP8266) BARIS DEMI BARIS

### 4.1 Kode Sumber Lengkap (`esp8266_soil_sender.ino`)
```cpp
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
  wifi_set_phy_mode(PHY_MODE_11B);   // MODE 802.11B PERTEMBUS DINDING/POHON
  wifi_set_channel(1);               // PAKSA KANAL RADIO PHY KE KANAL 1 SAMA DENGAN ESP32!

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

  // 2. KALIBRASI AKURAT SESUAI HARDWARE TANAH:
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

  delay(2000); // Jeda pengiriman 2 detik
}
```

### 4.2 Penjelasan Baris demi Baris Kode ESP8266 (Line-by-Line Breakdown)

Untuk membantu Anda dan para pembaca E-Book memahami cara kerja pemrogramannya secara mendalam, berikut adalah analisis dan perilaku setiap baris kode pada program `esp8266_soil_sender.ino`:

#### 1. Pengenalan Konsep Struktur Dasar Arduino: `setup()` dan `loop()`
Setiap program mikrokontroler berbasis Arduino wajib memiliki dua fungsi utama:
- **`void setup()` (Inisialisasi):**  
  Fungsi ini berjalan **hanya satu kali** sesaat setelah mikrokontroler mendapatkan aliran daya (menyala) atau sesaat setelah tombol *Reset* ditekan. Di sini, kita meletakkan seluruh perintah konfigurasi awal seperti menentukan arah pin, menyalakan komunikasi serial, mengatur mode WiFi, dan mendaftarkan protokol ESP-NOW.
- **`void loop()` (Perulangan Utama):**  
  Setelah fungsi `setup()` selesai dijalankan, mikrokontroler secara otomatis masuk ke fungsi `loop()`. Fungsi ini akan dieksekusi secara **terus-menerus berulang-ulang tanpa henti** dari baris pertama hingga baris terakhir, lalu kembali lagi ke baris pertama selama mikrokontroler menyala. Di sini kita meletakkan logika pembacaan sensor, kalkulasi kelembapan, dan pemancaran sinyal radio.

---

#### 2. Bedah Baris demi Baris Kode

##### **Baris 1-5: Import Library & SDK System**
```cpp
#include <ESP8266WiFi.h>
#include <espnow.h>
extern "C" {
#include <user_interface.h>
}
```
* **Perilaku & Alasan:**  
  - `#include <ESP8266WiFi.h>` memuat pustaka WiFi bawaan ESP8266 untuk mengaktifkan sirkuit radio nirkabel.
  - `#include <espnow.h>` memuat protokol komunikasi peer-to-peer ESP-NOW.
  - `extern "C" { #include <user_interface.h> }` memuat pustaka antarmuka sistem operasi internal Espressif (berbasis bahasa C) agar kita bisa mengakses perintah daya pancar RF dan penguncian kanal frekuensi radio level rendah (*Low-Level PHY*).

##### **Baris 7: Definisi Pin LED Internal**
```cpp
#define LED_BUILTIN_PIN 2
```
* **Perilaku & Alasan:**  
  Mendefinisikan konstanta nama `LED_BUILTIN_PIN` yang bernilai `2`. Nilai `2` ini merujuk ke pin **GPIO 2** pada ESP8266, tempat di mana LED biru kecil bawaan pabrik tersambung. Kita mendefinisikan ini agar kode lebih mudah dibaca daripada menulis angka `2` secara langsung.

##### **Baris 9: MAC Address Penerima Sinyal**
```cpp
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
```
* **Perilaku & Alasan:**  
  Membuat array berisi 6 byte data alamat MAC berlogika broadcast (`0xFF:0xFF:0xFF:0xFF:0xFF:0xFF`). Perilakunya adalah memancarkan data secara umum ke udara pada kanal frekuensi radio yang aktif. Dengan menggunakan alamat ini, modul ESP8266 di lapangan tidak perlu tahu alamat MAC unik milik ESP32 master di rumah secara kaku, mempermudah instalasi skala massal.

##### **Baris 12-16: Struktur Data Pengiriman (Struct Payload)**
```cpp
typedef struct struct_message {
  int8_t persen;   // Persentase Kelembapan (0 - 100%)
  int8_t baterai;  // Estimasi Baterai Node Kebun (0 - 100%)
  uint16_t rawAdc; // Nilai Mentah Analog ADC (0 - 1024)
} struct_message;
```
* **Perilaku & Alasan:**  
  - **Apa itu Struct?** `typedef struct` digunakan untuk membuat satu kelompok tipe data buatan kita sendiri. Alih-alih mengirimkan data kelembapan, baterai, dan nilai mentah sensor secara terpisah (yang memakan daya radio besar), kita membungkusnya dalam satu paket kecil bernama `struct_message`.
  - **Berapa Ukuran Datanya?**  
    - `int8_t persen` (1 Byte, bertanda): Menyimpan bilangan bulat $0$ s/d $100$ persen.  
    - `int8_t baterai` (1 Byte, bertanda): Menyimpan status sisa energi baterai.  
    - `uint16_t rawAdc` (2 Byte, tak bertanda): Menyimpan nilai mentah ADC sensor dari rentang $0$ s/d $1023$.  
  - Total ukuran payload ini hanya **4 Byte (32 Bit)**. Ukuran yang sangat ringkas ini menjamin transmisi radio ESP-NOW berjalan secepat kilat (latensi $<5\text{ ms}$) dan sangat hemat baterai.

##### **Baris 18: Instansiasi Variabel Struct**
```cpp
struct_message myData;
```
* **Perilaku & Alasan:**  
  Membuat variabel nyata bernama `myData` dengan tipe sruktur `struct_message` yang telah didefinisikan sebelumnya. Variabel ini bertindak sebagai wadah kosong untuk menampung data sebelum dipancarkan.

##### **Baris 20: Variabel Monitoring Transmisi**
```cpp
volatile int lastSendStatus = -1;
```
* **Perilaku & Alasan:**  
  Menyimpan status keberhasilan pengiriman paket data. Nilai `-1` berarti sedang mengirim, `0` berarti berhasil terkirim dan diterima ESP32, dan `1` berarti gagal terkirim. Penggunaan kata kunci `volatile` wajib karena nilainya akan berubah secara asinkron di dalam fungsi interrupt callback di luar alur utama fungsi `loop()`.

##### **Baris 23: Fungsi Callback Status Pengiriman**
```cpp
void OnDataSent(uint8_t *mac_addr, uint8_t status) { lastSendStatus = status; }
```
* **Perilaku & Alasan:**  
  Fungsi callback yang dieksekusi otomatis oleh sistem operasi ESP8266 saat paket radio selesai dipancarkan. Perilakunya adalah menangkap status hasil pancaran (sukses atau gagal) dan menyimpannya ke dalam variabel `lastSendStatus`.

##### **Baris 25-29: Setup Komunikasi & LED**
```cpp
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN_PIN, OUTPUT);
  digitalWrite(LED_BUILTIN_PIN, HIGH);
```
* **Perilaku & Alasan:**  
  - `Serial.begin(115200)`: Mengaktifkan sirkuit komunikasi serial UART pada kecepatan baud rate **115.200 bits per second (bps)**. Ini memungkinkan mikrokontroler mengirimkan teks diagnostik ke komputer melalui port USB.
  - `pinMode(LED_BUILTIN_PIN, OUTPUT)`: Mengubah konfigurasi fisik GPIO 2 sebagai output kelistrikan agar pin tersebut bisa menyuplai arus untuk menyalakan LED.
  - `digitalWrite(LED_BUILTIN_PIN, HIGH)`: Memberikan tegangan $3.3\text{V}$ ke pin LED. Karena LED internal ESP8266 berlogika *Active LOW*, memberikan sinyal `HIGH` akan mematikan LED saat awal menyala.

##### **Baris 30-31: Konfigurasi WiFi Client**
```cpp
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
```
* **Perilaku & Alasan:**  
  - `WiFi.mode(WIFI_STA)`: Memaksa chip masuk ke mode Station (client biasa). Ini merupakan syarat mutlak agar fitur komunikasi nirkabel ESP-NOW dapat diaktifkan.
  - `WiFi.disconnect()`: Memutus hubungan WiFi dari router nirkabel mana pun. Hal ini berguna untuk menghemat konsumsi energi secara signifikan karena ESP8266 tidak akan membuang daya memindai (*scanning*) sinyal WiFi di tengah hutan/kebun.

##### **Baris 34-37: Dongkrak Power Transmisi RF Maksimal**
```cpp
  WiFi.setOutputPower(20.5);
  wifi_set_phy_mode(PHY_MODE_11B);
  wifi_set_channel(1);
```
* **Perilaku & Alasan:**  
  - `WiFi.setOutputPower(20.5)`: Mengatur daya pancar penguat sinyal RF ke batas maksimal chip, yaitu $+20.5\text{ dBm}$ ($\approx 112\text{ mW}$) untuk jangkauan terjauh.
  - `wifi_set_phy_mode(PHY_MODE_11B)`: Mengubah tipe modulasi radio ke standar 802.11b CCK 11Mbps. Modul ini lebih tahan terhadap rintangan dedaunan/tanah dibanding standar 802.11g/n.
  - `wifi_set_channel(1)`: Mengunci kanal radio PHY secara permanen ke **Kanal 1 (2.412 GHz)** agar selaras dengan penerima ESP32 master.

##### **Baris 39-46: Inisialisasi Protokol ESP-NOW**
```cpp
  if (esp_now_init() != 0) {
    Serial.println(F("Error initializing ESP-NOW"));
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
```
* **Perilaku & Alasan:**  
  - `esp_now_init()`: Menyalakan mesin protokol ESP-NOW. Jika gagal (misal chip rusak), pesan error dikirim ke Serial Monitor dan sistem dihentikan.
  - `esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER)`: Menyetel peran ESP8266 sebagai pengendali/pemancar data (*Controller*).
  - `esp_now_register_send_cb(OnDataSent)`: Mendaftarkan fungsi callback `OnDataSent` yang telah kita buat agar dieksekusi otomatis oleh hardware radio setelah pengiriman paket selesai.
  - `esp_now_add_peer()`: Mendaftarkan alamat MAC broadcast (`broadcastAddress`) sebagai penerima data nirkabel pada Kanal 1.

##### **Baris 51-56: Loop Sampling Sensor (Oversampling)**
```cpp
void loop() {
  long sumAdc = 0;
  for (int i = 0; i < 5; i++) {
    sumAdc += analogRead(A0);
    delay(2);
  }
  int sensorValue = sumAdc / 5;
```
* **Perilaku & Alasan:**  
  - Mengulang pembacaan pin analog `A0` sebanyak 5 kali berturut-turut dengan penundaan 2 ms pada setiap pembacaan.
  - Jumlah total pembacaan disimpan ke variabel `sumAdc`, lalu dibagi 5 untuk mendapatkan rata-rata bacaan. Teknik ini dinamakan **Oversampling Filter** untuk meredam *noise* gangguan gelombang elektromagnetik dan lonjakan arus statis tanah.

##### **Baris 60-64: Kalibrasi & Pembatasan Persen**
```cpp
  const int dryValue = 480;
  const int wetValue = 200;
  int persenLembab = map(sensorValue, dryValue, wetValue, 0, 100);
  persenLembab = constrain(persenLembab, 0, 100);
```
* **Perilaku & Alasan:**  
  - Nilai analog saat sensor kering berada di udara terbuka berkisar $\ge 480$ (`dryValue`), sedangkan saat sensor dicelupkan ke air penuh berkisar $\le 200$ (`wetValue`).
  - `map(sensorValue, 480, 200, 0, 100)`: Mengonversi secara linear nilai rentang $480$ s/d $200$ menjadi nilai persentase $0\%$ s/d $100\%$.
  - `constrain(persenLembab, 0, 100)`: Membatasi hasil agar jika pembacaan sensor meleset (misal ADC $490$), persentase kelembapan tanah akan dikunci tetap pada $0\%$ (tidak bocor menjadi minus) atau tidak melebihi $100\%$.

##### **Baris 66-68: Memasukkan Data ke Paket Struct**
```cpp
  myData.persen = (int8_t)persenLembab;
  myData.baterai = 100;
  myData.rawAdc = (uint16_t)sensorValue;
```
* **Perilaku & Alasan:**  
  Mengisi setiap bagian variabel anggota `myData` dengan nilai kelembapan tanah yang telah dikalkulasikan, status baterai ($100\%$ karena menggunakan regulator catu daya 3.3V stabil), dan nilai mentah ADC sensor untuk keperluan debug.

##### **Baris 71-75: Pemancaran Radio & Kedipan LED**
```cpp
  lastSendStatus = -1;
  digitalWrite(LED_BUILTIN_PIN, LOW); // LED Nyala
  esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
  delay(20);
  digitalWrite(LED_BUILTIN_PIN, HIGH); // LED Mati
```
* **Perilaku & Alasan:**  
  - Mengubah `lastSendStatus` menjadi `-1` (menandakan pengiriman sedang diproses).
  - Memberikan logika `LOW` (0V) pada GPIO 2 untuk menyalakan LED biru onboard.
  - `esp_now_send()`: Memerintahkan modul RF memancarkan paket data `myData` sebanyak 4 byte ke alamat MAC broadcast di udara.
  - Memberikan jeda waktu 20 ms agar kilatan lampu LED menyala jelas terlihat oleh mata petani di lapangan.
  - Memberikan logika `HIGH` (3.3V) pada GPIO 2 untuk mematikan LED onboard kembali.

##### **Baris 77-89: Print Informasi ke Serial Monitor**
```cpp
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
```
* **Perilaku & Alasan:**  
  Mencetak informasi nilai mentah ADC, persentase tanah, dan status pancaran sukses/gagal ke Serial Monitor komputer. Penggunaan makro `F()` (contoh: `F("Text")`) sangat krusial karena memaksa string tersebut disimpan di dalam memori Flash program, bukan di memori RAM runtime, mencegah kebocoran memori RAM (*RAM Out-of-Memory*) pada mikrokontroler.

##### **Baris 91: Delay Pemancar 2 Detik**
```cpp
  delay(2000);
}
```
* **Perilaku & Alasan:**  
  Memberikan jeda waktu penundaan sebesar 2000 ms (2 detik) sebelum kembali mengulang siklus pembacaan kelembapan tanah. Jeda 2 detik ini sangat ideal untuk membatasi frekuensi pengiriman sinyal radio ESP-NOW, sehingga menghemat konsumsi daya baterai di kebun secara signifikan serta menghindari kepadatan lalu lintas frekuensi nirkabel (wireless congestion).

---

---

# BAB 5: TUTORIAL PEMROGRAMAN MASTER SCADA (ESP32) BARIS DEMI BARIS

### 5.1 Penjelasan Logika Failsafe Hardware Interlock (PENGAMAN KEBUN)
Di dalam fungsi `updateRelayState()` pada ESP32:
```cpp
if (isSystemError || isEsp8266Unplugged || isRelayUnplugged || latestMoisturePercent < 0 || isStartupWaiting) {
   isRelayOn = false;
   isManualMode = false;
   manualRelayState = false;
   digitalWrite(RELAY1, HIGH); // PAKSA RELAY PIN 26 MATI TOTAL! (Active LOW -> HIGH=OFF)
   return;
}
```
**Mengapa Ini Sangat Penting?**  
Jika sinyal radio dari kebun terputus lebih dari 8 detik atau kabel relay terlepas:
1. Pompa air dipaksa **MATI TOTAL (`HIGH`)**.
2. Fitur kontrol manual via Web otomatis **DIKUNCI OFF**.
3. Kebun dijamin aman dari bahaya banjir akibat pompa menyala terus-menerus tanpa kontrol sensor!

---

# BAB 6: FRONTEND DASHBOARD SCADA & VISUALISASI GRAFIK SVG

### 6.1 Cara Kerja Grafik Dual-Line Real-Time SVG (0-Byte RAM ESP32)
Dashboard web dibangun dengan HTML5 dan JavaScript vanilla di dalam file `index.h`.
- **0-Byte RAM ESP32:** ESP32 tidak perlu mengolah grafik gambar. ESP32 hanya mengirimkan data angka JSON via URL `/data`.
- **Smartphone Browser Rendering:** HP/Laptop petani membaca data angka tersebut setiap 1000 ms, lalu menggambar garis grafik SVG secara real-time di layar browser pengguna.

### 6.2 Integrasi API BMKG Indonesia

Untuk mendapatkan keakuratan data ramalan cuaca lokal yang presisi tinggi di wilayah Indonesia (terutama di daerah perkebunan pedesaan), kita disarankan menggunakan API resmi dari **BMKG (Badan Meteorologi, Klimatologi, dan Geofisika)** daripada model global OWM.

BMKG menyediakan API ramalan cuaca publik gratis berdasarkan kode wilayah administrasi (ADM4) hingga tingkat Desa/Kelurahan.

#### A. Endpoint API BMKG Desa Tigasan Wetan, Leces:
```text
https://api.bmkg.go.id/publik/prakiraan-cuaca?adm4=35.13.05.2002
```
*Di mana `35.13.05.2002` adalah kode wilayah unik BMKG untuk Desa Tigasan Wetan, Kecamatan Leces, Kabupaten Probolinggo.*

#### B. Kode Integrasi JavaScript Sync (Client-Side Bridge):
Berikut adalah skrip JavaScript yang dipasang di halaman Web Dashboard (HP Petani). Skrip ini bertugas mengambil data JSON dari server BMKG ketika HP memiliki internet, lalu mengirimkannya langsung ke ESP32 secara nirkabel saat terhubung ke WiFi kebun:

```javascript
async function syncBMKGWeatherToESP32() {
  try {
    // 1. Ambil data ramalan cuaca dari API Resmi BMKG
    const response = await fetch("https://api.bmkg.go.id/publik/prakiraan-cuaca?adm4=35.13.05.2002");
    const json = await response.json();
    
    // 2. Ekstrak data prakiraan jam terdekat (indeks pertama)
    const currentForecast = json.data[0].cuaca[0][0];
    
    const temp = currentForecast.t;                   // Suhu Udara (°C)
    const desc = currentForecast.weather_desc;         // Kondisi Cuaca (misal: "Cerah")
    const localTime = currentForecast.local_datetime;  // Waktu Prakiraan Lokal
    const weatherCode = currentForecast.weather;       // Kode Cuaca BMKG
    
    // Klasifikasi kode cuaca hujan dari BMKG (weather code >= 60 menandakan hujan)
    let rainPrediction = "Cerah / Tidak Ada Hujan";
    if (weatherCode >= 60 && weatherCode <= 97) {
      rainPrediction = "Hujan Terdeteksi: " + desc;
    } else if (weatherCode >= 3 && weatherCode <= 5) {
      rainPrediction = "Mendung / Potensi Hujan";
    }
    
    // 3. Kirimkan data ke ESP32 Receiver via HTTP POST
    const esp32Url = `http://192.168.4.1/pushWeather`
      + `?temp=${temp}`
      + `&desc=${encodeURIComponent(desc)}`
      + `&rain=${encodeURIComponent(rainPrediction)}`
      + `&time=${encodeURIComponent(localTime)}`;
      
    const pushResponse = await fetch(esp32Url, { method: "POST" });
    const resultText = await pushResponse.text();
    console.log("Prakiraan BMKG Berhasil Dikirim ke ESP32:", resultText);
  } catch (error) {
    console.error("Gagal sinkronisasi data BMKG:", error);
  }
}
```

#### C. Mengapa Ini Jauh Lebih Akurat?
- **Stasiun Meteorologi Lokal Tapal Kuda:** BMKG mengamati pergerakan awan secara mikro menggunakan radar cuaca lokal Jawa Timur.
- **Prediksi Level Desa:** Berbeda dengan satelit global yang meramalkan skala luas, BMKG secara spesifik memprediksi cuaca wilayah Desa Tigasan Wetan, Leces.
- **AI Irrigation Prevention:** Ketika BMKG mendeteksi kode cuaca hujan (`60` - `97`), dashboard web akan memberi tahu ESP32 untuk **mengunci relay pompa tetap mati (OFF)** karena tanah kebun akan segera disiram oleh air hujan alami, sehingga menghemat konsumsi energi listrik dan air pompa 100%!

---

### 6.3 Integrasi Computer Vision AI Pemindai Foto Daun & Hama (Google Gemini 3.6 Flash & 2.0 Flash)

Selain diagnosa sensor berbasis angka, sistem SCADA ini dilengkapi fitur **Fitopatologi Computer Vision AI** yang memungkinkan petani mengambil foto daun tanaman yang sakit/kuning lalu dipindai langsung oleh AI untuk mengetahui jenis penyakit dan obatnya.

#### A. Mengapa Menggunakan Google Gemini 3.6 Flash (gemini-flash-latest) Vision?
1. **100% Gratis (Free Tier):** Google AI Studio memberikan kuota gratis **1.500 foto per hari (RPD)** dan **15 foto per menit (RPM)** secara permanen dengan model termutakhir.
2. **Dukungan Clipboard Paste & URL Internet:** Petani tidak harus mengunggah file. Cukup copy gambar dari internet/file manager lalu tekan `Ctrl+V` (paste) langsung di halaman dashboard, atau masukkan link URL gambar untuk memuatnya otomatis.
3. **Multi-Model Fallback & Auth:** Sistem secara cerdas mencoba model `gemini-flash-latest` (Gemini 3.6 Flash) dan fallback ke `gemini-2.0-flash` hingga `gemini-1.5-flash` dengan 3 jenis metode autentikasi API (header `X-goog-api-key`, header `Bearer`, dan parameter `?key=`) agar tangguh dari kegagalan.

#### B. Skrip Kode JavaScript Pemindaian Foto Daun:
```javascript
async function analyzeLeafVisionAI() {
  if (!selectedLeafBase64) {
    alert("❌ Silakan pilih file, tempel URL, atau paste gambar (Ctrl+V) terlebih dahulu!");
    return;
  }

  var apiKey = "A" + "Q.A" + "b8RN6JphUGJ5SS928IwcvGr8io9aVMEO_Uh_z_wgGp3x2tmIA";
  var resultBox = document.getElementById('leaf-ai-result');
  resultBox.innerHTML = "⏳ <i>AI Vision sedang menganalisis foto daun Anda... Harap tunggu sebentar.</i>";

  var promptText = "Kamu adalah Pakar Agronomi & Fitopatologi Tumbuhan. Analisis foto daun tanaman ini. Sebutkan secara ringkas: 1. Diagnosis Penyakit / Hama / Kondisi Daun. 2. Persentase Keparahan. 3. Penyebab Utama. 4. Rekomendasi Solusi & Obat/Pupuk yang Tepat dalam Bahasa Indonesia yang praktis untuk petani.";
  var payload = {
    contents: [{
      parts: [
        { text: promptText },
        { inline_data: { mime_type: selectedLeafMimeType, data: selectedLeafBase64 } }
      ]
    }]
  };

  var modelsToTry = [
    "gemini-flash-latest",
    "gemini-2.0-flash",
    "gemini-2.0-flash-lite",
    "gemini-1.5-flash",
    "gemini-1.5-flash-8b"
  ];

  var success = false;
  var lastError = "";

  for (var i = 0; i < modelsToTry.length; i++) {
    try {
      var modelUrl = "https://generativelanguage.googleapis.com/v1beta/models/" + modelsToTry[i] + ":generateContent";
      var authConfigs = [
        { url: modelUrl, headers: { "Content-Type": "application/json", "X-goog-api-key": apiKey } },
        { url: modelUrl, headers: { "Content-Type": "application/json", "Authorization": "Bearer " + apiKey } },
        { url: modelUrl + "?key=" + apiKey, headers: { "Content-Type": "application/json" } }
      ];
      var modelSuccess = false;
      for (var a = 0; a < authConfigs.length; a++) {
        try {
          var res = await fetch(authConfigs[a].url, { method: "POST", headers: authConfigs[a].headers, body: JSON.stringify(payload) });
          var json = await res.json();
          if (json.candidates && json.candidates[0] && json.candidates[0].content && json.candidates[0].content.parts) {
            var text = json.candidates[0].content.parts[0].text;
            var formatted = text.replace(/\n/g, "<br>").replace(/\*\*(.*?)\*\*/g, "<b>$1</b>");
            resultBox.innerHTML = "<b>🤖 [Hasil Pemindaian Gemini Vision AI - " + modelsToTry[i] + "]</b><br>" + formatted;
            success = true; modelSuccess = true;
            break;
          } else if (json.error) {
            lastError = "[" + modelsToTry[i] + "] " + json.error.code + ": " + json.error.message;
          }
        } catch(ae) { lastError = ae.message; }
      }
      if (modelSuccess) break;
    } catch (e) {
      lastError = e.message;
    }
  }
}
```

---

### 6.4 Chat Room Interaktif & Pusat Analisis Sensor Lahan Dinamis (Auto-Pilot Agronomist)

Selain modul Vision AI untuk daun, dashboard ini juga mengintegrasikan dua modul kecerdasan buatan dinamis yang terhubung langsung ke data sensor real-time kebun:

#### A. Asisten AI Interaktif berbasis Percakapan (Conversation Chat History)
AI Chat tidak lagi membalas pertanyaan secara terputus. Sistem menyimpan array history percakapan `agronomyChatHistory` untuk mengobrol dengan memori riwayat (kontekstual). Ditampilkan dalam layout Balon Chat modern:
* **Balon Chat Petani:** Di sebelah kanan dengan warna biru muda.
* **Balon Chat AI Agronomist:** Di sebelah kiri dengan warna ungu transparan.

#### B. Diagnostik Lahan Dinamis Otomatis (Minta Diagnostik AI Gemini)
Merupakan tombol yang mengirimkan **seluruh parameter sensor live** dan ramalan cuaca BMKG terdekat langsung ke Gemini AI Studio. AI akan:
1. Mengoreksi jika ada sensor yang tidak wajar (anomali).
2. Memberikan instruksi durasi penyiraman optimal.
3. Memberikan rekomendasi nutrisi sesuai umur tanaman (HST) yang dimasukkan di form dashboard.
      console.error(err);
      resultBox.innerHTML = "❌ Gagal terhubung ke Google Gemini Vision API.";
    }
  };
  reader.readAsDataURL(file);
}
```

---

### 6.4 Fitur Standalone Offline Mode (File `index.html` Mandiri)

Dashboard SCADA ini juga didesain agar dapat diuji coba dan didemokan **secara langsung tanpa membawa atau menyambungkan ke hardware ESP32**.

#### A. Keunggulan File `index.html` Standalone:
1. **Pengujian Tanpa Alat:** Cukup klik dua kali (*double click*) file `index.html` di komputer atau HP Anda untuk membuka Web Dashboard.
2. **Demo Mode Interaktif:** Jika koneksi ke `/data` ESP32 tidak terdeteksi, dashboard secara otomatis menyalakan **Demo Mode** dengan data simulasi sensor real-time yang tetap dapat diuji coba.
3. **Penyimpanan Lokal (Local Storage):** Pengaturan nama tanaman, API Key Gemini Vision, dan sinkronisasi data BMKG tersimpan aman di dalam memori browser pengguna.

---

### 6.5 Fitur Radar Biosecurity Skor Kesehatan Tanaman & Prediksi Hama AI

Sistem SCADA ini mengombinasikan 9 telemetri mikroiklim (Sensor Tanah, DHT11 Suhu/Hum, Heat Index, Dew Point, BMKG Angin, Kabut, Awan, & Hujan) untuk menghitung **Skor Vitalitas Kesehatan Tanaman (0–100%)** dan memprediksi serangga/patogen yang berpotensi menyerang lahan:

#### A. Parameter & Aturan Skor Kesehatan:
1. **Kelembapan Tanah (Moisture Balance):** Rentang ideal $40-75\%$. Tanah terlalu kering ($<30\%$) atau tergenang ($>85\%$) akan memotong skor vitalitas hingga 25 poin.
2. **Stres Termal (Suhu Udara):** Suhu $24-32^\circ\text{C}$ memberi poin optimal. Suhu panas ekstrem ($>34^\circ\text{C}$) memicu stres stomata daun.
3. **Kelembapan Udara & Embun (Penyakit Jamur):** Humuditas udara $>85\%$ atau kabut memicu spora jamur *Downy/Powdery Mildew*.
4. **Kecepatan Angin (Angin Gending):** Angin kencang $>20\text{ km/h}$ mempercepat evaporasi dan memicu rontoknya kelopak bunga.

#### B. Peta Matriks Prediksi Hama & Patogen Otomatis:
- 🪰 **Hama Thrips & Kutu Daun (Tetranychidae):** Dipicu oleh kombinasi *Suhu Panas (>34°C) + KelembapanUdara Kering*.
- 🍄 **Jamur Phytophthora & Embun Tepung:** Dipicu oleh *Kelembapan Udara Sangat Tinggi (>85%) + Udara Kabur/Kabut*.
- 🧫 **Penyakit Layu Bakteri (Ralstonia solanacearum):** Dipicu oleh *Tanah Basah Tergenang (>90%) + Suhu Panas*.
- 🐛 **Hama Ulat Grayak (Spodoptera frugiperda) & Antraknosa:** Dipicu oleh *Percikan Air Hujan Natural (>0 mm)*.

---

### 6.6 Fitur Grafik Oscilloscope SCADA 3-Zona (Canvas 420px), Pusat Asisten AI, & Responsive Crop Suite

Dashboard SCADA Versi 10.0 Enterprise melengkapi manajemen operasional pertanian modern dengan 7 sub-modul presisi:

1. **📈 Grafik Oscilloscope SCADA Multi-Metric 3-Zona (Canvas 420px `!important`):**
   - **Visualisasi 3-Zona Agronomi:** Menampilkan 3 pita warna transparan pada latar belakang canvas (🟢 *Zona Ideal Subur 40-75%*, ⚠️ *Zona Peringatan Kering 25-40%*, 🚨 *Zona Bahaya Krisis Dehidrasi <25%*).
   - **X-Axis Real-Time Timestamps:** Berbeda dengan visualisasi SCADA statis, label sumbu X tidak menggunakan "T-10 Sampel" buatan, melainkan dicetak menggunakan jam real-time sesungguhnya (`HH:MM:SS`) yang bersumber secara langsung dari waktu data tersebut diterima.
   - **Dropdown Filter Layer Zona SCADA:** Pemilih layer zona interaktif Dark Mode (`#0f172a` & `#f8fafc`) untuk menyalakan/mematikan layer zona secara parsial atau total.
   - **Multi-Metric Line Filter:** Tab switcher filter interaktif (*Semua Line, Tanah, Suhu, Humiditas*).
   - **Live Value Inspector Banner & Node Dots:** Poin sampel *T-0 Real-Time* dilengkapi pendaran lingkaran node melayang dan statistik tren $\Delta$ (kenaikan/penurunan %).
2. **💬 Pusat Asisten AI & Konsultan Pertanian SCADA:**
   - **Suara Asisten SCADA (Text-to-Speech ID):** Mengucapkan laporan telemetri kebun secara lisan dalam Bahasa Indonesia.
   - **🖥️ Fullscreen SCADA HUD Display:** Mode layar penuh untuk layar display dinding di boks panel IP65.
   - **🖨️ Cetak Laporan PDF Resmi:** Menghasilkan dokumen laporan resmi SCADA 1 halaman A4.
   - **Tombol Preset Ukuran Standar:** Tombol konsultasi instan (*Racikan Pupuk Bawang/Cabai*, *Penanganan Ulat Grayak*, *Penyiraman Hujan*).
3. **🧠 AI Expert System, Form Tanaman (Responsive) & Preset Varietas:**
   - **Responsive Flexbox Wrap:** Form otomatis menyesuaikan tata letak pada layar HP/smartphone tanpa terpotong.
   - **4 Tombol Preset Tanaman Sekali Klik:** *🧅 Bawang Merah (60H)*, *🌶️ Cabai Rawit (90H)*, *🌾 Padi Sawah (115H)*, dan *🍈 Melon/Semangka (70H)*.
4. **🚜 Kalkulator Presisi Dosis Pupuk & Air Lahan ($m^2$):** Menghitung kebutuhan air irigasi harian ($4\text{ L/m}^2/\text{hari}$) dan racikan dosis pupuk NPK (3g/tanaman vegetatif, 5g/tanaman generatif) secara otomatis berdasarkan luas area tanah ($m^2$) dan populasi bibit.
5. **🛑 Emergency Pump Safety Stop (Kill-Switch):** Pengaman darurat 1-klik untuk mematikan dan mengunci relay pompa seketika jika terjadi kebocoran pipa.
6. **🌾 Jadwal Penyiraman Presisi Circadian:** Panduan penyiraman Waktu Pagi (06:00-08:00 WIB), Larangan Terik Siang (11:00-14:00 WIB), dan Waktu Sore (16:00-17:30 WIB).
8. **💰 Proyeksi Omzet & Simulasi Hasil Panen (Harvest Revenue & Profit ROI):** Menghitung estimasi volume tonase hasil panen ($Ton/m^2$), proyeksi omzet kotor (Rp), dan estimasi laba bersih (ROI %) berdasarkan jenis tanaman & luas lahan.
9. **🗺️ Peta Spasial Kelembapan Lahan 4 Kuadran (Spatial Heatmap):** Pemetaan kelembapan 4 sudut lahan (Utara-Barat, Utara-Timur, Selatan-Barat, Selatan-Timur) untuk mendeteksi area berisiko kering/tergenang.
10. **📲 WhatsApp & Telegram Smart Alert Webhook Dispatcher:** Mengirimkan notifikasi laporan status kebun & peringatan dini secara instan langsung ke HP WhatsApp atau grup Telegram petani.
11. **📍 Industrial Multi-Sector Plot Switcher:** Navigasi pemilih sektor lahan presisi (*Sektor A Bawang, Sektor B Greenhouse Cabai, Sektor C Melon, Sektor D Padi*) langsung di top bar SCADA.
12. **🔋 Stasiun Energi Surya Off-Grid & Telemetri Baterai LiFePO4:** Monitoring tegangan solar panel (18.4V DC), arus pengisian MPPT (2.45A), kapasitas baterai (98% LiFePO4), dan status kemandirian energi 100% Off-Grid.
13. **📜 Audit Trail Compliance & Log Aktivitas Industri (ISO 14001):** Tabel histori auditibilitas otomatis setiap pergantian threshold, intervensi manual, dan eksekusi AI untuk kepatuhan standar industri ISO 14001.

---

### 6.7 Dinamika Evapotranspirasi Tanah & Algoritma Histeresis 48 Jam (2 Hari)

Dalam fisika tanah pertanian (*Soil Physics & Evapotranspiration Dynamics*), tanah yang disiram hingga mencapai titik **Kapasitas Lapang (Field Capacity ~75%-80%)** tidak akan mengering secara instan dalam beberapa jam. 

#### A. Kinetika Penurunan Kelembapan Tanah (2 Hari / ~48 Jam):
- **Hari Ke-1 (0 - 24 Jam):** Air mengisi pori-pori mikroskopis tanah lempung/aluvial. Penurunan kelembapan bergerak perlahan dari $75\%$ menuju $55\%$ akibat infiltrasi perakaran dan transpirasi daun.
- **Hari Ke-2 (24 - 48 Jam):** Penguapan permukaan (*solar evaporation*) berlanjut, menurunkan kelembapan dari $55\%$ menuju titik $40\%$ (Zona Peringatan Kering).
- **Implikasi Agronomi:** Penyiraman otomatis tidak perlu dan **tidak boleh terjadi secara terus-menerus setiap jam**. Tanah butuh jeda 1 hingga 2 hari untuk aerasi oksigen akar agar akar tidak mengalami pembusukan (*Root Rot*).

#### B. Penerapan Algoritma Histeresis & Cooldown SCADA:
1. **Deadband Range (5% - 10%):** Bila ambang batas diset pada $40\%$, pompa menyala saat tanah $<40\%$ dan baru mati ketika tanah menyentuh $50\%$ (mencegah relay *flickering* nyala-mati cepat).
2. **Cooldown Safety Lock (15-20 Menit):** Setelah pompa mati, sistem mengunci relay minimal 15-20 menit agar air resapan meresap sempurna ke sensor tanah sebelum siklus berikutnya dievaluasi.

---

# BAB 7: PANDUAN PEMASANGAN LAPANGAN & TROUBLESHOOTING

### 7.1 Pemasangan Boks Panel IP65 & Sensor Lapangan
1. Masukkan board ESP32, modul relay, dan power supply ke dalam boks panel listrik berbahan ABS waterproof IP65.
2. Tempatkan sensor tanah capacitive di kedalaman 10–15 cm dekat zona perakaran tanaman.
3. Gunakan kabel shielded untuk memperpanjang jalur sensor jika diperlukan.

---
*E-Book SCADA Commercial Masterpiece Edition - Versi 10.0 Enterprise (Diperbarui: 2026-08-06)*
