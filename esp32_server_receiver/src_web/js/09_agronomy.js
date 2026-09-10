/**
 * 09_agronomy.js - Crop Profile Presets & Physiological Agronomy Engine
 * Zero-Dummy Telemetry, Clean SCADA Architecture, Strict line limit < 400 lines
 */

function toggleAdvancedAgronomy() {
  var drawer = document.getElementById('agri-advanced-drawer');
  var icon = document.getElementById('agri-adv-icon');
  if (!drawer) return;
  var isHidden = (drawer.style.display === 'none' || drawer.style.display === '');
  drawer.style.display = isHidden ? 'block' : 'none';
  if (icon) icon.innerText = isHidden ? '▲' : '▼';
}

function applyCropPreset(name, stage, age, env, area) {
  var elName = document.getElementById('crop-name');
  var elStage = document.getElementById('crop-stage');
  var elAge = document.getElementById('crop-age-days');
  var elEnv = document.getElementById('crop-env');
  var elArea = document.getElementById('crop-area-size');

  if (elName) elName.value = name;
  if (elStage) elStage.value = stage;
  if (elAge) elAge.value = age;
  if (elEnv) elEnv.value = env;
  if (elArea) elArea.value = area;

  if (document.getElementById('crop-leaves-count')) document.getElementById('crop-leaves-count').value = (age >= 18 ? 6 : (age >= 12 ? 4 : 2));
  if (document.getElementById('crop-root-status')) document.getElementById('crop-root-status').value = (age >= 12 ? "solid" : "loose");
  if (document.getElementById('crop-ph-level')) document.getElementById('crop-ph-level').value = 6.5;
  if (document.getElementById('crop-ec-level')) document.getElementById('crop-ec-level').value = 1.0;
  updateCropAgronomyAnalysis();
}

function updateCropAgronomyAnalysis() {
  var nameInput = document.getElementById('crop-name');
  var name = nameInput ? nameInput.value.trim() : "";
  var ageInput = document.getElementById('crop-age-days');
  var ageStr = ageInput ? ageInput.value.trim() : "";

  // JIKA BELUM ADA INPUT/PRESET DIPILIH: TAMPILKAN STATUS MENUNGGU (ZERO DUMMY)
  if (!name || !ageStr) {
    renderWaitingAgronomyState();
    return;
  }

  var age = parseInt(ageStr, 10);
  if (isNaN(age) || age < 1) {
    renderWaitingAgronomyState();
    return;
  }

  var stage = document.getElementById('crop-stage') ? document.getElementById('crop-stage').value : 'semai';
  var env = document.getElementById('crop-env') ? document.getElementById('crop-env').value : 'polybag';
  var area = parseInt(document.getElementById('crop-area-size') ? document.getElementById('crop-area-size').value : 100, 10) || 100;

  var leavesCount = parseInt(document.getElementById('crop-leaves-count') ? document.getElementById('crop-leaves-count').value : 4, 10) || 4;
  var rootStatus = document.getElementById('crop-root-status') ? document.getElementById('crop-root-status').value : "solid";
  var phLevel = parseFloat(document.getElementById('crop-ph-level') ? document.getElementById('crop-ph-level').value : 6.5) || 6.5;
  var ecLevel = parseFloat(document.getElementById('crop-ec-level') ? document.getElementById('crop-ec-level').value : 1.0) || 1.0;

  // Persist to LocalStorage
  localStorage.setItem('crop_name', name);
  localStorage.setItem('crop_stage', stage);
  localStorage.setItem('crop_age', age);
  localStorage.setItem('crop_env', env);
  localStorage.setItem('crop_area', area);
  localStorage.setItem('crop_leaves', leavesCount);
  localStorage.setItem('crop_root', rootStatus);
  localStorage.setItem('crop_ph', phLevel);
  localStorage.setItem('crop_ec', ecLevel);

  // Live Telemetry Values from ESP32
  var tData = window.lastTelemetryData || {};
  var tempElem = document.getElementById('kpi-temp');
  var temp = (tData.temp !== undefined && !isNaN(parseFloat(tData.temp))) ? parseFloat(tData.temp) : (tempElem ? (parseFloat(tempElem.innerText) || null) : null);

  var titleElem = document.getElementById('ai-agri-title');
  if (titleElem) titleElem.innerText = "Panduan Lapangan: " + name + " (" + age + " Hari)";

  var targetAge = 21;
  var nameLower = name.toLowerCase();
  if (nameLower.indexOf("cabai") !== -1 || nameLower.indexOf("chili") !== -1) targetAge = 21;
  else if (nameLower.indexOf("bawang") !== -1 || nameLower.indexOf("onion") !== -1) targetAge = 15;
  else if (nameLower.indexOf("tomat") !== -1 || nameLower.indexOf("tomato") !== -1) targetAge = 25;
  else if (nameLower.indexOf("melon") !== -1) targetAge = 14;
  else if (nameLower.indexOf("padi") !== -1 || nameLower.indexOf("rice") !== -1) targetAge = 22;

  var daysRemaining = Math.max(0, targetAge - age);
  var baseReadiness = Math.round((age / targetAge) * 100);
  var readinessPercent = baseReadiness;

  if (stage === 'semai') {
    if (leavesCount < 3) readinessPercent = Math.min(readinessPercent, 40);
    if (rootStatus === 'loose') readinessPercent = Math.min(readinessPercent, 45);
  } else {
    readinessPercent = 100;
  }
  readinessPercent = Math.min(100, Math.max(0, readinessPercent));

  // 1. UPDATE KARTU KESIMPULAN UTAMA 3 DETIK (THE 3-SECOND RULE)
  var banner = document.getElementById('card-agri-status-banner');
  var statusTitle = document.getElementById('ai-status-title');
  var statusDesc = document.getElementById('ai-status-desc');
  var statusSub = document.getElementById('ai-status-sub');

  if (banner && statusTitle && statusDesc && statusSub) {
    if (stage !== 'semai') {
      banner.className = "agri-status-banner status-ready";
      statusTitle.innerHTML = '<span>STATUS: FASE PERTUMBUHAN LAHAN TERBUKA</span>';
      statusDesc.innerText = 'Tanaman ' + name + ' sedang aktif bertumbuh di lahan kebun. Pastikan pasokan air dan nutrisi terjaga sesuai kelembapan tanah aktual.';
      statusSub.innerHTML = 'Kebutuhan Air: <b>Penyiraman teratur pagi hari (07:00 WIB)</b> atau saat kelembapan tanah turun di bawah batas aman.';
    } else if (readinessPercent >= 85) {
      banner.className = "agri-status-banner status-ready";
      statusTitle.innerHTML = '<span>STATUS: BIBIT SIAP PINDAH TANAM (' + readinessPercent + '% SIAP)</span>';
      statusDesc.innerText = 'Akar sudah mengikat tanah persemaian dengan kokoh dan daun sejati sudah ' + leavesCount + ' helai. Bibit aman dipindah ke lahan terbuka tanpa risiko layu.';
      statusSub.innerHTML = 'Waktu pindah tanam terbaik: <b>Sore hari (15:30 - 17:30 WIB)</b> agar bibit tidak layu tersengat terik matahari di hari pertama.';
    } else if (readinessPercent >= 50) {
      banner.className = "agri-status-banner status-waiting";
      statusTitle.innerHTML = '<span>STATUS: TUNGGU ~' + daysRemaining + ' HARI LAGI DI PEMBIBITAN (' + readinessPercent + '% SIAP)</span>';
      statusDesc.innerText = 'Perakaran dan daun sejati masih dalam tahap pembentukan. Pertahankan kelembapan media semai dan jangan terburu-buru dipindah agar bibit tidak mati kaget (transplant shock).';
      statusSub.innerHTML = 'Adaptasi sinar matahari: <b>Buka naungan paranet 2 jam di pagi hari (07:00 - 09:00 WIB)</b> untuk memperkuat batang.';
    } else {
      banner.className = "agri-status-banner status-danger";
      statusTitle.innerHTML = '<span>STATUS: TUNDA PINDAH TANAM! AKAR MASIH MUDA (' + readinessPercent + '% SIAP)</span>';
      statusDesc.innerText = 'Media semai masih mudah rontok dan daun belum mencukupi. Pemindahan saat ini berisiko tinggi menyebabkan kematian bibit hingga 80%.';
      statusSub.innerHTML = 'Panduan perawatan: Jaga media semai tetap lembap dengan semprotan air halus (misting) setiap pagi.';
    }
  }

  // 2. UPDATE PROGRESS BAR KESIAPAN
  var elRPercent = document.getElementById('ai-readiness-percent');
  var elRBar = document.getElementById('ai-readiness-bar');
  if (elRPercent) elRPercent.innerText = readinessPercent + "% (" + (readinessPercent >= 85 ? "Bibit Siap Tanam!" : "Butuh ~" + daysRemaining + " Hari Lagi") + ")";
  if (elRBar) {
    elRBar.style.width = readinessPercent + "%";
    elRBar.style.background = readinessPercent >= 85 ? "linear-gradient(90deg, #10b981, #06b6d4)" : (readinessPercent >= 50 ? "linear-gradient(90deg, #f59e0b, #eab308)" : "#ef4444");
  }

  // 3. UPDATE 4 CHECKLIST PRAKTIS PETANI
  var elLeavesBadge = document.getElementById('ai-leaves-badge');
  var elLeavesHint = document.getElementById('ai-leaves-hint');
  if (elLeavesBadge) elLeavesBadge.innerText = leavesCount + " Helai";
  if (elLeavesHint) {
    elLeavesHint.innerText = leavesCount >= 4 ? "Lolos syarat daun" : "Masih butuh daun baru";
    elLeavesHint.style.color = leavesCount >= 4 ? "#10b981" : "#f59e0b";
  }

  var elRootBadge = document.getElementById('ai-root-badge');
  var elRootHint = document.getElementById('ai-root-hint');
  if (elRootBadge) elRootBadge.innerText = rootStatus === 'solid' ? "Padat Kuat" : "Masih Rapuh";
  if (elRootHint) {
    elRootHint.innerText = rootStatus === 'solid' ? "Tanah tidak rontok" : "Risiko tanah pecah";
    elRootHint.style.color = rootStatus === 'solid' ? "#10b981" : "#ef4444";
  }

  var elHeatVal = document.getElementById('ai-metric-heat');
  var elHeatNote = document.getElementById('ai-note-heat');
  if (elHeatVal) elHeatVal.innerText = temp !== null && !isNaN(temp) ? temp.toFixed(1) + "°C" : "--°C";
  if (elHeatNote) {
    if (temp && temp > 33) {
      elHeatNote.innerText = "Terik panas (Siram sore)";
      elHeatNote.style.color = "#ef4444";
    } else if (temp && temp >= 24) {
      elHeatNote.innerText = "Suhu aman / sejuk";
      elHeatNote.style.color = "#10b981";
    } else {
      elHeatNote.innerText = "Menunggu sensor";
      elHeatNote.style.color = "var(--text-sub)";
    }
  }

  var waterFactor = stage === 'semai' ? 2.5 : (stage === 'generatif' ? 5.0 : 4.0);
  var dailyWater = Math.round(area * waterFactor);
  var emberCor = Math.max(1, Math.round(dailyWater / 12)); // 1 ember cor / gembor ~12 Liter
  var elIrr = document.getElementById('ai-metric-irrigation');
  var elIrrNote = document.getElementById('ai-note-irrigation');
  if (elIrr) elIrr.innerText = "±" + dailyWater + " Liter";
  if (elIrrNote) elIrrNote.innerText = "±" + emberCor + " Gembor / Ember Cor";

  // 4. UPDATE PANDUAN PRAKTIS PETANI HARI INI
  var adviceBox = document.getElementById('ai-agronomy-advice');
  if (adviceBox) {
    var adviceText = "<b>Panduan Tindakan Petani Hari Ini:</b><br>";
    if (stage === 'semai') {
      if (readinessPercent >= 85) {
        adviceText += "Bibit <b>" + name + "</b> sudah sangat siap tanam! Siram media semai 1-2 jam sebelum dicabut agar media tetap basah dan akar tidak patah. Pindahkan ke lubang tanam di sore hari, lalu langsung siram air secukupnya.";
      } else {
        adviceText += "Bibit <b>" + name + "</b> masih membutuhkan waktu sekitar <b>" + daysRemaining + " hari lagi</b> di tempat pembibitan. Siram rutin tiap pagi (±" + emberCor + " gembor untuk area pembibitan Anda). Berikan pencahayaan sinar matahari pagi secara bertahap agar batang semakin kokoh.";
      }
    } else {
      adviceText += "Tanaman <b>" + name + "</b> sedang dalam tahap <b>" + (stage === 'generatif' ? 'Pembungaan & Buah' : 'Pertumbuhan Lahan') + "</b>. Kebutuhan air harian sekitar <b>" + dailyWater + " Liter</b> (±" + emberCor + " ember cor per bedeng). " + (temp && temp > 32 ? "Karena suhu terik (" + temp.toFixed(1) + "°C), prioritaskan penyiraman di sore hari." : "Kondisi iklim mikro sangat optimal untuk pembentukan hasil.");
    }
    adviceBox.innerHTML = adviceText;
  }

  if (typeof calculateFertigationDose === 'function') calculateFertigationDose();
  if (typeof calculateFinancialROI === 'function') calculateFinancialROI();
}

function renderWaitingAgronomyState() {
  var titleElem = document.getElementById('ai-agri-title');
  if (titleElem) titleElem.innerText = "Panduan Lapangan: Menunggu Konfigurasi Tanaman";

  var banner = document.getElementById('card-agri-status-banner');
  var statusTitle = document.getElementById('ai-status-title');
  var statusDesc = document.getElementById('ai-status-desc');
  var statusSub = document.getElementById('ai-status-sub');

  if (banner) banner.className = "agri-status-banner status-waiting";
  if (statusTitle) statusTitle.innerHTML = '<span>STATUS: MENUNGGU DATA KOMODITAS TANAMAN</span>';
  if (statusDesc) statusDesc.innerText = 'Pilih salah satu preset varietas atau masukkan nama dan usia tanaman pada formulir di bawah untuk memulai analisis agronomi presisi.';
  if (statusSub) statusSub.innerHTML = 'Status Formulir: <b>Standby (-- HST)</b>';

  var elRPercent = document.getElementById('ai-readiness-percent');
  var elRBar = document.getElementById('ai-readiness-bar');
  if (elRPercent) elRPercent.innerText = "--% (Menunggu Data)";
  if (elRBar) {
    elRBar.style.width = "0%";
    elRBar.style.background = "var(--border-subtle)";
  }

  var elLeavesBadge = document.getElementById('ai-leaves-badge');
  var elLeavesHint = document.getElementById('ai-leaves-hint');
  if (elLeavesBadge) elLeavesBadge.innerText = "-- Helai";
  if (elLeavesHint) {
    elLeavesHint.innerText = "Menunggu input daun";
    elLeavesHint.style.color = "var(--text-sub)";
  }

  var elRootBadge = document.getElementById('ai-root-badge');
  var elRootHint = document.getElementById('ai-root-hint');
  if (elRootBadge) elRootBadge.innerText = "--";
  if (elRootHint) {
    elRootHint.innerText = "Menunggu kondisi akar";
    elRootHint.style.color = "var(--text-sub)";
  }

  var tData = window.lastTelemetryData || {};
  var temp = (tData.temp !== undefined && !isNaN(parseFloat(tData.temp))) ? parseFloat(tData.temp) : null;
  var elHeatVal = document.getElementById('ai-metric-heat');
  var elHeatNote = document.getElementById('ai-note-heat');
  if (elHeatVal) elHeatVal.innerText = temp !== null ? temp.toFixed(1) + "°C" : "--°C";
  if (elHeatNote) {
    elHeatNote.innerText = temp !== null ? "Sensor aktif" : "Menunggu sensor";
    elHeatNote.style.color = "var(--text-sub)";
  }

  var elIrr = document.getElementById('ai-metric-irrigation');
  var elIrrNote = document.getElementById('ai-note-irrigation');
  if (elIrr) elIrr.innerText = "-- Liter";
  if (elIrrNote) elIrrNote.innerText = "Menunggu luas & fase";

  var adviceBox = document.getElementById('ai-agronomy-advice');
  if (adviceBox) {
    adviceBox.innerHTML = "<b>Pemberitahuan Sistem Agronomi:</b><br>Data tanaman belum diatur. Pilih salah satu preset tanaman di atas atau isi nama dan umur tanaman (HST) untuk mendapatkan kalkulasi presisi.";
  }
}

function clearCropFormUI() {
  updateCropAgronomyAnalysis();
}

function loadCropFormFromStorage() {
  var storedName = localStorage.getItem('crop_name');
  if (storedName) {
    var elName = document.getElementById('crop-name');
    if (elName) elName.value = storedName;
    if (localStorage.getItem('crop_stage') && document.getElementById('crop-stage')) document.getElementById('crop-stage').value = localStorage.getItem('crop_stage');
    if (localStorage.getItem('crop_age') && document.getElementById('crop-age-days')) document.getElementById('crop-age-days').value = localStorage.getItem('crop_age');
    if (localStorage.getItem('crop_env') && document.getElementById('crop-env')) document.getElementById('crop-env').value = localStorage.getItem('crop_env');
    if (localStorage.getItem('crop_area') && document.getElementById('crop-area-size')) document.getElementById('crop-area-size').value = localStorage.getItem('crop_area');
    if (localStorage.getItem('crop_leaves') && document.getElementById('crop-leaves-count')) document.getElementById('crop-leaves-count').value = localStorage.getItem('crop_leaves');
    if (localStorage.getItem('crop_root') && document.getElementById('crop-root-status')) document.getElementById('crop-root-status').value = localStorage.getItem('crop_root');
    if (localStorage.getItem('crop_ph') && document.getElementById('crop-ph-level')) document.getElementById('crop-ph-level').value = localStorage.getItem('crop_ph');
    if (localStorage.getItem('crop_ec') && document.getElementById('crop-ec-level')) document.getElementById('crop-ec-level').value = localStorage.getItem('crop_ec');
    updateCropAgronomyAnalysis();
  } else {
    // STRICT ZERO DUMMY: Jangan paksa preset jika user belum memilih!
    renderWaitingAgronomyState();
  }
}

function speakAgronomyVoiceReport() {
  if (!('speechSynthesis' in window)) {
    alert("Peramban Anda tidak mendukung fitur suara text-to-speech.");
    return;
  }

  var name = document.getElementById('crop-name') ? document.getElementById('crop-name').value : "";
  var age = document.getElementById('crop-age-days') ? document.getElementById('crop-age-days').value : "";
  var statusTitle = document.getElementById('ai-status-title') ? document.getElementById('ai-status-title').innerText : "";
  var tempElem = document.getElementById('kpi-temp');
  var temp = tempElem ? tempElem.innerText : "--";

  if (!name || !age) {
    var notSet = new SpeechSynthesisUtterance("Halo Mas, data tanaman sama umurnya diisi dulu ya biar infonya pas.");
    notSet.lang = 'id-ID';
    window.speechSynthesis.speak(notSet);
    return;
  }

  var textToSpeak = "Halo Mas, ini kabar kebun hari ini. Tanaman " + name + " sekarang umurnya " + age + " hari. " + statusTitle + ". Suhu di kebun " + temp + " derajat. Dijaga terus ya biar tanamannya makin subur!";

  var utterance = new SpeechSynthesisUtterance(textToSpeak);
  utterance.lang = 'id-ID';
  utterance.rate = 0.95;
  window.speechSynthesis.speak(utterance);
}


// ================= GEMINI AI INTEGRATION ================= //
window.DEFAULT_GEMINI_KEY = "";

window.getGeminiApiKey = function() {
  var key = localStorage.getItem("geminiApiKey");
  if (!key || key.trim().length === 0) {
    return "";
  }
  return key.trim();
};

function saveGeminiKey(val) {
  if (val && val.trim().length > 0) {
    localStorage.setItem("geminiApiKey", val.trim());
  } else {
    localStorage.removeItem("geminiApiKey");
  }
}

// Load key on init
document.addEventListener("DOMContentLoaded", function() {
  var storedKey = window.getGeminiApiKey();
  var input = document.getElementById("gemini-api-key");
  if (input) {
    input.value = storedKey;
  }
});

// ================= GEMINI MODEL DISCOVERY & FALLBACK ENGINE ================= //
window.fetchGeminiWithFallback = async function(apiKey, bodyObj) {
  // Use global key if not provided
  if (!apiKey || apiKey.trim().length === 0) {
    apiKey = window.getGeminiApiKey();
  }

  // Prioritize verified 100% available models (gemini-3.6-flash & gemini-3.5-flash)
  var candidateModels = [
    'gemini-3.6-flash',
    'gemini-3.5-flash',
    'gemini-3-flash-preview',
    'gemini-3.7-flash',
    'gemini-flash-latest',
    'gemini-pro-latest'
  ];

  try {
    const res = await fetch(`https://generativelanguage.googleapis.com/v1beta/models?key=${apiKey}`);
    if (res.ok) {
      const data = await res.json();
      if (data && data.models && data.models.length > 0) {
        const supported = data.models
          .filter(function(m) { 
            var name = (m.name || '').toLowerCase();
            if (name.includes('tts') || name.includes('embed') || name.includes('imagen') || name.includes('aqa') || name.includes('2.5-flash') || name.includes('1.5-flash')) {
              return false;
            }
            return m.supportedGenerationMethods && m.supportedGenerationMethods.includes('generateContent'); 
          })
          .map(function(m) { return m.name.replace('models/', ''); });
        
        if (supported.length > 0) {
          candidateModels = candidateModels.concat(supported);
        }
      }
    }
  } catch (e) {
    console.warn("Could not query model list:", e);
  }

  // Deduplicate and ensure no TTS/embedding models remain
  candidateModels = candidateModels.filter(function(m, idx) {
    var low = m.toLowerCase();
    return !low.includes('tts') && !low.includes('embed') && candidateModels.indexOf(m) === idx;
  });

  var lastError = null;
  for (var i = 0; i < candidateModels.length; i++) {
    var modelName = candidateModels[i];
    try {
      const response = await fetch(`https://generativelanguage.googleapis.com/v1beta/models/${modelName}:generateContent?key=${apiKey}`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(bodyObj)
      });
      const result = await response.json();
      if (result.error) {
        lastError = new Error(result.error.message || `API Error ${result.error.code}`);
        var errMsg = (result.error.message || '').toLowerCase();
        var code = result.error.code;
        if (code === 404 || code === 503 || code === 429 || code === 500 ||
            errMsg.includes('not found') || errMsg.includes('not supported') || 
            errMsg.includes('modality') || errMsg.includes('not enabled') || 
            errMsg.includes('high demand') || errMsg.includes('demand') ||
            errMsg.includes('overloaded') || errMsg.includes('exhausted') || 
            errMsg.includes('quota') || errMsg.includes('rate') || errMsg.includes('busy') ||
            errMsg.includes('try again later')) {
          console.warn(`Model ${modelName} returned error (${errMsg}), trying next candidate...`);
          continue;
        }
        throw lastError;
      }
      return result;
    } catch (err) {
      lastError = err;
      var errLower = (err.message || '').toLowerCase();
      if (errLower.includes('not found') || errLower.includes('not supported') || 
          errLower.includes('404') || errLower.includes('503') || errLower.includes('429') ||
          errLower.includes('modality') || errLower.includes('not enabled') || 
          errLower.includes('high demand') || errLower.includes('demand') ||
          errLower.includes('overloaded') || errLower.includes('exhausted') || 
          errLower.includes('quota') || errLower.includes('busy') ||
          errLower.includes('try again later')) {
        console.warn(`Model ${modelName} exception (${errLower}), trying next candidate...`);
        continue;
      }
      throw err;
    }
  }
  throw lastError || new Error("Seluruh model Gemini saat ini sedang sibuk atau tidak merespons. Silakan coba kembali dalam beberapa saat.");
};

async function askGeminiAgronomist() {
  var inputElem = document.getElementById("gemini-api-key");
  var apiKey = window.getGeminiApiKey();
  
  if (inputElem && inputElem.value) {
    apiKey = inputElem.value.trim();
    localStorage.setItem("geminiApiKey", apiKey);
  }
  
  if (!apiKey || apiKey.length < 5) {
    alert("Silakan masukkan API Key Gemini Anda di kolom konfigurasi terlebih dahulu.");
    return;
  }
  
  var btnText = document.getElementById("gemini-btn-text");
  var guidanceBox = document.getElementById("agronomy-guidance");
  var modeSel = document.getElementById("control-mode");
  
  btnText.innerHTML = "Menganalisis Iklim Mikro...";
  document.getElementById("btn-gemini-ai").style.opacity = "0.7";
  document.getElementById("btn-gemini-ai").disabled = true;
  
  var data = window.lastTelemetryData;
  if (!data || data.soil === undefined || data.soil < 0 || data.unplugged) {
    alert("Data sensor fisik ESP32 belum tersedia atau sensor kebun sedang terputus. AI hanya memproses telemetri riil dari lahan!");
    btnText.innerHTML = "Analisis Iklim Mikro &amp; Optimasi Ambang Batas (Gemini 2.5)";
    document.getElementById("btn-gemini-ai").style.opacity = "1";
    document.getElementById("btn-gemini-ai").disabled = false;
    return;
  }
  var cropName = "Padi / Cabai / Hortikultura Presisi";
  var stageInfo = document.getElementById("crop-stage");
  if (stageInfo) cropName = stageInfo.innerText;

  var prompt = `Anda adalah Insinyur Pertanian Presisi & Agronom Senior.
Saya punya kebun dengan telemetri riil berikut:
- Suhu Udara: ${data.temp}°C
- Kelembaban Udara: ${data.hum}%
- Kelembaban Tanah Aktual: ${data.soil}%
- Fase Tanaman: ${cropName}


Tolong evaluasi kondisi ini dan berikan rekomendasi mutlak (angka bulat) untuk ambang batas siram pompa (soil_threshold) dan batas panas darurat (temp_threshold).
Wajib balas dalam format JSON murni tanpa markdown, dengan struktur:
{
  "soil_threshold": <angka_persen_10_sampai_90>,
  "temp_threshold": <angka_derajat_20_sampai_45>,
  "analysis": "<2 kalimat singkat penjelasan mengapa Anda merekomendasikan angka tersebut>"
}`;

  try {
    const result = await window.fetchGeminiWithFallback(apiKey, {
      contents: [{ parts: [{ text: prompt }] }],
      generationConfig: { temperature: 0.2 }
    });
    
    let textOut = result.candidates[0].content.parts[0].text;
    textOut = textOut.replace(/```json/g, '').replace(/```/g, '').trim();
    const jsonOut = JSON.parse(textOut);
    
    // Update Sliders UI & ESP32
    if (jsonOut.soil_threshold) {
      document.getElementById('slider-soil').value = jsonOut.soil_threshold;
      onSliderManualChange('soil', jsonOut.soil_threshold);
    }
    if (jsonOut.temp_threshold) {
      document.getElementById('slider-temp').value = jsonOut.temp_threshold;
      onSliderManualChange('temp', jsonOut.temp_threshold);
    }
    
    // Auto-switch to Custom mode in UI (3) to show it's manually set by AI
    modeSel.value = "3";
    
    // Show AI guidance
    guidanceBox.innerHTML = `
      <div style="display:flex; flex-direction:column; gap:10px;">
        <div style="color:var(--text-main); font-weight:700; font-size:12px; margin-bottom:-4px; display:flex; gap:6px; align-items:center;">
          <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="#a855f7" stroke-width="2"><path d="M12 2v4m0 12v4M4.93 4.93l2.83 2.83m8.48 8.48l2.83 2.83M2 12h4m12 0h4M4.93 19.07l2.83-2.83m8.48-8.48l2.83-2.83"></path></svg>
          Hasil Analisis Gemini AI:
        </div>
        <div style="color:var(--text-sub); line-height:1.4;">${jsonOut.analysis}</div>
      </div>
    `;
    
    saveThresholds(true);
    alert("AI telah mengkalibrasi ulang parameter Anda! Mengirim ke NVS ESP32...");
    
  } catch (err) {
    console.error(err);
    alert("Gagal menghubungi AI. Pastikan API Key benar dan ada koneksi internet. Error: " + err.message);
  } finally {
    btnText.innerHTML = "Auto-Set via Gemini AI ✨";
    document.getElementById("btn-gemini-ai").style.opacity = "1";
    document.getElementById("btn-gemini-ai").disabled = false;
  }
}
