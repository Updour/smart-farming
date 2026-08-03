# 🌱 Smart Farm Offline-First System

> Sistem Irigasi Presisi & Stasiun Cuaca Berbasis ESP32, ESP8266, Protokol Radio ESP-NOW, dan Web Dashboard Interaktif Tanpa Internet.

![License](https://img.shields.io/badge/License-MIT-blue.svg)
![Platform](https://img.shields.io/badge/Platform-ESP32%20%7C%20ESP8266-green.svg)
![Protocol](https://img.shields.io/badge/Protocol-ESP--NOW%20%7C%20HTTP-orange.svg)

---

## 📌 Ikhtisar Proyek

Proyek ini dirancang khusus untuk mengatasi permasalahan **ketiadaan jaringan internet di area pertanian/perkebunan terpencil**. Dengan arsitektur **Offline-First (Store & Forward)**, stasiun pusat ESP32 dapat beroperasi 100% secara otonom, mengendalikan pompa air Sanyo 220V, mengelola lampu indikator *Traffic Light*, serta mencatat data kebun ke dalam memori *Flash* internal.

Pengguna dapat memantau dan mengendalikan alat melalui **Web Server Lokal** dari browser Smartphone tanpa perlu kuota internet, serta menyinkronkan data cuaca satelit global dari OpenWeatherMap melalui teknik *Browser Caching*.

---

## ✨ Fitur-Fitur Utama

- 📡 **Komunikasi Nirkabel ESP-NOW:** Pengiriman data tanah dari ESP8266 di tengah kebun ke ESP32 sejauh 100m+ tanpa perlu *Router* WiFi.
- ⚡ **Proteksi Dinamo Pompa Sanyo 20 Menit:** Fitur *Timer Cut-Off* otomatis yang mematikan pompa setelah 20 menit menyala untuk mencegah mesin gosong/terbakar akibat air habis.
- 🚨 **Sistem Fail-Safe & Auto-Recovery:** Indikator LED Merah berkedip cepat (*blinking*) saat sensor rusak atau sinyal terputus, dan otomatis pulih (*Recovery*) begitu koneksi tersambung kembali.
- 💻 **Web Serial Debug Console (0-Byte RAM):** Emulator terminal hitam di browser HP untuk membaca log *debug* ESP32 secara *real-time* tanpa membebani RAM ESP32.
- 🌐 **Sinkronisasi Satelit 2-Langkah:** Menggunakan memori sementara browser HP (*localStorage*) untuk mengambil data cuaca OpenWeatherMap saat ada internet, lalu menyuntikkannya ke ESP32.
- 💾 **Perekam Data Memori LittleFS & Auto-Wipe:** Data kebun dicatat otomatis setiap 1 jam. Jika ukuran file melebihi 50KB, memori akan melakukan *Auto-Wipe* otomatis agar *storage* awet seumur hidup.
- 📱 **Siap Kompatibel dengan Flutter App:** Menyediakan REST API JSON (`/data`, `/pushWeather`, `/downloadLog`) yang siap diintegrasikan ke aplikasi mobile Flutter & Firebase.

---

## 🛠️ Skema Rangkaian & Pinout

### Node Master (ESP32 Receiver & Server)
| Komponen / Modul | Pin Komponen | Pin ESP32 GPIO | Keterangan |
| :--- | :---: | :---: | :--- |
| **Sensor DHT11** | DATA | `GPIO 4` | Pembaca Suhu & Kelembapan Udara |
| **Modul Relay** | IN | `GPIO 25` | Kendali Pompa Sanyo 220V |
| **LED Hijau** | Anoda (+) | `GPIO 32` | Indikator Status AMAN |
| **LED Kuning** | Anoda (+) | `GPIO 33` | Indikator Status PERINGATAN |
| **LED Merah** | Anoda (+) | `GPIO 27` | Indikator Status BAHAYA / ERROR |

### Node Kebun (ESP8266 Sender)
| Komponen | Pin Sensor | Pin ESP8266 |
| :--- | :---: | :---: |
| **Soil Moisture** | A0 | `Pin A0` |

---

## 📁 Struktur Berkas Proyek

```text
Sketchbook/
├── esp32_server_receiver/
│   ├── esp32_server_receiver.ino  # Kode Utama ESP32 Master
│   ├── index.h                    # HTML/CSS/JS Web Dashboard & Console
│   └── backup/                    # Folder Salinan Kode Cadangan
└── esp8266_soil_sender/
    └── esp8266_soil_sender.ino    # Kode ESP8266 Pemancar Lapangan
```

---

## 🌐 Dokumentasi REST API Endpoints

| Endpoint | Method | Deskripsi |
| :--- | :---: | :--- |
| `/` | `GET` | Menampilkan Web Dashboard HTML |
| `/data` | `GET` | Mengembalikan status JSON sensor & relay |
| `/setMode` | `GET` | Ganti mode (`?m=auto` / `?m=manual`) |
| `/toggleRelay` | `GET` | Saklar relay manual (`?s=on` / `?s=off`) |
| `/pushWeather` | `POST` | Menyuntikkan data cuaca OpenWeatherMap dari HP |
| `/downloadLog`| `GET` | Mengunduh berkas rekaman laporan `log.csv` |
| `/clearLogs` | `POST` | Menghapus log memori di dalam LittleFS |

---

## 🚀 Panduan Memulai (Quick Start)

1. **Install Library Arduino IDE:**
   - `DHT sensor library` by Adafruit
   - `Adafruit Unified Sensor`
   - `LittleFS` (Bawaan ESP32 Board Package)
2. **Upload Kode Sender:**
   - Dapatkan MAC Address ESP32 Anda.
   - Masukkan MAC Address ESP32 ke dalam `esp8266_soil_sender.ino`.
   - Upload ke NodeMCU ESP8266.
3. **Upload Kode Receiver:**
   - Upload `esp32_server_receiver.ino` ke ESP32.
4. **Sambungkan WiFi:**
   - Hubungkan HP Anda ke WiFi `SmartFarm-ESP32` (Password: `12345678`).
   - Buka browser dan akses `http://192.168.4.1`.

---
*Dikembangkan dengan ❤️ untuk Solusi Pertanian Otomatis Berkelanjutan.*
