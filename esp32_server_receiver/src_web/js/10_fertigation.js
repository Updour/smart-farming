
function getPracticalUnit(grams) {
  if (grams < 15) return "Sejumput / Sedikit Sekali";
  if (grams <= 150) {
    var sdm = Math.round(grams / 15);
    return "~" + sdm + " Sendok Makan";
  }
  if (grams <= 1000) {
    var genggam = Math.round(grams / 50);
    var gelas = (grams / 200).toFixed(1);
    return "~" + gelas + " Gelas Aqua / " + genggam + " Genggam";
  }
  if (grams <= 50000) {
    var ember = (grams / 5000).toFixed(1);
    return "~" + ember + " Ember Cat (5kg)";
  }
  var karung = (grams / 50000).toFixed(1);
  return "~" + karung + " Karung (50kg)";
}
/**
 * 10_fertigation.js - Fertigation Dose, Macro/Micro Nutrient Calculator & Financial ROI
 * Precision Agriculture Engineering, Strict line limit < 400 lines
 */

var fertSpecs = {
  'npk_phonska': { name: 'NPK Phonska / NPK Mutiara 15-15-15', defaultPrice: 18000, dosePerM2: 2.5, ratio: 5, advice: 'Pupuk majemuk NPK seimbang untuk mendukung fase vegetatif & generatif secara konsisten.' },
  'urea': { name: 'UREA (Nitrogen 46%)', defaultPrice: 9000, dosePerM2: 1.8, ratio: 4, advice: 'Memacu pertumbuhan batang & hijau daun. Wajib langsung dikocor / dibenamkan agar Nitrogen tidak menguap (volatilisasi).' },
  'sp36': { name: 'SP-36 / TSP (Fosfat 36%)', defaultPrice: 12000, dosePerM2: 2.0, ratio: 5, advice: 'Sangat penting pada awal tanam untuk memacu perpanjangan akar, anakan, dan kekuatan struktur sel.' },
  'kcl': { name: 'KCL / MOP (Kalium 60%)', defaultPrice: 16000, dosePerM2: 2.2, ratio: 4, advice: 'Meningkatkan bobot buah, kadar gula/manis, ketebalan kulit buah, dan daya simpan panen.' },
  'za': { name: 'ZA (Sulfur 24% + N 21%)', defaultPrice: 8500, dosePerM2: 1.5, ratio: 4, advice: 'Kandungan Sulfur tinggi sangat penting untuk bawang merah & cabai demi ketajaman aroma, warna merah, dan rasa pedas.' },
  'kno3_putih': { name: 'KNO3 Putih (13-0-45)', defaultPrice: 38000, dosePerM2: 1.2, ratio: 3, advice: 'Pupuk premium pembungaan generatif tanpa klorida. Mencegah gugur bunga tanpa membuat media tanah asam.' },
  'mkp': { name: 'Pupuk MKP (0-52-34)', defaultPrice: 55000, dosePerM2: 1.0, ratio: 2.5, advice: 'Kombinasi tinggi Fosfat & Kalium murni. Sangat efektif untuk merangsang pembungaan serempak dan mencegah rontok calon buah.' },
  'ab_mix': { name: 'AB Mix Hydroponic / Fertigasi Presisi', defaultPrice: 35000, dosePerM2: 2.0, ratio: 5, advice: 'Formulasi hara makro & mikro terlarut 100% tanpa endapan. Cocok untuk sistem fertigasi drip otomatis.' },
  'dolomit': { name: 'Kapur Dolomit (CaMg)', defaultPrice: 2000, dosePerM2: 10.0, ratio: 10, advice: 'Mengandung Kalsium & Magnesium. Diaplikasikan pada media tanah yang asam (pH < 5.5) untuk menetralkan pH tanah.' },
  'humic': { name: 'Asam Humat Organik (Humic Acid)', defaultPrice: 45000, dosePerM2: 0.5, ratio: 1, advice: 'Pembenah struktur tanah organik. Meningkatkan Kapasitas Tukar Kation (KTK) dan memperlancar penyerapan NPK oleh akar.' }
};

function onFertilizerTypeChange(isManualEdit = false) {
  var type = document.getElementById('fert-formula') ? document.getElementById('fert-formula').value : 'npk_phonska';
  var spec = fertSpecs[type] || fertSpecs['npk_phonska'];
  var priceInput = document.getElementById('fert-price-kg');
  
  // Jika ini diubah dari dropdown pupuk, gunakan default price
  if (!isManualEdit && priceInput) {
    priceInput.value = spec.defaultPrice;
  }
  
  // Save to LocalStorage
  if (priceInput) localStorage.setItem('fert_price', priceInput.value);
  localStorage.setItem('fert_type', type);
  
  calculateFertigationDose();
}

function initFertigation() {
  var savedPrice = localStorage.getItem('fert_price');
  var savedType = localStorage.getItem('fert_type');
  var savedMethod = localStorage.getItem('fert_method');
  
  var sPlant = localStorage.getItem('fin_plant_count');
  var sYield = localStorage.getItem('fin_yield');
  var sPrice = localStorage.getItem('fin_price');
  var sCost = localStorage.getItem('fin_cost');
  var sArea = localStorage.getItem('crop_area');
  var sStage = localStorage.getItem('crop_stage');
  var sName = localStorage.getItem('crop_name');
  
  if (savedType && document.getElementById('fert-formula')) document.getElementById('fert-formula').value = savedType;
  if (savedPrice && document.getElementById('fert-price-kg')) document.getElementById('fert-price-kg').value = savedPrice;
  if (savedMethod && document.getElementById('fert-method')) document.getElementById('fert-method').value = savedMethod;
  
  if (sPlant && document.getElementById('fin-plant-count')) document.getElementById('fin-plant-count').value = sPlant;
  if (sYield && document.getElementById('fin-yield-per-plant')) document.getElementById('fin-yield-per-plant').value = sYield;
  if (sPrice && document.getElementById('fin-market-price')) document.getElementById('fin-market-price').value = sPrice;
  if (sCost && document.getElementById('fin-modal-cost')) document.getElementById('fin-modal-cost').value = sCost;
  
  if (sArea && document.getElementById('crop-area-size')) document.getElementById('crop-area-size').value = sArea;
  if (sStage && document.getElementById('crop-stage')) document.getElementById('crop-stage').value = sStage;
  if (sName && document.getElementById('crop-name')) document.getElementById('crop-name').value = sName;
  
  calculateFertigationDose();
  if (typeof calculateFinancialROI === 'function') calculateFinancialROI();
}
// Panggil init saat modul dimuat
setTimeout(initFertigation, 500);


function calculateFertigationDose() {
  var type = document.getElementById('fert-formula') ? document.getElementById('fert-formula').value : 'npk_phonska';
  var method = document.getElementById('fert-method') ? document.getElementById('fert-method').value : 'kocor';
  var priceKg = parseFloat(document.getElementById('fert-price-kg') ? document.getElementById('fert-price-kg').value : 18000) || 18000;
  var area = parseInt(document.getElementById('crop-area-size') ? document.getElementById('crop-area-size').value : 100, 10) || 100;
  var stage = document.getElementById('crop-stage') ? document.getElementById('crop-stage').value : 'semai';

  var spec = fertSpecs[type] || fertSpecs['npk_phonska'];
  var dosePerM2 = spec.dosePerM2;

  if (stage === 'semai') dosePerM2 = spec.dosePerM2 * 0.5;
  else if (stage === 'generatif') dosePerM2 = spec.dosePerM2 * 1.4;

  var totalGrams = Math.round(area * dosePerM2);
  var totalKg = (totalGrams / 1000).toFixed(2);
  var waterLiters = Math.round(totalGrams / spec.ratio);
  var costPerApp = Math.round((totalGrams / 1000) * priceKg);

  var methodText = "<b>Kocor Fertigasi:</b> Larutkan <b>" + totalGrams + " Gram</b> ke dalam <b>" + waterLiters + " Liter Air</b> (" + spec.ratio + " g/L)";
  if (method === 'tabur') {
    methodText = "<b>Tabur / Benam:</b> Dosis <b>" + totalKg + " kg</b> dibenamkan di parit guludan sejarak 10-15 cm dari pangkal batang.";
  } else if (method === 'foliar') {
    methodText = "<b>Semprot Foliar Daun:</b> Larutkan <b>" + Math.round(totalGrams * 0.4) + " Gram</b> ke dalam <b>" + Math.round(waterLiters * 0.8) + " Liter Air Sprayer</b>.";
  }

  var box = document.getElementById('fert-result-box');
  if (box) {
    // ==== INTEGRASI TELEMETRI REAL-TIME (SESUAI ATURAN .AGENTS) ====
    var liveWarning = "";
    var tData = window.lastTelemetryData || {};
    var hasSoil = (tData.soil !== undefined && tData.soil >= 0);
    var hasTemp = (tData.temp !== undefined && !isNaN(parseFloat(tData.temp)));
    var soilVwc = hasSoil ? tData.soil : null;
    var tempC = hasTemp ? parseFloat(tData.temp) : null;
    
    // Save method
    localStorage.setItem('fert_method', method);
    if (document.getElementById('fert-price-kg')) localStorage.setItem('fert_price', priceKg);

    // Evaluasi Kelembapan Tanah (VWC)
    if (soilVwc !== null && soilVwc < 45 && method === 'tabur') {
      liveWarning += "<div style='margin-top:12px; padding:10px; background:rgba(220,38,38,0.15); border-left:4px solid #dc2626; border-radius:4px; color:#fca5a5;'>";
      liveWarning += "<b>⚠️ PERINGATAN KEKERINGAN (Sensor Tanah: " + soilVwc + "%):</b> Tanah terlalu kering untuk aplikasi pupuk tabur. Pupuk tidak akan larut dan berisiko meracuni akar (Plasmolisis). Sangat disarankan beralih ke metode <b>Kocor Fertigasi</b> atau hidupkan Pompa Irigasi sebelum memupuk.</div>";
    }
    
    // Evaluasi Suhu (Foliar)
    if (tempC !== null && tempC > 32.0 && method === 'foliar') {
      liveWarning += "<div style='margin-top:12px; padding:10px; background:rgba(234,179,8,0.15); border-left:4px solid #eab308; border-radius:4px; color:#fde047;'>";
      liveWarning += "<b>⚠️ PERINGATAN SUHU PANAS (Sensor Suhu: " + tempC + "°C):</b> Aplikasi semprot daun (Foliar) pada suhu terik akan menyebabkan penguapan instan dan daun terbakar (Leaf Burn). Tunda penyemprotan hingga sore hari (Suhu < 28°C).</div>";
    }


    if (soilVwc >= 60 && soilVwc <= 80 && method === 'kocor') {
      liveWarning += "<div style='margin-top:12px; padding:10px; background:rgba(16,185,129,0.15); border-left:4px solid #10b981; border-radius:4px; color:#6ee7b7;'>";
      liveWarning += "<b>✅ KONDISI OPTIMAL (Sensor Tanah: " + soilVwc + "%):</b> Kelembapan tanah ideal. Akar tanaman akan menyerap nutrisi fertigasi dengan efisiensi maksimal.</div>";
    }

    var mtdTitle = method === 'tabur' ? 'Tabur / Benam' : method === 'kocor' ? 'Kocor Fertigasi' : 'Semprot Foliar';
    var svgLeaf = '<svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M11 20A7 7 0 0 1 9.8 6.1C15.5 5 17 4.48 19 2c1 2 2 4.18 2 8 0 5.5-4.78 10-10 10Z"/><path d="M2 21c0-3 1.85-5.36 5.08-6C9.5 14.52 12 13 13 12"/></svg>';
    var svgScale = '<svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M20.59 13.41l-7.17 7.17a2 2 0 0 1-2.83 0L2 12V2h10l8.59 8.59a2 2 0 0 1 0 2.82z"></path><line x1="7" y1="7" x2="7.01" y2="7"></line></svg>';
    var svgMoney = '<svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><rect x="2" y="6" width="20" height="12" rx="2"></rect><circle cx="12" cy="12" r="2"></circle><path d="M6 12h.01M18 12h.01"></path></svg>';
    var svgDrop = '<svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M12 2.69l5.66 5.66a8 8 0 1 1-11.31 0z"></path></svg>';
    var svgInfo = '<svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="10"></circle><line x1="12" y1="16" x2="12" y2="12"></line><line x1="12" y1="8" x2="12.01" y2="8"></line></svg>';
    
    var methodDesc = "";
    if (method === 'kocor') methodDesc = "Larutkan <b style='color:var(--text-main);'>" + totalGrams + " Gram</b> ke dalam <b style='color:#38bdf8;'>" + waterLiters + " Liter Air</b> (" + spec.ratio + " g/L). Siramkan merata ke media tanam.";
    else if (method === 'tabur') methodDesc = "Timbang <b style='color:var(--text-main);'>" + totalKg + " kg</b> (" + totalGrams + " g). Benamkan ke parit/lubang berjarak 10-15 cm dari pangkal batang.";
    else if (method === 'foliar') methodDesc = "Larutkan <b style='color:var(--text-main);'>" + Math.round(totalGrams * 0.4) + " Gram</b> ke dalam <b style='color:#38bdf8;'>" + Math.round(waterLiters * 0.8) + " Liter Air</b>. Semprotkan berupa kabut (mist) merata ke stomata daun.";

    var htmlUI = `
      <div style="background: rgba(255,255,255,0.02); border: 1px solid rgba(255,255,255,0.06); border-radius: 12px; overflow: hidden; margin-top:16px;">
        <!-- Header -->
        <div style="background: rgba(16, 185, 129, 0.1); border-bottom: 1px solid rgba(16, 185, 129, 0.15); padding: 14px 18px; display: flex; align-items: center; gap: 12px;">
          <div style="color: #10b981; display:flex; align-items:center; justify-content:center; width:32px; height:32px; background:rgba(16,185,129,0.15); border-radius:8px;">${svgLeaf}</div>
          <div>
            <div style="font-size:11px; font-weight:600; text-transform:uppercase; letter-spacing:0.5px; color:#10b981; margin-bottom:2px;">Rekomendasi Presisi (${area} m²)</div>
            <div style="font-weight:700; color:var(--text-main); font-size:15px; letter-spacing:-0.3px;">${spec.name}</div>
          </div>
        </div>
        
        <!-- Grid Metrics -->
        <div style="display: grid; grid-template-columns: repeat(2, 1fr); gap: 1px; background: rgba(255,255,255,0.05);">
          <!-- Dose Box -->
          <div style="background: rgba(0,0,0,0.3); padding: 16px;">
            <div style="font-size:11px; color:var(--text-sub); display:flex; align-items:center; gap:6px; margin-bottom:8px; font-weight:500;">
              <span style="color:#a78bfa;">${svgScale}</span> Total Dosis
            </div>
            <div style="font-size:22px; font-weight:800; color:var(--text-main); letter-spacing:-0.5px; font-family:'Outfit', sans-serif;">
              ${totalGrams} <span style="font-size:13px; color:var(--text-sub); font-weight:600;">Gram</span>
            </div>
            <div style="font-size:11px; color:var(--text-sub); margin-top:4px; margin-bottom:8px;">≈ ${totalKg} kg (${dosePerM2.toFixed(1)} g/m²)</div>
            <div style="font-size:12px; padding:6px; background:rgba(255,255,255,0.1); border-radius:6px; color:#6ee7b7; font-weight:600; display:inline-block;">
              Takaran Praktis: ${getPracticalUnit(totalGrams)}
            </div>
          </div>
          
          <!-- Cost Box -->
          <div style="background: rgba(0,0,0,0.3); padding: 16px;">
            <div style="font-size:11px; color:var(--text-sub); display:flex; align-items:center; gap:6px; margin-bottom:8px; font-weight:500;">
              <span style="color:#fbbf24;">${svgMoney}</span> Estimasi Biaya
            </div>
            <div style="font-size:20px; font-weight:800; color:#34d399; letter-spacing:-0.5px; font-family:'Outfit', sans-serif;">
              Rp ${costPerApp.toLocaleString('id-ID')}
            </div>
            <div style="font-size:11px; color:var(--text-sub); margin-top:4px;">Per Aplikasi</div>
          </div>
        </div>
        
        <!-- Method Section -->
        <div style="background: rgba(0,0,0,0.2); padding: 16px; border-top: 1px solid rgba(255,255,255,0.03);">
           <div style="font-size:11px; color:var(--text-sub); display:flex; align-items:center; gap:6px; margin-bottom:8px; font-weight:600;">
              <span style="color:#38bdf8;">${svgDrop}</span> Metode Aplikasi: ${mtdTitle}
            </div>
            <div style="font-size:13px; color:#94a3b8; line-height:1.6;">${methodDesc}</div>
        </div>

        <!-- Agronomy Advice -->
        <div style="background: rgba(59, 130, 246, 0.05); padding: 16px; border-top: 1px solid rgba(59, 130, 246, 0.15);">
            <div style="font-size:11px; color:#60a5fa; display:flex; align-items:center; gap:6px; margin-bottom:8px; font-weight:600;">
              <span>${svgInfo}</span> Petunjuk Agronomi
            </div>
            <div style="font-size:13px; color:#bfdbfe; line-height:1.6;">${spec.advice}</div>
        </div>
      </div>
      ${liveWarning}
    `;
    
    box.innerHTML = htmlUI;
  }
}

function calculateFinancialROI() {
  var plantCount = parseInt(document.getElementById('fin-plant-count') ? document.getElementById('fin-plant-count').value : 400, 10) || 400;
  var yieldPerPlant = parseFloat(document.getElementById('fin-yield-per-plant') ? document.getElementById('fin-yield-per-plant').value : 1.2) || 1.2;
  var marketPrice = parseFloat(document.getElementById('fin-market-price') ? document.getElementById('fin-market-price').value : 45000) || 45000;
  var modalCost = parseFloat(document.getElementById('fin-modal-cost') ? document.getElementById('fin-modal-cost').value : 3500000) || 3500000;
  var area = parseInt(document.getElementById('crop-area-size') ? document.getElementById('crop-area-size').value : 100, 10) || 100;

  // Save to LocalStorage
  if (document.getElementById('fin-plant-count')) localStorage.setItem('fin_plant_count', plantCount);
  if (document.getElementById('fin-yield-per-plant')) localStorage.setItem('fin_yield', yieldPerPlant);
  if (document.getElementById('fin-market-price')) localStorage.setItem('fin_price', marketPrice);
  if (document.getElementById('fin-modal-cost')) localStorage.setItem('fin_cost', modalCost);


  var totalKg = plantCount * yieldPerPlant;
  var grossRevenue = totalKg * marketPrice;
  var netProfit = grossRevenue - modalCost;
  var roiPercent = Math.round((netProfit / modalCost) * 100);

  if (document.getElementById('fin-total-kg')) document.getElementById('fin-total-kg').innerText = Math.round(totalKg) + " kg (" + (totalKg / 1000).toFixed(2) + " Ton)";
  if (document.getElementById('fin-productivity')) document.getElementById('fin-productivity').innerText = "Produktivitas " + (totalKg / area).toFixed(1) + " kg/m²";
  if (document.getElementById('fin-gross-revenue')) document.getElementById('fin-gross-revenue').innerText = "Rp " + Math.round(grossRevenue).toLocaleString('id-ID');
  if (document.getElementById('fin-net-profit')) document.getElementById('fin-net-profit').innerText = "Rp " + Math.round(netProfit).toLocaleString('id-ID');

  var roiElem = document.getElementById('fin-roi-percent');
  if (roiElem) {
    roiElem.innerText = "ROI: " + (roiPercent >= 0 ? "+" : "") + roiPercent + "%";
    roiElem.style.color = roiPercent >= 0 ? "var(--accent-emerald)" : "var(--accent-rose)";
  }
}

// ================= GEMINI AI FERTIGATION ================= //
async function askGeminiFertilizer() {
  var apiKey = (window.getGeminiApiKey && window.getGeminiApiKey()) || localStorage.getItem("geminiApiKey") || window.DEFAULT_GEMINI_KEY;
  if (!apiKey || apiKey.length < 5) {
    alert("Silakan masukkan API Key Gemini Anda di tab Agronomi terlebih dahulu.");
    return;
  }
  
  var btn = document.getElementById('btn-gemini-fert');
  if (btn) {
    btn.innerHTML = '⏳ Menganalisis...';
    btn.disabled = true;
  }

  var tData = window.lastTelemetryData;
  if (!tData || tData.soil === undefined || tData.soil < 0 || tData.unplugged) {
    alert("Data sensor fisik ESP32 belum tersedia atau sensor kebun sedang terputus. AI hanya memproses telemetri riil dari lahan!");
    if (btn) {
      btn.innerHTML = '⚡ Analisis Lapangan Praktis (AI)';
      btn.disabled = false;
    }
    return;
  }
  var soilVwc = tData.soil;
  var tempC = tData.temp !== undefined ? tData.temp : '--';
  var fertType = document.getElementById('fert-formula') ? document.getElementById('fert-formula').value : '';
  
  var promptStr = `Saya seorang petani. Lahan saya saat ini bersuhu ${tempC}C dan kelembapan tanah ${soilVwc}%. Saya berencana memberikan pupuk ${fertType}. 
Tolong berikan 3 kalimat saran praktis (tanpa basa-basi) apakah cuaca/kondisi tanah saat ini cocok untuk pemupukan, dan sebutkan alternatif takaran lokal (misal sendok makan, gelas aqua, genggam) yang mudah dipahami petani tanpa timbangan.`;


  try {
    const result = await window.fetchGeminiWithFallback(apiKey, {
      contents: [{ parts: [{ text: promptStr }] }]
    });

    var aiText = result.candidates[0].content.parts[0].text.replace(/\*/g, '');
    var aiBox = document.getElementById('live-warning-inject');
    if (aiBox) {
      aiBox.innerHTML += `<div style='margin-top:12px; padding:12px; background:rgba(168,85,247,0.1); border:1px solid rgba(168,85,247,0.3); border-radius:8px; color:#d8b4fe; font-size:13px; line-height:1.6;'>
        <div style='display:flex; align-items:center; gap:6px; font-weight:700; margin-bottom:6px;'>
          <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="#a855f7" stroke-width="2"><path d="M12 2v4m0 12v4M4.93 4.93l2.83 2.83m8.48 8.48l2.83 2.83M2 12h4m12 0h4M4.93 19.07l2.83-2.83m8.48-8.48l2.83-2.83"></path></svg>
          Rekomendasi Gemini AI
        </div>
        ${aiText}
      </div>`;
    }
  } catch (err) {
    alert("Gagal menghubungi Gemini AI: " + err.message);
  } finally {
    if (btn) {
      btn.innerHTML = '✨ Konsultasi AI Gemini';
      btn.disabled = false;
    }
  }
}
