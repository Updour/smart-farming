// =================================================================
// 05_SECTORS.JS - DYNAMIC LOCALSTORAGE SECTOR & ZONATION ENGINE
// Target lines: ~150 (Max < 350)
// =================================================================

var SAMPLE_PRESET_SECTORS = [
  { id: 'sec_a', code: 'SEKTOR A', name: 'Cabai Rawit Merah', stage: 'semai', age: 14, area: 100, env: 'open' },
  { id: 'sec_b', code: 'SEKTOR B', name: 'Greenhouse Tomat Servo', stage: 'vegetatif', age: 35, area: 200, env: 'greenhouse' },
  { id: 'sec_c', code: 'SEKTOR C', name: 'Bawang Merah Bima', stage: 'generatif', age: 45, area: 300, env: 'open' },
  { id: 'sec_d', code: 'SEKTOR D', name: 'Padi Sawah Ciherang', stage: 'vegetatif', age: 60, area: 1000, env: 'open' }
];

function getStoredSectors() {
  try {
    var raw = localStorage.getItem('smartfarm_sectors');
    if (raw) {
      var parsed = JSON.parse(raw);
      if (Array.isArray(parsed)) return parsed;
    }
  } catch (e) {
    console.warn("Gagal membaca smartfarm_sectors dari localStorage", e);
  }
  return []; // STRICT ZERO DUMMY DEFAULT
}

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
          'Sistem zonasi lahan masih bersih. Daftarkan petak kebun Anda untuk mulai mengalkulasi kebutuhan air, evapotranspirasi, dan analisis fenologi tanaman.' +
        '</div>' +
        '<div style="display:flex; gap:10px; flex-wrap:wrap; justify-content:center;">' +
          '<button type="button" class="btn btn-primary" onclick="openAddSectorForm()" style="padding:8px 18px; font-size:12px;">' +
            '+ Tambah Sektor Baru' +
          '</button>' +
          '<button type="button" class="btn btn-secondary" onclick="loadSamplePresetSectors()" style="padding:8px 16px; font-size:12px;">' +
            'Muat Contoh Template (Opsional)' +
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

    var envBadge = sec.env === 'greenhouse' ? ' • GH' : '';
    var areaFormatted = Number(sec.area).toLocaleString('id-ID');

    btn.innerHTML =
      '<div class="sector-card-top">' +
        '<span class="sector-code">' + escapeHtml(sec.code || 'SEKTOR') + '</span>' +
        '<span class="sector-pulse"></span>' +
      '</div>' +
      '<div class="sector-name">' + escapeHtml(sec.name || 'Tanaman') + '</div>' +
      '<div class="sector-specs">' + areaFormatted + ' m² • ' + formatStageLabel(sec.stage) + envBadge + '</div>';

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

  sectors.forEach(function(s) {
    var el = document.getElementById('btn-sector-' + s.id);
    if (el) {
      if (s.id === secId) {
        el.className = 'sector-card active btn-primary';
      } else {
        el.className = 'sector-card btn-secondary';
      }
    }
  });

  if (typeof applyCropPreset === 'function') {
    applyCropPreset(sec.name, sec.stage, sec.age, (sec.env || 'open'), sec.area);
  }

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
