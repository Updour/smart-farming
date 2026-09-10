# 📖 BUKU PANDUAN PINTAR SMART FARM IOT & KEBUN MODERN ESP32
**Buku Pegangan Lapangan Super Praktis: Dari Nol Belanja Alat, Pasang Kabel, Isi Program, Pantau Kebun Lewat Layar HP, Hingga Otomatis Siram & Pupuk (Khusus Pemula, Orang Awam & Petani)**

---

*Penyusun: Tim Lapangan Smart Farm, Dokter Tanaman & Praktisi Pertanian Presisi*  
*Edisi: 4.2 (Edisi Lengkap & Bebas Macet: Panduan Driver USB, Daftar Belanja & Kode Program)*  
*Lokasi Proyek: `/media/budgei/Development2/arduino/Sketchbook`*  
*Program Alat: `esp32_server_receiver.ino` (Kotak Rumah) & `esp8266_soil_sender.ino` (Kotak Kebun)*

---

## 🧭 KAMUS CEPAT ORANG AWAM (BACA INI DULU!)

Bagi Anda yang baru pertama kali menyentuh alat elektronik atau merasa "gaptek", jangan takut! Semua istilah canggih di sistem ini sebenarnya punya padanan sederhana di kehidupan sehari-hari:

| Istilah di Layar / Buku | Artinya dalam Bahasa Sehari-hari | Analogi Gampangnya |
| :--- | :--- | :--- |
| **ESP32 (Kotak Rumah)** | Otak komputer utama yang ditaruh di dalam rumah/panel gubuk. | Seperti **Kepala Mandor** yang mencatat data dan memencet sakelar pompa. |
| **ESP8266 (Kotak Kebun)** | Alat kecil pengirim data yang ditancapkan dekat bedengan tanah. | Seperti **Kurir Kebun** yang tiap detik lari ngabarin kondisi tanah ke rumah. |
| **ESP-NOW** | Sinyal radio khusus jarak jauh antar alat tanpa internet. | Mirip **Walkie-Talkie (HT)** satpam, tetap nyambung walau sinyal HP mati total. |
| **Zero-Dummy (Anti-Bohong)**| Sistem pantang menampilkan angka rekaan/palsu. | Kalau kabel copot, layar tulis **`--`**, bukan pura-pura sehat. |
| **VWC (Kelembapan Tanah)** | Berapa persen basahnya tanah kebun. | Seperti seberapa basah **spons cuci piring** saat diperas air. |
| **VPD (Tingkat Haus Daun)** | Seberapa kencang udara menyedot air dari daun. | Makin terik & kering anginnya, daun makin haus dan gampang layu. |
| **Relai (Relay)** | Sakelar listrik otomatis pemutus dan penyambung kabel pompa. | Seperti **sakelar lampu dinding**, tapi dipencet otomatis oleh komputer. |
| **RTC (Jam Digital)** | Modul jam yang punya baterai koin sendiri. | Seperti **jam dinding**, tetap tahu jam berapa walau mati lampu. |
| **Digital Twin LCD** | Cermin kembaran layar kotak kebun yang muncul di layar HP. | Seperti **kaca cermin**; apa yang tampil di kebun, persis sama di HP Anda. |
| **Traffic Light (Lampu 3 Warna)**| Lampu tiang penunjuk kondisi lahan (Merah, Kuning, Hijau). | Persis **lampu merah jalan raya**: Merah bahaya, Kuning siap-siap, Hijau aman. |
| **Arduino IDE** | Aplikasi gratis di komputer/laptop untuk mengisi program ke alat. | Seperti **kabel data pengisi lagu ke HP**, memasukkan nyawa ke otak alat. |
| **Driver USB (CH340/CP2102)**| Kunci pembuka agar laptop bisa menyapa papan ESP. | Seperti **kunci pintu**, tanpa ini laptop tidak tahu ada alat yang dicolok. |

---

## 📑 DAFTAR ISI PANDUAN

1. [BAB 1: Cara Kerja Kebun Pintar (Tanpa Kuota & Tanpa Ribet)](#bab-1-cara-kerja-kebun-pintar-tanpa-kuota--tanpa-ribet)
2. [BAB 2: Daftar Belanja Alat, Peringatan Sensor, & Panduan Pasang Kabel](#bab-2-daftar-belanja-alat-peringatan-sensor--panduan-pasang-kabel)
3. [BAB 3: Cara Buka Layar Kontrol di HP (Semudah Buka Browser)](#bab-3-cara-buka-layar-kontrol-di-hp-semudah-buka-browser)
4. [BAB 4: Tab 1 — Layar Pantau Utama, Cermin LCD & Lampu Kebun](#bab-4-tab-1--layar-pantau-utama-cermin-lcd--lampu-kebun)
5. [BAB 5: Tab 2 — Grafik Naik-Turun Kebun & Kirim Laporan via WhatsApp](#bab-5-tab-2--grafik-naik-turun-kebun--kirim-laporan-via-whatsapp)
6. [BAB 6: Tab 3 — Atur Pompa Siram Otomatis & Lampu Pemanas Malam](#bab-6-tab-3--atur-pompa-siram-otomatis--lampu-pemanas-malam)
7. [BAB 7: Tab 4 — Umur Tanaman (HST) & Kebutuhan Air Tiap Fase](#bab-7-tab-4--umur-tanaman-hst--kebutuhan-air-tiap-fase)
8. [BAB 8: Tab 5 — Prakiraan Hujan Satelit BMKG (Otomatis Tunda Siram)](#bab-8-tab-5--prakiraan-hujan-satelit-bmkg-otomatis-tunda-siram)
9. [BAB 9: Tab 6 — Takaran Pupuk Dapur (Sendok Makan, Gelas Aqua, Tangki Semprot)](#bab-9-tab-6--takaran-pupuk-dapur-sendok-makan-gelas-aqua-tangki-semprot)
10. [BAB 10: Tab 7 — Dokter Tanaman & Cek Penyakit Lewat Foto Daun](#bab-10-tab-7--dokter-tanaman--cek-penyakit-lewat-foto-daun)
11. [BAB 11: Tab 8 — Catatan Riwayat Kebun & Simpan Cadangan](#bab-11-tab-8--catatan-riwayat-kebun--simpan-cadangan)
12. [BAB 12: Pertolongan Pertama Masalah Lapangan (Tanya-Jawab Cepat)](#bab-12-pertolongan-pertama-masalah-lapangan-tanya-jawab-cepat)
13. [BAB 13: Panduan Lengkap Arduino IDE & Solusi Driver Port COM Laptop](#bab-13-panduan-lengkap-arduino-ide--solusi-driver-port-com-laptop)
14. [BAB 14: Kode Lengkap Kotak Kebun (ESP8266 Sender) & Cara Kerjanya](#bab-14-kode-lengkap-kotak-kebun-esp8266-sender--cara-kerjanya)
15. [BAB 15: Kode Lengkap Kotak Rumah (ESP32 Gateway) & Kompilasi Web Otomatis](#bab-15-kode-lengkap-kotak-rumah-esp32-gateway--kompilasi-web-otomatis)
16. [BAB 16: Panduan Mengubah Nama WiFi, Sandi, & Jam Siram Sesuai Keinginan](#bab-16-panduan-mengubah-nama-wifi-sandi--jam-siram-sesuai-keinginan)

---

## BAB 1: CARA KERJA KEBUN PINTAR (TANPA KUOTA & TANPA RIBET)

### 1.1. Bayangkan Anda Punya Penjaga Kebun Pribadi 24 Jam
Alat ini bekerja tanpa kenal lelah untuk menjaga tanaman Anda:
1. **Mencelupkan Jari ke Tanah**: Setiap saat mengecek apakah tanah masih basah atau sudah kering kerontang.
2. **Merasakan Hawa Udara**: Membaca apakah hawa di sekitar daun terlalu panas menyengat atau sejuk nyaman.
3. **Menyalakan Keran Otomatis**: Jika tanah kering, pompa otomatis hidup. Begitu tanah sudah kenyang air, pompa langsung mati.
4. **Memberitahu Anda Lewat Lampu**: Ada lampu 3 warna di tiang kebun. Cukup lirik dari jauh: kalau hijau artinya aman, kuning waspada, merah darurat.
5. **Bisa Dilihat dari HP Sambil Ngopi**: Cukup sambungkan HP Anda ke sinyal alat, seluruh kondisi kebun terbuka di layar tanpa pulsa sama sekali.

### 1.2. Prinsip "Anti-Bohong" (Zero-Dummy)
Banyak alat di pasaran yang jika kabelnya putus, di layar HP masih tertulis "28°C" pura-pura normal. Di alat kita:
* **Jujur 100%**: Kalau kabel pengirim di kebun lepas atau kehabisan baterai, layar akan jujur menulis **`--`** dan bertuliskan **"Sensor Terputus"**.
* **Lampu Hijau Hanya untuk yang Benar-Benar Aman**: Lampu hijau tidak akan pernah menyala kalau alat sedang ragu atau tidak ada data.

---

## BAB 2: DAFTAR BELANJA ALAT, PERINGATAN SENSOR, & PANDUAN PASANG KABEL

Banyak orang ragu memulai karena takut biayanya mahal hingga jutaan rupiah. Padahal seluruh komponen sistem ini sangat murah dan mudah dibeli di toko online (Tokopedia, Shopee, Bukalapak) dengan total modal hanya sekitar **Rp 180.000 sampai Rp 220.000 saja!**

### 2.1. Daftar Belanja Komponen Lengkap (Tinggal Cari di Toko Online):

| No | Nama Barang yang Dicari | Fungsi Sederhana | Perkiraan Harga |
| :---: | :--- | :--- | :--- |
| 1 | **ESP32 DevKit V1 (30 Pin)** | Otak utama penerima & pemancar web di rumah | Rp 55.000 – Rp 65.000 |
| 2 | **NodeMCU ESP8266 (atau Wemos D1 Mini)** | Kotak kecil pengirim data nirkabel dari kebun | Rp 25.000 – Rp 35.000 |
| 3 | **Sensor Kelembapan Tanah Kapasitif v1.2** *(Bilah Hitam)* | Mengukur basahnya tanah tanpa takut karatan | Rp 12.000 – Rp 18.000 |
| 4 | **Sensor Suhu Udara DHT11** | Mengukur sejuk/panasnya hawa daun tanaman | Rp 12.000 – Rp 15.000 |
| 5 | **Layar LCD 16x2 Biru + Modul I2C Backpack** | Layar kecil kotak rumah (hemat kabel, cukup 4 pin)| Rp 25.000 – Rp 30.000 |
| 6 | **Modul Jam RTC DS1302 + Baterai Koin CR2032** | Menjaga jam tetap cocok walau listrik padam | Rp 8.000 – Rp 12.000 |
| 7 | **Modul Relai 2-Channel 5V (Relay Module)** | Sakelar otomatis untuk pompa air & lampu pemanas | Rp 12.000 – Rp 16.000 |
| 8 | **Lampu LED 5mm (Hijau, Kuning, Merah) + Resistor 220Ω** | Lampu tiang penunjuk kondisi lahan 3 warna | Rp 3.000 – Rp 5.000 |
| 9 | **Kabel Jumper Pelangi Dupont (Female-to-Female & Male-to-Female)** | Kabel colok praktis tanpa perlu solder timah | Rp 10.000 – Rp 15.000 |
| 10 | **Adaptor Charger HP 5V 2A + Kabel Micro-USB** | Sumber listrik colokan rumah & kotak kebun | Rp 20.000 – Rp 25.000 |
| **TOTAL ESTIMASI MODAL** | *(Dapat komputer canggih pemantau kebun lengkap!)* | **± Rp 180.000 – Rp 220.000** |

---

### 2.2. ⚠️ Peringatan Emas: Jangan Salah Beli Sensor Tanah!
Di toko online ada 2 macam sensor tanah dengan harga mirip, tetapi kualitasnya bagaikan bumi dan langit:
* ❌ **JANGAN BELI: Sensor Tanah Resistif (Bilah Garpu Besi Terbuka)**:
  * Bentuknya bercabang dua seperti garpu makan dengan lapisan kuningan/besi terbuka.
  * **Kelemahannya**: Karena besi dialiri listrik langsung di dalam lumpur basah, bilahnya akan **berkarat, menghitam, dan hancur lebur dalam 1–2 minggu saja** akibat korosi kimia (*elektrolisis*).
* ✅ **WAJIB BELI: Sensor Kelembapan Tanah Kapasitif v1.2 (Bilah Hitam Rata)**:
  * Bentuknya sebilah papan tipis warna hitam mulus tanpa ada kawat logam telanjang.
  * **Kelebihannya**: Sensor ini mengukur medan frekuensi, bukan arus setrum. **Tahan berbulan-bulan hingga bertahun-tahun di dalam lumpur kebun tanpa pernah berkarat!**

---

### 2.3. Aturan Emas Warna Kabel (Hafalkan 3 Ini Saja!)
Agar tidak korslet, selalu samakan warna kabel:
* 🔴 **Kabel MERAH**: Sumber Listrik Positif (**3.3V atau 5V**).
* ⚫ **Kabel HITAM**: Jalur Arus Negatif / Arde (**GND**).
* 🟡 **Kabel KUNING / WARNA LAIN**: Jalur Kirim Data / Sinyal.

---

### 2.4. Bagian 1: Alat di Bedengan Kebun (ESP8266 + Sensor Tanah)

Alat ini ditaruh di dalam toples/kotak plastik kedap air di tengah bedengan tanaman.

```
  ┌──────────────────────────────────┐            ┌────────────────────────────────┐
  │ Sensor Tanah Kapasitif Hitam     │            │ Kotak Pemancar (ESP8266)       │
  │                                  │            │                                │
  │ Kaki VCC (Kabel Merah)           ├───────────►│ Lubang pin bertuliskan 3.3V    │
  │ Kaki GND (Kabel Hitam)           ├───────────►│ Lubang pin bertuliskan GND     │
  │ Kaki AOUT (Kabel Kuning)         ├───────────►│ Lubang pin bertuliskan A0      │
  └──────────────────────────────────┘            │                                │
                                                  │ Colok Powerbank / Charger 5V   │
                                                  └────────────────────────────────┘
```

> ⚠️ **Cara Tancap Sensor ke Tanah**: 
> Tancapkan bilah hitam ke tanah **hanya sampai batas garis putih melintang**. Jangan kubur bagian kepala yang ada komponennya ke dalam lumpur agar tahan bertahun-tahun.

---

### 2.5. Bagian 2: Panel Utama di Rumah / Gubuk (ESP32)

Alat ini adalah "bos" yang menerima data dari kebun, menampilkan angka di layar LCD biru, dan menyalakan pompa air.

```
                                  ┌────────────────────────────────┐
                                  │      PAPAN UTAMA ESP32         │
                                  │                                │
     Sensor Suhu Udara (DHT11) ───┤ Lubang Pin 4                   │
                                  │                                │
     Layar LCD Biru (Kabel SDA) ──┤ Lubang Pin 21                  │
     Layar LCD Biru (Kabel SCL) ──┤ Lubang Pin 22                  │
                                  │                                │
     Jam Digital RTC (Kabel DAT) ─┤ Lubang Pin 14                  │
     Jam Digital RTC (Kabel CLK) ─┤ Lubang Pin 12                  │
     Jam Digital RTC (Kabel RST) ─┤ Lubang Pin 13                  │
                                  │                                │
     Kabel Sakelar Pompa Air ─────┤ Lubang Pin 26                  │
     Kabel Sakelar Lampu Malam ───┤ Lubang Pin 25                  │
                                  │                                │
     Lampu Tiang Hijau ───────────┤ Lubang Pin 32 (Lewat Resistor) │
     Lampu Tiang Kuning ──────────┤ Lubang Pin 33 (Lewat Resistor) │
     Lampu Tiang Merah ───────────┤ Lubang Pin 27 (Lewat Resistor) │
                                  │                                │
     Listrik Colokan Adaptor ─────┤ Lubang VIN (5V) & Lubang GND   │
                                  └────────────────────────────────┘
```

#### Cara Pasang Sakelar Pompa Air (Kotak Relai Hitam):
* Anggap relai seperti **sakelar lampu biasa** di dinding rumah Anda.
* Ambil satu kabel listrik pompa air Anda (biasanya kabel warna cokelat/hitam), lalu **gunting di tengah**.
* Masukkan ujung guntingan pertama ke lubang baut bertuliskan **COM**, dan ujung guntingan kedua ke lubang **NO**. Kencangkan bautnya dengan obeng kecil.
* Saat alat memberi perintah siram, relai berbunyi *"cetuk"* dan pompa air langsung mengalir!

---

## BAB 3: CARA BUKA LAYAR KONTROL DI HP (SEMUDAH BUKA BROWSER)

Anda tidak perlu download aplikasi apapun di Google Play Store atau repot bikin akun.

### 3.1. Tiga Langkah Sangat Mudah:
1. **Nyalakan Listrik Alat**: Colokkan charger adaptor ESP32 ke stopkontak rumah.
2. **Buka Menu Wi-Fi di HP Anda**:
   * Cari nama Wi-Fi: **`SmartFarm_ESP32`**.
   * Klik sambungkan (jaringan lokal ini gratis, tidak menyedot kuota internet HP Anda).
3. **Buka Google Chrome atau Safari di HP**:
   * Ketik angka ini di tempat biasa Anda mengetik alamat web:
     ```
     192.168.4.1
     ```
   * Tekan tombol **Cari / Masuk**. Layar kendali kebun langsung terbuka seketika!

### 3.2. Bikin Ikon di Layar Depan HP (Biar Besok Tinggal Sekali Klik)
* Di Google Chrome HP, klik titik tiga (`⋮`) di pojok kanan atas ➔ Pilih **"Tambahkan ke Layar Utama" (Add to Home screen)**.
* Ikon Smart Farm akan muncul di layar HP Anda layaknya aplikasi resmi. Besok tinggal sentuh ikon itu untuk pantau kebun!

---

## BAB 4: TAB 1 — LAYAR PANTAU UTAMA, CERMIN LCD & LAMPU KEBUN

Saat web terbuka di HP, Anda berada di **Tab 1: Monitoring**:

```
┌────────────────────────────────────────────────────────────────────────┐
│ 🟢 NORMAL   │ 💧 POMPA: MATI    │ 💡 LAMPU: MATI       │ 🕒 JAM 14:25  │
├────────────────────────────────────────────────────────────────────────┤
│ [📊 MONITORING]  [📈 GRAFIK 24J]  [⚙️ KONTROL]  [🌱 TANAMAN]  [🛰️ BMKG]│
├────────────────────────────────────────────────────────────────────────┤
│ ┌────────────────────────────────────────────────────────────────────┐ │
│ │  CERMIN LAYAR KOTAK KEBUN (DIGITAL TWIN LCD)                       │ │
│ │  ┌──────────────────────────────────────────────────────────────┐  │ │
│ │  │ [1] T:29.4C  H:68%  S:72%  [OK]                              │  │ │
│ │  │ [2] 14:25:00 | POMPA:OFF  LAMPU:OFF                          │  │ │
│ │  └──────────────────────────────────────────────────────────────┘  │ │
│ └────────────────────────────────────────────────────────────────────┘ │
│ ┌────────────────┐ ┌────────────────┐ ┌────────────────┐ ┌───────────┐ │
│ │ BASAHNYA TANAH │ │ SUHU UDARA     │ │ KABUT UDARA    │ │ KEKUATAN  │ │
│ │ (PERSEN AIR)   │ │ SEKITAR DAUN   │ │ (KELEMBAPAN)   │ │ BATERAI   │ │
│ │     72 %       │ │    29.4 °C     │ │     68 %       │ │   4.12 V  │ │
│ └────────────────┘ └────────────────┘ └────────────────┘ └───────────┘ │
│ ┌────────────────────────────────────────────────────────────────────┐ │
│ │  LAMPU LALU LINTAS LAHAN (TIANG FISIK PIN 27, 33, 32)              │ │
│ │  [MERAH: MATI]       [KUNING: MATI]        [HIJAU: MENYALA TERANG] │ │
│ └────────────────────────────────────────────────────────────────────┘ │
└────────────────────────────────────────────────────────────────────────┘
```

### 4.1. Cermin Layar Kotak Kebun (Digital Twin LCD)
* **Apa gunanya?** Ini adalah cermin langsung dari layar kecil biru yang ada di kotak alat. Jadi walau Anda sedang rebahan di kamar atau duduk di teras, Anda bisa melihat persis apa yang sedang tertulis di layar kotak kebun tanpa harus jalan kaki ke luar.
* **Layar 1**: Menampilkan Suhu (T), Kelembapan Udara (H), dan Kelembapan Tanah (S).
* **Layar 2**: Menampilkan Jam digital saat ini, serta status pompa dan lampu malam.

### 4.2. Empat Kartu Angka Utama (Cara Bacanya Super Gampang)
1. **Basahnya Tanah (72%)**: 
   * Jika angka di atas 60%: Tanah basah segar, tanaman kenyang.
   * Jika angka di bawah 45%: Tanah mulai kering kehausan, perlu disiram.
2. **Suhu Udara Sekitar Daun (29.4 °C)**:
   * Menunjukkan apakah kebun sedang sejuk atau terik panas. Kalau sudah tembus di atas 35°C, tanaman mulai kegerahan.
3. **Kabut Udara / Kelembapan (68%)**:
   * Menunjukkan seberapa lembap angin di sekitar bedengan.
4. **Kekuatan Baterai Kebun (4.12 V)**:
   * Menunjukkan isi baterai alat di bedengan. Kalau angkanya turun di bawah 3.7 Volt, saatnya ganti baterai atau cas ulang powerbank.

### 4.3. Lampu Lalu Lintas Kebun (Tiang 3 Warna)
Di tiang kebun ada lampu 3 warna. Dari kejauhan Anda cukup melirik warnanya:
* 🟢 **Lampu Hijau Menyala (Pin 32)**: **Aman Santai!** Tanah kenyang air, suhu sejuk, tanaman tumbuh tenang.
* 🟡 **Lampu Kuning Menyala (Pin 33)**: **Waspada / Siap-siap!** Tanah mulai agak kering dan sebentar lagi akan disiram, atau jam jadwal siram pagi/sore sedang aktif.
* 🔴 **Lampu Merah Berkedip (Pin 27)**: **Darurat Bahaya!** Terjadi saat hawa panas membakar ($>35^\circ$C) atau kabel alat di kebun terlepas. Pompa air otomatis dikunci mati agar kebun tidak banjir.

---

## BAB 5: TAB 2 — GRAFIK NAIK-TURUN KEBUN & KIRIM LAPORAN VIA WHATSAPP

### 5.1. Osiloskop Garis Hidup (Melihat Keringnya Tanah dari Jam ke Jam)
Di tab kedua, Anda melihat garis warna-warni yang berjalan seperti detak jantung di rumah sakit:
* **Garis Biru**: Air tanah. Anda bisa melihat bagaimana air berkurang pelan-pelan saat matahari siang terik, dan langsung melonjak naik begitu pompa menyiram.
* **Garis Oranye**: Suhu udara. Memperlihatkan puncak panas siang hari dan sejuknya malam hari.

### 5.2. Tombol Ajaib "Kirim Laporan WA"
Bagi Anda yang mengelola kebun milik juragan, kelompok tani, atau keluarga:
* Cukup klik tombol hijau bertuliskan **"Kirim Laporan WA"**.
* HP Anda akan otomatis membuka WhatsApp dengan teks laporan yang sudah tersusun rapi: tanggal, jam, umur tanaman, kondisi tanah, dan catatan apakah tanaman aman. Anda tinggal tekan kirim tanpa perlu mengetik panjang!

---

## BAB 6: TAB 3 — ATUR POMPA SIRAM OTOMATIS & LAMPU PEMANAS MALAM

Di tab ketiga, Anda bisa mengatur kapan alat harus menyiram tanaman:

### 6.1. Dua Cara Menyiram:
1. **Cara Otomatis (Sensor Air)**:
   * Anda tentukan batasnya dengan menggeser slider: *"Kalau tanah kering sampai 50%, tolong pompa dinyalakan. Begitu tanah sudah basah 75%, tolong pompa dimatikan sendiri ya."*
   * Pompa akan bekerja sendiri siang malam tanpa perlu Anda tungguin.
2. **Cara Jadwal Jam (Alarm RTC)**:
   * Anda bisa atur jam siram seperti memasang alarm di HP:
     * **Siram Pagi**: Tiap pukul 06:00 pagi selama 15 menit.
     * **Siram Sore**: Tiap pukul 17:00 sore selama 10 menit.
3. **Cara Manual (Tombol Sakelar)**:
   * Mau ngetes pipa atau nyiram dadakan? Cukup sentuh tombol **"Nyalakan Pompa"** di layar HP, pompa langsung berputar. Sentuh lagi untuk mematikan.

### 6.2. Pengaman Otomatis Anti-Banjir (Penyelamat Kebun!)
Pernahkah Anda khawatir saat ditinggal pergi, kabel sensor copot lalu pompa nyala terus sampai tanaman mati busuk dan rumah kebanjiran?
* **Alat ini punya Otak Pengaman 60 Detik**: Jika selama 1 menit alat di rumah tidak mendengar kabar dari alat di kebun, sistem langsung memutuskan: *"Gawat, sinyal hilang! Matikan pompa air sekarang juga!"*
* Kebun Anda dijamin 100% selamat dari kebanjiran.

---

## BAB 7: TAB 4 — UMUR TANAMAN (HST) & KEBUTUHAN AIR TIAP FASE

Tanaman itu seperti manusia: waktu masih bayi minumnya beda dengan saat sudah remaja dan dewasa.

### 7.1. Cukup Pilih Tanggal Tanam Sekali Saja
Di tab ini, Anda tinggal memilih tanggal kapan bibit pertama kali ditanam ke bedengan. Sistem akan menghitung sendiri: *"Hari ini tanaman bapak/ibu berumur 24 Hari Setelah Tanam (HST)"*. Data ini tidak akan hilang walau HP mati.

### 7.2. Empat Babak Hidup Tanaman:

| Babak Hidup | Umur (HST) | Hausnya Air | Yang Perlu Diperhatikan Petani |
| :--- | :--- | :--- | :--- |
| **1. Masa Semai (Bayi)** | 0 – 14 Hari | Sedang (60-70%) | Jaga tanah tetap lembap gembur. Jangan becek lumpur agar akar bayi tidak busuk. |
| **2. Masa Vegetatif (Remaja)**| 15 – 35 Hari| Banyak (65-75%) | Daun dan cabang bertambah banyak. Butuh pupuk pembentuk daun (Nitrogen). |
| **3. Masa Berbunga (Dewasa)** | 36 – 55 Hari| Paling Banyak (70-80%)| Bunga mulai bermunculan. Jangan sampai telat disiram agar bunga tidak rontok. |
| **4. Masa Buah & Panen** | 56 Hari ke atas| Dikurangi (50-60%)| Kurangi air sedikit agar rasa buah lebih manis dan tidak gampang membusuk di pohon. |

---

## BAB 8: TAB 5 — PRAKIRAAN HUJAN SATELIT BMKG (OTOMATIS TUNDA SIRAM)

### 8.1. Menyambung ke Satelit Cuaca BMKG
Alat ini terhubung dengan stasiun cuaca BMKG terdekat (wilayah Leces, Probolinggo).

### 8.2. Fitur Cerdas: Jangan Buang-Buang Listrik & Air Sumur!
* Jika satelit melihat awan mendung tebal dan memprediksi sebentar lagi akan turun hujan lebat, sistem akan memberi tanda peringatan: *"Bakal ada hujan deras sebentar lagi, penyiraman pompa ditunda dulu."*
* Mengapa harus menyiram pakai pompa dan buang listrik jika alam sebentar lagi menyiramkannya secara gratis?

---

## BAB 9: TAB 6 — TAKARAN PUPUK DAPUR (SENDOK MAKAN, GELAS AQUA, TANGKI SEMPROT)

Buku-buku pertanian sering menyuruh: *"Campurkan 3.42 gram pupuk per meter persegi"*. Di kebun mana ada yang bawa timbangan emas?
Sistem kami mengubah hitungan rumit itu menjadi **Alat Takar Nyata yang Ada di Dapur**:

| Takaran Nyata di Dapur | Sama dengan Berapa Banyak Pupuk Butiran? |
| :--- | :--- |
| 🥄 **1 Sendok Makan Peres (sdm)** | Kira-kira **10 Gram** pupuk NPK |
| 🥄 **1 Sendok Makan Munjung (sdm)**| Kira-kira **15 Gram** pupuk NPK |
| 🥛 **1 Gelas Plastik Aqua (220 ml)**| Kira-kira **1 Gelas = 150 sampai 180 Gram** pupuk NPK |
| 🪣 **1 Ember Cat / Ember Cor Kecil** | Kira-kira **10 sampai 15 Liter Air** larutan kocor |
| 🎒 **1 Tangki Semprot Gendong (Knapsack)** | Kira-kira **16 Liter Air** |
| ✋ **1 Genggam Tangan Petani** | Kira-kira **35 sampai 40 Gram** untuk pupuk tabur melingkar |

### Contoh Nyata di Layar:
Jika Anda mengetik luas bedengan cabai $25\,\text{m}^2$, layar tidak hanya memberi angka gram, tapi langsung menuliskan instruksi yang sangat gampang dipahami:
> **Petunjuk Praktis Lapangan**:  
> *"Ambil **1 Gelas Plastik Aqua munjung** pupuk NPK. Larutkan ke dalam **1 Ember Cor (15 Liter air)**, lalu aduk sampai larut. Siramkan larutan tersebut sebanyak **1 cangkir kecil** ke setiap lubang tanaman."*

---

## BAB 10: TAB 7 — DOKTER TANAMAN & CEK PENYAKIT LEWAT FOTO DAUN

Tidak perlu bingung mencari mantri atau penyuluh pertanian saat tanaman Anda mendadak sakit:

### 10.1. Tanya Jawab Bebas (Konsultasi Pakar)
* Anda bisa mengetik pertanyaan apa saja dengan bahasa Indonesia sehari-hari, misalnya:
  * *"Kenapa daun cabai saya keriting dan menguning?"*
  * *"Di kebun suhunya 32 derajat dan tanah 40%, tanaman saya harus diapain ya?"*
* **Dokter Pintar**: Jawaban yang diberikan bukan teori kosong, karena sistem langsung membaca suhu kebun dan basahnya tanah Anda saat itu juga.

### 10.2. Cek Penyakit dari Foto Daun (Kamera HP)
1. Ambil foto daun yang berbercak hitam, berjamur putih, atau bolong dimakan hama.
2. Klik tombol **"Pilih Foto Daun"** di layar HP.
3. Sistem akan memeriksa foto tersebut dan memberi tahu: jenis penyakitnya (misal: patek / antraknosa atau jamur karat daun), lengkap dengan nama obat semprot atau fungisida yang harus Anda beli di toko pertanian.

---

## BAB 11: TAB 8 — CATATAN RIWAYAT KEBUN & SIMPAN CADANGAN

### 11.1. Layar Catatan Langsung (Live Telemetry Feed)
Di tab ini Anda bisa melihat tulisan baris demi baris dari paket data yang masuk dari kebun setiap saat. Seperti struk kasir yang mencatat waktu, suhu, dan kelembapan secara berurutan.

### 11.2. Tombol Simpan Cadangan (Backup Pengaturan)
Semua setelan yang sudah Anda atur (target air, tanggal tanam, jam siram) bisa Anda unduh ke HP dalam bentuk file cadangan dengan menekan tombol **"Unduh Cadangan JSON"**. Jika suatu hari ganti HP, tinggal unggah lagi file tersebut dan semua setelan kembali seperti semula.

---

## BAB 12: PERTOLONGAN PERTAMA MASALAH LAPANGAN (TANYA-JAWAB CEPAT)

Kalau ada sesuatu yang aneh terjadi pada alat, jangan panik! Baca tabel solusi di bawah ini:

| Kejadian di Alat / Layar | Artinya Apa? | Apa yang Harus Saya Lakukan? |
| :--- | :--- | :--- |
| **Layar HP ada tulisan `--` dan "Menunggu Sensor..."** | Alat rumah sedang menunggu kiriman data pertama dari alat kebun. | Tunggu sekitar 10–15 detik. Pastikan alat di kebun baterainya sudah terpasang dan lampunya menyala. |
| **Lampu tiang Merah berkedip-kedip terus** | Sensor di kebun mati / kabelnya copot, atau hawa kebun sangat panas ($>35^\circ\text{C}$). | Cek kotak alat di kebun: apakah baterainya habis? Apakah kabelnya lepas ditarik binatang? |
| **Lampu tiang Kuning berkedip perlahan** | Alat baru saja dinyalakan listriknya (*Booting* awal). | Santai saja, itu normal. Beberapa detik lagi akan berubah hijau setelah sinyal masuk. |
| **Layar LCD biru di kotak kebun gelap / tidak ada tulisan** | Kabel listriknya kendur atau setelan kontrasnya kurang pas. | Pastikan kabel LCD dicolok ke pin **5V (VIN)**. Ambil obeng kecil minus, lalu putar baut kecil warna biru di belakang layar LCD sampai hurufnya terlihat tajam. |
| **Pompa berbunyi *"cetuk"* tapi air tidak keluar** | Sambungan kabel ke pompa air atau pipa ada yang macet. | Cek apakah colokan listrik pompa sudah terpasang ke stopkontak PLN. Cek apakah sumur ada airnya atau pipa tersumbat kotoran. |
| **HP mendadak putus dari Wi-Fi `SmartFarm_ESP32`** | HP Anda mendeteksi Wi-Fi ini tidak ada internetnya, lalu otomatis pindah ke kuota HP. | Buka menu Wi-Fi di HP, sambungkan lagi ke `SmartFarm_ESP32`. Jika ada pesan pop-up bertuliskan *"Jaringan ini tidak ada internet, tetap sambungkan?"*, centang pilihan **"Ya / Tetap Terhubung"**. |
| **Bilah sensor tanah berkerak putih** | Ada sisa garam pupuk yang menempel di lempeng sensor. | Cabut sensor tanah, lap lempeng hitamnya dengan kain lap basah sampai bersih, lalu tancapkan kembali ke tanah. |

---

## BAB 13: PANDUAN LENGKAP ARDUINO IDE & SOLUSI DRIVER PORT COM LAPTOP

Banyak orang awam mengira mengisi program ke mikrokontroler itu sesulit meretas komputer. Padahal kenyataannya: **semudah menyalin teks dan menekan tombol panah di layar komputer!**

### 13.1. Langkah 1: Download & Pasang Arduino IDE di Komputer / Laptop
1. Buka browser komputer Anda dan kunjungi situs resmi:
   ```
   https://www.arduino.cc/en/software
   ```
2. Pilih versi komputer Anda (**Windows**, **macOS**, atau **Linux**).
3. Klik tombol **"Just Download"** (Gratis 100%), lalu pasang seperti memasang aplikasi biasa.

---

### 13.2. Langkah 2: Mengenalkan ESP32 dan ESP8266 ke Arduino IDE
Secara bawaan, Arduino IDE hanya kenal papan Arduino biasa. Kita perlu memberitahunya alamat untuk mengunduh papan pintar ESP:
1. Buka Arduino IDE di komputer Anda.
2. Klik menu di kiri atas: **File ➔ Preferences** (atau tekan `Ctrl + ,`).
3. Pada kotak isian bernama **"Additional boards manager URLs"**, salin dan tempelkan 2 alamat berikut (pisahkan dengan tanda koma):
   ```
   https://espressif.github.io/arduino-esp32/package_esp32_index.json, http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
4. Klik tombol **OK**.
5. Sekarang buka menu: **Tools ➔ Board ➔ Boards Manager**.
   * Di kotak pencarian, ketik: `esp32` ➔ Klik tombol **Install**.
   * Di kotak pencarian, ketik: `esp8266` ➔ Klik tombol **Install**.
   * Tunggu sampai selesai. Sekarang komputer Anda sudah kenal kedua papan kebun pintar ini!

---

### 13.3. Langkah 3: Memasang 4 Buku Panduan Tambahan (Library)
Library adalah kode bantuan agar Arduino bisa langsung bicara dengan layar LCD, sensor suhu, dan jam RTC:
1. Di Arduino IDE, klik menu: **Sketch ➔ Include Library ➔ Manage Libraries...** (atau klik ikon buku di bilah kiri).
2. Cari dan klik tombol **Install** untuk 4 nama berikut satu per satu:
   1. `LiquidCrystal_I2C` (pilih karya Frank de Brabander)
   2. `Rtc by Makuna` (pilih karya Michael C. Miller)
   3. `DHT sensor library` (pilih karya Adafruit)
   4. `Adafruit Unified Sensor` (pilih karya Adafruit)

---

### 13.4. 💡 Tips Penyelamat: "Kenapa Port COM Tidak Muncul di Laptop?" (Driver USB CH340 / CP2102)
Ini adalah masalah nomor satu yang dialami 90% pemula: kabel USB sudah dicolok ke laptop, tetapi menu **Tools ➔ Port** di Arduino IDE berwarna abu-abu redup atau tidak bisa diklik!

* **Penyebabnya**: Laptop Anda belum punya driver chip penerjemah USB (kebanyakan papan ESP buatan pabrik menggunakan chip **CH340** atau **CP2102**).
* **Solusi Cepat 1 Menit**:
  1. Cari di Google: *"Download Driver CH340 Windows"* (atau buka situs resmi WCH: `https://www.wch.cn/downloads/CH341SER_ZIP.html`).
  2. Ekstrak file zip, lalu buka file `SETUP.EXE` dan klik tombol **INSTALL**.
  3. *(Jika papan Anda memakai chip persegi kecil bertuliskan SILABS CP2102)*: Cari di Google *"CP210x Universal Windows Driver"* dari Silicon Labs, lalu klik install.
  4. Cabut kabel USB papan ESP dari laptop, lalu colokkan kembali.
  5. Buka lagi menu **Tools ➔ Port**: Seketika nama port (misalnya **COM3**, **COM4**, atau **COM5**) akan langsung muncul berwarna hitam dan siap dipilih!

> 💡 **Penting Juga**: Gunakan kabel data USB yang bagus (yang biasa dipakai untuk transfer file foto dari HP ke laptop). Jangan gunakan kabel murahan yang hanya berfungsi untuk mengisi daya listrik saja (*charging-only cable*), karena kabel charger biasa tidak memiliki jalur kabel data di dalamnya.

---

### 13.5. Langkah 4: Cara Memasukkan Program ke Papan (Flashing / Upload)
1. Colokkan kabel USB papan ESP ke lubang laptop.
2. Di Arduino IDE:
   * **Untuk Kotak Kebun (ESP8266)**: Buka menu **Tools ➔ Board** ➔ Pilih **NodeMCU 1.0 (ESP-12E Module)** atau **LOLIN(WEMOS) D1 R2 & mini**.
   * **Untuk Kotak Rumah (ESP32)**: Buka menu **Tools ➔ Board** ➔ Pilih **DOIT ESP32 DEVKIT V1**.
   * **Pilih Lubang Port (COM)**: Buka menu **Tools ➔ Port** ➔ Pilih port yang sudah terdeteksi (misal `COM3` atau `COM4`).
3. Klik tombol **Tanda Panah Kanan (➔) [Upload]** di pojok kiri atas jendela Arduino IDE.
4. *(Khusus beberapa papan ESP32)*: Jika saat upload di layar bawah muncul tulisan `Connecting........_____.....`, cukup **tekan dan tahan tombol kecil bertuliskan "BOOT"** di papan ESP32 selama 2 detik sampai proses persentase upload mulai berjalan, lalu lepas tombolnya.
5. Tunggu sampai muncul tulisan warna hijau:
   ```
   Done uploading.
   ```
6. **Selamat!** Program sudah tertanam permanen di dalam otak alat Anda dan tidak akan hilang walau listrik dicabut!

---

### 13.6. Langkah 5: Memeriksa Kejujuran Alat via Serial Monitor (115200 Baud)
Ingin melihat apa yang sedang dipikirkan oleh alat Anda?
1. Di Arduino IDE, klik ikon kaca pembesar di pojok kanan atas bernama **Serial Monitor** (atau tekan `Ctrl + Shift + M`).
2. Di pojok kanan bawah jendela Serial Monitor, pastikan kecepatannya dipilih: **115200 baud**.
3. Layar komputer akan langsung mencetak data riil setiap detik:
   ```
   [ESP8266 SENDER AKURAT] Raw ADC: 380 | Soil: 68% | Status Transmisi: SUKSES KIRIM DATA
   ```
   Jika teks ini sudah mengalir, tandanya alat kebun Anda sudah 100% sehat dan siap dipasang di bedengan!

---

## BAB 14: KODE LENGKAP KOTAK KEBUN (ESP8266 SENDER) & CARA KERJANYA

Berikut adalah seluruh isi kode program untuk kotak pemancar di bedengan tanah ([esp8266_soil_sender.ino](file:///media/budgei/Development2/arduino/Sketchbook/esp8266_soil_sender/esp8266_soil_sender.ino)). Kode ini sangat ringkas (kurang dari 100 baris) sehingga pemula pun bisa membacanya dengan sangat mudah:

```cpp
#include <ESP8266WiFi.h>
#include <espnow.h>
extern "C" {
#include <user_interface.h>
}

#define LED_BUILTIN_PIN 2 // Lampu kecil biru di papan ESP8266

// Alamat Siaran Bersama (Broadcast) ke Seluruh Udara Kebun
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Amplop Surat Data (Harus Persis Sama dengan Kotak Rumah ESP32)
typedef struct struct_message {
  int8_t persen;   // Basahnya tanah (0 sampai 100%)
  int8_t baterai;  // Estimasi baterai kebun (0 sampai 100%)
  uint16_t rawAdc; // Angka mentah sensor tanah (0 sampai 1024)
} struct_message;

struct_message myData;
volatile int lastSendStatus = -1;

void OnDataSent(uint8_t *mac_addr, uint8_t status) { 
  lastSendStatus = status; 
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN_PIN, OUTPUT);
  digitalWrite(LED_BUILTIN_PIN, HIGH); // Lampu mati awal

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // DONGKRAK DAYA PANCAR RADIO KE MAKSIMAL (Tembus Pepohonan s.d 300 Meter)
  WiFi.setOutputPower(20.5);
  wifi_set_phy_mode(PHY_MODE_11B); // Gelombang radio panjang tembus dinding
  wifi_set_channel(1);            // Kanal 1 (Wajib sama dengan ESP32 di rumah!)

  if (esp_now_init() != 0) {
    Serial.println(F("Gagal menghidupkan radio ESP-NOW"));
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {
  // 1. MEMBACA TANAH 5 KALI BIAR HASILNYA TENANG & TIDAK GOYANG
  long sumAdc = 0;
  for (int i = 0; i < 5; i++) {
    sumAdc += analogRead(A0);
    delay(2);
  }
  int sensorValue = sumAdc / 5;

  // 2. RUMUS KALIBRASI TANAH KEBUN ANDA:
  // Angka 480 = Tanah Kering (0%), Angka 200 = Tanah Tergenang Air (100%)
  const int dryValue = 480;
  const int wetValue = 200;

  int persenLembab = map(sensorValue, dryValue, wetValue, 0, 100);
  persenLembab = constrain(persenLembab, 0, 100);

  // Masukkan angka ke amplop surat
  myData.persen = (int8_t)persenLembab;
  myData.baterai = 100; // Tegangan stabil adaptor 5V
  myData.rawAdc = (uint16_t)sensorValue;

  // 3. KEDIPKAN LAMPU BIRU & TEMBAKKAN SURAT DATA KE UDARA
  lastSendStatus = -1;
  digitalWrite(LED_BUILTIN_PIN, LOW); // Lampu biru menyala sebentar
  esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
  delay(20);
  digitalWrite(LED_BUILTIN_PIN, HIGH); // Lampu biru mati lagi

  Serial.print(F("Data Terkirim -> Tanah: "));
  Serial.print(persenLembab);
  Serial.println(F("%"));

  delay(2000); // Istirahat 2 detik, lalu kirim lagi
}
```

### 💡 Penjelasan Baris Kode Penting untuk Orang Awam:
1. **`broadcastAddress = {0xFF, ...}`**: Artinya surat data dilempar ke udara secara terbuka (*Broadcast*). Anda tidak perlu repot mencari alamat MAC Address atau nomor seri papan.
2. **`wifi_set_channel(1)`**: Menetapkan bahwa gelombang radio berjalan di **Kanal 1**. Ini kunci rahasianya! Kotak rumah ESP32 juga berada di Kanal 1 sehingga keduanya langsung nyambung seketika.
3. **`dryValue = 480` dan `wetValue = 200`**: Ini adalah batas kalibrasi tanah Anda. Kalau di kebun Anda saat kering terbaca `520`, Anda tinggal mengganti angka `480` menjadi `520`!
4. **`delay(2000)`**: Mengatur jeda pengiriman setiap 2 detik sekali agar hemat daya dan tidak boros baterai.

---

## BAB 15: KODE LENGKAP KOTAK RUMAH (ESP32 GATEWAY) & KOMPILASI WEB OTOMATIS

Kotak rumah dikendalikan oleh file [esp32_server_receiver.ino](file:///media/budgei/Development2/arduino/Sketchbook/esp32_server_receiver/esp32_server_receiver.ino). File ini adalah stasiun pusat komando yang sangat cerdas:
1. **Penerima Radio ESP-NOW**: Menangkap paket data kelembapan tanah dan daya baterai dari kotak kebun tanpa router.
2. **Sensor Suhu Udara DHT11 (Pin 4)**: Mengukur suhu lingkungan dan kelembapan udara.
3. **Layar LCD 16x2 I2C (Pin 21 SDA, Pin 22 SCL)**: Menampilkan status fisik dengan 7 karakter grafis kustom CGRAM.
4. **Jam Digital RTC DS1302 (Pin 14 DAT, Pin 12 CLK, Pin 13 RST)**: Menjaga jadwal siram pagi & sore tetap akurat walau listrik padam.
5. **Relai Pompa Air (Pin 26) & Relai Lampu Pemanas (Pin 25)**: Sakelar otomatis dengan logika *Active-Low*.
6. **Lampu Tiang Fisik 3 Warna**: Pin 32 (Hijau/Aman), Pin 33 (Kuning/Standby-Waspada), Pin 27 (Merah/Darurat Panas-Putus).
7. **Penyedia Wi-Fi Mandiri & Web SCADA**: Memancarkan SSID `SmartFarm_ESP32` dan menyajikan dashboard SCADA ke HP Anda.

---

### 15.1. Cara Otomatis Meracik Tampilan Web (build_web.py)
Sebelum meng-upload kode program C++ di bawah ke papan ESP32, seluruh tampilan web di folder `src_web/` dikemas menjadi satu file header `index.h`. Anda **tidak perlu** mengedit kode HTML/CSS secara manual. Cukup jalankan perintah otomatis ini di terminal komputer:
```bash
python3 esp32_server_receiver/build_web.py
```
Dalam waktu 1 detik, script akan otomatis menghasilkan file `index.h` yang memuat seluruh antarmuka web, CSS modern, dan JavaScript.

---

### 15.2. KODE SUMBER LENGKAP: esp32_server_receiver.ino
Berikut adalah seluruh isi kode program lengkap stasiun utama ESP32 yang bisa langsung disalin (*copy-paste*) ke Arduino IDE:

```cpp
#include "index.h"
#include <DHT.h>
#include <LittleFS.h>
#include <WebServer.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h> // UNTUK LOCK CHANNEL RADIO PHY ESP32 & DONGKRAK POWER MAKSIMAL

#include <Preferences.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

Preferences preferences;
ThreeWire myWire(14, 12, 13); // DAT/IO, CLK/SCLK, RST/CE
RtcDS1302<ThreeWire> Rtc(myWire);

// --- KONFIGURASI LCD 16X2 I2C (PIN 21 SDA, PIN 22 SCL) ---
#define LCD_SDA_PIN 21
#define LCD_SCL_PIN 22
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

uint8_t lamp_sched_en = 1;
uint8_t lamp_on_h = 18;
uint8_t lamp_on_m = 0;
uint8_t lamp_dur = 12; // dalam jam



// --- KONFIGURASI DHT, RELAY (PIN 26 KHUSUS) & TRAFFIC LIGHT ---
#define DHTPIN 4
#define DHTTYPE DHT11

// RELAY PADA PIN 26 (DS1 / RELAY PIN 26)
#define RELAY1 26
#define RELAY2 25 // LAMPU PENERANGAN

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
    file.print(satRainPred);
    file.print(',');
    file.println(isLampOn ? "1" : "0");
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
    esp8266Rssi = -99;
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
      esp8266Rssi = -99;
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

  if (isStartupWaiting) {
    bgClass = F("peringatan");
    textStatus = F("⏳ INISIALISASI: Menunggu Sinyal ESP8266...");
  } else if (isEsp8266Unplugged || latestMoisturePercent < 0) {
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
  json.reserve(1200);
  json = "{";
  if (isDhtValid) {
    json += "\"suhuC\":\"" + String(lastSuhuC, 1) + "\",";
    json += "\"suhuF\":\"" + String(lastSuhuF, 1) + "\",";
    json += "\"hum\":\"" + String(lastKelembapanUdara, 1) + "\",";
    json += "\"heatC\":\"" + String(lastHeatIndexC, 1) + "\",";
    json += "\"heatF\":\"" + String(lastHeatIndexF, 1) + "\",";
    json += "\"dew\":\"" + String(lastDewPoint, 1) + "\",";
  } else {
    json += "\"suhuC\":\"--\",";
    json += "\"suhuF\":\"--\",";
    json += "\"hum\":\"--\",";
    json += "\"heatC\":\"--\",";
    json += "\"heatF\":\"--\",";
    json += "\"dew\":\"--\",";
  }
  if (isEsp8266Unplugged || latestMoisturePercent < 0) {
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
  json += "\"statusColor\":\"" + bgClass + "\",";
  json += "\"statusText\":\"" + textStatus + "\",";
  json += "\"errorMsg\":\"" + systemErrorMsg + "\",";
  json += "\"isWaiting\":" + String(isStartupWaiting ? 1 : 0) + ",";
  json += "\"waitingPercent\":" + String(waitingPercent) + ",";
  json += "\"rtcTime\":\"" + rtcTimeStr + "\",";
  json += "\"rtcValid\":" + String(rtcValid) + ",";
  json += "\"rtcRunning\":" + String(rtcRunning) + ",";
  json += "\"satTemp\":\"" + satTemp + "\",";
  json += "\"satDesc\":\"" + satDesc + "\",";
  json += "\"satRainPred\":\"" + satRainPred + "\",";
  json += "\"satTime\":\"" + satTime + "\",";
  json += "\"tempDiff\":\"" + String(tempDiff, 1) + "\",";
  json += "\"isManual\":" + String(isManualMode ? 1 : 0) + ",";
  json += "\"relayOn\":" + String(isRelayOn ? 1 : 0) + ",";
  json += "\"lampOn\":" + String(isLampOn ? 1 : 0) + ",";
  json += "\"lampManual\":" + String(isLampManualMode ? 1 : 0) + ",";
  json += "\"l_en\":" + String(lamp_sched_en) + ",";
  json += "\"l_h\":" + String(lamp_on_h) + ",";
  json += "\"l_m\":" + String(lamp_on_m) + ",";
  json += "\"l_dur\":" + String(lamp_dur) + ",";
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
  Serial.println(F(" [LITTLEFS] Berkas Log & Seluruh Counter Akumulasi Pompa "
                   "Berhasil Direset Total!"));
  server.send(200, "text/plain",
              F("Log Storage & Counter Pompa Berhasil Direset Total!"));
}

void handleSetThreshold() {
  if (server.hasArg("soil") && server.hasArg("temp")) {
    batasTanah = server.arg("soil").toInt();
    batasSuhu = server.arg("temp").toFloat();
    preferences.putInt("batasTanah", batasTanah);
    preferences.putFloat("batasSuhu", batasSuhu);
    server.send(200, "text/plain", "Threshold Berhasil Disimpan di NVS!");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}


// ================= HANDLER GROW LIGHT =================
void handleSetLampMode() {
  if (server.hasArg("m")) {
    String m = server.arg("m");
    if (m == "manual") {
      isLampManualMode = true;
    } else {
      isLampManualMode = false;
    }
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Missing arg m");
  }
}

void handleToggleLamp() {
  if (server.hasArg("s")) {
    String s = server.arg("s");
    if (s == "on") {
      manualLampState = true;
    } else {
      manualLampState = false;
    }
    // Update PIN langsung jika mode manual aktif
    if (isLampManualMode) {
      digitalWrite(RELAY2, manualLampState ? LOW : HIGH); // LOW = ON
      isLampOn = manualLampState;
    }
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Missing arg s");
  }
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
    
    server.send(200, "text/plain", "Jadwal Lampu Tersimpan");
  } else {
    server.send(400, "text/plain", "Missing args");
  }
}

void updateLampState() {
  if (isLampManualMode) {
    // Mode manual diurus saat handleToggleLamp dipanggil, 
    // Tapi untuk memastikan pin sinkron dengan manualLampState:
    digitalWrite(RELAY2, manualLampState ? LOW : HIGH);
    isLampOn = manualLampState;
  } else {
    // Mode Otomatis RTC
    if (lamp_sched_en) {
      RtcDateTime now = Rtc.GetDateTime();
      if (now.IsValid()) {
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
      }
    } else {
      // Auto, tapi schedule mati
      digitalWrite(RELAY2, HIGH); // OFF
      isLampOn = false;
    }
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
bool isRtcScheduleActive = false;

void updateRelayState() {
  bool butuhON = false;
  isRtcScheduleActive = false;

  // AUTO SCHEDULER RTC LOGIC
  if (!isManualMode && Rtc.GetIsRunning() && Rtc.IsDateTimeValid()) {
    RtcDateTime now = Rtc.GetDateTime();
    uint8_t h = now.Hour();
    uint8_t m = now.Minute();
    
    // Check Slot 1
    if (sched1_en && h == sched1_h && m >= sched1_m && m < (sched1_m + sched1_dur)) {
       isRtcScheduleActive = true;
    }
    // Check Slot 2
    if (sched2_en && h == sched2_h && m >= sched2_m && m < (sched2_m + sched2_dur)) {
       isRtcScheduleActive = true;
    }
  }

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

// ledState: 0=semua mati, 1=merah, 2=kuning, 3=hijau, 4=rtc-cycle
// Nilai ini mencerminkan sinyal GPIO AKTUAL yang diperintahkan firmware.
uint8_t ledState = 0;

// ==== LOGIKA SEJATI INDIKATOR LAMPU TRAFFIC LIGHT (ACTIVE HIGH / COMMON GND)
// ====
void updateLEDState() {
  if (isRtcScheduleActive) {
    // JADWAL RTC AKTIF -> LAMPU TRAFFIC MENYALA GANTIAN (MERAH -> KUNING -> HIJAU)
    unsigned long cycle = (millis() / 500) % 3; // Ganti setiap 500ms
    digitalWrite(LED_MERAH, (cycle == 0) ? HIGH : LOW);
    digitalWrite(LED_KUNING, (cycle == 1) ? HIGH : LOW);
    digitalWrite(LED_HIJAU, (cycle == 2) ? HIGH : LOW);
    ledState = 4; // 4 = rtc-cycle
  } else if (isSystemError || isEsp8266Unplugged || latestMoisturePercent < 0) {
    // 1. SINYAL HILANG / TERPUTUS / DICABUT -> LAMPU MERAH BERKEDIP (250ms)!
    digitalWrite(LED_HIJAU, LOW);  // OFF
    digitalWrite(LED_KUNING, LOW); // OFF
    digitalWrite(LED_MERAH,
                 ((millis() / 250) % 2) ? HIGH : LOW); // HIGH = NYALA
    ledState = 1; // 1 = merah
  } else if (isStartupWaiting) {
    // 2. AWAL BOOT / MENUNGGU KONEKSI (WAITING) -> LAMPU KUNING BERKEDIP
    // (500ms)!
    digitalWrite(LED_HIJAU, LOW); // OFF
    digitalWrite(LED_MERAH, LOW); // OFF
    digitalWrite(LED_KUNING,
                 ((millis() / 500) % 2) ? HIGH : LOW); // HIGH = NYALA
    ledState = 2; // 2 = kuning
  } else {
    // 3. KONDISI 1: BAHAYA PANAS (>= 35.0 C) -> LAMPU MERAH SOLID ON
    if (lastSuhuC >= suhuBahaya) {
      digitalWrite(LED_HIJAU, LOW);  // OFF
      digitalWrite(LED_KUNING, LOW); // OFF
      digitalWrite(LED_MERAH, HIGH); // HIGH = NYALA SOLID
      ledState = 1; // 1 = merah
    }
    // 4. KONDISI 2: PERINGATAN MENYIRAM (> 30.0 C ATAU TANAH < 45%) -> LAMPU
    // KUNING SOLID ON
    else if (lastSuhuC > batasSuhu || latestMoisturePercent < batasTanah) {
      digitalWrite(LED_HIJAU, LOW);   // OFF
      digitalWrite(LED_MERAH, LOW);   // OFF
      digitalWrite(LED_KUNING, HIGH); // HIGH = NYALA SOLID
      ledState = 2; // 2 = kuning
    }
    // 5. KONDISI 3: AMAN (SUHU <= 30.0 C DAN TANAH >= 45%) -> LAMPU HIJAU SOLID
    // ON
    else {
      digitalWrite(LED_KUNING, LOW); // OFF
      digitalWrite(LED_MERAH, LOW);  // OFF
      digitalWrite(LED_HIJAU, HIGH); // HIGH = NYALA SOLID
      ledState = 3; // 3 = hijau
    }
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
    
    RtcDateTime dt(y, m, d, h, min, s);
    Rtc.SetDateTime(dt);
    
    server.send(200, "application/json", "{\"status\":\"ok\",\"rtcTime\":\"" + String(h) + ":" + String(min) + "\"}");
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
    } else if (slot == 2) {
      sched2_en = server.arg("en").toInt();
      sched2_h = server.arg("h").toInt();
      sched2_m = server.arg("m").toInt();
      sched2_dur = server.arg("dur").toInt();
      preferences.putUChar("s2_en", sched2_en);
      preferences.putUChar("s2_h", sched2_h);
      preferences.putUChar("s2_m", sched2_m);
      preferences.putUChar("s2_dur", sched2_dur);
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
    server.send(200, "application/json", "{\"status\":\"ok\"}");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

void handleSetCropProfile() {
  if (server.hasArg("mode")) {
    cropMode = server.arg("mode").toInt();
    preferences.putUChar("cropMode", cropMode);
    server.send(200, "application/json", "{\"status\":\"ok\"}");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
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
    Serial.println(F("ℹ️ [LCD 16x2] Modul I2C tidak terdeteksi di Pin 21/22. Sistem berjalan tanpa LCD."));
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

  // Rotasi halaman tiap 4 detik (jika tidak sedang darurat / menyiram)
  if (now - lastLcdPageRotate >= 4000) {
    lcdScreenPage = (lcdScreenPage + 1) % 2;
    lastLcdPageRotate = now;
  }

  // JIKA SENSOR KEBUN TERPUTUS (PRIORITAS DARURAT)
  if (isEsp8266Unplugged) {
    lcd->setCursor(0, 0);
    lcd->print(F("!  PERINGATAN  !"));
    lcd->setCursor(0, 1);
    lcd->print(F("SENSOR TERPUTUS "));
    return;
  }

  // JIKA SUHU EKSTREM BAHAYA (>= 35 C)
  if (isDhtValid && lastSuhuC >= suhuBahaya) {
    lcd->setCursor(0, 0);
    lcd->print(F("! BAHAYA  SUHU !"));
    lcd->setCursor(0, 1);
    char bufHot[17];
    snprintf(bufHot, sizeof(bufHot), "Suhu:%.1fC PANAS", lastSuhuC);
    lcd->print(bufHot);
    return;
  }

  bool blinkState = ((now / 500) % 2 == 0); // Kedipan animasi 0.5 detik

  if (isRelayOn || lcdScreenPage == 0) {
    // ========================================================
    // LAYAR 1: TELEMETRI UTAMA 100% SERBA IKON
    // Baris 1: 🌡28.4C 💧74%  AUTO  (Pas 16 Kolom)
    // Baris 2: 🌱58%  ⚙OFF  💡OFF  (Pas 16 Kolom)
    // ========================================================

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
    // Mode kerja murni tanpa kurung: AUTO vs MAN
    if (isManualMode) {
      lcd->print(F(" MAN"));
    } else {
      lcd->print(F("AUTO"));
    }

    // --- BARIS 2: TANAH, POMPA, & LAMPU ---
    lcd->setCursor(0, 1);
    lcd->write(byte(2)); // 🌱 Icon Tunas/Tanah
    if (latestMoisturePercent >= 0) {
      char tBuf[5];
      snprintf(tBuf, sizeof(tBuf), "%-3d%%", latestMoisturePercent);
      lcd->print(tBuf);
    } else {
      lcd->print(F("--% "));
    }

    lcd->print(F("  "));
    lcd->write(byte(3)); // ⚙️ Icon Pompa
    if (isRelayOn) {
      if (blinkState) lcd->print(F("ON "));
      else            lcd->print(F("   ")); // Animasi kedip saat menyiram
    } else if (relayCooldown) {
      lcd->print(F("CLD"));
    } else {
      lcd->print(F("OFF"));
    }

    lcd->print(F("  "));
    lcd->write(byte(4)); // 💡 Icon Lampu
    if (isLampOn) {
      lcd->print(F("ON "));
    } else {
      lcd->print(F("OFF"));
    }

  } else {
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
    if (!isEsp8266Unplugged && latestMoisturePercent >= 0) {
      char rBuf[6];
      snprintf(rBuf, sizeof(rBuf), "%3ddB", esp8266Rssi);
      lcd->print(rBuf);
    } else {
      lcd->print(F(" --dB"));
    }
    lcd->print(F(" 192.168.4.1"));
  }
}

void setup() {
  Serial.begin(115200);
  initLcd16x2(); // Inisialisasi LCD 16x2 I2C Otomatis

  // Initialize NVS Preferences
  preferences.begin("smartfarm", false);
  pumpLph = preferences.getUShort("pumpLph", 1800);
  pumpWatt = preferences.getUShort("pumpWatt", 25);
  plnTariff = preferences.getUShort("plnTariff", 415);
  cropMode = preferences.getUChar("cropMode", 0);
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

  lamp_sched_en = preferences.getUChar("l_en", 1);
  lamp_on_h = preferences.getUChar("l_h", 18);
  lamp_on_m = preferences.getUChar("l_m", 0);
  lamp_dur = preferences.getUChar("l_dur", 12);


  // Initialize RTC
  Rtc.Begin();
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
  server.begin();
}

void loop() {
  server.handleClient();

  checkSystemStatus();

  updateRelayState();

  updateLEDState();

  updateLcdDisplay(); // Refresh LCD 16x2 Real-Time & Non-Blocking

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

```

---

### 15.3. Penjelasan 3 Bagian Kunci Kode ESP32 untuk Pemula:
1. **Bagian Konfigurasi Pin & Variabel NVS (Baris Awal)**:
   * Menentukan kaki pin hardware: DHT11 di Pin 4, I2C LCD di Pin 21 & 22, RTC di Pin 14/12/13, Pompa di Pin 26, Lampu di Pin 25, dan LED tiang di Pin 32/33/27.
   * Variabel `sched1_h = 6` dan `sched2_h = 17` adalah jadwal jam siram pagi (06:00) dan sore (17:00).
2. **Bagian Web Server & Jalur Data JSON (Fungsi handleData, handleControl, dll)**:
   * Mengirimkan data sensor ke HP dalam bentuk JSON setiap kali HP meminta pembaruan data secara real-time.
   * Menerima perintah sentuh dari tombol di HP saat Anda ingin menyalakan pompa atau mengubah jadwal jam.
3. **Bagian setup() dan loop() (Jantung Otomatisasi Kebun)**:
   * `setup()`: Menginisialisasi sensor DHT11, LCD 16x2, jam RTC, radio ESP-NOW, dan server web.
   * `loop()`: Berputar tanpa henti memeriksa apakah tanah kebun sudah kering di bawah ambang batas (otomatis hidupkan pompa), mengecek apakah sudah jam 6 pagi / jam 5 sore (otomatis siram jadwal), dan memeriksa apakah sensor terputus lebih dari 60 detik (*Watchdog Failsafe* otomatis matikan pompa).

---

## BAB 16: PANDUAN MENGUBAH NAMA WIFI, SANDI, & JAM SIRAM SESUAI KEINGINAN

Anda ingin mengubah nama WiFi dari `SmartFarm_ESP32` menjadi `Kebun_Pak_Haji`? Atau ingin mengganti kata sandinya? Sangat mudah!

### 16.1. Mengubah Nama Wi-Fi & Kata Sandi:
1. Buka file [esp32_server_receiver.ino](file:///media/budgei/Development2/arduino/Sketchbook/esp32_server_receiver/esp32_server_receiver.ino) di Arduino IDE.
2. Cari baris kode berikut (gunakan `Ctrl + F` dan ketik `WiFi.softAP`):
   ```cpp
   WiFi.softAP("SmartFarm-ESP32", "12345678", 1);
   ```
3. Ubah teks di dalam tanda petik sesuai nama kebun dan sandi yang Anda inginkan, misalnya:
   ```cpp
   WiFi.softAP("Kebun_Berkah_Cabai", "kebun12345", 1);
   ```
   > ⚠️ **PENTING**: Angka `, 1);` di bagian ujung **JANGAN DIUBAH!** Itu adalah nomor Kanal Radio 1 agar tetap nyambung dengan kotak pengirim di kebun.
4. Tekan tombol **Upload (➔)**. Selesai! Sekarang nama Wi-Fi di HP Anda sudah berubah.

### 16.2. Mengubah Jadwal Jam Siram Otomatis:
Anda punya dua pilihan yang sangat fleksibel:
* **Pilihan 1 (Paling Gampang, Lewat Layar HP)**: Buka tab **[⚙️ Kontrol]** di HP Anda, ubah jam siram pagi/sore pada kolom yang tersedia, lalu klik tombol **Simpan**. Jadwal otomatis tersimpan permanen di memori alat tanpa perlu colok kabel ke laptop!
* **Pilihan 2 (Lewat Kode Program)**: Di dalam [esp32_server_receiver.ino](file:///media/budgei/Development2/arduino/Sketchbook/esp32_server_receiver/esp32_server_receiver.ino), cari baris konfigurasi default:
  ```cpp
  uint8_t sched1_h = 6;  // Pukul 06 pagi
  uint8_t sched1_m = 0;  // Menit 00
  uint8_t sched1_dur = 15; // Siram selama 15 menit
  ```
  Ubah angka tersebut sesuai kebiasaan berkebun Anda, lalu tekan tombol Upload.

---

## 🌻 PESAN PENUTUP
Teknologi kebun modern ini dibuat bukan untuk membuat petani bingung dengan rumus rumit, melainkan untuk menjadi **sahabat setia di lahan**: menjaga tanaman tetap subur, menghemat pemakaian air dan listrik pompa, serta melipatgandakan hasil panen bapak dan ibu petani sekalian.

*Selamat berkebun pintar, semoga panduan ini bermanfaat & panen Anda berlimpah ruah!*
