#ifndef INDEX_H
#define INDEX_H

#include <Arduino.h>

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="id">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Smart Farm SCADA Enterprise Control Center</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; font-family: 'Segoe UI', Tahoma, sans-serif; }
    body { background: #070a12; color: #f8fafc; padding: 15px; }
    .card { background: rgba(30, 41, 59, 0.6); backdrop-filter: blur(12px); border: 1px solid rgba(255,255,255,0.08); padding: 20px; border-radius: 16px; box-shadow: 0 10px 30px rgba(0,0,0,0.5); margin-bottom: 20px; }
    h1 { color: #38bdf8; text-align: center; font-size: 26px; text-shadow: 0 0 15px rgba(56,189,248,0.4); margin-bottom: 15px; }
    .value { font-size: 26px; font-weight: bold; color: #38bdf8; }
    .status { font-size: 18px; font-weight: bold; padding: 14px; border-radius: 12px; text-align: center; color: white; margin-bottom:15px; text-shadow: 0 2px 4px rgba(0,0,0,0.4); }
    .aman { background: linear-gradient(135deg, #10b981, #059669); box-shadow: 0 4px 15px rgba(16,185,129,0.4); }
    .peringatan { background: linear-gradient(135deg, #f59e0b, #d97706); box-shadow: 0 4px 15px rgba(245,158,11,0.4); }
    .bahaya { background: linear-gradient(135deg, #ef4444, #dc2626); box-shadow: 0 4px 15px rgba(239,68,68,0.4); }
    
    .btn { padding: 12px 18px; border: none; border-radius: 10px; color: white; font-size: 14px; font-weight: bold; cursor: pointer; margin-right: 5px; margin-bottom: 10px; flex: 1 1 calc(50% - 10px); min-width: 120px; text-align: center; transition: all 0.2s; text-decoration: none; display: inline-block; }
    .btn:active { transform: scale(0.95); }
    .btn-auto { background: linear-gradient(135deg, #0284c7, #0369a1); }
    .btn-manual { background: linear-gradient(135deg, #ea580c, #c2410c); }
    .btn-on { background: linear-gradient(135deg, #059669, #047857); }
    .btn-off { background: linear-gradient(135deg, #dc2626, #991b1b); }
    .btn-sat { background: linear-gradient(135deg, #7c3aed, #6d28d9); }
    .btn-sync { background: linear-gradient(135deg, #0d9488, #0f766e); }
    .btn-danger { background: linear-gradient(135deg, #b91c1c, #7f1d1d); }
    .btn-reboot { background: linear-gradient(135deg, #d97706, #92400e); }
    .flex-container { display: flex; flex-wrap: wrap; gap: 10px; margin-bottom: 10px; }
    .section-title { font-size: 13px; color: #38bdf8; font-weight: bold; text-transform: uppercase; letter-spacing: 1.5px; margin-bottom: 12px; border-bottom: 1px solid rgba(255,255,255,0.08); padding-bottom: 6px; display: flex; align-items: center; gap: 8px; }
    
    .stat-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(130px, 1fr)); gap: 10px; }
    .stat-box { background: rgba(15, 23, 42, 0.7); padding: 12px; border-radius: 10px; border: 1px solid rgba(255,255,255,0.05); }
    .stat-label { font-size: 11px; color: #94a3b8; }
    .stat-val { font-size: 15px; font-weight: bold; color: #f1f5f9; margin-top: 4px; }
    .sub-note { font-size: 10px; margin-top: 5px; font-weight: 500; display: block; }

    .progress-bg { background: rgba(255,255,255,0.1); border-radius: 6px; height: 10px; width: 100%; margin-top: 6px; overflow: hidden; }
    .progress-fill { height: 100%; border-radius: 6px; transition: width 0.5s ease-in-out; }
    .fill-green { background: linear-gradient(90deg, #10b981, #34d399); }
    .fill-yellow { background: linear-gradient(90deg, #f59e0b, #fbbf24); }
    .fill-red { background: linear-gradient(90deg, #ef4444, #f87171); }
    .fill-blue { background: linear-gradient(90deg, #0284c7, #38bdf8); }
    .fill-purple { background: linear-gradient(90deg, #8b5cf6, #c084fc); }

    .console-box { background-color: #020617; color: #38bdf8; font-family: 'Consolas', 'Courier New', monospace; font-size: 11px; padding: 12px; border-radius: 10px; height: 180px; overflow-y: auto; white-space: pre-wrap; border: 1px solid #1e293b; }
    
    .chart-box { background: rgba(2, 6, 23, 0.8); border-radius: 10px; padding: 10px; border: 1px solid #1e293b; }
    svg { width: 100%; height: 120px; }
    .line-soil { fill: none; stroke: #38bdf8; stroke-width: 3; stroke-linecap: round; stroke-linejoin: round; }
    .line-temp { fill: none; stroke: #f59e0b; stroke-width: 2; stroke-dasharray: 4; stroke-linecap: round; }

    @keyframes blinkFast { 0% { opacity: 1; } 50% { opacity: 0.3; } 100% { opacity: 1; } }
    @keyframes blinkSlow { 0% { opacity: 1; } 50% { opacity: 0.6; } 100% { opacity: 1; } }
    .error-blink { animation: blinkFast 0.5s linear infinite; background: linear-gradient(135deg, #dc2626, #7f1d1d); }
    .waiting-blink { animation: blinkSlow 1.5s linear infinite; background: linear-gradient(135deg, #d97706, #b45309); }
  </style>
</head>
<body>
  <h1>🌱 Smart Farm SCADA Enterprise Control Center</h1>
  
  <div class="card">
    <div class="section-title">🏆 Indeks Kesehatan Lahan & AI Diagnostic</div>
    <div style="display:flex; align-items:center; justify-content:space-between; margin-bottom: 5px;">
      <span style="font-size:14px; color:#cbd5e1;">Skor Kelayakan Lahan:</span>
      <span style="font-size:24px; font-weight:bold; color:#10b981;" id="health-score-val">100%</span>
    </div>
    <div class="progress-bg"><div class="progress-fill fill-green" id="health-bar" style="width: 100%;"></div></div>
    
    <p style="font-size:12px; color:#38bdf8; margin-top:10px; font-weight:bold;" id="ai-recom-val">💡 Rekomendasi AI: Kondisi Lahan Sangat Optimal untuk Pertumbuhan.</p>
    <p style="font-size:12px; color:#4ade80; margin-top:6px; font-weight:bold; background:rgba(16,185,129,0.1); padding:8px; border-radius:8px; border:1px solid rgba(16,185,129,0.2);" id="plant-summary-val">🤖 Kesimpulan AI Tanaman: Memuat analisis biologis tanaman...</p>
  </div>

  <div class="card">
    <div class="section-title">⚡ Status & Kontrol Utama</div>
    <div id="status-box" class="status aman">Loading...</div>
    <div class="stat-grid">
      <div class="stat-box">
        <div class="stat-label">Mode Operasional</div>
        <div class="stat-val" id="mode-text">Loading...</div>
        <span class="sub-note" style="color:#94a3b8;">Auto Threshold / Manual</span>
      </div>
      <div class="stat-box">
        <div class="stat-label">Status Relay Pompa</div>
        <div class="stat-val" id="relay-text">Loading...</div>
        <span class="sub-note" style="color:#94a3b8;">Proteksi Maks 20m Nonstop</span>
      </div>
    </div>
  </div>

  <div class="card">
    <div class="section-title">🎛️ Kendali Manual & Mode Override</div>
    <div class="flex-container">
      <button class="btn btn-auto" onclick="setMode('auto')">Pilih Mode AUTO</button>
      <button class="btn btn-manual" onclick="setMode('manual')">Pilih Mode MANUAL</button>
      <button class="btn btn-sync" onclick="copyTelemetryJSON()">📋 Copy JSON Telemetry (API)</button>
    </div>
    
    <div id="manual-controls" style="opacity: 1; pointer-events: auto; transition: 0.3s; margin-top: 10px;">
      <div class="flex-container">
        <button class="btn btn-on" onclick="toggleRelay('on')">⚡ NYALAKAN Pompa (Manual Instan)</button>
        <button class="btn btn-off" onclick="toggleRelay('off')">🛑 MATIKAN Pompa (Manual Instan)</button>
      </div>
    </div>
  </div>

  <div class="card">
    <div class="section-title">⚖️ Komparasi Sensor Kebun vs Satelit & Prediksi Hujan</div>
    <div class="stat-grid">
      <div class="stat-box">
        <div class="stat-label">🌡️ Suhu Sensor Kebun</div>
        <div class="stat-val" id="comp-dht-temp" style="color:#38bdf8;">- &deg;C</div>
        <span class="sub-note" style="color:#38bdf8;">📍 Sensor DHT11 Lokal</span>
      </div>
      <div class="stat-box">
        <div class="stat-label">🛰️ Suhu Satelit Global</div>
        <div class="stat-val" id="comp-sat-temp" style="color:#f59e0b;">- &deg;C</div>
        <span class="sub-note" style="color:#94a3b8;">🌐 Open-Meteo API</span>
      </div>
      <div class="stat-box">
        <div class="stat-label">📊 Selisih Varian (Δ)</div>
        <div class="stat-val" id="comp-diff-temp">- &deg;C</div>
        <span class="sub-note" id="diff-note" style="color:#94a3b8;">Analisis Suhu Lahan</span>
      </div>
      <div class="stat-box">
        <div class="stat-label">🌧️ Prakiraan Hujan Satelit</div>
        <div class="stat-val" id="comp-rain-pred" style="color:#a855f7;">Cerah / Tidak Hujan</div>
        <span class="sub-note" style="color:#a855f7;">🔮 Est. 1 - 3 Jam Ke Depan</span>
      </div>
    </div>
  </div>

  <div class="card">
    <div class="section-title">🌐 Sinkronisasi Cuaca Satelit</div>
    <div class="flex-container">
      <button class="btn btn-sat" onclick="fetchSatelliteData()">1. Tarik Data Satelit (Pakai 4G/Internet)</button>
      <button class="btn btn-sync" onclick="pushDataToESP()">2. Suntikkan ke ESP32 (Konek WiFi ESP32)</button>
    </div>
    <p style="font-size:12px; color:#94a3b8; margin-top:5px;">Satelit Terakhir: <b id="sat-info" style="color:#f8fafc;">Belum Disinkronkan</b></p>
  </div>

  <div class="card">
    <div class="section-title">📈 Grafik Dual Trend Real-Time (Biru: Tanah % | Kuning: Suhu °C)</div>
    <div class="chart-box">
      <svg viewBox="0 0 500 100" preserveAspectRatio="none">
        <polyline class="line-soil" id="soil-polyline" points="0,50 50,50 100,50 150,50 200,50 250,50 300,50 350,50 400,50 450,50 500,50" />
        <polyline class="line-temp" id="temp-polyline" points="0,70 50,70 100,70 150,70 200,70 250,70 300,70 350,70 400,70 450,70 500,70" />
      </svg>
    </div>
  </div>

  <div class="card">
    <div class="section-title">💸 Kalkulator Biaya Listrik PLN 450 VA (Pompa 125W)</div>
    <div class="stat-grid">
      <div class="stat-box">
        <div class="stat-label">💸 Est. Biaya Listrik (Subsidi)</div>
        <div class="stat-val" id="cost-val" style="color:#f59e0b;">Rp 0.0</div>
        <span class="sub-note" style="color:#10b981;">🟢 Tarif Rp 415 / kWh</span>
      </div>
      <div class="stat-box">
        <div class="stat-label">⚡ Energi Terpakai</div>
        <div class="stat-val" id="kwh-val">0.000 kWh</div>
        <span class="sub-note" style="color:#38bdf8;">Akumulasi Listrik PLN</span>
      </div>
      <div class="stat-box">
        <div class="stat-label">💦 Volume Air Terpakai</div>
        <div class="stat-val" id="water-val">0 Liter</div>
        <span class="sub-note" style="color:#38bdf8;">Debit Pompa 60 L/min</span>
      </div>
      <div class="stat-box">
        <div class="stat-label">🔥 Laju Penguapan Air</div>
        <div class="stat-val" id="evap-val" style="color:#f59e0b;">0.0 %/Jam</div>
        <span class="sub-note" style="color:#94a3b8;">Fisika Retensi Air Tanah</span>
      </div>
      <div class="stat-box">
        <div class="stat-label">🚰 Total Akumulasi Pompa</div>
        <div class="stat-val" id="pump-count-val">0 Kali</div>
        <span class="sub-note" style="color:#94a3b8;">Tersimpan Di LittleFS</span>
      </div>
      <div class="stat-box">
        <div class="stat-label">⏱️ Total Durasi Pompa</div>
        <div class="stat-val" id="pump-time-val">0 Menit</div>
        <span class="sub-note" style="color:#94a3b8;">Akumulasi Detik Nyala</span>
      </div>
    </div>
  </div>

  <div class="card">
    <div class="section-title">🏥 Diagnostik Kesehatan & Umur Hardware</div>
    <div class="stat-grid">
      <div class="stat-box">
        <div class="stat-label">🔌 Umur Saklar Relay</div>
        <div class="stat-val" id="relay-life-val">100%</div>
        <div class="progress-bg"><div class="progress-fill fill-purple" id="relay-bar" style="width: 100%;"></div></div>
        <span class="sub-note" style="color:#c084fc;">Kondisi Mekanis Baik</span>
      </div>
      <div class="stat-box">
        <div class="stat-label">🌡️ Kesehatan DHT11</div>
        <div class="stat-val" id="dht-health-val">100%</div>
        <span class="sub-note" id="dht-sub-note" style="color:#10b981;">🟢 Sensor Suhu Normal</span>
      </div>
      <div class="stat-box">
        <div class="stat-label">🌱 Sensor Tanah Kebun</div>
        <div class="stat-val" id="soil-health-val">100%</div>
        <span class="sub-note" id="soil-sub-note" style="color:#10b981;">🟢 Radio ESP-NOW Aktif</span>
      </div>
      <div class="stat-box">
        <div class="stat-label">📡 Radio Kebun (ESP8266)</div>
        <div class="stat-val" id="rssi-val">- dBm</div>
        <div class="progress-bg"><div class="progress-fill fill-blue" id="rssi-bar" style="width: 0%;"></div></div>
        <span class="sub-note" id="rssi-sub-note" style="color:#38bdf8;">Kualitas Sinyal Radio</span>
      </div>
      <div class="stat-box">
        <div class="stat-label">🧠 Sisa RAM ESP32</div>
        <div class="stat-val" id="ram-val">- KB</div>
        <span class="sub-note" id="ram-sub-note" style="color:#10b981;">🟢 Free Heap Memory</span>
      </div>
    </div>
  </div>

  <div class="card">
    <div class="section-title">📡 Detail Telemetri Sensor</div>
    <div class="stat-grid">
      <div class="stat-box">
        <div class="stat-label">🌱 Kelembapan Tanah</div>
        <div class="stat-val value" id="soil-val">- %</div>
        <div class="progress-bg"><div class="progress-fill fill-green" id="soil-bar" style="width: 0%;"></div></div>
        <span class="sub-note" id="soil-cat-val" style="color:#10b981; font-weight:bold;">Loading...</span>
      </div>
      <div class="stat-box">
        <div class="stat-label">⏳ Sisa Retensi Air Tanah</div>
        <div class="stat-val" id="soil-dep-val" style="color:#38bdf8;">- Hari</div>
        <span class="sub-note" style="color:#94a3b8;">Est. Ketahanan Air</span>
      </div>
      <div class="stat-box">
        <div class="stat-label">📊 Raw ADC Pin A0</div>
        <div class="stat-val" id="raw-adc-val" style="color:#38bdf8;">- ADC</div>
        <span class="sub-note" style="color:#94a3b8;">Nilai Mentah Sensor</span>
      </div>
      <div class="stat-box">
        <div class="stat-label">🔋 Baterai ESP8266</div>
        <div class="stat-val" id="node-bat-val" style="color:#10b981;">100%</div>
        <span class="sub-note" style="color:#10b981;">3.3V Regulated</span>
      </div>
      <div class="stat-box">
        <div class="stat-label">🌡️ Suhu Udara DHT11</div>
        <div class="stat-val value" id="suhu-val">- &deg;C</div>
        <span class="sub-note" id="suhu-f-val">- &deg;F</span>
      </div>
      <div class="stat-box">
        <div class="stat-label">💧 Kelembapan Udara</div>
        <div class="stat-val value" id="hum-val">- %</div>
        <span class="sub-note">Hygrometer DHT11</span>
      </div>
      <div class="stat-box">
        <div class="stat-label">🔥 Heat Index (Terasa)</div>
        <div class="stat-val" id="heat-val" style="color:#f59e0b;">- &deg;C</div>
        <span class="sub-note" id="heat-f-val">- &deg;F</span>
      </div>
      <div class="stat-box">
        <div class="stat-label">❄️ Dew Point (Titik Embun)</div>
        <div class="stat-val" id="dew-val" style="color:#38bdf8;">- &deg;C</div>
        <span class="sub-note">Indikator Embun</span>
      </div>
    </div>
  </div>

  <div class="card">
    <div class="section-title">💾 Log Storage & Admin Remote</div>
    <div class="flex-container">
      <a class="btn btn-sync" href="/downloadLog" target="_blank">📥 Download Log CSV</a>
      <button class="btn btn-danger" onclick="clearESPLog()">🧹 Hapus Log & Reset Counter</button>
      <button class="btn btn-reboot" onclick="rebootESP32()">🔄 Soft Reboot ESP32</button>
    </div>
  </div>

  <div class="card">
    <div class="section-title">💻 Web Serial Debug Console Live Stream</div>
    <div class="console-box" id="console-log">=========================================\n[SYSTEM] SCADA ENTERPRISE ONLINE\n=========================================\n</div>
  </div>

<script>
var lastState = { relay: null, waiting: null, error: null, disconnected: false };
var soilHistory = [50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50];
var tempHistory = [30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30];
var latestJSONRaw = "";

function logToConsole(msg) {
  var c = document.getElementById('console-log');
  var now = new Date();
  var timeStr = now.toTimeString().split(' ')[0] + "." + String(now.getMilliseconds()).padStart(3, '0');
  c.innerText += "[" + timeStr + "] " + msg + "\n";
  c.scrollTop = c.scrollHeight;
}

function updateSVGChart(newSoil, newTemp) {
  soilHistory.shift(); soilHistory.push(newSoil);
  tempHistory.shift(); tempHistory.push(newTemp);
  
  var ptsSoil = "", ptsTemp = "";
  for(var i=0; i<soilHistory.length; i++) {
    var x = i * 50;
    var ySoil = 100 - soilHistory[i]; 
    var yTemp = 100 - (tempHistory[i] * 2); 
    ptsSoil += x + "," + ySoil + " ";
    ptsTemp += x + "," + yTemp + " ";
  }
  document.getElementById('soil-polyline').setAttribute('points', ptsSoil.trim());
  document.getElementById('temp-polyline').setAttribute('points', ptsTemp.trim());
}

function copyTelemetryJSON() {
  if(latestJSONRaw !== "") {
    navigator.clipboard.writeText(latestJSONRaw).then(() => {
      alert("📋 JSON Telemetry disalin ke Clipboard!");
      logToConsole("📋 SYSTEM: JSON Telemetry Copied to Clipboard.");
    });
  }
}

function fetchSatelliteData() {
  logToConsole("🌐 Mengambil data cuaca satelit dari Open-Meteo API...");
  fetch('https://api.open-meteo.com/v1/forecast?latitude=-7.25&longitude=112.75&current_weather=true')
  .then(res => res.json())
  .then(data => {
      var temp = data.current_weather.temperature;
      var code = data.current_weather.weathercode;
      var desc = "Cerah";
      var rainPred = "Cerah / Tidak Ada Hujan";
      if(code >= 50) { desc = "Hujan Ringan/Lebat"; rainPred = "🌧️ PREDIKSI HUJAN TURUN!"; }
      else if(code >= 1) { desc = "Berawan"; rainPred = "☁️ Berawan / Potensi Mendung"; }

      var now = new Date();
      var timeStr = now.getHours() + ":" + String(now.getMinutes()).padStart(2, '0');

      localStorage.setItem('sat_temp', temp);
      localStorage.setItem('sat_desc', desc);
      localStorage.setItem('sat_rain', rainPred);
      localStorage.setItem('sat_time', timeStr);

      updateSatUI();
      logToConsole("✅ SUKSES Tarik Data Satelit: " + temp + "°C (" + desc + "). Silakan hubungkan WiFi ke ESP32 lalu klik 'Suntikkan ke ESP32'.");
      alert("✅ Data Satelit Berhasil Ditarik: " + temp + "°C (" + desc + ")\n\nLangkah selanjutnya:\n1. Hubungkan WiFi HP ke ESP32 (SmartFarm-ESP32)\n2. Klik tombol '2. Suntikkan ke ESP32'");
  })
  .catch(err => {
      logToConsole("❌ GAGAL tarik data satelit! Pastikan koneksi internet 4G HP aktif.");
      alert("❌ Gagal mengambil data satelit. Pastikan internet 4G HP Anda aktif.");
  });
}

function updateSatUI() {
  var temp = localStorage.getItem('sat_temp');
  var desc = localStorage.getItem('sat_desc');
  var time = localStorage.getItem('sat_time');
  if(temp && desc && time) {
      document.getElementById('sat-info').innerHTML = temp + "&deg;C (" + desc + ") - " + time;
  }
}

function pushDataToESP() {
  var temp = localStorage.getItem('sat_temp');
  var desc = localStorage.getItem('sat_desc');
  var rain = localStorage.getItem('sat_rain');
  var time = localStorage.getItem('sat_time');

  if(!temp || !desc || !time) {
      alert("❌ Data satelit belum ditarik! Klik '1. Tarik Data Satelit' terlebih dahulu.");
      return;
  }

  logToConsole("📤 Menyuntikkan data satelit ke ESP32...");
  var body = 'temp=' + encodeURIComponent(temp) + '&desc=' + encodeURIComponent(desc) + '&rain=' + encodeURIComponent(rain) + '&time=' + encodeURIComponent(time);
  
  fetch('/pushWeather', {
      method: 'POST',
      headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
      body: body
  })
  .then(res => res.text())
  .then(txt => {
      logToConsole("✅ SUKSES: " + txt);
      alert("✅ " + txt);
      fetchData();
  })
  .catch(err => {
      logToConsole("❌ GAGAL menyuntikkan data ke ESP32! Pastikan WiFi HP terhubung ke 'SmartFarm-ESP32'.");
      alert("❌ Gagal terhubung ke ESP32! Pastikan WiFi HP terhubung ke 'SmartFarm-ESP32'.");
  });
}

function clearESPLog() {
  if(confirm("Apakah Anda yakin ingin menghapus seluruh log penyimpanan CSV di LittleFS ESP32 dan mereset counter pompa?")) {
     logToConsole("🧹 Mengirim perintah reset log LittleFS & counter pompa ke ESP32...");
     fetch('/clearLogs', { method: 'POST' })
     .then(res => res.text())
     .then(txt => {
        logToConsole("✅ " + txt);
        alert("✅ " + txt);
        fetchData();
     });
  }
}

function rebootESP32() {
  if(confirm("Apakah Anda yakin ingin me-reboot ESP32 System secara remote?")) {
     logToConsole("🔄 Mengirim perintah Soft Reboot ke ESP32...");
     fetch('/reboot', { method: 'POST' })
     .then(res => res.text())
     .then(txt => {
        logToConsole("🔄 ESP32 sedang melakukan reboot...");
        alert("🔄 ESP32 sedang melakukan Soft Reboot. Tunggu 5 detik lalu muat ulang halaman.");
     });
  }
}

function fetchData() {
  fetch('/data')
    .then(res => res.json())
    .then(data => {
      latestJSONRaw = JSON.stringify(data, null, 2);
      if(lastState.disconnected) {
         logToConsole("✅ KONEKSI PULIH: Berhasil terhubung kembali dengan ESP32!");
         lastState.disconnected = false;
      }

      document.getElementById('suhu-val').innerHTML = data.suhuC + "&deg;C";
      document.getElementById('suhu-f-val').innerHTML = data.suhuF + "&deg;F";
      document.getElementById('hum-val').innerText = data.hum + "%";
      document.getElementById('heat-val').innerHTML = data.heatC + "&deg;C";
      document.getElementById('heat-f-val').innerHTML = data.heatF + "&deg;F";
      document.getElementById('dew-val').innerHTML = data.dew + "&deg;C";
      
      var soilVal = parseInt(data.soil);
      document.getElementById('soil-val').innerText = soilVal + "%";
      document.getElementById('soil-bar').style.width = soilVal + "%";
      
      if(data.soilCategory) document.getElementById('soil-cat-val').innerText = data.soilCategory;
      if(data.soilDepletion) document.getElementById('soil-dep-val').innerText = data.soilDepletion;
      if(data.plantSummary) document.getElementById('plant-summary-val').innerText = data.plantSummary;
      if(data.rawAdc) document.getElementById('raw-adc-val').innerText = data.rawAdc + " ADC";
      if(data.nodeBat) document.getElementById('node-bat-val').innerText = data.nodeBat + "%";

      document.getElementById('evap-val').innerText = data.evaporation + " %/Jam";
      document.getElementById('comp-dht-temp').innerHTML = data.suhuC + " &deg;C";
      document.getElementById('comp-sat-temp').innerHTML = (data.satTemp !== "-") ? data.satTemp + " &deg;C" : "-";
      
      if(data.satTemp !== "-") {
         var diff = parseFloat(data.tempDiff);
         var diffStr = (diff >= 0 ? "+" : "") + diff + " &deg;C";
         document.getElementById('comp-diff-temp').innerHTML = diffStr;
         document.getElementById('diff-note').innerText = (diff >= 0 ? "Kebun Lebih Hangat" : "Kebun Lebih Sejuk");
      } else {
         document.getElementById('comp-diff-temp').innerText = "-";
      }
      
      document.getElementById('comp-rain-pred').innerText = data.satRainPred ? data.satRainPred : "Cerah / Tidak Hujan";
      
      updateSVGChart(soilVal, parseFloat(data.suhuC));

      document.getElementById('cost-val').innerText = "Rp " + data.costIdr;
      document.getElementById('kwh-val').innerText = data.kWhUsed + " kWh";
      document.getElementById('water-val').innerText = data.waterLiters + " Liter";
      document.getElementById('pump-count-val').innerText = data.pumpCount + " Kali";
      document.getElementById('pump-time-val').innerText = Math.floor(parseInt(data.totalPumpSecs) / 60) + " Menit";

      document.getElementById('health-score-val').innerText = data.farmHealth + "%";
      document.getElementById('health-bar').style.width = data.farmHealth + "%";

      var recomEl = document.getElementById('ai-recom-val');
      if(data.satRainPred && (data.satRainPred.includes("Hujan") || data.satRainPred.includes("Mendung"))) {
        recomEl.innerHTML = "💡 Rekomendasi AI: 🌧️ SATELIT PREDIKSI AKAN HUJAN! Ditunda penyiraman otomatis untuk menghemat listrik PLN & air.";
        recomEl.style.color = "#a855f7";
      } else if(data.farmHealth >= 80) {
        recomEl.innerHTML = "💡 Rekomendasi AI: Kondisi Lahan Optimal! Waktu Terbaik Menyiram: 06:00 - 08:00 WIB (Penguapan Rendah).";
        recomEl.style.color = "#38bdf8";
      } else if(data.farmHealth >= 50) {
        recomEl.innerHTML = "💡 Rekomendasi AI: Lahan Mulai Kering/Hangat. Disarankan Penyiraman Ringan.";
        recomEl.style.color = "#f59e0b";
      } else {
        recomEl.innerHTML = "⚠️ Rekomendasi AI: PERINGATAN! Lahan Sangat Kering/Panas. Lakukan Penyiraman Segera!";
        recomEl.style.color = "#ef4444";
      }

      document.getElementById('relay-life-val').innerText = data.relayLife + "%";
      document.getElementById('relay-bar').style.width = data.relayLife + "%";
      
      var dhtSub = document.getElementById('dht-sub-note');
      if(data.dhtHealth == 100) {
        document.getElementById('dht-health-val').innerText = "100% (Sehat)";
        dhtSub.innerText = "🟢 Sensor Suhu Aktif & Normal"; dhtSub.style.color = "#10b981";
      } else {
        document.getElementById('dht-health-val').innerText = "0% (Rusak/Terlepas)";
        dhtSub.innerText = "🔴 Kabel DHT11 Terputus/Terlepas!"; dhtSub.style.color = "#ef4444";
      }

      var soilSubNote = document.getElementById('soil-sub-note');
      if(data.soilHealth == 100) {
        document.getElementById('soil-health-val').innerText = "100% (Presisi)";
        soilSubNote.innerText = "🟢 Terhubung Radio ESP-NOW"; soilSubNote.style.color = "#10b981";
      } else {
        document.getElementById('soil-health-val').innerText = "0% (Terputus)";
        soilSubNote.innerText = "🔴 Sinyal ESP8266 Terputus / Baterai Habis!"; soilSubNote.style.color = "#ef4444";
      }

      var rssiInt = parseInt(data.rssi);
      var rssiSub = document.getElementById('rssi-sub-note');
      document.getElementById('rssi-val').innerText = data.rssi + " dBm (" + data.signalQuality + "%)";
      document.getElementById('rssi-bar').style.width = data.signalQuality + "%";
      if(rssiInt >= -65) {
        rssiSub.innerText = "🟢 Sinyal Sangat Kuat & Dekat (<30m)"; rssiSub.style.color = "#10b981";
      } else if(rssiInt >= -80) {
        rssiSub.innerText = "🟡 Sinyal Cukup Baik (Jarak 30-70m)"; rssiSub.style.color = "#f59e0b";
      } else {
        rssiSub.innerText = "🔴 Sinyal Lemah! Terlalu Jauh (>80m)"; rssiSub.style.color = "#ef4444";
      }

      document.getElementById('ram-val').innerText = data.freeHeap + " KB";

      logToConsole("🌱 [TELEMETRY] Tanah: " + data.soil + "% (" + data.soilCategory + ") | Suhu: " + data.suhuC + "°C | Hum: " + data.hum + "% | Raw: " + data.rawAdc + " ADC | Sinyal: " + data.rssi + " dBm");

      var sb = document.getElementById('status-box');
      if (data.isWaiting == 1) {
          sb.className = "status waiting-blink";
          sb.innerHTML = "⏳ MENUNGGU KONEKSI ESP8266 ⏳<br><span style='font-size:13px;'>" + data.errorMsg + "</span>";
      } else if (data.errorMsg && data.errorMsg !== "") {
          sb.className = "status error-blink";
          sb.innerHTML = "⚠️ SYSTEM ERROR DARURAT ⚠️<br><span style='font-size:13px;'>" + data.errorMsg + "</span>";
      } else {
          sb.className = "status " + data.statusColor;
          sb.innerText = data.statusText;
      }
      
      if(data.isManual == 1) {
          document.getElementById('mode-text').innerHTML = "<span style='color:#f97316'>MANUAL</span>";
      } else {
          document.getElementById('mode-text').innerHTML = "<span style='color:#38bdf8'>AUTO</span>";
      }

      if(data.relayOn == 1) {
          document.getElementById('relay-text').innerHTML = "<span style='color:#ef4444;'>ON (Menyala)</span>";
      } else if(data.cooldown == 1 && data.isManual == 0) {
          document.getElementById('relay-text').innerHTML = "<span style='color:#94a3b8;'>OFF (Pendinginan Maks 20m)</span>";
      } else {
          document.getElementById('relay-text').innerHTML = "<span style='color:#94a3b8;'>OFF (Mati)</span>";
      }
      
      if(data.satTemp && data.satTemp !== "-") {
          document.getElementById('sat-info').innerHTML = data.satTemp + "&deg;C (" + data.satDesc + ") - " + data.satTime;
      }
    })
    .catch(e => {
       if(!lastState.disconnected) {
          logToConsole("⚠️ KONEKSI TERPUTUS: Tidak dapat menjangkau ESP32...");
          lastState.disconnected = true;
       }
    });
}

function setMode(mode) { fetch('/setMode?m=' + mode).then(() => fetchData()); }
function toggleRelay(state) { fetch('/toggleRelay?s=' + state).then(() => fetchData()); }

setInterval(fetchData, 1000);
window.onload = function() { updateSatUI(); fetchData(); };
</script>
</body>
</html>
)rawliteral";

#endif
