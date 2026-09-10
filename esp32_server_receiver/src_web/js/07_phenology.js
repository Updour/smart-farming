/**
 * 07_phenology.js - Biological Milestones, GDD, VPD, VWC & Agronomic Intelligence
 * Zero-Dummy, 100% Real ESP Sensor Telemetry, Strict line limit < 400 lines
 */

function updatePhenologyAI(sec, tVal, hVal, sVal, vpdVal) {
  if (!sec && typeof getActiveSector === 'function') sec = getActiveSector();

  var cropTag = document.getElementById('pheno-crop-tag');
  var healthBadge = document.getElementById('pheno-health-badge');
  var gddValElem = document.getElementById('pheno-gdd-val');
  var gddNoteElem = document.getElementById('pheno-gdd-note');
  var vpdValElem = document.getElementById('pheno-vpd-val');
  var vpdNoteElem = document.getElementById('pheno-vpd-note');
  var vwcValElem = document.getElementById('pheno-vwc-val');
  var vwcNoteElem = document.getElementById('pheno-vwc-note');
  var adviceElem = document.getElementById('pheno-ai-advice');
  var etaBadge = document.getElementById('pheno-target-eta');
  var timelineFill = document.getElementById('pheno-timeline-fill');

  if (!cropTag) return;

  if (!sec) {
    cropTag.innerText = "Belum Ada Sektor Kebun Dikonfigurasi";
    if (healthBadge) {
      healthBadge.innerText = "Standby (Menunggu Sektor)";
      healthBadge.style.color = "var(--text-sub)";
      healthBadge.style.borderColor = "var(--card-border)";
    }
    for (var i = 1; i <= 4; i++) {
      var stepEl = document.getElementById('stage-step-' + i);
      var lblEl = document.getElementById('stage-label-' + i);
      var subEl = document.getElementById('stage-sub-' + i);
      if (lblEl) lblEl.innerText = "Tahap " + i;
      if (subEl) subEl.innerText = "--";
      if (stepEl) stepEl.className = 'pheno-stage-item';
    }
    if (timelineFill) timelineFill.style.width = "0%";
    if (etaBadge) {
      etaBadge.innerText = "Menunggu Sektor Aktif";
      etaBadge.style.color = "var(--text-sub)";
      etaBadge.style.borderColor = "var(--card-border)";
    }
    if (gddValElem) gddValElem.innerText = "-- GDD/hari";
    if (gddNoteElem) gddNoteElem.innerText = "Pilih atau tambahkan sektor lahan";
    if (vpdValElem) vpdValElem.innerText = "-- kPa";
    if (vpdNoteElem) {
      vpdNoteElem.innerText = "Menunggu konfigurasi tanaman";
      vpdNoteElem.style.color = "var(--text-sub)";
    }
    if (vwcValElem) vwcValElem.innerText = "--% VWC";
    if (vwcNoteElem) {
      vwcNoteElem.innerText = "Menunggu telemetri sektor";
      vwcNoteElem.style.color = "var(--text-sub)";
    }
    if (adviceElem) {
      adviceElem.innerHTML = "<i>Belum ada sektor kebun yang aktif. Daftarkan petak lahan Anda melalui tombol <b>[+ Tambah Sektor]</b> di panel Zonasi Lahan untuk mengaktifkan pemodelan fenologi &amp; agronomis presisi.</i>";
    }
    return;
  }

  var cropName = sec.name || 'Tanaman';
  var cropStage = (sec.stage || 'semai').toLowerCase();
  var cropAge = parseInt(sec.age, 10) || 1;
  var cropEnv = sec.env || 'open';

  // Crop Species Agronomic Base Constants
  var nameLow = cropName.toLowerCase();
  var tBase = 10;
  var nurseryDays = 21;
  var harvestDays = 75;

  if (nameLow.indexOf('cabai') !== -1 || nameLow.indexOf('chili') !== -1 || nameLow.indexOf('pepper') !== -1) {
    tBase = 12;
    nurseryDays = 21;
    harvestDays = 90;
  } else if (nameLow.indexOf('tomat') !== -1 || nameLow.indexOf('tomato') !== -1) {
    tBase = 10;
    nurseryDays = 25;
    harvestDays = 80;
  } else if (nameLow.indexOf('bawang') !== -1 || nameLow.indexOf('shallot') !== -1 || nameLow.indexOf('onion') !== -1) {
    tBase = 8;
    nurseryDays = 35;
    harvestDays = 65;
  } else if (nameLow.indexOf('padi') !== -1 || nameLow.indexOf('rice') !== -1) {
    tBase = 10;
    nurseryDays = 20;
    harvestDays = 115;
  }

  var envLabel = cropEnv === 'greenhouse' ? 'Greenhouse' : 'Lahan Terbuka';
  cropTag.innerText = (sec.code || 'SEKTOR') + ": " + cropName + " • " + cropAge + " HST (" + formatStageLabel(cropStage) + " - " + envLabel + ")";

  // 1. Biological Milestones Tracker
  var mLabels = [];
  var mSubs = [];
  var currentStep = 1;
  var percent = 0;

  if (cropStage === 'semai') {
    mLabels = ['Imbibisi & Enzim', 'Perkecambahan', 'Daun Sejati & Akar', 'Siap Pindah Tanam'];
    mSubs = ['0–3 HST', '4–7 HST', '8–14 HST', '15–' + nurseryDays + ' HST'];

    if (cropAge <= 3) {
      currentStep = 1;
      percent = Math.min(25, Math.max(5, (cropAge / 3) * 25));
    } else if (cropAge <= 7) {
      currentStep = 2;
      percent = 25 + ((cropAge - 3) / 4) * 25;
    } else if (cropAge <= 14) {
      currentStep = 3;
      percent = 50 + ((cropAge - 7) / 7) * 25;
    } else if (cropAge <= nurseryDays) {
      currentStep = 4;
      percent = 75 + ((cropAge - 14) / Math.max(1, (nurseryDays - 14))) * 25;
    } else {
      currentStep = 4;
      percent = 100;
    }

    var daysLeft = Math.max(0, nurseryDays - cropAge);
    if (daysLeft === 0) {
      etaBadge.innerText = "Siap Pindah Tanam Hari Ini";
      etaBadge.style.color = "var(--accent-emerald)";
      etaBadge.style.borderColor = "var(--accent-emerald)";
    } else {
      etaBadge.innerText = "Pindah Tanam: " + daysLeft + " Hari Lagi (" + nurseryDays + " HST)";
      etaBadge.style.color = "var(--accent-blue)";
      etaBadge.style.borderColor = "var(--accent-blue)";
    }
  } else {
    // Vegetatif, Generatif, Panen
    mLabels = ['Fase Semai', 'Fase Vegetatif', 'Fase Generatif', 'Panen Raya'];
    mSubs = ['0–' + nurseryDays + ' HST', (nurseryDays + 1) + '–45 HST', '46–' + (harvestDays - 10) + ' HST', harvestDays + '+ HST'];

    if (cropAge <= nurseryDays) {
      currentStep = 1;
      percent = 20;
    } else if (cropAge <= 45) {
      currentStep = 2;
      percent = 25 + ((cropAge - nurseryDays) / Math.max(1, (45 - nurseryDays))) * 25;
    } else if (cropAge <= (harvestDays - 10)) {
      currentStep = 3;
      percent = 50 + ((cropAge - 45) / Math.max(1, (harvestDays - 55))) * 25;
    } else {
      currentStep = 4;
      percent = Math.min(100, 75 + ((cropAge - harvestDays + 10) / 10) * 25);
    }

    var harvestLeft = Math.max(0, harvestDays - cropAge);
    if (harvestLeft === 0) {
      etaBadge.innerText = "Masa Panen Raya Aktif";
      etaBadge.style.color = "var(--accent-emerald)";
    } else {
      etaBadge.innerText = "Estimasi Panen: " + harvestLeft + " Hari Lagi";
      etaBadge.style.color = "var(--accent-purple, #a855f7)";
    }
  }

  // Update Steps UI
  for (var j = 1; j <= 4; j++) {
    var stepEl = document.getElementById('stage-step-' + j);
    var lblEl = document.getElementById('stage-label-' + j);
    var subEl = document.getElementById('stage-sub-' + j);

    if (lblEl) lblEl.innerText = mLabels[j - 1];
    if (subEl) subEl.innerText = mSubs[j - 1];

    if (stepEl) {
      stepEl.className = 'pheno-stage-item';
      if (j < currentStep) stepEl.classList.add('completed');
      else if (j === currentStep) stepEl.classList.add('current');
    }
  }
  if (timelineFill) timelineFill.style.width = Math.min(100, Math.max(0, percent)) + "%";

  // 2. GDD Calculation
  var hasRealTemp = (tVal !== null && !isNaN(tVal) && tVal > 0);
  if (hasRealTemp && gddValElem && gddNoteElem) {
    var dailyGdd = Math.max(0, tVal - tBase).toFixed(1);
    var totalGdd = Math.round(cropAge * dailyGdd);
    gddValElem.innerText = dailyGdd + " GDD/hari";
    gddNoteElem.innerText = "Akumulasi: ±" + totalGdd + " GDD (Tbase " + tBase + "°C)";
  } else if (gddValElem && gddNoteElem) {
    gddValElem.innerText = "-- GDD/hari";
    gddNoteElem.innerText = "Menunggu telemetri suhu DHT11...";
  }

  // 3. VPD Calculation
  var hasRealVpd = (vpdVal !== null && !isNaN(vpdVal) && vpdVal >= 0);
  if (hasRealVpd && vpdValElem && vpdNoteElem) {
    vpdValElem.innerText = vpdVal.toFixed(2) + " kPa";
    if (vpdVal < 0.35) {
      vpdNoteElem.innerText = "Udara terlalu basah (Risiko jamur & damping-off)";
      vpdNoteElem.style.color = "var(--accent-amber)";
    } else if (vpdVal > 1.2 && cropStage === 'semai') {
      vpdNoteElem.innerText = "Udara terlalu kering (Stres kutikula daun muda)";
      vpdNoteElem.style.color = "var(--accent-amber)";
    } else {
      vpdNoteElem.innerText = "Tekanan transpirasi seimbang (Stomata optimal)";
      vpdNoteElem.style.color = "var(--text-sub)";
    }
  } else if (vpdValElem && vpdNoteElem) {
    vpdValElem.innerText = "-- kPa";
    vpdNoteElem.innerText = "Menunggu sensor suhu/RH DHT11...";
    vpdNoteElem.style.color = "var(--text-sub)";
  }

  // 4. VWC Calculation
  var hasRealSoil = (sVal !== null && !isNaN(sVal) && sVal >= 0);
  if (hasRealSoil && vwcValElem && vwcNoteElem) {
    vwcValElem.innerText = sVal.toFixed(1) + "% VWC";
    if (sVal > 80) {
      vwcNoteElem.innerText = "Media Jenuh (Bahaya Rebah Kecambah Pythium)";
      vwcNoteElem.style.color = "var(--accent-rose)";
    } else if (sVal < 45) {
      vwcNoteElem.innerText = "Media Kering (Akar Bibit Terhambat)";
      vwcNoteElem.style.color = "var(--accent-amber)";
    } else {
      vwcNoteElem.innerText = "Aerasi pori & kadar air media semai ideal";
      vwcNoteElem.style.color = "var(--text-sub)";
    }
  } else if (vwcValElem && vwcNoteElem) {
    vwcValElem.innerText = "--% VWC";
    vwcNoteElem.innerText = "Menunggu telemetri radio ESP8266...";
    vwcNoteElem.style.color = "var(--text-sub)";
  }

  // 5. Synthesis & Dynamic Actionable Advice
  var adviceText = "";
  var badgeText = "Laju Tumbuh Normal";
  var badgeColor = "var(--accent-emerald)";

  if (cropStage === 'semai') {
    if (hasRealSoil && sVal > 80) {
      badgeText = "Bahaya Rebah Semai (Damping-Off)";
      badgeColor = "var(--accent-rose)";
      adviceText = "Media semai terlalu jenuh air (" + sVal.toFixed(1) + "%). Hentikan penyiraman segera dan buka ventilasi/naungan agar media mengering ke 65% untuk mencegah serangan jamur Pythium.";
    } else if (hasRealSoil && sVal < 45) {
      badgeText = "Dehidrasi Benih / Semai";
      badgeColor = "var(--accent-amber)";
      adviceText = "Kelembapan tanah rendah (" + sVal.toFixed(1) + "%). Lakukan penyiraman semprot embun halus (fine misting) agar biji atau akar muda tidak mengalami plasmolisis kering.";
    } else if (cropAge <= 3) {
      adviceText = "Fase imbibisi air & aktivasi enzim giberelin. Jaga media tanam tetap lembap gelap (suhu 26–28°C). Jangan biarkan benih tergenang air bebas.";
    } else if (cropAge <= 7) {
      adviceText = "Kotiledon mulai terbuka. Berikan paparan sinar matahari pagi (jam 07.00–09.30) agar batang kokoh dan terhindar dari etiolasi (bibit kurus tinggi pucat).";
    } else if (cropAge <= 14) {
      adviceText = "Daun sejati dan perakaran muda aktif berkembang. Mulai perkenalkan nutrisi fertigasi starter encer (EC 0.8–1.0 mS/cm) dan jaga VPD di kisaran 0.5–0.8 kPa.";
    } else {
      adviceText = "Fase pengerasan batang (hardening-off). Kurangi naungan secara bertahap selama 3 hari ke depan untuk melatih kutikula daun sebelum dipindah tanam ke lahan terbuka.";
    }
  } else if (cropStage === 'vegetatif') {
    adviceText = "Pertumbuhan vegetatif pesat (batang dan kanopi daun). Tanaman membutuhkan asupan Nitrogen tinggi (NPK/Urea). Pastikan kelembapan tanah dipertahankan pada 65–75%.";
  } else if (cropStage === 'generatif') {
    adviceText = "Inisiasi pembungaan dan pembentukan buah. Naikkan rasio Fosfat (P) dan Kalium (K) untuk mencegah kerontokan bunga. Hindari stres kekeringan ekstrem.";
  } else {
    adviceText = "Fase pematangan dan pengisian hasil panen. Kurangi suplai air secara bertahap untuk meningkatkan kadar gula (Brix) dan daya simpan hasil panen.";
  }

  if (healthBadge) {
    healthBadge.innerText = badgeText;
    healthBadge.style.color = badgeColor;
    healthBadge.style.borderColor = badgeColor;
  }
  if (adviceElem) adviceElem.innerText = adviceText;
}
