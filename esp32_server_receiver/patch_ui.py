import re

with open('index.h', 'r') as f:
    html = f.read()

# 1. Add CSS
css_add = """
    @keyframes spin { 100% { transform: rotate(360deg); } }
    @keyframes rain { 0% { transform: translateY(-5px); opacity: 0; } 50% { opacity: 1; } 100% { transform: translateY(15px); opacity: 0; } }
    .sun-icon { width: 50px; height: 50px; fill: #fbbf24; animation: spin 10s linear infinite; }
    .cloud-icon { width: 50px; height: 50px; fill: #94a3b8; }
    .rain-drop { fill: #38bdf8; animation: rain 1.5s infinite; }
    .weather-widget { display: flex; align-items: center; gap: 15px; background: rgba(0,0,0,0.3); padding: 10px; border-radius: 10px; margin-top: 10px; }
  </style>"""
html = html.replace("  </style>", css_add)

# 2. Add New Cards after "Kendali Manual & Mode Override" Card
new_cards = """
  <div class="card">
    <div class="section-title">🎚️ Pengaturan Batas Otomatis (Threshold)</div>
    <div style="margin-bottom:15px;">
      <div class="stat-label" style="display:flex; justify-content:space-between;">
        <span>Batas Tanah Minimal (Siram jika di bawah ini):</span>
        <b id="soil-slider-val" style="color:#10b981;">45%</b>
      </div>
      <input type="range" id="soil-slider" min="10" max="90" value="45" style="width:100%; cursor:pointer;" oninput="document.getElementById('soil-slider-val').innerText = this.value + '%'">
    </div>
    <div style="margin-bottom:15px;">
      <div class="stat-label" style="display:flex; justify-content:space-between;">
        <span>Batas Suhu Maksimal (Siram jika di atas ini):</span>
        <b id="temp-slider-val" style="color:#ef4444;">30&deg;C</b>
      </div>
      <input type="range" id="temp-slider" min="20" max="45" value="30" style="width:100%; cursor:pointer;" oninput="document.getElementById('temp-slider-val').innerText = this.value + '&deg;C'">
    </div>
    <button class="btn btn-sync" onclick="saveThreshold()" style="width:100%;">⚙️ Simpan Pengaturan ke ESP32</button>
  </div>

  <div class="card">
    <div class="section-title">🧠 AI Expert System & Form Tanaman</div>
    <div style="display:flex; gap:10px; margin-bottom:10px;">
      <input type="text" id="plant-name" placeholder="Nama Tanaman (misal: Cabai)" style="flex:1; padding:10px; border-radius:6px; border:1px solid #1e293b; background:rgba(255,255,255,0.05); color:white; outline:none;">
      <input type="date" id="plant-date" style="flex:1; padding:10px; border-radius:6px; border:1px solid #1e293b; background:rgba(255,255,255,0.05); color:white; outline:none; color-scheme:dark;">
    </div>
    <button class="btn btn-sat" onclick="savePlantData()" style="width:100%;">💾 Simpan & Analisis Tanaman</button>
    <div class="stat-box" style="margin-top:15px; border-left: 4px solid #8b5cf6;">
      <div class="stat-label" style="color:#c084fc; font-weight:bold; margin-bottom:5px;">📊 Hasil Analisis Pakar AI</div>
      <div class="stat-val" id="ai-expert-result" style="font-size:13px; color:#f8fafc; font-weight:normal; line-height:1.6;">Belum ada data tanaman. Silakan isi form di atas.</div>
    </div>
  </div>
"""
# Insert after the end of manual card
insert_idx = html.find('</div>', html.find('<div id="manual-controls"')) + 14
html = html[:insert_idx] + new_cards + html[insert_idx:]

# 3. Update Weather Widget
old_sat_info = '<p style="font-size:12px; color:#94a3b8; margin-top:5px;">Satelit Terakhir: <b id="sat-info" style="color:#f8fafc;">Belum Disinkronkan</b></p>'
new_sat_info = """
    <div class="weather-widget">
      <div id="weather-icon">
        <svg class="cloud-icon" viewBox="0 0 24 24"><path d="M19.35 10.04C18.67 6.59 15.64 4 12 4 9.11 4 6.6 5.64 5.35 8.04 2.34 8.36 0 10.91 0 14c0 3.31 2.69 6 6 6h13c2.76 0 5-2.24 5-5 0-2.64-2.05-4.78-4.65-4.96z"/></svg>
      </div>
      <div>
        <div style="font-size:11px; color:#94a3b8;">Satelit Terakhir Tersinkron:</div>
        <b id="sat-info" style="font-size:15px; color:#f8fafc;">Belum Disinkronkan</b>
      </div>
    </div>
"""
html = html.replace(old_sat_info, new_sat_info)

# 4. Add JavaScript Functions
js_add = """
var audioCtx = null;
var lastAlarmTime = 0;
function playAlarm() {
  var now = Date.now();
  if(now - lastAlarmTime < 2000) return; // play every 2 seconds
  lastAlarmTime = now;
  try {
    if(!audioCtx) audioCtx = new (window.AudioContext || window.webkitAudioContext)();
    var osc = audioCtx.createOscillator();
    var gain = audioCtx.createGain();
    osc.connect(gain);
    gain.connect(audioCtx.destination);
    osc.type = "square";
    osc.frequency.setValueAtTime(600, audioCtx.currentTime);
    osc.frequency.setValueAtTime(1000, audioCtx.currentTime + 0.1);
    gain.gain.setValueAtTime(0.1, audioCtx.currentTime);
    gain.gain.exponentialRampToValueAtTime(0.001, audioCtx.currentTime + 0.5);
    osc.start(audioCtx.currentTime);
    osc.stop(audioCtx.currentTime + 0.5);
  } catch(e){}
}

function saveThreshold() {
  var s = document.getElementById('soil-slider').value;
  var t = document.getElementById('temp-slider').value;
  logToConsole("⚙️ Menyimpan pengaturan ke ESP32... (Tanah: " + s + "%, Suhu: " + t + "C)");
  fetch('/setThreshold?soil=' + s + '&temp=' + t)
  .then(res => res.text())
  .then(txt => {
      logToConsole("✅ " + txt);
      alert("✅ Pengaturan Berhasil Disimpan di ESP32!");
  });
}

function savePlantData() {
  var pName = document.getElementById('plant-name').value;
  var pDate = document.getElementById('plant-date').value;
  if(pName && pDate) {
    localStorage.setItem('plant_name', pName);
    localStorage.setItem('plant_date', pDate);
    alert('✅ Data tanaman berhasil disimpan di Browser!');
    runExpertAI();
  } else {
    alert('Harap isi Nama Tanaman dan Tanggal Tanam!');
  }
}

function runExpertAI() {
  var pName = localStorage.getItem('plant_name');
  var pDate = localStorage.getItem('plant_date');
  if(pName && pDate) {
    document.getElementById('plant-name').value = pName;
    document.getElementById('plant-date').value = pDate;
    
    var t1 = new Date(pDate);
    var t2 = new Date();
    var diffTime = t2 - t1;
    var diffDays = Math.ceil(diffTime / (1000 * 60 * 60 * 24)) - 1;
    if(diffDays < 0) diffDays = 0;
    
    var soilV = parseFloat(document.getElementById('soil-val').innerText) || 50;
    var tempV = parseFloat(document.getElementById('suhu-val').innerText) || 30;
    var rainV = localStorage.getItem('sat_rain') || "";
    
    var aiText = "🌱 <b>Tanaman " + pName + "</b> Anda saat ini berumur <b>" + diffDays + " Hari</b>.<br><br>";
    if(diffDays < 14) aiText += "🌿 <b>Fase Vegetatif Awal:</b> Butuh banyak air dan Nitrogen. Pemupukan Urea/NPK disarankan dosis ringan.<br>";
    else if(diffDays < 45) aiText += "🌸 <b>Fase Generatif:</b> Tanaman mulai berbunga/berbuah. Tingkatkan asupan Kalium (Pupuk KCL/Phonska).<br>";
    else aiText += "🌾 <b>Fase Panen:</b> Mendekati masa panen. Kurangi penyiraman agar buah tidak busuk atau pecah.<br>";
    
    if(soilV < 40) aiText += "⚠️ <span style='color:#ef4444;'>Peringatan:</span> Tanah terdeteksi kering, segera hidupkan pompa.<br>";
    if(tempV > 34) aiText += "🔥 <span style='color:#ef4444;'>Peringatan:</span> Suhu >34C dapat memicu stres panas dan bunga rontok.<br>";
    
    if(rainV.includes("Hujan")) aiText += "<br>🌧️ <b>Saran Cuaca:</b> Tunda pemupukan! Satelit memprediksi hujan akan turun, pupuk padat bisa hanyut tercuci air.";
    
    document.getElementById('ai-expert-result').innerHTML = aiText;
  }
}
"""
html = html.replace("var latestJSONRaw = \"\";", "var latestJSONRaw = \"\";\n" + js_add)

# 5. Modify updateSatUI for SVG logic
old_update_sat = """function updateSatUI() {
  var temp = localStorage.getItem('sat_temp');
  var desc = localStorage.getItem('sat_desc');
  var time = localStorage.getItem('sat_time');
  if(temp && desc && time) {
      document.getElementById('sat-info').innerHTML = temp + "&deg;C (" + desc + ") - " + time;
  }
}"""
new_update_sat = """function updateSatUI() {
  var temp = localStorage.getItem('sat_temp');
  var desc = localStorage.getItem('sat_desc');
  var time = localStorage.getItem('sat_time');
  if(temp && desc && time) {
      document.getElementById('sat-info').innerHTML = temp + "&deg;C (" + desc + ") - " + time;
      var svgIcon = "";
      if(desc.toLowerCase().includes("hujan")) {
          svgIcon = `<svg class="cloud-icon" viewBox="0 0 24 24">
            <path d="M18.9 9.32c-.08-.42-.2-.84-.37-1.25-.49-1.22-1.35-2.22-2.45-2.85C14.98 4.6 13.52 4.25 12 4.25c-2.4 0-4.63 1.25-5.88 3.32C3.86 8 2 10.05 2 12.5c0 2.48 2.02 4.5 4.5 4.5h2v-2H6.5c-1.38 0-2.5-1.12-2.5-2.5 0-1.26.93-2.33 2.17-2.48l1.04-.13.3-.98c.6-1.95 2.37-3.31 4.49-3.31 1.48 0 2.84.75 3.65 1.96l.57.85.99-.21c.88-.19 1.79.13 2.33.82.54.7.67 1.63.34 2.44L19 15h1.5C21.88 15 23 13.88 23 12.5c0-1.19-.84-2.21-1.99-2.44l-2.11-.74z"/>
            <path class="rain-drop" d="M12 18.29l1.41-1.41c.4-.4.4-1.04 0-1.41l-1.41-1.41-1.41 1.41c-.4.4-.4 1.04 0 1.41L12 18.29z"/>
            <path class="rain-drop" style="animation-delay: 0.5s" d="M8 19.29l1.41-1.41c.4-.4.4-1.04 0-1.41L8 15.06l-1.41 1.41c-.4.4-.4 1.04 0 1.41L8 19.29z"/>
            <path class="rain-drop" style="animation-delay: 0.2s" d="M16 19.29l1.41-1.41c.4-.4.4-1.04 0-1.41l-1.41-1.41-1.41 1.41c-.4.4-.4 1.04 0 1.41L16 19.29z"/>
          </svg>`;
      } else {
          svgIcon = `<svg class="sun-icon" viewBox="0 0 24 24">
            <path d="M12 2.25c-.41 0-.75.34-.75.75v2c0 .41.34.75.75.75s.75-.34.75-.75v-2c0-.41-.34-.75-.75-.75zm5.66 3.09c-.29-.29-.77-.29-1.06 0s-.29.77 0 1.06l1.41 1.41c.29.29.77.29 1.06 0s.29-.77 0-1.06l-1.41-1.41zM21 11.25h-2c-.41 0-.75.34-.75.75s.34.75.75.75h2c.41 0 .75-.34.75-.75s-.34-.75-.75-.75zM17.66 18.84c.29.29.77.29 1.06 0s.29-.77 0-1.06l-1.41-1.41c-.29-.29-.77-.29-1.06 0s-.29.77 0 1.06l1.41 1.41zM12 18.25c-.41 0-.75.34-.75.75v2c0 .41.34.75.75.75s.75-.34.75-.75v-2c0-.41-.34-.75-.75-.75zm-5.66-3.09c.29.29.77.29 1.06 0s.29-.77 0-1.06l-1.41-1.41c-.29-.29-.77-.29-1.06 0s-.29.77 0 1.06l1.41 1.41zM3 11.25h-2c-.41 0-.75.34-.75.75s.34.75.75.75h2c.41 0 .75-.34.75-.75s-.34-.75-.75-.75zM6.34 5.16c-.29-.29-.77-.29-1.06 0s-.29.77 0 1.06l1.41 1.41c.29.29.77.29 1.06 0s.29-.77 0-1.06L6.34 5.16zM12 7.25c-2.62 0-4.75 2.13-4.75 4.75s2.13 4.75 4.75 4.75 4.75-2.13 4.75-4.75-2.13-4.75-4.75-4.75z"/>
          </svg>`;
      }
      document.getElementById('weather-icon').innerHTML = svgIcon;
  }
}"""
html = html.replace(old_update_sat, new_update_sat)

# 6. Trigger runExpertAI inside fetchData
fetch_block = "if(data.isManual == 1) {"
html = html.replace(fetch_block, "runExpertAI(); // Update AI analysis\n      " + fetch_block)

# 7. Add alarm trigger in fetchData
fetch_error_block = """      if (data.isWaiting == 1 || (data.errorMsg && data.errorMsg !== "")) {"""
new_error_block = """      if (data.errorMsg && data.errorMsg !== "") playAlarm();\n""" + fetch_error_block
html = html.replace(fetch_error_block, new_error_block)

# 8. Sync Slider values dynamically when data arrives
sync_slider = """
      if(data.batasSuhu) {
         document.getElementById('temp-slider').value = data.batasSuhu;
         document.getElementById('temp-slider-val').innerText = data.batasSuhu + "°C";
      }
      if(data.batasTanah) {
         document.getElementById('soil-slider').value = data.batasTanah;
         document.getElementById('soil-slider-val').innerText = data.batasTanah + "%";
      }
"""
html = html.replace("document.getElementById('cost-val').innerText = \"Rp \" + data.costIdr;", sync_slider + "\n      document.getElementById('cost-val').innerText = \"Rp \" + data.costIdr;")

# 9. Call runExpertAI on load
html = html.replace("window.onload = function() { updateSatUI(); fetchData(); };", "window.onload = function() { updateSatUI(); runExpertAI(); fetchData(); };")


with open('index.h', 'w') as f:
    f.write(html)
print("UI Patched!")
