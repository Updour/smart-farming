// =================================================================
// 04_TELEMETRY_ACTUATORS.JS - ACTUATOR, SCHEDULE & METRICS SYNC
// Target lines: ~170 (Max < 350)
// =================================================================

function updateActuatorAndScheduleUI(data, friendlyRtc, hasTemp, tVal, hasHum, hVal) {
  // ================= BMKG COMPARISON & MICROCLIMATE ENGINE ================= //
  var satT = parseFloat(data.satTemp);
  if (isNaN(satT)) satT = parseFloat(localStorage.getItem('bmkg_temp'));
  var satH = parseFloat(localStorage.getItem('bmkg_hum'));

  // 1. Varians Suhu Lahan vs BMKG Satelit
  var tDiffStr = "--";
  var elTempDiff = document.getElementById('bmkg-temp-diff');
  var elTempInsight = document.getElementById('bmkg-temp-insight');
  if (hasTemp && !isNaN(satT)) {
    var tDiffVal = (tVal - satT).toFixed(1);
    tDiffStr = (tDiffVal > 0 ? "+" : "") + tDiffVal + "°C";
    if (elTempInsight) {
      if (parseFloat(tDiffVal) > 2.0) {
        elTempInsight.innerText = "Lahan Lebih Panas";
        elTempInsight.style.color = "#f59e0b";
      } else if (parseFloat(tDiffVal) < -2.0) {
        elTempInsight.innerText = "Lahan Lebih Sejuk";
        elTempInsight.style.color = "#38bdf8";
      } else {
        elTempInsight.innerText = "Suhu Serasi Satelit";
        elTempInsight.style.color = "#10b981";
      }
    }
  }
  var bDhtT = document.getElementById('bmkg-dht-temp');
  var bSatT = document.getElementById('bmkg-sat-temp');
  if (bDhtT) bDhtT.innerText = hasTemp ? (tVal + "°C") : "--°C";
  if (bSatT) bSatT.innerText = (!isNaN(satT) ? (satT + "°C") : "--°C");
  if (elTempDiff) {
    elTempDiff.innerText = tDiffStr;
    elTempDiff.style.color = (tDiffStr.startsWith("+") ? "#f59e0b" : "#38bdf8");
  }

  // 2. Varians Kelembapan Lahan vs BMKG Satelit
  var hDiffStr = "--";
  var elHumDiff = document.getElementById('bmkg-hum-diff');
  var elHumInsight = document.getElementById('bmkg-hum-insight');
  if (hasHum && !isNaN(satH)) {
    var hDiffVal = (hVal - satH).toFixed(1);
    hDiffStr = (hDiffVal > 0 ? "+" : "") + hDiffVal + "%";
    if (elHumInsight) {
      if (parseFloat(hDiffVal) > 5.0) {
        elHumInsight.innerText = "Tajuk Cenderung Basah";
        elHumInsight.style.color = "#38bdf8";
      } else if (parseFloat(hDiffVal) < -5.0) {
        elHumInsight.innerText = "Aerasi Tajuk Kering";
        elHumInsight.style.color = "#f59e0b";
      } else {
        elHumInsight.innerText = "Kelembapan Normal";
        elHumInsight.style.color = "#10b981";
      }
    }
  }
  var bDhtH = document.getElementById('bmkg-dht-hum');
  var bSatH = document.getElementById('bmkg-sat-hum');
  if (bDhtH) bDhtH.innerText = hasHum ? (hVal + "%") : "--%";
  if (bSatH) bSatH.innerText = (!isNaN(satH) ? (satH + "%") : "--%");
  if (elHumDiff) {
    elHumDiff.innerText = hDiffStr;
    elHumDiff.style.color = (hDiffStr.startsWith("+") ? "#38bdf8" : "#10b981");
  }

  // Physical calculations: Murni dari sensor riil DHT11 lahan
  var tempNum = (hasTemp && !isNaN(parseFloat(tVal))) ? parseFloat(tVal) : null;
  var humNum = (hasHum && !isNaN(parseFloat(hVal))) ? parseFloat(hVal) : null;
  var hasMicroclimateData = (tempNum !== null && humNum !== null);

  // 3. Heat Index (Indeks Panas Terasa)
  var heatC = data.heatC;
  var heatF = data.heatF;
  if ((!heatC || heatC === "--") && hasMicroclimateData) {
    var tempF = tempNum * 1.8 + 32;
    var hiF = 0.5 * (tempF + 61.0 + ((tempF - 68.0) * 1.2) + (humNum * 0.094));
    if (hiF >= 80) {
      hiF = -42.379 + 2.04901523 * tempF + 10.14333127 * humNum - 0.22475541 * tempF * humNum
            - 0.00683783 * tempF * tempF - 0.05481717 * humNum * humNum
            + 0.00122874 * tempF * tempF * humNum + 0.00085282 * tempF * humNum * humNum
            - 0.00000199 * tempF * tempF * humNum * humNum;
    }
    heatF = hiF.toFixed(1);
    heatC = ((hiF - 32) / 1.8).toFixed(1);
  }
  var elHeatC = document.getElementById('bmkg-heat-index');
  var elHeatF = document.getElementById('bmkg-heat-f');
  var elHeatStat = document.getElementById('bmkg-heat-status');
  var elHeatBadge = document.getElementById('bmkg-heat-badge');
  if (elHeatC) elHeatC.innerText = hasMicroclimateData ? (heatC + "°C") : "--°C";
  if (elHeatF) elHeatF.innerText = hasMicroclimateData ? (heatF + "°F") : "--°F";
  if (elHeatStat) {
    var hNum = parseFloat(heatC);
    var hStat = "--";
    if (hasMicroclimateData && !isNaN(hNum)) {
      if (hNum >= 38) hStat = "Stres Termal Bahaya!";
      else if (hNum >= 32) hStat = "Waspada Panas Ekstrem";
      else if (hNum >= 27) hStat = "Hangat Normal";
      else if (hNum >= 22) hStat = "Optimal / Nyaman";
      else hStat = "Sensasi Sejuk";
    }
    elHeatStat.innerText = hStat;
    if (elHeatBadge) elHeatBadge.innerText = hasMicroclimateData ? hStat : "Termal Riil";
  }

  // 4. Dew Point & Absolute Humidity (Magnus Formula)
  var dew = data.dew;
  var absHum = data.absHum;
  if ((!dew || dew === "--") && hasMicroclimateData) {
    var a = 17.27, b = 237.7;
    var alpha = ((a * tempNum) / (b + tempNum)) + Math.log(humNum / 100.0);
    dew = ((b * alpha) / (a - alpha)).toFixed(1);
    absHum = ((216.7 * (humNum / 100.0) * 6.112 * Math.exp((17.67 * tempNum) / (tempNum + 243.5))) / (273.15 + tempNum)).toFixed(1);
  }
  var elDew = document.getElementById('bmkg-dew-point');
  var elAbsHum = document.getElementById('bmkg-abs-hum');
  var elDewStat = document.getElementById('bmkg-dew-status');
  var elDewBadge = document.getElementById('bmkg-dew-badge');
  if (elDew) elDew.innerText = hasMicroclimateData ? (dew + "°C") : "--°C";
  if (elAbsHum) elAbsHum.innerText = hasMicroclimateData ? (absHum + " g/m³") : "-- g/m³";
  if (elDewStat) {
    var dewDiff = hasMicroclimateData ? (tempNum - parseFloat(dew)) : null;
    var dStat = "--";
    if (dewDiff !== null && !isNaN(dewDiff)) {
      if (dewDiff <= 1.5) dStat = "Kondensasi Embun Jenuh";
      else if (dewDiff <= 3.0) dStat = "Potensi Embun Pagi";
      else dStat = "Bebas Embun";
    }
    elDewStat.innerText = dStat;
    if (elDewBadge) elDewBadge.innerText = hasMicroclimateData ? (dewDiff !== null && dewDiff <= 2 ? "Embun Aktif" : "Bebas Embun") : "Magnus";
  }

  // 5. VPD (Defisit Tekanan Uap & Status Stomata)
  var vpd = data.vpd;
  if ((!vpd || vpd === "--") && hasMicroclimateData) {
    var es = 0.61078 * Math.exp((17.27 * tempNum) / (tempNum + 237.3));
    var ea = es * (humNum / 100.0);
    vpd = Math.max(0, es - ea).toFixed(2);
  }
  var elVpd = document.getElementById('bmkg-vpd-val');
  var elVpdStat = document.getElementById('bmkg-vpd-status');
  var elVpdBadge = document.getElementById('bmkg-vpd-badge');
  if (elVpd) elVpd.innerText = hasMicroclimateData ? (vpd + " kPa") : "-- kPa";
  if (elVpdStat) {
    var vpdNum = parseFloat(vpd);
    var vStat = "--";
    var vBadge = "Stomata";
    if (hasMicroclimateData && !isNaN(vpdNum)) {
      if (vpdNum < 0.4) {
        vStat = "Terlalu Lembap (Risiko Jamur)";
        vBadge = "Risiko Jamur";
      } else if (vpdNum < 0.8) {
        vStat = "Ideal Bibit / Fase Semai";
        vBadge = "Semai";
      } else if (vpdNum <= 1.2) {
        vStat = "Optimal Vegetatif & Bunga";
        vBadge = "Optimal";
      } else if (vpdNum <= 1.6) {
        vStat = "Kering (Transpirasi Tinggi)";
        vBadge = "Transpirasi +";
      } else {
        vStat = "Stres Dehidrasi (Stomata Tutup)";
        vBadge = "Stomata Tutup";
      }
    }
    elVpdStat.innerText = vStat;
    if (elVpdBadge) elVpdBadge.innerText = hasMicroclimateData ? vBadge : "Stomata";
  }

  // 6. Evaporation Rate (Laju Penguapan Air Lahan)
  var evap = data.evaporation;
  if ((!evap || evap === "--") && hasMicroclimateData && vpd && !isNaN(parseFloat(vpd))) {
    evap = ((0.7 * parseFloat(vpd) + 0.15 * (tempNum / 10.0)) * 1.05).toFixed(1);
  }
  var elEvap = document.getElementById('bmkg-evap-val');
  var elEvapStat = document.getElementById('bmkg-evap-status');
  var elEvapLoss = document.getElementById('bmkg-evap-loss');
  var elEvapBadge = document.getElementById('bmkg-evap-badge');
  if (elEvap) elEvap.innerText = hasMicroclimateData ? (evap + " mm/hari") : "-- mm/hari";
  if (elEvapLoss) elEvapLoss.innerText = hasMicroclimateData ? (evap + " L/m²") : "-- L/m²";
  if (elEvapStat) {
    var eNum = parseFloat(evap);
    var eStat = "--";
    if (hasMicroclimateData && !isNaN(eNum)) {
      if (eNum > 6.0) eStat = "Penguapan Cepat (Kering)";
      else if (eNum < 2.5) eStat = "Penguapan Lambat (Basah)";
      else eStat = "Penguapan Sedang";
    }
    elEvapStat.innerText = eStat;
    if (elEvapBadge) elEvapBadge.innerText = hasMicroclimateData ? (eNum > 5.0 ? "Evap Tinggi" : "Evap Normal") : "Penman ET";
  }

  // 7 & 8. BMKG Atmosphere, Wind & Rain Interlock
  var rawWindDir = localStorage.getItem('bmkg_wind_dir') || "S";
  var elWindDir = document.getElementById('bmkg-wind-dir');
  var elWindSpd = document.getElementById('bmkg-wind-speed');
  var elSatDesc = document.getElementById('bmkg-sat-desc');
  var elRainPred = document.getElementById('bmkg-rain-pred');
  var elRecom = document.getElementById('bmkg-recommendation');
  var elLastSync = document.getElementById('bmkg-last-sync');

  if (elWindDir && typeof parseWindDirection === 'function') elWindDir.innerText = parseWindDirection(rawWindDir);
  if (elWindSpd) elWindSpd.innerText = localStorage.getItem('bmkg_wind_spd') || "-- km/jam";
  if (elSatDesc) elSatDesc.innerText = localStorage.getItem('bmkg_desc') || (data.satDesc || "Memuat BMKG...");
  
  var rainText = localStorage.getItem('bmkg_rain') || data.satRainPred || "Cerah (Tidak Ada Hujan)";
  if (elRainPred) {
    elRainPred.innerText = rainText;
    elRainPred.style.color = (rainText.toLowerCase().includes("hujan")) ? "#f59e0b" : "#10b981";
  }

  if (elRecom) {
    if (rainText.toLowerCase().includes("hujan")) {
      elRecom.innerText = "Peringatan Hujan Satelit: Menunda siklus pompa untuk efisiensi air lahan.";
      elRecom.style.color = "#f59e0b";
    } else {
      elRecom.innerText = "Cuaca Kondusif: Penyiraman otomatis beroperasi penuh mengikuti sensor tanah.";
      elRecom.style.color = "var(--text-sub)";
    }
  }
  if (elLastSync) {
    var syncTime = localStorage.getItem('bmkg_sync_time') || data.satTime || "-";
    elLastSync.innerText = syncTime + (syncTime !== "-" ? " WIB (Leces)" : "");
  }

  // Mode & Relay States
  var isManual = (data.isManual == 1);


  var btnPumpAuto = document.getElementById('btn-pump-mode-auto');
  var btnPumpManual = document.getElementById('btn-pump-mode-manual');
  if (btnPumpAuto && btnPumpManual) {
    if (isManual) {
      btnPumpManual.className = "segment-btn active-manual";
      btnPumpAuto.className = "segment-btn";
    } else {
      btnPumpAuto.className = "segment-btn active";
      btnPumpManual.className = "segment-btn";
    }
  }

  var btnOn = document.getElementById('btn-pump-on');
  var btnOff = document.getElementById('btn-pump-off');
  var mDesc = document.getElementById('mode-desc-text');
  if (btnOn) btnOn.disabled = !isManual;
  if (btnOff) btnOff.disabled = !isManual;
  if (mDesc) mDesc.innerText = isManual ? "Mode Manual" : "Mode Otomatis (Sensor & RTC)";

  var isRelayOn = (data.relayOn == 1);
  var modeText = (data.manual == 1) ? " (Manual)" : " (Auto)";
  var relayBadge = document.getElementById('relay-status-badge');
  var liveBar = document.getElementById('pump-live-bar');
  if (relayBadge) {
    if (isRelayOn) {
      relayBadge.innerHTML = '<span class="badge-dot dot-green" style="background:#10b981; box-shadow:0 0 6px #10b981;"></span><span>Pompa Menyala' + modeText + '</span>';
      relayBadge.style.color = "#10b981";
      relayBadge.style.borderColor = "rgba(16, 185, 129, 0.4)";
      relayBadge.style.background = "rgba(16, 185, 129, 0.15)";
      if (liveBar) liveBar.style.display = 'flex';
    } else {
      relayBadge.innerHTML = '<span class="badge-dot dot-red" style="background:#ef4444;"></span><span>Standby' + modeText + '</span>';
      relayBadge.style.color = "#ef4444";
      relayBadge.style.borderColor = "rgba(239, 68, 68, 0.4)";
      relayBadge.style.background = "rgba(239, 68, 68, 0.15)";
      if (liveBar) liveBar.style.display = 'none';
    }
  }


  // Lamp Mode & States
  var isLampManual = (data.lampManual == 1);
  var btnLampAuto = document.getElementById('btn-lamp-mode-auto');
  var btnLampManual = document.getElementById('btn-lamp-mode-manual');
  if (btnLampAuto && btnLampManual) {
    if (isLampManual) {
      btnLampManual.className = "segment-btn active-manual";
      btnLampAuto.className = "segment-btn";
    } else {
      btnLampAuto.className = "segment-btn active";
      btnLampManual.className = "segment-btn";
    }
  }

  var btnLampOn = document.getElementById('btn-lamp-on');
  var btnLampOff = document.getElementById('btn-lamp-off');
  var lDesc = document.getElementById('lamp-mode-desc-text');
  if (btnLampOn) btnLampOn.disabled = !isLampManual;
  if (btnLampOff) btnLampOff.disabled = !isLampManual;
  if (lDesc) lDesc.innerText = isLampManual ? "Mode Manual" : "Mode Otomatis (18:00 - 06:00)";

  var isLampOn = (data.lampOn == 1);
  var isLampManual = (data.lampManual == 1);
  var lModeText = isLampManual ? " (Manual)" : " (Auto)";
  var lampBadge = document.getElementById('lamp-status-badge');
  if (lampBadge) {
    if (isLampOn) {
      lampBadge.innerHTML = '<span class="badge-dot dot-yellow" style="background:#eab308; box-shadow:0 0 6px #eab308;"></span><span>Lampu Menyala' + lModeText + '</span>';
      lampBadge.style.color = "#eab308";
      lampBadge.style.borderColor = "rgba(234, 179, 8, 0.4)";
      lampBadge.style.background = "rgba(234, 179, 8, 0.15)";
    } else {
      lampBadge.innerHTML = '<span class="badge-dot dot-gray" style="background:#94a3b8;"></span><span>Standby' + lModeText + '</span>';
      lampBadge.style.color = "#94a3b8";
      lampBadge.style.borderColor = "rgba(148, 163, 184, 0.4)";
      lampBadge.style.background = "rgba(148, 163, 184, 0.15)";
    }
  }

  // Synchronize Global Header Badges (Compliant with Rule 3.C)
  var bgPump = document.getElementById('badge-global-pump');
  var txtPump = document.getElementById('text-global-pump');
  if (bgPump && txtPump) {
    if (isRelayOn) {
      txtPump.innerText = "Pompa ON";
      bgPump.className = "badge-pill badge-pump-active";
      bgPump.title = "Pompa Air Aktif (Menyiram Lahan)";
    } else {
      txtPump.innerText = "Pompa OFF";
      bgPump.className = "badge-pill";
      bgPump.style.color = "var(--text-sub)";
      bgPump.title = "Pompa Air Standby / Mati";
    }
  }

  var bgLamp = document.getElementById('badge-global-lamp');
  var txtLamp = document.getElementById('text-global-lamp');
  if (bgLamp && txtLamp) {
    if (isLampOn) {
      txtLamp.innerText = "Lampu ON";
      bgLamp.className = "badge-pill badge-lamp-active";
      bgLamp.title = "Lampu Pemanas/Growlight Aktif";
    } else {
      txtLamp.innerText = "Lampu OFF";
      bgLamp.className = "badge-pill";
      bgLamp.style.color = "var(--text-sub)";
      bgLamp.title = "Lampu Pemanas/Growlight Standby / Mati";
    }
  }

  var bgMode = document.getElementById('badge-global-mode');
  var txtMode = document.getElementById('text-global-mode');
  if (bgMode && txtMode) {
    if (isManual) {
      txtMode.innerText = "MAN";
      bgMode.className = "badge-pill badge-mode-manual";
      bgMode.title = "Mode Kendali: Manual - Klik untuk Buka Tab Kendali";
    } else {
      txtMode.innerText = "AUTO";
      bgMode.className = "badge-pill badge-mode-auto";
      bgMode.title = "Mode Kendali: Otomatis (Sensor & Jadwal) - Klik untuk Buka Tab Kendali";
    }
  }

  // Update SCADA Real-Time Running Ticker Bar
  updateScadaTicker(data, friendlyRtc, hasTemp, tVal, hasHum, hVal, isRelayOn, isLampOn, isManual);


  var statCount = document.getElementById('stat-pump-count');
  if (statCount) statCount.innerText = (data.pumpCount !== undefined ? data.pumpCount : "--") + " Kali";

  var totalSecs = data.totalPumpSecs !== undefined ? data.totalPumpSecs : -1;
    var timeStr = "-- Detik";
  if (totalSecs !== -1) {
    var mins = Math.floor(totalSecs / 60);
    var secs = totalSecs % 60;
    timeStr = totalSecs + " Detik";
    if (mins > 0) timeStr += " (" + mins + "m " + secs + "s)";
  }
  var statSecs = document.getElementById('stat-pump-secs');
  if (statSecs) statSecs.innerText = timeStr;

  var statWater = document.getElementById('stat-water-liters');
  var statCost = document.getElementById('stat-cost-idr');
  if (statWater) statWater.innerText = (data.waterLiters !== undefined ? data.waterLiters : "--") + " Liter";
  if (statCost) statCost.innerText = "Rp " + (data.costIdr !== undefined ? data.costIdr : "--");

  if (data.pumpLph !== undefined && document.activeElement.id !== 'cfg-pump-lph') {
    var elLph = document.getElementById('cfg-pump-lph');
    if (elLph) elLph.value = data.pumpLph;
  }
  if (data.pumpWatt !== undefined && document.activeElement.id !== 'cfg-pump-watt') {
    var elWatt = document.getElementById('cfg-pump-watt');
    if (elWatt) elWatt.value = data.pumpWatt;
  }
  if (data.plnTariff !== undefined && document.activeElement.id !== 'cfg-pln-tariff') {
    var elTariff = document.getElementById('cfg-pln-tariff');
    if (elTariff) elTariff.value = parseFloat(data.plnTariff);
  }

  // RTC & Schedule Form
  var rtcLive = document.getElementById('rtc-live-time');
  var rtcValid = document.getElementById('rtc-valid-badge');
  if (rtcLive) rtcLive.innerText = friendlyRtc;
  if (rtcValid) {
    rtcValid.innerText = data.rtcValid ? "RTC Valid" : "Fallback Mode";
    rtcValid.style.color = data.rtcValid ? "var(--primary)" : "var(--warning)";
  }

  if (data.sched1_h !== undefined &&
    document.activeElement.id !== 'sched1-en' &&
    document.activeElement.id !== 'sched1-time' &&
    document.activeElement.id !== 'sched1-dur') {
    var s1en = document.getElementById('sched1-en');
    var s1tm = document.getElementById('sched1-time');
    var s1dur = document.getElementById('sched1-dur');
    if (s1en) s1en.checked = (data.sched1_en == 1);
    if (s1tm) s1tm.value = (data.sched1_h < 10 ? '0' : '') + data.sched1_h + ':' + (data.sched1_m < 10 ? '0' : '') + data.sched1_m;
    if (s1dur) s1dur.value = data.sched1_dur;
  }

  if (data.sched2_h !== undefined &&
    document.activeElement.id !== 'sched2-en' &&
    document.activeElement.id !== 'sched2-time' &&
    document.activeElement.id !== 'sched2-dur') {
    var s2en = document.getElementById('sched2-en');
    var s2tm = document.getElementById('sched2-time');
    var s2dur = document.getElementById('sched2-dur');
    if (s2en) s2en.checked = (data.sched2_en == 1);
    if (s2tm) s2tm.value = (data.sched2_h < 10 ? '0' : '') + data.sched2_h + ':' + (data.sched2_m < 10 ? '0' : '') + data.sched2_m;
    if (s2dur) s2dur.value = data.sched2_dur;
  }

  if (data.l_h !== undefined &&
    document.activeElement.id !== 'lamp-sched-en' &&
    document.activeElement.id !== 'lamp-sched-time' &&
    document.activeElement.id !== 'lamp-sched-dur') {
    var l_en = document.getElementById('lamp-sched-en');
    var l_tm = document.getElementById('lamp-sched-time');
    var l_dur = document.getElementById('lamp-sched-dur');
    if (l_en) l_en.checked = (data.l_en == 1);
    if (l_tm) l_tm.value = (data.l_h < 10 ? '0' : '') + data.l_h + ':' + (data.l_m < 10 ? '0' : '') + data.l_m;
    if (l_dur) l_dur.value = data.l_dur;
  }

  // Crop Profile & Thresholds from NVS
  if (data.cropMode !== undefined && document.activeElement.id !== 'crop-profile-select') {
    var sel = document.getElementById('crop-profile-select');
    if (sel && sel.value != data.cropMode) {
      sel.value = data.cropMode;
      if (typeof updateCropProfileUI === 'function') updateCropProfileUI(data.cropMode);
    }
  }
  if (data.batasTanah !== undefined && document.activeElement.id !== 'slider-soil') {
    var elSoil = document.getElementById('slider-soil');
    if (elSoil) elSoil.value = data.batasTanah;
    var valSoil = document.getElementById('val-slider-soil');
    if (valSoil) valSoil.innerText = data.batasTanah + '%';
  }
  if (data.batasSuhu !== undefined && document.activeElement.id !== 'slider-temp') {
    var elTemp = document.getElementById('slider-temp');
    if (elTemp) elTemp.value = data.batasSuhu;
    var valTemp = document.getElementById('val-slider-temp');
    if (valTemp) valTemp.innerText = data.batasSuhu + '°C';
  }
}

/**
 * Update SCADA Real-Time Running Marquee Ticker with 100% Live Telemetry & Micro-Animations
 */
function updateScadaTicker(data, friendlyRtc, hasTemp, tVal, hasHum, hVal, isRelayOn, isLampOn, isManual) {
  var t1 = document.getElementById('ticker-text');
  var t2 = document.getElementById('ticker-text-clone');
  if (!t1 && !t2) return;

  var tempStr = (hasTemp && tVal !== null && !isNaN(tVal)) ? tVal.toFixed(1) + '°C' : '--°C';
  var humStr = (hasHum && hVal !== null && !isNaN(hVal)) ? Math.round(hVal) + '%' : '--%';
  var soilVal = (data && data.soil !== undefined && data.soil >= 0) ? Math.round(data.soil) + '%' : '--%';

  var pumpBadge = isRelayOn
    ? '<span style="color:#06b6d4;font-weight:700;"><span class="ticker-pulse-cyan"></span>ON</span>'
    : '<span style="color:#94a3b8;">OFF</span>';

  var lampBadge = isLampOn
    ? '<span style="color:#eab308;font-weight:700;"><span class="ticker-pulse-gold"></span>ON</span>'
    : '<span style="color:#94a3b8;">OFF</span>';

  var modeBadge = isManual
    ? '<span style="color:#f59e0b;font-weight:700;">MAN</span>'
    : '<span style="color:#10b981;font-weight:700;">AUTO</span>';

  var rtcStr = (friendlyRtc && friendlyRtc !== '--') ? friendlyRtc : ((data && data.time) ? data.time : '--:-- WIB');
  var sigStr = (data && data.rssi !== undefined && data.rssi !== 0) ? data.rssi + ' dBm' : '-- dBm';
  var vpdStr = (data && data.vpd !== undefined && !isNaN(parseFloat(data.vpd))) ? parseFloat(data.vpd).toFixed(2) + ' kPa' : null;
  var batStr = (data && data.battery !== undefined && data.battery > 0) ? data.battery + '%' : null;

  var sep = '&nbsp;&nbsp;<span style="opacity:0.35;">•</span>&nbsp;&nbsp;';

  var items = [
    'Suhu: <strong style="color:var(--text-main);">' + tempStr + '</strong>',
    'RH Udara: <strong style="color:var(--text-main);">' + humStr + '</strong>',
    'Tanah: <strong style="color:var(--text-main);">' + soilVal + '</strong>',
    'Pompa: ' + pumpBadge,
    'Lampu: ' + lampBadge,
    'Mode: ' + modeBadge,
    'Waktu: <strong style="color:var(--text-main);">' + rtcStr + '</strong>',
    'Sinyal: ' + sigStr
  ];

  if (vpdStr) items.push('VPD: ' + vpdStr);
  if (batStr) items.push('Baterai: ' + batStr);

  var html = items.join(sep);
  if (t1) t1.innerHTML = html;
  if (t2) t2.innerHTML = html;

  if (typeof renderVirtualLcdRows === 'function') {
    renderVirtualLcdRows();
  }
}


