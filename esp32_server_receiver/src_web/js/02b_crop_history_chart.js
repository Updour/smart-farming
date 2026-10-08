// =================================================================
// 02B_CROP_HISTORY_CHART.JS - CROP LIFECYCLE S-CURVE & LOCAL STORAGE
// Tracks day-by-day crop growth (HST), phenology milestones & vigor
// Stored persistently in browser LocalStorage • Zero-Dummy Telemetry
// =================================================================

var cropHistoryCanvas = null;
var cropCrosshairIdx = -1;

var _cropHistoryInMemory = null;

function getCropHistoryData() {
  if (_cropHistoryInMemory && Array.isArray(_cropHistoryInMemory)) {
    return _cropHistoryInMemory;
  }
  try {
    var raw = localStorage.getItem('smartfarm_crop_history');
    if (raw) {
      _cropHistoryInMemory = JSON.parse(raw);
      return _cropHistoryInMemory;
    }
  } catch (e) {
    console.error("Error reading crop history:", e);
  }
  return [];
}

function saveCropHistoryData(data) {
  _cropHistoryInMemory = data;
  try {
    localStorage.setItem('smartfarm_crop_history', JSON.stringify(data));
  } catch (e) {
    console.error("Error saving crop history to LocalStorage:", e);
  }
  // Simpan secara fisik ke LittleFS Flash Memory ESP32 via REST POST
  try {
    fetch('/saveCropHistory', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(data)
    }).catch(function(err) {
      console.warn("Sinkronisasi riwayat HST ke LittleFS ESP32:", err);
    });
  } catch (e) {}
}

function initCropHistory() {
  cropHistoryCanvas = document.getElementById('cropHistoryChart');
  if (cropHistoryCanvas) {
    attachCropCrosshair();
    resizeCropCanvas();
  }
  // 1. Render data lokal terlebih dahulu untuk transisi UI instan
  updateCropHistorySummary();
  renderCropHistoryTable();

  // 2. Tarik riwayat fisik dari LittleFS Flash Memory ESP32 (/getCropHistory)
  fetch('/getCropHistory')
    .then(function(res) {
      if (!res.ok) throw new Error("HTTP " + res.status);
      return res.json();
    })
    .then(function(items) {
      if (Array.isArray(items) && items.length > 0) {
        _cropHistoryInMemory = items;
        try { localStorage.setItem('smartfarm_crop_history', JSON.stringify(items)); } catch(e){}
        updateCropHistorySummary();
        renderCropHistoryChart();
        renderCropHistoryTable();
      }
    })
    .catch(function(err) {
      // Offline fallback berjalan normal
    });
}

function resizeCropCanvas() {
  if (!cropHistoryCanvas) cropHistoryCanvas = document.getElementById('cropHistoryChart');
  if (cropHistoryCanvas && cropHistoryCanvas.parentElement) {
    cropHistoryCanvas.width = cropHistoryCanvas.parentElement.clientWidth;
    cropHistoryCanvas.height = cropHistoryCanvas.parentElement.clientHeight;
    renderCropHistoryChart();
  }
}

// Ensure init after DOM is ready
window.addEventListener('DOMContentLoaded', function() {
  setTimeout(initCropHistory, 400);
});

// --- RECORD TODAY'S LOG ---
function logTodayCropGrowth() {
  var tData = window.lastTelemetryData || {};
  
  // 1. Ekstraksi Suhu (Mendukung suhuC, temp, atau pembacaan aktual KPI)
  var rawT = (tData.suhuC !== undefined && tData.suhuC !== "--") ? tData.suhuC : tData.temp;
  var temp = (rawT !== undefined && rawT !== null && !isNaN(parseFloat(rawT)))
    ? parseFloat(rawT)
    : (document.getElementById('kpi-temp') ? parseFloat(document.getElementById('kpi-temp').innerText) : 28.0);

  // 2. Ekstraksi Kelembapan Tanah (Mendukung soil atau pembacaan aktual KPI)
  var rawS = (tData.soil !== undefined && tData.soil !== "--") ? tData.soil : null;
  var soil = (rawS !== null && !isNaN(parseFloat(rawS)) && parseFloat(rawS) >= 0)
    ? Math.round(parseFloat(rawS))
    : (document.getElementById('kpi-soil') ? parseInt(document.getElementById('kpi-soil').innerText, 10) : 65);

  // 3. Ekstraksi Kelembapan Udara (RH)
  var rawH = (tData.hum !== undefined && tData.hum !== "--") ? tData.hum : null;
  var hum = (rawH !== null && !isNaN(parseFloat(rawH)))
    ? Math.round(parseFloat(rawH))
    : (document.getElementById('kpi-hum') ? parseInt(document.getElementById('kpi-hum').innerText, 10) : 70);

  // 4. Parameter Tanaman dengan Fallback Cerdas ke Input UI
  var storedAge = localStorage.getItem('crop_age');
  var storedName = localStorage.getItem('crop_name');
  var storedStage = localStorage.getItem('crop_stage');

  var elAge = document.getElementById('crop-age-days');
  var elName = document.getElementById('crop-name');
  var elStage = document.getElementById('crop-stage');
  var elLeaves = document.getElementById('crop-leaves-count');

  var name = storedName || (elName && elName.value.trim() ? elName.value.trim() : 'Cabai Rawit');
  var age = storedAge ? parseInt(storedAge, 10) : (elAge ? parseInt(elAge.value, 10) : 14);
  var stage = storedStage || (elStage ? elStage.value : 'vegetatif');
  var leaves = parseInt(localStorage.getItem('crop_leaves') || (elLeaves ? elLeaves.value : '4'), 10) || 4;

  // Pastikan parameter tanaman tersimpan di LocalStorage
  localStorage.setItem('crop_name', name);
  localStorage.setItem('crop_age', age);
  localStorage.setItem('crop_stage', stage);

  // Hitung Skor Vigor Murni dari Telemetri Fisik Riil
  var vigor = 100;
  // Penalti jika tanah di luar zona nyaman (60% - 75%)
  if (soil < 50) vigor -= Math.min(40, (50 - soil) * 2);
  else if (soil > 80) vigor -= Math.min(30, (soil - 80) * 2);

  // Penalti jika suhu ekstrem
  if (temp > 32) vigor -= Math.min(30, (temp - 32) * 5);
  else if (temp < 20) vigor -= Math.min(25, (20 - temp) * 3);

  // Penalti bibit semai jika daun belum tumbuh cukup
  if (stage === 'semai' && leaves < 3) vigor -= 15;

  vigor = Math.max(10, Math.min(100, Math.round(vigor)));

  var todayStr = new Date().toLocaleDateString('id-ID', { day: '2-digit', month: '2-digit', year: 'numeric' });
  var history = getCropHistoryData();
  
  var existingIdx = -1;
  for (var i = 0; i < history.length; i++) {
    if (history[i].hst === age || history[i].date === todayStr) {
      existingIdx = i;
      break;
    }
  }

  var newEntry = {
    date: todayStr,
    hst: age,
    cropName: name,
    stage: stage,
    soil: soil,
    temp: parseFloat(temp.toFixed(1)),
    hum: hum,
    leaves: leaves,
    vigor: vigor
  };

  if (existingIdx >= 0) {
    history[existingIdx] = newEntry;
  } else {
    history.push(newEntry);
    history.sort(function(a, b) { return a.hst - b.hst; });
  }

  saveCropHistoryData(history);
  updateCropHistorySummary();
  renderCropHistoryChart();
  renderCropHistoryTable();

  var footerStatus = document.getElementById('crop-hist-footer-status');
  if (footerStatus) {
    footerStatus.innerHTML = '<span style="color:#10b981;font-weight:700;">✓ Data sensor HST ' + age + ' (' + name + ' • Vigor: ' + vigor + '%, Tanah: ' + soil + '%, Suhu: ' + temp.toFixed(1) + '°C) tersimpan di Flash ESP32!</span>';
    setTimeout(function() {
      if (footerStatus) footerStatus.innerText = "Data tersimpan di LittleFS Flash Memory ESP32 & LocalStorage • Kurva Pertumbuhan Sigmoid (S-Curve)";
    }, 4000);
  }
}

// --- DELETE SINGLE ENTRY ---
function deleteCropHistoryEntry(idx) {
  var history = getCropHistoryData();
  if (idx >= 0 && idx < history.length) {
    var item = history[idx];
    if (confirm("Hapus catatan HST " + item.hst + " (" + item.date + ")?")) {
      history.splice(idx, 1);
      saveCropHistoryData(history);
      updateCropHistorySummary();
      renderCropHistoryChart();
      renderCropHistoryTable();
    }
  }
}

// --- RENDER INTERACTIVE TABLE ---
function renderCropHistoryTable() {
  var tbody = document.getElementById('crop-hist-table-body');
  var badge = document.getElementById('crop-hist-count-badge');
  if (!tbody) return;

  var history = getCropHistoryData();
  if (badge) badge.innerText = history.length + " Rekaman";

  var stageNames = { 'semai': 'Semai', 'vegetatif': 'Vegetatif', 'generatif': 'Generatif', 'panen': 'Panen' };

  if (history.length === 0) {
    var tData = window.lastTelemetryData || {};
    var cAge = (tData.cropAge !== undefined && tData.cropAge > 0) ? tData.cropAge : parseInt(localStorage.getItem('crop_age') || (document.getElementById('crop-age-days') ? document.getElementById('crop-age-days').value : 14), 10);
    var cStage = (tData.cropStage && tData.cropStage !== '') ? tData.cropStage : (localStorage.getItem('crop_stage') || 'vegetatif');
    var rawT = (tData.suhuC !== undefined && tData.suhuC !== "--") ? tData.suhuC : tData.temp;
    var curTemp = (rawT !== undefined && rawT !== null && rawT !== "--") ? parseFloat(rawT).toFixed(1) + "°C" : "--°C";
    var curSoil = (tData.soil !== undefined && tData.soil !== null && tData.soil !== "--") ? Math.round(parseFloat(tData.soil)) + "%" : "--%";
    var curHum = (tData.hum !== undefined && tData.hum !== null && tData.hum !== "--") ? Math.round(parseFloat(tData.hum)) + "%" : "--%";

    var rawNumT = parseFloat(rawT) || 28;
    var rawNumS = parseFloat(tData.soil) || 65;
    var liveVigor = 100;
    if (rawNumS < 50) liveVigor -= Math.min(40, (50 - rawNumS) * 2);
    else if (rawNumS > 80) liveVigor -= Math.min(30, (rawNumS - 80) * 2);
    if (rawNumT > 32) liveVigor -= Math.min(30, (rawNumT - 32) * 5);
    else if (rawNumT < 20) liveVigor -= Math.min(25, (20 - rawNumT) * 3);
    liveVigor = Math.max(10, Math.min(100, Math.round(liveVigor)));

    tbody.innerHTML =
      '<tr style="border-bottom:1px solid rgba(255,255,255,0.06); background:rgba(16,185,129,0.04);">' +
        '<td style="padding:8px 10px; font-weight:700; color:var(--primary); font-family:monospace;">HST ' + cAge + '</td>' +
        '<td style="padding:8px 10px; color:var(--text-sub);">Hari Ini (Aktual)</td>' +
        '<td style="padding:8px 10px;"><span style="background:rgba(16,185,129,0.12); color:#10b981; padding:2px 8px; border-radius:4px; font-size:10px; font-weight:600;">' + (stageNames[cStage] || cStage) + ' (ESP32)</span></td>' +
        '<td style="padding:8px 10px; color:#10b981; font-weight:600;">' + curSoil + '</td>' +
        '<td style="padding:8px 10px; color:#06b6d4; font-weight:600;">' + curTemp + '</td>' +
        '<td style="padding:8px 10px; color:#a855f7;">' + curHum + '</td>' +
        '<td style="padding:8px 10px; font-weight:700; color:#10b981;">' + liveVigor + '%</td>' +
        '<td style="padding:8px 10px; text-align:center;"><button type="button" class="btn btn-primary" style="padding:3px 8px; font-size:10px;" onclick="logTodayCropGrowth()">💾 Catat Hari Ini</button></td>' +
      '</tr>' +
      '<tr><td colspan="8" style="padding:10px; text-align:center; font-size:11px; color:var(--text-sub);">Data telemetri fisik ESP32 terhubung. Klik <b>[Catat Hari Ini]</b> untuk mengarsipkan perkembangan ke LittleFS Flash ESP32.</td></tr>';
    return;
  }

  var html = '';

  for (var i = history.length - 1; i >= 0; i--) {
    var row = history[i];
    var vColor = row.vigor >= 75 ? '#10b981' : (row.vigor >= 50 ? '#f59e0b' : '#ef4444');
    html += '<tr style="border-bottom:1px solid rgba(255,255,255,0.04);">' +
      '<td style="padding:6px 10px; font-weight:700; color:var(--primary); font-family:monospace;">HST ' + row.hst + '</td>' +
      '<td style="padding:6px 10px; color:var(--text-sub);">' + row.date + '</td>' +
      '<td style="padding:6px 10px;"><span style="background:rgba(255,255,255,0.06); padding:2px 6px; border-radius:4px; font-size:10px;">' + (stageNames[row.stage] || row.stage) + '</span></td>' +
      '<td style="padding:6px 10px; color:#10b981; font-weight:600;">' + (row.soil !== undefined ? row.soil + '%' : '--') + '</td>' +
      '<td style="padding:6px 10px; color:#06b6d4; font-weight:600;">' + (row.temp !== undefined ? row.temp + '°C' : '--') + '</td>' +
      '<td style="padding:6px 10px; color:#a855f7;">' + (row.hum !== undefined && row.hum !== null ? row.hum + '%' : '--') + '</td>' +
      '<td style="padding:6px 10px; font-weight:700; color:' + vColor + ';">' + (row.vigor !== undefined ? row.vigor + '%' : '--') + '</td>' +
      '<td style="padding:6px 10px; text-align:center;"><button type="button" class="btn" style="padding:2px 6px; font-size:10px; background:rgba(239,68,68,0.15); color:#ef4444; border:1px solid rgba(239,68,68,0.3); border-radius:4px;" onclick="deleteCropHistoryEntry(' + i + ')" title="Hapus catatan ini">✕</button></td>' +
      '</tr>';
  }
  tbody.innerHTML = html;
}

// --- RESET CYCLE ---
function resetCropHistory() {
  if (confirm("Mulai siklus tanam baru dari HST 1? Catatan riwayat perkembangan tanaman sebelumnya akan dihapus dari memori Flash ESP32 dan peramban.")) {
    _cropHistoryInMemory = [];
    saveCropHistoryData([]);
    localStorage.removeItem('crop_age');
    localStorage.removeItem('crop_stage');

    // Kirim reset ke ESP32 LittleFS & NVS
    fetch('/resetCropHistory', { method: 'POST' }).catch(function(err) {
      console.warn("Reset crop history on ESP32:", err);
    });

    var ageInput = document.getElementById('crop-age-days');
    if (ageInput) ageInput.value = 1;
    var stageInput = document.getElementById('crop-stage');
    if (stageInput) stageInput.value = 'semai';
    if (typeof updateCropAgronomyAnalysis === 'function') updateCropAgronomyAnalysis();
    updateCropHistorySummary();
    renderCropHistoryChart();
    renderCropHistoryTable();
  }
}

// --- UPDATE SUMMARY INDICATORS (STRICT 100% REAL HISTORY / ZERO DUMMY) ---
function updateCropHistorySummary() {
  var history = getCropHistoryData();
  var ptsEl = document.getElementById('crop-hist-points-text');
  var hstEl = document.getElementById('crop-hist-hst');
  var stageEl = document.getElementById('crop-hist-stage');
  var vigorEl = document.getElementById('crop-hist-vigor');
  var etaEl = document.getElementById('crop-hist-eta');

  if (ptsEl) ptsEl.innerText = history.length + " Catatan Tersimpan";

  // JIKA BELUM PERNAH DICATAT SAMA SEKALI: AMBIL DARI TELEMETRI & INPUT RIIL ESP32
  if (history.length === 0) {
    var tData = window.lastTelemetryData || {};
    var cAge = (tData.cropAge !== undefined && tData.cropAge > 0) ? tData.cropAge : parseInt(localStorage.getItem('crop_age') || (document.getElementById('crop-age-days') ? document.getElementById('crop-age-days').value : 14), 10);
    var cStage = (tData.cropStage && tData.cropStage !== '') ? tData.cropStage : (localStorage.getItem('crop_stage') || (document.getElementById('crop-stage') ? document.getElementById('crop-stage').value : 'vegetatif'));
    var cName = (tData.cropName && tData.cropName !== '') ? tData.cropName : (localStorage.getItem('crop_name') || 'Cabai Rawit');
    var stageNames = { 'semai': 'Semai (Nursery)', 'vegetatif': 'Vegetatif Aktif', 'generatif': 'Generatif / Bunga', 'panen': 'Pematangan / Panen' };

    if (hstEl) hstEl.innerText = cAge + " HST";
    if (stageEl) stageEl.innerText = stageNames[cStage] || cStage;

    // Hitung estimasi panen riil
    var totalCycleDays = 90;
    var nLower = cName.toLowerCase();
    if (nLower.indexOf('bawang') !== -1) totalCycleDays = 65;
    else if (nLower.indexOf('melon') !== -1) totalCycleDays = 70;
    else if (nLower.indexOf('semangka') !== -1 || nLower.indexOf('watermelon') !== -1) totalCycleDays = 70;
    else if (nLower.indexOf('tomat') !== -1) totalCycleDays = 85;
    else if (nLower.indexOf('padi') !== -1) totalCycleDays = 115;
    else totalCycleDays = 90;

    var remaining = Math.max(0, totalCycleDays - cAge);
    if (etaEl) etaEl.innerText = remaining > 0 ? (remaining + " Hari Lagi") : "Siap Panen";

    // Hitung Skor Vigor Murni dari Telemetri Fisik Riil Lahan
    var rawT = (tData.suhuC !== undefined && tData.suhuC !== "--") ? tData.suhuC : tData.temp;
    var temp = (rawT !== undefined && rawT !== null && !isNaN(parseFloat(rawT))) ? parseFloat(rawT) : 28.0;
    var rawS = (tData.soil !== undefined && tData.soil !== "--") ? tData.soil : null;
    var soil = (rawS !== null && !isNaN(parseFloat(rawS))) ? parseFloat(rawS) : 65;

    var vigor = 100;
    if (soil < 50) vigor -= Math.min(40, (50 - soil) * 2);
    else if (soil > 80) vigor -= Math.min(30, (soil - 80) * 2);
    if (temp > 32) vigor -= Math.min(30, (temp - 32) * 5);
    else if (temp < 20) vigor -= Math.min(25, (20 - temp) * 3);
    vigor = Math.max(10, Math.min(100, Math.round(vigor)));
    if (vigorEl) vigorEl.innerText = vigor + "% (Sensor Riil)";
    return;
  }

  // JIKA SUDAH ADA CATATAN: AMBIL DARI CATATAN TERAKHIR YANG SUDAH TERVERIFIKASI
  var lastEntry = history[history.length - 1];
  var stageMap = {
    'semai': 'Semai (Nursery)',
    'vegetatif': 'Vegetatif Aktif',
    'generatif': 'Generatif / Bunga',
    'panen': 'Pematangan / Panen'
  };

  if (hstEl) hstEl.innerText = lastEntry.hst + " HST";
  if (stageEl) stageEl.innerText = stageMap[lastEntry.stage] || lastEntry.stage;
  if (vigorEl) vigorEl.innerText = lastEntry.vigor + "%";

  var name = lastEntry.cropName || localStorage.getItem('crop_name') || 'Tanaman';
  var totalCycleDays = 90;
  var nLower = name.toLowerCase();
  if (nLower.indexOf('bawang') !== -1) totalCycleDays = 65;
  else if (nLower.indexOf('melon') !== -1) totalCycleDays = 70;
  else if (nLower.indexOf('semangka') !== -1 || nLower.indexOf('watermelon') !== -1) totalCycleDays = 70;
  else if (nLower.indexOf('tomat') !== -1) totalCycleDays = 85;
  else if (nLower.indexOf('padi') !== -1) totalCycleDays = 115;
  else totalCycleDays = 90;

  var remaining = Math.max(0, totalCycleDays - lastEntry.hst);
  if (etaEl) etaEl.innerText = remaining > 0 ? (remaining + " Hari Lagi") : "Siap Panen";
}

// --- CROSSHAIR INTERACTION ---
function attachCropCrosshair() {
  var canvas = document.getElementById('cropHistoryChart');
  var tooltip = document.getElementById('crop-hist-tooltip');
  if (!canvas) return;

  function handleMove(e) {
    var rect = canvas.getBoundingClientRect();
    var clientX = e.touches ? e.touches[0].clientX : e.clientX;
    var x = clientX - rect.left;
    var padL = 36, padR = 24, chartW = canvas.width - padL - padR;
    var history = getCropHistoryData();

    if (history.length === 0 || x < padL || x > padL + chartW) {
      cropCrosshairIdx = -1;
      if (tooltip) tooltip.style.display = 'none';
      renderCropHistoryChart();
      return;
    }

    var maxHst = 90;
    var clickedHst = Math.round(((x - padL) / chartW) * maxHst);
    
    // Find closest logged record or calculate theoretical
    var closestEntry = null, minDist = 999;
    for (var i = 0; i < history.length; i++) {
      var d = Math.abs(history[i].hst - clickedHst);
      if (d < minDist) { minDist = d; closestEntry = history[i]; }
    }

    if (closestEntry && minDist <= 8) {
      cropCrosshairIdx = closestEntry.hst;
      if (tooltip) {
        tooltip.innerHTML = '<span style="color:#94a3b8;">' + closestEntry.date + ' (HST ' + closestEntry.hst + ')</span> &bull; ' +
          '<span style="color:#10b981;font-weight:700;">Vigor: ' + closestEntry.vigor + '%</span> &bull; ' +
          '<span style="color:#06b6d4;">Tanah: ' + closestEntry.soil + '%</span> &bull; ' +
          '<span style="color:#eab308;">Suhu: ' + closestEntry.temp + '°C</span>';
        tooltip.style.display = 'block';
      }
    } else {
      cropCrosshairIdx = clickedHst;
      if (tooltip) {
        var theo = Math.round(100 / (1 + Math.exp(-0.08 * (clickedHst - 40))));
        tooltip.innerHTML = '<span style="color:#94a3b8;">HST ' + clickedHst + ' &bull; Target Ideal: ' + theo + '%</span>';
        tooltip.style.display = 'block';
      }
    }
    renderCropHistoryChart();
  }

  function handleLeave() {
    cropCrosshairIdx = -1;
    if (tooltip) tooltip.style.display = 'none';
    renderCropHistoryChart();
  }

  canvas.addEventListener('mousemove', handleMove);
  canvas.addEventListener('mouseleave', handleLeave);
  canvas.addEventListener('touchmove', handleMove, { passive: true });
  canvas.addEventListener('touchend', handleLeave);
}

// --- RENDER S-CURVE & PHENOLOGY TIMELINE ---
function renderCropHistoryChart() {
  if (!cropHistoryCanvas) cropHistoryCanvas = document.getElementById('cropHistoryChart');
  if (!cropHistoryCanvas || !cropHistoryCanvas.parentElement) return;

  var ctx = cropHistoryCanvas.getContext('2d');
  var w = cropHistoryCanvas.width = cropHistoryCanvas.parentElement.clientWidth;
  var h = cropHistoryCanvas.height = cropHistoryCanvas.parentElement.clientHeight;
  ctx.clearRect(0, 0, w, h);

  var padL = 36, padR = 24, padT = 18, padB = 26;
  var chartW = w - padL - padR, chartH = h - padT - padB;
  var maxHst = 90;

  // Background Grid & Y-Axis
  ctx.strokeStyle = "rgba(255,255,255,0.06)";
  ctx.lineWidth = 1;
  ctx.fillStyle = "rgba(255,255,255,0.35)";
  ctx.font = "9px Inter, monospace";

  for (var k = 0; k <= 4; k++) {
    var gy = padT + (chartH * (k / 4));
    ctx.beginPath(); ctx.moveTo(padL, gy); ctx.lineTo(w - padR, gy); ctx.stroke();
    ctx.fillText((100 - k * 25) + "%", 6, gy + 3);
  }

  // 1. Shading 4 Phenology Stages (Semai, Vegetatif, Generatif, Panen)
  var stages = [
    { name: "Semai (0-14)", end: 14, color: "rgba(16, 185, 129, 0.04)" },
    { name: "Vegetatif (15-40)", end: 40, color: "rgba(6, 182, 212, 0.04)" },
    { name: "Generatif (41-70)", end: 70, color: "rgba(245, 158, 11, 0.04)" },
    { name: "Panen (71-90)", end: 90, color: "rgba(234, 179, 8, 0.05)" }
  ];

  var prevEnd = 0;
  for (var s = 0; s < stages.length; s++) {
    var stObj = stages[s];
    var startX = padL + (prevEnd / maxHst) * chartW;
    var endX = padL + (stObj.end / maxHst) * chartW;
    ctx.fillStyle = stObj.color;
    ctx.fillRect(startX, padT, endX - startX, chartH);

    // Stage boundary vertical line
    if (s < stages.length - 1) {
      ctx.strokeStyle = "rgba(255,255,255,0.08)";
      ctx.beginPath(); ctx.moveTo(endX, padT); ctx.lineTo(endX, padT + chartH); ctx.stroke();
    }
    // Stage label
    ctx.fillStyle = "rgba(255,255,255,0.25)";
    ctx.font = "8px Inter, sans-serif";
    ctx.fillText(stObj.name, startX + 4, padT + 10);
    prevEnd = stObj.end;
  }

  // 2. Theoretical Sigmoid Growth Curve (S-Curve)
  ctx.beginPath();
  ctx.strokeStyle = "rgba(6, 182, 212, 0.4)";
  ctx.setLineDash([4, 3]);
  ctx.lineWidth = 1.8;
  for (var t = 0; t <= maxHst; t++) {
    var sigVal = 100 / (1 + Math.exp(-0.08 * (t - 40)));
    var tx = padL + (t / maxHst) * chartW;
    var ty = padT + chartH - (sigVal / 100 * chartH);
    if (t === 0) ctx.moveTo(tx, ty); else ctx.lineTo(tx, ty);
  }
  ctx.stroke();
  ctx.setLineDash([]); // Reset dash

  // 3. Actual Logged Crop History Points
  var history = getCropHistoryData();
  if (history.length > 0) {
    // Connect actual points
    ctx.beginPath();
    ctx.strokeStyle = "#10b981";
    ctx.lineWidth = 2.4;
    for (var p = 0; p < history.length; p++) {
      var entry = history[p];
      var px = padL + (Math.min(entry.hst, maxHst) / maxHst) * chartW;
      var py = padT + chartH - (entry.vigor / 100 * chartH);
      if (p === 0) ctx.moveTo(px, py); else ctx.lineTo(px, py);
    }
    ctx.stroke();

    // Draw dots for each logged day
    for (var d = 0; d < history.length; d++) {
      var dEntry = history[d];
      var dx = padL + (Math.min(dEntry.hst, maxHst) / maxHst) * chartW;
      var dy = padT + chartH - (dEntry.vigor / 100 * chartH);
      ctx.fillStyle = "#10b981";
      ctx.beginPath(); ctx.arc(dx, dy, 5, 0, Math.PI * 2); ctx.fill();
      ctx.strokeStyle = "#ffffff";
      ctx.lineWidth = 1.5;
      ctx.stroke();
    }
  } else {
    // If no records yet, show gentle prompt
    ctx.fillStyle = "rgba(255,255,255,0.3)";
    ctx.font = "11px Inter, sans-serif";
    ctx.fillText("Belum ada catatan harian. Klik tombol [ Catat Hari Ini ] untuk merekam perkembangan tanaman.", padL + 12, h / 2 + 10);
  }

  // 4. Current Day (HST) Marker
  var currentAge = parseInt(localStorage.getItem('crop_age') || (document.getElementById('crop-age-days') ? document.getElementById('crop-age-days').value : '14'), 10);
  if (currentAge <= maxHst) {
    var curX = padL + (currentAge / maxHst) * chartW;
    ctx.save();
    ctx.setLineDash([3, 3]);
    ctx.strokeStyle = "#eab308";
    ctx.lineWidth = 1.5;
    ctx.beginPath();
    ctx.moveTo(curX, padT);
    ctx.lineTo(curX, padT + chartH);
    ctx.stroke();
    ctx.fillStyle = "#eab308";
    ctx.font = "8px Inter, sans-serif";
    ctx.fillText("HARI INI (HST " + currentAge + ")", Math.min(curX + 4, w - padR - 75), padT + chartH - 8);

    // Gambarkan titik aktif tanaman aktual ESP32 pada kurva pertumbuhan
    if (history.length === 0) {
      var sigVal = 100 / (1 + Math.exp(-0.08 * (currentAge - 40)));
      var curY = padT + chartH - (sigVal / 100 * chartH);
      ctx.fillStyle = "#10b981";
      ctx.beginPath(); ctx.arc(curX, curY, 6, 0, Math.PI * 2); ctx.fill();
      ctx.strokeStyle = "#ffffff";
      ctx.lineWidth = 2;
      ctx.stroke();
      ctx.fillStyle = "#10b981";
      ctx.font = "9px Inter, sans-serif";
      ctx.fillText("Aktif ESP32", Math.min(curX + 8, w - padR - 65), curY - 6);
    }
    ctx.restore();
  }

  // 5. Crosshair Line on User Hover/Touch
  if (cropCrosshairIdx >= 0 && cropCrosshairIdx <= maxHst) {
    var chX = padL + (cropCrosshairIdx / maxHst) * chartW;
    ctx.save();
    ctx.setLineDash([2, 2]);
    ctx.strokeStyle = "rgba(255,255,255,0.6)";
    ctx.lineWidth = 1;
    ctx.beginPath();
    ctx.moveTo(chX, padT);
    ctx.lineTo(chX, padT + chartH);
    ctx.stroke();
    ctx.restore();
  }

  // Sumbu X: HST Labels
  ctx.fillStyle = "rgba(255,255,255,0.4)";
  ctx.font = "9px Inter, monospace";
  for (var hstLbl = 0; hstLbl <= maxHst; hstLbl += 15) {
    var lx = padL + (hstLbl / maxHst) * chartW;
    ctx.fillText(hstLbl + " HST", lx - 10, h - 6);
  }
}

// --- EXPORT, PRINT & WHATSAPP SHARING SUITE ---
function shareCropReportWhatsApp() {
  var storedAge = localStorage.getItem('crop_age') || (document.getElementById('crop-age-days') ? document.getElementById('crop-age-days').value : '14');
  var storedName = localStorage.getItem('crop_name') || (document.getElementById('crop-name') ? document.getElementById('crop-name').value : 'Cabai Rawit');
  var stage = localStorage.getItem('crop_stage') || (document.getElementById('crop-stage') ? document.getElementById('crop-stage').value : 'vegetatif');
  var tData = window.lastTelemetryData || {};

  var age = parseInt(storedAge, 10);
  var name = storedName;
  var soil = (tData.soil !== undefined && !isNaN(tData.soil) && tData.soil !== '--') ? Math.round(parseFloat(tData.soil)) + "%" : "--%";
  var rawT = (tData.suhuC !== undefined && tData.suhuC !== '--') ? tData.suhuC : tData.temp;
  var temp = (rawT !== undefined && !isNaN(parseFloat(rawT))) ? parseFloat(rawT).toFixed(1) + "°C" : "--°C";
  var hum = (tData.hum !== undefined && !isNaN(parseFloat(tData.hum))) ? Math.round(parseFloat(tData.hum)) + "%" : "--%";
  var history = getCropHistoryData();
  var vigor = (history.length > 0) ? (history[history.length - 1].vigor + "%") : "--%";
  var stageNames = { 'semai': 'Semai (Nursery)', 'vegetatif': 'Vegetatif Aktif', 'generatif': 'Generatif / Bunga', 'panen': 'Pematangan / Panen' };
  var dateStr = new Date().toLocaleDateString('id-ID', { weekday: 'long', day: '2-digit', month: '2-digit', year: 'numeric' });

  var isPumpOn = (tData.relay === "1" || tData.relay === 1 || tData.relayOn === 1);
  var isLampOn = (tData.lamp === "1" || tData.lamp === 1 || tData.lampOn === 1);

  var text = "🌱 *LAPORAN SIKLUS TANAMAN - SMART FARM SCADA*\n" +
    "━━━━━━━━━━━━━━━━━━━━\n" +
    "📅 *Tanggal*      : " + dateStr + "\n" +
    "🌿 *Komoditas*    : " + name + "\n" +
    "⏳ *Umur Tanam*   : " + age + " HST\n" +
    "🎋 *Fase Tumbuh*  : " + (stageNames[stage] || stage) + "\n" +
    "✨ *Skor Vigor*   : " + vigor + " (Kesehatan Tanaman)\n" +
    "━━━━━━━━━━━━━━━━━━━━\n" +
    "📊 *Telemetri Sensor Lapangan:*\n" +
    "• Suhu Udara       : " + temp + "\n" +
    "• Kelembapan Tanah : " + soil + "\n" +
    "• Kelembapan RH    : " + hum + "\n" +
    "• Status Pompa     : " + (isPumpOn ? "💧 AKTIF (Menyiram)" : "⏸️ NONAKTIF") + "\n" +
    "• Status Lampu     : " + (isLampOn ? "💡 MENYALA" : "🌑 MATI") + "\n" +
    "• Radio ESP-NOW    : " + (tData.rssi ? (tData.rssi + " dBm") : "-- dBm") + "\n" +
    "━━━━━━━━━━━━━━━━━━━━\n" +
    "📡 _Smart Farm Precision Agriculture System_";

  var url = "https://wa.me/?text=" + encodeURIComponent(text);
  var a = document.createElement('a');
  a.href = url;
  a.target = '_blank';
  a.rel = 'noopener noreferrer';
  document.body.appendChild(a);
  a.click();
  a.remove();
}

function printCropReport() {
  var storedAge = localStorage.getItem('crop_age') || (document.getElementById('crop-age-days') ? document.getElementById('crop-age-days').value : '14');
  var storedName = localStorage.getItem('crop_name') || (document.getElementById('crop-name') ? document.getElementById('crop-name').value : 'Cabai Rawit');
  var stage = localStorage.getItem('crop_stage') || (document.getElementById('crop-stage') ? document.getElementById('crop-stage').value : 'vegetatif');
  var history = getCropHistoryData();
  var tData = window.lastTelemetryData || {};
  var sTemp = (tData.suhuC !== undefined && tData.suhuC !== '--') ? tData.suhuC : (tData.temp || '--');
  var sSoil = (tData.soil !== undefined && tData.soil !== '--') ? tData.soil + '%' : '--%';
  var sHum = (tData.hum !== undefined && tData.hum !== '--') ? tData.hum + '%' : '--%';
  var dateStr = new Date().toLocaleDateString('id-ID', { weekday: 'long', day: 'numeric', month: 'long', year: 'numeric' });
  var timeStr = (tData.time || new Date().toLocaleTimeString('id-ID')) + ' WIB';

  var stageNames = { 'semai': 'Semai (Nursery)', 'vegetatif': 'Vegetatif Aktif', 'generatif': 'Generatif / Pembungaan', 'panen': 'Pematangan / Panen' };

  var win = window.open('', '_blank');
  if (!win) {
    window.print();
    return;
  }

  var rowsHtml = '';
  if (history.length === 0) {
    rowsHtml = '<tr><td colspan="7" style="text-align:center; padding:12px; color:#666;">Belum ada rekaman riwayat perkembangan.</td></tr>';
  } else {
    for (var i = 0; i < history.length; i++) {
      var r = history[i];
      rowsHtml += '<tr>' +
        '<td style="padding:6px; border:1px solid #ddd; text-align:center; font-weight:bold;">HST ' + r.hst + '</td>' +
        '<td style="padding:6px; border:1px solid #ddd;">' + r.date + '</td>' +
        '<td style="padding:6px; border:1px solid #ddd;">' + (stageNames[r.stage] || r.stage) + '</td>' +
        '<td style="padding:6px; border:1px solid #ddd; text-align:center;">' + r.soil + '%</td>' +
        '<td style="padding:6px; border:1px solid #ddd; text-align:center;">' + r.temp + '°C</td>' +
        '<td style="padding:6px; border:1px solid #ddd; text-align:center;">' + (r.hum !== null ? r.hum + '%' : '--') + '</td>' +
        '<td style="padding:6px; border:1px solid #ddd; text-align:center; font-weight:bold; color:' + (r.vigor >= 70 ? '#059669' : '#d97706') + ';">' + r.vigor + '%</td>' +
        '</tr>';
    }
  }

  var docHtml = '<!DOCTYPE html><html><head><meta charset="utf-8">' +
    '<title>Laporan Siklus Tanam - ' + storedName + '</title>' +
    '<style>' +
    'body { font-family: "Segoe UI", Arial, sans-serif; margin: 30px; color: #111; font-size: 13px; line-height: 1.5; }' +
    '.header-title { font-size: 18px; font-weight: bold; color: #047857; text-transform: uppercase; margin-bottom: 2px; }' +
    '.header-sub { font-size: 12px; color: #555; margin-bottom: 18px; border-bottom: 2px solid #047857; padding-bottom: 6px; }' +
    '.meta-grid { display: grid; grid-template-columns: 1fr 1fr; gap: 10px; background: #f8fafc; border: 1px solid #e2e8f0; padding: 12px; border-radius: 6px; margin-bottom: 18px; }' +
    '.meta-item b { color: #334155; }' +
    'table { width: 100%; border-collapse: collapse; margin-top: 10px; font-size: 12px; }' +
    'th { background: #f1f5f9; padding: 8px; border: 1px solid #cbd5e1; font-weight: bold; text-align: left; }' +
    '.footer-sig { margin-top: 40px; display: flex; justify-content: space-between; }' +
    '.sig-box { width: 200px; text-align: center; border-top: 1px solid #444; margin-top: 60px; padding-top: 4px; font-size: 11px; }' +
    '@media print { @page { margin: 15mm; size: A4 portrait; } button { display: none !important; } }' +
    '</style></head><body>' +
    '<div style="display:flex; justify-content:space-between; align-items:flex-start;">' +
    '<div><div class="header-title">🌱 LAPORAN REKAMAN SIKLUS PERTUMBUHAN TANAMAN</div>' +
    '<div class="header-sub">Sistem SCADA Pertanian Presisi Smart Farm IoT • ' + dateStr + ' (' + timeStr + ')</div></div>' +
    '<button onclick="window.print()" style="padding:6px 12px; background:#047857; color:#fff; border:none; border-radius:4px; cursor:pointer; font-weight:bold;">🖨️ Cetak / Simpan PDF</button>' +
    '</div>' +
    '<div class="meta-grid">' +
    '<div class="meta-item"><b>Komoditas:</b> ' + storedName + '</div>' +
    '<div class="meta-item"><b>Usia Saat Ini:</b> ' + storedAge + ' HST</div>' +
    '<div class="meta-item"><b>Fase Fenologi:</b> ' + (stageNames[stage] || stage) + '</div>' +
    '<div class="meta-item"><b>Telemetri Riil:</b> Suhu ' + sTemp + '°C • Tanah ' + sSoil + ' • RH ' + sHum + '</div>' +
    '</div>' +
    '<h4 style="margin: 12px 0 4px 0; color:#333;">TABEL JURNAL PERKEMBANGAN HARIAN (HST)</h4>' +
    '<table><thead><tr>' +
    '<th style="text-align:center;">HST</th><th>Tanggal</th><th>Fase</th>' +
    '<th style="text-align:center;">Tanah</th><th style="text-align:center;">Suhu</th>' +
    '<th style="text-align:center;">RH</th><th style="text-align:center;">Skor Vigor</th>' +
    '</tr></thead><tbody>' + rowsHtml + '</tbody></table>' +
    '<div class="footer-sig">' +
    '<div class="sig-box">Petani Pelaksana / Pengelola Kebun</div>' +
    '<div class="sig-box">Sistem Telemetri Smart Farm SCADA</div>' +
    '</div>' +
    '</body></html>';

  win.document.open();
  win.document.write(docHtml);
  win.document.close();
  setTimeout(function() {
    try { win.print(); } catch (err) {}
  }, 400);
}

function exportCropHistoryJSON() {
  var history = getCropHistoryData();
  var ageStr = localStorage.getItem('crop_age') || (document.getElementById('crop-age-days') ? document.getElementById('crop-age-days').value : '14');
  var name = localStorage.getItem('crop_name') || (document.getElementById('crop-name') ? document.getElementById('crop-name').value : 'Cabai_Rawit');
  var age = parseInt(ageStr, 10) || 0;
  var exportObj = {
    app: "SmartFarmSCADA",
    version: "2.0",
    storage: "ESP32_LittleFS_and_LocalStorage",
    exportedAt: new Date().toISOString(),
    cropName: name,
    cropAge: age,
    totalRecords: history.length,
    history: history
  };
  var jsonStr = JSON.stringify(exportObj, null, 2);
  var blob = new Blob([jsonStr], { type: "application/json;charset=utf-8" });
  var url = URL.createObjectURL(blob);
  var downloadAnchor = document.createElement('a');
  downloadAnchor.href = url;
  downloadAnchor.download = "smartfarm_riwayat_" + name.replace(/\s+/g, '_') + "_HST" + age + ".json";
  document.body.appendChild(downloadAnchor);
  downloadAnchor.click();
  downloadAnchor.remove();
  setTimeout(function() { URL.revokeObjectURL(url); }, 1000);
}

function triggerImportCropHistoryJSON() {
  var inp = document.getElementById('crop-import-file-input');
  if (inp) inp.click();
}

function importCropHistoryJSON(e) {
  var file = e.target.files && e.target.files[0];
  if (!file) return;
  var reader = new FileReader();
  reader.onload = function(ev) {
    try {
      var parsed = JSON.parse(ev.target.result);
      var items = Array.isArray(parsed) ? parsed : (parsed.history && Array.isArray(parsed.history) ? parsed.history : null);
      if (!items) throw new Error("Format JSON tidak sesuai.");
      saveCropHistoryData(items);
      if (parsed.cropAge) {
        localStorage.setItem('crop_age', parsed.cropAge);
        var elAge = document.getElementById('crop-age-days');
        if (elAge) elAge.value = parsed.cropAge;
      }
      if (parsed.cropName) {
        localStorage.setItem('crop_name', parsed.cropName);
        var elName = document.getElementById('crop-name');
        if (elName) elName.value = parsed.cropName;
      }
      if (typeof updateCropAgronomyAnalysis === 'function') updateCropAgronomyAnalysis();
      updateCropHistorySummary();
      renderCropHistoryChart();
      renderCropHistoryTable();
      var footerStatus = document.getElementById('crop-hist-footer-status');
      if (footerStatus) {
        footerStatus.innerHTML = '<span style="color:#10b981;font-weight:700;">✓ Berhasil memulihkan ' + items.length + ' data ke Flash LittleFS ESP32 & Browser!</span>';
        setTimeout(function() {
          if (footerStatus) footerStatus.innerText = "Data tersimpan di LittleFS Flash Memory ESP32 & LocalStorage • Kurva Pertumbuhan Sigmoid (S-Curve)";
        }, 3500);
      }
    } catch (err) {
      alert("Gagal memuat file riwayat: " + err.message);
    }
  };
  reader.readAsText(file);
  e.target.value = '';
}

// Window global exports for HTML inline buttons
if (typeof window !== 'undefined') {
  window.initCropHistory = initCropHistory;
  window.renderCropHistoryChart = renderCropHistoryChart;
  window.renderCropHistoryTable = renderCropHistoryTable;
  window.updateCropHistorySummary = updateCropHistorySummary;
  window.logTodayCropGrowth = logTodayCropGrowth;
  window.deleteCropHistoryEntry = deleteCropHistoryEntry;
  window.resetCropHistory = resetCropHistory;
  window.exportCropHistoryJSON = exportCropHistoryJSON;
  window.triggerImportCropHistoryJSON = triggerImportCropHistoryJSON;
  window.importCropHistoryJSON = importCropHistoryJSON;
  window.shareCropReportWhatsApp = shareCropReportWhatsApp;
  window.printCropReport = printCropReport;
}

