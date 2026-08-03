const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Smart Farm Dashboard</title>
  <style>
    body { font-family: 'Segoe UI', Tahoma, sans-serif; background-color: #f4f7f6; color: #333; margin: 0; padding: 15px; }
    .card { background: white; padding: 20px; border-radius: 12px; box-shadow: 0 4px 10px rgba(0,0,0,0.05); margin-bottom: 20px; }
    h1 { color: #2c3e50; text-align: center; font-size: 24px;}
    .value { font-size: 24px; font-weight: bold; color: #2980b9; }
    .status { font-size: 18px; font-weight: bold; padding: 12px; border-radius: 8px; text-align: center; color: white; margin-bottom:15px; }
    .aman { background-color: #27ae60; }
    .peringatan { background-color: #f39c12; }
    .bahaya { background-color: #c0392b; }
    .btn { padding: 12px 20px; border: none; border-radius: 8px; color: white; font-size: 16px; font-weight: bold; cursor: pointer; margin-right: 10px; margin-bottom: 10px; flex: 1 1 calc(50% - 10px); min-width: 120px; text-align: center;}
    .btn-auto { background-color: #3498db; }
    .btn-manual { background-color: #e67e22; }
    .btn-on { background-color: #c0392b; }
    .btn-off { background-color: #7f8c8d; }
    .btn-dummy { background-color: #8e44ad; }
    .flex-container { display: flex; flex-wrap: wrap; gap: 10px; margin-bottom: 15px;}
    .section-title { font-size: 14px; color: #7f8c8d; text-transform: uppercase; margin-bottom: 10px; border-bottom: 1px solid #ddd; padding-bottom: 5px;}
    
    /* Animasi Kelap-Kelip untuk Error */
    @keyframes blink {
      0% { opacity: 1; }
      50% { opacity: 0.5; }
      100% { opacity: 1; }
    }
    .error-blink { animation: blink 1s linear infinite; background-color: #c0392b; }
  </style>
</head>
<body>
  <h1>🌱 Smart Farm Dashboard</h1>
  
  <div class="card">
    <div class="section-title">Status Sistem</div>
    <div id="status-box" class="status aman">Loading...</div>
    <p>Status Mode: <span id="mode-text" style="font-weight:bold;">Loading...</span></p>
    <p>Relay Utama (Pompa): <span id="relay-text" style="font-weight:bold;">Loading...</span></p>
    <p id="esp-warning" style="color:red; font-size:12px; display:none;">⚠️ Peringatan: Data dari ESP8266 terputus/Timeout!</p>
  </div>

  <div class="card">
    <div class="section-title">🎛️ Kendali Web Jarak Jauh</div>
    <div class="flex-container">
      <button class="btn btn-auto" onclick="setMode('auto')">Pilih Mode AUTO</button>
      <button class="btn btn-manual" onclick="setMode('manual')">Pilih Mode MANUAL</button>
    </div>
    
    <div id="manual-controls" style="opacity: 0.5; pointer-events: none; transition: 0.3s;">
      <p style="font-size:12px; margin-bottom: 5px;">(Fitur di bawah hanya aktif jika Anda memilih Mode Manual)</p>
      <div class="flex-container">
        <button class="btn btn-on" onclick="toggleRelay('on')">NYALAKAN Pompa</button>
        <button class="btn btn-off" onclick="toggleRelay('off')">MATIKAN Pompa</button>
      </div>
    </div>
    
    <div class="section-title" style="margin-top:20px;">🕹️ Tombol Cadangan (Fitur Mendatang)</div>
    <div class="flex-container">
      <button class="btn btn-dummy" onclick="alert('Tombol Kipas belum diprogram di ESP32')">Kipas Tambahan</button>
      <button class="btn btn-dummy" onclick="alert('Tombol Lampu belum diprogram di ESP32')">Lampu Tumbuh</button>
    </div>
  </div>

  <div class="card">
    <div class="section-title">📡 Sensor Tanah (Dari ESP8266)</div>
    <p>Kelembapan Tanah: <span class="value" id="soil-val">- %</span></p>
  </div>

  <div class="card">
    <div class="section-title">☁️ Cuaca Lokal (Dari DHT11)</div>
    <p>Suhu Udara: <br><span class="value"><span id="temp-c">-</span> &deg;C | <span id="temp-f">-</span> &deg;F</span></p>
    <p>Kelembapan Udara: <br><span class="value"><span id="hum-val">-</span> %</span></p>
    <p>Terasa Seperti: <br><span class="value"><span id="heat-c">-</span> &deg;C | <span id="heat-f">-</span> &deg;F</span></p>
    <p>Titik Embun (Dew): <br><span class="value"><span id="dew-val">-</span> &deg;C</span></p>
  </div>

<script>
// Fungsi mengambil data JSON dari ESP32 dengan AJAX
function fetchData() {
  fetch('/data')
    .then(response => response.json())
    .then(data => {
      // Perbarui Data Cuaca
      document.getElementById('temp-c').innerText = data.suhuC;
      document.getElementById('temp-f').innerText = data.suhuF;
      document.getElementById('hum-val').innerText = data.hum;
      document.getElementById('heat-c').innerText = data.heatC;
      document.getElementById('heat-f').innerText = data.heatF;
      document.getElementById('dew-val').innerText = data.dew;
      
      // Perbarui Data Tanah
      document.getElementById('soil-val').innerText = data.soil;
      
      // Perbarui Kotak Status (Dengan Penanganan Error)
      var sb = document.getElementById('status-box');
      if (data.errorMsg && data.errorMsg !== "") {
          // JIKA ADA ERROR
          sb.className = "status error-blink";
          sb.innerHTML = "⚠️ SYSTEM ERROR ⚠️<br><span style='font-size:14px;'>" + data.errorMsg + "</span>";
          document.getElementById('esp-warning').style.display = 'none'; // Sembunyikan peringatan kecil
      } else {
          // JIKA NORMAL
          sb.className = "status " + data.statusColor;
          sb.innerText = data.statusText;
          if(data.timeout == 1) {
              document.getElementById('esp-warning').style.display = 'block';
          } else {
              document.getElementById('esp-warning').style.display = 'none';
          }
      }
      
      // Kunci/Buka Tombol Manual
      var manualControls = document.getElementById('manual-controls');
      if(data.isManual == 1) {
          document.getElementById('mode-text').innerHTML = "<span style='color:#e67e22'>MANUAL (Kendali Web)</span>";
          manualControls.style.opacity = "1";
          manualControls.style.pointerEvents = "auto";
      } else {
          document.getElementById('mode-text').innerHTML = "<span style='color:#3498db'>AUTO (Ikut Sensor)</span>";
          manualControls.style.opacity = "0.5";
          manualControls.style.pointerEvents = "none";
      }

      // Perbarui Teks Relay
      if(data.relayOn == 1) {
          document.getElementById('relay-text').innerHTML = "<span style='color:#c0392b;'>ON (Menyala)</span>";
      } else if(data.cooldown == 1 && data.isManual == 0) {
          document.getElementById('relay-text').innerHTML = "<span style='color:#7f8c8d;'>OFF (Sistem Terkunci - Pompa Kering!)</span>";
      } else {
          document.getElementById('relay-text').innerHTML = "<span style='color:#7f8c8d;'>OFF (Mati)</span>";
      }
    })
    .catch(error => console.log("Gagal mengambil data", error));
}

function setMode(mode) {
  fetch('/setMode?m=' + mode).then(() => fetchData()); 
}

function toggleRelay(state) {
  fetch('/toggleRelay?s=' + state).then(() => fetchData());
}

setInterval(fetchData, 2000);
window.onload = fetchData;
</script>
</body>
</html>
)rawliteral";
