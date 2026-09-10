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
  if (scadaCanvas) {
    attachScadaCrosshair();
    resizeCanvas();
  }
  var hCanvas = document.getElementById('hourlyChart');
  if (hCanvas) attachHourlyCrosshair();
}

function resizeCanvas() {
  if (!scadaCanvas) scadaCanvas = document.getElementById('scadaChart');
  if (scadaCanvas && scadaCanvas.parentElement) {
    scadaCanvas.width = scadaCanvas.parentElement.clientWidth;
    scadaCanvas.height = scadaCanvas.parentElement.clientHeight;
    drawChart();
  }
  var hCanvas = document.getElementById('hourlyChart');
  if (hCanvas && hCanvas.parentElement) {
    hCanvas.width = hCanvas.parentElement.clientWidth;
    hCanvas.height = hCanvas.parentElement.clientHeight;
    renderHourlyChart();
  }
  if (typeof resizeCropCanvas === 'function') resizeCropCanvas();
}

window.addEventListener('resize', resizeCanvas);

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
  if (soil === null || temp === null || isNaN(soil) || isNaN(temp)) return;
  var hVal = (hum !== null && !isNaN(hum)) ? hum : (scadaHistory.hum.length > 0 ? scadaHistory.hum[scadaHistory.hum.length - 1] : 60);
  if (soil < 0 || soil > 100 || temp < -10 || temp > 65 || hVal < 0 || hVal > 100) return;

  scadaHistory.time.push(new Date().toLocaleTimeString());
  scadaHistory.soil.push(soil);
  scadaHistory.temp.push(temp);
  scadaHistory.hum.push(hVal);

  var a = 0.35, len = scadaHistory.soil.length;
  if (len === 1) {
    scadaFiltered.soil.push(soil); scadaFiltered.temp.push(temp); scadaFiltered.hum.push(hVal);
  } else {
    var pS = scadaFiltered.soil[len - 2], pT = scadaFiltered.temp[len - 2], pH = scadaFiltered.hum[len - 2];
    scadaFiltered.soil.push(a * soil + (1 - a) * pS);
    scadaFiltered.temp.push(a * temp + (1 - a) * pT);
    scadaFiltered.hum.push(a * hVal + (1 - a) * pH);
  }

  while (scadaHistory.soil.length > oscBufferSize) {
    scadaHistory.time.shift(); scadaHistory.soil.shift(); scadaHistory.temp.shift(); scadaHistory.hum.shift();
    scadaFiltered.soil.shift(); scadaFiltered.temp.shift(); scadaFiltered.hum.shift();
  }

  var vS = document.getElementById('osc-val-soil'), vT = document.getElementById('osc-val-temp'), vH = document.getElementById('osc-val-hum');
  if (vS) vS.innerText = Math.round(soil) + "%";
  if (vT) vT.innerText = temp.toFixed(1) + "°C";
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
    ctx.beginPath(); ctx.arc(tipX, tipY, 4, 0, Math.PI * 2); ctx.fill();
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
        tooltip.innerHTML = '<span style="color:#94a3b8;">Jam ' + (hr < 10 ? '0' : '') + hr + ':00</span> &bull; ' +
          '<span style="color:#10b981;font-weight:700;">Tanah: ' + aS + '%</span> &bull; ' +
          '<span style="color:#06b6d4;font-weight:700;">Suhu: ' + aT + '°C</span> &bull; ' +
          '<span style="color:#a855f7;font-weight:700;">RH: ' + aH + '%</span> &bull; ' +
          '<span style="color:#f59e0b;font-weight:700;">Pompa: ' + pS + 's</span>';
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
  if (!scadaCanvas) return;
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
  var ctx = canvas.getContext('2d'), w = canvas.width = canvas.parentElement.clientWidth, h = canvas.height = canvas.parentElement.clientHeight;
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
  if (!logs || logs.length === 0) {
    ctx.fillStyle = "rgba(255,255,255,0.35)";
    ctx.font = "12px Inter, sans-serif";
    ctx.fillText("Belum Ada Data Log Per Jam (ESP32 Sedang Mengumpulkan Rekaman)", padL + 10, h / 2);
    var st = document.getElementById('hourly-summary-text');
    if (st) st.innerText = "Deteksi Evaporasi: Menunggu Rekaman Log LittleFS";
    return;
  }

  var hourlyMap = {};
  for (var hr = 0; hr < 24; hr++) hourlyMap[hr] = { tempSum: 0, humSum: 0, soilSum: 0, pumpSecsMax: 0, lampActive: 0, count: 0 };

  for (var i = 0; i < logs.length; i++) {
    var item = logs[i], hIdx = -1;
    if (item.time && item.time.indexOf(":") !== -1) {
      var tP = item.time.split(" ");
      hIdx = parseInt(tP[tP.length - 1].split(":")[0], 10);
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
  window.lastHourlyStatsMap = hourlyMap;

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
  if (statPumpEl) statPumpEl.innerText = pumpActivations + " Kali (" + totalPumpSecs + "s)";
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
      var barH = Math.min((pSecs / 1800) * chartH, chartH);
      if (barH > 0) {
        var bx = padL + b * step + step * 0.2;
        ctx.fillStyle = "rgba(245, 158, 11, 0.4)";
        ctx.fillRect(bx, padT + chartH - barH, step * 0.6, barH);
        ctx.strokeStyle = "#f59e0b";
        ctx.strokeRect(bx, padT + chartH - barH, step * 0.6, barH);
      }
    }
  }

  function drawHourlyLine(key, scale, color) {
    ctx.beginPath();
    ctx.strokeStyle = color;
    ctx.lineWidth = 2.4;
    var first = true;
    for (var s = 0; s < numSlots; s++) {
      var hr = hoursList[s];
      if (hourlyMap[hr].count > 0) {
        var avg = hourlyMap[hr][key] / hourlyMap[hr].count;
        var sx = padL + s * step + step / 2;
        var sy = padT + chartH - (avg / scale * chartH);
        if (first) { ctx.moveTo(sx, sy); first = false; } else ctx.lineTo(sx, sy);
        if (key === 'tempSum' && avg > maxTemp) { maxTemp = avg; maxHour = hr; }
      }
    }
    if (!first) ctx.stroke();
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
      : "Deteksi Evaporasi: Menunggu Rekam Log Per Jam";
  }
}
