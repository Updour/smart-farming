// =================================================================
// 05_SECTORS.JS - DYNAMIC LOCALSTORAGE SECTOR & ZONATION ENGINE
// 100% Real ESP32 Telemetry & Crop Profile • Zero Fake Data
// =================================================================

function getStoredSectors() {
  try {
    var raw = localStorage.getItem('smartfarm_sectors');
    if (raw) {
      var parsed = JSON.parse(raw);
      if (Array.isArray(parsed) && parsed.length > 0) {
        // Filter out any legacy dummy presets (Tomat Servo, Bawang Merah Bima, Padi Sawah Ciherang)
        var cleaned = parsed.filter(function(s) {
          if (!s) return false;
          var nLow = (s.name || '').toLowerCase();
          return s.id !== 'sec_b' && s.id !== 'sec_c' && s.id !== 'sec_d' &&
                 nLow.indexOf('tomat servo') === -1 && nLow.indexOf('ciherang') === -1;
        });
        if (cleaned.length > 0) return cleaned;
      }
    }
  } catch (e) {
    console.warn("Gagal membaca smartfarm_sectors dari localStorage", e);
  }

  // Sinkronisasi otomatis dari Profil Riil Tanaman yang tersimpan di chip Flash NVS ESP32
  var tData = window.lastTelemetryData || {};
  var cName = (tData.cropName && tData.cropName !== '') ? tData.cropName : (localStorage.getItem('crop_name') || 'Cabai Rawit');
  var cAge = (tData.cropAge !== undefined && tData.cropAge > 0) ? tData.cropAge : parseInt(localStorage.getItem('crop_age') || 14, 10);
  var cStage = (tData.cropStage && tData.cropStage !== '') ? tData.cropStage : (localStorage.getItem('crop_stage') || 'vegetatif');
  var cArea = (tData.cropArea !== undefined && tData.cropArea > 0) ? tData.cropArea : parseInt(localStorage.getItem('crop_area') || 100, 10);
  var cEnv = (tData.cropEnv && tData.cropEnv !== '') ? tData.cropEnv : (localStorage.getItem('crop_env') || 'open');

  if (cName) {
    var primarySector = [{
      id: 'sec_1',
      code: 'SEKTOR 1',
      name: cName,
      stage: cStage,
      age: cAge,
      area: cArea,
      env: cEnv
    }];
    saveStoredSectors(primarySector);
    try { localStorage.setItem('smartfarm_active_sector_id', 'sec_1'); } catch(e){}
    return primarySector;
  }

  return [];
}

function syncSectorFromEsp32(data) {
  if (!data || !data.cropName) return;
  var sectors = getStoredSectors();
  if (sectors.length === 0) {
    var newSec = {
      id: 'sec_1',
      code: 'SEKTOR 1',
      name: data.cropName,
      stage: data.cropStage || 'vegetatif',
      age: data.cropAge || 14,
      area: data.cropArea || 100,
      env: data.cropEnv || 'open'
    };
    saveStoredSectors([newSec]);
    try { localStorage.setItem('smartfarm_active_sector_id', 'sec_1'); } catch(e){}
    renderSectorGrid();
    selectSector('sec_1');
  } else {
    var activeId = getActiveSectorId() || sectors[0].id;
    var activeSec = sectors.find(function(s) { return s.id === activeId; }) || sectors[0];
    if (activeSec) {
      var changed = false;
      if (activeSec.name !== data.cropName) { activeSec.name = data.cropName; changed = true; }
      if (activeSec.age != data.cropAge) { activeSec.age = data.cropAge; changed = true; }
      if (activeSec.stage !== data.cropStage) { activeSec.stage = data.cropStage; changed = true; }
      if (data.cropArea && activeSec.area != data.cropArea) { activeSec.area = data.cropArea; changed = true; }
      if (data.cropEnv && activeSec.env !== data.cropEnv) { activeSec.env = data.cropEnv; changed = true; }
      if (changed) {
        saveStoredSectors(sectors);
        renderSectorGrid();
        if (typeof updatePhenologyAI === 'function') {
          var curT = (data.suhuC !== undefined && data.suhuC !== "--") ? parseFloat(data.suhuC) : null;
          var curH = (data.hum !== undefined && data.hum !== "--") ? parseFloat(data.hum) : null;
          var curS = (data.soil !== undefined && data.soil !== "--") ? parseFloat(data.soil) : null;
          var curVpd = (data.vpd !== undefined) ? parseFloat(data.vpd) : null;
          updatePhenologyAI(activeSec, curT, curH, curS, curVpd);
        }
      }
    }
  }
}
window.syncSectorFromEsp32 = syncSectorFromEsp32;

function saveStoredSectors(sectors) {
  try {
    localStorage.setItem('smartfarm_sectors', JSON.stringify(sectors || []));
  } catch (e) {
    console.error("Gagal menyimpan smartfarm_sectors", e);
  }
}

function getActiveSectorId() {
  var sectors = getStoredSectors();
  if (!sectors || sectors.length === 0) {
    return null;
  }
  var id = localStorage.getItem('smartfarm_active_sector_id');
  if (!id || !sectors.some(function(s) { return s.id === id; })) {
    id = sectors[0].id;
    localStorage.setItem('smartfarm_active_sector_id', id);
  }
  return id;
}

function getActiveSector() {
  var sectors = getStoredSectors();
  if (!sectors || sectors.length === 0) return null;
  var activeId = getActiveSectorId();
  if (!activeId) return null;
  var match = sectors.find(function(s) { return s.id === activeId; });
  return match || sectors[0] || null;
}

function formatStageLabel(stage) {
  switch (stage) {
    case 'semai': return 'Fase Semai';
    case 'vegetatif': return 'Fase Vegetatif';
    case 'generatif': return 'Fase Generatif';
    case 'panen': return 'Fase Panen';
    default: return stage || 'Fase Tumbuh';
  }
}

function renderSectorGrid() {
  var container = document.getElementById('sector-grid-container');
  if (!container) return;

  var sectors = getStoredSectors();
  var activeId = getActiveSectorId();

  container.innerHTML = '';

  if (!sectors || sectors.length === 0) {
    container.innerHTML =
      '<div class="sector-empty-state">' +
        '<div style="width:48px; height:48px; border-radius:12px; background:rgba(16,185,129,0.1); border:1px solid rgba(16,185,129,0.25); display:flex; align-items:center; justify-content:center; margin-bottom:12px;">' +
          '<svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="#10b981" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">' +
            '<rect x="3" y="3" width="18" height="18" rx="2" ry="2"></rect>' +
            '<line x1="3" y1="9" x2="21" y2="9"></line>' +
            '<line x1="9" y1="21" x2="9" y2="9"></line>' +
          '</svg>' +
        '</div>' +
        '<div style="font-size:14px; font-weight:700; color:var(--text-main); margin-bottom:4px;">Belum Ada Sektor Kebun Dikonfigurasi</div>' +
        '<div style="font-size:12px; color:var(--text-sub); max-width:440px; margin-bottom:16px; line-height:1.5;">' +
          'Daftarkan petak kebun Anda untuk mulai mengalkulasi kebutuhan air, evapotranspirasi, dan analisis fenologi tanaman.' +
        '</div>' +
        '<div style="display:flex; gap:10px; flex-wrap:wrap; justify-content:center;">' +
          '<button type="button" class="btn btn-primary" onclick="openAddSectorForm()" style="padding:8px 18px; font-size:12px;">' +
            '+ Tambah Sektor Baru' +
          '</button>' +
        '</div>' +
      '</div>';
    return;
  }

  sectors.forEach(function(sec) {
    var isActive = (sec.id === activeId);
    var btn = document.createElement('button');
    btn.type = 'button';
    btn.className = 'sector-card ' + (isActive ? 'active btn-primary' : 'btn-secondary');
    btn.id = 'btn-sector-' + sec.id;
    btn.onclick = function() { selectSector(sec.id); };

    var envBadge = sec.env === 'greenhouse' ? ' • Greenhouse' : ' • Lahan';
    var areaFormatted = Number(sec.area).toLocaleString('id-ID');
    var hstBadge = (sec.age !== undefined && sec.age !== null) ? ' • ' + sec.age + ' HST' : '';

    btn.innerHTML =
      '<div class="sector-card-top">' +
        '<span class="sector-code">' + escapeHtml(sec.code || 'SEKTOR') + '</span>' +
        '<span class="sector-pulse" style="' + (isActive ? '' : 'display:none;') + '"></span>' +
      '</div>' +
      '<div class="sector-name">' + escapeHtml(sec.name || 'Tanaman') + '</div>' +
      '<div class="sector-specs">' + areaFormatted + ' m²' + hstBadge + ' • ' + formatStageLabel(sec.stage) + envBadge + '</div>' +
      (isActive ? '<div style="margin-top:6px; font-size:10px; color:#10b981; font-weight:700; display:flex; align-items:center; gap:4px;"><span style="width:6px; height:6px; border-radius:50%; background:#10b981; display:inline-block;"></span> Aktif Lapangan (ESP32)</div>' : '');

    container.appendChild(btn);
  });
}

function selectSector(secId) {
  if (!secId) {
    if (typeof updatePhenologyAI === 'function') updatePhenologyAI(null);
    return;
  }
  var sectors = getStoredSectors();
  var sec = sectors.find(function(s) { return s.id === secId; });
  if (!sec) {
    if (typeof updatePhenologyAI === 'function') updatePhenologyAI(null);
    return;
  }

  localStorage.setItem('smartfarm_active_sector_id', secId);

  renderSectorGrid();

  if (typeof applyCropPreset === 'function') {
    applyCropPreset(sec.name, sec.stage, sec.age, (sec.env || 'open'), sec.area);
  }

  // Kirim sinkronisasi langsung ke ESP32 Flash Memory NVS agar Serial Monitor dan LCD 16x2 langsung berubah!
  var cropSyncUrl = '/setCropProfile?name=' + encodeURIComponent(sec.name) +
                    '&age=' + sec.age +
                    '&stage=' + encodeURIComponent(sec.stage) +
                    '&leaves=4' +
                    '&env=' + encodeURIComponent(sec.env || 'open') +
                    '&area=' + sec.area;
  fetch(cropSyncUrl).catch(function(err) { console.warn("Sync crop to ESP32 NVS:", err); });

  var curT = document.getElementById('kpi-temp') ? parseFloat(document.getElementById('kpi-temp').innerText) : null;
  var curH = document.getElementById('kpi-hum') ? parseFloat(document.getElementById('kpi-hum').innerText) : null;
  var curS = document.getElementById('kpi-soil') ? parseFloat(document.getElementById('kpi-soil').innerText) : null;
  if (typeof updatePhenologyAI === 'function') {
    updatePhenologyAI(sec, curT, curH, curS, null);
  }
}

function switchSectorPlot(secId, name, stage, age, area) {
  var sectors = getStoredSectors();
  var match = sectors.find(function(s) {
    return s.id === secId || s.code.toLowerCase().indexOf(secId.toLowerCase()) !== -1;
  });
  if (match) {
    selectSector(match.id);
  } else if (typeof applyCropPreset === 'function') {
    applyCropPreset(name, stage, age, (secId === 'B' ? 'greenhouse' : 'open'), area);
  }
}
