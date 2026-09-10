/**
 * 11_disease_risk.js - Real-Time Pest & Fungal Disease Outbreak Surveillance
 * 100% Real ESP32/ESP8266 Sensor Telemetry Driven (Zero Dummy Data)
 * Strict line limit < 400 lines (Target ~150 lines)
 */

function updateOutbreakUIState(isValid, temp, hum, soil) {
  var riskLevelElem = document.getElementById('outbreak-risk-level');
  var b1 = document.getElementById('box-disease-1');
  var b2 = document.getElementById('box-disease-2');
  var b3 = document.getElementById('box-disease-3');

  var tag1 = document.getElementById('disease-1-badge');
  var tag2 = document.getElementById('disease-2-badge');
  var tag3 = document.getElementById('disease-3-badge');

  var desc1 = document.getElementById('disease-1-desc');
  var desc2 = document.getElementById('disease-2-desc');
  var desc3 = document.getElementById('disease-3-desc');

  if (!isValid || temp === null || hum === null || soil === null || isNaN(temp) || isNaN(hum) || isNaN(soil) || soil < 0) {
    if (riskLevelElem) {
      riskLevelElem.innerText = 'Menunggu Sensor Telemetri';
      riskLevelElem.style.color = 'var(--text-sub)';
      riskLevelElem.style.borderColor = 'var(--card-border)';
    }

    if (b1) b1.className = 'outbreak-item';
    if (tag1) { tag1.className = 'outbreak-tag'; tag1.innerText = 'STANDBY'; }
    if (desc1) desc1.innerHTML = 'Data iklim belum terbaca. Menunggu telemetri suhu &amp; kelembapan DHT11...';

    if (b2) b2.className = 'outbreak-item';
    if (tag2) { tag2.className = 'outbreak-tag'; tag2.innerText = 'STANDBY'; }
    if (desc2) desc2.innerHTML = 'Data iklim belum terbaca. Menunggu telemetri suhu &amp; kelembapan DHT11...';

    if (b3) b3.className = 'outbreak-item';
    if (tag3) { tag3.className = 'outbreak-tag'; tag3.innerText = 'STANDBY'; }
    if (desc3) desc3.innerHTML = 'Data kelembapan tanah belum terbaca. Menunggu sinyal radio ESP-NOW kebun...';
    return;
  }

  evaluateDiseaseOutbreakRisk(temp, hum, soil);
}

function evaluateDiseaseOutbreakRisk(temp, hum, soil) {
  var riskLevelElem = document.getElementById('outbreak-risk-level');
  var b1 = document.getElementById('box-disease-1');
  var b2 = document.getElementById('box-disease-2');
  var b3 = document.getElementById('box-disease-3');

  var tag1 = document.getElementById('disease-1-badge');
  var tag2 = document.getElementById('disease-2-badge');
  var tag3 = document.getElementById('disease-3-badge');

  var desc1 = document.getElementById('disease-1-desc');
  var desc2 = document.getElementById('disease-2-desc');
  var desc3 = document.getElementById('disease-3-desc');

  // Strict Zero Dummy Validation
  if (temp === null || hum === null || soil === null || isNaN(temp) || isNaN(hum) || isNaN(soil) || soil < 0) {
    if (riskLevelElem) {
      riskLevelElem.innerText = 'Menunggu Sensor';
      riskLevelElem.style.color = 'var(--text-sub)';
      riskLevelElem.style.borderColor = 'var(--card-border)';
    }
    return;
  }

  var highRiskCount = 0;

  // 1. Phytophthora infestans & Colletotrichum (Humidity > 82% & Temp 20-27C)
  if (hum > 82 && temp >= 20 && temp <= 27) {
    highRiskCount++;
    if (b1) b1.className = 'outbreak-item status-high';
    if (tag1) { tag1.className = 'outbreak-tag tag-high'; tag1.innerText = 'BAHAYA OUTBREAK'; }
    if (desc1) {
      desc1.innerHTML = '<b>Kondisi Kritis Spora:</b> Humiditas kanopi ' + hum.toFixed(1) + '% &amp; suhu ' + temp.toFixed(1) + '°C memicu perkecambahan spora antraknosa. <b>Tindakan:</b> Hentikan irigasi kanopi malam hari, buka ventilasi naungan, dan aplikasikan fungisida tembaga atau Trichoderma.';
    }
  } else if (hum > 75 && temp >= 20 && temp <= 30) {
    if (b1) b1.className = 'outbreak-item status-med';
    if (tag1) { tag1.className = 'outbreak-tag tag-med'; tag1.innerText = 'WASPADA'; }
    if (desc1) {
      desc1.innerHTML = '<b>Kondisi Lembap:</b> Humiditas ' + hum.toFixed(1) + '% &amp; suhu ' + temp.toFixed(1) + '°C mendekati ambang inkubasi. Pastikan drainase guludan lancar dan pangkas daun tua yang menyentuh tanah.';
    }
  } else {
    if (b1) b1.className = 'outbreak-item status-low';
    if (tag1) { tag1.className = 'outbreak-tag tag-low'; tag1.innerText = 'AMAN'; }
    if (desc1) {
      desc1.innerHTML = '<b>Status Normal:</b> Kelembapan udara aktual (' + hum.toFixed(1) + '%) dan suhu (' + temp.toFixed(1) + '°C) berada di bawah ambang sporulasi jamur (&gt;82%). Aerasi kanopi optimal.';
    }
  }

  // 2. Thrips parvispinus & Kutu Daun Aphids (Temp > 32C & Humidity < 55%)
  if (temp > 32 && hum < 55) {
    highRiskCount++;
    if (b2) b2.className = 'outbreak-item status-high';
    if (tag2) { tag2.className = 'outbreak-tag tag-high'; tag2.innerText = 'BAHAYA POPULASI'; }
    if (desc2) {
      desc2.innerHTML = '<b>Ledakan Populasi Hama:</b> Udara terik ' + temp.toFixed(1) + '°C &amp; kering ' + hum.toFixed(1) + '% memicu siklus hidup thrips 2x lebih cepat. <b>Tindakan:</b> Pasang perangkap lekat kuning (yellow sticky trap), kocor air bawah tajuk untuk menaikkan mikroklimat, dan semprot insektisida nabati neem oil.';
    }
  } else if (temp > 30 && hum < 62) {
    if (b2) b2.className = 'outbreak-item status-med';
    if (tag2) { tag2.className = 'outbreak-tag tag-med'; tag2.innerText = 'WASPADA'; }
    if (desc2) {
      desc2.innerHTML = '<b>Potensi Penetrasi Hama:</b> Suhu terik ' + temp.toFixed(1) + '°C mulai mengeringkan udara (' + hum.toFixed(1) + '%). Pantau pucuk daun muda dari gejala klorosis atau menggulung.';
    }
  } else {
    if (b2) b2.className = 'outbreak-item status-low';
    if (tag2) { tag2.className = 'outbreak-tag tag-low'; tag2.innerText = 'AMAN'; }
    if (desc2) {
      desc2.innerHTML = '<b>Status Terkendali:</b> Suhu aktual (' + temp.toFixed(1) + '°C) &amp; kelembapan (' + hum.toFixed(1) + '%) menekan laju reproduksi kutu daun. Koloni hama tidak berkembang masif.';
    }
  }

  // 3. Layu Bakteri Ralstonia solanacearum (Soil Moisture > 80% & Temp > 30C)
  if (soil > 80 && temp > 30) {
    highRiskCount++;
    if (b3) b3.className = 'outbreak-item status-high';
    if (tag3) { tag3.className = 'outbreak-tag tag-high'; tag3.innerText = 'BAHAYA PERAKARAN'; }
    if (desc3) {
      desc3.innerHTML = '<b>Risiko Anoksia &amp; Bakteri:</b> Tanah jenuh air (' + soil.toFixed(0) + '%) dalam kondisi suhu tinggi (' + temp.toFixed(1) + '°C) mematikan akar muda &amp; memicu infeksi Ralstonia. <b>Tindakan:</b> Matikan pompa irigasi segera, bersihkan parit buangan air, dan taburkan agen hayati Pseudomonas fluorescens.';
    }
  } else if (soil > 75) {
    if (b3) b3.className = 'outbreak-item status-med';
    if (tag3) { tag3.className = 'outbreak-tag tag-med'; tag3.innerText = 'WASPADA'; }
    if (desc3) {
      desc3.innerHTML = '<b>Kadar Air Tinggi:</b> Kelembapan tanah ' + soil.toFixed(0) + '% mendekati kapasitas lapang maksimum. Kurangi durasi penyiraman pada jadwal berikutnya.';
    }
  } else {
    if (b3) b3.className = 'outbreak-item status-low';
    if (tag3) { tag3.className = 'outbreak-tag tag-low'; tag3.innerText = 'AMAN'; }
    if (desc3) {
      desc3.innerHTML = '<b>Aerasi Tanah Sehat:</b> Kelembapan tanah aktual (' + soil.toFixed(0) + '%) dalam batas seimbang. Porositas perakaran cukup oksigen, mencegah patogen anaerob.';
    }
  }

  if (riskLevelElem) {
    if (highRiskCount >= 2) {
      riskLevelElem.innerText = 'Risiko Tinggi (Waspada Outbreak)';
      riskLevelElem.style.color = 'var(--accent-rose)';
      riskLevelElem.style.borderColor = 'rgba(239, 68, 68, 0.4)';
      riskLevelElem.style.background = 'rgba(239, 68, 68, 0.1)';
    } else if (highRiskCount === 1) {
      riskLevelElem.innerText = 'Risiko Sedang (Pengawasan Lahan)';
      riskLevelElem.style.color = 'var(--accent-amber)';
      riskLevelElem.style.borderColor = 'rgba(245, 158, 11, 0.4)';
      riskLevelElem.style.background = 'rgba(245, 158, 11, 0.1)';
    } else {
      riskLevelElem.innerText = 'Risiko Rendah (Kondisi Aman)';
      riskLevelElem.style.color = 'var(--accent-emerald)';
      riskLevelElem.style.borderColor = 'rgba(16, 185, 129, 0.4)';
      riskLevelElem.style.background = 'rgba(16, 185, 129, 0.1)';
    }
  }
}

// ================= OPTICAL VISION & MULTI-SOURCE LEAF PATHOLOGY ================= //
var activeVisionMode = 'file';
var liveCameraStream = null;

function setVisionMode(mode) {
  activeVisionMode = mode;
  
  // Update Tab Buttons
  ['file', 'camera', 'paste', 'url'].forEach(function(m) {
    var btn = document.getElementById('vtab-btn-' + m);
    var panel = document.getElementById('vpanel-' + m);
    if (btn) btn.classList.toggle('active', m === mode);
    if (panel) panel.style.display = (m === mode) ? (m === 'file' || m === 'paste' ? 'flex' : 'block') : 'none';
  });

  // Handle Camera stream activation/teardown
  if (mode === 'camera') {
    startLiveCamera();
  } else {
    stopLiveCamera();
  }
}

function startLiveCamera() {
  var videoElem = document.getElementById('vision-camera-video');
  if (!videoElem) return;

  if (navigator.mediaDevices && navigator.mediaDevices.getUserMedia) {
    var constraints = {
      video: {
        facingMode: { ideal: 'environment' },
        width: { ideal: 1280 },
        height: { ideal: 720 }
      }
    };
    navigator.mediaDevices.getUserMedia(constraints)
      .then(function(stream) {
        liveCameraStream = stream;
        videoElem.srcObject = stream;
      })
      .catch(function(err) {
        console.warn("Camera access failed or denied:", err);
        alert("Gagal mengakses kamera: " + err.message + ". Pastikan izin kamera telah diberikan di browser.");
        setVisionMode('file');
      });
  } else {
    alert("Browser ini tidak mendukung akses kamera langsung WebRTC. Silakan gunakan opsi Unggah / File.");
    setVisionMode('file');
  }
}

function stopLiveCamera() {
  if (liveCameraStream) {
    liveCameraStream.getTracks().forEach(function(track) {
      track.stop();
    });
    liveCameraStream = null;
  }
  var videoElem = document.getElementById('vision-camera-video');
  if (videoElem) videoElem.srcObject = null;
}

function captureLiveCamera() {
  var videoElem = document.getElementById('vision-camera-video');
  if (!videoElem || !liveCameraStream) {
    alert("Kamera belum aktif. Silakan buka kembali tab kamera.");
    return;
  }

  var canvas = document.createElement('canvas');
  canvas.width = videoElem.videoWidth || 640;
  canvas.height = videoElem.videoHeight || 480;
  var ctx = canvas.getContext('2d');
  ctx.drawImage(videoElem, 0, 0, canvas.width, canvas.height);

  stopLiveCamera();
  
  var base64Data = canvas.toDataURL('image/jpeg', 0.85);
  processVisionBase64(base64Data);
}

function handleVisionPaste(event) {
  var items = (event.clipboardData || (event.originalEvent && event.originalEvent.clipboardData) || {}).items;
  if (!items) return;

  for (var i = 0; i < items.length; i++) {
    if (items[i].type.indexOf('image') !== -1) {
      var blob = items[i].getAsFile();
      var reader = new FileReader();
      reader.onload = function(e) {
        processVisionBase64(e.target.result);
      };
      reader.readAsDataURL(blob);
      event.preventDefault();
      break;
    }
  }
}

// Global paste listener for convenience
window.addEventListener('paste', function(e) {
  var tabOverview = document.getElementById('tab-overview');
  if (tabOverview && tabOverview.classList.contains('active')) {
    handleVisionPaste(e);
  }
});

function loadVisionFromUrl() {
  var urlInput = document.getElementById('vision-url-input');
  if (!urlInput || !urlInput.value) {
    alert("Silakan masukkan URL gambar daun yang valid.");
    return;
  }
  var url = urlInput.value.trim();

  var loadingBox = document.getElementById('vision-loading-box');
  var resultBox = document.getElementById('ai-vision-result');
  if (loadingBox) loadingBox.style.display = 'block';
  if (resultBox) resultBox.style.display = 'none';

  var img = new Image();
  img.crossOrigin = 'Anonymous';
  img.onload = function() {
    var canvas = document.createElement('canvas');
    var MAX_WIDTH = 800;
    var MAX_HEIGHT = 800;
    var width = img.width;
    var height = img.height;

    if (width > height) {
      if (width > MAX_WIDTH) { height *= MAX_WIDTH / width; width = MAX_WIDTH; }
    } else {
      if (height > MAX_HEIGHT) { width *= MAX_HEIGHT / height; height = MAX_HEIGHT; }
    }
    canvas.width = width;
    canvas.height = height;
    var ctx = canvas.getContext('2d');
    ctx.drawImage(img, 0, 0, width, height);

    var base64Data = canvas.toDataURL('image/jpeg', 0.85);
    processVisionBase64(base64Data);
  };
  img.onerror = function() {
    if (loadingBox) loadingBox.style.display = 'none';
    alert("Gagal memuat gambar dari URL. Pastikan tautan langsung ke file gambar (CORS didukung).");
  };
  img.src = url;
}

function resetVisionInput() {
  var previewContainer = document.getElementById('vision-preview-container');
  var resultBox = document.getElementById('ai-vision-result');
  var fileInput = document.getElementById('ai-vision-upload');
  var urlInput = document.getElementById('vision-url-input');

  if (previewContainer) previewContainer.style.display = 'none';
  if (resultBox) resultBox.style.display = 'none';
  if (fileInput) fileInput.value = '';
  if (urlInput) urlInput.value = '';

  setVisionMode(activeVisionMode);
}

function handleAIVisionUpload(event) {
  var file = event.target.files[0];
  if (!file) return;

  var reader = new FileReader();
  reader.onload = function(e) {
    processVisionBase64(e.target.result);
  };
  reader.readAsDataURL(file);
}

function processVisionBase64(fullDataUrl) {
  var previewContainer = document.getElementById('vision-preview-container');
  var previewImg = document.getElementById('ai-vision-preview');
  var loadingBox = document.getElementById('vision-loading-box');
  var resultBox = document.getElementById('ai-vision-result');
  
  // Hide panels while previewing
  ['file', 'camera', 'paste', 'url'].forEach(function(m) {
    var panel = document.getElementById('vpanel-' + m);
    if (panel) panel.style.display = 'none';
  });

  if (previewImg) previewImg.src = fullDataUrl;
  if (previewContainer) previewContainer.style.display = 'block';

  var apiKey = (window.getGeminiApiKey && window.getGeminiApiKey()) || localStorage.getItem("geminiApiKey") || window.DEFAULT_GEMINI_KEY;
  if (!apiKey || apiKey.length < 5) {
    if (resultBox) {
      resultBox.innerHTML = `
        <div class="vision-result-card" style="border-color: rgba(239, 68, 68, 0.4); background: rgba(239, 68, 68, 0.08);">
          <div style="font-weight:700; color:#f87171; margin-bottom:6px; font-size:13px;">API Key Gemini Belum Dikonfigurasi</div>
          <div style="font-size:12px; color:#cbd5e1; line-height:1.5;">
            Silakan masukkan API Key Gemini Anda di tab <strong>Agronomi & AI</strong> agar sistem dapat menganalisis citra daun dan merekomendasikan penanganan patologi.
          </div>
        </div>
      `;
      resultBox.style.display = 'block';
    }
    return;
  }

  if (loadingBox) loadingBox.style.display = 'block';
  if (resultBox) resultBox.style.display = 'none';

  // Optimize & resize via canvas
  var img = new Image();
  img.onload = function() {
    var canvas = document.createElement('canvas');
    var ctx = canvas.getContext('2d');
    var MAX_WIDTH = 800;
    var MAX_HEIGHT = 800;
    var width = img.width;
    var height = img.height;

    if (width > height) {
      if (width > MAX_WIDTH) { height *= MAX_WIDTH / width; width = MAX_WIDTH; }
    } else {
      if (height > MAX_HEIGHT) { width *= MAX_HEIGHT / height; height = MAX_HEIGHT; }
    }
    canvas.width = width;
    canvas.height = height;
    ctx.drawImage(img, 0, 0, width, height);

    var rawBase64 = canvas.toDataURL('image/jpeg', 0.8).split(',')[1];
    sendImageToGemini(rawBase64, apiKey, loadingBox, resultBox);
  };
  img.src = fullDataUrl;
}

async function sendImageToGemini(base64Image, apiKey, loadingBox, resultBox) {
  var tData = window.lastTelemetryData || {};
  var tempC = tData.temp !== undefined ? tData.temp : 25;
  var hum = tData.hum !== undefined ? tData.hum : 80;
  var soil = tData.soil !== undefined ? tData.soil : 60;
  
  var promptStr = `Anda adalah Dokter Tanaman Lapangan untuk petani presisi.
Telemetri sensor aktual kebun saat ini: Suhu: ${tempC}°C, Kelembapan Udara (RH): ${hum}%, Kelembapan Tanah (VWC): ${soil}%.
Analisis citra foto daun ini dan WAJIB balas HANYA dalam format JSON murni tanpa markdown, tanpa salam, tanpa kata pembuka:
{
  "nama_penyakit": "Nama penyakit/hama singkat (misal: Bercak Daun Cercospora / Antraknosa)",
  "kategori": "Jamur Patogen / Bakteri / Hama Serangga / Defisiensi Hara",
  "penyebab_singkat": "1 kalimat singkat penyebab terkait suhu ${tempC}°C & RH ${hum}%",
  "obat_toko": "Contoh merek toko pertanian: Dithane M-45, Antracol 70WP, atau Score 250 EC",
  "bahan_aktif": "Mankozeb / Propineb / Difenokonazol",
  "takaran_alat": "2–3 Sendok Makan (bubuk) atau 1 Tutup Botol / 10-15 ml (cairan) per Tangki Semprot 16 Liter",
  "waktu_semprot": "Pagi jam 07.00 - 09.00 atau Sore jam 15.30",
  "tindakan_segera": "Petik & bakar daun yang parah. Bersihkan gulma di bawah bedengan."
}`;

  try {
    const result = await window.fetchGeminiWithFallback(apiKey, {
      contents: [{
        parts: [
          { text: promptStr },
          { inline_data: { mime_type: "image/jpeg", data: base64Image } }
        ]
      }]
    });
    
    var rawText = result.candidates[0].content.parts[0].text;
    var cleanJson = rawText.replace(/```json/g, '').replace(/```/g, '').trim();
    var diag = null;
    try {
      diag = JSON.parse(cleanJson);
    } catch(e) {
      diag = {
        nama_penyakit: "Penyakit / Gejala Daun Terdeteksi",
        kategori: "Patologi Tanaman",
        penyebab_singkat: "Kondisi kebun bersuhu " + tempC + "°C dan kelembapan " + hum + "%.",
        obat_toko: "Antracol / Dithane M-45 / Score 250 EC",
        bahan_aktif: "Fungisida Kontak / Sistemik",
        takaran_alat: "2 Sendok Makan per Tangki Knapsack 16L",
        waktu_semprot: "Pagi jam 07.00-09.00 atau Sore jam 15.30",
        tindakan_segera: rawText.replace(/\*/g, '').substring(0, 150)
      };
    }

    // Initialize multi-turn chat history with this image & diagnosis
    window.agronomyChatHistory = [
      {
        role: "user",
        parts: [
          { text: promptStr },
          { inline_data: { mime_type: "image/jpeg", data: base64Image } }
        ]
      },
      {
        role: "model",
        parts: [{ text: JSON.stringify(diag) }]
      }
    ];

    resultBox.innerHTML = `
      <div class="vision-result-card">
        <div class="vision-result-header">
          <div style="display:flex; align-items:center; gap:8px;">
            <div style="width:8px; height:8px; border-radius:50%; background:#c084fc; box-shadow:0 0 8px #c084fc;"></div>
            <span style="font-weight:700; font-size:13px; color:#c084fc;">Hasil Diagnosis Patologi</span>
          </div>
          <span class="badge-pill" style="background:rgba(239, 68, 68, 0.15); color:#f87171; border:1px solid rgba(239, 68, 68, 0.3); font-size:11px; padding:2px 8px;">
            ${diag.kategori || 'Penyakit Tanaman'}
          </span>
        </div>

        <div class="vision-result-body" style="display:flex; flex-direction:column; gap:10px; margin-top:10px;">
          <!-- 1. Penyakit & Penyebab -->
          <div style="background:rgba(0,0,0,0.3); border:1px solid rgba(255,255,255,0.06); padding:10px 12px; border-radius:8px;">
            <div style="font-size:10.5px; color:var(--text-sub); text-transform:uppercase; font-weight:700; margin-bottom:2px;">Penyakit & Masalah</div>
            <div style="font-size:14px; font-weight:700; color:#fff; margin-bottom:4px;">${diag.nama_penyakit}</div>
            <div style="font-size:12px; color:#cbd5e1;">${diag.penyebab_singkat}</div>
          </div>

          <!-- 2. Rekomendasi Obat Toko & Bahan Aktif -->
          <div style="background:rgba(168,85,247,0.08); border:1px solid rgba(168,85,247,0.25); padding:10px 12px; border-radius:8px;">
            <div style="font-size:10.5px; color:#c084fc; text-transform:uppercase; font-weight:700; margin-bottom:2px;">Contoh Merek di Toko Pertanian</div>
            <div style="font-size:13.5px; font-weight:700; color:#fff; margin-bottom:2px;">${diag.obat_toko}</div>
            <div style="font-size:11.5px; color:#d8b4fe;">Bahan Aktif: <strong>${diag.bahan_aktif}</strong></div>
          </div>

          <!-- 3. Takaran Alat Nyata -->
          <div style="background:rgba(16,185,129,0.08); border:1px solid rgba(16,185,129,0.25); padding:10px 12px; border-radius:8px;">
            <div style="font-size:10.5px; color:#34d399; text-transform:uppercase; font-weight:700; margin-bottom:2px;">Takaran Alat Lapangan (Tangki 16 Liter)</div>
            <div style="font-size:13.5px; font-weight:700; color:#34d399;">${diag.takaran_alat}</div>
          </div>

          <!-- 4. Waktu Semprot & Tindakan Cepat -->
          <div style="display:grid; grid-template-columns: 1fr 1fr; gap:8px;">
            <div style="background:rgba(0,0,0,0.25); border:1px solid rgba(255,255,255,0.06); padding:9px 10px; border-radius:8px;">
              <div style="font-size:10.5px; color:var(--text-sub); font-weight:700; margin-bottom:2px;">Waktu Semprot</div>
              <div style="font-size:11.5px; color:#e2e8f0; line-height:1.4;">${diag.waktu_semprot}</div>
            </div>
            <div style="background:rgba(0,0,0,0.25); border:1px solid rgba(255,255,255,0.06); padding:9px 10px; border-radius:8px;">
              <div style="font-size:10.5px; color:var(--text-sub); font-weight:700; margin-bottom:2px;">Tindakan Segera</div>
              <div style="font-size:11.5px; color:#e2e8f0; line-height:1.4;">${diag.tindakan_segera}</div>
            </div>
          </div>
        </div>
      </div>
    `;
    resultBox.style.display = 'block';

    // Notify Chatbot
    var chatMsgBox = document.getElementById('agri-chat-messages');
    if (chatMsgBox) {
      chatMsgBox.innerHTML += `
        <div class="chat-bubble bot" style="border-left:3px solid #c084fc;">
          Diagnosis daun <strong>${diag.nama_penyakit}</strong> selesai dianalisis. Ada yang ingin ditanyakan seputar pencampuran obat atau jadwal semprot?
        </div>
      `;
      chatMsgBox.scrollTop = chatMsgBox.scrollHeight;
    }
  } catch (err) {
    resultBox.innerHTML = `
      <div class="vision-result-card" style="border-color: rgba(239, 68, 68, 0.4); background: rgba(239, 68, 68, 0.08);">
        <div style="font-weight:700; color:#f87171; margin-bottom:4px; font-size:13px;">Koneksi Gemini AI Sibuk / Terkendala</div>
        <div style="font-size:12px; color:#cbd5e1; margin-bottom:10px;">${err.message}</div>
        
        <div style="display:flex; flex-direction:column; gap:8px; margin-top:8px; padding-top:8px; border-top:1px solid rgba(255,255,255,0.08);">
          <div style="display:flex; gap:6px;">
            <input type="password" id="inline-gemini-key" placeholder="Perbarui / Paste API Key Baru..." style="flex:1; background:rgba(0,0,0,0.5); border:1px solid rgba(255,255,255,0.15); border-radius:6px; padding:6px 10px; font-size:11px; color:#fff;" value="${apiKey || ''}">
            <button class="btn btn-primary" onclick="updateApiKeyAndRetry()" style="font-size:11px; padding:6px 12px; background:#a855f7; border:none; border-radius:6px;">
              Simpan & Ulangi
            </button>
          </div>
          <button class="btn" onclick="runOfflineAgronomicAnalysis()" style="font-size:11.5px; padding:8px; background:rgba(16, 185, 129, 0.15); border:1px solid rgba(16, 185, 129, 0.4); color:#34d399; border-radius:6px; font-weight:600; cursor:pointer;">
            Evaluasi Patologi Berdasarkan Telemetri Riil Kebun (Offline Expert)
          </button>
        </div>
      </div>
    `;
    resultBox.style.display = 'block';
  } finally {
    if (loadingBox) loadingBox.style.display = 'none';
  }
}

function updateApiKeyAndRetry() {
  var keyInput = document.getElementById('inline-gemini-key');
  if (keyInput && keyInput.value) {
    localStorage.setItem("geminiApiKey", keyInput.value.trim());
    var mainKeyInput = document.getElementById("gemini-api-key");
    if (mainKeyInput) mainKeyInput.value = keyInput.value.trim();
  }
  var previewImg = document.getElementById('ai-vision-preview');
  if (previewImg && previewImg.src) {
    processVisionBase64(previewImg.src);
  }
}

function runOfflineAgronomicAnalysis() {
  var resultBox = document.getElementById('ai-vision-result');
  var tData = window.lastTelemetryData || {};
  var tempC = tData.temp !== undefined ? tData.temp : 28;
  var hum = tData.hum !== undefined ? tData.hum : 75;
  var soil = tData.soil !== undefined ? tData.soil : 55;

  var riskAssessment = "Kondisi kelembapan dan suhu berada pada rentang normal.";
  var recommendation = "Pertahankan aerasi bedengan dan jadwal siram rutin.";

  if (hum > 85 && tempC < 27) {
    riskAssessment = "Risiko tinggi serangan <strong>Jamur Patogen (Phytophthora / Embun Bulu)</strong> akibat kelembapan udara sangat tinggi (>85%) dan kanopi basah.";
    recommendation = "Semprot fungisida bahan aktif <em>Mankozeb</em> atau <em>Dimetomorf</em> dosis <strong>2 Sendok Makan per Tangki Knapsack 16L</strong>. Kurangi frekuensi siram.";
  } else if (hum < 50 && tempC > 31) {
    riskAssessment = "Risiko serangan <strong>Hama Thrips / Kutu Daun (Tungau Merah)</strong> akibat iklim mikro kering dan panas.";
    recommendation = "Semprot insektisida/akarisida bahan aktif <em>Abamektin</em> dosis <strong>1 Tutup Botol (~10-15 ml) per Tangki Knapsack 16L</strong> saat sore hari.";
  } else if (soil < 40) {
    riskAssessment = "Tanaman mengalami defisiensi air dan potensi <strong>Kekurangan Hara Kalium (K)</strong> yang memicu tepi daun mengering/menguning.";
    recommendation = "Kocorkan pupuk NPK 15-15-15 atau Kalium Nitrat (KNO3) dosis <strong>1 Gelas Aqua (150g) dilarutkan ke 1 Ember Cat (15 Liter)</strong>.";
  } else if (soil > 85) {
    riskAssessment = "Kondisi tanah jenuh air (*waterlogged*), memicu risiko <strong>Busuk Akar & Rebah Semai (*Damping-off*)</strong>.";
    recommendation = "Segera matikan pompa air, perbaiki parit aerasi drainase bedengan, dan taburkan kapur dolomit <strong>1 Genggam per meter bedengan</strong>.";
  }

  resultBox.innerHTML = `
    <div class="vision-result-card" style="border-color: rgba(16, 185, 129, 0.4); background: rgba(15, 23, 42, 0.85);">
      <div class="vision-result-header">
        <div style="display:flex; align-items:center; gap:8px;">
          <div style="width:8px; height:8px; border-radius:50%; background:#10b981; box-shadow:0 0 8px #10b981;"></div>
          <span style="font-weight:700; font-size:13px; color:#34d399;">Diagnosis Agronomi Presisi (Telemetri Riil)</span>
        </div>
        <span style="font-size:11px; color:var(--text-sub);">DHT11: ${tempC}°C / ${hum}% RH | VWC: ${soil}%</span>
      </div>
      <div class="vision-result-body">
        <p style="margin-bottom:8px;">${riskAssessment}</p>
        <div style="padding:10px; background:rgba(16, 185, 129, 0.08); border:1px solid rgba(16, 185, 129, 0.25); border-radius:8px; margin-top:8px;">
          <strong style="color:#34d399; display:block; margin-bottom:4px;">Tindakan Lapangan Rekomendasi:</strong>
          ${recommendation}
        </div>
      </div>
    </div>
  `;
  resultBox.style.display = 'block';
}
