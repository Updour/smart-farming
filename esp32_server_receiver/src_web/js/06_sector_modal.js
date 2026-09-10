/**
 * 06_sector_modal.js - Smart Farm Sector Management Modal & Form Handlers
 * Clean Architecture, Modal Dialogs & LocalStorage Persistence
 * Strict line limit < 400 lines
 */

function openSectorManagerModal() {
  var modal = document.getElementById('modal-sector-manager');
  if (modal) {
    modal.style.display = 'flex';
    cancelSectorForm();
    renderSectorManagerList();
  }
}

function closeSectorManagerModal() {
  var modal = document.getElementById('modal-sector-manager');
  if (modal) modal.style.display = 'none';
}

function renderSectorManagerList() {
  var list = document.getElementById('sector-manager-list');
  if (!list) return;

  var sectors = getStoredSectors();
  var activeId = getActiveSectorId();

  list.innerHTML = '';
  if (sectors.length === 0) {
    list.innerHTML =
      '<div style="text-align:center; padding:24px 12px; color:var(--text-sub); font-size:12px; border:1px dashed rgba(255,255,255,0.1); border-radius:10px;">' +
        'Belum ada sektor kebun tersimpan di LocalStorage.<br>' +
        '<button type="button" class="btn btn-primary" onclick="openAddSectorForm()" style="margin-top:10px; padding:6px 14px; font-size:11px;">+ Tambah Sektor Baru</button>' +
      '</div>';
    return;
  }

  sectors.forEach(function(sec) {
    var isActive = (sec.id === activeId);
    var item = document.createElement('div');
    item.className = 'sector-manage-item ' + (isActive ? 'is-active' : '');

    var envText = sec.env === 'greenhouse' ? 'Greenhouse' : 'Lahan Terbuka';
    var areaFormatted = Number(sec.area).toLocaleString('id-ID');

    item.innerHTML =
      '<div style="display:flex; flex-direction:column; gap:2px; flex:1; min-width:0;">' +
        '<div style="display:flex; align-items:center; gap:8px;">' +
          '<span class="sector-code" style="font-size:10px;">' + escapeHtml(sec.code) + '</span>' +
          '<strong style="font-size:13px; color:var(--text-main); white-space:nowrap; overflow:hidden; text-overflow:ellipsis;">' + escapeHtml(sec.name) + '</strong>' +
          (isActive ? '<span style="font-size:10px; color:#10b981; font-weight:700;">● Aktif</span>' : '') +
        '</div>' +
        '<span style="font-size:11px; color:var(--text-sub);">' +
          areaFormatted + ' m² • ' + formatStageLabel(sec.stage) + ' (' + sec.age + ' HST) • ' + envText +
        '</span>' +
      '</div>' +
      '<div style="display:flex; gap:6px; align-items:center; flex-shrink:0;">' +
        (!isActive ? '<button type="button" class="btn btn-secondary" onclick="selectSectorFromModal(\'' + sec.id + '\')" style="padding:4px 8px; font-size:11px;">Pilih</button>' : '') +
        '<button type="button" class="btn btn-secondary" onclick="openEditSectorForm(\'' + sec.id + '\')" style="padding:4px 8px; font-size:11px;">Edit</button>' +
        '<button type="button" class="btn btn-secondary" onclick="deleteSector(\'' + sec.id + '\')" style="padding:4px 8px; font-size:11px; color:#ef4444;" title="Hapus Sektor">Hapus</button>' +
      '</div>';

    list.appendChild(item);
  });
}

function selectSectorFromModal(id) {
  selectSector(id);
  renderSectorManagerList();
}

function openAddSectorForm() {
  openSectorManagerModal();
  var wrapper = document.getElementById('sector-form-wrapper');
  var title = document.getElementById('sector-form-title');
  var sectors = getStoredSectors();
  var nextLetter = String.fromCharCode(65 + (sectors ? sectors.length : 0));

  if (wrapper) wrapper.style.display = 'block';
  if (title) title.innerText = '+ Tambah Sektor Baru';

  document.getElementById('edit-sector-id').value = '';
  document.getElementById('sec-input-code').value = 'SEKTOR ' + nextLetter;
  document.getElementById('sec-input-name').value = '';
  document.getElementById('sec-input-stage').value = 'semai';
  document.getElementById('sec-input-age').value = '';
  document.getElementById('sec-input-area').value = 100;
  document.getElementById('sec-input-env').value = 'open';
}

function openEditSectorForm(id) {
  var sectors = getStoredSectors();
  var sec = sectors.find(function(s) { return s.id === id; });
  if (!sec) return;

  var wrapper = document.getElementById('sector-form-wrapper');
  var title = document.getElementById('sector-form-title');

  if (wrapper) wrapper.style.display = 'block';
  if (title) title.innerText = 'Edit ' + sec.code;

  document.getElementById('edit-sector-id').value = sec.id;
  document.getElementById('sec-input-code').value = sec.code || '';
  document.getElementById('sec-input-name').value = sec.name || '';
  document.getElementById('sec-input-stage').value = sec.stage || 'semai';
  document.getElementById('sec-input-age').value = sec.age || '';
  document.getElementById('sec-input-area').value = sec.area || 100;
  document.getElementById('sec-input-env').value = sec.env || 'open';
}

function cancelSectorForm() {
  var wrapper = document.getElementById('sector-form-wrapper');
  if (wrapper) wrapper.style.display = 'none';
}

function saveSectorFormData() {
  var id = document.getElementById('edit-sector-id').value;
  var code = (document.getElementById('sec-input-code').value || '').trim();
  var name = (document.getElementById('sec-input-name').value || '').trim();
  var stage = document.getElementById('sec-input-stage').value;
  var age = parseInt(document.getElementById('sec-input-age').value, 10) || 1;
  var area = parseInt(document.getElementById('sec-input-area').value, 10) || 100;
  var env = document.getElementById('sec-input-env').value;

  if (!code) {
    alert("Harap masukkan kode atau nama sektor!");
    return;
  }
  if (!name) {
    alert("Harap masukkan varietas atau nama komoditas tanaman!");
    return;
  }

  var sectors = getStoredSectors();

  if (id) {
    var idx = sectors.findIndex(function(s) { return s.id === id; });
    if (idx !== -1) {
      sectors[idx] = { id: id, code: code, name: name, stage: stage, age: age, area: area, env: env };
    }
  } else {
    var newId = 'sec_' + Date.now();
    sectors.push({ id: newId, code: code, name: name, stage: stage, age: age, area: area, env: env });
    id = newId;
  }

  saveStoredSectors(sectors);
  renderSectorGrid();
  renderSectorManagerList();
  cancelSectorForm();

  if (id === getActiveSectorId() || sectors.length === 1) {
    selectSector(id);
  }
}

function deleteSector(id) {
  var sectors = getStoredSectors();
  var target = sectors.find(function(s) { return s.id === id; });
  if (!target) return;

  if (!confirm("Hapus " + target.code + " (" + target.name + ") dari LocalStorage?")) return;

  sectors = sectors.filter(function(s) { return s.id !== id; });
  saveStoredSectors(sectors);

  if (sectors.length === 0) {
    localStorage.removeItem('smartfarm_active_sector_id');
    localStorage.removeItem('crop_name');
    renderSectorGrid();
    renderSectorManagerList();
    if (typeof clearCropFormUI === 'function') clearCropFormUI();
    if (typeof updatePhenologyAI === 'function') updatePhenologyAI(null);
  } else {
    if (getActiveSectorId() === id) {
      localStorage.setItem('smartfarm_active_sector_id', sectors[0].id);
      selectSector(sectors[0].id);
    } else {
      renderSectorGrid();
      renderSectorManagerList();
    }
  }
}

function clearAllSectors() {
  if (!confirm("Kosongkan semua data sektor kebun dari LocalStorage?\n\nTampilan zonasi lahan dan kalibrasi akan dikembalikan ke kondisi awal bersih.")) return;
  saveStoredSectors([]);
  localStorage.removeItem('smartfarm_active_sector_id');
  localStorage.removeItem('crop_name');
  localStorage.removeItem('crop_stage');
  localStorage.removeItem('crop_age');
  localStorage.removeItem('crop_area');
  renderSectorGrid();
  renderSectorManagerList();
  if (typeof clearCropFormUI === 'function') clearCropFormUI();
  if (typeof updatePhenologyAI === 'function') updatePhenologyAI(null);
}

function loadSamplePresetSectors() {
  if (!confirm("Muat 4 contoh template sektor pertanian presisi (Cabai, Tomat GH, Bawang, Padi)?")) return;
  saveStoredSectors(SAMPLE_PRESET_SECTORS);
  localStorage.setItem('smartfarm_active_sector_id', SAMPLE_PRESET_SECTORS[0].id);
  renderSectorGrid();
  renderSectorManagerList();
  selectSector(SAMPLE_PRESET_SECTORS[0].id);
}

function resetSectorsToDefault() {
  clearAllSectors();
}

function escapeHtml(str) {
  if (!str) return '';
  return String(str)
    .replace(/&/g, '&amp;')
    .replace(/</g, '&lt;')
    .replace(/>/g, '&gt;')
    .replace(/"/g, '&quot;');
}
