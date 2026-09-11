# Smart IoT 8-Channel Lamp Scheduler (ESP32 + RTC DS1302 + Relay 8CH)

Aplikasi IoT cerdas untuk kontrol otomatis 8 channel lampu berbasis jadwal waktu presisi (RTC DS1302) dan kontrol nirkabel via Web Dashboard modern (*Glassmorphism Dark Mode*).

---

## 1. Skema Rangkaian Wiring Lengkap

### A. Pengaturan Jumper Selektor Relay (Wajib Dilakukan):
Pada modul relay 8-channel, di samping setiap channel (S1, S2, ..., S8) terdapat 3 pin dengan jumper bertuliskan **`COM`**, **`HIGH`**, dan **`LOW`**:
* **Pasang jumper menghubungkan pin `COM` dan `HIGH`** di seluruh channel (S1 s/d S8).
* Ini mengubah seluruh 8 channel menjadi **Active HIGH** (Relay ON saat pin GPIO ESP32 bernilai `HIGH` / 3.3V).

---

### B. Pinout ESP32 ke Modul Relay 8-Channel (Terminal Input IN1 - IN8)
| Pin Terminal Relay | Pin ESP32 | Keterangan |
| :--- | :--- | :--- |
| **VCC / DC+** | **VIN (5V)** | Sumber daya koil relay dari 5V (atau adaptor 5V eksternal) |
| **GND / DC-** | **GND** | Ground ESP32 |
| **IN1 (S1)** | **GPIO 25** | Lampu 1 (Teras) |
| **IN2 (S2)** | **GPIO 26** | Lampu 2 (Taman) |
| **IN3 (S3)** | **GPIO 32** | Lampu 3 (Garasi) |
| **IN4 (S4)** | **GPIO 33** | Lampu 4 (Balkon) |
| **IN5 (S5)** | **GPIO 27** | Lampu 5 (Ruang Tamu) |
| **IN6 (S6)** | **GPIO 18** | Lampu 6 (Dapur) |
| **IN7 (S7)** | **GPIO 19** | Lampu 7 (Koridor) |
| **IN8 (S8)** | **GPIO 23** | Lampu 8 (Halaman Belakang) |

---

### C. Pinout ESP32 ke Modul RTC DS1302
| Pin DS1302 | Pin ESP32 | Keterangan |
| :--- | :--- | :--- |
| **VCC** | **3.3V** | Level logika aman untuk GPIO ESP32 |
| **GND** | **GND** | Ground umum |
| **DAT / IO** | **GPIO 14** | Jalur Data 3-Wire |
| **CLK / SCLK** | **GPIO 12** | Jalur Clock Serial |
| **RST / CE** | **GPIO 13** | Chip Enable / Reset |

---

### D. Terminal Output Screw Relay ke Beban Lampu (220V PLN)
Di setiap relay terdapat 3 terminal baut output:
1. **COM (Common)** $\rightarrow$ Sambungkan ke kabel **Fasa (Listrik Api)** PLN.
2. **NO (Normally Open)** $\rightarrow$ Sambungkan ke salah satu kabel **Lampu**.
3. **NC (Normally Closed)** $\rightarrow$ **Biarkan Kosong** (tidak digunakan).
*(Kabel Netral dari stopkontak PLN langsung menuju kabel netral lampu).*

---

## 2. Cara Upload ke ESP32

1. Buka Arduino IDE.
2. Buka file: [esp32_lamp_scheduler.ino](file:///media/budgei/Development2/arduino/Sketchbook/esp32_lamp_scheduler/esp32_lamp_scheduler.ino).
3. Pilih Board: **ESP32 Dev Module** (atau DOIT ESP32 DEVKIT V1).
4. Pastikan library `Rtc_by_Makuna` telah terpasang di Arduino IDE.
5. Hubungkan ESP32 via kabel USB dan tekan **Upload**.

---

## 3. Cara Mengakses Web Dashboard

1. Nyalakan ESP32. ESP32 otomatis memancarkan Access Point WiFi:
   - **SSID**: `Smart-Lamp-AP`
   - **Password**: `12345678`
2. Sambungkan HP atau Laptop ke WiFi `Smart-Lamp-AP`.
3. Buka browser dan kunjungi: **`http://192.168.4.1`**
4. Atau untuk menguji tampilan dashboard di komputer secara offline, Anda bisa langsung membuka file [index.html](file:///media/budgei/Development2/arduino/Sketchbook/esp32_lamp_scheduler/index.html).

---

## 4. Fitur Utama Sistem 8-Channel

1. **Kontrol Mandiri 8 Lampu**: Masing-masing channel memiliki sakelar manual dan jadwal independen (ON & OFF).
2. **Jadwal Melewati Jam 00:00 (Overnight)**: Mendukung jadwal semalam suntuk (misal nyala jam 18:00, padam jam 06:00).
3. **1-Klik Sync Jam RTC**: Waktu RTC DS1302 langsung dicocokkan dengan detik jam di HP Anda.
4. **Nama Lampu Bisa Diedit**: Ubah nama lampu sesuai ruangan dan tersimpan permanen di memori Flash NVS ESP32.
5. **Aksi Massal**: Tombol *Semua ON*, *Semua OFF*, *Semua AUTO*.
