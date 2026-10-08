// =================================================================
// 01_NAVIGATION.JS - TAB NAVIGATION CONTROLLER
// Target lines: < 70
// =================================================================

function switchTab(tabId) {
  var buttons = document.querySelectorAll('.tab-btn');
  buttons.forEach(function (btn) {
    btn.classList.remove('active');
  });

  var activeBtn = document.querySelector('.tab-btn[data-tab="' + tabId + '"]');
  if (activeBtn) {
    activeBtn.classList.add('active');
  }

  var contents = document.querySelectorAll('.tab-content');
  contents.forEach(function (c) {
    c.classList.remove('active');
  });

  var targetContent = document.getElementById('tab-' + tabId);
  if (targetContent) {
    targetContent.classList.add('active');
  }

  // Auto trigger canvas redraws when switching to charts tab
  if (tabId === 'charts') {
    if (typeof fetchAndParseLogs === 'function') fetchAndParseLogs();
    setTimeout(function () {
      if (typeof resizeCanvas === 'function') resizeCanvas();
      if (typeof renderHourlyChart === 'function') renderHourlyChart();
      if (typeof resizeCropCanvas === 'function') resizeCropCanvas();
      if (typeof renderCropHistoryTable === 'function') renderCropHistoryTable();
    }, 60);
    setTimeout(function () {
      if (typeof resizeCanvas === 'function') resizeCanvas();
      if (typeof resizeCropCanvas === 'function') resizeCropCanvas();
    }, 250);
  }

  // Auto trigger log fetching when switching to logs tab
  if (tabId === 'logs') {
    if (typeof fetchAndParseLogs === 'function') fetchAndParseLogs();
  }
}

// Removed mobile nav fix - fixed cleanly in HTML

// =================================================================
// VIRTUAL LCD 16x2 HARDWARE MONITOR CONTROLLER (DIGITAL TWIN)
// =================================================================
var virtualLcdPage = 0;
var virtualLcdAutoRotate = true;
var virtualLcdThemes = ['lcd-theme-blue', 'lcd-theme-green', 'lcd-theme-amber', 'lcd-theme-dark'];
var virtualLcdThemeIdx = 0;
var lastVirtualLcdRotate = Date.now();

function toggleVirtualLcdDrawer() {
  var drawer = document.getElementById('virtual-lcd-drawer');
  var btn = document.getElementById('btn-toggle-virtual-lcd');
  if (!drawer) return;

  var isHidden = (drawer.style.display === 'none' || drawer.style.display === '');
  drawer.style.display = isHidden ? 'flex' : 'none';
  if (btn) {
    if (isHidden) btn.classList.add('active');
    else btn.classList.remove('active');
  }
  try {
    localStorage.setItem('smartfarm_virtual_lcd_open', isHidden ? '1' : '0');
  } catch (e) {}

  if (isHidden) renderVirtualLcdRows();
}

function setVirtualLcdPage(page) {
  virtualLcdPage = page;
  var b0 = document.getElementById('btn-lcd-page-0');
  var b1 = document.getElementById('btn-lcd-page-1');
  if (b0) b0.className = (page === 0 ? 'lcd-btn active' : 'lcd-btn');
  if (b1) b1.className = (page === 1 ? 'lcd-btn active' : 'lcd-btn');
  renderVirtualLcdRows();
}

function toggleVirtualLcdRotate() {
  virtualLcdAutoRotate = !virtualLcdAutoRotate;
  var btn = document.getElementById('btn-lcd-auto-rotate');
  if (btn) {
    btn.innerText = virtualLcdAutoRotate ? 'Auto: ON' : 'Auto: OFF';
    btn.style.color = virtualLcdAutoRotate ? '#38bdf8' : '#94a3b8';
  }
}

function cycleLcdBacklight() {
  virtualLcdThemeIdx = (virtualLcdThemeIdx + 1) % virtualLcdThemes.length;
  var bezel = document.getElementById('lcd-screen-bezel');
  if (bezel) {
    virtualLcdThemes.forEach(function (t) { bezel.classList.remove(t); });
    bezel.classList.add(virtualLcdThemes[virtualLcdThemeIdx]);
  }
  try {
    localStorage.setItem('smartfarm_virtual_lcd_theme', virtualLcdThemeIdx);
  } catch (e) {}
}

function renderVirtualLcdRows() {
  var r1 = document.getElementById('virtual-lcd-row-1');
  var r2 = document.getElementById('virtual-lcd-row-2');
  if (!r1 || !r2) return;

  var d = window.lastTelemetryData;
  if (!d) {
    r1.innerText = 'MENUNGGU ESP32..';
    r2.innerText = 'SINKRONISASI....';
    return;
  }

  var isManual = (d.manual == 1);
  var modeStr = isManual ? ' MAN' : 'AUTO';

  var isRelayOn = (d.relayOn == 1);
  var isLampOn = (d.lampOn == 1);

  // Check emergency states from real hardware
  if (d.unplugged || (d.soil === -1 && d.isWaiting != 1)) {
    r1.innerText = '!  PERINGATAN  !';
    r2.innerText = 'SENSOR TERPUTUS ';
    return;
  }

  // Check initial startup waiting state
  if (d.isWaiting == 1) {
    var pct = (d.waitingPercent !== undefined) ? d.waitingPercent : 0;
    var wSec = (d.waitingSecs !== undefined) ? d.waitingSecs : 0;
    r1.innerText = '! MEMUAT SISTEM !';
    r2.innerText = ('Tunggu:' + (pct < 10 ? ' ' : '') + pct + '% ' + wSec + 's/60s   ').substring(0, 16);
    return;
  }

  var rawT = (d.suhuC !== undefined && d.suhuC !== null && d.suhuC !== '--') ? d.suhuC : (d.temp || null);
  var tVal = (rawT !== null && !isNaN(parseFloat(rawT))) ? parseFloat(rawT) : null;

  if (tVal !== null && tVal >= 35.0) {
    r1.innerText = '! BAHAYA  SUHU !';
    r2.innerText = 'Suhu:' + tVal.toFixed(1) + 'C PANAS';
    return;
  }

  if (isRelayOn || virtualLcdPage === 0) {
    // Layar 1: T:28.4C H:74%  AUTO
    //         S: 0%   P:ON   L:ON 
    var rawH = (d.hum !== undefined && d.hum !== null && d.hum !== '--') ? d.hum : null;
    var hVal = (rawH !== null && !isNaN(parseFloat(rawH))) ? Math.round(parseFloat(rawH)) : null;
    var rawS = (d.soil !== undefined && d.soil !== null && d.soil !== '--') ? d.soil : null;
    var sVal = (rawS !== null && !isNaN(parseFloat(rawS)) && parseFloat(rawS) >= 0) ? Math.round(parseFloat(rawS)) : null;

    var tempFormatted = (tVal !== null) ? ((tVal < 10 ? ' ' : '') + tVal.toFixed(1) + 'C') : '--.-C';
    var humFormatted = (hVal !== null) ? ((hVal < 10 ? ' ' : '') + hVal + '%') : '--%';
    var soilFormatted = (sVal !== null) ? ((sVal < 10 ? '  ' : (sVal < 100 ? ' ' : '')) + sVal + '%') : ' --%';

    var pumpStr = isRelayOn ? 'ON ' : (d.cooldown ? 'CLD' : 'OFF');
    var lampStr = isLampOn ? 'ON ' : 'OFF';

    var bSuhu = (d.batasSuhu !== undefined && !isNaN(parseFloat(d.batasSuhu))) ? parseFloat(d.batasSuhu) : 30.0;
    if (tVal !== null && tVal > bSuhu) {
      r1.innerText = ('T:' + tempFormatted + ' [PANAS] ' + modeStr).substring(0, 16);
    } else {
      r1.innerText = ('T:' + tempFormatted + ' H:' + humFormatted + ' ' + modeStr).substring(0, 16);
    }
    r2.innerText = 'S:' + soilFormatted + ' P:' + pumpStr + ' L:' + lampStr;
  } else {
    // Layar 2: 16:52:30  AUTO
    //         -62dB 192.168.4.1
    var timeStr = d.rtcTime || d.time || '--:--:--';
    if (timeStr.indexOf(' ') !== -1) {
      timeStr = timeStr.split(' ')[1] || timeStr;
    }
    if (timeStr.length > 8) timeStr = timeStr.slice(0, 8);
    while (timeStr.length < 8) timeStr += ' ';

    var rawAdcVal = (d.rawAdc !== undefined && d.rawAdc !== '--' && d.rawAdc !== '-') ? d.rawAdc : '---';
    var adcStr = 'A0:' + rawAdcVal;
    while (adcStr.length < 7) adcStr += ' ';

    var rssiStr = (d.rssi !== undefined && d.rssi !== 0 && d.rssi !== '--') ? (d.rssi + 'dB') : ' --dB';
    while (rssiStr.length < 5) rssiStr = ' ' + rssiStr;

    r1.innerText = timeStr + '  ' + modeStr;
    r2.innerText = adcStr + ' ' + rssiStr + ' WEB';
  }
}


// Auto-timer for Virtual LCD 16x2
setInterval(function () {
  var drawer = document.getElementById('virtual-lcd-drawer');
  if (!drawer || drawer.style.display === 'none') return;

  var now = Date.now();
  if (virtualLcdAutoRotate && (now - lastVirtualLcdRotate >= 4000)) {
    virtualLcdPage = (virtualLcdPage + 1) % 2;
    lastVirtualLcdRotate = now;
    var b0 = document.getElementById('btn-lcd-page-0');
    var b1 = document.getElementById('btn-lcd-page-1');
    if (b0) b0.className = (virtualLcdPage === 0 ? 'lcd-btn active' : 'lcd-btn');
    if (b1) b1.className = (virtualLcdPage === 1 ? 'lcd-btn active' : 'lcd-btn');
  }

  renderVirtualLcdRows();
}, 500);

// Init Virtual LCD on page load
document.addEventListener('DOMContentLoaded', function () {
  try {
    var savedOpen = localStorage.getItem('smartfarm_virtual_lcd_open');
    if (savedOpen === '1') {
      var drawer = document.getElementById('virtual-lcd-drawer');
      var btn = document.getElementById('btn-toggle-virtual-lcd');
      if (drawer) drawer.style.display = 'flex';
      if (btn) btn.classList.add('active');
    }

    var savedTheme = localStorage.getItem('smartfarm_virtual_lcd_theme');
    if (savedTheme !== null) {
      virtualLcdThemeIdx = parseInt(savedTheme, 10) % virtualLcdThemes.length;
      var bezel = document.getElementById('lcd-screen-bezel');
      if (bezel) {
        virtualLcdThemes.forEach(function (t) { bezel.classList.remove(t); });
        bezel.classList.add(virtualLcdThemes[virtualLcdThemeIdx]);
      }
    }
  } catch (e) {}
});

