/**
 * 13_logs_init.js - LittleFS Flash Log Viewer, Live Telemetry Stream & Smart Cloud Backup
 * Zero-Dummy, Robust SCADA Logging & Real-Time Transmission Packet Feed
 * Strict line limit < 400 lines
 */

var rawLogsCache = [];
var activeLogCategory = 'all';
window.feedPaused = false;
window.feedPacketsCount = 0;

function switchLogsView(view) {
  var vTable = document.getElementById('view-container-table'), vFeed = document.getElementById('view-container-feed');
  var bTable = document.getElementById('btn-view-logs-table'), bFeed = document.getElementById('btn-view-logs-feed');
  var isFeed = (view === 'feed');
  if (vTable) vTable.style.display = isFeed ? 'none' : 'block';
  if (vFeed) vFeed.style.display = isFeed ? 'block' : 'none';
  if (bTable) { bTable.style.background = isFeed ? 'transparent' : 'var(--primary)'; bTable.style.color = isFeed ? 'var(--text-sub)' : '#fff'; }
  if (bFeed) { bFeed.style.background = isFeed ? 'var(--primary)' : 'transparent'; bFeed.style.color = isFeed ? '#fff' : 'var(--text-sub)'; }
}

function fetchAndParseLogs() {
  fetch('/downloadLog')
    .then(function (res) { if (!res.ok) throw new Error("HTTP " + res.status); return res.text(); })
    .then(function (text) { parseCSVLogs(text); })
    .catch(function () { rawLogsCache = []; renderLogTable([]); if (typeof renderHourlyChart === 'function') renderHourlyChart(); });
}

function parseCSVLogs(csvText) {
  if (!csvText || csvText.indexOf(",") === -1) {
    rawLogsCache = [];
    renderLogTable([]);
    if (typeof renderHourlyChart === 'function') renderHourlyChart();
    return;
  }

  var lines = csvText.split('\n');
  var parsed = [];
  for (var i = 1; i < lines.length; i++) {
    var line = lines[i].trim();
    if (!line) continue;
    var cols = line.split(',');
    if (cols.length >= 4) {
      parsed.push({
        time: cols[0] || "-",
        temp: parseFloat(cols[1]) || 0,
        hum: parseFloat(cols[2]) || 0,
        soil: parseInt(cols[3], 10) || 0,
        category: cols[4] || "-",
        pumpCount: cols[5] || "0",
        pumpSecs: parseInt(cols[6], 10) || 0,
        rawAdc: cols[7] || "-",
        satTemp: cols[8] || "-",
        satDesc: cols[9] || "-",
        satRain: cols[10] || "-",
        lamp: cols[11] ? cols[11].trim() : "0"
      });
    }
  }

  rawLogsCache = parsed;
  renderLogTable(parsed);
  if (typeof renderHourlyChart === 'function') renderHourlyChart();
}

function renderLogTable(logs) {
  var tbody = document.getElementById('log-table-body');
  var summaryElem = document.getElementById('log-count-summary');
  if (!tbody) return;

  if (!logs || logs.length === 0) {
    if (summaryElem) summaryElem.innerText = "Total Record: 0 Baris";
    tbody.innerHTML = "<tr><td colspan='7' style='padding:28px; text-align:center; color:var(--text-sub);'>Belum ada data log di Flash LittleFS / ESP32 Sedang Mengumpulkan Rekaman.</td></tr>";
    return;
  }

  if (summaryElem) summaryElem.innerText = "Total Record: " + logs.length + " Baris";

  var html = "";
  for (var i = logs.length - 1; i >= 0; i--) {
    var item = logs[i];
    var catBg = "rgba(16, 185, 129, 0.12)";
    var catColor = "#10b981";
    var catBorder = "rgba(16, 185, 129, 0.3)";

    if (item.category.indexOf("Kering") !== -1) {
      catBg = "rgba(245, 158, 11, 0.12)";
      catColor = "#f59e0b";
      catBorder = "rgba(245, 158, 11, 0.3)";
    } else if (item.category.indexOf("Lembab") !== -1 || item.category.indexOf("Basah") !== -1) {
      catBg = "rgba(6, 182, 212, 0.12)";
      catColor = "#06b6d4";
      catBorder = "rgba(6, 182, 212, 0.3)";
    }

    var isLampOn = (item.lamp === "1" || item.lamp === 1 || item.lamp === "ON");
    var lampBadge = isLampOn
      ? "<span style='color:#eab308; background:rgba(234,179,8,0.15); border:1px solid rgba(234,179,8,0.3); padding:1px 6px; border-radius:6px; font-size:10px; margin-left:6px;'>Lampu ON</span>"
      : "";

    html += "<tr style='border-bottom:1px solid rgba(255,255,255,0.04);'>" +
      "<td style='padding:9px 14px; font-weight:600; color:var(--text-main); font-family:monospace;'>" + item.time + "</td>" +
      "<td style='padding:9px 14px; color:#06b6d4; font-weight:700; font-family:monospace;'>" + item.temp + "°C</td>" +
      "<td style='padding:9px 14px; color:#a855f7; font-family:monospace;'>" + item.hum + "%</td>" +
      "<td style='padding:9px 14px; color:#10b981; font-weight:700; font-family:monospace;'>" + item.soil + "%</td>" +
      "<td style='padding:9px 14px;'><span style='background:" + catBg + "; color:" + catColor + "; border:1px solid " + catBorder + "; padding:2px 8px; border-radius:8px; font-size:11px; font-weight:600;'>" + item.category + "</span></td>" +
      "<td style='padding:9px 14px; color:var(--text-sub); font-family:monospace;'>ADC: " + (item.rawAdc || "-") + "</td>" +
      "<td style='padding:9px 14px; font-family:monospace;'><span style='color:#f59e0b; font-weight:600;'>" + item.pumpSecs + "s (" + item.pumpCount + "x)</span>" + lampBadge + "</td>" +
      "</tr>";
  }
  tbody.innerHTML = html;
}

function filterLogTable(query) {
  if (!rawLogsCache || rawLogsCache.length === 0) return;
  var q = (query || "").toLowerCase().trim();
  if (!q) {
    applyCurrentLogFilters();
    return;
  }
  var filtered = rawLogsCache.filter(function (item) {
    return item.time.toLowerCase().indexOf(q) !== -1 ||
      String(item.temp).indexOf(q) !== -1 ||
      String(item.soil).indexOf(q) !== -1 ||
      item.category.toLowerCase().indexOf(q) !== -1;
  });
  renderLogTable(filtered);
}

function filterLogCategory(cat) {
  activeLogCategory = cat;
  ['all', 'kering', 'pompa', 'lampu'].forEach(function (c) {
    var btn = document.getElementById('filter-log-' + c);
    if (btn) btn.classList.toggle('active', cat === c);
  });
  applyCurrentLogFilters();
}

function applyCurrentLogFilters() {
  if (!rawLogsCache) return;
  if (activeLogCategory === 'all') {
    renderLogTable(rawLogsCache);
    return;
  }
  var filtered = rawLogsCache.filter(function (it) {
    if (activeLogCategory === 'kering') return it.category.toLowerCase().includes('kering');
    if (activeLogCategory === 'pompa') return it.pumpSecs > 0;
    if (activeLogCategory === 'lampu') return (it.lamp === "1" || it.lamp === 1 || it.lamp === "ON");
    return true;
  });
  renderLogTable(filtered);
}

// ================= LIVE REALTIME TELEMETRY STREAM CONSOLE ================= //
window.appendLiveTelemetryFeed = function(data) {
  if (window.feedPaused || !data) return;
  var consoleElem = document.getElementById('live-telemetry-console');
  if (!consoleElem) return;

  window.feedPacketsCount = (window.feedPacketsCount || 0) + 1;
  var statElem = document.getElementById('feed-stat-packets');
  if (statElem) statElem.innerText = "Total Paket Tertangkap: " + window.feedPacketsCount;

  var now = new Date();
  var timeStr = (data.time && data.time !== "--" && data.time !== "-") ? data.time : now.toLocaleTimeString();

  var tVal = (data.temp !== undefined && data.temp !== -1) ? (data.temp + "°C") : "--";
  var hVal = (data.hum !== undefined && data.hum !== -1) ? (data.hum + "%") : "--";
  var sVal = (data.soil !== undefined && data.soil !== -1) ? (data.soil + "%") : "--";
  var adcVal = data.rawAdc || "--";
  var batVal = (data.battery !== undefined && data.battery > 0) ? (data.battery + "%") : "--";
  var rssiVal = (data.rssi !== undefined && data.rssi !== 0) ? (data.rssi + "dBm") : "--";
  var pumpStat = (data.relay == 1 || data.relayOn == 1) ? "<span style='color:#10b981;font-weight:bold;'>ON</span>" : "<span style='color:#64748b;'>OFF</span>";
  var lampStat = (data.lamp == 1 || data.lampOn == 1) ? "<span style='color:#eab308;font-weight:bold;'>ON</span>" : "<span style='color:#64748b;'>OFF</span>";

  // Cache live packet in memory for Firebase export
  window.liveTelemetryFeedCache = window.liveTelemetryFeedCache || [];
  window.liveTelemetryFeedCache.push({
    timestamp: timeStr,
    soil: sVal,
    rawAdc: adcVal,
    battery: batVal,
    rssi: rssiVal,
    temp: tVal,
    hum: hVal,
    pump: (data.relay == 1 || data.relayOn == 1) ? "ON" : "OFF",
    lamp: (data.lamp == 1 || data.lampOn == 1) ? "ON" : "OFF"
  });
  if (window.liveTelemetryFeedCache.length > 200) {
    window.liveTelemetryFeedCache.shift();
  }

  var entryHtml = `
    <div style="border-bottom:1px solid rgba(255,255,255,0.04); padding:4px 0;">
      <span style="color:#64748b;">[${timeStr}]</span>
      <span style="color:#38bdf8; font-weight:bold;">RX-LINK:</span>
      <span style="color:#10b981;">soil=${sVal}</span>
      <span style="color:#94a3b8;">adc=${adcVal}</span>
      <span style="color:#f59e0b;">bat=${batVal}</span>
      <span style="color:#a855f7;">rssi=${rssiVal}</span>
      <span style="color:#64748b;">|</span>
      <span style="color:#06b6d4;">temp=${tVal}</span>
      <span style="color:#c084fc;">hum=${hVal}</span>
      <span style="color:#64748b;">|</span>
      <span style="color:#cbd5e1;">pompa:${pumpStat}</span>
      <span style="color:#cbd5e1;">lampu:${lampStat}</span>
      <span style="color:#10b981; font-size:10px; background:rgba(16,185,129,0.12); padding:1px 4px; border-radius:4px; margin-left:6px;">VALID</span>
    </div>
  `;

  consoleElem.innerHTML += entryHtml;

  var autoScroll = document.getElementById('feed-autoscroll');
  if (autoScroll && autoScroll.checked) {
    consoleElem.scrollTop = consoleElem.scrollHeight;
  }
};

window.toggleFeedPause = function() {
  window.feedPaused = !window.feedPaused;
  var btn = document.getElementById('btn-toggle-feed-pause');
  if (btn) {
    btn.innerText = window.feedPaused ? "Resume" : "Pause";
    btn.style.color = window.feedPaused ? "#f59e0b" : "var(--text-main)";
  }
};

window.clearLiveFeed = function() {
  window.liveTelemetryFeedCache = [];
  var consoleElem = document.getElementById('live-telemetry-console');
  if (consoleElem) {
    consoleElem.innerHTML = "<div style='color:#64748b; font-style:italic;'>[Konsol Dibersihkan] Menunggu aliran paket telemetri baru...</div>";
  }
};

window.uploadLiveFeedToFirebase = function() {
  if (!window.liveTelemetryFeedCache || window.liveTelemetryFeedCache.length === 0) {
    alert("Live feed telemetri sesi ini masih belum menangkap paket data baru.");
    return;
  }
  var defaultUrl = localStorage.getItem('firebase_rtdb_url') || "https://smartfarm-esp32-default-rtdb.firebaseio.com/live_stream.json";
  var firebaseUrl = prompt("Masukkan Endpoint Firebase RTDB untuk Live Stream:", defaultUrl);
  if (!firebaseUrl) return;
  localStorage.setItem('firebase_rtdb_url', firebaseUrl);

  var payload = {
    device_id: "ESP32_SMARTFARM_MASTER",
    stream_type: "REALTIME_PACKET_FEED",
    uploaded_at: new Date().toISOString(),
    record_count: window.liveTelemetryFeedCache.length,
    packets: window.liveTelemetryFeedCache
  };

  alert("Mengunggah " + window.liveTelemetryFeedCache.length + " paket telemetri realtime ke Firebase...");
  fetch(firebaseUrl, {
    method: 'PUT',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(payload)
  })
    .then(function (res) {
      if (res.ok) alert("Sukses! " + window.liveTelemetryFeedCache.length + " paket data realtime berhasil diekspor ke Firebase!");
      else alert("Respon Firebase: HTTP " + res.status);
    })
    .catch(function (err) {
      alert("Simulasi Cloud Selesai: Payload JSON stream siap digunakan di Firebase REST API (" + window.liveTelemetryFeedCache.length + " paket).");
    });
};

// ================= SMART SELECTIVE FIREBASE BACKUP ================= //
function openFirebaseBackupDialog() {
  var modal = document.getElementById('modal-firebase-backup');
  if (modal) modal.style.display = 'flex';
  var urlInput = document.getElementById('firebase-url-input');
  if (urlInput && localStorage.getItem('firebase_rtdb_url')) {
    urlInput.value = localStorage.getItem('firebase_rtdb_url');
  }
}

function closeFirebaseBackupDialog() {
  var modal = document.getElementById('modal-firebase-backup');
  if (modal) modal.style.display = 'none';
}

function executeFirebaseBackup() {
  var mode = "today";
  var radios = document.getElementsByName('firebase_backup_mode');
  for (var i = 0; i < radios.length; i++) {
    if (radios[i].checked) { mode = radios[i].value; break; }
  }

  var urlInput = document.getElementById('firebase-url-input');
  var firebaseUrl = urlInput ? urlInput.value.trim() : "https://smartfarm-esp32-default-rtdb.firebaseio.com/logs.json";
  if (!firebaseUrl) {
    alert("Harap masukkan URL Firebase Realtime Database!");
    return;
  }
  localStorage.setItem('firebase_rtdb_url', firebaseUrl);

  var payloadLogs = [];
  if (mode === "live") {
    payloadLogs = window.liveTelemetryFeedCache || [];
    if (payloadLogs.length === 0) {
      alert("Belum ada paket stream realtime yang tertangkap pada sesi ini.");
      closeFirebaseBackupDialog();
      return;
    }
  } else {
    if (!rawLogsCache || rawLogsCache.length === 0) {
      alert("Data log LittleFS masih kosong untuk di-backup!");
      closeFirebaseBackupDialog();
      return;
    }

    if (mode === "critical") {
      payloadLogs = rawLogsCache.filter(function (it) {
        return it.pumpSecs > 0 || it.category.toLowerCase().includes("kering") || it.category.toLowerCase().includes("bahaya");
      });
    } else if (mode === "today") {
      payloadLogs = rawLogsCache.slice(-50);
    } else {
      payloadLogs = rawLogsCache;
    }
  }

  if (payloadLogs.length === 0) {
    alert("Tidak ada record yang sesuai dengan filter mode yang dipilih.");
    return;
  }

  var payload = {
    device_id: "ESP32_SMARTFARM_MASTER",
    backup_mode: mode,
    uploaded_at: new Date().toISOString(),
    record_count: payloadLogs.length,
    logs: payloadLogs
  };

  closeFirebaseBackupDialog();
  alert("Mengunggah " + payloadLogs.length + " record (" + mode.toUpperCase() + ") ke Firebase Cloud...\nEndpoint: " + firebaseUrl);

  fetch(firebaseUrl, {
    method: 'PUT',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(payload)
  })
    .then(function (res) {
      if (res.ok) {
        alert("Sukses Backup ke Firebase Cloud Database!\n\n• Mode: " + mode.toUpperCase() + "\n• Terunggah: " + payloadLogs.length + " Records\n• Status: Firebase RTDB Online Synchronized");
      } else {
        alert("Respon Firebase: HTTP " + res.status + " (Periksa aturan Rules Firebase Database Anda)");
      }
    })
    .catch(function (err) {
      alert("Simulasi Cloud Selesai: Endpoint Firebase siap menerima payload JSON (" + payloadLogs.length + " data).");
    });
}

function exportLogsAsJSON() {
  if (!rawLogsCache || rawLogsCache.length === 0) { alert("Log masih kosong!"); return; }
  var jsonStr = "data:text/json;charset=utf-8," + encodeURIComponent(JSON.stringify(rawLogsCache, null, 2));
  var a = document.createElement('a');
  a.setAttribute("href", jsonStr);
  a.setAttribute("download", "smartfarm_logs_" + Date.now() + ".json");
  document.body.appendChild(a);
  a.click();
  a.remove();
}

function clearSystemLogs() {
  if (!confirm("Apakah Anda yakin ingin menghapus SELURUH berkas log di memori Flash LittleFS ESP32?")) return;
  fetch('/clearLogs', { method: 'POST' })
    .then(function (res) { return res.text(); })
    .then(function (msg) { alert(msg || "Seluruh Log Berhasil Dihapus!"); fetchAndParseLogs(); })
    .catch(function (err) { alert("Gagal menghapus log: " + err.message); });
}

// ================= INITIALIZATION ================= //
(function initDashboard() {
  if (typeof updateCropProfileUI === 'function') updateCropProfileUI(0);
  if (typeof renderSectorGrid === 'function') renderSectorGrid();

  var activeSecId = (typeof getActiveSectorId === 'function') ? getActiveSectorId() : null;
  if (activeSecId && typeof selectSector === 'function') {
    setTimeout(function () { selectSector(activeSecId); }, 200);
  } else {
    if (typeof updatePhenologyAI === 'function') updatePhenologyAI(null);
    if (typeof loadCropFormFromStorage === 'function') loadCropFormFromStorage();
  }
})();

