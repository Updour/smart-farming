/**
 * Smart Farm Precision Agriculture SCADA - Core Application Manifest
 * =================================================================
 * Architecture: Clean Zero-Framework Modular JavaScript (Max < 400 lines/file)
 * All modules reside in esp32_server_receiver/src_web/js/:
 *
 *   01_navigation.js          - Tab switching & canvas triggers (~35 lines)
 *   02_charts.js              - Oscilloscope & 24h hourly canvas graphs (~205 lines)
 *   03_telemetry.js           - Sensor validation, traffic light, KPI DOM updates (~240 lines)
 *   04_telemetry_actuators.js  - Relays, pump metrics, schedule & NVS sync (~235 lines)
 *   05_sectors.js             - LocalStorage sector CRUD & grid renderer (~170 lines)
 *   06_sector_modal.js        - Sector manager modal & form dialogs (~220 lines)
 *   07_phenology.js           - GDD, VPD, VWC & seedling growth analytics (~265 lines)
 *   08_controls.js            - Actuator commands, pump calibration & sliders (~210 lines)
 *   09_agronomy.js            - Crop presets, hardening & agronomy engine (~275 lines)
 *   10_fertigation.js         - NPK formula & financial ROI calculator (~85 lines)
 *   11_disease_risk.js        - Sensor-driven pest & fungal risk evaluator (~120 lines)
 *   12_weather.js             - BMKG satellite forecast & wind vector parser (~135 lines)
 *   13_logs_init.js           - LittleFS log table, backup & dashboard startup (~205 lines)
 * =================================================================
 */

(function loadSmartFarmModules() {
  var modules = [
    'src_web/js/01_navigation.js',
    'src_web/js/02_charts.js',
    'src_web/js/02b_crop_history_chart.js',
    'src_web/js/03_telemetry.js',
    'src_web/js/04_telemetry_actuators.js',
    'src_web/js/05_sectors.js',
    'src_web/js/06_sector_modal.js',
    'src_web/js/07_phenology.js',
    'src_web/js/08_controls.js',
    'src_web/js/09_agronomy.js',
    'src_web/js/10_fertigation.js',
    'src_web/js/11_disease_risk.js',
    'src_web/js/12_weather.js',
    'src_web/js/13_logs_init.js',
    'src_web/js/14_agri_chat.js'
  ];

  // If app.js is included in an environment without modular script tags, dynamically inject them
  if (typeof window !== 'undefined' && typeof document !== 'undefined') {
    var existingScripts = Array.prototype.slice.call(document.getElementsByTagName('script'));
    var hasModularScripts = existingScripts.some(function(s) {
      return s.src && s.src.indexOf('01_navigation.js') !== -1;
    });

    if (!hasModularScripts) {
      modules.forEach(function(src) {
        var el = document.createElement('script');
        el.src = src;
        el.async = false;
        document.head.appendChild(el);
      });
    }
  }
})();
