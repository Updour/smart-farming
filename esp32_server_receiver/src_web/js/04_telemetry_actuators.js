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
  var rawHeatC = (data && data.heatC !== undefined && data.heatC !== null && data.heatC !== "undefined" && data.heatC !== "--") ? parseFloat(data.heatC) : NaN;
  var heatC = !isNaN(rawHeatC) ? rawHeatC.toFixed(1) : null;
  var rawHeatF = (data && data.heatF !== undefined && data.heatF !== null && data.heatF !== "undefined" && data.heatF !== "--") ? parseFloat(data.heatF) : NaN;
  var heatF = !isNaN(rawHeatF) ? rawHeatF.toFixed(1) : null;

  var hiTemp = (hasMicroclimateData && tempNum !== null) ? tempNum : (!isNaN(satT) ? satT : null);
  var hiHum = (hasMicroclimateData && humNum !== null) ? humNum : (!isNaN(satH) ? satH : null);

  if ((!heatC || !heatF) && hiTemp !== null && hiHum !== null) {
    var tempF = hiTemp * 1.8 + 32;
    var hiF = 0.5 * (tempF + 61.0 + ((tempF - 68.0) * 1.2) + (hiHum * 0.094));
    if (hiF >= 80) {
      hiF = -42.379 + 2.04901523 * tempF + 10.14333127 * hiHum - 0.22475541 * tempF * hiHum
            - 0.00683783 * tempF * tempF - 0.05481717 * hiHum * hiHum
            + 0.00122874 * tempF * tempF * hiHum + 0.00085282 * tempF * hiHum * hiHum
            - 0.00000199 * tempF * tempF * hiHum * hiHum;
    }
    if (!isNaN(hiF)) {
      heatF = hiF.toFixed(1);
      heatC = ((hiF - 32) / 1.8).toFixed(1);
    }
  }
  var elHeatC = document.getElementById('bmkg-heat-index');
  var elHeatF = document.getElementById('bmkg-heat-f');
  var elHeatStat = document.getElementById('bmkg-heat-status');
  var elHeatBadge = document.getElementById('bmkg-heat-badge');
  if (elHeatC) elHeatC.innerText = (heatC && heatC !== "NaN" && heatC !== "undefined") ? (heatC + "°C") : "--°C";
  if (elHeatF) elHeatF.innerText = (heatF && heatF !== "NaN" && heatF !== "undefined") ? (heatF + "°F") : "--°F";
  if (elHeatStat) {
    var hNum = parseFloat(heatC);
    var hStat = "--";
    if (!isNaN(hNum)) {
      if (hNum >= 38) hStat = "Stres Termal Bahaya!";
      else if (hNum >= 32) hStat = "Waspada Panas Ekstrem";
      else if (hNum >= 27) hStat = "Hangat Normal";
      else if (hNum >= 22) hStat = "Optimal / Nyaman";
      else hStat = "Sensasi Sejuk";
    }
    elHeatStat.innerText = hStat;
    if (elHeatBadge) elHeatBadge.innerText = hasMicroclimateData ? hStat : (heatC ? "Satelit BMKG" : "Termal Riil");
  }

  // 4. Dew Point & Absolute Humidity (Magnus Formula)
  var rawDew = (data && data.dew !== undefined && data.dew !== null && data.dew !== "undefined" && data.dew !== "--") ? parseFloat(data.dew) : NaN;
  var dew = !isNaN(rawDew) ? rawDew.toFixed(1) : null;
  var absHum = null;

  if (hasMicroclimateData && tempNum !== null && humNum !== null && humNum > 0) {
    var a = 17.27, b = 237.7;
    var alpha = ((a * tempNum) / (b + tempNum)) + Math.log(humNum / 100.0);
    var calcDew = (b * alpha) / (a - alpha);
    if (!isNaN(calcDew)) {
      if (!dew) dew = calcDew.toFixed(1);
    }
    var calcAbs = (216.7 * (humNum / 100.0) * 6.112 * Math.exp((17.67 * tempNum) / (tempNum + 243.5))) / (273.15 + tempNum);
    if (!isNaN(calcAbs)) {
      absHum = calcAbs.toFixed(1);
    }
  } else if (!dew && !isNaN(satT) && !isNaN(satH) && satH > 0) {
    // Fallback cerdas: Jika sensor kebun belum terhubung tapi data cuaca satelit BMKG ada
    var aS = 17.27, bS = 237.7;
    var alphaS = ((aS * satT) / (bS + satT)) + Math.log(satH / 100.0);
    var calcDewS = (bS * alphaS) / (aS - alphaS);
    if (!isNaN(calcDewS)) dew = calcDewS.toFixed(1);
    var calcAbsS = (216.7 * (satH / 100.0) * 6.112 * Math.exp((17.67 * satT) / (satT + 243.5))) / (273.15 + satT);
    if (!isNaN(calcAbsS)) absHum = calcAbsS.toFixed(1);
  }

  var elDew = document.getElementById('bmkg-dew-point');
  var elAbsHum = document.getElementById('bmkg-abs-hum');
  var elDewStat = document.getElementById('bmkg-dew-status');
  var elDewBadge = document.getElementById('bmkg-dew-badge');
  if (elDew) elDew.innerText = (dew && dew !== "NaN" && dew !== "undefined") ? (dew + "°C") : "--°C";
  if (elAbsHum) elAbsHum.innerText = (absHum && absHum !== "NaN" && absHum !== "undefined") ? (absHum + " g/m³") : "-- g/m³";
  if (elDewStat) {
    var dewDiff = (hasMicroclimateData && dew && !isNaN(parseFloat(dew))) ? (tempNum - parseFloat(dew)) : null;
    var dStat = "--";
    if (dewDiff !== null && !isNaN(dewDiff)) {
      if (dewDiff <= 1.5) dStat = "Kondensasi Embun Jenuh";
      else if (dewDiff <= 3.0) dStat = "Potensi Embun Pagi";
      else dStat = "Bebas Embun";
    } else if (!hasMicroclimateData && !isNaN(satT) && dew && !isNaN(parseFloat(dew))) {
      var satDiff = satT - parseFloat(dew);
      if (satDiff <= 1.5) dStat = "Potensi Embun Satelit";
      else dStat = "Bebas Embun (Satelit)";
    }
    elDewStat.innerText = dStat;
    if (elDewBadge) {
      if (hasMicroclimateData) {
        elDewBadge.innerText = (dewDiff !== null && dewDiff <= 2 ? "Embun Aktif" : "Bebas Embun");
      } else if (!isNaN(satT) && dew) {
        elDewBadge.innerText = "Estimasi Satelit";
      } else {
        elDewBadge.innerText = "Magnus Termal";
      }
    }
  }

  // 5. VPD (Defisit Tekanan Uap & Status Stomata)
  var rawVpd = (data && data.vpd !== undefined && data.vpd !== null && data.vpd !== "undefined" && data.vpd !== "--") ? parseFloat(data.vpd) : NaN;
  var vpd = !isNaN(rawVpd) ? rawVpd.toFixed(2) : null;
  if (!vpd && hasMicroclimateData && tempNum !== null && humNum !== null) {
    var es = 0.61078 * Math.exp((17.27 * tempNum) / (tempNum + 237.3));
    var ea = es * (humNum / 100.0);
    var calcVpd = Math.max(0, es - ea);
    if (!isNaN(calcVpd)) vpd = calcVpd.toFixed(2);
  } else if (!vpd && !isNaN(satT) && !isNaN(satH)) {
    var esS = 0.61078 * Math.exp((17.27 * satT) / (satT + 237.3));
    var eaS = esS * (satH / 100.0);
    var calcVpdS = Math.max(0, esS - eaS);
    if (!isNaN(calcVpdS)) vpd = calcVpdS.toFixed(2);
  }
  var elVpd = document.getElementById('bmkg-vpd-val');
  var elVpdStat = document.getElementById('bmkg-vpd-status');
  var elVpdBadge = document.getElementById('bmkg-vpd-badge');
  if (elVpd) elVpd.innerText = (vpd && vpd !== "NaN" && vpd !== "undefined") ? (vpd + " kPa") : "-- kPa";
  if (elVpdStat) {
    var vpdNum = parseFloat(vpd);
    var vStat = "--";
    var vBadge = "Stomata";
    if (!isNaN(vpdNum)) {
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
    if (elVpdBadge) elVpdBadge.innerText = vBadge;
  }

  // 6. Evaporation Rate (Laju Penguapan Air Lahan)
  var evap = null;
  var evapTemp = (hasMicroclimateData && tempNum !== null) ? tempNum : (!isNaN(satT) ? satT : null);
  if (vpd && !isNaN(parseFloat(vpd)) && evapTemp !== null) {
    var calcEvap = ((0.7 * parseFloat(vpd) + 0.15 * (evapTemp / 10.0)) * 1.05);
    if (!isNaN(calcEvap)) evap = calcEvap.toFixed(1);
  }
  var elEvap = document.getElementById('bmkg-evap-val');
  var elEvapStat = document.getElementById('bmkg-evap-status');
  var elEvapLoss = document.getElementById('bmkg-evap-loss');
  var elEvapBadge = document.getElementById('bmkg-evap-badge');
  if (elEvap) elEvap.innerText = (evap && evap !== "NaN" && evap !== "undefined") ? (evap + " mm/hari") : "-- mm/hari";
  if (elEvapLoss) elEvapLoss.innerText = (evap && evap !== "NaN" && evap !== "undefined") ? (evap + " L/m²") : "-- L/m²";
  if (elEvapStat) {
    var eNum = parseFloat(evap);
    var eStat = "--";
    if (!isNaN(eNum)) {
      if (eNum > 6.0) eStat = "Penguapan Cepat (Kering)";
      else if (eNum < 2.5) eStat = "Penguapan Lambat (Basah)";
      else eStat = "Penguapan Sedang";
    }
    elEvapStat.innerText = eStat;
    if (elEvapBadge) elEvapBadge.innerText = (!isNaN(eNum) && eNum > 5.0) ? "Evap Tinggi" : "Penman ET";
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
  if (mDesc) mDesc.innerText = isManual ? "Mode Manual (Aktuator Terbuka)" : "Mode Otomatis (Sensor & RTC)";

  var isRelayOn = (data.relayOn == 1);
  var modeText = isManual ? " (Manual)" : " (Auto)";
  var relayBadge = document.getElementById('relay-status-badge');
  var liveBar = document.getElementById('pump-live-bar');
  var liveTimer = document.getElementById('pump-live-timer');
  var liveSub = document.getElementById('pump-live-subtitle');

  if (relayBadge) {
    if (isRelayOn) {
      relayBadge.innerHTML = '<span class="badge-dot dot-green" style="background:#06b6d4; box-shadow:0 0 10px #06b6d4;"></span><span style="font-weight:800; letter-spacing:0.5px; color:#06b6d4;">💧 SEDANG MENYIRAM' + modeText + '</span>';
      relayBadge.style.color = "#06b6d4";
      relayBadge.style.borderColor = "rgba(6, 182, 212, 0.5)";
      relayBadge.style.background = "rgba(6, 182, 212, 0.18)";
      if (liveBar) liveBar.style.display = 'flex';
      if (liveTimer) {
        liveTimer.innerHTML = '<span style="color:#06b6d4; font-weight:800;">💧 IRIGASI AKTIF</span>';
      }
      if (liveSub) {
        var sValText = (data.soil !== undefined && data.soil !== null && data.soil !== "--") ? data.soil + "%" : "--";
        liveSub.innerText = "Pin 26 Aktif • Air Mengalir ke Perakaran (Kelembapan: " + sValText + ")";
      }
    } else {
      relayBadge.innerHTML = '<span class="badge-dot dot-gray" style="background:#94a3b8;"></span><span>STANDBY (Mati)' + modeText + '</span>';
      relayBadge.style.color = "var(--text-sub)";
      relayBadge.style.borderColor = "rgba(255, 255, 255, 0.1)";
      relayBadge.style.background = "rgba(0, 0, 0, 0.2)";
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
  if (lDesc) lDesc.innerText = isLampManual ? "Mode Manual (Aktuator Terbuka)" : (data.l_en == 1 ? "Mode Otomatis (Jadwal RTC Aktif)" : "Mode Otomatis (Lampu Nonaktif / Mati)");

  var isLampOn = (data.lampOn == 1);
  var lModeText = isLampManual ? " (Manual)" : " (Auto)";
  var lampBadge = document.getElementById('lamp-status-badge');
  if (lampBadge) {
    if (isLampOn) {
      lampBadge.innerHTML = '<span class="badge-dot dot-yellow" style="background:#eab308; box-shadow:0 0 6px #eab308;"></span><span>AKTIF (Menyala)' + lModeText + '</span>';
      lampBadge.style.color = "#eab308";
      lampBadge.style.borderColor = "rgba(234, 179, 8, 0.4)";
      lampBadge.style.background = "rgba(234, 179, 8, 0.15)";
    } else {
      lampBadge.innerHTML = '<span class="badge-dot dot-gray" style="background:#94a3b8;"></span><span>NONAKTIF (Mati)' + lModeText + '</span>';
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
      txtPump.innerText = "Pompa AKTIF";
      bgPump.className = "badge-pill badge-pump-active";
      bgPump.title = "Pompa Air Aktif (Menyiram Lahan)";
    } else {
      txtPump.innerText = "Pompa MATI";
      bgPump.className = "badge-pill";
      bgPump.style.color = "var(--text-sub)";
      bgPump.title = "Pompa Air Nonaktif / Mati";
    }
  }

  var bgLamp = document.getElementById('badge-global-lamp');
  var txtLamp = document.getElementById('text-global-lamp');
  if (bgLamp && txtLamp) {
    if (isLampOn) {
      txtLamp.innerText = "Lampu AKTIF";
      bgLamp.className = "badge-pill badge-lamp-active";
      bgLamp.title = "Lampu Pemanas/Growlight Aktif";
    } else {
      txtLamp.innerText = "Lampu MATI";
      bgLamp.className = "badge-pill";
      bgLamp.style.color = "var(--text-sub)";
      bgLamp.title = "Lampu Pemanas/Growlight Nonaktif / Mati";
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
  if (statCount) statCount.innerText = (data.pumpCount !== undefined ? data.pumpCount : 0) + " Nyala";

  var statOffCount = document.getElementById('stat-pump-off-count');
  if (statOffCount) statOffCount.innerText = (data.pumpOffCount !== undefined ? data.pumpOffCount : 0) + " Mati";

  var totalSecs = data.totalPumpSecs !== undefined ? data.totalPumpSecs : -1;
  var durStr = "-- Detik";
  var subSecsStr = "-- Detik Akumulasi";
  if (totalSecs !== -1) {
    var hrs = Math.floor(totalSecs / 3600);
    var mins = Math.floor((totalSecs % 3600) / 60);
    var secs = totalSecs % 60;
    if (hrs > 0) {
      durStr = hrs + "j " + mins + "m " + secs + "s";
    } else if (mins > 0) {
      durStr = mins + " Menit " + secs + "s";
    } else {
      durStr = secs + " Detik";
    }
    subSecsStr = totalSecs + " Detik Total Nyala";
  }

  var statDur = document.getElementById('stat-pump-duration');
  if (statDur) statDur.innerText = durStr;
  var statSecs = document.getElementById('stat-pump-secs');
  if (statSecs) statSecs.innerText = subSecsStr;

  var statWater = document.getElementById('stat-water-liters');
  if (statWater) statWater.innerText = (data.waterLiters !== undefined ? data.waterLiters : "0.0") + " Liter";
  var statWaterRate = document.getElementById('stat-water-rate');
  if (statWaterRate) statWaterRate.innerText = "Debit: " + (data.pumpLph || 1800) + " L/jam";

  var statCost = document.getElementById('stat-cost-idr');
  if (statCost) statCost.innerText = "Rp " + (data.costIdr !== undefined ? data.costIdr : "0");
  var statKwh = document.getElementById('stat-kwh-used');
  if (statKwh) statKwh.innerText = "Energi: " + (data.kWhUsed !== undefined ? data.kWhUsed : "0.000") + " kWh";

  if (data.pumpLph !== undefined && document.activeElement.id !== 'cfg-pump-lph') {
    var elLph = document.getElementById('cfg-pump-lph');
    if (elLph) elLph.value = data.pumpLph;
    try { localStorage.setItem('smartfarm_pump_lph', data.pumpLph); } catch (e) {}
  }
  if (data.pumpWatt !== undefined && document.activeElement.id !== 'cfg-pump-watt') {
    var elWatt = document.getElementById('cfg-pump-watt');
    if (elWatt) elWatt.value = data.pumpWatt;
    try { localStorage.setItem('smartfarm_pump_watt', data.pumpWatt); } catch (e) {}
  }
  if (data.plnTariff !== undefined && document.activeElement.id !== 'cfg-pln-tariff') {
    var elTariff = document.getElementById('cfg-pln-tariff');
    if (elTariff) elTariff.value = parseFloat(data.plnTariff);
    try { localStorage.setItem('smartfarm_pump_tariff', data.plnTariff); } catch (e) {}
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
    var s1h = parseInt(data.sched1_h, 10);
    var s1m = parseInt(data.sched1_m, 10);
    var s1timeStr = (s1h < 10 ? '0' : '') + s1h + ':' + (s1m < 10 ? '0' : '') + s1m;
    if (s1en) s1en.checked = (data.sched1_en == 1);
    if (s1tm) s1tm.value = s1timeStr;
    if (s1dur) s1dur.value = data.sched1_dur;
    try {
      localStorage.setItem('smartfarm_sched1_en', data.sched1_en);
      localStorage.setItem('smartfarm_sched1_time', s1timeStr);
      localStorage.setItem('smartfarm_sched1_dur', data.sched1_dur);
    } catch (e) {}
  }

  if (data.sched2_h !== undefined &&
    document.activeElement.id !== 'sched2-en' &&
    document.activeElement.id !== 'sched2-time' &&
    document.activeElement.id !== 'sched2-dur') {
    var s2en = document.getElementById('sched2-en');
    var s2tm = document.getElementById('sched2-time');
    var s2dur = document.getElementById('sched2-dur');
    var s2h = parseInt(data.sched2_h, 10);
    var s2m = parseInt(data.sched2_m, 10);
    var s2timeStr = (s2h < 10 ? '0' : '') + s2h + ':' + (s2m < 10 ? '0' : '') + s2m;
    if (s2en) s2en.checked = (data.sched2_en == 1);
    if (s2tm) s2tm.value = s2timeStr;
    if (s2dur) s2dur.value = data.sched2_dur;
    try {
      localStorage.setItem('smartfarm_sched2_en', data.sched2_en);
      localStorage.setItem('smartfarm_sched2_time', s2timeStr);
      localStorage.setItem('smartfarm_sched2_dur', data.sched2_dur);
    } catch (e) {}
  }

  if (data.l_h !== undefined &&
    document.activeElement.id !== 'lamp-sched-en' &&
    document.activeElement.id !== 'lamp-sched-time' &&
    document.activeElement.id !== 'lamp-sched-dur') {
    var l_en = document.getElementById('lamp-sched-en');
    var l_tm = document.getElementById('lamp-sched-time');
    var l_dur = document.getElementById('lamp-sched-dur');
    var lh = parseInt(data.l_h, 10);
    var lm = parseInt(data.l_m, 10);
    var ltimeStr = (lh < 10 ? '0' : '') + lh + ':' + (lm < 10 ? '0' : '') + lm;
    if (l_en) l_en.checked = (data.l_en == 1);
    if (l_tm) l_tm.value = ltimeStr;
    if (l_dur) l_dur.value = data.l_dur;
    try {
      localStorage.setItem('smartfarm_lamp_en', data.l_en);
      localStorage.setItem('smartfarm_lamp_time', ltimeStr);
      localStorage.setItem('smartfarm_lamp_dur', data.l_dur);
    } catch (e) {}
  }

  // Crop Profile & Thresholds from NVS
  if (data.cropMode !== undefined && document.activeElement.id !== 'crop-profile-select') {
    var sel = document.getElementById('crop-profile-select');
    if (sel && sel.value != data.cropMode) {
      sel.value = data.cropMode;
      if (typeof updateCropProfileUI === 'function') updateCropProfileUI(data.cropMode);
    }
  }
  if (data.cropName !== undefined && data.cropName !== '' && document.activeElement.id !== 'crop-name') {
    var elCropName = document.getElementById('crop-name');
    if (elCropName && elCropName.value !== data.cropName) elCropName.value = data.cropName;
    try { localStorage.setItem('crop_name', data.cropName); } catch(e){}
  }
  if (data.cropAge !== undefined && document.activeElement.id !== 'crop-age-days') {
    var elCropAge = document.getElementById('crop-age-days');
    if (elCropAge && elCropAge.value != data.cropAge) elCropAge.value = data.cropAge;
    try { localStorage.setItem('crop_age', data.cropAge); } catch(e){}
  }
  if (data.cropStage !== undefined && document.activeElement.id !== 'crop-stage') {
    var elCropStage = document.getElementById('crop-stage');
    if (elCropStage && elCropStage.value !== data.cropStage) elCropStage.value = data.cropStage;
    try { localStorage.setItem('crop_stage', data.cropStage); } catch(e){}
  }
  if (data.cropLeaves !== undefined && document.activeElement.id !== 'crop-leaves-count') {
    var elCropLeaves = document.getElementById('crop-leaves-count');
    if (elCropLeaves && elCropLeaves.value != data.cropLeaves) elCropLeaves.value = data.cropLeaves;
    try { localStorage.setItem('crop_leaves', data.cropLeaves); } catch(e){}
  }
  if (data.cropEnv !== undefined && data.cropEnv !== '' && document.activeElement.id !== 'crop-env') {
    var elCropEnv = document.getElementById('crop-env');
    if (elCropEnv && elCropEnv.value !== data.cropEnv) elCropEnv.value = data.cropEnv;
    try { localStorage.setItem('crop_env', data.cropEnv); } catch(e){}
  }
  if (data.cropArea !== undefined && document.activeElement.id !== 'crop-area-size') {
    var elCropArea = document.getElementById('crop-area-size');
    if (elCropArea && elCropArea.value != data.cropArea) elCropArea.value = data.cropArea;
    try { localStorage.setItem('crop_area', data.cropArea); } catch(e){}
  }

  // Jika data profil tanaman dari ESP32 NVS baru tiba, sinkronkan sektor zonasi, agronomis & ringkasan
  if (data.cropName) {
    if (typeof window.syncSectorFromEsp32 === 'function') {
      window.syncSectorFromEsp32(data);
    }
    if (!window._initialCropHydrated) {
      window._initialCropHydrated = true;
      if (typeof updateCropAgronomyAnalysis === 'function') updateCropAgronomyAnalysis();
      if (typeof updateCropHistorySummary === 'function') updateCropHistorySummary();
    }
  }
  if (data.batasTanah !== undefined && document.activeElement.id !== 'slider-soil' && !window._sliderSaveTimer) {
    var elSoil = document.getElementById('slider-soil');
    var bSoilNum = parseInt(data.batasTanah, 10);
    if (elSoil && !isNaN(bSoilNum)) {
      elSoil.value = bSoilNum;
      var valSoil = document.getElementById('val-slider-soil');
      if (valSoil) valSoil.innerText = bSoilNum + '%';
      try { localStorage.setItem('smartfarm_batasTanah', bSoilNum); } catch(e){}
    }
  }
  if (data.batasSuhu !== undefined && document.activeElement.id !== 'slider-temp' && !window._sliderSaveTimer) {
    var elTemp = document.getElementById('slider-temp');
    var bTempNum = parseFloat(data.batasSuhu);
    if (elTemp && !isNaN(bTempNum)) {
      elTemp.value = bTempNum;
      var valTemp = document.getElementById('val-slider-temp');
      if (valTemp) valTemp.innerText = (Number.isInteger(bTempNum) ? bTempNum : bTempNum.toFixed(1)) + '°C';
      try { localStorage.setItem('smartfarm_batasSuhu', bTempNum); } catch(e){}
    }
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


