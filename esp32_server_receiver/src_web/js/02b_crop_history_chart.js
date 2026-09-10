// =================================================================
// 02B_CROP_HISTORY_CHART.JS - CROP LIFECYCLE S-CURVE & LOCAL STORAGE
// Tracks day-by-day crop growth (HST), phenology milestones & vigor
// Stored persistently in browser LocalStorage
// =================================================================

var cropHistoryCanvas = null;
var cropCrosshairIdx = -1;

function getCropHistoryData() {
  try {
    var raw = localStorage.getItem('smartfarm_crop_history');
    if (raw) return JSON.parse(raw);
  } catch (e) {
    console.error("Error reading crop history:", e);
  }
  return [];
}

function saveCropHistoryData(data) {
  try {
    localStorage.setItem('smartfarm_crop_history', JSON.stringify(data));
  } catch (e) {
    console.error("Error saving crop history:", e);
  }
}

function initCropHistory() {
  cropHistoryCanvas = document.getElementById('cropHistoryChart');
  if (cropHistoryCanvas) {
    attachCropCrosshair();
    resizeCropCanvas();
  }
  updateCropHistorySummary();
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
  var tData = window.lastTelemetryData;
  if (!tData || tData.soil === undefined || tData.temp === undefined || isNaN(parseFloat(tData.soil)) || isNaN(parseFloat(tData.temp))) {
    alert("Gagal mencatat: Data sensor fisik ESP32 belum diterima. Pastikan perangkat aktif.");
    return;
  }

  var storedAge = localStorage.getItem('crop_age');
  var storedName = localStorage.getItem('crop_name');
  var storedStage = localStorage.getItem('crop_stage');

  if (!storedAge || !storedName) {
    alert("Silakan tentukan komoditas dan umur tanaman terlebih dahulu di tab Kontrol & Agronomi.");
    return;
  }

  var age = parseInt(storedAge, 10);
  var name = storedName;
  var stage = storedStage || 'semai';
  var leaves = parseInt(localStorage.getItem('crop_leaves') || '4', 10);
  
  var soil = Math.round(parseFloat(tData.soil));
  var temp = parseFloat(tData.temp);
  var hum = (tData.hum !== undefined && !isNaN(parseFloat(tData.hum))) ? Math.round(parseFloat(tData.hum)) : null;

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
    temp: temp,
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

  var footerStatus = document.getElementById('crop-hist-footer-status');
  if (footerStatus) {
    footerStatus.innerHTML = '<span style="color:#10b981;">✓ Data riil sensor HST ' + age + ' (Vigor: ' + vigor + '%, Tanah: ' + soil + '%) berhasil disimpan!</span>';
    setTimeout(function() {
      if (footerStatus) footerStatus.innerText = "Data tersimpan di LocalStorage peramban • Kurva Pertumbuhan Sigmoid (S-Curve)";
    }, 3500);
  }
}

// --- RESET CYCLE ---
function resetCropHistory() {
  if (confirm("Mulai siklus tanam baru dari HST 1? Catatan riwayat perkembangan tanaman sebelumnya akan dihapus.")) {
    saveCropHistoryData([]);
    localStorage.removeItem('crop_age');
    localStorage.removeItem('crop_stage');
    var ageInput = document.getElementById('crop-age-days');
    if (ageInput) ageInput.value = 1;
    var stageInput = document.getElementById('crop-stage');
    if (stageInput) stageInput.value = 'semai';
    if (typeof updateCropAgronomyAnalysis === 'function') updateCropAgronomyAnalysis();
    updateCropHistorySummary();
    renderCropHistoryChart();
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

  // JIKA BELUM PERNAH DICATAT SAMA SEKALI: WAJIB TAMPILKAN STATUS KOSONG (--)
  if (history.length === 0) {
    if (hstEl) hstEl.innerText = "-- HST";
    if (stageEl) stageEl.innerText = "--";
    if (vigorEl) vigorEl.innerText = "--%";
    if (etaEl) etaEl.innerText = "-- Hari Lagi";
    return;
  }

  // JIKA SUDAH ADA CATATAN: AMBIL DARI CATATAN TERAKHIR YANG SUDAH TERVERIFIKASI
  var lastEntry = history[history.length - 1];
  var stageNames = {
    'semai': 'Semai (Nursery)',
    'vegetatif': 'Vegetatif Aktif',
    'generatif': 'Generatif / Bunga',
    'panen': 'Pematangan / Panen'
  };

  if (hstEl) hstEl.innerText = lastEntry.hst + " HST";
  if (stageEl) stageEl.innerText = stageNames[lastEntry.stage] || lastEntry.stage;
  if (vigorEl) vigorEl.innerText = lastEntry.vigor + "%";

  var name = lastEntry.cropName || localStorage.getItem('crop_name') || 'Tanaman';
  var totalCycleDays = 90;
  var nLower = name.toLowerCase();
  if (nLower.indexOf('bawang') !== -1) totalCycleDays = 65;
  else if (nLower.indexOf('melon') !== -1) totalCycleDays = 70;
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
      ctx.beginPath(); ctx.arc(dx, dy, 4.5, 0, Math.PI * 2); ctx.fill();
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
  var currentAge = parseInt(localStorage.getItem('crop_age') || '14', 10);
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
  var storedAge = localStorage.getItem('crop_age');
  var storedName = localStorage.getItem('crop_name');
  var stage = localStorage.getItem('crop_stage') || 'semai';
  var tData = window.lastTelemetryData || {};

  if (!storedName || !storedAge) {
    alert("Perhatian: Varietas dan usia tanaman belum dikonfigurasi. Silakan isi formulir tanaman di tab Kontrol sebelum membagikan laporan.");
    return;
  }

  var age = parseInt(storedAge, 10);
  var name = storedName;
  var soil = (tData.soil !== undefined && !isNaN(tData.soil)) ? Math.round(tData.soil) + "%" : "--%";
  var temp = (tData.temp !== undefined && !isNaN(tData.temp)) ? parseFloat(tData.temp).toFixed(1) + "°C" : "--°C";
  var hum = (tData.hum !== undefined && !isNaN(tData.hum)) ? Math.round(tData.hum) + "%" : "--%";
  var history = getCropHistoryData();
  var vigor = (history.length > 0) ? (history[history.length - 1].vigor + "%") : "--%";
  var stageNames = { 'semai': 'Semai (Nursery)', 'vegetatif': 'Vegetatif Aktif', 'generatif': 'Generatif / Bunga', 'panen': 'Pematangan / Panen' };
  var dateStr = new Date().toLocaleDateString('id-ID', { day: '2-digit', month: '2-digit', year: 'numeric' });

  var text = "[LAPORAN SIKLUS TANAMAN - SMART FARM SCADA]\n" +
    "------------------------------------\n" +
    "Tanggal    : " + dateStr + "\n" +
    "Umur Tanam : " + age + " HST\n" +
    "Komoditas  : " + name + "\n" +
    "Fase       : " + (stageNames[stage] || stage) + "\n" +
    "Skor Vigor : " + vigor + "\n\n" +
    "Telemetri Sensor Lapangan:\n" +
    "• Suhu Udara       : " + temp + "\n" +
    "• Kelembapan Tanah : " + soil + "\n" +
    "• Kelembapan RH    : " + hum + "\n" +
    "• Status Pompa     : " + ((tData.relay === "1" || tData.relay === 1) ? "Aktif Menyiram" : "Mati") + "\n" +
    "• Radio ESP-NOW    : " + (tData.rssi ? (tData.rssi + " dBm") : "-- dBm") + "\n" +
    "------------------------------------\n" +
    "Smart Farm Precision Agriculture System";

  var url = "https://api.whatsapp.com/send?text=" + encodeURIComponent(text);
  window.open(url, '_blank');
}

function printCropReport() {
  window.print();
}

function exportCropHistoryJSON() {
  var history = getCropHistoryData();
  var ageStr = localStorage.getItem('crop_age');
  var name = localStorage.getItem('crop_name') || 'Tanaman';
  var age = ageStr ? parseInt(ageStr, 10) : 0;
  var exportObj = {
    app: "SmartFarmSCADA",
    exportedAt: new Date().toISOString(),
    cropName: name,
    cropAge: age > 0 ? age : null,
    history: history
  };
  var dataStr = "data:text/json;charset=utf-8," + encodeURIComponent(JSON.stringify(exportObj, null, 2));
  var downloadAnchor = document.createElement('a');
  downloadAnchor.setAttribute("href", dataStr);
  var fileSuffix = age > 0 ? ("_HST" + age) : "";
  downloadAnchor.setAttribute("download", "smartfarm_riwayat_" + name.replace(/\s+/g, '_') + fileSuffix + ".json");
  document.body.appendChild(downloadAnchor);
  downloadAnchor.click();
  downloadAnchor.remove();
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
      if (parsed.cropAge) localStorage.setItem('crop_age', parsed.cropAge);
      if (parsed.cropName) localStorage.setItem('crop_name', parsed.cropName);
      updateCropHistorySummary();
      renderCropHistoryChart();
      var footerStatus = document.getElementById('crop-hist-footer-status');
      if (footerStatus) {
        footerStatus.innerHTML = '<span style="color:#10b981;">✓ Berhasil memulihkan ' + items.length + ' data riwayat tanam!</span>';
        setTimeout(function() {
          if (footerStatus) footerStatus.innerText = "Data tersimpan di LocalStorage peramban • Kurva Pertumbuhan Sigmoid (S-Curve)";
        }, 3500);
      }
    } catch (err) {
      alert("Gagal memuat file riwayat: " + err.message);
    }
  };
  reader.readAsText(file);
  e.target.value = '';
}
