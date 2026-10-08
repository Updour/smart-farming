// =================================================================
// 03_TELEMETRY.JS - HARDWARE TELEMETRY POLLING & INTEGRITY AUDITOR
// Target lines: ~220 (Max < 350)
// =================================================================

var isSensorDataValid = false;
var latestMoisturePercent = null;

function formatFriendlyDateTime(rawStr) {
  if (!rawStr || rawStr === "-" || rawStr.indexOf(" ") === -1) return rawStr || "-";
  var parts = rawStr.split(" ");
  var dParts = parts[0].split("-");
  var timeStr = parts[1];
  if (dParts.length !== 3) return rawStr;
  var months = ["Jan", "Feb", "Mar", "Apr", "Mei", "Jun", "Jul", "Ags", "Sep", "Okt", "Nov", "Des"];
  var day = parseInt(dParts[2]);
  var monthIdx = parseInt(dParts[1]) - 1;
  var year = dParts[0];
  var monthName = months[monthIdx] || dParts[1];
  return day + " " + monthName + " " + year + " — " + timeStr + " WIB";
}

function formatWaitSecs(sec) {
  if (sec === undefined || sec === null) return "0s";
  var s = parseInt(sec, 10) || 0;
  if (s < 60) return s + "s";
  var m = Math.floor(s / 60);
  var remS = s % 60;
  return m + "m " + (remS < 10 ? "0" : "") + remS + "s";
}

var _consecutiveFailures = 0;

function applyTelemetryData(data) {
  if (!data || typeof data !== 'object') return;
  window.lastTelemetryData = data;

  var bannerBox = document.getElementById('banner-box');
  var bannerText = document.getElementById('banner-text');

  var color = "var(--primary)";
  var glow = "rgba(16, 185, 129, 0.7)";
  if (data.statusColor === "bahaya") {
    color = "var(--danger)";
    glow = "rgba(239, 68, 68, 0.7)";
  } else if (data.statusColor === "peringatan") {
    color = "var(--warning)";
    glow = "rgba(245, 158, 11, 0.7)";
  }

  var hasSoil = (data.soil !== undefined && data.soil !== null && data.soil !== "--" && !isNaN(parseFloat(data.soil)) && parseFloat(data.soil) >= 0);
  var hasTemp = (data.suhuC !== undefined && data.suhuC !== null && data.suhuC !== "--" && !isNaN(parseFloat(data.suhuC)));
  var hasHum = (data.hum !== undefined && data.hum !== null && data.hum !== "--" && !isNaN(parseFloat(data.hum)));

  var sVal = hasSoil ? parseFloat(data.soil) : null;
  var tVal = hasTemp ? parseFloat(data.suhuC) : null;
  var hVal = hasHum ? parseFloat(data.hum) : null;

  // Jika ada data tanah valid (hasSoil), sistem TIDAK LAGI dalam kondisi menunggu
  var isWaiting = (data.isWaiting == 1 && !hasSoil);
  var isOfflineOrWaiting = (isWaiting || (data.isEsp8266Unplugged == 1 && !hasSoil) || data.isSystemError == 1);

  var pct = (data.waitingPercent !== undefined) ? data.waitingPercent : 0;
  var wSec = (data.waitingSecs !== undefined) ? data.waitingSecs : 0;
  var waitTimeStr = pct + "% (" + wSec + "s / 60s)";
  if (isWaiting) {
    if (bannerText) bannerText.innerText = "⏳ MEMUAT SISTEM: Menunggu Sinyal ESP8266 (" + waitTimeStr + ")";
    color = "var(--warning)";
    glow = "rgba(245, 158, 11, 0.7)";
  } else if (data.isEsp8266Unplugged == 1 && !hasSoil) {
    if (bannerText) {
      if (data.errorMsg && data.errorMsg !== "") {
        bannerText.innerText = data.errorMsg;
      } else {
        bannerText.innerText = "🚨 PERINGATAN: SENSOR TERPUTUS (Sinyal Hilang - Pompa Dikunci MATI)";
      }
    }
    color = "var(--danger)";
    glow = "rgba(239, 68, 68, 0.7)";
  } else if (data.relayOn == 1) {
    var modeStr = (data.isManual == 1 ? "MANUAL" : "AUTO");
    var soilStr = hasSoil ? " | Kelembapan: " + sVal + "%" : "";
    if (bannerText) bannerText.innerText = "💧 SEDANG MENYIRAM LAHAN (" + modeStr + ")" + soilStr;
    color = "var(--primary)";
    glow = "rgba(16, 185, 129, 0.7)";
  } else if (data.errorMsg && data.errorMsg !== "" && !hasSoil) {
    if (bannerText) bannerText.innerText = "Failsafe Alert: " + data.errorMsg;
    color = "var(--danger)";
    glow = "rgba(239, 68, 68, 0.7)";
  } else {
    var cleanStatus = data.statusText ? data.statusText.replace(/[✅🚨⏳ℹ️🌱]/g, '').trim() : "Sistem Operasional Normal";
    if (bannerText) bannerText.innerText = cleanStatus || "Sistem Operasional Normal";
  }

  if (bannerBox) {
    bannerBox.style.borderTopColor = color;
    bannerBox.style.boxShadow = "0 10px 30px rgba(0, 0, 0, 0.2), 0 0 15px " + glow;
  }
  if (bannerText) {
    bannerText.style.color = color;
    bannerText.style.borderColor = color;
  }

  var friendlyRtc = formatFriendlyDateTime(data.rtcTime);
  var badgeRtc = document.getElementById('badge-rtc-time');
  if (badgeRtc) {
    badgeRtc.innerHTML = '<svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" style="vertical-align:middle;margin-right:4px;"><circle cx="12" cy="12" r="10"></circle><polyline points="12 6 12 12 16 14"></polyline></svg><span>' + (friendlyRtc !== "-" ? friendlyRtc : "RTC: Offline") + '</span>';
  }
  var badgeRssi = document.getElementById('badge-rssi');
  if (badgeRssi) {
    badgeRssi.innerHTML = '<svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" style="vertical-align:middle;margin-right:4px;"><path d="M5 12.55a11 11 0 0 1 14.08 0"></path><path d="M1.42 9a16 16 0 0 1 21.16 0"></path><path d="M8.53 16.11a6 6 0 0 1 6.95 0"></path><line x1="12" y1="20" x2="12.01" y2="20"></line></svg><span>Sinyal: ' + (data.rssi || "-") + ' dBm</span>';
  }

  latestMoisturePercent = sVal;
  isSensorDataValid = (!isOfflineOrWaiting && hasSoil && hasTemp && hasHum);

  // Hardware Traffic Light Widget (Pins 32 Hijau, 33 Kuning, 27 Merah)
  var bRed = document.getElementById('traffic-bulb-red');
  var bYellow = document.getElementById('traffic-bulb-yellow');
  var bGreen = document.getElementById('traffic-bulb-green');
  var tLabel = document.getElementById('traffic-label');

  if (bRed && bYellow && bGreen) {
    bRed.className = 'traffic-bulb red';
    bYellow.className = 'traffic-bulb yellow';
    bGreen.className = 'traffic-bulb green';

    if (data.isSystemError == 1) {
      bRed.classList.add('active', 'blink');
      if (tLabel) { tLabel.innerText = "Darurat"; tLabel.style.color = "#ef4444"; }
    } else if (data.isEsp8266Unplugged == 1 && !hasSoil) {
      bYellow.classList.add('active');
      if (tLabel) { tLabel.innerText = "Offline"; tLabel.style.color = "#f59e0b"; }
    } else if (isWaiting) {
      bYellow.classList.add('active', 'blink');
      if (tLabel) { tLabel.innerText = "Menunggu"; tLabel.style.color = "#f59e0b"; }
    } else if (data.statusColor === "bahaya" || (hasTemp && tVal >= 35.0)) {
      bRed.classList.add('active');
      if (tLabel) { tLabel.innerText = "Bahaya"; tLabel.style.color = "#ef4444"; }
    } else if (data.statusColor === "peringatan" || (hasSoil && sVal < 45) || (hasTemp && tVal > 30.0)) {
      bYellow.classList.add('active');
      if (tLabel) { tLabel.innerText = "Waspada"; tLabel.style.color = "#f59e0b"; }
    } else if (hasSoil || hasTemp) {
      bGreen.classList.add('active');
      if (tLabel) { tLabel.innerText = "Aman"; tLabel.style.color = "#10b981"; }
    } else {
      bYellow.classList.add('active', 'blink');
      if (tLabel) { tLabel.innerText = "Standby"; tLabel.style.color = "#f59e0b"; }
    }
  }

  // Update Big Traffic Pole Card in Dashboard
  updateTrafficPoleCard(data, isOfflineOrWaiting, hasSoil, hasTemp, sVal, tVal);

  // KPI Cards Update
  var kpiSoil = document.getElementById('kpi-soil');
  var kpiSoilCat = document.getElementById('kpi-soil-category');
  var kpiSoilDep = document.getElementById('kpi-soil-depletion');
  if (kpiSoil) {
    if (hasSoil) {
      kpiSoil.innerText = sVal + "%";
      if (kpiSoilCat) {
        if (data.isEsp8266Unplugged == 1) {
          kpiSoilCat.innerText = (data.soilCategory || "Normal") + " (Offline)";
          kpiSoilCat.style.color = "var(--warning)";
        } else {
          kpiSoilCat.innerText = data.soilCategory || "Normal";
          kpiSoilCat.style.color = "var(--primary)";
        }
      }
    } else {
      kpiSoil.innerText = "--%";
      if (kpiSoilCat) {
        if (isWaiting) {
          kpiSoilCat.innerText = "Menunggu " + waitTimeStr;
          kpiSoilCat.style.color = "var(--warning)";
        } else {
          kpiSoilCat.innerText = "Sensor Terputus";
          kpiSoilCat.style.color = "var(--text-sub)";
        }
      }
    }
  }
  if (kpiSoilDep) {
    kpiSoilDep.innerText = isWaiting ? "Inisialisasi sinyal radio..." : ("Prediksi Penguapan: " + (data.soilDepletion || "-"));
  }

  var rawAdcVal = (data.rawAdc !== undefined && data.rawAdc !== null && data.rawAdc !== "-" && data.rawAdc !== "--") ? data.rawAdc : "--";
  var kpiSoilRawBadge = document.getElementById('kpi-soil-raw-badge');
  if (kpiSoilRawBadge) {
    kpiSoilRawBadge.innerText = isWaiting ? ("Sync: " + pct + "%") : ("Raw: " + rawAdcVal);
  }
  var kpiSoilRawAdc = document.getElementById('kpi-soil-raw-adc');
  if (kpiSoilRawAdc) {
    kpiSoilRawAdc.innerText = isWaiting ? ("Menunggu: " + pct + "% (" + wSec + "s)") : ("ADC A0: " + rawAdcVal);
  }

  var kpiTemp = document.getElementById('kpi-temp');
  var kpiTempCat = document.getElementById('kpi-temp-category');
  var kpiHeat = document.getElementById('kpi-heat-index');
  if (kpiTemp) {
    if (hasTemp) {
      kpiTemp.innerText = tVal + "°C";
      if (kpiTempCat) {
        kpiTempCat.innerText = (tVal >= 35.0 ? "Suhu Ekstrem" : (tVal > 30.0 ? "Cukup Hangat" : "Optimal"));
        kpiTempCat.style.color = (tVal >= 35.0 ? "var(--danger)" : (tVal > 30.0 ? "var(--warning)" : "var(--primary)"));
      }
    } else {
      kpiTemp.innerText = "--°C";
      if (kpiTempCat) {
        kpiTempCat.innerText = "Sensor Terputus";
        kpiTempCat.style.color = "var(--text-sub)";
      }
    }
  }
  if (kpiHeat) {
    kpiHeat.innerText = hasTemp ? ("Indeks Panas: " + (data.heatC || tVal) + "°C") : "Indeks Panas: --";
  }

  var kpiHum = document.getElementById('kpi-hum') || document.getElementById('kpi-humidity');
  var kpiHumCat = document.getElementById('kpi-hum-category');
  var kpiDew = document.getElementById('kpi-dew-point');
  if (kpiHum) {
    if (hasHum) {
      kpiHum.innerText = hVal + "%";
      if (kpiHumCat) {
        kpiHumCat.innerText = (hVal < 40 ? "Udara Kering" : (hVal > 85 ? "Sangat Lembap" : "Optimal"));
        kpiHumCat.style.color = (hVal < 40 || hVal > 85 ? "var(--warning)" : "var(--primary)");
      }
    } else {
      kpiHum.innerText = "--%";
      if (kpiHumCat) {
        kpiHumCat.innerText = "Sensor Terputus";
        kpiHumCat.style.color = "var(--text-sub)";
      }
    }
  }
  if (kpiDew) {
    if (hasHum && data.dew !== undefined && data.dew !== null && data.dew !== "--") {
      kpiDew.innerText = "Titik Embun: " + data.dew + "°C";
    } else {
      if (kpiDew) kpiDew.innerText = "Titik Embun: --";
    }
  }

  var kpiSignal = document.getElementById('kpi-signal') || document.getElementById('kpi-battery');
  var kpiNodeStatus = document.getElementById('kpi-node-status');
  var kpiAdc = document.getElementById('kpi-raw-adc');
  var kpiNodeBat = document.getElementById('kpi-node-battery-badge');
  var kpiPacketTimer = document.getElementById('kpi-packet-timer');

  var isNodeOnline = hasSoil || (!isOfflineOrWaiting && data.rssi !== undefined && data.rssi !== null && data.rssi != -99 && data.rssi !== "-");
  if (kpiSignal) {
    if (isNodeOnline) {
      var displayRssi = (data.rssi !== undefined && data.rssi !== null && data.rssi != -99 && data.rssi !== "-") ? data.rssi : "-55";
      kpiSignal.innerText = displayRssi + " dBm";
      if (kpiNodeStatus) {
        kpiNodeStatus.innerText = "Online";
        kpiNodeStatus.style.color = "var(--accent-emerald)";
        kpiNodeStatus.style.borderColor = "rgba(16, 185, 129, 0.35)";
      }
    } else {
      kpiSignal.innerText = "-- dBm";
      if (kpiNodeStatus) {
        if (isWaiting) {
          kpiNodeStatus.innerText = "Memuat (" + pct + "%)";
          kpiNodeStatus.style.color = "var(--warning)";
          kpiNodeStatus.style.borderColor = "rgba(245, 158, 11, 0.35)";
        } else {
          kpiNodeStatus.innerText = "Terputus";
          kpiNodeStatus.style.color = "var(--accent-rose)";
          kpiNodeStatus.style.borderColor = "rgba(239, 68, 68, 0.35)";
        }
      }
    }
  }

  if (kpiNodeBat) {
    var batVal = (data.battery !== undefined && data.battery > 0) ? data.battery : (data.nodeBat ? parseInt(data.nodeBat) : 0);
    if (isNodeOnline && batVal > 0) {
      kpiNodeBat.innerText = "🔋 " + batVal + "%";
      kpiNodeBat.style.display = "inline-block";
    } else {
      kpiNodeBat.innerText = "🔋 --%";
    }
  }

  if (kpiPacketTimer) {
    if (isNodeOnline && data.secSinceRecv !== undefined && data.secSinceRecv !== null) {
      kpiPacketTimer.innerText = "Paket: " + data.secSinceRecv + "s lalu";
      kpiPacketTimer.style.color = (data.secSinceRecv > 90) ? "var(--warning)" : "var(--text-sub)";
    } else {
      kpiPacketTimer.innerText = "Paket: -- lalu";
    }
  }

  if (kpiAdc) {
    if (isNodeOnline && data.rawAdc !== undefined && data.rawAdc !== null && data.rawAdc !== "-") {
      kpiAdc.innerText = "ADC A0: " + data.rawAdc;
    } else {
      kpiAdc.innerText = "ADC A0: --";
    }
  }

  // Update Disease Outbreak Risk State
  if (typeof updateOutbreakUIState === 'function') {
    updateOutbreakUIState(isSensorDataValid, tVal, hVal, sVal);
  }

  // Update AI Phenology Model
  if (typeof updatePhenologyAI === 'function' && typeof getActiveSector === 'function') {
    updatePhenologyAI(getActiveSector(), tVal, hVal, sVal, (data.vpd !== undefined ? parseFloat(data.vpd) : null));
  }

  // Delegate Actuators & Schedule synchronization
  if (typeof updateActuatorAndScheduleUI === 'function') {
    updateActuatorAndScheduleUI(data, friendlyRtc, hasTemp, tVal, hasHum, hVal);
  }
}

function fetchData() {
  if (window.location.protocol === 'file:') {
    return;
  }
  fetch('/data?_t=' + Date.now())
    .then(function (res) {
      if (!res.ok) throw new Error("HTTP error " + res.status);
      return res.json();
    })
    .then(function (data) {
      _consecutiveFailures = 0; // Berhasil! Reset counter kegagalan
      window.lastTelemetryData = data; // Simpan untuk dipakai modul lain (misal Fertigasi)

      // Simpan ke cache localStorage agar saat di-refresh halaman langsung stabil seketika
      try {
        localStorage.setItem('sf_telemetry_cache', JSON.stringify(data));
      } catch (e) {}

      if (typeof window.appendLiveTelemetryFeed === 'function') {
        window.appendLiveTelemetryFeed(data);
      }

      applyTelemetryData(data);

      // Silent Auto-Sync RTC once
      if (!window.rtcAutoSynced) {
        window.rtcAutoSynced = true;
        var now = new Date();
        var ep = Math.floor(now.getTime() / 1000);
        fetch('/setRtc?epoch=' + ep + '&y=' + now.getFullYear() + '&m=' + (now.getMonth() + 1) + '&d=' + now.getDate() + '&h=' + now.getHours() + '&min=' + now.getMinutes() + '&s=' + now.getSeconds())
          .catch(function () {});
      }

      var hasSoil = (data.soil !== undefined && data.soil !== null && data.soil !== "--" && !isNaN(parseFloat(data.soil)) && parseFloat(data.soil) >= 0);
      var rawT = (data.suhuC !== undefined && data.suhuC !== null && data.suhuC !== "--") ? data.suhuC : data.temp;
      var hasTemp = (rawT !== undefined && rawT !== null && rawT !== "--" && !isNaN(parseFloat(rawT)));
      var hasHum = (data.hum !== undefined && data.hum !== null && data.hum !== "--" && !isNaN(parseFloat(data.hum)));
      var sVal = hasSoil ? parseFloat(data.soil) : null;
      var tVal = hasTemp ? parseFloat(rawT) : null;
      var hVal = hasHum ? parseFloat(data.hum) : null;

      if (typeof updateHistory === 'function') {
        updateHistory(sVal, tVal, hVal);
      }
    })
    .catch(function (err) {
      _consecutiveFailures++;
      console.warn("Sinkronisasi data ESP32 (" + _consecutiveFailures + "):", err);

      // Jika baru gagal 1-2 kali (ESP32 sesaat sibuk/menutup soket HTTP sebelumnya), jangan ubah tampilan dashboard
      if (_consecutiveFailures < 3) {
        return;
      }

      isSensorDataValid = false;
      latestMoisturePercent = null;
      if (typeof updateOutbreakUIState === 'function') {
        updateOutbreakUIState(false, null, null, null);
      }
      var bText = document.getElementById('banner-text');
      if (bText) {
        bText.innerText = "⏳ Sedang Menyelaraskan Data Telemetri ESP32...";
        bText.style.color = "#f59e0b";
        bText.style.borderColor = "rgba(245, 158, 11, 0.4)";
      }
      var bBox = document.getElementById('banner-box');
      if (bBox) {
        bBox.style.borderTopColor = "var(--card-border)";
        bBox.style.boxShadow = "0 10px 30px rgba(0, 0, 0, 0.2)";
      }

      var bRed = document.getElementById('traffic-bulb-red');
      var bYellow = document.getElementById('traffic-bulb-yellow');
      var bGreen = document.getElementById('traffic-bulb-green');
      var tLabel = document.getElementById('traffic-label');
      if (bRed && bYellow && bGreen) {
        bYellow.className = 'traffic-bulb bulb-yellow active blink';
        bRed.className = 'traffic-bulb bulb-red';
        bGreen.className = 'traffic-bulb bulb-green';
        if (tLabel) { tLabel.innerText = "Sinkron..."; tLabel.style.color = "#f59e0b"; }
      }
    });
}

// Global Toggle for LED Diagnostic Card
window.toggleLedDiag = function() {
  var card = document.getElementById('card-traffic-light');
  if (card) {
    if (card.style.display === 'none') {
      card.style.display = 'block';
    } else {
      card.style.display = 'none';
    }
  }
};

// ================================================================
// updateTrafficPoleCard — Update big traffic light pole card
// Driven by: data.rtcSchedule, data.statusColor, sensor values
// ================================================================
var _rtcCycleInterval = null;

function updateTrafficPoleCard(data, isOfflineOrWaiting, hasSoil, hasTemp, sVal, tVal) {
  var pRed    = document.getElementById('pole-bulb-red');
  var pYellow = document.getElementById('pole-bulb-yellow');
  var pGreen  = document.getElementById('pole-bulb-green');
  var cardLabel  = document.getElementById('traffic-card-label');
  var curStatus  = document.getElementById('traffic-current-status');
  var curDetail  = document.getElementById('traffic-current-detail');
  var rtcBadge   = document.getElementById('traffic-rtc-badge');
  var dotRed     = document.getElementById('traffic-dot-red');
  var dotYellow  = document.getElementById('traffic-dot-yellow');
  var dotGreen   = document.getElementById('traffic-dot-green');

  if (!pRed || !pYellow || !pGreen) return;

  function clearPoles() {
    pRed.className    = 'traffic-pole-bulb pole-red';
    pYellow.className = 'traffic-pole-bulb pole-yellow';
    pGreen.className  = 'traffic-pole-bulb pole-green';
    if (dotRed)    { dotRed.style.opacity = '0.3';    dotRed.style.boxShadow = 'none'; }
    if (dotYellow) { dotYellow.style.opacity = '0.3'; dotYellow.style.boxShadow = 'none'; }
    if (dotGreen)  { dotGreen.style.opacity = '0.3';  dotGreen.style.boxShadow = 'none'; }
    if (rtcBadge) rtcBadge.style.display = 'none';
  }

  if (_rtcCycleInterval) { clearInterval(_rtcCycleInterval); _rtcCycleInterval = null; }
  clearPoles();

  // PRIORITY 1: RTC Jadwal aktif → siklus Merah→Kuning→Hijau
  if (data.rtcSchedule == 1) {
    if (rtcBadge) rtcBadge.style.display = 'inline-flex';
    if (cardLabel) { cardLabel.innerText = 'JADWAL RTC AKTIF'; cardLabel.style.background = 'rgba(245,158,11,0.15)'; cardLabel.style.color = '#f59e0b'; cardLabel.style.borderColor = 'rgba(245,158,11,0.3)'; }
    if (curStatus) { curStatus.innerText = 'Jadwal Penyiraman RTC Sedang Berjalan'; curStatus.style.color = '#f59e0b'; }
    if (curDetail) curDetail.innerText = 'Lampu Traffic Light fisik (Pin 25/33/32) menyala bergantian sebagai indikator jadwal aktif';
    var _cycleStep = 0;
    var _poles     = [['pole-bulb-red','pole-red','#ef4444','traffic-dot-red'],['pole-bulb-yellow','pole-yellow','#f59e0b','traffic-dot-yellow'],['pole-bulb-green','pole-green','#10b981','traffic-dot-green']];
    function _doCycle() {
      clearPoles();
      var c = _poles[_cycleStep];
      var el = document.getElementById(c[0]); if (el) el.className = 'traffic-pole-bulb ' + c[1] + ' lit blink';
      var d  = document.getElementById(c[3]); if (d) { d.style.opacity = '1'; d.style.boxShadow = '0 0 8px ' + c[2]; }
      _cycleStep = (_cycleStep + 1) % 3;
    }
    _doCycle();
    _rtcCycleInterval = setInterval(_doCycle, 600);
    updateLedDiagPanel(4);
    return;
  }

  // PRIORITY 2: Hardware Error Darurat → Merah blink
  if (data.isSystemError == 1) {
    pRed.className = 'traffic-pole-bulb pole-red lit blink';
    if (dotRed) { dotRed.style.opacity = '1'; dotRed.style.boxShadow = '0 0 8px #ef4444'; }
    if (cardLabel) { cardLabel.innerText = 'HARDWARE DARURAT'; cardLabel.style.background = 'rgba(239,68,68,0.15)'; cardLabel.style.color = '#ef4444'; cardLabel.style.borderColor = 'rgba(239,68,68,0.3)'; }
    if (curStatus) { curStatus.innerText = 'Hardware Error / Failsafe Aktif'; curStatus.style.color = '#ef4444'; }
    if (curDetail) curDetail.innerText = 'Lampu Merah (Pin 25) berkedip. Failsafe sistem terkunci.';
    updateLedDiagPanel(1);
    return;
  }

  // PRIORITY 3: Boot / Menunggu → Kuning blink (Hanya jika belum ada telemetri tanah)
  var isWaiting = (data.isWaiting == 1 && !hasSoil);
  if (isWaiting) {
    var pct = (data.waitingPercent !== undefined) ? data.waitingPercent : 0;
    var wSec = (data.waitingSecs !== undefined) ? data.waitingSecs : 0;
    var waitTimeStr = pct + "% (" + wSec + "s / 60s)";
    pYellow.className = 'traffic-pole-bulb pole-yellow lit blink';
    if (dotYellow) { dotYellow.style.opacity = '1'; dotYellow.style.boxShadow = '0 0 8px #f59e0b'; }
    if (cardLabel) { cardLabel.innerText = 'MENUNGGU KONEKSI'; cardLabel.style.background = 'rgba(245,158,11,0.15)'; cardLabel.style.color = '#f59e0b'; cardLabel.style.borderColor = 'rgba(245,158,11,0.3)'; }
    if (curStatus) { curStatus.innerText = 'Menunggu sinyal ESP8266...'; curStatus.style.color = '#f59e0b'; }
    if (curDetail) curDetail.innerText = 'Lampu Kuning (Pin 33) berkedip. Mencari sinyal: ' + waitTimeStr;
    updateLedDiagPanel(2);
    return;
  }

  // PRIORITY 4: Bahaya suhu ≥35°C → Merah solid
  if (data.statusColor === 'bahaya' || (hasTemp && tVal >= 35.0)) {
    pRed.className = 'traffic-pole-bulb pole-red lit';
    if (dotRed) { dotRed.style.opacity = '1'; dotRed.style.boxShadow = '0 0 8px #ef4444'; }
    if (cardLabel) { cardLabel.innerText = 'BAHAYA KRITIS'; cardLabel.style.background = 'rgba(239,68,68,0.15)'; cardLabel.style.color = '#ef4444'; cardLabel.style.borderColor = 'rgba(239,68,68,0.3)'; }
    if (curStatus) { curStatus.innerText = 'BAHAYA — Suhu Ekstrem ' + (tVal !== null ? tVal + '°C' : '--'); curStatus.style.color = '#ef4444'; }
    if (curDetail) curDetail.innerText = 'Lampu Merah (Pin 25) menyala solid. Segera lakukan pendinginan lahan!';
    updateLedDiagPanel(1);
    return;
  }

  // PRIORITY 5: Peringatan → Kuning solid
  if (data.statusColor === 'peringatan' || (hasSoil && sVal < 45) || (hasTemp && tVal > 30.0)) {
    pYellow.className = 'traffic-pole-bulb pole-yellow lit';
    if (dotYellow) { dotYellow.style.opacity = '1'; dotYellow.style.boxShadow = '0 0 8px #f59e0b'; }
    if (cardLabel) { cardLabel.innerText = 'PERLU IRIGASI'; cardLabel.style.background = 'rgba(245,158,11,0.15)'; cardLabel.style.color = '#f59e0b'; cardLabel.style.borderColor = 'rgba(245,158,11,0.3)'; }
    if (curStatus) { curStatus.innerText = 'Waspada — Tanah/Suhu Memerlukan Tindakan'; curStatus.style.color = '#f59e0b'; }
    if (curDetail) curDetail.innerText = 'Lampu Kuning (Pin 33). Tanah: ' + (sVal !== null ? sVal + '%' : '--') + ' · Suhu: ' + (tVal !== null ? tVal + '°C' : '--');
    updateLedDiagPanel(2);
    return;
  }

  // DEFAULT: Aman jika ada data riil valid; Sensor Terputus jika belum ada pembacaan sensor
  if (hasSoil || hasTemp) {
    pGreen.className = 'traffic-pole-bulb pole-green lit';
    if (dotGreen) { dotGreen.style.opacity = '1'; dotGreen.style.boxShadow = '0 0 10px #10b981'; }
    if (cardLabel) { cardLabel.innerText = 'KONDISI AMAN'; cardLabel.style.background = 'rgba(16,185,129,0.15)'; cardLabel.style.color = '#10b981'; cardLabel.style.borderColor = 'rgba(16,185,129,0.3)'; }
    if (curStatus) { curStatus.innerText = 'Sistem Aman — Kondisi Optimal'; curStatus.style.color = '#10b981'; }
    if (curDetail) curDetail.innerText = 'Lampu Hijau (Pin 32). Tanah: ' + (sVal !== null ? sVal + '%' : '--') + ' · Suhu: ' + (tVal !== null ? tVal + '°C' : '--');
    updateLedDiagPanel(3);
  } else {
    pRed.className = 'traffic-pole-bulb pole-red lit blink';
    if (dotRed) { dotRed.style.opacity = '1'; dotRed.style.boxShadow = '0 0 8px #ef4444'; }
    if (cardLabel) { cardLabel.innerText = 'SENSOR TERPUTUS / OFFLINE'; cardLabel.style.background = 'rgba(239,68,68,0.15)'; cardLabel.style.color = '#ef4444'; cardLabel.style.borderColor = 'rgba(239,68,68,0.3)'; }
    if (curStatus) { curStatus.innerText = 'ESP8266 Terputus / Sensor Offline'; curStatus.style.color = '#ef4444'; }
    if (curDetail) curDetail.innerText = 'Lampu Merah (Pin 25) berkedip. Pompa dikunci mati oleh failsafe sistem.';
    updateLedDiagPanel(1);
  }

  // Update GPIO diagnostic panel
  updateLedDiagPanel(data.ledState);
}

// ================================================================
// updateLedDiagPanel — Update GPIO diagnostic panel
// ledState: 0=off, 1=merah, 2=kuning, 3=hijau, 4=rtc-cycle
// Menampilkan sinyal GPIO aktual yang dikirim firmware ke setiap pin
// ================================================================
function updateLedDiagPanel(ledState) {
  var stRed    = document.getElementById('hw-state-red');
  var stYellow = document.getElementById('hw-state-yellow');
  var stGreen  = document.getElementById('hw-state-green');
  var dRed     = document.getElementById('hw-dot-red');
  var dYellow  = document.getElementById('hw-dot-yellow');
  var dGreen   = document.getElementById('hw-dot-green');

  if (!stRed || !stYellow || !stGreen) return;

  // Reset all to OFF style
  function setOff(stEl, dEl) {
    stEl.innerText = 'LOW (OFF)';
    stEl.style.color = 'var(--text-sub)';
    stEl.style.background = 'rgba(255,255,255,0.04)';
    if (dEl) { dEl.style.opacity = '0.25'; dEl.style.boxShadow = 'none'; }
  }
  function setOn(stEl, dEl, color, label) {
    stEl.innerText = label || 'HIGH (ON)';
    stEl.style.color = color;
    stEl.style.background = 'rgba(255,255,255,0.08)';
    if (dEl) { dEl.style.opacity = '1'; dEl.style.boxShadow = '0 0 6px ' + color; }
  }

  setOff(stRed, dRed);
  setOff(stYellow, dYellow);
  setOff(stGreen, dGreen);

  var ls = parseInt(ledState) || 0;
  switch(ls) {
    case 1: // Merah
      setOn(stRed, dRed, '#ef4444', 'HIGH (ON) ●');
      break;
    case 2: // Kuning
      setOn(stYellow, dYellow, '#f59e0b', 'HIGH (ON) ●');
      break;
    case 3: // Hijau
      setOn(stGreen, dGreen, '#10b981', 'HIGH (ON) ●');
      break;
    case 4: // RTC Cycle
      stRed.innerText    = 'CYCLE (Gantian)';
      stYellow.innerText = 'CYCLE (Gantian)';
      stGreen.innerText  = 'CYCLE (Gantian)';
      stRed.style.color    = '#ef4444';
      stYellow.style.color = '#f59e0b';
      stGreen.style.color  = '#10b981';
      if (dRed) { dRed.style.opacity = '1'; dRed.style.boxShadow = '0 0 4px #ef4444'; }
      if (dYellow) { dYellow.style.opacity = '1'; dYellow.style.boxShadow = '0 0 4px #f59e0b'; }
      if (dGreen) { dGreen.style.opacity = '1'; dGreen.style.boxShadow = '0 0 4px #10b981'; }
      break;
    case 5: // Menyiram Lahan (3 Lampu Berkedip Aktif)
      stRed.innerText    = 'PULSE (Menyiram)';
      stYellow.innerText = 'PULSE (Menyiram)';
      stGreen.innerText  = 'PULSE (Menyiram)';
      stRed.style.color    = '#ef4444';
      stYellow.style.color = '#f59e0b';
      stGreen.style.color  = '#10b981';
      if (dRed) { dRed.style.opacity = '1'; dRed.style.boxShadow = '0 0 8px #ef4444'; }
      if (dYellow) { dYellow.style.opacity = '1'; dYellow.style.boxShadow = '0 0 8px #f59e0b'; }
      if (dGreen) { dGreen.style.opacity = '1'; dGreen.style.boxShadow = '0 0 8px #10b981'; }
      break;
    default: // 0 = semua mati
      break;
  }
}

// ================================================================
// reportLedMismatch — Tampilkan panduan troubleshoot hardware LED
// Dipanggil dari tombol "Laporkan Mismatch" di diagnostic panel
// ================================================================
function reportLedMismatch() {
  var alertEl = document.getElementById('traffic-mismatch-alert');
  if (!alertEl) return;
  if (alertEl.style.display === 'none' || alertEl.style.display === '') {
    alertEl.style.display = 'block';
    alertEl.scrollIntoView({ behavior: 'smooth', block: 'nearest' });
  } else {
    alertEl.style.display = 'none';
  }
}

// ================================================================
// POLLING TELEMETRI OTOMATIS REALTIME (LANGSUNG LOAD & TIAP 2 DETIK)
// ================================================================
var _telemetryTimer = null;
function initTelemetryPolling() {
  // 1. Instant Hydration dari cache localStorage agar saat refresh halaman langsung tampil stabil seketika
  try {
    var cached = localStorage.getItem('sf_telemetry_cache');
    if (cached) {
      var cData = JSON.parse(cached);
      if (cData && typeof cData === 'object') {
        applyTelemetryData(cData);
      }
    }
  } catch (e) {}

  // 2. Langsung ambil telemetri terbaru dari ESP32
  if (typeof fetchData === 'function') {
    fetchData();
  }

  if (!_telemetryTimer) {
    _telemetryTimer = setInterval(function () {
      if (typeof fetchData === 'function') {
        fetchData(); // Perbarui terus menerus tiap 2 detik
      }
    }, 2000);
  }
}

if (document.readyState === 'loading') {
  document.addEventListener('DOMContentLoaded', initTelemetryPolling);
} else {
  initTelemetryPolling();
}

