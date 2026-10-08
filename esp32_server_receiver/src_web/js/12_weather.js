/**
 * 12_weather.js - BMKG Satellite Weather Integration & Microclimate Telemetry Engine
 * Real-time Forecast API adm4: 35.13.05.2002 (Kec. Leces, Probolinggo)
 * Fallback to Open-Meteo Satellite & Offline Local Cache
 * Zero-Dummy Telemetry, Physical Thermodynamic Formulas & Clean SCADA UI
 * Strict line limit < 400 lines
 */

function parseWindDirection(wd) {
  if (!wd) return "Selatan (S)";
  var str = String(wd).trim();
  var map = {
    "N": "Utara (N)", "NNE": "Utara-Timur Laut (NNE)", "NE": "Timur Laut (NE)",
    "ENE": "Timur-Timur Laut (ENE)", "E": "Timur (E)", "ESE": "Timur-Tenggara (ESE)",
    "SE": "Tenggara (SE)", "SSE": "Selatan-Tenggara (SSE)", "S": "Selatan (S)",
    "SSW": "Selatan-Barat Daya (SSW)", "SW": "Barat Daya (SW)", "WSW": "Barat-Barat Daya (WSW)",
    "W": "Barat (W)", "WNW": "Barat-Barat Laut (WNW)", "NW": "Barat Laut (NW)", "NNW": "Utara-Barat Laut (NNW)"
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

function fetchWithTimeout(url, timeoutMs) {
  timeoutMs = timeoutMs || 4500;
  if (typeof AbortController === 'undefined') {
    return fetch(url);
  }
  var controller = new AbortController();
  var timer = setTimeout(function () { controller.abort(); }, timeoutMs);
  return fetch(url, { signal: controller.signal }).finally(function () {
    clearTimeout(timer);
  });
}

function parseOpenMeteoPayload(json) {
  if (!json || !json.current) return null;
  var c = json.current;
  var code = c.weather_code || 0;
  var desc = "Cerah Berawan";
  if (code === 0) desc = "Cerah";
  else if (code === 1 || code === 2) desc = "Cerah Berawan";
  else if (code === 3) desc = "Berawan";
  else if (code === 45 || code === 48) desc = "Berkabut";
  else if (code >= 51 && code <= 55) desc = "Gerimis";
  else if (code >= 61 && code <= 65) desc = "Hujan";
  else if (code >= 80 && code <= 82) desc = "Hujan Lebat";
  else if (code >= 95) desc = "Hujan Petir";

  var dirs = ["Utara (N)", "Timur Laut (NE)", "Timur (E)", "Tenggara (SE)", "Selatan (S)", "Barat Daya (SW)", "Barat (W)", "Barat Laut (NW)"];
  var dirDeg = c.wind_direction_10m !== undefined ? c.wind_direction_10m : 180;
  var dirIdx = Math.round((dirDeg % 360) / 45) % 8;
  var windDirStr = dirs[dirIdx];

  return {
    t: c.temperature_2m !== undefined ? String(Math.round(c.temperature_2m * 10) / 10) : "--",
    hu: c.relative_humidity_2m !== undefined ? String(Math.round(c.relative_humidity_2m)) : "--",
    weather_desc: desc,
    wd: windDirStr,
    wd_to: "",
    ws: c.wind_speed_10m !== undefined ? Math.round(c.wind_speed_10m) + " km/jam" : "-- km/jam",
    tp: c.precipitation !== undefined ? c.precipitation : 0,
    tcc: null,
    source: "Open-Meteo"
  };
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

  // Segera perbarui kartu mikroklimat turunan (Dew Point, VPD, Heat Index) dari cache
  if (typeof updateActuatorAndScheduleUI === 'function') {
    var d = window.lastTelemetryData || {};
    var rawT = (d.suhuC !== undefined && d.suhuC !== null && d.suhuC !== "--") ? d.suhuC : d.temp;
    var hasT = (rawT !== undefined && rawT !== null && rawT !== -1 && rawT !== "--" && !isNaN(parseFloat(rawT)));
    var hasH = (d.hum !== undefined && d.hum !== null && d.hum !== -1 && d.hum !== "--" && !isNaN(parseFloat(d.hum)));
    updateActuatorAndScheduleUI(d, d.time || tStr || "--:--", hasT, rawT, hasH, d.hum);
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

  var parsedWeather = null;
  var providerName = "BMKG";

  // 1. Coba Sumber Utama: API Resmi BMKG
  try {
    var res = await fetchWithTimeout('https://api.bmkg.go.id/publik/prakiraan-cuaca?adm4=35.13.05.2002', 4500);
    if (!res.ok) throw new Error("HTTP " + res.status);
    var json = await res.json();
    if (json && json.data && json.data[0] && json.data[0].cuaca) {
      var item = pickCurrentBMKGCuaca(json.data[0].cuaca);
      if (item) {
        parsedWeather = {
          t: (item.t !== undefined) ? String(item.t) : "--",
          hu: (item.hu !== undefined) ? String(item.hu) : "--",
          weather_desc: item.weather_desc || item.weather_desc_en || "Prakiraan Cuaca",
          wd: (item.wd && item.wd_to) ? (item.wd + " -> " + item.wd_to) : (item.wd || "--"),
          ws: (item.ws !== undefined ? item.ws + " km/jam" : "-- km/jam"),
          tp: (item.tp !== undefined) ? parseFloat(item.tp) : 0,
          tcc: (item.tcc !== undefined) ? item.tcc : null,
          source: "BMKG"
        };
      }
    }
  } catch (errBMKG) {
    console.warn("Server BMKG utama tidak merespon, mencoba satelit Open-Meteo fallback...", errBMKG);
  }

  // 2. Fallback Otomatis: Satelit Open-Meteo (Kec. Leces, Probolinggo: -7.8806, 113.2331)
  if (!parsedWeather) {
    try {
      var urlMeteo = 'https://api.open-meteo.com/v1/forecast?latitude=-7.8806&longitude=113.2331&current=temperature_2m,relative_humidity_2m,precipitation,weather_code,wind_speed_10m,wind_direction_10m&timezone=Asia%2FJakarta';
      var resMeteo = await fetchWithTimeout(urlMeteo, 4500);
      if (resMeteo.ok) {
        var jsonMeteo = await resMeteo.json();
        parsedWeather = parseOpenMeteoPayload(jsonMeteo);
        if (parsedWeather) providerName = "Satelit";
      }
    } catch (errMeteo) {
      console.warn("Satelit Open-Meteo fallback juga gagal (perangkat kemungkinan offline):", errMeteo);
    }
  }

  // 3. Jika Berhasil Mendapatkan Data Cuaca
  if (parsedWeather) {
    try {
      var temp = parsedWeather.t;
      var hum = parsedWeather.hu;
      var desc = parsedWeather.weather_desc;
      var windDir = parsedWeather.wd;
      var windSpd = parsedWeather.ws;
      var tpVal = parsedWeather.tp;
      var tccVal = parsedWeather.tcc;

      var rain = (tpVal > 0) ? ("Presipitasi " + tpVal + " mm/jam (Hujan)") : (desc + " (Tidak Hujan)");
      var skyDesc = desc + (tccVal !== null && tccVal !== undefined ? " (" + tccVal + "% Awan)" : "");
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
      if (document.getElementById('bmkg-last-sync')) document.getElementById('bmkg-last-sync').innerText = tStr + " WIB (" + providerName + ")";
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
      if (typeof updateActuatorAndScheduleUI === 'function') {
        var d = window.lastTelemetryData || {};
        var rawT = (d.suhuC !== undefined && d.suhuC !== null && d.suhuC !== "--") ? d.suhuC : d.temp;
        var hasT = (rawT !== undefined && rawT !== null && rawT !== -1 && rawT !== "--" && !isNaN(parseFloat(rawT)));
        var hasH = (d.hum !== undefined && d.hum !== null && d.hum !== -1 && d.hum !== "--" && !isNaN(parseFloat(d.hum)));
        updateActuatorAndScheduleUI(d, d.time || tStr || "--:--", hasT, rawT, hasH, d.hum);
      }
    } catch (renderErr) {
      console.warn("Error rendering weather data:", renderErr);
    }
  } else {
    // 4. Kasus Keduanya Gagal (Perangkat Offline / Hanya terkoneksi ke Access Point Lokal ESP32)
    console.info("Sinkronisasi cuaca satelit tertunda: Perangkat dalam mode offline (WiFi lokal ESP32).");
    if (badge) badge.innerText = "Mode Offline (Cache)";
    if (dot) dot.style.background = "#f59e0b";
    
    // Tampilkan data tersimpan terakhir jika ada
    loadCachedBMKGData();

    if (!silent) {
      alert("ℹ️ Mode Offline (Jaringan Lokal ESP32)\n\n" +
            "Perangkat Anda saat ini terhubung langsung ke WiFi AP lokal ESP32 ('SmartFarm-ESP32') tanpa kuota internet luar, sehingga server satelit BMKG tidak dapat dijangkau saat ini.\n\n" +
            "✅ Dashboard tetap berjalan normal dengan menampilkan rekaman cuaca satelit terakhir yang tersimpan di memori.");
    }
  }

  if (btn) {
    btn.disabled = false;
    btn.style.opacity = '1';
  }
  if (icon) icon.style.animation = "";
}

// Auto init on DOM ready
document.addEventListener("DOMContentLoaded", function () {
  loadCachedBMKGData();
});
