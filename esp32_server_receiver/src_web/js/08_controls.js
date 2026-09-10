/**
 * 08_controls.js - Actuators, Relay, Schedule & Threshold Controller
 * Zero-Dummy, Real Hardware Actuation on Pin 26 & NVS Config
 * Strict line limit < 400 lines
 */

function toggleSystemMode(isManual) {
  var modeStr = isManual ? "manual" : "auto";
  fetch('/setMode?m=' + modeStr)
    .then(function (res) { if (typeof fetchData === 'function') fetchData(); })
    .catch(function (err) { alert("Gagal ubah mode"); });
}

function sendRelayCommand(stateStr) {
  fetch('/toggleRelay?s=' + stateStr)
    .then(function (res) { if (typeof fetchData === 'function') fetchData(); })
    .catch(function (err) { alert("Gagal kirim perintah relay"); });
}


function toggleLampSystemMode(isManual) {
  var modeStr = isManual ? "manual" : "auto";
  fetch('/setLampMode?m=' + modeStr)
    .then(function (res) { if (typeof fetchData === 'function') fetchData(); })
    .catch(function (err) { alert("Gagal ubah mode lampu"); });
}

function sendLampRelayCommand(stateStr) {
  fetch('/toggleLamp?s=' + stateStr)
    .then(function (res) { if (typeof fetchData === 'function') fetchData(); })
    .catch(function (err) { alert("Gagal kirim perintah lampu"); });
}

function resetPumpStats() {
  if (confirm("Apakah Anda yakin ingin mereset statistik akumulasi air & durasi pompa ke 0?")) {
    fetch('/resetStats')
      .then(function (res) { if (typeof fetchData === 'function') fetchData(); })
      .catch(function (err) { alert("Gagal reset statistik"); });
  }
}

function togglePumpConfigForm() {
  var b = document.getElementById('pump-config-body');
  var a = document.getElementById('pump-config-arrow');
  if (!b) return;
  if (b.style.display === 'none') {
    b.style.display = 'block';
    if (a) a.innerText = '▲';
  } else {
    b.style.display = 'none';
    if (a) a.innerText = '▼';
  }
}

function savePumpConfig() {
  var lph = document.getElementById('cfg-pump-lph').value;
  var watt = document.getElementById('cfg-pump-watt').value;
  var tariff = document.getElementById('cfg-pln-tariff').value;

  fetch('/setPumpConfig?lph=' + lph + '&watt=' + watt + '&tariff=' + tariff)
    .then(function (res) { return res.json(); })
    .then(function (json) {
      alert("Kalibrasi Pompa & Listrik Disimpan!\n" +
        "• Debit Pompa: " + lph + " Liter/Jam (1800 L/H = 0.5 L/detik)\n" +
        "• Daya Pompa: " + watt + " Watt\n" +
        "• Tarif Listrik PLN: Rp " + tariff + " / kWh");
      if (typeof fetchData === 'function') fetchData();
    })
    .catch(function (err) { alert("Gagal simpan kalibrasi pompa"); });
}

function syncRtcWithBrowser() {
  var now = new Date();
  var ep = Math.floor(now.getTime() / 1000);
  var y = now.getFullYear();
  var m = now.getMonth() + 1;
  var d = now.getDate();
  var h = now.getHours();
  var min = now.getMinutes();
  var s = now.getSeconds();

  var url = '/setRtc?epoch=' + ep + '&y=' + y + '&m=' + m + '&d=' + d + '&h=' + h + '&min=' + min + '&s=' + s;
  fetch(url)
    .then(function (res) { return res.json(); })
    .then(function (json) {
      alert("Jam RTC Berhasil Disinkronkan ke Jam Browser (WIB)!\nWaktu RTC Terbaru: " + (typeof formatFriendlyDateTime === 'function' ? formatFriendlyDateTime(json.rtcTime) : json.rtcTime));
      if (typeof fetchData === 'function') fetchData();
    })
    .catch(function (err) {
      if (typeof fetchData === 'function') fetchData();
    });
}


function saveLampSchedule() {
  var en = document.getElementById('lamp-sched-en').checked ? 1 : 0;
  var tVal = document.getElementById('lamp-sched-time').value || '18:00';
  var dur = document.getElementById('lamp-sched-dur').value || 12;
  var parts = tVal.split(':');
  var h = parseInt(parts[0], 10);
  var m = parseInt(parts[1], 10);
  
  fetch('/setLampSchedule?en=' + en + '&h=' + h + '&m=' + m + '&dur=' + dur)
    .then(function (res) { return res.json(); })
    .then(function (json) {
      alert("Jadwal Lampu Grow Light berhasil disimpan ke NVS ESP32!");
      if (typeof fetchData === 'function') fetchData();
    })
    .catch(function (err) { alert("Gagal simpan jadwal lampu"); });
}

function saveSchedule(slotNum) {
  var en = document.getElementById('sched' + slotNum + '-en').checked ? 1 : 0;
  var tVal = document.getElementById('sched' + slotNum + '-time').value || '06:00';
  var dur = document.getElementById('sched' + slotNum + '-dur').value || 15;
  var parts = tVal.split(':');
  var h = parseInt(parts[0], 10);
  var m = parseInt(parts[1], 10);
  fetch('/setSchedule?slot=' + slotNum + '&en=' + en + '&h=' + h + '&m=' + m + '&dur=' + dur)
    .then(function (res) { return res.json(); })
    .then(function (json) {
      alert("Jadwal Slot " + slotNum + " berhasil disimpan ke NVS!");
      if (typeof fetchData === 'function') fetchData();
    })
    .catch(function (err) { alert("Gagal simpan jadwal"); });
}

var CROP_GUIDES = {
  0: {
    name: "Penyemaian Benih (Nursery / Kecambah)",
    badge: "Mode Semai Benih",
    soil: 65,
    temp: 30.0,
    guide: `<div style="display:flex; flex-direction:column; gap:10px;">
        <div style="color:var(--text-main); font-weight:700; font-size:12px; margin-bottom:-4px;">Karakteristik & Target Agronomi:</div>
        <div style="display:flex; gap:8px; align-items:flex-start;">
          <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="var(--primary)" stroke-width="2" style="margin-top:1px; flex-shrink:0;"><path d="M12 2.69l5.66 5.66a8 8 0 1 1-11.31 0z"></path></svg>
          <div><b>Kelembaban Tinggi (Min 65%):</b> Menjaga akar dangkal kecambah agar tidak mencapai titik layu kritis (55%).</div>
        </div>
        <div style="display:flex; gap:8px; align-items:flex-start;">
          <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="var(--warning)" stroke-width="2" style="margin-top:1px; flex-shrink:0;"><path d="M14 14.76V3.5a2.5 2.5 0 0 0-5 0v11.26a4.5 4.5 0 1 0 5 0z"></path></svg>
          <div><b>Batas Panas (Maks 30°C):</b> Mencegah stres termal pada daun muda. Otomatis siram darurat jika suhu ekstrem.</div>
        </div>
        <div style="display:flex; gap:8px; align-items:flex-start;">
          <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="var(--secondary)" stroke-width="2" style="margin-top:1px; flex-shrink:0;"><path d="M12 22s8-4 8-10V5l-8-3-8 3v7c0 6 8 10 8 10z"></path></svg>
          <div><b>Proteksi Jamur:</b> <i>Interlock</i> aktif mencegah genangan tanah yang memicu penyakit <i>Damping-off</i>.</div>
        </div>
      </div>`
  },
  1: {
    name: "Tanaman Buah (Vegetatif: Daun, Batang & Akar)",
    badge: "Buah (Vegetatif)",
    soil: 50,
    temp: 32.0,
    guide: `<div style="display:flex; flex-direction:column; gap:10px;">
        <div style="color:var(--text-main); font-weight:700; font-size:12px; margin-bottom:-4px;">Karakteristik & Target Agronomi:</div>
        <div style="display:flex; gap:8px; align-items:flex-start;">
          <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="var(--primary)" stroke-width="2" style="margin-top:1px; flex-shrink:0;"><path d="M12 2.69l5.66 5.66a8 8 0 1 1-11.31 0z"></path></svg>
          <div><b>Kelembaban Sedang (Min 50%):</b> Sengaja diatur lebih rendah untuk mendorong akar menembus tanah lebih dalam mencari air.</div>
        </div>
        <div style="display:flex; gap:8px; align-items:flex-start;">
          <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="var(--warning)" stroke-width="2" style="margin-top:1px; flex-shrink:0;"><path d="M14 14.76V3.5a2.5 2.5 0 0 0-5 0v11.26a4.5 4.5 0 1 0 5 0z"></path></svg>
          <div><b>Toleransi Panas (Maks 32°C):</b> Tanaman di fase ini lebih kebal terhadap suhu ekstrem, fokus pada pelebaran tajuk daun.</div>
        </div>
      </div>`
  },
  2: {
    name: "Tanaman Buah (Generatif: Bunga & Buah)",
    badge: "Buah (Generatif)",
    soil: 45,
    temp: 33.0,
    guide: `<div style="display:flex; flex-direction:column; gap:10px;">
        <div style="color:var(--text-main); font-weight:700; font-size:12px; margin-bottom:-4px;">Karakteristik & Target Agronomi:</div>
        <div style="display:flex; gap:8px; align-items:flex-start;">
          <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="var(--primary)" stroke-width="2" style="margin-top:1px; flex-shrink:0;"><path d="M12 2.69l5.66 5.66a8 8 0 1 1-11.31 0z"></path></svg>
          <div><b>Irigasi Terkontrol (Min 45%):</b> Mencegah penyiraman berlebih yang dapat menyebabkan kulit buah pecah atau bunga rontok.</div>
        </div>
        <div style="display:flex; gap:8px; align-items:flex-start;">
          <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="var(--warning)" stroke-width="2" style="margin-top:1px; flex-shrink:0;"><path d="M14 14.76V3.5a2.5 2.5 0 0 0-5 0v11.26a4.5 4.5 0 1 0 5 0z"></path></svg>
          <div><b>Fokus Pematangan (Maks 33°C):</b> Iklim hangat dan kelembaban rendah membantu meningkatkan kadar gula (Brix) buah.</div>
        </div>
      </div>`
  },
  3: {
    name: "Kustom / Pengaturan Manual Slider",
    badge: "Mode Kustom",
    soil: null,
    temp: null,
    guide: `<div style="display:flex; flex-direction:column; gap:10px;">
        <div style="color:var(--text-main); font-weight:700; font-size:12px; margin-bottom:-4px;">Karakteristik & Target Agronomi:</div>
        <div style="display:flex; gap:8px; align-items:flex-start;">
          <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="var(--secondary)" stroke-width="2" style="margin-top:1px; flex-shrink:0;"><circle cx="12" cy="12" r="3"></circle><path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1 0 2.83 2 2 0 0 1-2.83 0l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-2 2 2 2 0 0 1-2-2v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83 0 2 2 0 0 1 0-2.83l.06-.06a1.65 1.65 0 0 0 .33-1.82 1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1-2-2 2 2 0 0 1 2-2h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 0-2.83 2 2 0 0 1 2.83 0l.06.06a1.65 1.65 0 0 0 1.82.33H9a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 2-2 2 2 0 0 1 2 2v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 0 2 2 0 0 1 0 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82V9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 2 2 2 2 0 0 1-2 2h-.09a1.65 1.65 0 0 0-1.51 1z"></path></svg>
          <div><b>Kontrol Penuh (Manual Override):</b> Batas siram tanah dan ambang suhu darurat sepenuhnya ditentukan oleh Anda.</div>
        </div>
        <div style="display:flex; gap:8px; align-items:flex-start;">
          <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="var(--text-sub)" stroke-width="2" style="margin-top:1px; flex-shrink:0;"><path d="M17 21v-2a4 4 0 0 0-4-4H5a4 4 0 0 0-4 4v2"></path><circle cx="9" cy="7" r="4"></circle><path d="M23 21v-2a4 4 0 0 0-3-3.87"></path><path d="M16 3.13a4 4 0 0 1 0 7.75"></path></svg>
          <div><b>Pengaturan Bebas:</b> Silakan geser <i>slider</i> di bawah ini untuk mengatur parameter operasional sesuai preferensi budidaya kustom Anda.</div>
        </div>
      </div>`
  }
};

function updateCropProfileUI(mode) {
  mode = parseInt(mode, 10);
  var g = CROP_GUIDES[mode];
  if (g) {
    var guideEl = document.getElementById('crop-profile-guidance');
    if (guideEl) guideEl.innerHTML = g.guide;
    var badgeEl = document.getElementById('badge-crop-profile');
    if (badgeEl) badgeEl.innerText = g.badge;
  }
}

function onCropProfileChange(mode) {
  mode = parseInt(mode, 10);
  var g = CROP_GUIDES[mode];
  updateCropProfileUI(mode);

  if (mode !== 3 && g && g.soil !== null) {
    var sSoil = document.getElementById('slider-soil');
    if (sSoil) sSoil.value = g.soil;
    var vSoil = document.getElementById('val-slider-soil');
    if (vSoil) vSoil.innerText = g.soil + '%';

    var sTemp = document.getElementById('slider-temp');
    if (sTemp) sTemp.value = g.temp;
    var vTemp = document.getElementById('val-slider-temp');
    if (vTemp) vTemp.innerText = g.temp + '°C';
    
    saveThresholds(true);
  }

  fetch('/setCropProfile?mode=' + mode)
    .then(function (r) { return r.json(); })
    .then(function (res) {
      if (mode === 0 && document.getElementById('crop-stage')) {
        document.getElementById('crop-stage').value = 'semai';
        if (typeof updateCropAgronomyAnalysis === 'function') updateCropAgronomyAnalysis();
      } else if (mode === 1 && document.getElementById('crop-stage')) {
        document.getElementById('crop-stage').value = 'vegetatif';
        if (typeof updateCropAgronomyAnalysis === 'function') updateCropAgronomyAnalysis();
      } else if (mode === 2 && document.getElementById('crop-stage')) {
        document.getElementById('crop-stage').value = 'generatif';
        if (typeof updateCropAgronomyAnalysis === 'function') updateCropAgronomyAnalysis();
      }
    })
    .catch(function (err) {
      console.warn("setCropProfile error", err);
    });
}

function onSliderManualChange(type, val) {
  if (type === 'soil') {
    var vSoil = document.getElementById('val-slider-soil');
    if (vSoil) vSoil.innerText = val + '%';
  } else {
    var vTemp = document.getElementById('val-slider-temp');
    if (vTemp) vTemp.innerText = val + '°C';
  }
  var sel = document.getElementById('crop-profile-select');
  if (sel && sel.value !== "3") {
    sel.value = "3";
    updateCropProfileUI(3);
  }
}

function saveThresholds(silent) {
  var soilVal = document.getElementById('slider-soil').value;
  var tempVal = document.getElementById('slider-temp').value;
  fetch('/setThreshold?soil=' + soilVal + '&temp=' + tempVal)
    .then(function (res) {
      if (!silent) alert("Batas Ambang Berhasil Disimpan Permanen ke ESP32!");
      if (typeof fetchData === 'function') fetchData();
    })
    .catch(function (err) {
      if (!silent) alert("Gagal simpan threshold");
      else console.error("Auto-save failed");
    });
}

function clearSystemLogs() {
  if (confirm("Apakah Anda yakin ingin menghapus seluruh log LittleFS?")) {
    fetch('/clearLogs', { method: 'POST' })
      .then(function (res) {
        alert("Log Berhasil Direset!");
        if (typeof fetchData === 'function') fetchData();
        if (typeof fetchAndParseLogs === 'function') fetchAndParseLogs();
      })
      .catch(function (err) { alert("Gagal hapus log"); });
  }
}
