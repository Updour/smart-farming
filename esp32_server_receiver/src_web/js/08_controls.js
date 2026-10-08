/**
 * 08_controls.js - Actuators, Relay, Schedule & Threshold Controller
 * Zero-Dummy, Real Hardware Actuation on Pin 26 & NVS Config
 * Strict line limit < 400 lines
 */

function toggleSystemMode(isManual) {
  var modeStr = isManual ? "manual" : "auto";
  var btnPumpAuto = document.getElementById('btn-pump-mode-auto');
  var btnPumpManual = document.getElementById('btn-pump-mode-manual');
  var btnOn = document.getElementById('btn-pump-on');
  var btnOff = document.getElementById('btn-pump-off');
  var mDesc = document.getElementById('mode-desc-text');
  
  // Instant visual feedback
  if (btnPumpAuto && btnPumpManual) {
    if (isManual) {
      btnPumpManual.className = "segment-btn active-manual";
      btnPumpAuto.className = "segment-btn";
    } else {
      btnPumpAuto.className = "segment-btn active";
      btnPumpManual.className = "segment-btn";
    }
  }
  if (btnOn) btnOn.disabled = !isManual;
  if (btnOff) btnOff.disabled = !isManual;
  if (mDesc) mDesc.innerText = isManual ? "Mode Manual (Aktuator Terbuka)" : "Mode Otomatis (Sensor & RTC)";

  fetch('/setMode?m=' + modeStr)
    .then(function (res) { if (typeof fetchData === 'function') fetchData(); })
    .catch(function (err) { alert("Gagal ubah mode pompa"); });
}

function sendRelayCommand(stateStr) {
  var btnOn = document.getElementById('btn-pump-on');
  if (btnOn && btnOn.disabled) {
    alert("Tombol Pompa Terkunci! Pompa berada dalam Mode Otomatis (AUTO). Ubah ke Mode Manual terlebih dahulu jika ingin menyalakan/mematikan secara manual.");
    return;
  }
  fetch('/toggleRelay?s=' + stateStr)
    .then(function (res) {
      if (!res.ok) {
        return res.text().then(function (txt) { throw new Error(txt); });
      }
      if (typeof fetchData === 'function') fetchData();
    })
    .catch(function (err) { alert(err.message || "Gagal kirim perintah relay"); });
}


function toggleLampSystemMode(isManual) {
  var modeStr = isManual ? "manual" : "auto";
  var btnLampAuto = document.getElementById('btn-lamp-mode-auto');
  var btnLampManual = document.getElementById('btn-lamp-mode-manual');
  var btnLampOn = document.getElementById('btn-lamp-on');
  var btnLampOff = document.getElementById('btn-lamp-off');
  var mLampDesc = document.getElementById('lamp-mode-desc-text');
  
  // Instant visual feedback
  if (btnLampAuto && btnLampManual) {
    if (isManual) {
      btnLampManual.className = "segment-btn active-manual";
      btnLampAuto.className = "segment-btn";
    } else {
      btnLampAuto.className = "segment-btn active";
      btnLampManual.className = "segment-btn";
    }
  }
  if (btnLampOn) btnLampOn.disabled = !isManual;
  if (btnLampOff) btnLampOff.disabled = !isManual;
  if (mLampDesc) mLampDesc.innerText = isManual ? "Mode Manual (Aktuator Terbuka)" : "Mode Otomatis (Timer & Jadwal)";

  fetch('/setLampMode?m=' + modeStr)
    .then(function (res) { if (typeof fetchData === 'function') fetchData(); })
    .catch(function (err) { alert("Gagal ubah mode lampu"); });
}

function sendLampRelayCommand(stateStr) {
  var btnLampOn = document.getElementById('btn-lamp-on');
  if (btnLampOn && btnLampOn.disabled) {
    alert("Tombol Lampu Terkunci! Lampu berada dalam Mode Otomatis (AUTO). Ubah ke Mode Manual terlebih dahulu jika ingin menyalakan/mematikan secara manual.");
    return;
  }
  fetch('/toggleLamp?s=' + stateStr)
    .then(function (res) {
      if (!res.ok) {
        return res.text().then(function (txt) { throw new Error(txt); });
      }
      if (typeof fetchData === 'function') fetchData();
    })
    .catch(function (err) { alert(err.message || "Gagal kirim perintah lampu"); });
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
  var lph = document.getElementById('cfg-pump-lph').value || 1800;
  var watt = document.getElementById('cfg-pump-watt').value || 25;
  var tariff = document.getElementById('cfg-pln-tariff').value || 415;

  try {
    localStorage.setItem('smartfarm_pump_lph', lph);
    localStorage.setItem('smartfarm_pump_watt', watt);
    localStorage.setItem('smartfarm_pump_tariff', tariff);
  } catch (e) {}

  var statusBadge = document.getElementById('pump-save-status');
  if (statusBadge) {
    statusBadge.innerHTML = '<span class="badge-dot dot-yellow" style="background:#eab308;"></span>Menyimpan ke ESP32...';
    statusBadge.style.color = 'var(--warning)';
  }

  fetch('/setPumpConfig?lph=' + lph + '&watt=' + watt + '&tariff=' + tariff)
    .then(function (res) { return res.json(); })
    .then(function (json) {
      if (statusBadge) {
        statusBadge.innerHTML = '<span class="badge-dot dot-green" style="background:#10b981;"></span>Tersimpan di ESP32 ✓';
        statusBadge.style.color = 'var(--primary)';
      }
      if (typeof fetchData === 'function') fetchData();
    })
    .catch(function (err) {
      if (statusBadge) {
        statusBadge.innerHTML = '<span class="badge-dot dot-yellow" style="background:#eab308;"></span>Tersimpan di Cache Browser';
        statusBadge.style.color = 'var(--warning)';
      }
    });
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

  var rtcLive = document.getElementById('rtc-live-time');
  if (rtcLive) rtcLive.innerText = "Sinkronisasi...";

  var url = '/setRtc?epoch=' + ep + '&y=' + y + '&m=' + m + '&d=' + d + '&h=' + h + '&min=' + min + '&s=' + s;
  fetch(url)
    .then(function (res) { return res.json(); })
    .then(function (json) {
      var rtcValid = document.getElementById('rtc-valid-badge');
      if (rtcValid) {
        rtcValid.innerText = "RTC Tersinkron";
        rtcValid.style.color = "var(--primary)";
      }
      if (typeof fetchData === 'function') fetchData();
    })
    .catch(function (err) {
      if (typeof fetchData === 'function') fetchData();
    });
}

function saveLampSchedule() {
  var elEn = document.getElementById('lamp-sched-en');
  var elTm = document.getElementById('lamp-sched-time');
  var elDur = document.getElementById('lamp-sched-dur');
  var en = elEn && elEn.checked ? 1 : 0;
  var tVal = (elTm && elTm.value) ? elTm.value : '18:00';
  var dur = (elDur && elDur.value) ? parseInt(elDur.value, 10) : 12;
  if (isNaN(dur) || dur < 1) dur = 12;
  var parts = tVal.split(':');
  var h = parseInt(parts[0], 10);
  if (isNaN(h)) h = 18;
  var m = parseInt(parts[1], 10);
  if (isNaN(m)) m = 0;

  try {
    localStorage.setItem('smartfarm_lamp_en', en);
    localStorage.setItem('smartfarm_lamp_time', tVal);
    localStorage.setItem('smartfarm_lamp_dur', dur);
  } catch (e) {}

  var statusBadge = document.getElementById('lamp-save-status');
  if (statusBadge) {
    statusBadge.innerHTML = '<span class="badge-dot dot-yellow" style="background:#eab308;"></span>Menyimpan ke ESP32...';
    statusBadge.style.color = 'var(--warning)';
  }

  fetch('/setLampSchedule?en=' + en + '&h=' + h + '&m=' + m + '&dur=' + dur)
    .then(function (res) {
      if (!res.ok) throw new Error("Status " + res.status);
      if (statusBadge) {
        statusBadge.innerHTML = '<span class="badge-dot dot-green" style="background:#10b981;"></span>Jadwal Lampu Tersimpan di ESP32 ✓';
        statusBadge.style.color = 'var(--primary)';
      }
      if (typeof fetchData === 'function') fetchData();
    })
    .catch(function (err) {
      if (statusBadge) {
        statusBadge.innerHTML = '<span class="badge-dot dot-yellow" style="background:#eab308;"></span>Tersimpan di Cache Browser';
        statusBadge.style.color = 'var(--warning)';
      }
    });
}

function saveSchedule(slotNum) {
  var elEn = document.getElementById('sched' + slotNum + '-en');
  var elTm = document.getElementById('sched' + slotNum + '-time');
  var elDur = document.getElementById('sched' + slotNum + '-dur');
  var en = elEn && elEn.checked ? 1 : 0;
  var defaultTime = (slotNum === 1 ? '06:00' : '17:00');
  var defaultDur = (slotNum === 1 ? 15 : 10);
  var tVal = (elTm && elTm.value) ? elTm.value : defaultTime;
  var dur = (elDur && elDur.value) ? parseInt(elDur.value, 10) : defaultDur;
  if (isNaN(dur) || dur < 1) dur = defaultDur;
  var parts = tVal.split(':');
  var h = parseInt(parts[0], 10);
  if (isNaN(h)) h = (slotNum === 1 ? 6 : 17);
  var m = parseInt(parts[1], 10);
  if (isNaN(m)) m = 0;

  try {
    localStorage.setItem('smartfarm_sched' + slotNum + '_en', en);
    localStorage.setItem('smartfarm_sched' + slotNum + '_time', tVal);
    localStorage.setItem('smartfarm_sched' + slotNum + '_dur', dur);
  } catch (e) {}

  var statusBadge = document.getElementById('sched' + slotNum + '-save-status');
  if (statusBadge) {
    statusBadge.innerHTML = '<span class="badge-dot dot-yellow" style="background:#eab308;"></span>Menyimpan ke ESP32...';
    statusBadge.style.color = 'var(--warning)';
  }

  fetch('/setSchedule?slot=' + slotNum + '&en=' + en + '&h=' + h + '&m=' + m + '&dur=' + dur)
    .then(function (res) {
      if (!res.ok) throw new Error("Status " + res.status);
      if (statusBadge) {
        statusBadge.innerHTML = '<span class="badge-dot dot-green" style="background:#10b981;"></span>Slot ' + slotNum + ' Tersimpan di ESP32 ✓';
        statusBadge.style.color = 'var(--primary)';
      }
      if (typeof fetchData === 'function') fetchData();
    })
    .catch(function (err) {
      if (statusBadge) {
        statusBadge.innerHTML = '<span class="badge-dot dot-yellow" style="background:#eab308;"></span>Tersimpan di Cache Browser';
        statusBadge.style.color = 'var(--warning)';
      }
    });
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

var _sliderSaveTimer = null;
window._sliderSaveTimer = null;

function onSliderManualChange(type, val) {
  if (type === 'soil') {
    var vSoil = document.getElementById('val-slider-soil');
    if (vSoil) vSoil.innerText = val + '%';
    try { localStorage.setItem('smartfarm_batasTanah', val); } catch (e) {}
  } else {
    var vTemp = document.getElementById('val-slider-temp');
    if (vTemp) vTemp.innerText = val + '°C';
    try { localStorage.setItem('smartfarm_batasSuhu', val); } catch (e) {}
  }
  var sel = document.getElementById('crop-profile-select');
  if (sel && sel.value !== "3") {
    sel.value = "3";
    updateCropProfileUI(3);
  }

  var statusBadge = document.getElementById('threshold-save-status');
  if (statusBadge) {
    statusBadge.innerHTML = '<span class="badge-dot dot-yellow" style="background:#eab308;"></span>Menyimpan ke ESP32...';
    statusBadge.style.color = 'var(--warning)';
  }

  // Debounced auto-save ke ESP32 600ms setelah user selesai menggeser
  if (_sliderSaveTimer) clearTimeout(_sliderSaveTimer);
  window._sliderSaveTimer = true;
  _sliderSaveTimer = setTimeout(function () {
    window._sliderSaveTimer = null;
    saveThresholds(true);
  }, 600);
}

function saveThresholds(silent) {
  var elSoil = document.getElementById('slider-soil');
  var elTemp = document.getElementById('slider-temp');
  if (!elSoil || !elTemp) return;
  var soilVal = elSoil.value;
  var tempVal = elTemp.value;

  try {
    localStorage.setItem('smartfarm_batasTanah', soilVal);
    localStorage.setItem('smartfarm_batasSuhu', tempVal);
  } catch (e) {}

  var statusBadge = document.getElementById('threshold-save-status');
  if (statusBadge) {
    statusBadge.innerHTML = '<span class="badge-dot dot-yellow" style="background:#eab308;"></span>Menyimpan...';
    statusBadge.style.color = 'var(--warning)';
  }

  fetch('/setThreshold?soil=' + encodeURIComponent(soilVal) + '&temp=' + encodeURIComponent(tempVal))
    .then(function (res) {
      if (res.ok) {
        if (statusBadge) {
          statusBadge.innerHTML = '<span class="badge-dot dot-green" style="background:#10b981;"></span>Tersimpan di ESP32 ✓';
          statusBadge.style.color = 'var(--primary)';
        }
        if (!silent) alert("Batas Ambang Berhasil Disimpan Permanen ke NVS ESP32!\nTanah: < " + soilVal + "% | Suhu: > " + tempVal + "°C");
      } else {
        if (statusBadge) {
          statusBadge.innerHTML = '<span class="badge-dot dot-red" style="background:#ef4444;"></span>Gagal Simpan ✗';
          statusBadge.style.color = 'var(--danger)';
        }
        if (!silent) alert("Gagal menyimpan threshold ke ESP32");
      }
      if (typeof fetchData === 'function') fetchData();
    })
    .catch(function (err) {
      if (statusBadge) {
        statusBadge.innerHTML = '<span class="badge-dot dot-yellow" style="background:#eab308;"></span>Tersimpan di Cache Browser';
        statusBadge.style.color = 'var(--warning)';
      }
      if (!silent) alert("Gagal koneksi ke ESP32. Nilai tersimpan di browser.");
      else console.error("Auto-save failed", err);
    });
}

document.addEventListener('DOMContentLoaded', function () {
  try {
    // 1. Ambang Batas Sensor
    var savedSoil = localStorage.getItem('smartfarm_batasTanah');
    var savedTemp = localStorage.getItem('smartfarm_batasSuhu');
    if (savedSoil !== null && savedSoil !== '') {
      var elSoil = document.getElementById('slider-soil');
      var valSoil = document.getElementById('val-slider-soil');
      if (elSoil) elSoil.value = savedSoil;
      if (valSoil) valSoil.innerText = savedSoil + '%';
    }
    if (savedTemp !== null && savedTemp !== '') {
      var elTemp = document.getElementById('slider-temp');
      var valTemp = document.getElementById('val-slider-temp');
      if (elTemp) elTemp.value = savedTemp;
      if (valTemp) valTemp.innerText = savedTemp + '°C';
    }

    // 2. Jadwal Slot 1 (Pagi)
    var s1en = localStorage.getItem('smartfarm_sched1_en');
    var s1tm = localStorage.getItem('smartfarm_sched1_time');
    var s1dur = localStorage.getItem('smartfarm_sched1_dur');
    if (s1en !== null) {
      var elS1En = document.getElementById('sched1-en');
      if (elS1En) elS1En.checked = (s1en == '1' || s1en === true);
    }
    if (s1tm) {
      var elS1Tm = document.getElementById('sched1-time');
      if (elS1Tm) elS1Tm.value = s1tm;
    }
    if (s1dur) {
      var elS1Dur = document.getElementById('sched1-dur');
      if (elS1Dur) elS1Dur.value = s1dur;
    }

    // 3. Jadwal Slot 2 (Sore)
    var s2en = localStorage.getItem('smartfarm_sched2_en');
    var s2tm = localStorage.getItem('smartfarm_sched2_time');
    var s2dur = localStorage.getItem('smartfarm_sched2_dur');
    if (s2en !== null) {
      var elS2En = document.getElementById('sched2-en');
      if (elS2En) elS2En.checked = (s2en == '1' || s2en === true);
    }
    if (s2tm) {
      var elS2Tm = document.getElementById('sched2-time');
      if (elS2Tm) elS2Tm.value = s2tm;
    }
    if (s2dur) {
      var elS2Dur = document.getElementById('sched2-dur');
      if (elS2Dur) elS2Dur.value = s2dur;
    }

    // 4. Jadwal Lampu Grow Light
    var lEn = localStorage.getItem('smartfarm_lamp_en');
    var lTm = localStorage.getItem('smartfarm_lamp_time');
    var lDur = localStorage.getItem('smartfarm_lamp_dur');
    if (lEn !== null) {
      var elLEn = document.getElementById('lamp-sched-en');
      if (elLEn) elLEn.checked = (lEn == '1' || lEn === true);
    }
    if (lTm) {
      var elLTm = document.getElementById('lamp-sched-time');
      if (elLTm) elLTm.value = lTm;
    }
    if (lDur) {
      var elLDur = document.getElementById('lamp-sched-dur');
      if (elLDur) elLDur.value = lDur;
    }

    // 5. Kalibrasi & Spesifikasi Pompa
    var pLph = localStorage.getItem('smartfarm_pump_lph');
    var pWatt = localStorage.getItem('smartfarm_pump_watt');
    var pTariff = localStorage.getItem('smartfarm_pump_tariff');
    if (pLph) {
      var elLph = document.getElementById('cfg-pump-lph');
      if (elLph) elLph.value = pLph;
    }
    if (pWatt) {
      var elWatt = document.getElementById('cfg-pump-watt');
      if (elWatt) elWatt.value = pWatt;
    }
    if (pTariff) {
      var elTariff = document.getElementById('cfg-pln-tariff');
      if (elTariff) elTariff.value = pTariff;
    }
  } catch (e) {
    console.warn("Hydrate form from localStorage failed", e);
  }
});

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
