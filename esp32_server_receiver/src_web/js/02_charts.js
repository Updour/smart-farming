// =================================================================
// 02_CHARTS.JS - LIVE SCADA OSCILLOSCOPE & 24H HOURLY AGGREGATION
// Real-Time Oscilloscope, Dual Y-Axis, Setpoints, Crosshairs & 24H Stats
// =================================================================

var scadaHistory = { time: [], soil: [], temp: [], hum: [] };
var scadaFiltered = { soil: [], temp: [], hum: [] };
var scadaCanvas = null;

var oscSeries = { soil: true, temp: true, hum: true };
var oscThresholdActive = true;
var oscFilterMode = 'raw';
var oscBufferSize = 30;
var oscCrosshairIdx = -1;

var hourlySeries = { soil: true, temp: true, hum: true, pump: true, lamp: true };
var hourlyRange = 'all';
var hourlyCrosshairIdx = -1;

function initScadaCanvas() {
  scadaCanvas = document.getElementById('scadaChart');
  
  // Hydrate buffer dari sessionStorage agar saat pindah tab atau refresh tidak reset ke 0
  try {
    var cachedBuf = sessionStorage.getItem('smartfarm_osc_buf');
    if (cachedBuf) {
      var parsed = JSON.parse(cachedBuf);
      if (parsed && parsed.soil && parsed.soil.length > 0) {
        scadaHistory = parsed;
        scadaFiltered = { soil: [], temp: [], hum: [] };
        var a = 0.35;
        for (var i = 0; i < scadaHistory.soil.length; i++) {
          if (i === 0) {
            scadaFiltered.soil.push(scadaHistory.soil[0]);
            scadaFiltered.temp.push(scadaHistory.temp[0]);
            scadaFiltered.hum.push(scadaHistory.hum[0]);
          } else {
            scadaFiltered.soil.push(a * scadaHistory.soil[i] + (1 - a) * scadaFiltered.soil[i - 1]);
            scadaFiltered.temp.push(a * scadaHistory.temp[i] + (1 - a) * scadaFiltered.temp[i - 1]);
            scadaFiltered.hum.push(a * scadaHistory.hum[i] + (1 - a) * scadaFiltered.hum[i - 1]);
          }
        }
        var st = document.getElementById('osc-status-text');
        if (st) st.innerText = "Buffer: " + scadaHistory.soil.length + " Titik (" + (oscFilterMode === 'ema' ? 'EMA Filter' : 'Raw Data') + ")";
      }
    }
  } catch (e) {}

  if (scadaCanvas) {
    attachScadaCrosshair();
    resizeCanvas();
  }
  var hCanvas = document.getElementById('hourlyChart');
  if (hCanvas) attachHourlyCrosshair();

  // Otomatis preload log LittleFS untuk chart 24 jam
  if (typeof fetchAndParseLogs === 'function') {
    fetchAndParseLogs();
  }
}

function resizeCanvas() {
  if (!scadaCanvas) scadaCanvas = document.getElementById('scadaChart');
  if (scadaCanvas && scadaCanvas.parentElement) {
    var pW = scadaCanvas.parentElement.clientWidth;
    var pH = scadaCanvas.parentElement.clientHeight || 230;
    if (pW > 0) {
      scadaCanvas.width = pW;
      scadaCanvas.height = pH;
      drawChart();
    }
  }
  var hCanvas = document.getElementById('hourlyChart');
  if (hCanvas && hCanvas.parentElement) {
    var hpW = hCanvas.parentElement.clientWidth;
    var hpH = hCanvas.parentElement.clientHeight || 250;
    if (hpW > 0) {
      hCanvas.width = hpW;
      hCanvas.height = hpH;
      renderHourlyChart();
    }
  }
  if (typeof resizeCropCanvas === 'function') resizeCropCanvas();
}

window.addEventListener('resize', resizeCanvas);
window.addEventListener('DOMContentLoaded', function () {
  setTimeout(initScadaCanvas, 200);
});

// --- TOGGLE CHIP HELPER ---
function setChipUI(id, active) {
  var el = document.getElementById(id);
  if (el) {
    el.classList.toggle('active', active);
    el.classList.toggle('inactive', !active);
  }
}

function toggleOscSeries(key) {
  if (oscSeries[key] !== undefined) {
    oscSeries[key] = !oscSeries[key];
    setChipUI('osc-toggle-' + key, oscSeries[key]);
    drawChart();
  }
}

function toggleOscThreshold() {
  oscThresholdActive = !oscThresholdActive;
  setChipUI('osc-toggle-threshold', oscThresholdActive);
  drawChart();
}

function setOscFilterMode(mode) {
  oscFilterMode = mode;
  var bRaw = document.getElementById('btn-osc-raw');
  var bEma = document.getElementById('btn-osc-ema');
  if (bRaw) bRaw.classList.toggle('active', mode === 'raw');
  if (bEma) bEma.classList.toggle('active', mode === 'ema');
  var st = document.getElementById('osc-status-text');
  if (st) st.innerText = "Buffer: " + scadaHistory.soil.length + " Titik (" + (mode === 'ema' ? 'EMA Filter' : 'Raw Data') + ")";
  drawChart();
}

function setOscBufferSize(size) {
  oscBufferSize = size;
  var b30 = document.getElementById('btn-buf-30');
  var b60 = document.getElementById('btn-buf-60');
  if (b30) b30.classList.toggle('active', size === 30);
  if (b60) b60.classList.toggle('active', size === 60);
  while (scadaHistory.soil.length > oscBufferSize) {
    scadaHistory.time.shift(); scadaHistory.soil.shift(); scadaHistory.temp.shift(); scadaHistory.hum.shift();
    scadaFiltered.soil.shift(); scadaFiltered.temp.shift(); scadaFiltered.hum.shift();
  }
  drawChart();
}

function toggleHourlySeries(key) {
  if (hourlySeries[key] !== undefined) {
    hourlySeries[key] = !hourlySeries[key];
    setChipUI('h-toggle-' + key, hourlySeries[key]);
    renderHourlyChart();
  }
}

function setHourlyRange(range) {
  hourlyRange = range;
  ['all', 'day', 'night'].forEach(function(r) {
    var b = document.getElementById('btn-h-' + r);
    if (b) b.classList.toggle('active', range === r);
  });
  var ax = document.getElementById('hourly-axis-text');
  if (ax) {
    if (range === 'day') ax.innerText = "Sumbu X: Jam 06:00 s/d 18:00 WIB (Siang)";
    else if (range === 'night') ax.innerText = "Sumbu X: Jam 18:00 s/d 06:00 WIB (Malam)";
    else ax.innerText = "Sumbu X: Jam 00:00 s/d 23:00 WIB (24 Jam Penuh)";
  }
  renderHourlyChart();
}

function refreshHourlyData() {
  var b = document.getElementById('btn-refresh-hourly');
  if (b) { b.style.opacity = '0.5'; b.style.pointerEvents = 'none'; }
  if (typeof fetchAndParseLogs === 'function') fetchAndParseLogs(); else renderHourlyChart();
  setTimeout(function () { if (b) { b.style.opacity = '1'; b.style.pointerEvents = 'auto'; } }, 700);
}

// --- DATA INGESTION & EMA FILTER ---
function updateHistory(soil, temp, hum) {
  var tData = window.lastTelemetryData || {};

  // Ekstraksi suhu cerdas (mendukung suhuC dan temp)
  if (temp === null || isNaN(temp)) {
    var rawT = (tData.suhuC !== undefined && tData.suhuC !== "--") ? tData.suhuC : tData.temp;
    if (rawT !== undefined && rawT !== null && !isNaN(parseFloat(rawT))) temp = parseFloat(rawT);
  }

  // Ekstraksi kelembapan tanah
  if (soil === null || isNaN(soil)) {
    var rawS = (tData.soil !== undefined && tData.soil !== "--") ? tData.soil : null;
    if (rawS !== null && !isNaN(parseFloat(rawS)) && parseFloat(rawS) >= 0) soil = parseFloat(rawS);
  }

  // Ekstraksi kelembapan udara (RH)
  if (hum === null || isNaN(hum)) {
    var rawH = (tData.hum !== undefined && tData.hum !== "--") ? tData.hum : null;
    if (rawH !== null && !isNaN(parseFloat(rawH))) hum = parseFloat(rawH);
  }

  // Jika kedua sensor tanah dan suhu sama sekali belum siap, abaikan pencatatan
  if ((soil === null || isNaN(soil)) && (temp === null || isNaN(temp))) return;

  // Pertahankan nilai terakhir jika salah satu sensor offline sesaat
  var sVal = (soil !== null && !isNaN(soil)) ? soil : (scadaHistory.soil.length > 0 ? scadaHistory.soil[scadaHistory.soil.length - 1] : 0);
  var tVal = (temp !== null && !isNaN(temp)) ? temp : (scadaHistory.temp.length > 0 ? scadaHistory.temp[scadaHistory.temp.length - 1] : 28);
  var hVal = (hum !== null && !isNaN(hum)) ? hum : (scadaHistory.hum.length > 0 ? scadaHistory.hum[scadaHistory.hum.length - 1] : 65);

  if (sVal < 0 || sVal > 100 || tVal < -10 || tVal > 65 || hVal < 0 || hVal > 100) return;

  var nowStr = new Date().toLocaleTimeString('id-ID');
  scadaHistory.time.push(nowStr);
  scadaHistory.soil.push(sVal);
  scadaHistory.temp.push(tVal);
  scadaHistory.hum.push(hVal);

  var a = 0.35, len = scadaHistory.soil.length;
  if (len === 1) {
    scadaFiltered.soil.push(sVal); scadaFiltered.temp.push(tVal); scadaFiltered.hum.push(hVal);
  } else {
    var pS = scadaFiltered.soil[len - 2], pT = scadaFiltered.temp[len - 2], pH = scadaFiltered.hum[len - 2];
    scadaFiltered.soil.push(a * sVal + (1 - a) * pS);
    scadaFiltered.temp.push(a * tVal + (1 - a) * pT);
    scadaFiltered.hum.push(a * hVal + (1 - a) * pH);
  }

  while (scadaHistory.soil.length > oscBufferSize) {
    scadaHistory.time.shift(); scadaHistory.soil.shift(); scadaHistory.temp.shift(); scadaHistory.hum.shift();
    scadaFiltered.soil.shift(); scadaFiltered.temp.shift(); scadaFiltered.hum.shift();
  }

  try {
    sessionStorage.setItem('smartfarm_osc_buf', JSON.stringify(scadaHistory));
  } catch (e) {}

  var vS = document.getElementById('osc-val-soil'), vT = document.getElementById('osc-val-temp'), vH = document.getElementById('osc-val-hum');
  if (vS) vS.innerText = Math.round(sVal) + "%";
  if (vT) vT.innerText = tVal.toFixed(1) + "°C";
  if (vH && hVal !== null) vH.innerText = Math.round(hVal) + "%";
  var st = document.getElementById('osc-status-text');
  if (st) st.innerText = "Buffer: " + scadaHistory.soil.length + " Titik (" + (oscFilterMode === 'ema' ? 'EMA Filter' : 'Raw Data') + ")";
  drawChart();
}

// --- CURVE RENDER HELPER ---
function drawLiveCurve(ctx, pts, scaleMax, padL, padT, chartW, chartH, maxSlots, color, doArea, glow) {
  var count = pts.length;
  if (count === 0) return;

  var stepX = chartW / Math.max(maxSlots - 1, 1);

  // Jika baru 1 titik, langsung render lingkaran/titik penanda agar tidak kosong
  if (count === 1) {
    var pY = padT + chartH - (pts[0] / scaleMax * chartH);
    ctx.fillStyle = color;
    ctx.beginPath();
    ctx.arc(padL, pY, 4.5, 0, Math.PI * 2);
    ctx.fill();
    ctx.strokeStyle = "#ffffff";
    ctx.lineWidth = 1.5;
    ctx.stroke();
    return;
  }

  if (doArea) {
    var grad = ctx.createLinearGradient(0, padT, 0, padT + chartH);
    grad.addColorStop(0, "rgba(16, 185, 129, 0.22)");
    grad.addColorStop(1, "rgba(16, 185, 129, 0.0)");
    ctx.beginPath();
    for (var i = 0; i < count; i++) {
      var x = padL + i * stepX, y = padT + chartH - (pts[i] / scaleMax * chartH);
      if (i === 0) ctx.moveTo(x, y); else ctx.lineTo(x, y);
    }
    ctx.lineTo(padL + (count - 1) * stepX, padT + chartH);
    ctx.lineTo(padL, padT + chartH);
    ctx.closePath();
    ctx.fillStyle = grad;
    ctx.fill();
  }

  ctx.beginPath();
  ctx.strokeStyle = color;
  ctx.lineWidth = 2.4;
  for (var j = 0; j < count; j++) {
    var px = padL + j * stepX, py = padT + chartH - (pts[j] / scaleMax * chartH);
    if (j === 0) ctx.moveTo(px, py); else ctx.lineTo(px, py);
  }
  ctx.stroke();

  if (glow) {
    var tipX = padL + (count - 1) * stepX, tipY = padT + chartH - (pts[count - 1] / scaleMax * chartH);
    ctx.fillStyle = color;
    ctx.beginPath(); ctx.arc(tipX, tipY, 4.5, 0, Math.PI * 2); ctx.fill();
    ctx.strokeStyle = "#ffffff";
    ctx.lineWidth = 1.5;
    ctx.stroke();
  }
}

// --- CROSSHAIR INTERACTIVITY ---
function attachScadaCrosshair() {
  var canvas = document.getElementById('scadaChart');
  var tooltip = document.getElementById('osc-crosshair-tooltip');
  if (!canvas) return;

  function handleMove(e) {
    var rect = canvas.getBoundingClientRect();
    var clientX = e.touches ? e.touches[0].clientX : e.clientX;
    var x = clientX - rect.left;
    var padL = 36, padR = 36, chartW = canvas.width - padL - padR;
    var ptsCount = scadaHistory.soil.length;
    if (ptsCount === 0 || x < padL || x > padL + chartW) {
      oscCrosshairIdx = -1;
      if (tooltip) tooltip.style.display = 'none';
      drawChart();
      return;
    }
    var slots = Math.max(oscBufferSize, 10);
    var stepX = chartW / Math.max(slots - 1, 1);
    var relIdx = Math.round((x - padL) / stepX);
    if (relIdx >= 0 && relIdx < ptsCount) {
      oscCrosshairIdx = relIdx;
      if (tooltip) {
        var t = scadaHistory.time[relIdx] || "--";
        var s = Math.round(scadaHistory.soil[relIdx]);
        var tp = scadaHistory.temp[relIdx] ? scadaHistory.temp[relIdx].toFixed(1) : "--";
        var hm = scadaHistory.hum[relIdx] ? Math.round(scadaHistory.hum[relIdx]) : "--";
        tooltip.innerHTML = '<span style="color:#94a3b8;">' + t + '</span> &bull; ' +
          '<span style="color:#10b981;font-weight:700;">Tanah: ' + s + '%</span> &bull; ' +
          '<span style="color:#06b6d4;font-weight:700;">Suhu: ' + tp + '°C</span> &bull; ' +
          '<span style="color:#a855f7;font-weight:700;">RH: ' + hm + '%</span>';
        tooltip.style.display = 'block';
      }
      drawChart();
    }
  }

  function handleLeave() {
    oscCrosshairIdx = -1;
    if (tooltip) tooltip.style.display = 'none';
    drawChart();
  }

  canvas.addEventListener('mousemove', handleMove);
  canvas.addEventListener('mouseleave', handleLeave);
  canvas.addEventListener('touchmove', handleMove, { passive: true });
  canvas.addEventListener('touchend', handleLeave);
}

function attachHourlyCrosshair() {
  var canvas = document.getElementById('hourlyChart');
  var tooltip = document.getElementById('hourly-crosshair-tooltip');
  if (!canvas) return;

  function handleMove(e) {
    var rect = canvas.getBoundingClientRect();
    var clientX = e.touches ? e.touches[0].clientX : e.clientX;
    var x = clientX - rect.left;
    var padL = 36, padR = 36, chartW = canvas.width - padL - padR;
    var hoursList = getActiveHoursList();
    if (hoursList.length === 0 || x < padL || x > padL + chartW) {
      hourlyCrosshairIdx = -1;
      if (tooltip) tooltip.style.display = 'none';
      renderHourlyChart();
      return;
    }
    var step = chartW / hoursList.length;
    var slotIdx = Math.floor((x - padL) / step);
    if (slotIdx >= 0 && slotIdx < hoursList.length) {
      hourlyCrosshairIdx = slotIdx;
      var hr = hoursList[slotIdx];
      var stats = window.lastHourlyStatsMap ? window.lastHourlyStatsMap[hr] : null;
      if (tooltip && stats && stats.count > 0) {
        var aS = (stats.soilSum / stats.count).toFixed(0);
        var aT = (stats.tempSum / stats.count).toFixed(1);
        var aH = (stats.humSum / stats.count).toFixed(0);
        var pS = stats.pumpSecsMax || 0;
        var pStr = pS >= 60 ? Math.floor(pS / 60) + "m " + (pS % 60) + "s" : pS + "s";
        tooltip.innerHTML = '<span style="color:#94a3b8;">Jam ' + (hr < 10 ? '0' : '') + hr + ':00</span> &bull; ' +
          '<span style="color:#10b981;font-weight:700;">Tanah: ' + aS + '%</span> &bull; ' +
          '<span style="color:#06b6d4;font-weight:700;">Suhu: ' + aT + '°C</span> &bull; ' +
          '<span style="color:#a855f7;font-weight:700;">RH: ' + aH + '%</span> &bull; ' +
          '<span style="color:#f59e0b;font-weight:700;">Pompa: ' + pStr + '</span>';
        tooltip.style.display = 'block';
      }
      renderHourlyChart();
    }
  }

  function handleLeave() {
    hourlyCrosshairIdx = -1;
    if (tooltip) tooltip.style.display = 'none';
    renderHourlyChart();
  }

  canvas.addEventListener('mousemove', handleMove);
  canvas.addEventListener('mouseleave', handleLeave);
  canvas.addEventListener('touchmove', handleMove, { passive: true });
  canvas.addEventListener('touchend', handleLeave);
}

function getActiveHoursList() {
  var list = [];
  if (hourlyRange === 'day') for (var d = 6; d <= 18; d++) list.push(d);
  else if (hourlyRange === 'night') {
    for (var n1 = 18; n1 < 24; n1++) list.push(n1);
    for (var n2 = 0; n2 <= 6; n2++) list.push(n2);
  } else {
    for (var a = 0; a < 24; a++) list.push(a);
  }
  return list;
}

// --- RENDER OSCILLOSCOPE ---
function drawChart() {
  if (!scadaCanvas) scadaCanvas = document.getElementById('scadaChart');
  if (!scadaCanvas || !scadaCanvas.parentElement) return;
  var pW = scadaCanvas.parentElement.clientWidth;
  var pH = scadaCanvas.parentElement.clientHeight || 230;
  if (pW <= 0) return; // Tab charts sedang tersembunyi
  if (scadaCanvas.width !== pW || scadaCanvas.height !== pH) {
    scadaCanvas.width = pW;
    scadaCanvas.height = pH;
  }
  var ctx = scadaCanvas.getContext('2d'), w = scadaCanvas.width, h = scadaCanvas.height;
  ctx.clearRect(0, 0, w, h);

  var padL = 36, padR = 36, padT = 16, padB = 22;
  var chartW = w - padL - padR, chartH = h - padT - padB;

  // Dual Y-Axis Grid & Labels
  ctx.strokeStyle = "rgba(255,255,255,0.06)";
  ctx.lineWidth = 1;
  ctx.font = "9px Inter, monospace";

  for (var k = 0; k <= 4; k++) {
    var gy = padT + (chartH * (k / 4));
    ctx.beginPath(); ctx.moveTo(padL, gy); ctx.lineTo(w - padR, gy); ctx.stroke();
    // Left Y-Axis (0 - 100%)
    ctx.fillStyle = "rgba(255,255,255,0.4)";
    ctx.fillText((100 - k * 25) + "%", 6, gy + 3);
    // Right Y-Axis (0 - 50°C)
    ctx.fillStyle = "rgba(6, 182, 212, 0.75)";
    ctx.fillText((50 - k * 12.5).toFixed(0) + "°C", w - padR + 6, gy + 3);
  }

  // Zona Nyaman & Setpoint Thresholds
  if (oscThresholdActive) {
    // 1. Zona Nyaman Tanah (60% - 75%)
    var y75 = padT + chartH - (75 / 100 * chartH);
    var y60 = padT + chartH - (60 / 100 * chartH);
    ctx.fillStyle = "rgba(16, 185, 129, 0.07)";
    ctx.fillRect(padL, y75, chartW, y60 - y75);
    ctx.fillStyle = "rgba(16, 185, 129, 0.4)";
    ctx.font = "8px Inter, sans-serif";
    ctx.fillText("ZONA NYAMAN TANAH (60-75%)", padL + 6, y75 + 10);

    // 2. Ambang Batas Siram (Setpoint Soil Moisture)
    var sThresh = (window.lastTelemetryData && window.lastTelemetryData.batasTanah !== undefined) ? window.lastTelemetryData.batasTanah : 50;
    var yThresh = padT + chartH - (sThresh / 100 * chartH);
    ctx.save();
    ctx.setLineDash([4, 4]);
    ctx.strokeStyle = "rgba(239, 68, 68, 0.85)";
    ctx.lineWidth = 1.4;
    ctx.beginPath();
    ctx.moveTo(padL, yThresh);
    ctx.lineTo(padL + chartW, yThresh);
    ctx.stroke();
    ctx.fillStyle = "#ef4444";
    ctx.font = "8px Inter, sans-serif";
    ctx.fillText("BATAS SIRAM (" + sThresh + "%)", padL + chartW - 90, yThresh - 3);
    ctx.restore();

    // 3. Ambang Batas Bahaya Panas (Temperature Danger Line)
    var tThresh = (window.lastTelemetryData && window.lastTelemetryData.batasSuhu !== undefined) ? window.lastTelemetryData.batasSuhu : 32;
    var yTempThresh = padT + chartH - (tThresh / 50 * chartH);
    ctx.save();
    ctx.setLineDash([3, 3]);
    ctx.strokeStyle = "rgba(245, 158, 11, 0.75)";
    ctx.lineWidth = 1.2;
    ctx.beginPath();
    ctx.moveTo(padL, yTempThresh);
    ctx.lineTo(padL + chartW, yTempThresh);
    ctx.stroke();
    ctx.fillStyle = "#f59e0b";
    ctx.font = "8px Inter, sans-serif";
    ctx.fillText("BATAS SUHU (" + tThresh + "°C)", padL + 6, yTempThresh - 3);
    ctx.restore();
  }

  var d = (oscFilterMode === 'ema') ? scadaFiltered : scadaHistory;
  if (d.soil.length < 1) {
    ctx.fillStyle = "rgba(255,255,255,0.35)";
    ctx.font = "12px Inter, sans-serif";
    ctx.fillText("Oscilloscope SCADA Aktif - Menunggu Sinyal Telemetri ESP...", padL + 20, h / 2);
    return;
  }

  var slots = Math.max(oscBufferSize, 10);
  if (oscSeries.soil) drawLiveCurve(ctx, d.soil, 100, padL, padT, chartW, chartH, slots, '#10b981', true, true);
  if (oscSeries.hum)  drawLiveCurve(ctx, d.hum, 100, padL, padT, chartW, chartH, slots, '#a855f7', false, true);
  if (oscSeries.temp) drawLiveCurve(ctx, d.temp, 50, padL, padT, chartW, chartH, slots, '#06b6d4', false, true);

  // Draw Crosshair on Hover/Touch
  if (oscCrosshairIdx >= 0 && oscCrosshairIdx < d.soil.length) {
    var stepX = chartW / Math.max(slots - 1, 1);
    var chX = padL + oscCrosshairIdx * stepX;
    ctx.save();
    ctx.setLineDash([2, 2]);
    ctx.strokeStyle = "rgba(255, 255, 255, 0.5)";
    ctx.lineWidth = 1;
    ctx.beginPath();
    ctx.moveTo(chX, padT);
    ctx.lineTo(chX, padT + chartH);
    ctx.stroke();
    ctx.restore();
  }

  if (scadaHistory.time.length > 1) {
    ctx.fillStyle = "rgba(255,255,255,0.3)";
    ctx.font = "8px Inter, monospace";
    ctx.fillText(scadaHistory.time[0], padL, h - 6);
    ctx.fillText(scadaHistory.time[scadaHistory.time.length - 1], w - padR - 35, h - 6);
  }
}

// --- RENDER 24H HOURLY AGGREGATION ---
function renderHourlyChart() {
  var canvas = document.getElementById('hourlyChart');
  if (!canvas || !canvas.parentElement) return;
  var pW = canvas.parentElement.clientWidth;
  var pH = canvas.parentElement.clientHeight || 250;
  if (pW <= 0) return; // Tab charts sedang tersembunyi
  if (canvas.width !== pW || canvas.height !== pH) {
    canvas.width = pW;
    canvas.height = pH;
  }
  var ctx = canvas.getContext('2d'), w = canvas.width, h = canvas.height;
  ctx.clearRect(0, 0, w, h);

  var padL = 36, padR = 36, padT = 16, padB = 24;
  var chartW = w - padL - padR, chartH = h - padT - padB;

  ctx.strokeStyle = "rgba(255,255,255,0.05)";
  ctx.lineWidth = 1;
  ctx.font = "9px Inter, monospace";

  for (var k = 0; k <= 4; k++) {
    var gy = padT + (chartH * (k / 4));
    ctx.beginPath(); ctx.moveTo(padL, gy); ctx.lineTo(w - padR, gy); ctx.stroke();
    // Left Y-Axis
    ctx.fillStyle = "rgba(255,255,255,0.35)";
    ctx.fillText((100 - k * 25) + "%", 6, gy + 3);
    // Right Y-Axis
    ctx.fillStyle = "rgba(6, 182, 212, 0.7)";
    ctx.fillText((50 - k * 12.5).toFixed(0) + "°C", w - padR + 6, gy + 3);
  }

  var logs = (typeof rawLogsCache !== 'undefined') ? rawLogsCache : [];
  var hourlyMap = {};
  for (var hr = 0; hr < 24; hr++) {
    hourlyMap[hr] = { tempSum: 0, humSum: 0, soilSum: 0, pumpSecsMax: 0, lampActive: 0, count: 0 };
  }

  // 1. Agregasi dari rekaman CSV LittleFS
  if (logs && logs.length > 0) {
    for (var i = 0; i < logs.length; i++) {
      var item = logs[i], hIdx = -1;
      if (item.time && item.time.indexOf(":") !== -1) {
        var tP = item.time.split(" ");
        var timePart = tP[tP.length - 1];
        hIdx = parseInt(timePart.split(":")[0], 10);
      }
      if (hIdx >= 0 && hIdx < 24) {
        hourlyMap[hIdx].tempSum += item.temp;
        hourlyMap[hIdx].humSum += item.hum;
        hourlyMap[hIdx].soilSum += item.soil;
        hourlyMap[hIdx].pumpSecsMax = Math.max(hourlyMap[hIdx].pumpSecsMax, item.pumpSecs || 0);
        if (item.lamp === "1" || item.lamp === 1 || item.lamp === true || item.lamp === "ON") {
          hourlyMap[hIdx].lampActive += 1;
        }
        hourlyMap[hIdx].count += 1;
      }
    }
  }

  // 2. Gabungkan data telemetri live terkini ke slot jam saat ini
  var tData = window.lastTelemetryData;
  if (tData) {
    var nowHour = new Date().getHours();
    var curT = (tData.suhuC !== undefined && tData.suhuC !== "--") ? parseFloat(tData.suhuC) : (tData.temp !== undefined && tData.temp !== "--" ? parseFloat(tData.temp) : null);
    var curS = (tData.soil !== undefined && tData.soil !== "--" && !isNaN(parseFloat(tData.soil))) ? parseFloat(tData.soil) : null;
    var curH = (tData.hum !== undefined && tData.hum !== "--") ? parseFloat(tData.hum) : null;

    if (curT !== null && curS !== null) {
      hourlyMap[nowHour].tempSum += curT;
      hourlyMap[nowHour].soilSum += curS;
      hourlyMap[nowHour].humSum += (curH !== null ? curH : 65);
      if (tData.hourlySecs !== undefined) {
        hourlyMap[nowHour].pumpSecsMax = Math.max(hourlyMap[nowHour].pumpSecsMax, parseInt(tData.hourlySecs, 10));
      }
      if (tData.lampOn == 1 || tData.lamp == 1) hourlyMap[nowHour].lampActive += 1;
      hourlyMap[nowHour].count += 1;
    }
  }

  window.lastHourlyStatsMap = hourlyMap;

  // Hitung total titik yang valid
  var totalValidPoints = 0;
  for (var c = 0; c < 24; c++) {
    if (hourlyMap[c].count > 0) totalValidPoints++;
  }

  if (totalValidPoints === 0) {
    ctx.fillStyle = "rgba(255,255,255,0.35)";
    ctx.font = "12px Inter, sans-serif";
    ctx.fillText("Sedang Menyelaraskan Log LittleFS Flash Memory...", padL + 10, h / 2);
    var stPrompt = document.getElementById('hourly-summary-text');
    if (stPrompt) stPrompt.innerText = "Deteksi Evaporasi: Menunggu Rekaman Log LittleFS";
    return;
  }

  // Compute 24-Hour Quick Statistics
  var minTemp24 = 999, maxTemp24 = -999, totalSoilSum = 0, totalSoilCount = 0, totalPumpSecs = 0, pumpActivations = 0, lampHours = 0;
  for (var chk = 0; chk < 24; chk++) {
    var hm = hourlyMap[chk];
    if (hm.count > 0) {
      var hTemp = hm.tempSum / hm.count;
      if (hTemp < minTemp24) minTemp24 = hTemp;
      if (hTemp > maxTemp24) maxTemp24 = hTemp;
      totalSoilSum += hm.soilSum;
      totalSoilCount += hm.count;
    }
    if (hm.pumpSecsMax > 0) {
      totalPumpSecs += hm.pumpSecsMax;
      pumpActivations++;
    }
    if (hm.lampActive > 0) lampHours++;
  }

  var statTempEl = document.getElementById('stat-24h-temp');
  var statSoilEl = document.getElementById('stat-24h-soil');
  var statPumpEl = document.getElementById('stat-24h-pump');
  var statLampEl = document.getElementById('stat-24h-lamp');

  if (statTempEl && minTemp24 !== 999) statTempEl.innerText = minTemp24.toFixed(1) + "°C / " + maxTemp24.toFixed(1) + "°C";
  if (statSoilEl && totalSoilCount > 0) statSoilEl.innerText = Math.round(totalSoilSum / totalSoilCount) + "%";
  if (statPumpEl) {
    var pDurStr = totalPumpSecs >= 60 ? Math.floor(totalPumpSecs / 60) + "m " + (totalPumpSecs % 60) + "s" : totalPumpSecs + "s";
    statPumpEl.innerText = pumpActivations + " Kali (" + pDurStr + ")";
  }
  if (statLampEl) statLampEl.innerText = lampHours + " Jam";

  var hoursList = getActiveHoursList();
  var numSlots = hoursList.length, step = chartW / numSlots, maxTemp = 0, maxHour = -1;

  // Render Lamp Background Highlight
  if (hourlySeries.lamp) {
    var schedStart = parseInt(localStorage.getItem('lamp_sched_start') || '18', 10);
    var schedEnd = parseInt(localStorage.getItem('lamp_sched_end') || '6', 10);

    for (var bL = 0; bL < numSlots; bL++) {
      var lHour = hoursList[bL];
      var isLampOn = (hourlyMap[lHour].lampActive > 0);
      if (!isLampOn) {
        if (schedStart > schedEnd ? (lHour >= schedStart || lHour < schedEnd) : (lHour >= schedStart && lHour < schedEnd)) {
          isLampOn = true;
        }
      }
      if (isLampOn) {
        var lx = padL + bL * step;
        ctx.fillStyle = "rgba(234, 179, 8, 0.08)";
        ctx.fillRect(lx, padT, step, chartH);
        ctx.fillStyle = "#eab308";
        ctx.fillRect(lx + 1, padT, step - 2, 3);
      }
    }
  }

  // Render Pump Bars
  if (hourlySeries.pump) {
    for (var b = 0; b < numSlots; b++) {
      var pHour = hoursList[b], pSecs = hourlyMap[pHour].pumpSecsMax;
      var barH = Math.min((pSecs / 300) * chartH, chartH); // 300s (5 menit) skala penuh
      if (barH > 0) {
        var bx = padL + b * step + step * 0.2;
        ctx.fillStyle = "rgba(245, 158, 11, 0.4)";
        ctx.fillRect(bx, padT + chartH - barH, step * 0.6, barH);
        ctx.strokeStyle = "#f59e0b";
        ctx.strokeRect(bx, padT + chartH - barH, step * 0.6, barH);
      }
    }
  }

  // Draw Connected Hourly Lines & Guaranteed Visible Points
  function drawHourlyLine(key, scale, color) {
    ctx.beginPath();
    ctx.strokeStyle = color;
    ctx.lineWidth = 2.4;
    var first = true;
    var validPts = [];

    for (var s = 0; s < numSlots; s++) {
      var hr = hoursList[s];
      if (hourlyMap[hr].count > 0) {
        var avg = hourlyMap[hr][key] / hourlyMap[hr].count;
        var sx = padL + s * step + step / 2;
        var sy = padT + chartH - (avg / scale * chartH);
        validPts.push({ x: sx, y: sy, avg: avg, hr: hr });
        if (first) { ctx.moveTo(sx, sy); first = false; } else ctx.lineTo(sx, sy);
        if (key === 'tempSum' && avg > maxTemp) { maxTemp = avg; maxHour = hr; }
      }
    }
    if (!first) ctx.stroke();

    // Render lingkaran untuk setiap titik agar titik tunggal/jarang tetap 100% terlihat
    ctx.fillStyle = color;
    for (var p = 0; p < validPts.length; p++) {
      ctx.beginPath();
      ctx.arc(validPts[p].x, validPts[p].y, 3.5, 0, Math.PI * 2);
      ctx.fill();
      ctx.strokeStyle = "#ffffff";
      ctx.lineWidth = 1;
      ctx.stroke();
    }
  }

  if (hourlySeries.soil) drawHourlyLine('soilSum', 100, '#10b981');
  if (hourlySeries.hum)  drawHourlyLine('humSum', 100, '#a855f7');
  if (hourlySeries.temp) drawHourlyLine('tempSum', 50, '#06b6d4');

  // Draw Crosshair for Hourly
  if (hourlyCrosshairIdx >= 0 && hourlyCrosshairIdx < numSlots) {
    var hChX = padL + hourlyCrosshairIdx * step + step / 2;
    ctx.save();
    ctx.setLineDash([2, 2]);
    ctx.strokeStyle = "rgba(255, 255, 255, 0.5)";
    ctx.lineWidth = 1;
    ctx.beginPath();
    ctx.moveTo(hChX, padT);
    ctx.lineTo(hChX, padT + chartH);
    ctx.stroke();
    ctx.restore();
  }

  ctx.fillStyle = "rgba(255,255,255,0.4)";
  ctx.font = "9px Inter, monospace";
  var stepLbl = (numSlots > 16) ? 3 : 2;
  for (var l = 0; l < numSlots; l += stepLbl) {
    var lHour = hoursList[l];
    ctx.fillText((lHour < 10 ? '0' : '') + lHour + ":00", padL + l * step + 2, h - 6);
  }

  var sumText = document.getElementById('hourly-summary-text');
  if (sumText) {
    sumText.innerText = (maxHour >= 0)
      ? "Deteksi Evaporasi: Puncak Suhu (" + maxTemp.toFixed(1) + "°C) Jam " + (maxHour < 10 ? '0' : '') + maxHour + ":00 WIB"
      : "Deteksi Evaporasi: Siklus Diurnal Terpantau Normal";
  }
}

// Window Globals Binding untuk inline HTML onclick
window.initScadaCanvas = initScadaCanvas;
window.resizeCanvas = resizeCanvas;
window.drawChart = drawChart;
window.renderHourlyChart = renderHourlyChart;
window.updateHistory = updateHistory;
window.toggleOscSeries = toggleOscSeries;
window.toggleOscThreshold = toggleOscThreshold;
window.setOscFilterMode = setOscFilterMode;
window.setOscBufferSize = setOscBufferSize;
window.toggleHourlySeries = toggleHourlySeries;
window.setHourlyRange = setHourlyRange;
window.refreshHourlyData = refreshHourlyData;
