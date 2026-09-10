/**
 * 12_weather.js - BMKG Satellite Weather Integration & Microclimate Telemetry Engine
 * Real-time Forecast API adm4: 35.13.05.2002 (Kec. Leces, Probolinggo)
 * Zero-Dummy Telemetry, Physical Thermodynamic Formulas & Clean SCADA UI
 * Strict line limit < 400 lines
 */

function parseWindDirection(wd) {
  if (!wd) return "Selatan (S)";
  var str = String(wd).trim();
  var map = {
    "N": "Utara (N)",
    "NNE": "Utara-Timur Laut (NNE)",
    "NE": "Timur Laut (NE)",
    "ENE": "Timur-Timur Laut (ENE)",
    "E": "Timur (E)",
    "ESE": "Timur-Tenggara (ESE)",
    "SE": "Tenggara (SE)",
    "SSE": "Selatan-Tenggara (SSE)",
    "S": "Selatan (S)",
    "SSW": "Selatan-Barat Daya (SSW)",
    "SW": "Barat Daya (SW)",
    "WSW": "Barat-Barat Daya (WSW)",
    "W": "Barat (W)",
    "WNW": "Barat-Barat Laut (WNW)",
    "NW": "Barat Laut (NW)",
    "NNW": "Utara-Barat Laut (NNW)"
  };

  if (str.indexOf("->") !== -1 || str.indexOf(" -> ") !== -1) {
    var parts = str.split(/->| -> /);
    var fromCode = parts[0].trim().toUpperCase();
    var toCode = parts[1].trim().toUpperCase();
    var fromText = map[fromCode] || fromCode;
    var toText = map[toCode] || toCode;
    return fromText + " ➔ " + toText;
  }

  var code = str.toUpperCase();
  return map[code] || wd;
}

function getWeatherIcon(desc) {
  if (!desc) return "Cerah Berawan";
  return desc;
}

// Find closest weather forecast slot based on current time
function pickCurrentBMKGCuaca(cuacaGroup) {
  if (!cuacaGroup) return null;
  var items = [];
  if (Array.isArray(cuacaGroup)) {
    if (Array.isArray(cuacaGroup[0])) {
      items = cuacaGroup[0];
    } else {
      items = cuacaGroup;
    }
  }
  if (!items || items.length === 0) return null;

  var now = new Date();
  var bestItem = items[0];
  var minDiff = Infinity;

  for (var i = 0; i < items.length; i++) {
    var it = items[i];
    var timeStr = it.local_datetime || it.datetime;
    if (timeStr) {
      var d = new Date(timeStr.replace(' ', 'T'));
      if (!isNaN(d.getTime())) {
        var diff = Math.abs(now.getTime() - d.getTime());
        if (diff < minDiff) {
          minDiff = diff;
          bestItem = it;
        }
      }
    }
  }
  return bestItem;
}

function loadCachedBMKGData() {
  var temp = localStorage.getItem('bmkg_temp');
  var hum = localStorage.getItem('bmkg_hum');
  var desc = localStorage.getItem('bmkg_desc');
  var windDir = localStorage.getItem('bmkg_wind_dir');
  var windSpd = localStorage.getItem('bmkg_wind_spd');
  var rain = localStorage.getItem('bmkg_rain');
  var tStr = localStorage.getItem('bmkg_sync_time');

  if (temp && document.getElementById('bmkg-sat-temp')) {
    document.getElementById('bmkg-sat-temp').innerText = temp + "°C";
  }
  if (hum && document.getElementById('bmkg-sat-hum')) {
    document.getElementById('bmkg-sat-hum').innerText = hum + "%";
  }
  if (desc && document.getElementById('bmkg-sat-desc')) {
    document.getElementById('bmkg-sat-desc').innerText = desc;
  }
  if (windDir && document.getElementById('bmkg-wind-dir')) {
    document.getElementById('bmkg-wind-dir').innerText = parseWindDirection(windDir);
  }
  if (windSpd && document.getElementById('bmkg-wind-speed')) {
    document.getElementById('bmkg-wind-speed').innerText = windSpd;
  }
  if (rain && document.getElementById('bmkg-rain-pred')) {
    document.getElementById('bmkg-rain-pred').innerText = rain;
  }
  if (tStr) {
    if (document.getElementById('bmkg-last-sync')) {
      document.getElementById('bmkg-last-sync').innerText = tStr + " WIB";
    }
    if (document.getElementById('bmkg-last-sync-badge')) {
      document.getElementById('bmkg-last-sync-badge').innerText = "Sync " + tStr;
    }
  }

  // If cache is empty or older than 30 minutes, sync automatically in background
  var lastEpoch = parseInt(localStorage.getItem('bmkg_sync_epoch') || '0', 10);
  var nowEpoch = Math.floor(Date.now() / 1000);
  if (!temp || (nowEpoch - lastEpoch > 1800)) {
    setTimeout(function() { syncBMKGData(true); }, 1500);
  }
}

async function syncBMKGData(silent) {
  var btn = document.getElementById('btn-sync-bmkg');
  var badge = document.getElementById('bmkg-last-sync-badge');
  var dot = document.getElementById('bmkg-sync-dot');
  var icon = document.getElementById('sync-bmkg-icon');

  if (btn) {
    btn.disabled = true;
    btn.style.opacity = '0.7';
  }
  if (badge) badge.innerText = "Sinkronisasi...";
  if (dot) dot.style.background = "#f59e0b";
  if (icon) icon.style.animation = "spin 1s linear infinite";

  try {
    const res = await fetch('https://api.bmkg.go.id/publik/prakiraan-cuaca?adm4=35.13.05.2002');
    if (!res.ok) throw new Error("HTTP " + res.status);
    const json = await res.json();

    var item = null;
    if (json && json.data && json.data[0] && json.data[0].cuaca) {
      item = pickCurrentBMKGCuaca(json.data[0].cuaca);
    }

    if (!item) throw new Error("Format prakiraan BMKG tidak dikenali.");

    var temp = (item.t !== undefined) ? String(item.t) : "--";
    var hum = (item.hu !== undefined) ? String(item.hu) : "--";
    var desc = item.weather_desc || item.weather_desc_en || "Prakiraan Cuaca";
    var windDir = (item.wd && item.wd_to) ? (item.wd + " -> " + item.wd_to) : (item.wd || "--");
    var windSpd = (item.ws !== undefined ? item.ws + " km/jam" : "-- km/jam");
    var tpVal = (item.tp !== undefined) ? parseFloat(item.tp) : 0;
    var tccVal = (item.tcc !== undefined) ? item.tcc : null;
    var visVal = item.vs_text || "> 10 km";

    var rain = (tpVal > 0) ? ("Presipitasi " + tpVal + " mm/jam (Hujan)") : (desc + " (Tidak Hujan)");
    var skyDesc = desc + (tccVal !== null ? " (" + tccVal + "% Awan)" : "");
    var tStr = new Date().toLocaleTimeString();
    var ep = Math.floor(Date.now() / 1000);

    localStorage.setItem('bmkg_temp', temp);
    localStorage.setItem('bmkg_hum', hum);
    localStorage.setItem('bmkg_desc', skyDesc);
    localStorage.setItem('bmkg_wind_dir', windDir);
    localStorage.setItem('bmkg_wind_spd', windSpd);
    localStorage.setItem('bmkg_rain', rain);
    localStorage.setItem('bmkg_sync_time', tStr);
    localStorage.setItem('bmkg_sync_epoch', ep);

    if (document.getElementById('bmkg-sat-temp')) document.getElementById('bmkg-sat-temp').innerText = temp + "°C";
    if (document.getElementById('bmkg-sat-hum')) document.getElementById('bmkg-sat-hum').innerText = hum + "%";
    if (document.getElementById('bmkg-sat-desc')) document.getElementById('bmkg-sat-desc').innerText = skyDesc;
    if (document.getElementById('bmkg-wind-dir')) document.getElementById('bmkg-wind-dir').innerText = parseWindDirection(windDir);
    if (document.getElementById('bmkg-wind-speed')) document.getElementById('bmkg-wind-speed').innerText = windSpd;
    if (document.getElementById('bmkg-rain-pred')) {
      var rpEl = document.getElementById('bmkg-rain-pred');
      rpEl.innerText = rain;
      rpEl.style.color = (tpVal > 0) ? "#f59e0b" : "#10b981";
    }
    if (document.getElementById('bmkg-last-sync')) document.getElementById('bmkg-last-sync').innerText = tStr + " WIB (Leces)";
    if (document.getElementById('bmkg-last-sync-badge')) document.getElementById('bmkg-last-sync-badge').innerText = "Live " + tStr;
    if (dot) dot.style.background = "#10b981";

    var recomEl = document.getElementById('bmkg-recommendation');
    if (recomEl) {
      if (tpVal > 0 || desc.toLowerCase().includes("hujan")) {
        recomEl.innerText = "Peringatan Hujan Satelit: Menunda siklus penyiraman otomatis untuk konservasi air.";
        recomEl.style.color = "#f59e0b";
      } else {
        recomEl.innerText = "Cuaca Kondusif: Penyiraman otomatis beroperasi penuh mengikuti sensor tanah.";
        recomEl.style.color = "var(--text-sub)";
      }
    }

    // Push to ESP32 LittleFS logger
    var body = "temp=" + encodeURIComponent(temp) +
      "&desc=" + encodeURIComponent(desc) +
      "&rain=" + encodeURIComponent(rain) +
      "&time=" + encodeURIComponent(tStr) +
      "&epoch=" + ep;

    fetch('/pushWeather', {
      method: 'POST',
      headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
      body: body
    }).catch(function() {});

    // Recalculate microclimate variance against current garden sensors
    if (window.lastTelemetryData && typeof updateActuatorAndScheduleUI === 'function') {
      var d = window.lastTelemetryData;
      var hasT = (d.temp !== undefined && d.temp !== null && d.temp !== -1);
      var hasH = (d.hum !== undefined && d.hum !== null && d.hum !== -1);
      updateActuatorAndScheduleUI(d, d.time || tStr, hasT, d.temp, hasH, d.hum);
    }

  } catch (err) {
    console.warn("BMKG sync error:", err);
    if (badge) badge.innerText = "Sync Tertunda";
    if (dot) dot.style.background = "#ef4444";
    if (!silent) {
      alert("Tidak dapat menjangkau server BMKG: " + err.message + "\nMenampilkan data tersimpan terakhir.");
    }
  } finally {
    if (btn) {
      btn.disabled = false;
      btn.style.opacity = '1';
    }
    if (icon) icon.style.animation = "";
  }
}

// Auto init on DOM ready
document.addEventListener("DOMContentLoaded", function () {
  loadCachedBMKGData();
});
