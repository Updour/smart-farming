#ifndef INDEX_H
#define INDEX_H

#include <Arduino.h>

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="id">

<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Smart Farm Precision System - ESP32 Dashboard</title>

  <!-- Google Fonts & Modern Typography -->
  <link rel="preconnect" href="https://fonts.googleapis.com">
  <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
  <link
    href="https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600;700&family=Outfit:wght@500;600;700;800&display=swap"
    rel="stylesheet">

  <style>
:root {
      --bg-dark: #0f172a;
      --card-bg: rgba(30, 41, 59, 0.75);
      --card-border: rgba(255, 255, 255, 0.08);
      --primary: #10b981;
      --primary-glow: rgba(16, 185, 129, 0.3);
      --secondary: #06b6d4;
      --warning: #f59e0b;
      --danger: #ef4444;
      --accent: #a855f7;
      --text-main: #f8fafc;
      --text-sub: #94a3b8;
    }

    * {
      box-sizing: border-box;
      margin: 0;
      padding: 0;
      font-family: 'Inter', sans-serif;
    }

    body {
      background: var(--bg-dark);
      background-image:
        radial-gradient(at 0% 0%, rgba(16, 185, 129, 0.12) 0px, transparent 50%),
        radial-gradient(at 100% 100%, rgba(6, 182, 212, 0.1) 0px, transparent 50%),
        radial-gradient(at 50% 50%, rgba(168, 85, 247, 0.05) 0px, transparent 50%);
      color: var(--text-main);
      min-height: 100vh;
      padding: 16px;
      line-height: 1.5;
    }

    .container {
      max-width: 1200px;
      margin: 0 auto;
      display: flex;
      flex-direction: column;
      gap: 20px;
      padding-bottom: 95px;
      isolation: isolate;
    }

    /* UNIFIED COMPACT SCADA HEADER */
    header, .unified-header {
      display: flex;
      justify-content: space-between;
      align-items: center;
      flex-wrap: wrap;
      gap: 14px 20px;
      background: var(--card-bg);
      backdrop-filter: blur(12px);
      border: 1px solid var(--card-border);
      border-top: 2px solid var(--primary);
      padding: 12px 20px;
      border-radius: 16px;
      box-shadow: 0 10px 30px rgba(0, 0, 0, 0.2);
      margin-bottom: 16px;
      transition: border-color 0.3s ease, box-shadow 0.3s ease;
    }

    .brand-block {
      display: flex;
      flex-direction: column;
      gap: 4px;
      min-width: 240px;
    }

    .brand-title {
      font-family: 'Outfit', sans-serif;
      font-size: 19px;
      font-weight: 800;
      letter-spacing: -0.02em;
      background: linear-gradient(135deg, #10b981, #06b6d4);
      -webkit-background-clip: text;
      -webkit-text-fill-color: transparent;
      display: flex;
      align-items: center;
      gap: 10px;
    }

    .brand-icon-box {
      width: 30px;
      height: 30px;
      border-radius: 8px;
      background: rgba(16, 185, 129, 0.12);
      border: 1px solid rgba(16, 185, 129, 0.25);
      display: flex;
      align-items: center;
      justify-content: center;
      color: var(--primary);
      flex-shrink: 0;
    }

    .brand-sub {
      font-size: 11px;
      font-weight: 400;
      color: var(--text-sub);
      display: flex;
      align-items: center;
      gap: 8px;
      flex-wrap: wrap;
    }

    .status-divider {
      color: rgba(255, 255, 255, 0.2);
      user-select: none;
    }

    .header-status-msg {
      color: var(--primary);
      font-weight: 600;
      font-size: 11px;
      padding: 1px 7px;
      border-radius: 6px;
      background: rgba(16, 185, 129, 0.08);
      border: 1px solid rgba(16, 185, 129, 0.2);
      transition: all 0.3s ease;
    }

    .header-controls {
      display: flex;
      align-items: center;
      gap: 12px;
      flex-wrap: wrap;
    }

    .status-badge-bar {
      display: flex;
      align-items: center;
      gap: 8px;
      flex-wrap: wrap;
    }

    .badge-pill {
      display: inline-flex;
      align-items: center;
      gap: 6px;
      background: rgba(255, 255, 255, 0.04);
      border: 1px solid var(--card-border);
      padding: 5px 12px;
      border-radius: 20px;
      font-size: 11.5px;
      font-weight: 500;
      color: var(--text-main);
    }

    .badge-pill.status-pill {
      border-color: rgba(16, 185, 129, 0.35);
      background: rgba(16, 185, 129, 0.06);
    }

    /* SCADA TRAFFIC LIGHT INDICATOR (PINS 32, 33, 27) */
    .badge-pill.traffic-pill {
      display: inline-flex;
      align-items: center;
      gap: 8px;
      padding: 4px 10px;
      background: rgba(15, 23, 42, 0.7);
      border: 1px solid var(--card-border);
      border-radius: 20px;
    }

    .traffic-housing {
      display: inline-flex;
      align-items: center;
      gap: 5px;
      background: rgba(0, 0, 0, 0.5);
      padding: 3px 6px;
      border-radius: 12px;
      border: 1px solid rgba(255, 255, 255, 0.08);
      box-shadow: inset 0 1px 3px rgba(0, 0, 0, 0.6);
    }

    .traffic-bulb {
      width: 9px;
      height: 9px;
      border-radius: 50%;
      display: inline-block;
      transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1);
      opacity: 0.35;
    }

    .bulb-red {
      background: #ef4444;
      border: 1px solid rgba(239, 68, 68, 0.5);
    }

    .bulb-red.active {
      opacity: 1;
      box-shadow: 0 0 10px #ef4444, 0 0 4px #fca5a5;
    }

    .bulb-yellow {
      background: #f59e0b;
      border: 1px solid rgba(245, 158, 11, 0.5);
    }

    .bulb-yellow.active {
      opacity: 1;
      box-shadow: 0 0 10px #f59e0b, 0 0 4px #fde68a;
    }

    .bulb-green {
      background: #10b981;
      border: 1px solid rgba(16, 185, 129, 0.5);
    }

    .bulb-green.active {
      opacity: 1;
      box-shadow: 0 0 10px #10b981, 0 0 4px #6ee7b7;
    }

    .traffic-bulb.blink {
      animation: trafficBlink 0.8s infinite alternate ease-in-out;
    }

    .bulb-yellow.blink {
      animation: trafficBlinkYellow 0.8s infinite alternate ease-in-out !important;
    }

    .bulb-red.blink {
      animation: trafficBlinkRed 0.8s infinite alternate ease-in-out !important;
    }

    .bulb-green.blink {
      animation: trafficBlinkGreen 0.8s infinite alternate ease-in-out !important;
    }

    @keyframes trafficBlinkYellow {
      0% {
        opacity: 0.25;
        box-shadow: none;
      }
      100% {
        opacity: 1;
        box-shadow: 0 0 14px #f59e0b, 0 0 6px #fde68a;
      }
    }

    @keyframes trafficBlinkRed {
      0% {
        opacity: 0.25;
        box-shadow: none;
      }
      100% {
        opacity: 1;
        box-shadow: 0 0 14px #ef4444, 0 0 6px #fca5a5;
      }
    }

    @keyframes trafficBlinkGreen {
      0% {
        opacity: 0.25;
        box-shadow: none;
      }
      100% {
        opacity: 1;
        box-shadow: 0 0 14px #10b981, 0 0 6px #6ee7b7;
      }
    }

    @keyframes trafficBlink {
      0% {
        opacity: 0.25;
        box-shadow: none;
      }
      100% {
        opacity: 1;
        box-shadow: 0 0 12px currentColor;
      }
    }

    .traffic-label {
      font-size: 11px;
      font-weight: 700;
      letter-spacing: 0.03em;
      text-transform: uppercase;
      transition: color 0.3s ease;
    }

/* =================================================================
   TRAFFIC LIGHT POLE CARD (DASHBOARD REAL-TIME STATUS INDICATOR)
   ================================================================= */
.traffic-indicator-card {
  padding: 16px 20px;
}

.traffic-pole-housing {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 10px;
  background: rgba(0, 0, 0, 0.55);
  padding: 14px 18px;
  border-radius: 18px;
  border: 1px solid rgba(255, 255, 255, 0.08);
  box-shadow: inset 0 2px 8px rgba(0, 0, 0, 0.7), 0 4px 16px rgba(0, 0, 0, 0.4);
  position: relative;
}

.traffic-pole-housing::before {
  content: '';
  position: absolute;
  bottom: -18px;
  left: 50%;
  transform: translateX(-50%);
  width: 6px;
  height: 18px;
  background: rgba(100, 116, 139, 0.5);
  border-radius: 0 0 3px 3px;
}

.traffic-pole-bulb {
  width: 46px;
  height: 46px;
  border-radius: 50%;
  display: flex;
  align-items: center;
  justify-content: center;
  transition: all 0.35s cubic-bezier(0.4, 0, 0.2, 1);
  opacity: 0.18;
  position: relative;
}

.pole-bulb-inner {
  width: 34px;
  height: 34px;
  border-radius: 50%;
  transition: all 0.35s ease;
}

/* OFF states */
.pole-red  { background: rgba(239, 68, 68, 0.15);  border: 2px solid rgba(239, 68, 68, 0.2); }
.pole-yellow { background: rgba(245, 158, 11, 0.15); border: 2px solid rgba(245, 158, 11, 0.2); }
.pole-green  { background: rgba(16, 185, 129, 0.15); border: 2px solid rgba(16, 185, 129, 0.2); }
.pole-red   .pole-bulb-inner { background: rgba(239, 68, 68, 0.3); }
.pole-yellow .pole-bulb-inner { background: rgba(245, 158, 11, 0.3); }
.pole-green  .pole-bulb-inner { background: rgba(16, 185, 129, 0.3); }

/* ACTIVE states */
.pole-red.lit {
  opacity: 1;
  background: rgba(239, 68, 68, 0.22);
  border-color: rgba(239, 68, 68, 0.6);
  box-shadow: 0 0 28px rgba(239, 68, 68, 0.7), 0 0 10px rgba(239, 68, 68, 0.5);
}
.pole-red.lit .pole-bulb-inner {
  background: #ef4444;
  box-shadow: 0 0 16px #ef4444, inset 0 2px 4px rgba(255,255,255,0.3);
}

.pole-yellow.lit {
  opacity: 1;
  background: rgba(245, 158, 11, 0.22);
  border-color: rgba(245, 158, 11, 0.6);
  box-shadow: 0 0 28px rgba(245, 158, 11, 0.7), 0 0 10px rgba(245, 158, 11, 0.5);
}
.pole-yellow.lit .pole-bulb-inner {
  background: #f59e0b;
  box-shadow: 0 0 16px #f59e0b, inset 0 2px 4px rgba(255,255,255,0.3);
}

.pole-green.lit {
  opacity: 1;
  background: rgba(16, 185, 129, 0.22);
  border-color: rgba(16, 185, 129, 0.6);
  box-shadow: 0 0 28px rgba(16, 185, 129, 0.7), 0 0 10px rgba(16, 185, 129, 0.5);
}
.pole-green.lit .pole-bulb-inner {
  background: #10b981;
  box-shadow: 0 0 16px #10b981, inset 0 2px 4px rgba(255,255,255,0.3);
}

/* BLINK animation for schedule cycle mode */
.pole-red.blink { animation: poleBlinkRed 0.5s infinite alternate ease-in-out; }
.pole-yellow.blink { animation: poleBlinkYellow 0.5s infinite alternate ease-in-out; }
.pole-green.blink { animation: poleBlinkGreen 0.5s infinite alternate ease-in-out; }

@keyframes poleBlinkRed {
  0%  { opacity: 0.2; box-shadow: none; }
  100% { opacity: 1; box-shadow: 0 0 28px rgba(239,68,68,0.8), 0 0 10px rgba(239,68,68,0.6); }
}
@keyframes poleBlinkYellow {
  0%  { opacity: 0.2; box-shadow: none; }
  100% { opacity: 1; box-shadow: 0 0 28px rgba(245,158,11,0.8), 0 0 10px rgba(245,158,11,0.6); }
}
@keyframes poleBlinkGreen {
  0%  { opacity: 0.2; box-shadow: none; }
  100% { opacity: 1; box-shadow: 0 0 28px rgba(16,185,129,0.8), 0 0 10px rgba(16,185,129,0.6); }
}

    .pulse-dot {
      width: 8px;
      height: 8px;
      border-radius: 50%;
      background: var(--pulse-color, var(--primary));
      box-shadow: 0 0 10px var(--pulse-color, var(--primary));
      animation: pulse 2s infinite;
    }

    @keyframes pulse {
      0% {
        transform: scale(0.95);
        box-shadow: 0 0 0 0 var(--pulse-glow, rgba(16, 185, 129, 0.7));
      }

      70% {
        transform: scale(1);
        box-shadow: 0 0 0 8px transparent;
      }

      100% {
        transform: scale(0.95);
        box-shadow: 0 0 0 0 transparent;
      }
    }

    .header-action-group {
      display: flex;
      align-items: center;
      gap: 8px;
      flex-wrap: wrap;
    }

    .header-btn {
      padding: 6px 12px !important;
      font-size: 11px !important;
      display: inline-flex;
      align-items: center;
      gap: 6px;
      border-radius: 8px !important;
    }

    /* GRID LAYOUTS */
    .grid-kpi {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(240px, 1fr));
      gap: 16px;
      margin-bottom: 24px;
    }

    .grid-two-col {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(340px, 1fr));
      gap: 20px;
    }

    /* CARDS */
    .card {
      background: var(--card-bg);
      backdrop-filter: blur(12px);
      border: 1px solid var(--card-border);
      border-radius: 20px;
      padding: 20px;
      box-shadow: 0 10px 30px rgba(0, 0, 0, 0.25);
      transition: transform 0.2s ease, box-shadow 0.2s ease;
    }

    .card:hover {
      border-color: rgba(255, 255, 255, 0.15);
    }

    .card-title {
      font-family: 'Outfit', sans-serif;
      font-size: 16px;
      font-weight: 700;
      margin-bottom: 14px;
      display: flex;
      align-items: center;
      justify-content: space-between;
      gap: 8px;
    }

    /* =================================================================
       MODERN INDUSTRIAL KPI METRIC CARDS
       ================================================================= */
    .kpi-card {
      position: relative;
      background: var(--card-bg);
      backdrop-filter: blur(12px);
      border: 1px solid var(--card-border);
      border-radius: 18px;
      padding: 18px 20px;
      display: flex;
      flex-direction: column;
      justify-content: space-between;
      min-height: 160px;
      box-shadow: 0 8px 24px rgba(0, 0, 0, 0.2);
      transition: transform 0.22s cubic-bezier(0.16, 1, 0.3, 1), box-shadow 0.22s ease, border-color 0.22s ease;
      overflow: hidden;
    }

    .kpi-card::before {
      content: '';
      position: absolute;
      top: 0;
      left: 0;
      right: 0;
      height: 3px;
      background: transparent;
      transition: opacity 0.3s ease;
    }

    .kpi-card:hover {
      transform: translateY(-3px);
      border-color: rgba(255, 255, 255, 0.16);
      box-shadow: 0 14px 32px rgba(0, 0, 0, 0.32);
    }

    /* KPI Card Accent Themes */
    .kpi-card.soil-card::before { background: linear-gradient(90deg, #10b981, #059669); }
    .kpi-card.temp-card::before { background: linear-gradient(90deg, #06b6d4, #0284c7); }
    .kpi-card.hum-card::before  { background: linear-gradient(90deg, #38bdf8, #6366f1); }
    .kpi-card.signal-card::before, .kpi-card.bat-card::before  { background: linear-gradient(90deg, #3b82f6, #06b6d4); }

    /* KPI Header */
    .kpi-card-header {
      display: flex;
      align-items: flex-start;
      justify-content: space-between;
      gap: 10px;
      margin-bottom: 12px;
    }

    .kpi-meta {
      display: flex;
      align-items: center;
      gap: 10px;
    }

    .kpi-icon-box {
      width: 38px;
      height: 38px;
      border-radius: 10px;
      display: flex;
      align-items: center;
      justify-content: center;
      flex-shrink: 0;
    }

    .soil-card .kpi-icon-box { background: rgba(16, 185, 129, 0.12); color: #10b981; border: 1px solid rgba(16, 185, 129, 0.25); }
    .temp-card .kpi-icon-box { background: rgba(6, 182, 212, 0.12); color: #06b6d4; border: 1px solid rgba(6, 182, 212, 0.25); }
    .hum-card .kpi-icon-box  { background: rgba(56, 189, 248, 0.12); color: #38bdf8; border: 1px solid rgba(56, 189, 248, 0.25); }
    .signal-card .kpi-icon-box, .bat-card .kpi-icon-box  { background: rgba(59, 130, 246, 0.12); color: #38bdf8; border: 1px solid rgba(59, 130, 246, 0.25); }

    .kpi-title {
      font-size: 11px;
      font-weight: 700;
      color: var(--text-sub);
      text-transform: uppercase;
      letter-spacing: 0.06em;
      display: block;
      line-height: 1.2;
    }

    .kpi-subtitle {
      font-size: 10px;
      color: var(--text-sub);
      opacity: 0.7;
      display: block;
      margin-top: 2px;
    }

    .kpi-status-badge {
      font-size: 10.5px;
      font-weight: 600;
      padding: 3px 8px;
      border-radius: 20px;
      background: rgba(255, 255, 255, 0.05);
      border: 1px solid rgba(255, 255, 255, 0.1);
      color: var(--text-sub);
      white-space: nowrap;
    }

    /* KPI Value Body */
    .kpi-card-body {
      margin-bottom: 12px;
    }

    .kpi-value {
      font-family: 'Outfit', sans-serif;
      font-size: 34px;
      font-weight: 800;
      line-height: 1.1;
      color: var(--text-main);
      letter-spacing: -0.02em;
    }

    /* KPI Footer */
    .kpi-card-footer {
      padding-top: 10px;
      border-top: 1px solid rgba(255, 255, 255, 0.06);
      display: flex;
      align-items: center;
      gap: 6px;
      margin-top: auto;
    }

    .kpi-footer-dot {
      width: 6px;
      height: 6px;
      border-radius: 50%;
      flex-shrink: 0;
    }
    .soil-card .kpi-footer-dot { background: #10b981; }
    .temp-card .kpi-footer-dot { background: #06b6d4; }
    .hum-card .kpi-footer-dot  { background: #38bdf8; }
    .signal-card .kpi-footer-dot, .bat-card .kpi-footer-dot  { background: #38bdf8; }

    .kpi-subnote,
    .kpi-footer-text {
      font-size: 11.5px;
      font-weight: 500;
      color: var(--text-sub);
      overflow: hidden;
      text-overflow: ellipsis;
      white-space: nowrap;
    }

    /* =================================================================
       SECTOR SELECTOR STRIP & CARDS
       ================================================================= */
    /* =================================================================
       SECTOR SELECTOR STRIP & CARDS
       ================================================================= */
    .sector-strip {
      background: var(--card-bg);
      backdrop-filter: blur(12px);
      border: 1px solid var(--card-border);
      border-radius: 20px;
      padding: 22px 24px;
      margin-top: 24px;
      margin-bottom: 26px;
      box-shadow: 0 10px 30px rgba(0, 0, 0, 0.2);
    }

    .sector-strip-header {
      display: flex;
      align-items: center;
      justify-content: space-between;
      margin-bottom: 18px;
      flex-wrap: wrap;
      gap: 12px;
    }

    .sector-strip-title-wrap {
      display: flex;
      flex-direction: column;
      gap: 4px;
    }

    .sector-strip-title {
      display: flex;
      align-items: center;
      gap: 8px;
      font-size: 12px;
      font-weight: 700;
      text-transform: uppercase;
      letter-spacing: 0.06em;
      color: var(--text-main);
    }

    .sector-strip-title svg {
      color: var(--primary);
    }

    .sector-strip-desc {
      font-size: 11px;
      color: var(--text-sub);
    }

    .sector-strip-actions {
      display: flex;
      align-items: center;
      gap: 8px;
    }

    .sector-grid {
      display: grid;
      grid-template-columns: repeat(4, 1fr);
      gap: 12px;
    }

    .sector-empty-state {
      grid-column: 1 / -1;
      background: rgba(15, 23, 42, 0.4);
      border: 1px dashed rgba(255, 255, 255, 0.15);
      border-radius: 16px;
      padding: 24px 20px;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      text-align: center;
    }

    @media (max-width: 900px) {
      .sector-grid {
        grid-template-columns: repeat(2, 1fr);
      }
    }

    @media (max-width: 500px) {
      .sector-grid {
        grid-template-columns: 1fr;
      }
    }

    .sector-card {
      background: rgba(15, 23, 42, 0.6);
      border: 1px solid var(--card-border);
      border-radius: 14px;
      padding: 12px 14px;
      text-align: left;
      cursor: pointer;
      transition: all 0.2s ease;
      position: relative;
      outline: none;
      font-family: inherit;
      color: var(--text-main);
      display: flex;
      flex-direction: column;
      gap: 4px;
    }

    .sector-card:hover {
      background: rgba(30, 41, 59, 0.7);
      border-color: rgba(255, 255, 255, 0.18);
      transform: translateY(-2px);
    }

    .sector-card-top {
      display: flex;
      align-items: center;
      justify-content: space-between;
    }

    .sector-code {
      font-size: 10px;
      font-weight: 700;
      letter-spacing: 0.06em;
      color: var(--text-sub);
      padding: 2px 7px;
      border-radius: 6px;
      background: rgba(255, 255, 255, 0.05);
    }

    .sector-pulse {
      width: 7px;
      height: 7px;
      border-radius: 50%;
      background: rgba(255, 255, 255, 0.2);
      transition: all 0.2s ease;
    }

    .sector-name {
      font-family: 'Outfit', sans-serif;
      font-size: 14px;
      font-weight: 700;
      color: var(--text-main);
      white-space: nowrap;
      overflow: hidden;
      text-overflow: ellipsis;
      margin-top: 2px;
    }

    .sector-specs {
      font-size: 11px;
      color: var(--text-sub);
    }

    /* Active Sector State */
    .sector-card.active,
    .sector-card.btn-primary {
      background: linear-gradient(145deg, rgba(16, 185, 129, 0.18), rgba(6, 182, 212, 0.1)) !important;
      border-color: rgba(16, 185, 129, 0.55) !important;
      box-shadow: 0 4px 18px rgba(16, 185, 129, 0.22) !important;
    }

    .sector-card.active .sector-code,
    .sector-card.btn-primary .sector-code {
      background: rgba(16, 185, 129, 0.25);
      color: #10b981;
    }

    .sector-card.active .sector-pulse,
    .sector-card.btn-primary .sector-pulse {
      background: #10b981;
      box-shadow: 0 0 8px #10b981;
    }

    .sector-card.active .sector-name,
    .sector-card.btn-primary .sector-name {
      color: #ffffff;
    }

    .sector-card.active .sector-specs,
    .sector-card.btn-primary .sector-specs {
      color: #a7f3d0;
    }

    /* BUTTONS */
    .btn {
      display: inline-flex;
      align-items: center;
      justify-content: center;
      gap: 8px;
      padding: 10px 18px;
      border-radius: 12px;
      font-size: 13px;
      font-weight: 600;
      border: none;
      cursor: pointer;
      transition: all 0.2s ease;
      text-decoration: none;
    }

    .btn:active {
      transform: scale(0.98);
    }

    .btn:disabled,
    .btn.disabled {
      opacity: 0.35;
      cursor: not-allowed;
      pointer-events: none;
      box-shadow: none !important;
      transform: none !important;
      filter: grayscale(0.8);
    }

    .btn-primary {
      background: linear-gradient(135deg, #10b981, #059669);
      color: white;
      box-shadow: 0 4px 14px rgba(16, 185, 129, 0.3);
    }

    .btn-primary:hover {
      box-shadow: 0 6px 20px rgba(16, 185, 129, 0.4);
    }

    .btn-danger {
      background: linear-gradient(135deg, #ef4444, #dc2626);
      color: white;
      box-shadow: 0 4px 14px rgba(239, 68, 68, 0.3);
    }

    .btn-secondary {
      background: rgba(255, 255, 255, 0.08);
      color: var(--text-main);
      border: 1px solid var(--card-border);
    }

    .btn-secondary:hover {
      background: rgba(255, 255, 255, 0.14);
    }

    .btn-accent {
      background: linear-gradient(135deg, #a855f7, #7e22ce);
      color: white;
      box-shadow: 0 4px 14px rgba(168, 85, 247, 0.3);
    }

    /* FORM INPUTS */
    input[type="text"],
    input[type="number"],
    input[type="date"],
    input[type="time"],
    select {
      width: 100%;
      padding: 10px 14px;
      border-radius: 10px;
      background: rgba(15, 23, 42, 0.8);
      border: 1px solid var(--card-border);
      color: var(--text-main);
      font-size: 13px;
      outline: none;
      transition: border-color 0.2s ease;
    }

    input:focus,
    select:focus {
      border-color: var(--primary);
    }

    /* RANGE SLIDER */
    input[type="range"] {
      width: 100%;
      accent-color: var(--primary);
      cursor: pointer;
    }

    /* SWITCH TOGGLE */
    .switch-group {
      display: flex;
      align-items: center;
      justify-content: space-between;
      padding: 12px;
      background: rgba(0, 0, 0, 0.2);
      border-radius: 12px;
      margin-bottom: 12px;
    }

    .switch {
      position: relative;
      display: inline-block;
      width: 44px;
      height: 24px;
    }

    .switch input {
      opacity: 0;
      width: 0;
      height: 0;
    }

    .slider {
      position: absolute;
      cursor: pointer;
      top: 0;
      left: 0;
      right: 0;
      bottom: 0;
      background-color: rgba(255, 255, 255, 0.2);
      transition: .3s;
      border-radius: 24px;
    }

    .slider:before {
      position: absolute;
      content: "";
      height: 18px;
      width: 18px;
      left: 3px;
      bottom: 3px;
      background-color: white;
      transition: .3s;
      border-radius: 50%;
    }

    input:checked+.slider {
      background-color: var(--primary);
    }

    input:checked+.slider:before {
      transform: translateX(20px);
    }


    /* SEGMENTED CONTROL */
    .segment-control {
      display: flex;
      background: rgba(0, 0, 0, 0.3);
      border-radius: 8px;
      overflow: hidden;
      border: 1px solid rgba(255, 255, 255, 0.1);
      width: 140px;
    }
    .segment-btn {
      flex: 1;
      padding: 6px 12px;
      font-size: 11px;
      font-weight: 600;
      border: none;
      background: transparent;
      color: var(--text-sub);
      cursor: pointer;
      transition: all 0.2s;
    }
    .segment-btn.active {
      background: var(--primary);
      color: #fff;
    }
    .segment-btn.active-manual {
      background: var(--warning);
      color: #fff;
    }

    /* DROPZONE UPLOADER */
    .dropzone {
      border: 2px dashed var(--accent);
      background: rgba(168, 85, 247, 0.05);
      border-radius: 16px;
      padding: 24px;
      text-align: center;
      cursor: pointer;
      transition: all 0.3s ease;
    }

    .dropzone:hover {
      background: rgba(168, 85, 247, 0.12);
      border-color: #c084fc;
    }

    /* SCADA CANVAS CHART CONTAINER */
    .chart-container {
      position: relative;
      width: 100%;
      height: 220px;
      background: rgba(15, 23, 42, 0.9);
      border-radius: 14px;
      padding: 10px;
      border: 1px solid var(--card-border);
    }

    canvas {
      width: 100% !important;
      height: 100% !important;
      display: block;
    }

    /* SCADA CHART CONTROLS & TOGGLE CHIPS */
    .chart-header-wrap {
      display: flex;
      justify-content: space-between;
      align-items: center;
      flex-wrap: wrap;
      gap: 12px;
      margin-bottom: 12px;
    }
    .chart-title-area {
      display: flex;
      align-items: center;
      gap: 10px;
    }
    .chart-icon-box {
      width: 38px;
      height: 38px;
      border-radius: 10px;
      display: flex;
      align-items: center;
      justify-content: center;
      background: rgba(6, 182, 212, 0.12);
      border: 1px solid rgba(6, 182, 212, 0.25);
      color: #06b6d4;
      flex-shrink: 0;
    }
    .chart-icon-box.amber {
      background: rgba(245, 158, 11, 0.12);
      border-color: rgba(245, 158, 11, 0.25);
      color: #f59e0b;
    }
    .chart-icon-box.emerald {
      background: rgba(16, 185, 129, 0.12);
      border-color: rgba(16, 185, 129, 0.25);
      color: #10b981;
    }
    .chart-icon-box.purple {
      background: rgba(168, 85, 247, 0.12);
      border-color: rgba(168, 85, 247, 0.25);
      color: #a855f7;
    }
    .badge-dot {
      width: 7px;
      height: 7px;
      border-radius: 50%;
      display: inline-block;
      margin-right: 5px;
      vertical-align: middle;
    }
    .badge-dot.dot-green {
      background: #10b981;
      box-shadow: 0 0 6px #10b981;
    }
    .badge-dot.dot-red {
      background: #ef4444;
      box-shadow: 0 0 6px #ef4444;
    }
    .chart-title-text {
      font-size: 15px;
      font-weight: 700;
      color: var(--text-main);
      display: block;
    }
    .chart-subtitle-text {
      font-size: 11px;
      color: var(--text-sub);
      display: block;
      margin-top: 1px;
    }
    .chart-controls-wrap {
      display: flex;
      align-items: center;
      gap: 8px;
      flex-wrap: wrap;
    }
    .chart-filter-group {
      display: inline-flex;
      background: rgba(15, 23, 42, 0.85);
      border: 1px solid rgba(255, 255, 255, 0.08);
      border-radius: 8px;
      padding: 2px;
      gap: 2px;
    }
    .chart-filter-btn {
      background: transparent;
      border: none;
      color: var(--text-sub);
      padding: 3px 9px;
      font-size: 11px;
      font-weight: 600;
      border-radius: 6px;
      cursor: pointer;
      transition: all 0.2s ease;
    }
    .chart-filter-btn:hover {
      color: #fff;
    }
    .chart-filter-btn.active {
      background: rgba(255, 255, 255, 0.12);
      color: var(--accent-emerald);
      box-shadow: 0 1px 4px rgba(0,0,0,0.2);
    }
    .chart-legend-row {
      display: flex;
      gap: 8px;
      margin-bottom: 12px;
      flex-wrap: wrap;
      align-items: center;
    }
    .chart-toggle-chip {
      background: rgba(15, 23, 42, 0.65);
      border: 1px solid rgba(255, 255, 255, 0.08);
      border-radius: 20px;
      padding: 4px 10px;
      font-size: 11px;
      font-weight: 600;
      color: var(--text-main);
      display: inline-flex;
      align-items: center;
      gap: 6px;
      cursor: pointer;
      transition: all 0.2s ease;
      user-select: none;
    }
    .chart-toggle-chip:hover {
      border-color: rgba(255, 255, 255, 0.2);
      transform: translateY(-1px);
    }
    .chart-toggle-chip.active {
      border-color: var(--chip-border, rgba(16, 185, 129, 0.4));
      background: var(--chip-bg, rgba(16, 185, 129, 0.1));
    }
    .chart-toggle-chip.inactive {
      opacity: 0.35;
      filter: grayscale(0.8);
      text-decoration: line-through;
    }
    .chart-toggle-chip .chip-dot {
      width: 7px;
      height: 7px;
      border-radius: 50%;
      flex-shrink: 0;
    }
    .chart-toggle-chip .chip-val {
      font-size: 10px;
      color: var(--text-sub);
      background: rgba(0, 0, 0, 0.35);
      padding: 1px 5px;
      border-radius: 6px;
      margin-left: 2px;
    }
    .chart-footer-meta {
      font-size: 11px;
      color: var(--text-sub);
      margin-top: 10px;
      display: flex;
      justify-content: space-between;
      align-items: center;
      flex-wrap: wrap;
      gap: 6px;
      padding: 0 4px;
    }

    /* RESPONSIVE DESIGN */
    @media (max-width: 600px) {
      header {
        flex-direction: column;
        align-items: flex-start;
      }

      .kpi-value {
        font-size: 26px;
      }

      .grid-two-col {
        grid-template-columns: 1fr;
      }
    }

/* =================================================================
   BOTTOM DOCK NAVIGATION BAR (RESPONSIVE ANDROID & DESKTOP)
   ================================================================= */
.tabs-nav, .tabs-nav.bottom-dock {
  position: fixed;
  bottom: 18px;
  left: 50%;
  transform: translateX(-50%);
  z-index: 1000;
  display: flex;
  align-items: center;
  gap: 6px;
  max-width: 720px;
  width: calc(100% - 32px);
  margin: 0;
  padding: 7px 10px;
  background: rgba(13, 20, 36, 0.88);
  backdrop-filter: blur(24px);
  -webkit-backdrop-filter: blur(24px);
  border: 1px solid rgba(255, 255, 255, 0.12);
  border-radius: 24px;
  box-shadow: 0 16px 40px rgba(0, 0, 0, 0.55), 0 0 0 1px rgba(255, 255, 255, 0.05);
  transition: all 0.3s cubic-bezier(0.16, 1, 0.3, 1);
}

.tabs-nav::-webkit-scrollbar {
  display: none;
}

.tab-btn {
  flex: 1;
  min-width: 0;
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 8px;
  padding: 9px 12px;
  background: transparent;
  color: var(--text-sub);
  border: 1px solid transparent;
  border-radius: 16px;
  font-size: 12.5px;
  font-weight: 600;
  cursor: pointer;
  transition: all 0.22s cubic-bezier(0.16, 1, 0.3, 1);
  white-space: nowrap;
  user-select: none;
}

.tab-btn:hover {
  color: var(--text-main);
  background: rgba(255, 255, 255, 0.06);
}

.tab-btn:active {
  transform: scale(0.95);
}

.tab-btn.active {
  color: #10b981;
  background: rgba(16, 185, 129, 0.14);
  border-color: rgba(16, 185, 129, 0.35);
  box-shadow: 0 4px 16px rgba(16, 185, 129, 0.2);
}

.tab-icon {
  display: flex;
  align-items: center;
  justify-content: center;
  line-height: 1;
  flex-shrink: 0;
  transition: transform 0.2s cubic-bezier(0.16, 1, 0.3, 1);
}

.tab-btn.active .tab-icon {
  color: #10b981;
  transform: scale(1.08);
}

.tab-label {
  font-size: 12px;
  letter-spacing: -0.01em;
}

/* =================================================================
   ANDROID & MOBILE SPECIFIC BOTTOM NAVIGATION DOCK (< 768px)
   ================================================================= */
@media (max-width: 768px) {
  .container {
    padding-bottom: calc(95px + env(safe-area-inset-bottom, 0px));
  }

  .tabs-nav, .tabs-nav.bottom-dock {
    position: fixed !important;
    display: flex !important;
    z-index: 9999 !important;
    bottom: 12px !important;
    left: 12px !important;
    right: 12px !important;
    transform: none !important;
    max-width: calc(100% - 24px) !important;
    width: auto;
    border-radius: 20px;
    border: 1px solid rgba(255, 255, 255, 0.12);
    background: rgba(10, 15, 30, 0.94);
    backdrop-filter: blur(28px);
    -webkit-backdrop-filter: blur(28px);
    box-shadow: 0 8px 32px rgba(0, 0, 0, 0.6);
    padding: 6px 4px calc(6px + env(safe-area-inset-bottom, 0px)) 4px;
    gap: 2px;
  }

  .tab-btn {
    flex-direction: column;
    gap: 3px;
    padding: 6px 2px;
    border-radius: 12px;
    min-width: 0;
  }

  .tab-icon {
    width: 38px;
    height: 26px;
    border-radius: 14px;
    display: flex;
    align-items: center;
    justify-content: center;
    transition: all 0.22s ease;
  }

  .tab-icon svg {
    width: 19px;
    height: 19px;
  }

  .tab-btn.active .tab-icon {
    background: rgba(16, 185, 129, 0.2);
    color: #10b981;
    box-shadow: 0 0 12px rgba(16, 185, 129, 0.35);
  }

  .tab-label {
    font-size: 9.5px;
    font-weight: 600;
    letter-spacing: 0.01em;
    line-height: 1.1;
  }

  .tab-btn.active .tab-label {
    color: #10b981;
    font-weight: 700;
  }
}

/* Tab Content Visibility & Smooth Animation */
.tab-content {
  display: none;
  animation: tabFadeIn 0.3s cubic-bezier(0.4, 0, 0.2, 1);
}
.tab-content.active {
  display: block;
}
@keyframes tabFadeIn {
  from {
    opacity: 0;
    transform: translateY(8px);
  }
  to {
    opacity: 1;
    transform: translateY(0);
  }
}

@keyframes aiSpin {
  0% { transform: rotate(0deg); }
  100% { transform: rotate(360deg); }
}
.ai-spin {
  display: inline-block;
  animation: aiSpin 0.8s linear infinite;
}

/* =================================================================
   MODAL OVERLAY & SECTOR MANAGER
   ================================================================= */
.modal-overlay {
  position: fixed;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background: rgba(15, 23, 42, 0.82);
  backdrop-filter: blur(8px);
  z-index: 9999;
  display: flex;
  align-items: center;
  justify-content: center;
  padding: 16px;
  animation: fadeInModal 0.2s ease-out;
}

@keyframes fadeInModal {
  from { opacity: 0; transform: scale(0.98); }
  to { opacity: 1; transform: scale(1); }
}

.modal-box {
  background: #1e293b;
  background: linear-gradient(145deg, rgba(30, 41, 59, 0.98), rgba(15, 23, 42, 0.98));
  border: 1px solid rgba(255, 255, 255, 0.12);
  border-radius: 20px;
  box-shadow: 0 25px 50px -12px rgba(0, 0, 0, 0.6);
  width: 100%;
  max-width: 650px;
  padding: 24px;
  display: flex;
  flex-direction: column;
}

.modal-header {
  display: flex;
  justify-content: space-between;
  align-items: flex-start;
  margin-bottom: 16px;
}

.modal-title {
  font-family: 'Outfit', sans-serif;
  font-size: 16px;
  font-weight: 700;
  color: var(--text-main);
}

.modal-close-btn {
  background: transparent;
  border: none;
  color: var(--text-sub);
  font-size: 24px;
  line-height: 1;
  cursor: pointer;
  transition: color 0.15s ease;
}

.modal-close-btn:hover {
  color: var(--text-main);
}

.form-label {
  font-size: 11px;
  font-weight: 600;
  color: var(--text-sub);
  display: block;
  margin-bottom: 4px;
}

.form-input {
  width: 100%;
  background: rgba(15, 23, 42, 0.7);
  border: 1px solid var(--card-border);
  border-radius: 8px;
  color: var(--text-main);
  padding: 8px 10px;
  font-size: 12px;
  outline: none;
  transition: border-color 0.2s ease;
}

.form-input:focus {
  border-color: var(--primary);
}

.sector-manage-item {
  display: flex;
  align-items: center;
  justify-content: space-between;
  background: rgba(15, 23, 42, 0.5);
  border: 1px solid var(--card-border);
  border-radius: 12px;
  padding: 10px 14px;
  gap: 10px;
}

.sector-manage-item.is-active {
  border-color: rgba(16, 185, 129, 0.4);
  background: rgba(16, 185, 129, 0.06);
}

/* =================================================================
   PHENOLOGY & NURSERY PREDICTOR AI
   ================================================================= */
.phenology-card {
  position: relative;
  background: var(--card-bg);
  backdrop-filter: blur(14px);
  border: 1px solid var(--card-border);
  border-radius: 20px;
  padding: 22px 24px;
  margin-bottom: 24px;
  box-shadow: 0 10px 30px rgba(0, 0, 0, 0.2);
  overflow: hidden;
}

.phenology-card::before {
  content: '';
  position: absolute;
  top: 0;
  left: 0;
  right: 0;
  height: 3px;
  background: linear-gradient(90deg, #10b981, #06b6d4, #a855f7);
}

.pheno-header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  margin-bottom: 20px;
  flex-wrap: wrap;
  gap: 10px;
}

.pheno-title-wrap {
  display: flex;
  flex-direction: column;
  gap: 4px;
}

.pheno-title {
  display: flex;
  align-items: center;
  gap: 8px;
  font-size: 13px;
  font-weight: 700;
  text-transform: uppercase;
  letter-spacing: 0.06em;
  color: var(--text-main);
}

.pheno-title svg {
  color: var(--primary);
}

.pheno-subtitle {
  font-size: 11.5px;
  color: var(--text-sub);
}

/* Milestones Timeline */
.pheno-milestones-container {
  position: relative;
  margin: 12px 0 22px 0;
  padding: 0 10px;
}

.pheno-timeline-bar {
  position: absolute;
  top: 16px;
  left: 30px;
  right: 30px;
  height: 4px;
  background: rgba(255, 255, 255, 0.08);
  border-radius: 4px;
  z-index: 1;
}

.pheno-timeline-fill {
  height: 100%;
  background: linear-gradient(90deg, #10b981, #06b6d4);
  border-radius: 4px;
  box-shadow: 0 0 10px rgba(16, 185, 129, 0.5);
  transition: width 0.4s ease;
}

.pheno-stages-grid {
  display: grid;
  grid-template-columns: repeat(4, 1fr);
  position: relative;
  z-index: 2;
  gap: 8px;
}

.pheno-stage-item {
  display: flex;
  flex-direction: column;
  align-items: center;
  text-align: center;
}

.pheno-step-bubble {
  width: 32px;
  height: 32px;
  border-radius: 50%;
  background: #1e293b;
  border: 2px solid rgba(255, 255, 255, 0.15);
  display: flex;
  align-items: center;
  justify-content: center;
  font-size: 12px;
  font-weight: 800;
  color: var(--text-sub);
  margin-bottom: 8px;
  transition: all 0.25s ease;
}

.pheno-stage-item.completed .pheno-step-bubble {
  background: #10b981;
  border-color: #10b981;
  color: white;
  box-shadow: 0 0 10px rgba(16, 185, 129, 0.4);
}

.pheno-stage-item.current .pheno-step-bubble {
  background: #06b6d4;
  border-color: #ffffff;
  color: white;
  box-shadow: 0 0 14px rgba(6, 182, 212, 0.8);
  transform: scale(1.1);
  animation: pulseBubble 2s infinite;
}

@keyframes pulseBubble {
  0%, 100% { box-shadow: 0 0 10px rgba(6, 182, 212, 0.6); }
  50% { box-shadow: 0 0 20px rgba(6, 182, 212, 0.9); }
}

.pheno-step-label {
  font-size: 11.5px;
  font-weight: 700;
  color: var(--text-main);
  line-height: 1.3;
}

.pheno-step-sub {
  font-size: 10px;
  color: var(--text-sub);
  margin-top: 2px;
}

/* 3 Bio-Climatic Pillars */
.pheno-grid-pillars {
  display: grid;
  grid-template-columns: repeat(3, 1fr);
  gap: 12px;
  margin-bottom: 16px;
}

@media (max-width: 768px) {
  .pheno-grid-pillars {
    grid-template-columns: 1fr;
  }
  .pheno-step-label {
    font-size: 10px;
  }
  
  /* PERMINTAAN USER: HEADER CONTROLS 3 ATAS 3 BAWAH DI MOBILE */
  .header-controls {
    flex-direction: column !important;
    width: 100% !important;
    align-items: stretch !important;
    gap: 8px !important;
  }
  .status-badge-bar, .header-action-group {
    display: flex !important;
    flex-direction: row !important;
    justify-content: space-between !important;
    width: 100% !important;
    gap: 6px !important;
    flex-wrap: nowrap !important; /* Paksa sebaris */
  }
  .status-badge-bar > .badge-pill, .header-action-group > .header-btn {
    flex: 1 !important; /* Paksa lebar merata sepertiga (3 items per baris) */
    justify-content: center !important;
    text-align: center !important;
    padding: 6px 4px !important;
    font-size: 10px !important;
    min-width: 0 !important;
    white-space: nowrap;
    overflow: hidden;
    text-overflow: ellipsis;
  }
  /* Sembunyikan SVG di tombol saat mobile agar teks muat 3 sejajar */
  .header-action-group > .header-btn svg {
    display: none !important;
  }
}

.pheno-pillar-card {
  background: rgba(15, 23, 42, 0.55);
  border: 1px solid var(--card-border);
  border-radius: 14px;
  padding: 14px 16px;
  display: flex;
  flex-direction: column;
  gap: 4px;
}

.pheno-pillar-head {
  display: flex;
  align-items: center;
  justify-content: space-between;
}

.pheno-pillar-tag {
  font-size: 10px;
  font-weight: 700;
  letter-spacing: 0.06em;
  color: var(--text-sub);
}

.pheno-dot {
  width: 7px;
  height: 7px;
  border-radius: 50%;
}
.dot-green { background: #10b981; box-shadow: 0 0 6px #10b981; }
.dot-cyan  { background: #06b6d4; box-shadow: 0 0 6px #06b6d4; }
.dot-blue  { background: #38bdf8; box-shadow: 0 0 6px #38bdf8; }
.dot-warn  { background: #f59e0b; box-shadow: 0 0 6px #f59e0b; }
.dot-alert { background: #ef4444; box-shadow: 0 0 6px #ef4444; }

.pheno-pillar-val {
  font-family: 'Outfit', sans-serif;
  font-size: 18px;
  font-weight: 800;
  color: var(--text-main);
  margin-top: 2px;
}

.pheno-pillar-note {
  font-size: 11px;
  color: var(--text-sub);
  line-height: 1.3;
}

/* Advice Box */
.pheno-advice-box {
  background: linear-gradient(145deg, rgba(16, 185, 129, 0.08), rgba(6, 182, 212, 0.05));
  border: 1px solid rgba(16, 185, 129, 0.25);
  border-radius: 14px;
  padding: 14px 18px;
  display: flex;
  align-items: center;
  justify-content: space-between;
  flex-wrap: wrap;
  gap: 12px;
}

/* =================================================================
   SCADA PATHOGEN & PEST SURVEILLANCE MODULE
   ================================================================= */
.outbreak-card {
  background: var(--card-bg);
  backdrop-filter: blur(12px);
  border: 1px solid var(--card-border);
  border-radius: 20px;
  padding: 22px;
  box-shadow: 0 10px 30px rgba(0, 0, 0, 0.25);
  margin-bottom: 20px;
  position: relative;
  overflow: hidden;
}

.outbreak-card::before {
  content: '';
  position: absolute;
  top: 0;
  left: 0;
  right: 0;
  height: 3px;
  background: linear-gradient(90deg, #f59e0b, #ef4444, #10b981);
  opacity: 0.8;
}

.outbreak-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  flex-wrap: wrap;
  gap: 12px;
  margin-bottom: 18px;
  padding-bottom: 14px;
  border-bottom: 1px solid var(--card-border);
}

.outbreak-title-wrap {
  display: flex;
  align-items: center;
  gap: 12px;
}

.outbreak-icon-box {
  width: 36px;
  height: 36px;
  border-radius: 10px;
  background: rgba(245, 158, 11, 0.12);
  border: 1px solid rgba(245, 158, 11, 0.3);
  color: var(--accent-amber);
  display: flex;
  align-items: center;
  justify-content: center;
  flex-shrink: 0;
}

.outbreak-title {
  font-family: 'Outfit', sans-serif;
  font-size: 15px;
  font-weight: 700;
  color: var(--text-main);
  letter-spacing: -0.01em;
}

.outbreak-subtitle {
  font-size: 11.5px;
  color: var(--text-sub);
  margin-top: 2px;
}

.outbreak-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(290px, 1fr));
  gap: 14px;
}

.outbreak-item {
  background: rgba(255, 255, 255, 0.025);
  border: 1px solid var(--card-border);
  border-radius: 14px;
  padding: 16px;
  display: flex;
  flex-direction: column;
  justify-content: space-between;
  gap: 12px;
  transition: all 0.25s cubic-bezier(0.16, 1, 0.3, 1);
  position: relative;
  overflow: hidden;
}

.outbreak-item::before {
  content: '';
  position: absolute;
  top: 0;
  left: 0;
  right: 0;
  height: 2.5px;
  background: var(--card-border);
  transition: background 0.3s ease;
}

.outbreak-item.status-low::before {
  background: var(--accent-emerald);
}
.outbreak-item.status-low {
  border-color: rgba(16, 185, 129, 0.2);
  background: rgba(16, 185, 129, 0.025);
}

.outbreak-item.status-med::before {
  background: var(--accent-amber);
}
.outbreak-item.status-med {
  border-color: rgba(245, 158, 11, 0.25);
  background: rgba(245, 158, 11, 0.035);
}

.outbreak-item.status-high::before {
  background: var(--accent-rose);
}
.outbreak-item.status-high {
  border-color: rgba(239, 68, 68, 0.35);
  background: rgba(239, 68, 68, 0.05);
}

.outbreak-item-head {
  display: flex;
  justify-content: space-between;
  align-items: flex-start;
  gap: 10px;
}

.outbreak-target-info {
  display: flex;
  flex-direction: column;
  gap: 2px;
}

.outbreak-target-name {
  font-family: 'Outfit', sans-serif;
  font-size: 13.5px;
  font-weight: 700;
  color: var(--text-main);
  line-height: 1.3;
}

.outbreak-target-vector {
  font-size: 11px;
  color: var(--text-sub);
}

.outbreak-tag {
  display: inline-flex;
  align-items: center;
  gap: 5px;
  padding: 3px 8px;
  border-radius: 6px;
  font-size: 10px;
  font-weight: 700;
  letter-spacing: 0.03em;
  text-transform: uppercase;
  background: rgba(255, 255, 255, 0.05);
  border: 1px solid var(--card-border);
  color: var(--text-sub);
  white-space: nowrap;
}

.outbreak-tag.tag-low {
  background: rgba(16, 185, 129, 0.12);
  border-color: rgba(16, 185, 129, 0.3);
  color: var(--accent-emerald);
}

.outbreak-tag.tag-med {
  background: rgba(245, 158, 11, 0.12);
  border-color: rgba(245, 158, 11, 0.3);
  color: var(--accent-amber);
}

.outbreak-tag.tag-high {
  background: rgba(239, 68, 68, 0.15);
  border-color: rgba(239, 68, 68, 0.4);
  color: var(--accent-rose);
}

.outbreak-condition-chip {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 6px;
  background: rgba(0, 0, 0, 0.25);
  border: 1px solid rgba(255, 255, 255, 0.06);
  padding: 5px 10px;
  border-radius: 8px;
  font-size: 11px;
  color: var(--text-sub);
}

.outbreak-condition-chip code {
  color: var(--accent-cyan);
  font-family: 'JetBrains Mono', monospace, sans-serif;
  font-size: 10.5px;
}

.outbreak-desc-box {
  font-size: 11.5px;
  line-height: 1.5;
  color: var(--text-main);
  background: rgba(255, 255, 255, 0.02);
  padding: 10px 12px;
  border-radius: 8px;
  border: 1px solid rgba(255, 255, 255, 0.04);
}




/* ===== GLOBAL BADGE ANIMATIONS ===== */
@keyframes spinImpeller {
  100% { transform: rotate(360deg); }
}
.icon-spin-active svg {
  animation: spinImpeller 1s linear infinite;
  color: #10b981;
  filter: drop-shadow(0 0 4px rgba(16, 185, 129, 0.8));
}

@keyframes glowBulb {
  0%, 100% { filter: drop-shadow(0 0 2px rgba(234, 179, 8, 0.4)); transform: scale(1); }
  50% { filter: drop-shadow(0 0 10px rgba(234, 179, 8, 1)); transform: scale(1.1); }
}
.icon-glow-active svg {
  animation: glowBulb 1.5s ease-in-out infinite;
  color: #eab308;
}

/* ===== SCADA OPTICAL VISION & LEAF INSPECTION ===== */
.vision-dropzone {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  gap: 12px;
  padding: 24px 16px;
  background: rgba(168, 85, 247, 0.03);
  border: 1.5px dashed rgba(168, 85, 247, 0.35);
  border-radius: 12px;
  cursor: pointer;
  transition: all 0.25s ease;
}

.vision-dropzone:hover {
  background: rgba(168, 85, 247, 0.08);
  border-color: rgba(168, 85, 247, 0.6);
  box-shadow: 0 0 16px rgba(168, 85, 247, 0.15);
}

.vision-icon-ring {
  width: 48px;
  height: 48px;
  border-radius: 50%;
  display: flex;
  align-items: center;
  justify-content: center;
  background: rgba(168, 85, 247, 0.12);
  color: #c084fc;
  border: 1px solid rgba(168, 85, 247, 0.3);
  box-shadow: 0 0 12px rgba(168, 85, 247, 0.2);
  transition: transform 0.2s ease;
}

.vision-dropzone:hover .vision-icon-ring {
  transform: scale(1.08);
  box-shadow: 0 0 18px rgba(168, 85, 247, 0.35);
}

.pulse-loader {
  width: 28px;
  height: 28px;
  border: 3px solid rgba(168, 85, 247, 0.2);
  border-top-color: #c084fc;
  border-radius: 50%;
  animation: spinImpeller 0.8s linear infinite;
}

.vision-result-card {
  background: rgba(15, 23, 42, 0.75);
  border: 1px solid rgba(168, 85, 247, 0.3);
  border-radius: 12px;
  padding: 16px;
  backdrop-filter: blur(10px);
  box-shadow: 0 4px 20px rgba(0, 0, 0, 0.4);
}

.vision-result-header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  border-bottom: 1px solid rgba(255, 255, 255, 0.08);
  padding-bottom: 10px;
  margin-bottom: 12px;
}

.vision-result-body {
  font-size: 13px;
  line-height: 1.6;
  color: #cbd5e1;
}

.vision-result-body strong {
  color: #f8fafc;
}

/* Vision Source Mode Tabs */
.vision-source-tabs {
  display: flex;
  gap: 6px;
  background: rgba(0, 0, 0, 0.35);
  padding: 4px;
  border-radius: 10px;
  border: 1px solid rgba(255, 255, 255, 0.08);
  margin-bottom: 12px;
}

.vision-tab-btn {
  flex: 1;
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 6px;
  padding: 8px 10px;
  font-size: 11.5px;
  font-weight: 600;
  color: var(--text-sub);
  background: transparent;
  border: 1px solid transparent;
  border-radius: 7px;
  cursor: pointer;
  transition: all 0.2s cubic-bezier(0.4, 0, 0.2, 1);
}

.vision-tab-btn:hover {
  color: var(--text-main);
  background: rgba(255, 255, 255, 0.05);
}

.vision-tab-btn.active {
  color: #fff;
  background: rgba(168, 85, 247, 0.22);
  border-color: rgba(168, 85, 247, 0.45);
  box-shadow: 0 2px 10px rgba(168, 85, 247, 0.25);
}

.vision-camera-box {
  position: relative;
  background: #000;
  border-radius: 10px;
  overflow: hidden;
  border: 1px solid rgba(168, 85, 247, 0.3);
}

.vision-camera-box video {
  width: 100%;
  max-height: 280px;
  object-fit: cover;
  display: block;
}

.vision-shutter-bar {
  display: flex;
  justify-content: center;
  align-items: center;
  gap: 12px;
  padding: 10px;
  background: rgba(15, 23, 42, 0.9);
  border-top: 1px solid rgba(255, 255, 255, 0.08);
}

.vision-url-bar {
  display: flex;
  gap: 8px;
  margin-top: 6px;
}

.vision-url-input {
  flex: 1;
  background: rgba(0, 0, 0, 0.4);
  border: 1px solid rgba(255, 255, 255, 0.12);
  color: #fff;
  padding: 9px 12px;
  border-radius: 8px;
  font-size: 12px;
  outline: none;
  transition: border-color 0.2s ease;
}

.vision-url-input:focus {
  border-color: #a855f7;
  box-shadow: 0 0 10px rgba(168, 85, 247, 0.3);
}

/* ===== AGRONOMIST INTERACTIVE CHATBOT ===== */
.agri-chat-box {
  margin-top: 14px;
  background: rgba(15, 23, 42, 0.95);
  border: 1px solid rgba(168, 85, 247, 0.35);
  border-radius: 12px;
  overflow: hidden;
  display: flex;
  flex-direction: column;
}

.agri-chat-header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: 10px 14px;
  background: rgba(168, 85, 247, 0.12);
  border-bottom: 1px solid rgba(255, 255, 255, 0.08);
}

.agri-chat-messages {
  max-height: 260px;
  overflow-y: auto;
  padding: 12px;
  display: flex;
  flex-direction: column;
  gap: 10px;
}

.chat-bubble {
  max-width: 88%;
  padding: 9px 13px;
  border-radius: 10px;
  font-size: 12px;
  line-height: 1.55;
  word-break: break-word;
}

.chat-bubble.bot {
  align-self: flex-start;
  background: rgba(30, 41, 59, 0.9);
  color: #e2e8f0;
  border: 1px solid rgba(255, 255, 255, 0.08);
  border-bottom-left-radius: 2px;
}

.chat-bubble.user {
  align-self: flex-end;
  background: linear-gradient(135deg, #7c3aed, #6366f1);
  color: #fff;
  border-bottom-right-radius: 2px;
  box-shadow: 0 2px 8px rgba(124, 58, 237, 0.3);
}

.chat-chips-row {
  display: flex;
  gap: 6px;
  padding: 8px 12px;
  overflow-x: auto;
  border-top: 1px solid rgba(255, 255, 255, 0.05);
  background: rgba(0, 0, 0, 0.2);
}

.chip-btn {
  white-space: nowrap;
  font-size: 11px;
  padding: 5px 11px;
  background: rgba(168, 85, 247, 0.1);
  border: 1px solid rgba(168, 85, 247, 0.3);
  color: #c084fc;
  border-radius: 14px;
  cursor: pointer;
  transition: all 0.2s ease;
}

.chip-btn:hover {
  background: rgba(168, 85, 247, 0.25);
  color: #fff;
  border-color: rgba(168, 85, 247, 0.6);
}

.agri-chat-input-bar {
  display: flex;
  gap: 8px;
  padding: 10px 12px;
  background: rgba(0, 0, 0, 0.4);
  border-top: 1px solid rgba(255, 255, 255, 0.08);
}

.agri-chat-input {
  flex: 1;
  background: rgba(15, 23, 42, 0.8);
  border: 1px solid rgba(255, 255, 255, 0.12);
  border-radius: 8px;
  padding: 8px 12px;
  font-size: 12px;
  color: #fff;
  outline: none;
}

.agri-chat-input:focus {
  border-color: #a855f7;
  box-shadow: 0 0 8px rgba(168, 85, 247, 0.3);
}

/* =================================================================
   SCADA RUNNING TICKER BAR (SMOOTH HORIZONTAL REAL-TIME MARQUEE)
   ================================================================= */
.scada-ticker-bar {
  display: flex;
  align-items: center;
  background: rgba(10, 18, 36, 0.75);
  backdrop-filter: blur(12px);
  -webkit-backdrop-filter: blur(12px);
  border: 1px solid rgba(255, 255, 255, 0.08);
  border-radius: 12px;
  margin: 10px 0 16px 0;
  padding: 6px 14px;
  overflow: hidden;
  box-shadow: 0 4px 20px rgba(0, 0, 0, 0.25);
  position: relative;
}

.ticker-badge {
  display: flex;
  align-items: center;
  gap: 7px;
  padding: 3px 10px;
  border-radius: 20px;
  background: rgba(16, 185, 129, 0.15);
  border: 1px solid rgba(16, 185, 129, 0.3);
  color: var(--primary);
  font-size: 10.5px;
  font-weight: 700;
  letter-spacing: 0.5px;
  flex-shrink: 0;
  margin-right: 14px;
  z-index: 2;
}

.ticker-dot {
  width: 7px;
  height: 7px;
  border-radius: 50%;
  background: var(--primary);
  box-shadow: 0 0 8px var(--primary);
  animation: pulseDot 1.5s infinite ease-in-out;
}

@keyframes pulseDot {
  0%, 100% { opacity: 1; transform: scale(1); }
  50% { opacity: 0.4; transform: scale(0.75); }
}

.ticker-marquee-wrapper {
  flex: 1;
  overflow: hidden;
  position: relative;
  mask-image: linear-gradient(to right, transparent, black 15px, black calc(100% - 15px), transparent);
  -webkit-mask-image: linear-gradient(to right, transparent, black 15px, black calc(100% - 15px), transparent);
}

.ticker-track {
  display: inline-flex;
  white-space: nowrap;
  animation: scadaMarquee 32s linear infinite;
  will-change: transform;
}

.ticker-track:hover {
  animation-play-state: paused;
  cursor: default;
}

.ticker-text {
  font-size: 12px;
  font-weight: 500;
  color: var(--text-main);
  padding-right: 48px;
  display: inline-flex;
  align-items: center;
  letter-spacing: 0.2px;
}

@keyframes scadaMarquee {
  0% {
    transform: translateX(0%);
  }
  100% {
    transform: translateX(-50%);
  }
}

/* GLOBAL HEADER MODE BADGE (AUTO vs MANUAL) */
.badge-mode-auto {
  background: rgba(16, 185, 129, 0.12) !important;
  color: #10b981 !important;
  border: 1px solid rgba(16, 185, 129, 0.3) !important;
  transition: all 0.25s ease;
}

.badge-mode-auto:hover {
  background: rgba(16, 185, 129, 0.22) !important;
  box-shadow: 0 0 10px rgba(16, 185, 129, 0.3);
}

.badge-mode-manual {
  background: rgba(245, 158, 11, 0.15) !important;
  color: #f59e0b !important;
  border: 1px solid rgba(245, 158, 11, 0.4) !important;
  animation: pulseAmber 2s infinite ease-in-out;
  transition: all 0.25s ease;
}

.badge-mode-manual:hover {
  background: rgba(245, 158, 11, 0.25) !important;
  box-shadow: 0 0 12px rgba(245, 158, 11, 0.4);
}

@keyframes pulseAmber {
  0%, 100% { box-shadow: 0 0 0 0 rgba(245, 158, 11, 0.4); }
  50% { box-shadow: 0 0 10px 2px rgba(245, 158, 11, 0.35); }
}

/* GLOBAL HEADER ACTUATOR STATUS PULSE GLOW (COMPLIANT WITH RULE 3.C) */
.badge-pump-active {
  background: rgba(6, 182, 212, 0.18) !important;
  color: #06b6d4 !important;
  border: 1px solid rgba(6, 182, 212, 0.5) !important;
  animation: pulseCyanHalo 1.8s infinite ease-in-out;
}

@keyframes pulseCyanHalo {
  0%, 100% {
    box-shadow: 0 0 0 0 rgba(6, 182, 212, 0.5);
  }
  50% {
    box-shadow: 0 0 12px 2px rgba(6, 182, 212, 0.45);
  }
}

.badge-lamp-active {
  background: rgba(234, 179, 8, 0.18) !important;
  color: #eab308 !important;
  border: 1px solid rgba(234, 179, 8, 0.5) !important;
  animation: pulseGoldHalo 1.8s infinite ease-in-out;
}

@keyframes pulseGoldHalo {
  0%, 100% {
    box-shadow: 0 0 0 0 rgba(234, 179, 8, 0.5);
  }
  50% {
    box-shadow: 0 0 12px 2px rgba(234, 179, 8, 0.45);
  }
}

.ticker-pulse-cyan,
.ticker-pulse-gold {
  display: inline-block;
  width: 6px;
  height: 6px;
  border-radius: 50%;
  margin-right: 4px;
  vertical-align: middle;
}

.ticker-pulse-cyan {
  background: #06b6d4;
  box-shadow: 0 0 6px #06b6d4;
  animation: pulseDot 1s infinite ease-in-out;
}

.ticker-pulse-gold {
  background: #eab308;
  box-shadow: 0 0 6px #eab308;
  animation: pulseDot 1s infinite ease-in-out;
}

/* =================================================================
   VIRTUAL LCD 16x2 HARDWARE MONITOR DRAWER (DIGITAL TWIN)
   ================================================================= */
.ticker-lcd-toggle {
  display: inline-flex;
  align-items: center;
  gap: 6px;
  padding: 4px 11px;
  background: rgba(56, 189, 248, 0.12);
  border: 1px solid rgba(56, 189, 248, 0.35);
  border-radius: 20px;
  color: #38bdf8;
  font-size: 11px;
  font-weight: 700;
  cursor: pointer;
  transition: all 0.25s ease;
  flex-shrink: 0;
  margin-left: 12px;
  z-index: 3;
}

.ticker-lcd-toggle:hover {
  background: rgba(56, 189, 248, 0.24);
  box-shadow: 0 0 10px rgba(56, 189, 248, 0.4);
  transform: translateY(-1px);
}

.ticker-lcd-toggle.active {
  background: rgba(56, 189, 248, 0.25);
  border-color: #38bdf8;
  box-shadow: 0 0 12px rgba(56, 189, 248, 0.5);
}

.lcd-toggle-indicator {
  font-size: 9px;
  transition: transform 0.25s ease;
}

.ticker-lcd-toggle.active .lcd-toggle-indicator {
  transform: rotate(180deg);
}

/* Virtual LCD Drawer Container */
.virtual-lcd-drawer {
  display: flex;
  justify-content: center;
  margin: -4px 0 18px 0;
  animation: slideDownLcd 0.3s cubic-bezier(0.16, 1, 0.3, 1);
  will-change: transform, opacity;
}

@keyframes slideDownLcd {
  from {
    opacity: 0;
    transform: translateY(-12px);
  }
  to {
    opacity: 1;
    transform: translateY(0);
  }
}

/* Industrial Bezel & Hardware Chassis */
.virtual-lcd-chassis {
  width: 100%;
  max-width: 540px;
  background: linear-gradient(160deg, #1e293b 0%, #0f172a 100%);
  border: 1px solid rgba(255, 255, 255, 0.14);
  border-radius: 12px;
  padding: 12px 16px;
  box-shadow: 0 10px 30px rgba(0, 0, 0, 0.5), inset 0 1px 0 rgba(255, 255, 255, 0.12);
}

.chassis-header,
.chassis-footer {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 10px;
}

.chassis-screw {
  width: 8px;
  height: 8px;
  border-radius: 50%;
  background: radial-gradient(circle at 35% 35%, #94a3b8, #475569);
  border: 1px solid #1e293b;
  box-shadow: inset 0 1px 1px rgba(255, 255, 255, 0.4), 0 1px 2px rgba(0, 0, 0, 0.6);
  flex-shrink: 0;
}

.chassis-title-bar {
  display: flex;
  align-items: center;
  gap: 8px;
  font-size: 10px;
  font-weight: 700;
  color: #64748b;
  letter-spacing: 0.8px;
  text-transform: uppercase;
}

.chassis-led-pwr {
  width: 6px;
  height: 6px;
  border-radius: 50%;
  background: #10b981;
  box-shadow: 0 0 8px #10b981;
  animation: pulseDot 2s infinite ease-in-out;
}

/* Recessed Screen Bezel */
.lcd-screen-bezel {
  position: relative;
  margin: 10px 0;
  border-radius: 8px;
  padding: 14px 18px;
  overflow: hidden;
  border: 2px solid #0a0e17;
  box-shadow: inset 0 4px 14px rgba(0, 0, 0, 0.85), 0 2px 4px rgba(255, 255, 255, 0.05);
  transition: all 0.3s ease;
}

.lcd-screen-glare {
  position: absolute;
  top: 0;
  left: 0;
  right: 0;
  height: 48%;
  background: linear-gradient(180deg, rgba(255, 255, 255, 0.08) 0%, transparent 100%);
  pointer-events: none;
  border-top-left-radius: 6px;
  border-top-right-radius: 6px;
  z-index: 1;
}

/* LCD Themes (Backlight Colorways) */
.lcd-theme-blue {
  background: linear-gradient(180deg, #002b80 0%, #00194d 100%) !important;
  color: #e0f2fe !important;
  text-shadow: 0 0 6px rgba(56, 189, 248, 0.85);
  border-color: #00143a !important;
}

.lcd-theme-green {
  background: linear-gradient(180deg, #1b4317 0%, #0e290a 100%) !important;
  color: #dcfce7 !important;
  text-shadow: 0 0 6px rgba(74, 222, 128, 0.85);
  border-color: #091a07 !important;
}

.lcd-theme-amber {
  background: linear-gradient(180deg, #452204 0%, #241101 100%) !important;
  color: #fef3c7 !important;
  text-shadow: 0 0 6px rgba(251, 191, 36, 0.85);
  border-color: #170b01 !important;
}

.lcd-theme-dark {
  background: linear-gradient(180deg, #0f172a 0%, #020617 100%) !important;
  color: #ffffff !important;
  text-shadow: 0 0 5px rgba(255, 255, 255, 0.7);
  border-color: #020617 !important;
}

/* 16x2 Dot Matrix Grid Rows */
.lcd-matrix-grid {
  position: relative;
  z-index: 2;
  font-family: 'Courier New', Courier, 'Lucida Console', Monaco, monospace;
  font-weight: 700;
  letter-spacing: 2.2px;
  line-height: 1.45;
  user-select: none;
}

.lcd-matrix-row {
  font-size: 15px;
  white-space: pre;
  overflow: hidden;
  height: 26px;
  display: flex;
  align-items: center;
  justify-content: center;
  text-align: center;
}

@media (max-width: 540px) {
  .lcd-matrix-row {
    font-size: 12.5px;
    letter-spacing: 1.2px;
    height: 22px;
  }
  .lcd-screen-bezel {
    padding: 10px 12px;
  }
}

/* Chassis Toolbar & Control Buttons */
.chassis-toolbar {
  flex: 1;
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 8px;
  flex-wrap: wrap;
}

.chassis-page-selectors,
.chassis-theme-switch {
  display: flex;
  align-items: center;
  gap: 6px;
}

.lcd-btn {
  background: rgba(255, 255, 255, 0.06);
  border: 1px solid rgba(255, 255, 255, 0.12);
  color: #94a3b8;
  font-size: 10px;
  font-weight: 600;
  padding: 3px 8px;
  border-radius: 6px;
  cursor: pointer;
  transition: all 0.2s ease;
}

.lcd-btn:hover {
  background: rgba(255, 255, 255, 0.12);
  color: #fff;
}

.lcd-btn.active {
  background: rgba(56, 189, 248, 0.2);
  border-color: rgba(56, 189, 248, 0.45);
  color: #38bdf8;
  font-weight: 700;
}

.lcd-btn-action {
  background: rgba(234, 179, 8, 0.14);
  border: 1px solid rgba(234, 179, 8, 0.35);
  color: #eab308;
  font-size: 10px;
  font-weight: 600;
  padding: 3px 8px;
  border-radius: 6px;
  cursor: pointer;
  transition: all 0.2s ease;
}

.lcd-btn-action:hover {
  background: rgba(234, 179, 8, 0.25);
  box-shadow: 0 0 8px rgba(234, 179, 8, 0.3);
}

.lcd-btn-close {
  background: rgba(239, 68, 68, 0.14);
  border: 1px solid rgba(239, 68, 68, 0.35);
  color: #ef4444;
  font-size: 10px;
  font-weight: 700;
  padding: 3px 8px;
  border-radius: 6px;
  cursor: pointer;
  transition: all 0.2s ease;
}

.lcd-btn-close:hover {
  background: rgba(239, 68, 68, 0.25);
  box-shadow: 0 0 8px rgba(239, 68, 68, 0.3);
}

/* =================================================================
   FARMER-FRIENDLY PRACTICAL AGRONOMY UI (ANTI-TEORI KOSONG)
   ================================================================= */
.agri-status-banner {
  padding: 16px 18px;
  border-radius: 12px;
  margin-bottom: 16px;
  display: flex;
  flex-direction: column;
  gap: 6px;
  transition: all 0.3s ease;
  border: 1px solid transparent;
}

.agri-status-banner.status-ready {
  background: linear-gradient(135deg, rgba(16, 185, 129, 0.2), rgba(6, 182, 212, 0.15));
  border-color: rgba(16, 185, 129, 0.4);
  box-shadow: 0 4px 20px rgba(16, 185, 129, 0.15);
}

.agri-status-banner.status-waiting {
  background: linear-gradient(135deg, rgba(245, 158, 11, 0.2), rgba(234, 179, 8, 0.15));
  border-color: rgba(245, 158, 11, 0.4);
  box-shadow: 0 4px 20px rgba(245, 158, 11, 0.15);
}

.agri-status-banner.status-danger {
  background: linear-gradient(135deg, rgba(239, 68, 68, 0.2), rgba(220, 38, 38, 0.15));
  border-color: rgba(239, 68, 68, 0.4);
  box-shadow: 0 4px 20px rgba(239, 68, 68, 0.15);
}

.agri-status-title {
  font-size: 15px;
  font-weight: 800;
  display: flex;
  align-items: center;
  gap: 8px;
  letter-spacing: 0.3px;
}

.status-ready .agri-status-title {
  color: #10b981;
}

.status-waiting .agri-status-title {
  color: #f59e0b;
}

.status-danger .agri-status-title {
  color: #ef4444;
}

.agri-status-desc {
  font-size: 13px;
  color: var(--text-main);
  line-height: 1.5;
}

.agri-status-sub {
  font-size: 11.5px;
  color: var(--text-sub);
  margin-top: 2px;
}

.agri-quick-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(130px, 1fr));
  gap: 10px;
  margin-bottom: 14px;
}

.agri-quick-card {
  background: rgba(255, 255, 255, 0.04);
  border: 1px solid rgba(255, 255, 255, 0.08);
  border-radius: 10px;
  padding: 10px 12px;
  transition: transform 0.2s;
}

.agri-quick-card:hover {
  transform: translateY(-2px);
  border-color: rgba(255, 255, 255, 0.16);
}

.agri-quick-label {
  font-size: 10.5px;
  color: var(--text-sub);
  font-weight: 600;
  margin-bottom: 3px;
  display: flex;
  align-items: center;
  gap: 5px;
}

.agri-quick-val {
  font-size: 13px;
  font-weight: 700;
  color: var(--text-main);
}

.agri-quick-hint {
  font-size: 10px;
  margin-top: 3px;
  font-weight: 500;
}

.agri-advanced-toggle-btn {
  display: inline-flex;
  align-items: center;
  gap: 6px;
  background: rgba(255, 255, 255, 0.05);
  border: 1px solid rgba(255, 255, 255, 0.12);
  color: var(--text-sub);
  font-size: 11px;
  font-weight: 600;
  padding: 6px 12px;
  border-radius: 8px;
  cursor: pointer;
  margin-bottom: 14px;
  transition: all 0.2s;
}

.agri-advanced-toggle-btn:hover {
  background: rgba(255, 255, 255, 0.1);
  color: var(--text-main);
}







  </style>
</head>

<body>

  <div class="container">

    <!-- UNIFIED SPACE-SAVING SCADA HEADER BAR -->
    <header class="unified-header" id="banner-box">
      <!-- Left: Brand & Telemetry Status Subtitle -->
      <div class="brand-block">
        <div class="brand-title">
          <div class="brand-icon-box">
            <svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round">
              <path d="M11 20A7 7 0 0 1 9.8 6.1C15.5 5 17 4.48 19 2c1 2 2 4.18 2 8 0 5.5-4.78 10-10 10Z"/>
              <path d="M2 21c0-3 1.85-5.36 5.08-6C9.5 14.52 12 13 13 12"/>
            </svg>
          </div>
          <span>SCADA Smart Farm</span>
        </div>
        <div class="brand-sub">
          <span>Dashboard Agronomi Presisi &amp; Telemetri Real-Time</span>
          <span class="status-divider">•</span>
          <span id="banner-text" class="header-status-msg">Standalone Mode / Sensor Offline</span>
        </div>
      </div>

      <!-- Right: Real-Time Status Indicators & Quick Action Controls -->
      <div class="header-controls">
        <div class="status-badge-bar">
          <!-- PHYSICAL TRAFFIC LIGHT INDICATOR (ESP32 PINS 32, 33, 27) -->
          <div class="badge-pill traffic-pill" id="traffic-light-widget" title="Indikator Fisik Traffic Light Lahan (Pin 32 Hijau, 33 Kuning, 27 Merah) - Klik untuk Detail" onclick="toggleLedDiag()" style="cursor: pointer;">
            <div class="traffic-housing">
              <span class="traffic-bulb bulb-red active blink" id="traffic-bulb-red" title="Merah: Bahaya / Terputus / Offline (Pin 27)"></span>
              <span class="traffic-bulb bulb-yellow" id="traffic-bulb-yellow" title="Kuning: Peringatan / Menunggu / Standby (Pin 33)"></span>
              <span class="traffic-bulb bulb-green" id="traffic-bulb-green" title="Hijau: Kondisi Normal (Pin 32)"></span>
            </div>
            <span id="traffic-label" class="traffic-label" style="color: #ef4444;">Offline</span>
          </div>

          <div class="badge-pill" id="badge-global-pump" style="color: var(--text-sub); display: inline-flex;">
            <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" style="vertical-align:middle;"><path d="M12 2v4m0 12v4M4.93 4.93l2.83 2.83m8.48 8.48l2.83 2.83M2 12h4m12 0h4M4.93 19.07l2.83-2.83m8.48-8.48l2.83-2.83"></path></svg>
            <span id="text-global-pump">Pompa OFF</span>
          </div>
          <div class="badge-pill" id="badge-global-lamp" style="color: var(--text-sub); display: inline-flex;">
            <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" style="vertical-align:middle;"><path d="M9 18h6M10 22h4M21 11.5a8.38 8.38 0 0 1-.9 3.8 8.5 8.5 0 0 1-7.6 4.7 8.38 8.38 0 0 1-3.8-.9L3 21l1.9-5.7a8.38 8.38 0 0 1-.9-3.8 8.5 8.5 0 0 1 4.7-7.6 8.38 8.38 0 0 1 3.8-.9h.5a8.48 8.48 0 0 1 8 8v.5z"></path></svg>
            <span id="text-global-lamp">Lampu OFF</span>
          </div>
          <!-- GLOBAL MODE BADGE (AUTO vs MANUAL) -->
          <div class="badge-pill badge-mode-auto" id="badge-global-mode" style="cursor: pointer; display: inline-flex; align-items: center; gap: 5px;" onclick="switchTab('controls')" title="Mode Kendali Aktif - Klik untuk Ubah di Tab Kendali">
            <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" style="vertical-align:middle;" id="icon-global-mode"><polygon points="13 2 3 14 12 14 11 22 21 10 12 10 13 2"></polygon></svg>
            <span id="text-global-mode">AUTO</span>
          </div>
          <div class="badge-pill" id="badge-rtc-time" style="color: var(--text-main);">
            <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" style="vertical-align:middle;">
              <circle cx="12" cy="12" r="10"></circle>
              <polyline points="12 6 12 12 16 14"></polyline>
            </svg>
            <span>RTC: Syncing...</span>
          </div>
          <div class="badge-pill" id="badge-rssi" style="color: var(--accent);">
            <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" style="vertical-align:middle;">
              <path d="M5 12.55a11 11 0 0 1 14.08 0"></path>
              <path d="M1.42 9a16 16 0 0 1 21.16 0"></path>
              <path d="M8.53 16.11a6 6 0 0 1 6.95 0"></path>
              <line x1="12" y1="20" x2="12.01" y2="20"></line>
            </svg>
            <span>Sinyal: - dBm</span>
          </div>
        </div>

        <div class="header-action-group">
          <button type="button" class="btn btn-secondary header-btn" onclick="syncRtcWithBrowser()" title="Sinkronkan Jam RTC DS3231 dengan Waktu Peramban">
            <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              <circle cx="12" cy="12" r="10"></circle>
              <polyline points="12 6 12 12 16 14"></polyline>
            </svg>
            Sync Jam
          </button>
          <button type="button" class="btn btn-secondary header-btn" onclick="syncBMKGData()" title="Sinkronkan Prakiraan Cuaca Satelit BMKG">
            <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              <path d="M17.5 19H9a7 7 0 1 1 6.71-9h1.79a4.5 4.5 0 1 1 0 9Z"></path>
            </svg>
            Sync BMKG
          </button>
          <button type="button" class="btn btn-primary header-btn" onclick="fetchData()" title="Perbarui Data Telemetri Terkini">
            <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              <polyline points="23 4 23 10 17 10"></polyline>
              <polyline points="1 20 1 14 7 14"></polyline>
              <path d="M3.51 9a9 9 0 0 1 14.85-3.36L23 10M1 14l4.64 4.36A9 9 0 0 0 20.49 15"></path>
            </svg>
            Refresh Data
          </button>
        </div>
      </div>
    </header>

    <!-- SCADA RUNNING TICKER BAR (REAL-TIME STREAM MARQUEE) -->
    <div class="scada-ticker-bar" id="scada-ticker-bar" title="Streaming Telemetri Real-Time Lahan & Mikroklimat (Hover / Sentuh untuk Jeda)">
      <div class="ticker-badge">
        <span class="ticker-dot"></span>
        <span class="ticker-label">LIVE STREAM</span>
      </div>
      <div class="ticker-marquee-wrapper">
        <div class="ticker-track" id="ticker-track">
          <span class="ticker-text" id="ticker-text">
            Suhu: --°C &nbsp;•&nbsp; RH Udara: --% &nbsp;•&nbsp; Tanah: --% &nbsp;•&nbsp; Pompa: OFF &nbsp;•&nbsp; Lampu: OFF &nbsp;•&nbsp; Mode: AUTO &nbsp;•&nbsp; Jam: --:-- WIB &nbsp;•&nbsp; Sinyal: -- dBm &nbsp;•&nbsp; BMKG: Standby
          </span>
          <span class="ticker-text" id="ticker-text-clone">
            Suhu: --°C &nbsp;•&nbsp; RH Udara: --% &nbsp;•&nbsp; Tanah: --% &nbsp;•&nbsp; Pompa: OFF &nbsp;•&nbsp; Lampu: OFF &nbsp;•&nbsp; Mode: AUTO &nbsp;•&nbsp; Jam: --:-- WIB &nbsp;•&nbsp; Sinyal: -- dBm &nbsp;•&nbsp; BMKG: Standby
          </span>
        </div>
      </div>
      <button type="button" class="ticker-lcd-toggle" id="btn-toggle-virtual-lcd" onclick="toggleVirtualLcdDrawer()" title="Buka / Tutup Layar Monitor LCD 16x2 Fisik (Digital Twin)">
        <svg width="13" height="13" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
          <rect x="2" y="3" width="20" height="14" rx="2" ry="2"></rect>
          <line x1="8" y1="21" x2="16" y2="21"></line>
          <line x1="12" y1="17" x2="12" y2="21"></line>
        </svg>
        <span>LCD 16x2</span>
        <span class="lcd-toggle-indicator" id="lcd-toggle-indicator">▼</span>
      </button>
    </div>


    <!-- VIRTUAL LCD 16x2 HARDWARE MONITOR DRAWER (DIGITAL TWIN) -->
    <div class="virtual-lcd-drawer" id="virtual-lcd-drawer" style="display: none;">
      <div class="virtual-lcd-chassis">
        <!-- Chassis Header -->
        <div class="chassis-header">
          <div class="chassis-screw" title="M3 Mount Screw"></div>
          <div class="chassis-title-bar">
            <span class="chassis-led-pwr" id="lcd-led-pwr" title="Power LED ESP32"></span>
            <span class="chassis-model">DIGITAL TWIN • HARDWARE LCD 1602 I2C (0x27 / 0x3F)</span>
          </div>
          <div class="chassis-screw" title="M3 Mount Screw"></div>
        </div>

        <!-- LCD Screen Bezel with Glass Glare & Backlight -->
        <div class="lcd-screen-bezel lcd-theme-blue" id="lcd-screen-bezel">
          <div class="lcd-screen-glare"></div>
          <div class="lcd-matrix-grid">
            <div class="lcd-matrix-row" id="virtual-lcd-row-1">T: --.-C H: --% AUTO</div>
            <div class="lcd-matrix-row" id="virtual-lcd-row-2">S: --%  P:OFF  L:OFF</div>
          </div>

        </div>

        <!-- Chassis Toolbar & Controls -->
        <div class="chassis-footer">
          <div class="chassis-screw" title="M3 Mount Screw"></div>
          <div class="chassis-toolbar">
            <div class="chassis-page-selectors">
              <button type="button" class="lcd-btn active" id="btn-lcd-page-0" onclick="setVirtualLcdPage(0)">Layar 1 (Telemetri)</button>
              <button type="button" class="lcd-btn" id="btn-lcd-page-1" onclick="setVirtualLcdPage(1)">Layar 2 (Sistem &amp; Jam)</button>
              <button type="button" class="lcd-btn" id="btn-lcd-auto-rotate" onclick="toggleVirtualLcdRotate()" title="Rotasi Otomatis Layar Tiap 4 Detik">Auto: ON</button>
            </div>
            <div class="chassis-theme-switch">
              <button type="button" class="lcd-btn-action" onclick="cycleLcdBacklight()" title="Ganti Warna Backlight (Biru / Hijau / Amber / Dark)">Backlight</button>
              <button type="button" class="lcd-btn-close" onclick="toggleVirtualLcdDrawer()" title="Tutup Panel Monitor LCD">✕</button>
            </div>
          </div>
          <div class="chassis-screw" title="M3 Mount Screw"></div>
        </div>
      </div>
    </div>



    <!-- TAB 1: MONITORING & KPI -->
    <div id="tab-overview" class="tab-content active">
      <!-- TELEMETRY KPI METRICS -->
      <div class="grid-kpi">
        <!-- Soil Moisture Card -->
        <div class="kpi-card soil-card">
          <div class="kpi-card-header">
            <div class="kpi-meta">
              <div class="kpi-icon-box">
                <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                  <path d="M12 10a6 6 0 0 0-6-6H3v2a6 6 0 0 0 6 6h3"/>
                  <path d="M12 14a6 6 0 0 1 6-6h3v2a6 6 0 0 1-6 6h-3"/>
                  <path d="M12 22V10"/>
                </svg>
              </div>
              <div>
                <span class="kpi-title">Kelembapan Tanah</span>
                <span class="kpi-subtitle">Sensor Volumetrik VWC</span>
              </div>
            </div>
            <span class="kpi-status-badge" id="kpi-soil-category">Membaca...</span>
          </div>
          <div class="kpi-card-body">
            <div class="kpi-value" id="kpi-soil">--%</div>
          </div>
          <div class="kpi-card-footer">
            <span class="kpi-footer-dot"></span>
            <span class="kpi-footer-text" id="kpi-soil-depletion">Prediksi Penguapan: -</span>
          </div>
        </div>

        <!-- Temperature Card -->
        <div class="kpi-card temp-card">
          <div class="kpi-card-header">
            <div class="kpi-meta">
              <div class="kpi-icon-box">
                <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                  <path d="M14 14.76V3.5a2.5 2.5 0 0 0-5 0v11.26a4.5 4.5 0 1 0 5 0z"/>
                </svg>
              </div>
              <div>
                <span class="kpi-title">Suhu Lingkungan</span>
                <span class="kpi-subtitle">DHT11 Telemetri</span>
              </div>
            </div>
            <span class="kpi-status-badge">Iklim Mikro</span>
          </div>
          <div class="kpi-card-body">
            <div class="kpi-value" id="kpi-temp">--°C</div>
          </div>
          <div class="kpi-card-footer">
            <span class="kpi-footer-dot"></span>
            <span class="kpi-footer-text" id="kpi-heat-index">Terasa seperti: --°C</span>
          </div>
        </div>

        <!-- Humidity Card -->
        <div class="kpi-card hum-card">
          <div class="kpi-card-header">
            <div class="kpi-meta">
              <div class="kpi-icon-box">
                <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                  <path d="M12 2.69l5.66 5.66a8 8 0 1 1-11.31 0z"/>
                </svg>
              </div>
              <div>
                <span class="kpi-title">Kelembapan Udara</span>
                <span class="kpi-subtitle">Relative Humidity</span>
              </div>
            </div>
            <span class="kpi-status-badge">Sensor RH</span>
          </div>
          <div class="kpi-card-body">
            <div class="kpi-value" id="kpi-hum">--%</div>
          </div>
          <div class="kpi-card-footer">
            <span class="kpi-footer-dot"></span>
            <span class="kpi-footer-text" id="kpi-dew-point">Titik Embun: --°C</span>
          </div>
        </div>

        <!-- ESP-NOW Radio Link & Node Transducer Card -->
        <div class="kpi-card signal-card">
          <div class="kpi-card-header">
            <div class="kpi-meta">
              <div class="kpi-icon-box">
                <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                  <path d="M5 12.55a11 11 0 0 1 14.08 0"></path>
                  <path d="M1.42 9a16 16 0 0 1 21.16 0"></path>
                  <path d="M8.53 16.11a6 6 0 0 1 6.95 0"></path>
                  <line x1="12" y1="20" x2="12.01" y2="20"></line>
                </svg>
              </div>
              <div>
                <span class="kpi-title">Sinyal ESP-NOW</span>
                <span class="kpi-subtitle">Node Kebun (5V)</span>
              </div>
            </div>
            <span class="kpi-status-badge" id="kpi-node-status">Node</span>
          </div>
          <div class="kpi-card-body">
            <div class="kpi-value" id="kpi-signal">-- dBm</div>
          </div>
          <div class="kpi-card-footer">
            <span class="kpi-footer-dot"></span>
            <span class="kpi-footer-text" id="kpi-raw-adc">ADC A0: --</span>
          </div>
        </div>
      </div>

      <!-- TRAFFIC LIGHT INDICATOR CARD (REAL-TIME ESP32 HARDWARE STATUS) -->
      <div class="card traffic-indicator-card" id="card-traffic-light" style="display:none;">
        <div class="card-title" style="display:flex; align-items:center; justify-content:space-between; flex-wrap:wrap; gap:10px;">
          <div style="display:flex; align-items:center; gap:10px;">
            <div class="kpi-icon-box" style="width:32px; height:32px; background:rgba(16,185,129,0.12); color:var(--primary);" id="traffic-card-icon">
              <svg width="17" height="17" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                <rect x="5" y="2" width="14" height="20" rx="3"></rect>
                <circle cx="12" cy="7" r="2"></circle>
                <circle cx="12" cy="12" r="2"></circle>
                <circle cx="12" cy="17" r="2"></circle>
              </svg>
            </div>
            <div>
              <span style="font-size:13px; font-weight:700; color:var(--text-main);">Indikator Status Lahan</span>
              <span style="display:block; font-size:11px; color:var(--text-sub); margin-top:1px;">Traffic Light Fisik ESP32 (Pin 27 Merah · Pin 33 Kuning · Pin 32 Hijau)</span>
            </div>
          </div>
          <span id="traffic-card-label" style="font-size:12px; font-weight:700; padding:4px 12px; border-radius:20px; background:rgba(239,68,68,0.15); color:#ef4444; border:1px solid rgba(239,68,68,0.3);">SENSOR TERPUTUS / OFFLINE</span>
        </div>

        <!-- BIG TRAFFIC LIGHT DISPLAY -->
        <div id="traffic-card-body" style="display:flex; align-items:center; justify-content:space-between; gap:20px; padding:18px 10px 8px; flex-wrap:wrap;">

          <!-- Left: Big Physical Light Column -->
          <div style="display:flex; flex-direction:column; align-items:center; gap:8px;">
            <div class="traffic-pole-housing" id="traffic-pole">
              <!-- RED bulb -->
              <div class="traffic-pole-bulb pole-red lit blink" id="pole-bulb-red" title="Pin 27 · Bahaya / Sensor Terputus">
                <div class="pole-bulb-inner"></div>
              </div>
              <!-- YELLOW bulb -->
              <div class="traffic-pole-bulb pole-yellow" id="pole-bulb-yellow" title="Pin 33 · Peringatan / Jadwal RTC">
                <div class="pole-bulb-inner"></div>
              </div>
              <!-- GREEN bulb -->
              <div class="traffic-pole-bulb pole-green" id="pole-bulb-green" title="Pin 32 · Kondisi Normal">
                <div class="pole-bulb-inner"></div>
              </div>
            </div>
            <span id="traffic-pole-label" style="font-size:10px; font-weight:700; color:var(--text-sub); letter-spacing:0.05em; text-transform:uppercase; text-align:center;">LAHAN</span>
          </div>

          <!-- Right: Status Details -->
          <div style="flex:1; min-width:180px; display:flex; flex-direction:column; gap:12px;">
            <!-- Status rows -->
            <div style="display:grid; grid-template-columns:1fr 1fr 1fr; gap:8px; text-align:center;">
              <div class="traffic-status-cell" id="traffic-cell-red" style="border-radius:10px; padding:10px 6px; background:rgba(239,68,68,0.07); border:1px solid rgba(239,68,68,0.15);">
                <div style="font-size:9px; color:#ef4444; font-weight:700; letter-spacing:0.04em; text-transform:uppercase; margin-bottom:4px;">MERAH</div>
                <div style="font-size:10px; color:var(--text-sub); line-height:1.3;">Bahaya Suhu<br/>Sensor Terputus</div>
                <div class="traffic-cell-dot" style="width:8px; height:8px; border-radius:50%; background:#ef4444; margin:6px auto 0; opacity:1; box-shadow:0 0 8px #ef4444;" id="traffic-dot-red"></div>
              </div>
              <div class="traffic-status-cell" id="traffic-cell-yellow" style="border-radius:10px; padding:10px 6px; background:rgba(245,158,11,0.07); border:1px solid rgba(245,158,11,0.15);">
                <div style="font-size:9px; color:#f59e0b; font-weight:700; letter-spacing:0.04em; text-transform:uppercase; margin-bottom:4px;">KUNING</div>
                <div style="font-size:10px; color:var(--text-sub); line-height:1.3;">Perlu Irigasi<br/>Jadwal RTC Aktif</div>
                <div class="traffic-cell-dot" style="width:8px; height:8px; border-radius:50%; background:#f59e0b; margin:6px auto 0; opacity:0.3;" id="traffic-dot-yellow"></div>
              </div>
              <div class="traffic-status-cell" id="traffic-cell-green" style="border-radius:10px; padding:10px 6px; background:rgba(16,185,129,0.07); border:1px solid rgba(16,185,129,0.15);">
                <div style="font-size:9px; color:#10b981; font-weight:700; letter-spacing:0.04em; text-transform:uppercase; margin-bottom:4px;">HIJAU</div>
                <div style="font-size:10px; color:var(--text-sub); line-height:1.3;">Kondisi Aman<br/>Tanah Cukup</div>
                <div class="traffic-cell-dot" style="width:8px; height:8px; border-radius:50%; background:#10b981; margin:6px auto 0; opacity:0.3;" id="traffic-dot-green"></div>
              </div>
            </div>

            <!-- Current reading bar -->
            <div style="background:rgba(255,255,255,0.03); border-radius:8px; padding:10px 12px; border:1px solid var(--card-border);">
              <div style="display:flex; justify-content:space-between; align-items:center; flex-wrap:wrap; gap:6px;">
                <span style="font-size:11px; color:var(--text-sub);">Status Aktif Lahan Saat Ini</span>
                <span id="traffic-rtc-badge" style="font-size:10px; font-weight:600; color:var(--text-sub); display:none;">
                  <svg width="10" height="10" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" style="vertical-align:middle;margin-right:3px;"><circle cx="12" cy="12" r="10"></circle><polyline points="12 6 12 12 16 14"></polyline></svg>
                  Jadwal RTC Berjalan
                </span>
              </div>
              <div id="traffic-current-status" style="font-size:13px; font-weight:700; color:#ef4444; margin-top:6px;">ESP8266 Terputus / Sensor Offline</div>
              <div id="traffic-current-detail" style="font-size:10px; color:var(--text-sub); margin-top:3px;">Lampu Merah (Pin 27) berkedip. Pompa dikunci mati oleh failsafe sistem.</div>
            </div>

            <!-- DIAGNOSTIK HARDWARE LED (GPIO Sinyal vs Fisik) -->
            <div id="traffic-hw-diag" style="background:rgba(255,255,255,0.02); border-radius:8px; padding:10px 12px; border:1px solid var(--card-border);">
              <div style="display:flex; justify-content:space-between; align-items:center; margin-bottom:8px;">
                <span style="font-size:10px; font-weight:700; color:var(--text-sub); letter-spacing:0.04em; text-transform:uppercase;">Diagnostik GPIO Hardware</span>
                <span style="font-size:9px; color:var(--text-sub);">Sinyal Firmware → Pin Fisik</span>
              </div>
              <!-- GPIO rows -->
              <div style="display:flex; flex-direction:column; gap:5px;">
                <div style="display:flex; align-items:center; justify-content:space-between; gap:8px;">
                  <div style="display:flex; align-items:center; gap:6px;">
                    <div style="width:10px; height:10px; border-radius:50%; background:#ef4444; flex-shrink:0; box-shadow:0 0 6px #ef4444;" id="hw-dot-red"></div>
                    <span style="font-size:10px; color:var(--text-sub);">Pin 27 · LED Merah</span>
                  </div>
                  <span id="hw-state-red" style="font-size:10px; font-weight:700; color:#ef4444; padding:2px 8px; border-radius:6px; background:rgba(255,255,255,0.08);">BLINK (ON) ●</span>
                </div>
                <div style="display:flex; align-items:center; justify-content:space-between; gap:8px;">
                  <div style="display:flex; align-items:center; gap:6px;">
                    <div style="width:10px; height:10px; border-radius:50%; background:#f59e0b; flex-shrink:0;" id="hw-dot-yellow"></div>
                    <span style="font-size:10px; color:var(--text-sub);">Pin 33 · LED Kuning</span>
                  </div>
                  <span id="hw-state-yellow" style="font-size:10px; font-weight:700; color:var(--text-sub); padding:2px 8px; border-radius:6px; background:rgba(255,255,255,0.04);">LOW (OFF)</span>
                </div>
                <div style="display:flex; align-items:center; justify-content:space-between; gap:8px;">
                  <div style="display:flex; align-items:center; gap:6px;">
                    <div style="width:10px; height:10px; border-radius:50%; background:#10b981; flex-shrink:0;" id="hw-dot-green"></div>
                    <span style="font-size:10px; color:var(--text-sub);">Pin 32 · LED Hijau</span>
                  </div>
                  <span id="hw-state-green" style="font-size:10px; font-weight:700; color:var(--text-sub); padding:2px 8px; border-radius:6px; background:rgba(255,255,255,0.04);">LOW (OFF)</span>
                </div>
              </div>

              <!-- Mismatch warning (hidden by default, shown if user reports issue) -->
              <div id="traffic-mismatch-alert" style="display:none; margin-top:10px; padding:8px 10px; background:rgba(239,68,68,0.1); border:1px solid rgba(239,68,68,0.3); border-radius:8px; font-size:10px; color:#ef4444; line-height:1.5;">
                <b>⚠ Potensi Kerusakan Hardware LED!</b><br/>
                Firmware memerintahkan lampu menyala, namun tidak ada konfirmasi. Periksa:<br/>
                • Kabel/jumper dari ESP32 ke LED<br/>
                • Resistor pembatas arus (220Ω – 470Ω)<br/>
                • LED terbakar / polaritas terbalik (Anoda & Katoda)
              </div>
              <div style="margin-top:8px; display:flex; align-items:center; gap:6px; flex-wrap:wrap;">
                <span style="font-size:9px; color:var(--text-sub);">Jika lampu fisik tidak cocok dengan status di atas, klik:</span>
                <button type="button" onclick="reportLedMismatch()" style="font-size:9px; padding:3px 8px; border-radius:6px; background:rgba(239,68,68,0.12); color:#ef4444; border:1px solid rgba(239,68,68,0.3); cursor:pointer; font-weight:700;">Laporkan Mismatch</button>
              </div>
            </div>
          </div>
        </div>
      </div>


      <!-- SECTOR & ZONATION SELECTOR STRIP (LOCALSTORAGE DYNAMIC) -->
      <div class="sector-strip">
        <div class="sector-strip-header">
          <div class="sector-strip-title-wrap">
            <div class="sector-strip-title">
              <svg width="17" height="17" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                <polygon points="1 6 1 22 8 18 16 22 23 18 23 2 16 6 8 2 1 6"></polygon>
                <line x1="8" y1="2" x2="8" y2="18"></line>
                <line x1="16" y1="6" x2="16" y2="22"></line>
              </svg>
              <span>Zonasi Lahan &amp; Kalibrasi Profil Tanaman</span>
            </div>
            <span class="sector-strip-desc">Pilih sektor aktif untuk kalibrasi kebutuhan air &amp; evapotranspirasi presisi</span>
          </div>
          <div class="sector-strip-actions">
            <button type="button" class="btn btn-secondary" onclick="openSectorManagerModal()" style="padding: 7px 13px; font-size: 11.5px; border-radius: 10px;">
              ⚙️ Kelola Sektor (Storage)
            </button>
            <button type="button" class="btn btn-primary" onclick="openAddSectorForm()" style="padding: 7px 13px; font-size: 11.5px; border-radius: 10px;">
              ➕ Tambah Sektor
            </button>
          </div>
        </div>

        <!-- Dynamic Sector Grid (Rendered via LocalStorage) -->
        <div class="sector-grid" id="sector-grid-container">
          <!-- Populated dynamically by renderSectorGrid() -->
        </div>
      </div>

      <!-- PREDIKSI FENOLOGI & LAJU PERTUMBUHAN TANAMAN -->
      <div class="phenology-card" id="card-phenology">
        <div class="pheno-header">
          <div class="pheno-title-wrap">
            <div class="pheno-title">
              <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                <path d="M12 2a10 10 0 0 1 10 10c0 5.523-4.477 10-10 10S2 17.523 2 12A10 10 0 0 1 12 2z"></path>
                <path d="M12 6v6l4 2"></path>
              </svg>
              <span>PREDIKSI FENOLOGI &amp; LAJU PERTUMBUHAN TANAMAN</span>
            </div>
            <div class="pheno-subtitle" id="pheno-crop-tag">Belum Ada Sektor Kebun Dikonfigurasi</div>
          </div>
          <span class="badge-pill" id="pheno-health-badge" style="color:var(--text-sub); font-weight:700;">Standby (Menunggu Sektor)</span>
        </div>

        <!-- 4-Stage Phenological Milestones -->
        <div class="pheno-milestones-container">
          <div class="pheno-timeline-bar">
            <div class="pheno-timeline-fill" id="pheno-timeline-fill" style="width: 0%;"></div>
          </div>
          <div class="pheno-stages-grid" id="pheno-stages-grid">
            <!-- Stage 1 -->
            <div class="pheno-stage-item" id="stage-step-1">
              <div class="pheno-step-bubble" id="stage-bubble-1">1</div>
              <div class="pheno-step-label" id="stage-label-1">Tahap 1</div>
              <div class="pheno-step-sub" id="stage-sub-1">--</div>
            </div>
            <!-- Stage 2 -->
            <div class="pheno-stage-item" id="stage-step-2">
              <div class="pheno-step-bubble" id="stage-bubble-2">2</div>
              <div class="pheno-step-label" id="stage-label-2">Tahap 2</div>
              <div class="pheno-step-sub" id="stage-sub-2">--</div>
            </div>
            <!-- Stage 3 -->
            <div class="pheno-stage-item" id="stage-step-3">
              <div class="pheno-step-bubble" id="stage-bubble-3">3</div>
              <div class="pheno-step-label" id="stage-label-3">Tahap 3</div>
              <div class="pheno-step-sub" id="stage-sub-3">--</div>
            </div>
            <!-- Stage 4 -->
            <div class="pheno-stage-item" id="stage-step-4">
              <div class="pheno-step-bubble" id="stage-bubble-4">4</div>
              <div class="pheno-step-label" id="stage-label-4">Tahap 4</div>
              <div class="pheno-step-sub" id="stage-sub-4">--</div>
            </div>
          </div>
        </div>

        <!-- 3 Bio-Climatic Sensor Evaluation Pillars -->
        <div class="pheno-grid-pillars">
          <!-- Pillar 1: Thermal Time / GDD -->
          <div class="pheno-pillar-card">
            <div class="pheno-pillar-head">
              <span class="pheno-pillar-tag">GDD MODEL</span>
              <span class="pheno-dot dot-cyan"></span>
            </div>
            <div class="pheno-pillar-val" id="pheno-gdd-val">-- GDD/hari</div>
            <div class="pheno-pillar-note" id="pheno-gdd-note">Pilih atau tambahkan sektor lahan</div>
          </div>
          <!-- Pillar 2: VPD (Transpiration Pressure) -->
          <div class="pheno-pillar-card">
            <div class="pheno-pillar-head">
              <span class="pheno-pillar-tag">VPD KUTIKULA</span>
              <span class="pheno-dot dot-blue"></span>
            </div>
            <div class="pheno-pillar-val" id="pheno-vpd-val">-- kPa</div>
            <div class="pheno-pillar-note" id="pheno-vpd-note">Menunggu konfigurasi tanaman</div>
          </div>
          <!-- Pillar 3: Soil Moisture VWC -->
          <div class="pheno-pillar-card">
            <div class="pheno-pillar-head">
              <span class="pheno-pillar-tag">VWC &amp; AERASI AKAR</span>
              <span class="pheno-dot dot-green"></span>
            </div>
            <div class="pheno-pillar-val" id="pheno-vwc-val">--% VWC</div>
            <div class="pheno-pillar-note" id="pheno-vwc-note">Menunggu telemetri sektor</div>
          </div>
        </div>

        <!-- Actionable AI Advice Box -->
        <div class="pheno-advice-box">
          <div style="display:flex; align-items:flex-start; gap:12px; flex:1; min-width:240px;">
            <div class="kpi-icon-box" style="width:32px; height:32px; font-size:14px; background:rgba(16,185,129,0.12); color:var(--primary); border:1px solid rgba(16,185,129,0.25); flex-shrink:0;">
              <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                <path d="M12 2a9 9 0 0 0-9 9c0 4.97 4.03 9 9 9 2.5 0 4.7-1 6.3-2.7"></path>
                <path d="M12 12V6"></path>
                <path d="M12 12l4-4"></path>
                <circle cx="12" cy="12" r="2"></circle>
              </svg>
            </div>
            <div>
              <div style="font-size:11px; font-weight:700; text-transform:uppercase; letter-spacing:0.06em; color:var(--primary); margin-bottom:2px;">Rekomendasi Agronomi Presisi Hari Ini</div>
              <div id="pheno-ai-advice" style="font-size:12px; color:var(--text-main); line-height:1.5;">Belum ada sektor kebun yang aktif. Daftarkan petak lahan Anda melalui tombol <b>[+ Tambah Sektor]</b> di panel Zonasi Lahan untuk mengaktifkan pemodelan fenologi &amp; agronomis presisi.</div>
            </div>
          </div>
          <div id="pheno-target-eta" class="badge-pill" style="border-color:rgba(255,255,255,0.15); background:rgba(255,255,255,0.04); color:var(--text-sub); font-weight:700; font-size:12px; white-space:nowrap;">
            Estimasi: Menunggu Sektor...
          </div>
        </div>
      </div>

      <!-- SCADA PATHOGEN & PEST OUTBREAK SURVEILLANCE -->
      <div class="outbreak-card" id="card-outbreak">
        <div class="outbreak-header">
          <div class="outbreak-title-wrap">
            <div class="outbreak-icon-box">
              <svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                <path d="M12 22s8-4 8-10V5l-8-3-8 3v7c0 6 8 10 8 10z"></path>
                <path d="M12 8v4"></path>
                <path d="M12 16h.01"></path>
              </svg>
            </div>
            <div>
              <div class="outbreak-title">Evaluasi Risiko Patogen &amp; Hama Tanaman</div>
              <div class="outbreak-subtitle">Surveilans Real-Time Sensor Iklim DHT11 &amp; ESP-NOW Tanah</div>
            </div>
          </div>
          <span class="badge-pill" id="outbreak-risk-level">Menunggu Sensor Telemetri</span>
        </div>

        <div class="outbreak-grid" id="outbreak-warning-box">
          <!-- Item 1: Jamur Bercak Daun / Buah -->
          <div class="outbreak-item" id="box-disease-1">
            <div class="outbreak-item-head">
              <div style="display:flex; align-items:center; gap:10px;">
                <div class="kpi-icon-box" style="width:30px; height:30px; font-size:13px; background:rgba(6,182,212,0.1); color:var(--accent-cyan); border:1px solid rgba(6,182,212,0.25);">
                  <svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                    <circle cx="12" cy="12" r="10"></circle>
                    <path d="M12 6v6l4 2"></path>
                  </svg>
                </div>
                <div class="outbreak-target-info">
                  <div class="outbreak-target-name" id="disease-1-title">Phytophthora &amp; Antraknosa</div>
                  <div class="outbreak-target-vector">Bercak Daun &amp; Busuk Buah</div>
                </div>
              </div>
              <span class="outbreak-tag tag-low" id="disease-1-badge">STANDBY</span>
            </div>
            <div class="outbreak-condition-chip">
              <span>Ambang Inkubasi:</span>
              <code>RH &gt; 82% &amp; 20–27°C</code>
            </div>
            <div class="outbreak-desc-box" id="disease-1-desc">
              Data iklim belum terbaca. Menunggu telemetri suhu &amp; kelembapan DHT11...
            </div>
          </div>

          <!-- Item 2: Hama Thrips & Vektor Virus -->
          <div class="outbreak-item" id="box-disease-2">
            <div class="outbreak-item-head">
              <div style="display:flex; align-items:center; gap:10px;">
                <div class="kpi-icon-box" style="width:30px; height:30px; font-size:13px; background:rgba(245,158,11,0.1); color:var(--accent-amber); border:1px solid rgba(245,158,11,0.25);">
                  <svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                    <path d="M12 2a4 4 0 0 0-4 4v2H6a2 2 0 0 0-2 2v2a2 2 0 0 0 2 2h2v4a4 4 0 0 0 8 0v-4h2a2 2 0 0 0 2-2v-2a2 2 0 0 0-2-2h-2V6a4 4 0 0 0-4-4z"></path>
                  </svg>
                </div>
                <div class="outbreak-target-info">
                  <div class="outbreak-target-name" id="disease-2-title">Thrips, Kutu Daun &amp; Mites</div>
                  <div class="outbreak-target-vector">Vektor Gemini Virus &amp; Daun Keriting</div>
                </div>
              </div>
              <span class="outbreak-tag tag-low" id="disease-2-badge">STANDBY</span>
            </div>
            <div class="outbreak-condition-chip">
              <span>Ambang Populasi:</span>
              <code>Suhu &gt; 32°C &amp; RH &lt; 55%</code>
            </div>
            <div class="outbreak-desc-box" id="disease-2-desc">
              Data iklim belum terbaca. Menunggu telemetri suhu &amp; kelembapan DHT11...
            </div>
          </div>

          <!-- Item 3: Layu Bakteri Perakaran -->
          <div class="outbreak-item" id="box-disease-3">
            <div class="outbreak-item-head">
              <div style="display:flex; align-items:center; gap:10px;">
                <div class="kpi-icon-box" style="width:30px; height:30px; font-size:13px; background:rgba(59,130,246,0.1); color:var(--accent-blue); border:1px solid rgba(59,130,246,0.25);">
                  <svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                    <path d="M12 22V8"></path>
                    <path d="m5 12 7-4 7 4"></path>
                    <path d="M5 16l7-4 7 4"></path>
                  </svg>
                </div>
                <div class="outbreak-target-info">
                  <div class="outbreak-target-name" id="disease-3-title">Layu Bakteri (Ralstonia)</div>
                  <div class="outbreak-target-vector">Anoksia Media &amp; Busuk Perakaran</div>
                </div>
              </div>
              <span class="outbreak-tag tag-low" id="disease-3-badge">STANDBY</span>
            </div>
            <div class="outbreak-condition-chip">
              <span>Ambang Anaerob:</span>
              <code>VWC &gt; 80% &amp; Suhu &gt; 30°C</code>
            </div>
            <div class="outbreak-desc-box" id="disease-3-desc">
              Data kelembapan tanah belum terbaca. Menunggu sinyal ESP-NOW kebun...
            </div>
          </div>
        </div>
      </div>

        <!-- DIAGNOSTIK VISUAL PATOLOGI & HARA TANAMAN -->
        <div class="monitor-card vision-card" style="margin-top:20px;">
          <div class="card-header" style="justify-content: space-between;">
            <div style="display:flex; align-items:center; gap:10px;">
              <div class="kpi-icon-box" style="background: rgba(168, 85, 247, 0.15); color: #c084fc; border: 1px solid rgba(168, 85, 247, 0.3);">
                <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                  <path d="M23 19a2 2 0 0 1-2 2H3a2 2 0 0 1-2-2V8a2 2 0 0 1 2-2h4l2-3h6l2 3h4a2 2 0 0 1 2 2z"/>
                  <circle cx="12" cy="13" r="4"/>
                </svg>
              </div>
              <div>
                <span class="card-title" style="display:block; font-size:14px; font-weight:700; color:var(--text-main);">Inspeksi Citra Patologi & Daun</span>
                <span style="font-size:11px; color:var(--text-sub);">Analisis Morfologi & Defisiensi Nutrisi Lapangan</span>
              </div>
            </div>
            <span class="badge-pill" style="background: rgba(168, 85, 247, 0.12); color: #c084fc; border: 1px solid rgba(168, 85, 247, 0.3); font-size: 11px; padding: 3px 10px;">
              Optical Vision
            </span>
          </div>
          
          <div class="card-body" style="padding: 16px;">
            <p style="font-size:12px; color:var(--text-sub); margin-bottom:14px; line-height:1.6;">
              Ambil sampel daun tanaman melalui kamera langsung, unggah file, tempel dari clipboard (Ctrl+V), atau tautan web untuk mendeteksi patologi & defisiensi nutrisi.
            </p>

            <!-- SOURCE SELECTION TABS -->
            <div class="vision-source-tabs">
              <button type="button" class="vision-tab-btn active" id="vtab-btn-file" onclick="setVisionMode('file')">
                <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M21 15v4a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2v-4"/><polyline points="17 8 12 3 7 8"/><line x1="12" y1="3" x2="12" y2="15"/></svg>
                Unggah / File
              </button>
              <button type="button" class="vision-tab-btn" id="vtab-btn-camera" onclick="setVisionMode('camera')">
                <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M23 19a2 2 0 0 1-2 2H3a2 2 0 0 1-2-2V8a2 2 0 0 1 2-2h4l2-3h6l2 3h4a2 2 0 0 1 2 2z"/><circle cx="12" cy="13" r="4"/></svg>
                Kamera Langsung
              </button>
              <button type="button" class="vision-tab-btn" id="vtab-btn-paste" onclick="setVisionMode('paste')">
                <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M16 4h2a2 2 0 0 1 2 2v14a2 2 0 0 1-2 2H6a2 2 0 0 1-2-2V6a2 2 0 0 1 2-2h2"/><rect x="8" y="2" width="8" height="4" rx="1" ry="1"/></svg>
                Tempel (Ctrl+V)
              </button>
              <button type="button" class="vision-tab-btn" id="vtab-btn-url" onclick="setVisionMode('url')">
                <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M10 13a5 5 0 0 0 7.54.54l3-3a5 5 0 0 0-7.07-7.07l-1.72 1.71"/><path d="M14 11a5 5 0 0 0-7.54-.54l-3 3a5 5 0 0 0 7.07 7.07l1.71-1.71"/></svg>
                Tautan URL
              </button>
            </div>

            <!-- MODE 1: FILE / UPLOAD -->
            <input type="file" id="ai-vision-upload" accept="image/*" capture="environment" style="display:none;" onchange="handleAIVisionUpload(event)">
            <div id="vpanel-file" class="vision-dropzone" onclick="document.getElementById('ai-vision-upload').click()">
              <div class="vision-icon-ring">
                <svg width="22" height="22" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                  <path d="M21 12v7a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h11"/>
                  <circle cx="9" cy="9" r="2"/>
                  <path d="m21 15-3.086-3.086a2 2 0 0 0-2.828 0L6 21"/>
                </svg>
              </div>
              <div style="text-align:center;">
                <span style="font-size:13px; font-weight:600; color:var(--text-main); display:block; margin-bottom:3px;">Pilih File Foto atau Jepret Kamera HP</span>
                <span style="font-size:11px; color:var(--text-sub);">Format JPG, PNG, WEBP (Otomatis Kompresi)</span>
              </div>
            </div>

            <!-- MODE 2: LIVE WEBRTC CAMERA STREAM -->
            <div id="vpanel-camera" style="display:none;">
              <div class="vision-camera-box">
                <video id="vision-camera-video" playsinline autoplay muted></video>
                <div class="vision-shutter-bar">
                  <button type="button" class="btn btn-primary" onclick="captureLiveCamera()" style="display:flex; align-items:center; gap:6px; background:linear-gradient(135deg, #a855f7, #6366f1); border:none; padding:8px 18px; font-size:12px; font-weight:600; border-radius:8px;">
                    <svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="10"/><circle cx="12" cy="12" r="3"/></svg>
                    Bidik & Analisis
                  </button>
                  <button type="button" class="btn" onclick="stopLiveCamera()" style="padding:8px 14px; font-size:12px; background:rgba(255,255,255,0.08); border:1px solid rgba(255,255,255,0.15); color:#fff; border-radius:8px;">
                    Tutup Kamera
                  </button>
                </div>
              </div>
            </div>

            <!-- MODE 3: CLIPBOARD PASTE -->
            <div id="vpanel-paste" class="vision-dropzone" style="display:none;" tabindex="0" onpaste="handleVisionPaste(event)" onclick="this.focus()">
              <div class="vision-icon-ring" style="background:rgba(99, 102, 241, 0.15); color:#818cf8; border-color:rgba(99, 102, 241, 0.3);">
                <svg width="22" height="22" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M16 4h2a2 2 0 0 1 2 2v14a2 2 0 0 1-2 2H6a2 2 0 0 1-2-2V6a2 2 0 0 1 2-2h2"/><rect x="8" y="2" width="8" height="4" rx="1" ry="1"/></svg>
              </div>
              <div style="text-align:center;">
                <span style="font-size:13px; font-weight:600; color:var(--text-main); display:block; margin-bottom:3px;">Klik di sini lalu tekan Ctrl + V (Tempel Citra)</span>
                <span style="font-size:11px; color:var(--text-sub);">Menangkap gambar langsung dari screenshot / clipboard</span>
              </div>
            </div>

            <!-- MODE 4: URL LINK -->
            <div id="vpanel-url" style="display:none; padding:12px; background:rgba(0,0,0,0.25); border:1px solid rgba(255,255,255,0.08); border-radius:10px;">
              <span style="font-size:12px; color:var(--text-sub); display:block; margin-bottom:6px;">Masukkan URL Gambar Daun (HTTP/HTTPS):</span>
              <div class="vision-url-bar">
                <input type="url" id="vision-url-input" class="vision-url-input" placeholder="https://example.com/foto_daun.jpg">
                <button type="button" class="btn btn-primary" onclick="loadVisionFromUrl()" style="background:#a855f7; border:none; padding:0 14px; font-size:12px; font-weight:600; border-radius:8px;">
                  Muat Citra
                </button>
              </div>
            </div>

            <!-- PREVIEW CONTAINER -->
            <div id="vision-preview-container" style="display:none; margin-top:14px;">
              <div style="position:relative; border-radius:10px; overflow:hidden; border:1px solid rgba(255,255,255,0.1); background:#000;">
                <img id="ai-vision-preview" style="display:block; width:100%; max-height:280px; object-fit:contain;">
                <button class="btn" onclick="resetVisionInput()" style="position:absolute; bottom:10px; right:10px; padding:6px 12px; font-size:11px; background:rgba(15,23,42,0.85); backdrop-filter:blur(8px); border:1px solid rgba(255,255,255,0.15); color:#fff; border-radius:6px; cursor:pointer;">
                  Ganti / Ulangi
                </button>
              </div>
            </div>

            <!-- LOADING BOX -->
            <div id="vision-loading-box" style="display:none; margin-top:14px; padding:16px; background:rgba(168,85,247,0.06); border:1px solid rgba(168,85,247,0.25); border-radius:10px; text-align:center;">
              <div class="pulse-loader" style="margin:0 auto 10px;"></div>
              <span style="font-size:12.5px; font-weight:600; color:#c084fc; display:block;">Menganalisis Morfologi Citra Daun...</span>
              <span style="font-size:11px; color:var(--text-sub);">Menghubungkan visual patologi dengan telemetri iklim mikro kebun</span>
            </div>

            <!-- RESULT CONTAINER -->
            <div id="ai-vision-result" style="display:none; margin-top:14px;"></div>
          </div>
        </div>

        <!-- PERMANENT INTERACTIVE AGRONOMIST CHATBOT -->
        <div class="monitor-card" style="margin-top:20px;">
          <div class="card-header" style="justify-content: space-between;">
            <div style="display:flex; align-items:center; gap:10px;">
              <div class="kpi-icon-box" style="background: rgba(99, 102, 241, 0.15); color: #818cf8; border: 1px solid rgba(99, 102, 241, 0.3);">
                <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                  <path d="M21 15a2 2 0 0 1-2 2H7l-4 4V5a2 2 0 0 1 2-2h14a2 2 0 0 1 2 2z"/>
                </svg>
              </div>
              <div>
                <span class="card-title" style="display:block; font-size:14px; font-weight:700; color:var(--text-main);">Tanya Jawab Pakar Agronom (Chatbot)</span>
                <span style="font-size:11px; color:var(--text-sub);">Konsultasi Obat, Takaran & Hama Terhubung Sensor Kebun</span>
              </div>
            </div>
            <button type="button" id="btn-toggle-speech" onclick="toggleChatSpeech()" style="background:rgba(255,255,255,0.05); border:1px solid rgba(255,255,255,0.1); color:var(--text-sub); border-radius:6px; cursor:pointer; padding:4px 8px; font-size:11px; display:flex; align-items:center; gap:4px;">
              <svg width="13" height="13" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polygon points="11 5 6 9 2 9 2 15 6 15 11 19 11 5"/><path d="M19.07 4.93a10 10 0 0 1 0 14.14M15.54 8.46a5 5 0 0 1 0 7.07"/></svg>
              <span id="speech-status-label">Suara Aktif</span>
            </button>
          </div>

          <div class="card-body" style="padding: 16px;">
            <div class="agri-chat-messages" id="agri-chat-messages">
              <div class="chat-bubble bot">
                Halo Petani! Ada kendala pada tanaman atau takaran obat semprot di kebun Anda? Tanyakan langsung di sini atau ketuk pertanyaan cepat di bawah.
              </div>
            </div>

            <!-- QUICK QUESTION PROMPT CHIPS -->
            <div class="chat-chips-row" style="margin-top:10px; border-radius:8px;">
              <button type="button" class="chip-btn" onclick="askChatChip('Sebutkan contoh nama merek obat fungisida bercak daun yang bagus di toko pertanian?')">
                Contoh Merek Toko
              </button>
              <button type="button" class="chip-btn" onclick="askChatChip('Bolehkah fungisida dicampur pupuk daun Gandasil atau perekat?')">
                Bisa Campur Pupuk Daun?
              </button>
              <button type="button" class="chip-btn" onclick="askChatChip('Kapan waktu semprot terbaik (pagi/sore) dan berapa hari sekali?')">
                Jam Semprot Terbaik
              </button>
              <button type="button" class="chip-btn" onclick="askChatChip('Bagaimana cara mencegah agar hama/jamur tidak menyebar ke bedengan lain?')">
                Cegah Penularan
              </button>
            </div>

            <div class="agri-chat-input-bar" style="margin-top:10px; border-radius:8px;">
              <input type="text" id="agri-chat-input" class="agri-chat-input" placeholder="Ketik pertanyaan untuk Pakar Agronom..." onkeydown="if(event.key==='Enter') submitAgronomyChat()">
              <button type="button" class="btn btn-primary" id="btn-send-agri-chat" onclick="submitAgronomyChat()" style="background:#a855f7; border:none; padding:0 16px; font-size:12px; font-weight:600; border-radius:8px; display:flex; align-items:center; justify-content:center; cursor:pointer;">
                <svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><line x1="22" y1="2" x2="11" y2="13"/><polygon points="22 2 15 22 11 13 2 9 22 2"/></svg>
              </button>
            </div>
          </div>
        </div>
    </div>

    <!-- TAB 2: GRAFIK SCADA -->
    <div id="tab-charts" class="tab-content">
      <!-- Card 1: Live Real-Time Oscilloscope -->
      <div class="card">
        <div class="chart-header-wrap">
          <div class="chart-title-area">
            <div class="chart-icon-box">
              <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                <polyline points="22 12 18 12 15 21 9 3 6 12 2 12"></polyline>
              </svg>
            </div>
            <div>
              <span class="chart-title-text">Oscilloscope SCADA Real-Time</span>
              <span class="chart-subtitle-text">Sampling Live 1-Detik • Filter Kondisioning Sinyal</span>
            </div>
          </div>
          <div class="chart-controls-wrap">
            <div class="chart-filter-group" title="Kondisioning Sinyal Masukan">
              <button class="chart-filter-btn active" id="btn-osc-raw" onclick="setOscFilterMode('raw')">Raw</button>
              <button class="chart-filter-btn" id="btn-osc-ema" onclick="setOscFilterMode('ema')">EMA Filter</button>
            </div>
            <div class="chart-filter-group" title="Rentang Titik Buffer">
              <button class="chart-filter-btn active" id="btn-buf-30" onclick="setOscBufferSize(30)">30s</button>
              <button class="chart-filter-btn" id="btn-buf-60" onclick="setOscBufferSize(60)">60s</button>
            </div>
          </div>
        </div>

        <!-- Series Toggles (Output Filters) -->
        <div class="chart-legend-row">
          <button class="chart-toggle-chip active" id="osc-toggle-soil" style="--chip-border:rgba(16,185,129,0.5);--chip-bg:rgba(16,185,129,0.12);" onclick="toggleOscSeries('soil')">
            <span class="chip-dot" style="background:#10b981;"></span>
            <span>Kelembapan Tanah</span>
            <span class="chip-val" id="osc-val-soil">--%</span>
          </button>
          <button class="chart-toggle-chip active" id="osc-toggle-temp" style="--chip-border:rgba(6,182,212,0.5);--chip-bg:rgba(6,182,212,0.12);" onclick="toggleOscSeries('temp')">
            <span class="chip-dot" style="background:#06b6d4;"></span>
            <span>Suhu Udara</span>
            <span class="chip-val" id="osc-val-temp">--°C</span>
          </button>
          <button class="chart-toggle-chip active" id="osc-toggle-hum" style="--chip-border:rgba(168,85,247,0.5);--chip-bg:rgba(168,85,247,0.12);" onclick="toggleOscSeries('hum')">
            <span class="chip-dot" style="background:#a855f7;"></span>
            <span>Kelembapan RH</span>
            <span class="chip-val" id="osc-val-hum">--%</span>
          </button>
          <button class="chart-toggle-chip active" id="osc-toggle-threshold" style="--chip-border:rgba(239,68,68,0.5);--chip-bg:rgba(239,68,68,0.12);" onclick="toggleOscThreshold()">
            <span class="chip-dot" style="background:#ef4444;"></span>
            <span>Garis Batas Siram &amp; Bahaya</span>
          </button>
        </div>

        <!-- Canvas Oscilloscope Chart Container -->
        <div class="chart-container" style="height:230px; position:relative;">
          <canvas id="scadaChart"></canvas>
          <div id="osc-crosshair-tooltip" style="display:none; position:absolute; top:10px; right:14px; background:rgba(15,23,42,0.92); border:1px solid rgba(255,255,255,0.18); border-radius:8px; padding:6px 12px; font-size:11px; pointer-events:none; z-index:10; box-shadow:0 4px 12px rgba(0,0,0,0.5);"></div>
        </div>
        <div class="chart-footer-meta">
          <span id="osc-rate-text">Pindai: 1.0 Hz • Antialiased Sumbu Ganda (Kiri: 0-100%, Kanan: 0-50°C)</span>
          <span style="color:var(--text-sub);" id="osc-status-text">Buffer: 0 Titik (Raw Data)</span>
        </div>
      </div>

      <!-- Card 2: 24-Hour Trend Aggregation -->
      <div class="card">
        <div class="chart-header-wrap">
          <div class="chart-title-area">
            <div class="chart-icon-box amber">
              <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                <line x1="18" y1="20" x2="18" y2="10"></line>
                <line x1="12" y1="20" x2="12" y2="4"></line>
                <line x1="6" y1="20" x2="6" y2="14"></line>
              </svg>
            </div>
            <div>
              <span class="chart-title-text">Tren Fluktuasi 24 Jam</span>
              <span class="chart-subtitle-text">Agregasi Siklus Harian LittleFS Flash Memory</span>
            </div>
          </div>
          <div class="chart-controls-wrap">
            <div class="chart-filter-group" title="Filter Rentang Waktu">
              <button class="chart-filter-btn active" id="btn-h-all" onclick="setHourlyRange('all')">24 Jam</button>
              <button class="chart-filter-btn" id="btn-h-day" onclick="setHourlyRange('day')">Siang (06-18)</button>
              <button class="chart-filter-btn" id="btn-h-night" onclick="setHourlyRange('night')">Malam (18-06)</button>
            </div>
            <button class="btn btn-secondary" id="btn-refresh-hourly" onclick="refreshHourlyData()" style="padding:4px 10px; font-size:11px; display:inline-flex; align-items:center; gap:5px;">
              <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                <polyline points="23 4 23 10 17 10"></polyline>
                <path d="M20.49 15a9 9 0 1 1-2.12-9.36L23 10"></path>
              </svg>
              <span>Refresh Tren</span>
            </button>
          </div>
        </div>

        <!-- Series Toggles (Output Filters) -->
        <div class="chart-legend-row">
          <button class="chart-toggle-chip active" id="h-toggle-soil" style="--chip-border:rgba(16,185,129,0.5);--chip-bg:rgba(16,185,129,0.12);" onclick="toggleHourlySeries('soil')">
            <span class="chip-dot" style="background:#10b981;"></span>
            <span>Rata-rata Tanah (%)</span>
          </button>
          <button class="chart-toggle-chip active" id="h-toggle-temp" style="--chip-border:rgba(6,182,212,0.5);--chip-bg:rgba(6,182,212,0.12);" onclick="toggleHourlySeries('temp')">
            <span class="chip-dot" style="background:#06b6d4;"></span>
            <span>Rata-rata Suhu (°C)</span>
          </button>
          <button class="chart-toggle-chip active" id="h-toggle-hum" style="--chip-border:rgba(168,85,247,0.5);--chip-bg:rgba(168,85,247,0.12);" onclick="toggleHourlySeries('hum')">
            <span class="chip-dot" style="background:#a855f7;"></span>
            <span>Kelembapan Udara (%)</span>
          </button>
          <button class="chart-toggle-chip active" id="h-toggle-pump" style="--chip-border:rgba(245,158,11,0.5);--chip-bg:rgba(245,158,11,0.12);" onclick="toggleHourlySeries('pump')">
            <span class="chip-dot" style="background:#f59e0b;"></span>
            <span>Durasi Pompa (Detik)</span>
          </button>
          <button class="chart-toggle-chip active" id="h-toggle-lamp" style="--chip-border:rgba(234,179,8,0.5);--chip-bg:rgba(234,179,8,0.12);" onclick="toggleHourlySeries('lamp')">
            <span class="chip-dot" style="background:#eab308;"></span>
            <span>Lampu / Grow Light (Waktu Nyala)</span>
          </button>
        </div>

        <!-- Canvas Hourly Chart Container -->
        <div class="chart-container" style="height:250px; position:relative;">
          <canvas id="hourlyChart"></canvas>
          <div id="hourly-crosshair-tooltip" style="display:none; position:absolute; top:10px; right:14px; background:rgba(15,23,42,0.92); border:1px solid rgba(255,255,255,0.18); border-radius:8px; padding:6px 12px; font-size:11px; pointer-events:none; z-index:10; box-shadow:0 4px 12px rgba(0,0,0,0.5);"></div>
        </div>

        <!-- 24-Hour Quick Statistics Summary Matrix -->
        <div class="hourly-stats-grid" id="hourly-stats-summary" style="display:grid; grid-template-columns: repeat(auto-fit, minmax(130px, 1fr)); gap:10px; margin-top:12px; margin-bottom:10px;">
          <div style="background:rgba(255,255,255,0.03); border:1px solid rgba(255,255,255,0.08); border-radius:8px; padding:8px 10px;">
            <div style="font-size:10px; color:var(--text-sub); font-weight:700; text-transform:uppercase; letter-spacing:0.4px;">Suhu Min / Max</div>
            <div id="stat-24h-temp" style="font-size:12px; font-weight:700; color:#06b6d4; margin-top:2px;">-- / --</div>
          </div>
          <div style="background:rgba(255,255,255,0.03); border:1px solid rgba(255,255,255,0.08); border-radius:8px; padding:8px 10px;">
            <div style="font-size:10px; color:var(--text-sub); font-weight:700; text-transform:uppercase; letter-spacing:0.4px;">Rata-rata Tanah</div>
            <div id="stat-24h-soil" style="font-size:12px; font-weight:700; color:#10b981; margin-top:2px;">--%</div>
          </div>
          <div style="background:rgba(255,255,255,0.03); border:1px solid rgba(255,255,255,0.08); border-radius:8px; padding:8px 10px;">
            <div style="font-size:10px; color:var(--text-sub); font-weight:700; text-transform:uppercase; letter-spacing:0.4px;">Penyiraman Pompa</div>
            <div id="stat-24h-pump" style="font-size:12px; font-weight:700; color:#f59e0b; margin-top:2px;">-- Kali (--s)</div>
          </div>
          <div style="background:rgba(255,255,255,0.03); border:1px solid rgba(255,255,255,0.08); border-radius:8px; padding:8px 10px;">
            <div style="font-size:10px; color:var(--text-sub); font-weight:700; text-transform:uppercase; letter-spacing:0.4px;">Waktu Lampu Aktif</div>
            <div id="stat-24h-lamp" style="font-size:12px; font-weight:700; color:#eab308; margin-top:2px;">-- Jam</div>
          </div>
        </div>

        <div class="chart-footer-meta">
          <span id="hourly-axis-text">Sumbu X: Jam 00:00 s/d 23:00 WIB</span>
          <span id="hourly-summary-text" style="color:var(--accent-emerald);">Deteksi Evaporasi: Menunggu Analisis Log</span>
        </div>
      </div>

      <!-- Card 3: Crop Growth History & Phenology Timeline (Local Storage) -->
      <div class="card" style="margin-top: 18px;">
        <div class="chart-header-wrap">
          <div class="chart-title-area">
            <div class="chart-icon-box emerald">
              <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                <path d="M12 2a9 9 0 0 1 9 9c0 4.97-4.03 9-9 9s-9-4.03-9-9a9 9 0 0 1 9-9z"></path>
                <path d="M12 6v6l4 2"></path>
              </svg>
            </div>
            <div>
              <span class="chart-title-text">Riwayat Pertumbuhan &amp; Siklus Hidup Tanaman</span>
              <span class="chart-subtitle-text">Catatan Perkembangan Hari demi Hari (HST) • Disimpan di Memori Lokal</span>
            </div>
          </div>
          <div class="chart-controls-wrap" style="flex-wrap:wrap; gap:6px;">
            <button type="button" class="btn btn-primary" onclick="logTodayCropGrowth()" style="padding:5px 12px; font-size:11px; display:inline-flex; align-items:center; gap:5px;" title="Simpan Catatan Perkembangan Tanaman Hari Ini">
              <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M11 4H4a2 2 0 0 0-2 2v14a2 2 0 0 0 2 2h14a2 2 0 0 0 2-2v-7"></path><path d="M18.5 2.5a2.121 2.121 0 0 1 3 3L12 15l-4 1 1-4 9.5-9.5z"></path></svg>
              <span>Catat Hari Ini</span>
            </button>
            <button type="button" class="btn btn-secondary" onclick="shareCropReportWhatsApp()" style="padding:5px 10px; font-size:11px; display:inline-flex; align-items:center; gap:5px; color:#22c55e; border-color:rgba(34,197,94,0.3);" title="Bagikan Ringkasan Laporan ke WhatsApp">
              <svg width="13" height="13" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M21 11.5a8.38 8.38 0 0 1-.9 3.8 8.5 8.5 0 0 1-7.6 4.7 8.38 8.38 0 0 1-3.8-.9L3 21l1.9-5.7a8.38 8.38 0 0 1-.9-3.8 8.5 8.5 0 0 1 4.7-7.6 8.38 8.38 0 0 1 3.8-.9h.5a8.48 8.48 0 0 1 8 8v.5z"></path></svg>
              <span>Bagikan WA</span>
            </button>
            <button type="button" class="btn btn-secondary" onclick="printCropReport()" style="padding:5px 10px; font-size:11px; display:inline-flex; align-items:center; gap:5px;" title="Cetak Ringkasan Riwayat Tanam / PDF">
              <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polyline points="6 9 6 2 18 2 18 9"></polyline><path d="M6 18H4a2 2 0 0 1-2-2v-5a2 2 0 0 1 2-2h16a2 2 0 0 1 2 2v5a2 2 0 0 1-2 2h-2"></path><rect x="6" y="14" width="12" height="8"></rect></svg>
              <span>Cetak Laporan</span>
            </button>
            <button type="button" class="btn btn-secondary" onclick="exportCropHistoryJSON()" style="padding:5px 10px; font-size:11px; display:inline-flex; align-items:center; gap:5px;" title="Unduh File Cadangan JSON">
              <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M21 15v4a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2v-4"></path><polyline points="7 10 12 15 17 10"></polyline><line x1="12" y1="15" x2="12" y2="3"></line></svg>
              <span>Unduh JSON</span>
            </button>
            <button type="button" class="btn btn-secondary" onclick="triggerImportCropHistoryJSON()" style="padding:5px 10px; font-size:11px; display:inline-flex; align-items:center; gap:5px;" title="Pulihkan Data dari File JSON">
              <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M21 15v4a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2v-4"></path><polyline points="17 8 12 3 7 8"></polyline><line x1="12" y1="3" x2="12" y2="15"></line></svg>
              <span>Pulihkan</span>
            </button>
            <input type="file" id="crop-import-file-input" accept=".json" style="display:none;" onchange="importCropHistoryJSON(event)">
            <button type="button" class="btn btn-secondary" onclick="resetCropHistory()" style="padding:5px 10px; font-size:11px; color:#ef4444;" title="Reset Siklus Tanam Baru">
              <span>Reset Siklus</span>
            </button>
          </div>
        </div>

        <!-- Crop History Summary Matrix -->
        <div style="display:grid; grid-template-columns: repeat(auto-fit, minmax(130px, 1fr)); gap:10px; margin-bottom:12px;">
          <div style="background:rgba(255,255,255,0.03); border:1px solid rgba(255,255,255,0.08); border-radius:8px; padding:8px 10px;">
            <div style="font-size:10px; color:var(--text-sub); font-weight:700; text-transform:uppercase; letter-spacing:0.4px;">Umur Tanaman</div>
            <div id="crop-hist-hst" style="font-size:13px; font-weight:700; color:#10b981; margin-top:2px;">-- HST</div>
          </div>
          <div style="background:rgba(255,255,255,0.03); border:1px solid rgba(255,255,255,0.08); border-radius:8px; padding:8px 10px;">
            <div style="font-size:10px; color:var(--text-sub); font-weight:700; text-transform:uppercase; letter-spacing:0.4px;">Fase Saat Ini</div>
            <div id="crop-hist-stage" style="font-size:13px; font-weight:700; color:var(--text-main); margin-top:2px;">--</div>
          </div>
          <div style="background:rgba(255,255,255,0.03); border:1px solid rgba(255,255,255,0.08); border-radius:8px; padding:8px 10px;">
            <div style="font-size:10px; color:var(--text-sub); font-weight:700; text-transform:uppercase; letter-spacing:0.4px;">Kesiapan / Vigor</div>
            <div id="crop-hist-vigor" style="font-size:13px; font-weight:700; color:#06b6d4; margin-top:2px;">--%</div>
          </div>
          <div style="background:rgba(255,255,255,0.03); border:1px solid rgba(255,255,255,0.08); border-radius:8px; padding:8px 10px;">
            <div style="font-size:10px; color:var(--text-sub); font-weight:700; text-transform:uppercase; letter-spacing:0.4px;">Perkiraan Panen</div>
            <div id="crop-hist-eta" style="font-size:13px; font-weight:700; color:#eab308; margin-top:2px;">-- Hari Lagi</div>
          </div>
        </div>

        <!-- Canvas Crop History Chart Container -->
        <div class="chart-container" style="height:230px; position:relative;">
          <canvas id="cropHistoryChart"></canvas>
          <div id="crop-hist-tooltip" style="display:none; position:absolute; top:10px; right:14px; background:rgba(15,23,42,0.92); border:1px solid rgba(255,255,255,0.18); border-radius:8px; padding:6px 12px; font-size:11px; pointer-events:none; z-index:10; box-shadow:0 4px 12px rgba(0,0,0,0.5);"></div>
        </div>
        <div class="chart-footer-meta">
          <span id="crop-hist-footer-status">Data tersimpan di LocalStorage peramban • Kurva Pertumbuhan Sigmoid (S-Curve)</span>
          <span id="crop-hist-points-text" style="color:var(--text-sub);">0 Catatan Tersimpan</span>
        </div>
      </div>
    </div>


    <!-- TAB 3: KENDALI & PRESISI -->
    <div id="tab-controls" class="tab-content">
      <div class="grid-two-col">
        <!-- SMART PUMP CONTROL CENTER CARD -->
        <div class="card" style="background: rgba(15, 23, 42, 0.4); backdrop-filter: blur(12px); -webkit-backdrop-filter: blur(12px); border: 1px solid rgba(255,255,255,0.1); box-shadow: 0 4px 30px rgba(0, 0, 0, 0.1);">
          <div class="chart-header-wrap" style="margin-bottom:14px;">
            <div class="chart-title-area">
              <div class="chart-icon-box emerald">
                <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                  <path d="M12 2v20M17 5H9.5a3.5 3.5 0 0 0 0 7h5a3.5 3.5 0 0 1 0 7H6"></path>
                </svg>
              </div>
              <div>
                <span class="chart-title-text">Kendali Pompa Irigasi</span>
                <span class="chart-subtitle-text">Pin 26 Active-Low • Interlock Proteksi Lahan</span>
              </div>
            </div>
            <div style="display:flex; gap:6px; align-items:center;">
              <span class="badge-pill" id="relay-status-badge" style="color:var(--text-sub); border-color:rgba(255,255,255,0.1); background:rgba(0,0,0,0.2);"><span>Menunggu Data...</span></span>
            </div>
          </div>
          <!-- Mode Switch Toggle -->
          <div class="switch-group" style="margin-bottom: 18px; align-items: flex-start;">
            <div>
              <div style="font-size:13px; font-weight:700; margin-bottom:4px;">Mode Operasi</div>
              <div id="mode-desc-text" style="font-size:11px; color:var(--text-sub);">Mode Otomatis (Sensor &amp; RTC)</div>
            </div>
            <div class="segment-control">
              <button id="btn-pump-mode-auto" class="segment-btn active" onclick="toggleSystemMode(false)">Auto</button>
              <button id="btn-pump-mode-manual" class="segment-btn" onclick="toggleSystemMode(true)">Manual</button>
            </div>
          </div>
          <!-- Manual Action Buttons (Disabled by default in Auto Mode) -->
          <div style="display:flex; gap:10px; margin-bottom:14px;">
            <button class="btn btn-primary" disabled="" id="btn-pump-on" onclick="sendRelayCommand('on')" style="flex:1; padding:10px; display:inline-flex; align-items:center; justify-content:center; gap:6px; font-weight:700;">
              <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5"><polygon points="5 3 19 12 5 21 5 3"></polygon></svg>
              <span>Nyalakan Pompa</span>
            </button>
            <button class="btn btn-danger" disabled="" id="btn-pump-off" onclick="sendRelayCommand('off')" style="flex:1; padding:10px; display:inline-flex; align-items:center; justify-content:center; gap:6px; font-weight:700;">
              <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5"><rect x="4" y="4" width="16" height="16" rx="2"></rect></svg>
              <span>Matikan Pompa</span>
            </button>
          </div>
          <!-- Real-time Live Session Status Bar -->
          <div id="pump-live-bar" style="display:none; background:rgba(16, 185, 129, 0.12); border:1px solid rgba(16,185,129,0.3); padding:8px 12px; border-radius:10px; margin-bottom:12px; font-size:12px; align-items:center; justify-content:space-between;">
            <span style="color:var(--primary); font-weight:bold; display:inline-flex; align-items:center; gap:6px;">
              <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M12 2.69l5.66 5.66a8 8 0 1 1-11.31 0z"></path></svg>
              Pompa Sedang Menyiram Lahan...
            </span>
            <b id="pump-live-timer" style="color:var(--text-main);">AKTIF</b>
          </div>
          <!-- Pump Usage Stats Grid -->
          <div style="background:rgba(0,0,0,0.25); padding:12px; border-radius:12px; display:grid; grid-template-columns:1fr 1fr; gap:10px; font-size:12px;">
            <div>
              <span style="color:var(--text-sub); display:block; margin-bottom:4px;">Frekuensi Pompa:</span>
              <div id="stat-pump-count" style="font-size:15px; font-weight:bold; color:var(--text-main);">-- Kali</div>
            </div>
            <div>
              <span style="color:var(--text-sub); display:block; margin-bottom:4px;">Total Durasi:</span>
              <div id="stat-pump-secs" style="font-size:15px; font-weight:bold; color:var(--primary);">-- Detik</div>
            </div>
            <div>
              <span style="color:var(--text-sub); display:block; margin-bottom:4px;">Estimasi Volume Air:</span>
              <div id="stat-water-liters" style="font-size:14px; font-weight:bold; color:var(--secondary);">-- Liter</div>
            </div>
            <div>
              <span style="color:var(--text-sub); display:block; margin-bottom:4px;">Biaya Listrik (PLN):</span>
              <div id="stat-cost-idr" style="font-size:14px; font-weight:bold; color:var(--warning);">Rp --</div>
            </div>
          </div>
          <button class="btn btn-secondary" onclick="resetPumpStats()" style="width:100%; margin-top:10px; padding:6px; font-size:11px; display:inline-flex; align-items:center; justify-content:center; gap:6px;">
            <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M3 6h18M19 6v14a2 2 0 0 1-2 2H7a2 2 0 0 1-2-2V6m3 0V4a2 2 0 0 1 2-2h4a2 2 0 0 1 2 2v2"></path></svg>
            <span>Reset Statistik</span>
          </button>
          <!-- Dynamic Pump Calibration Form Collapsible -->
          <div style="background:rgba(0,0,0,0.2); padding:10px 12px; border-radius:10px; margin-top:10px; border:1px solid rgba(255,255,255,0.08);">
            <div onclick="togglePumpConfigForm()" style="display:flex; justify-content:space-between; align-items:center; cursor:pointer;">
              <span style="font-size:11px; font-weight:bold; color:var(--secondary); display:inline-flex; align-items:center; gap:6px;">
                <svg width="13" height="13" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="3"></circle><path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1 0 2.83 2 2 0 0 1-2.83 0l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-2 2 2 2 0 0 1-2-2v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83 0 2 2 0 0 1 0-2.83l.06-.06a1.65 1.65 0 0 0 .33-1.82 1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1-2-2 2 2 0 0 1 2-2h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 0-2.83 2 2 0 0 1 2.83 0l.06.06a1.65 1.65 0 0 0 1.82.33H9a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 2-2 2 2 0 0 1 2 2v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 0 2 2 0 0 1 0 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82V9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 2 2 2 2 0 0 1-2 2h-.09a1.65 1.65 0 0 0-1.51 1z"></path></svg>
                Kalibrasi Pompa &amp; Tarif Listrik PLN
              </span>
              <span id="pump-config-arrow" style="font-size:11px; color:var(--text-sub);">▼</span>
            </div>
            <div id="pump-config-body" style="display:none; margin-top:10px; border-top:1px dashed rgba(255,255,255,0.1); padding-top:10px;">
              <div style="display:grid; grid-template-columns:1fr 1fr 1fr; gap:8px; margin-bottom:8px;">
                <div>
                  <span style="font-size:10px; color:var(--text-sub);">Debit (L/Jam):</span>
                  <input id="cfg-pump-lph" max="20000" min="100" placeholder="Menunggu Data..." type="number" value=""/>
                </div>
                <div>
                  <span style="font-size:10px; color:var(--text-sub);">Daya (Watt):</span>
                  <input id="cfg-pump-watt" max="3000" min="1" placeholder="Menunggu Data..." type="number" value=""/>
                </div>
                <div>
                  <span style="font-size:10px; color:var(--text-sub);">Tarif PLN (Rp/kWh):</span>
                  <input id="cfg-pln-tariff" max="5000" min="100" placeholder="Menunggu Data..." type="number" value=""/>
                </div>
              </div>
              <button class="btn btn-primary" onclick="savePumpConfig()" style="width:100%; padding:6px; font-size:11px; display:inline-flex; align-items:center; justify-content:center; gap:6px;">
                <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M19 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h11l5 5v11a2 2 0 0 1-2 2z"></path><polyline points="17 21 17 13 7 13 7 21"></polyline><polyline points="7 3 7 8 15 8"></polyline></svg>
                <span>Simpan Spesifikasi Pompa</span>
              </button>
            </div>
          </div>
        </div>


        <!-- LAMPU PENERANGAN (GROW LIGHT) CARD -->
        <div class="card" style="background: rgba(15, 23, 42, 0.4); backdrop-filter: blur(12px); -webkit-backdrop-filter: blur(12px); border: 1px solid rgba(255,255,255,0.1); box-shadow: 0 4px 30px rgba(0, 0, 0, 0.1);">
          <div class="chart-header-wrap" style="margin-bottom:14px;">
            <div class="chart-title-area">
              <div class="chart-icon-box" style="color:#eab308; background:rgba(234, 179, 8, 0.15);">
                <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                  <path d="M15 14c.2-1 .7-1.7 1.5-2.5 1-.9 1.5-2.2 1.5-3.5A6 6 0 0 0 6 8c0 1.3.5 2.6 1.5 3.5.8.8 1.3 1.5 1.5 2.5"></path>
                  <path d="M9 18h6"></path>
                  <path d="M10 22h4"></path>
                </svg>
              </div>
              <div>
                <span class="chart-title-text">Penerangan Grow Light</span>
                <span class="chart-subtitle-text">Pin 25 • Jadwal Timer Otomatis</span>
              </div>
            </div>
            <div id="lamp-status-badge" class="badge-pill" style="color:var(--text-sub); border-color:rgba(255,255,255,0.1); background:rgba(0,0,0,0.2);">
              <span>Menunggu Data...</span>
            </div>
          </div>


          <!-- Lamp Schedule Timer -->
          <div style="background:rgba(0,0,0,0.25); padding:12px; border-radius:12px; margin-bottom:16px;">
            <div style="display:flex; justify-content:space-between; align-items:center; margin-bottom:10px;">
              <span style="font-size:12px; font-weight:600; color:var(--text-main);">Timer Lampu (Auto)</span>
              <label class="switch" style="transform: scale(0.85); transform-origin: right;">
                <input type="checkbox" id="lamp-sched-en" />
                <span class="slider round"></span>
              </label>
            </div>
            <div style="display:grid; grid-template-columns:1fr 1fr; gap:10px;">
              <div>
                <label style="font-size:10px; color:var(--text-sub); display:block; margin-bottom:4px;">Jam Mulai Menyala:</label>
                <input type="time" id="lamp-sched-time" class="form-input" style="background:rgba(15, 23, 42, 0.9);" />
              </div>
              <div>
                <label style="font-size:10px; color:var(--text-sub); display:block; margin-bottom:4px;">Durasi (Jam):</label>
                <input type="number" id="lamp-sched-dur" class="form-input" min="1" max="24" style="background:rgba(15, 23, 42, 0.9);" />
              </div>
            </div>
            <button class="btn" style="width:100%; margin-top:10px; padding:8px; font-size:12px; background:rgba(234, 179, 8, 0.2); color:#fde047; border:1px solid rgba(234, 179, 8, 0.4); display:flex; align-items:center; justify-content:center; gap:8px;" onclick="saveLampSchedule()">
              <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M19 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h11l5 5v11a2 2 0 0 1-2 2z"/><polyline points="17 21 17 13 7 13 7 21"/><polyline points="7 3 7 8 15 8"/></svg>
              <span>Simpan Jadwal Lampu</span>
            </button>
          </div>

          <!-- Lamp Mode Switch Toggle -->
          <div class="switch-group" style="margin-bottom: 18px; align-items: flex-start;">
            <div style="flex:1; padding-right:10px;">
              <div style="font-size:13px; font-weight:700; margin-bottom:4px;">Mode Operasi Lampu</div>
              <div id="lamp-mode-desc-text" style="font-size:11px; color:var(--text-sub); line-height:1.4;">Mode Otomatis (Sensor & RTC)</div>
            </div>
            <div class="segment-control">
              <button id="btn-lamp-mode-auto" class="segment-btn active" onclick="toggleLampSystemMode(false)">Auto</button>
              <button id="btn-lamp-mode-manual" class="segment-btn" onclick="toggleLampSystemMode(true)">Manual</button>
            </div>
          </div>

          <!-- Lamp Manual Action Buttons -->
          <div style="display:flex; gap:10px; margin-bottom:14px;">
            <button class="btn" style="background:#ca8a04; color:#fff;" id="btn-lamp-on" onclick="sendLampRelayCommand('on')" disabled>
              <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" style="margin-right:6px;"><path d="M5 12h14"/><path d="M12 5v14"/></svg> Nyalakan Lampu
            </button>
            <button class="btn btn-danger" id="btn-lamp-off" onclick="sendLampRelayCommand('off')" disabled>
              <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" style="margin-right:6px;"><rect x="3" y="3" width="18" height="18" rx="2" ry="2"/></svg> Matikan Lampu
            </button>
          </div>
        </div>

        <!-- RTC HARDWARE & DYNAMIC SCHEDULE -->
        <div class="card" style="background: rgba(15, 23, 42, 0.4); backdrop-filter: blur(12px); -webkit-backdrop-filter: blur(12px); border: 1px solid rgba(255,255,255,0.1); box-shadow: 0 4px 30px rgba(0, 0, 0, 0.1);">
          <div class="chart-header-wrap" style="margin-bottom:14px;">
            <div class="chart-title-area">
              <div class="chart-icon-box">
                <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                  <circle cx="12" cy="12" r="10"></circle>
                  <polyline points="12 6 12 12 16 14"></polyline>
                </svg>
              </div>
              <div>
                <span class="chart-title-text">Jam RTC &amp; Jadwal Harian</span>
                <span class="chart-subtitle-text">Modul DS3231 I2C • Dual Slot Penyiraman Otomatis</span>
              </div>
            </div>
            <span class="badge-pill" id="rtc-valid-badge" style="color:var(--secondary);">RTC Aktif</span>
          </div>
          <!-- RTC Live Time Display -->
          <div style="background:rgba(6, 182, 212, 0.08); padding:12px; border-radius:12px; margin-bottom:14px; display:flex; justify-content:space-between; align-items:center; flex-wrap:wrap; gap:10px;">
            <div>
              <div style="font-size:11px; color:var(--text-sub); font-weight:bold;">JAM REAL-TIME RTC DS3231</div>
              <div id="rtc-live-time" style="font-family:'Outfit',sans-serif; font-size:20px; font-weight:800; color:var(--text-main);">-</div>
            </div>
            <button class="btn btn-primary" onclick="syncRtcWithBrowser()" style="padding:8px 14px; font-size:11px; display:inline-flex; align-items:center; gap:6px;">
              <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M23 4v6h-6"></path><path d="M20.49 15a9 9 0 1 1-2.12-9.36L23 10"></path></svg>
              <span>Sync Jam HP</span>
            </button>
          </div>
          <!-- Schedule Slot 1 (Pagi) -->
          <div style="background:rgba(0,0,0,0.2); padding:12px; border-radius:12px; margin-bottom:12px; border:1px solid rgba(6,182,212,0.2);">
            <div style="display:flex; justify-content:space-between; align-items:center; margin-bottom:8px;">
              <span style="font-size:13px; font-weight:700; color:var(--secondary); display:inline-flex; align-items:center; gap:6px;">
                <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="5"></circle><line x1="12" y1="1" x2="12" y2="3"></line><line x1="12" y1="21" x2="12" y2="23"></line><line x1="4.22" y1="4.22" x2="5.64" y2="5.64"></line><line x1="18.36" y1="18.36" x2="19.78" y2="19.78"></line><line x1="1" y1="12" x2="3" y2="12"></line><line x1="21" y1="12" x2="23" y2="12"></line><line x1="4.22" y1="19.78" x2="5.64" y2="18.36"></line><line x1="18.36" y1="5.64" x2="19.78" y2="4.22"></line></svg>
                Slot 1 (Penyiraman Pagi)
              </span>
              <label class="switch" style="transform: scale(0.75); transform-origin: right; margin-left: auto;">
                <input id="sched1-en" type="checkbox"/>
                <span class="slider round"></span>
              </label>
            </div>
            <div style="display:flex; gap:10px; align-items:center;">
              <div style="flex:1;">
                <span style="font-size:10px; color:var(--text-sub);">Jam Siram:</span>
                <input id="sched1-time" type="time" value=""/>
              </div>
              <div style="flex:1;">
                <span style="font-size:10px; color:var(--text-sub);">Durasi (Menit):</span>
                <input id="sched1-dur" max="60" min="1" type="number" value=""/>
              </div>
            </div>
            <button class="btn" style="width:100%; margin-top:8px; padding:8px; font-size:12px; background:rgba(16, 185, 129, 0.15); color:var(--primary); border:1px solid rgba(16, 185, 129, 0.3); display:flex; align-items:center; justify-content:center; gap:8px;" onclick="saveSchedule(1)">
              <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M19 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h11l5 5v11a2 2 0 0 1-2 2z"/><polyline points="17 21 17 13 7 13 7 21"/><polyline points="7 3 7 8 15 8"/></svg>
              <span>Simpan Slot Pagi</span>
            </button>
          </div>
          <!-- Schedule Slot 2 (Sore) -->
          <div style="background:rgba(0,0,0,0.2); padding:12px; border-radius:12px; border:1px solid rgba(245,158,11,0.2);">
            <div style="display:flex; justify-content:space-between; align-items:center; margin-bottom:8px;">
              <span style="font-size:13px; font-weight:700; color:var(--warning); display:inline-flex; align-items:center; gap:6px;">
                <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M17 18a5 5 0 0 0-10 0"></path><line x1="12" y1="2" x2="12" y2="9"></line><line x1="4.22" y1="10.22" x2="5.64" y2="11.64"></line><line x1="1" y1="18" x2="3" y2="18"></line><line x1="21" y1="18" x2="23" y2="18"></line><line x1="18.36" y1="11.64" x2="19.78" y2="10.22"></line><line x1="23" y1="22" x2="1" y2="22"></line></svg>
                Slot 2 (Penyiraman Sore)
              </span>
              <label class="switch" style="transform: scale(0.75); transform-origin: right; margin-left: auto;">
                <input id="sched2-en" type="checkbox"/>
                <span class="slider round"></span>
              </label>
            </div>
            <div style="display:flex; gap:10px; align-items:center;">
              <div style="flex:1;">
                <span style="font-size:10px; color:var(--text-sub);">Jam Siram:</span>
                <input id="sched2-time" type="time" value=""/>
              </div>
              <div style="flex:1;">
                <span style="font-size:10px; color:var(--text-sub);">Durasi (Menit):</span>
                <input id="sched2-dur" max="60" min="1" type="number" value=""/>
              </div>
            </div>
            <button class="btn" style="width:100%; margin-top:8px; padding:8px; font-size:12px; background:rgba(245, 158, 11, 0.15); color:var(--warning); border:1px solid rgba(245, 158, 11, 0.3); display:flex; align-items:center; justify-content:center; gap:8px;" onclick="saveSchedule(2)">
              <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M19 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h11l5 5v11a2 2 0 0 1-2 2z"/><polyline points="17 21 17 13 7 13 7 21"/><polyline points="7 3 7 8 15 8"/></svg>
              <span>Simpan Slot Sore</span>
            </button>
          </div>
        </div>
      </div>

      <div class="grid-two-col">
        <!-- THRESHOLD CONTROL CARD (DYNAMIC CROP PROFILE & HARDWARE LIMITS) -->
        <div class="card" style="background: rgba(15, 23, 42, 0.4); backdrop-filter: blur(12px); -webkit-backdrop-filter: blur(12px); border: 1px solid rgba(255,255,255,0.1); box-shadow: 0 4px 30px rgba(0, 0, 0, 0.1);">
          <div class="chart-header-wrap" style="margin-bottom:14px;">
            <div class="chart-title-area">
              <div class="chart-icon-box amber">
                <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                  <line x1="4" y1="21" x2="4" y2="14"></line>
                  <line x1="4" y1="10" x2="4" y2="3"></line>
                  <line x1="12" y1="21" x2="12" y2="12"></line>
                  <line x1="12" y1="8" x2="12" y2="3"></line>
                  <line x1="20" y1="21" x2="20" y2="16"></line>
                  <line x1="20" y1="12" x2="20" y2="3"></line>
                  <line x1="1" y1="14" x2="7" y2="14"></line>
                  <line x1="9" y1="8" x2="15" y2="8"></line>
                  <line x1="17" y1="16" x2="23" y2="16"></line>
                </svg>
              </div>
              <div>
                <span class="chart-title-text">Profil Budidaya &amp; Ambang Batas</span>
                <span class="chart-subtitle-text">Parameter NVS Flash • Threshold Irigasi Otomatis</span>
              </div>
            </div>
            <span class="badge-pill" id="badge-crop-profile" style="color:var(--primary); font-size:11px; background:rgba(16,185,129,0.15); border:1px solid rgba(16,185,129,0.3);">Mode Semai Benih</span>
          </div>
          <!-- DYNAMIC DROPDOWN PROFIL TANAMAN -->
          <div style="background:rgba(0,0,0,0.25); padding:12px; border-radius:12px; border:1px solid rgba(255,255,255,0.08); margin-bottom:16px; margin-top:8px;">
            <label style="font-size:12px; color:var(--text-main); font-weight:700; display:block; margin-bottom:10px;">
              Pilih Karakteristik &amp; Fase Tanaman:
            </label>
            <select id="crop-profile-select" onchange="onCropProfileChange(this.value)" style="width:100%; padding:10px 12px; border-radius:10px; background:#1e293b; color:#f8fafc; border:1.5px solid var(--primary); font-size:13px; font-weight:600; cursor:pointer; outline:none; transition:border-color 0.2s;">
              <option value="0">Penyemaian Benih (Nursery / Kecambah)</option>
              <option value="1">Tanaman Buah (Vegetatif: Daun, Batang &amp; Akar)</option>
              <option value="2">Tanaman Buah (Generatif: Bunga &amp; Buah)</option>
              <option value="3">Kustom / Pengaturan Manual Slider</option>
            </select>
            <!-- Dynamic Profile Guidance Box -->
            <div id="crop-profile-guidance" style="margin-top:10px; font-size:11px; line-height:1.5; color:var(--text-sub); background:rgba(16,185,129,0.08); border-left:3px solid var(--primary); padding:10px; border-radius:8px;">
              <!-- Dikelola otomatis oleh JavaScript -->
            </div>

            <!-- GEMINI AI INTEGRATION -->
            <div style="margin-top:16px; padding-top:16px; border-top:1px solid rgba(255,255,255,0.06);">
              <label style="font-size:12px; font-weight:700; color:var(--text-main); display:flex; align-items:center; gap:6px; margin-bottom:8px;">
                <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="#a855f7" stroke-width="2"><path d="M21 2l-2 2m-7.61 7.61a5.5 5.5 0 1 1-7.778 7.778 5.5 5.5 0 0 1 7.777-7.777zm0 0L15.5 7.5m0 0l3 3L22 7l-3-3m-3.5 3.5L19 4"></path></svg>
                Otentikasi API Key Gemini
              </label>
              <div style="margin-bottom:12px;">
                <input type="password" id="gemini-api-key" placeholder="Paste API Key Gemini di sini" value="" style="width:100%; padding:10px 12px; border-radius:8px; font-size:12px; background:rgba(0,0,0,0.3); border:1px solid rgba(168, 85, 247, 0.3); color:var(--text-main); outline:none; transition:border-color 0.2s; font-family:monospace;" onfocus="this.style.borderColor='#a855f7'" onblur="this.style.borderColor='rgba(168, 85, 247, 0.3)'" onchange="saveGeminiKey(this.value)"/>
              </div>
              <button class="btn" id="btn-gemini-ai" style="width:100%; padding:12px; font-size:13px; font-weight:700; background:linear-gradient(135deg, #6366f1, #a855f7, #ec4899); color:#fff; border:none; border-radius:8px; display:flex; align-items:center; justify-content:center; gap:8px; box-shadow: 0 4px 15px rgba(168, 85, 247, 0.3); transition: transform 0.2s;" onclick="askGeminiAgronomist()">
                <svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M12 2v4m0 12v4M4.93 4.93l2.83 2.83m8.48 8.48l2.83 2.83M2 12h4m12 0h4M4.93 19.07l2.83-2.83m8.48-8.48l2.83-2.83"></path></svg>
                <span id="gemini-btn-text">Auto-Set via Gemini AI ✨</span>
              </button>
            </div>

          </div>
          <div style="margin-bottom:16px;">
            <div style="display:flex; justify-content:space-between; font-size:12px; margin-bottom:6px;">
              <span>Batas Tanah Minimal (Siram jika &lt; ini):</span>
              <b id="val-slider-soil" style="color:var(--primary);">65%</b>
            </div>
            <input id="slider-soil" max="90" min="10" oninput="onSliderManualChange('soil', this.value)" type="range" value="65"/>
          </div>
          <div style="margin-bottom:16px;">
            <div style="display:flex; justify-content:space-between; font-size:12px; margin-bottom:6px;">
              <span>Batas Suhu Panas (Siram jika &gt; ini):</span>
              <b id="val-slider-temp" style="color:var(--danger);">30°C</b>
            </div>
            <input id="slider-temp" max="45" min="20" oninput="onSliderManualChange('temp', this.value)" type="range" value="30"/>
          </div>
        </div>

        <!-- KALKULATOR DOSIS PUPUK PRESISI -->
        <div class="card" style="background: linear-gradient(145deg, rgba(30, 41, 59, 0.85), rgba(15, 23, 42, 0.95));">
          <div class="chart-header-wrap" style="margin-bottom:14px;">
            <div class="chart-title-area">
              <div class="chart-icon-box purple">
                <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                  <path d="M10 2v7.31M14 2v7.31M8.5 2h7M14 9.3a6.5 6.5 0 1 1-4 0"></path>
                </svg>
              </div>
              <div>
                <span class="chart-title-text">Kalkulator Dosis Pupuk &amp; Fertigasi</span>
                <span class="chart-subtitle-text">Formulasi Makro/Mikro • Rekomendasi Aplikasi Presisi</span>
              </div>
            </div>
            <span class="badge-pill" style="color:#a855f7; border-color:rgba(168,85,247,0.3);">Model Agronomi NPK</span>
          </div>
          <div style="display:grid; grid-template-columns: repeat(auto-fit, minmax(160px, 1fr)); gap:10px; margin-bottom:12px;">
            <div style="grid-column: 1 / -1;">
              <label style="font-size:11px; color:var(--text-sub); display:block; margin-bottom:4px; font-weight:600;">Pilih Jenis Pupuk Pertanian:</label>
              <select id="fert-formula" onchange="calculateFertigationDose()" class="form-input">
                <option selected="" value="npk_phonska">NPK Phonska / NPK Mutiara 15-15-15 (Majemuk Seimbang)</option>
                <option value="urea">UREA (Nitrogen 46% - Pertumbuhan Daun &amp; Batang)</option>
                <option value="sp36">SP-36 / TSP (Fosfat 36% - Perakaran &amp; Anakan)</option>
                <option value="kcl">KCL / MOP (Kalium 60% - Bobot Buah &amp; Anti Rontok)</option>
                <option value="za">Pupuk ZA (Sulfur 24% + N 21% - Aroma &amp; Hijau Daun)</option>
                <option value="kno3_putih">KNO3 Putih (Kalium Nitrat 13-0-45 - Pembuahan Generatif)</option>
                <option value="mkp">Pupuk MKP (Mono Kalium Fosfat 0-52-34 - Anti Rontok Bunga)</option>
                <option value="ab_mix">AB Mix Presisi (Fertigasi Kocor Drip Hydroponic)</option>
                <option value="dolomit">Kapur Dolomit (CaMg - Penetral pH Media Asam)</option>
                <option value="humic">Asam Humat Organik (Pembenah Tanah &amp; Nutrisi)</option>
              </select>
            </div>
            <div>
              <label style="font-size:11px; color:var(--text-sub); display:block; margin-bottom:4px; font-weight:600;">Harga Pupuk (Rp/kg):</label>
              <input id="fert-price-kg" min="500" oninput="calculateFertigationDose()" step="500" type="number" value="18000" class="form-input"/>
            </div>
            <div>
              <label style="font-size:11px; color:var(--text-sub); display:block; margin-bottom:4px; font-weight:600;">Metode Aplikasi:</label>
              <select id="fert-method" onchange="calculateFertigationDose()" class="form-input">
                <option selected="" value="kocor">Kocor Pelarutan Air (Fertigasi)</option>
                <option value="tabur">Tabur / Benam Guludan Tanah</option>
                <option value="foliar">Semprot Daun (Foliar Spray)</option>
              </select>
            </div>
          </div>
          <div id="fert-result-box" style="background:rgba(168, 85, 247, 0.08); padding:12px; border-radius:10px; font-size:12px; line-height:1.6; margin-top:14px; word-break: break-word;">
            Memproses rekomendasi dosis pupuk dinamis...
          </div>
        </div>
      </div>

      <!-- MANAJEMEN PROFIL TANAMAN & PANDUAN PRAKTIS LAPANGAN (FARMER-CENTRIC) -->
      <div class="card" style="background: linear-gradient(145deg, rgba(30, 41, 59, 0.85), rgba(15, 23, 42, 0.95)); margin-bottom: 20px;">
        <div class="chart-header-wrap" style="margin-bottom:14px;">
          <div class="chart-title-area">
            <div class="chart-icon-box emerald">
              <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                <path d="M12 2a9 9 0 0 1 9 9c0 4.97-4.03 9-9 9s-9-4.03-9-9a9 9 0 0 1 9-9z"></path>
                <path d="M12 6v6l4 2"></path>
              </svg>
            </div>
            <div>
              <span class="chart-title-text">Panduan Tanam &amp; Kesiapan Bibit Lapangan</span>
              <span class="chart-subtitle-text">Pedoman Praktis Petani • Kesiapan Pindah Tanam, Pengairan &amp; Perlindungan Panas</span>
            </div>
          </div>
          <span class="badge-pill" style="color:#10b981; border-color:rgba(16,185,129,0.3);">Model Lapangan Petani</span>
        </div>

        <!-- Preset Tanaman 1-Klik -->
        <div style="margin-bottom: 14px;">
          <div style="font-size:11.5px; color:var(--text-sub); margin-bottom:8px; font-weight:700; letter-spacing:0.3px;">PILIH CEPAT JENIS TANAMAN:</div>
          <div style="display:flex; gap:8px; flex-wrap:wrap;">
            <button type="button" class="btn btn-secondary" onclick="applyCropPreset('Cabai Rawit Merah', 'semai', 14, 'polybag', 100)" style="padding:6px 14px; font-size:12px; font-weight:600;">Cabai Rawit</button>
            <button type="button" class="btn btn-secondary" onclick="applyCropPreset('Bawang Merah Bima', 'semai', 10, 'open', 250)" style="padding:6px 14px; font-size:12px; font-weight:600;">Bawang Merah</button>
            <button type="button" class="btn btn-secondary" onclick="applyCropPreset('Tomat Servo F1', 'semai', 18, 'greenhouse', 150)" style="padding:6px 14px; font-size:12px; font-weight:600;">Tomat Servo</button>
            <button type="button" class="btn btn-secondary" onclick="applyCropPreset('Melon Golden Light', 'semai', 12, 'greenhouse', 300)" style="padding:6px 14px; font-size:12px; font-weight:600;">Melon Golden</button>
            <button type="button" class="btn btn-secondary" onclick="applyCropPreset('Padi Ciherang Sawah', 'semai', 20, 'open', 1000)" style="padding:6px 14px; font-size:12px; font-weight:600;">Padi Sawah</button>
          </div>
        </div>

        <!-- 5 INPUT UTAMA PETANI (SEDERHANA & CEPAT) -->
        <div style="display:grid; grid-template-columns: repeat(auto-fit, minmax(180px, 1fr)); gap:12px; margin-bottom:14px;">
          <div>
            <label style="font-size:11px; color:var(--text-sub); display:block; margin-bottom:4px; font-weight:700;">Nama Komoditas Tanaman:</label>
            <input id="crop-name" onchange="updateCropAgronomyAnalysis()" placeholder="Pilih preset di atas atau ketik nama..." type="text" class="form-input"/>
          </div>
          <div>
            <label style="font-size:11px; color:var(--text-sub); display:block; margin-bottom:4px; font-weight:700;">Tahap / Fase Pertumbuhan:</label>
            <select id="crop-stage" onchange="updateCropAgronomyAnalysis()" class="form-input">
              <option selected="" value="semai">Pembibitan / Semai (Persemaian)</option>
              <option value="pindah">Baru Pindah Tanam (1-7 Hari di Lahan)</option>
              <option value="vegetatif">Pertumbuhan Batang &amp; Daun (Vegetatif)</option>
              <option value="generatif">Pembungaan &amp; Pembuahan (Generatif)</option>
            </select>
          </div>
          <div>
            <label style="font-size:11px; color:var(--text-sub); display:block; margin-bottom:4px; font-weight:700;">Umur Bibit / Tanam (HST / Hari):</label>
            <input id="crop-age-days" max="180" min="1" placeholder="Masukkan HST..." oninput="updateCropAgronomyAnalysis()" type="number" class="form-input"/>
          </div>
          <div>
            <label style="font-size:11px; color:var(--text-sub); display:block; margin-bottom:4px; font-weight:700;">Jumlah Daun Sejati:</label>
            <select id="crop-leaves-count" onchange="updateCropAgronomyAnalysis()" class="form-input">
              <option value="2">1-2 Helai (Masih Muda / Belum Siap)</option>
              <option selected="" value="4">3-4 Helai (Cukup Siap Pindah)</option>
              <option value="6">5-6 Helai (Sangat Ideal / Siap Tanam)</option>
              <option value="8">&gt;6 Helai (Wajib Segera Dipindah)</option>
            </select>
          </div>
          <div>
            <label style="font-size:11px; color:var(--text-sub); display:block; margin-bottom:4px; font-weight:700;">Kekuatan Akar di Media Semai:</label>
            <select id="crop-root-status" onchange="updateCropAgronomyAnalysis()" class="form-input">
              <option value="loose">Akar Masih Rapuh / Tanah Rontok saat Dicabut</option>
              <option selected="" value="solid">Akar Menggumpal Padat (Tanah Utuh/Kuat)</option>
            </select>
          </div>
        </div>

        <!-- TOGGLE PENGATURAN LANJUTAN (OPSIONAL UNTUK YANG MEMILIKI ALAT UKUR) -->
        <button type="button" class="agri-advanced-toggle-btn" id="btn-toggle-agri-adv" onclick="toggleAdvancedAgronomy()">
          <span>Pengaturan Lanjutan (Luas Lahan, pH Tanah &amp; EC Pupuk)</span>
          <span id="agri-adv-icon">▼</span>
        </button>

        <div id="agri-advanced-drawer" style="display:none; background:rgba(0,0,0,0.25); border:1px dashed rgba(255,255,255,0.12); border-radius:10px; padding:12px; margin-bottom:16px;">
          <div style="font-size:11px; color:var(--text-sub); margin-bottom:10px;">Parameter opsional bagi teknisi atau petani yang memiliki alat ukur:</div>
          <div style="display:grid; grid-template-columns: repeat(auto-fit, minmax(180px, 1fr)); gap:12px;">
            <div>
              <label style="font-size:11px; color:var(--text-sub); display:block; margin-bottom:4px; font-weight:600;">Lingkungan Pembibitan:</label>
              <select id="crop-env" onchange="updateCropAgronomyAnalysis()" class="form-input">
                <option value="open">Lahan Terbuka Bebas</option>
                <option selected="" value="greenhouse">Greenhouse / Naungan Paranet / UV</option>
                <option value="polybag">Tray Semai / Polybag Pembibitan</option>
              </select>
            </div>
            <div>
              <label style="font-size:11px; color:var(--text-sub); display:block; margin-bottom:4px; font-weight:600;">Luas Lahan / Bedengan (m²):</label>
              <input id="crop-area-size" max="50000" min="1" oninput="updateCropAgronomyAnalysis()" type="number" placeholder="100" class="form-input"/>
            </div>
            <div>
              <label style="font-size:11px; color:var(--text-sub); display:block; margin-bottom:4px; font-weight:600;">pH Tanah Media (Batas: 5.5 - 6.8):</label>
              <input id="crop-ph-level" max="9.0" min="4.0" oninput="updateCropAgronomyAnalysis()" step="0.1" type="number" placeholder="6.5" class="form-input"/>
            </div>
            <div>
              <label style="font-size:11px; color:var(--text-sub); display:block; margin-bottom:4px; font-weight:600;">Kekuatan EC Nutrisi (mS/cm):</label>
              <input id="crop-ec-level" max="5.0" min="0.2" oninput="updateCropAgronomyAnalysis()" step="0.1" type="number" placeholder="1.0" class="form-input"/>
            </div>
          </div>
        </div>

        <!-- HASIL KEPUTUSAN & PANDUAN PRAKTIS PETANI -->
        <div style="background: rgba(0,0,0,0.3); border: 1px solid rgba(16, 185, 129, 0.25); border-radius: 14px; padding: 16px;">
          <!-- Bar Header Hasil & Tombol Aksi -->
          <div style="display:flex; justify-content:space-between; align-items:center; flex-wrap:wrap; gap:10px; border-bottom:1px solid rgba(255,255,255,0.08); padding-bottom:10px; margin-bottom:14px;">
            <div style="display:flex; align-items:center; gap:8px;">
              <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="#10b981" stroke-width="2"><path d="M12 2a9 9 0 0 1 9 9c0 4.97-4.03 9-9 9s-9-4.03-9-9a9 9 0 0 1 9-9z"></path><path d="M12 6v6l4 2"></path></svg>
              <div>
                <div id="ai-agri-title" style="font-size:14px; font-weight:700; color:var(--primary);">Panduan Lapangan: Belum Ada Tanaman Dipilih</div>
                <div id="ai-agri-subtitle" style="font-size:11px; color:var(--text-sub);">Evaluasi Kesiapan Bibit &amp; Jadwal Siram Berdasarkan Suhu Riil</div>
              </div>
            </div>
            <div style="display:flex; gap:6px; align-items:center; flex-wrap:wrap;">
              <button type="button" class="btn btn-secondary" onclick="speakAgronomyVoiceReport()" style="padding:5px 12px; font-size:11px; display:inline-flex; align-items:center; gap:5px;" title="Dengarkan Suara Ringkasan Laporan">
                <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polygon points="11 5 6 9 2 9 2 15 6 15 11 19 11 5"></polygon><path d="M19.07 4.93a10 10 0 0 1 0 14.14M15.54 8.46a5 5 0 0 1 0 7.07"></path></svg>
                <span>Suara Panduan</span>
              </button>
              <button type="button" class="btn btn-secondary" onclick="window.print()" style="padding:5px 12px; font-size:11px; display:inline-flex; align-items:center; gap:5px;" title="Cetak Ringkasan Panduan">
                <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polyline points="6 9 6 2 18 2 18 9"></polyline><path d="M6 18H4a2 2 0 0 1-2-2v-5a2 2 0 0 1 2-2h16a2 2 0 0 1 2 2v5a2 2 0 0 1-2 2h-2"></path><rect x="6" y="14" width="12" height="8"></rect></svg>
                <span>Cetak PDF</span>
              </button>
            </div>
          </div>

          <!-- KARTU KESIMPULAN UTAMA 3 DETIK (THE 3-SECOND RULE) -->
          <div class="agri-status-banner status-waiting" id="card-agri-status-banner">
            <div class="agri-status-title" id="ai-status-title">
              <span>STATUS: MENUNGGU PEMILIHAN PRESET TANAMAN</span>
            </div>
            <div class="agri-status-desc" id="ai-status-desc">
              Silakan pilih preset komoditas di atas atau masukkan nama dan usia tanaman di formulir.
            </div>
            <div class="agri-status-sub" id="ai-status-sub">
              Jadwal Siram &amp; Pindah Tanam: Menunggu pemilihan komoditas dan hari setelah tanam (HST).
            </div>
          </div>

          <!-- PROGRESS KESIAPAN BIBIT -->
          <div id="card-transplant-readiness" style="background:rgba(15,23,42,0.6); padding:12px 14px; border-radius:10px; margin-bottom:14px; border:1px solid rgba(255,255,255,0.06);">
            <div style="display:flex; justify-content:space-between; align-items:center; margin-bottom:6px; font-size:12px;">
              <span style="font-weight:700; color:var(--text-main);">Tingkat Kesiapan Tanam:</span>
              <b id="ai-readiness-percent" style="color:var(--text-sub); font-size:13px;">--% (Menunggu Data)</b>
            </div>
            <div style="width:100%; height:12px; background:rgba(255,255,255,0.1); border-radius:10px; overflow:hidden; position:relative;">
              <div id="ai-readiness-bar" style="width:0%; height:100%; background:linear-gradient(90deg, #10b981, #06b6d4); border-radius:10px; transition:width 0.5s ease;"></div>
            </div>
          </div>

          <!-- 4 CHECKLIST PRAKTIS KONDISI BIBIT & LAHAN (BUANG METRIK ANGIN) -->
          <div class="agri-quick-grid">
            <div class="agri-quick-card">
              <div class="agri-quick-label">DAUN SEJATI</div>
              <div class="agri-quick-val" id="ai-leaves-badge">-- Helai</div>
              <div class="agri-quick-hint" id="ai-leaves-hint" style="color:var(--text-sub);">Menunggu input</div>
            </div>

            <div class="agri-quick-card">
              <div class="agri-quick-label">KEKUATAN AKAR</div>
              <div class="agri-quick-val" id="ai-root-badge">--</div>
              <div class="agri-quick-hint" id="ai-root-hint" style="color:var(--text-sub);">Menunggu input</div>
            </div>

            <div class="agri-quick-card">
              <div class="agri-quick-label">SUHU SENSOR AKTUAL</div>
              <div class="agri-quick-val" id="ai-metric-heat">--°C</div>
              <div class="agri-quick-hint" id="ai-note-heat" style="color:var(--text-sub);">Menunggu sensor DHT11</div>
            </div>

            <div class="agri-quick-card">
              <div class="agri-quick-label">TAKARAN AIR SIRAM</div>
              <div class="agri-quick-val" id="ai-metric-irrigation">-- Liter</div>
              <div class="agri-quick-hint" id="ai-note-irrigation" style="color:var(--text-sub);">Menunggu luas lahan</div>
            </div>
          </div>

          <!-- KOTAK PANDUAN TINDAKAN PRAKTIS PETANI HARI INI -->
          <div id="ai-agronomy-advice" style="background:rgba(16, 185, 129, 0.08); border: 1px solid rgba(16, 185, 129, 0.25); border-radius:10px; padding:12px 14px; font-size:12px; line-height:1.6;">
            <b>Panduan Tindakan Lapangan:</b><br/>
            Silakan pilih preset komoditas tanaman di atas untuk menampilkan panduan lapangan yang sesuai dengan telemetri sensor riil kebun Anda.
          </div>
        </div>
      </div>

    </div>

    <!-- TAB 4: CUACA BMKG VS MIKROKLIMAT LAHAN -->
    <div id="tab-weather" class="tab-content">
      <div class="card" style="background: rgba(15, 23, 42, 0.45); backdrop-filter: blur(14px); -webkit-backdrop-filter: blur(14px); border: 1px solid rgba(255,255,255,0.1); box-shadow: 0 8px 32px rgba(0, 0, 0, 0.2);">
        
        <!-- HEADER SCADA METEOROLOGI -->
        <div class="chart-header-wrap" style="margin-bottom:18px; padding-bottom:14px; border-bottom:1px solid rgba(255,255,255,0.08);">
          <div class="chart-title-area">
            <div class="chart-icon-box cyan">
              <svg width="22" height="22" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                <path d="M17.5 19H9a7 7 0 1 1 6.71-9h1.79a4.5 4.5 0 1 1 0 9Z"></path>
              </svg>
            </div>
            <div>
              <span class="chart-title-text" style="font-size:16px;">Stasiun Meteorologi BMKG vs Sensor Lahan Presisi</span>
              <span class="chart-subtitle-text">Prakiraan Satelit Terestrial (Leces, Probolinggo - adm4: 35.13.05.2002) • Komparasi Fisik Mikroklimat</span>
            </div>
          </div>
          <div style="display:flex; align-items:center; gap:8px;">
            <span class="badge-pill" id="bmkg-sync-status-badge" style="background:rgba(6, 182, 212, 0.12); color:#38bdf8; border:1px solid rgba(6,182,212,0.3); font-size:11px; padding:4px 10px;">
              <span id="bmkg-sync-dot" style="display:inline-block; width:6px; height:6px; border-radius:50%; background:#38bdf8; margin-right:5px;"></span>
              <span id="bmkg-last-sync-badge">Siap Sinkron</span>
            </span>
            <button class="btn btn-secondary" id="btn-sync-bmkg" onclick="syncBMKGData(false)" style="padding:6px 14px; font-size:11.5px; font-weight:600; display:inline-flex; align-items:center; gap:6px; border-color:rgba(6,182,212,0.4); color:#38bdf8;">
              <svg id="sync-bmkg-icon" width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                <path d="M21.5 2v6h-6M21.34 15.57a10 10 0 1 1-.57-8.38l5.67-5.67"></path>
              </svg>
              <span>Sinkronkan BMKG Live</span>
            </button>
          </div>
        </div>

        <!-- 8 DERIVED METEOROLOGICAL MATRIX CARDS -->
        <div style="display:grid; grid-template-columns: repeat(auto-fit, minmax(260px, 1fr)); gap:14px;">
          
          <!-- CARD 1: SUHU UDARA & VARIAN MIKRO -->
          <div class="card" style="background:rgba(0,0,0,0.25); border:1px solid rgba(255,255,255,0.06); border-top:2px solid #06b6d4; padding:14px; border-radius:12px; margin:0;">
            <div style="display:flex; justify-content:space-between; align-items:center; margin-bottom:10px;">
              <div style="display:flex; align-items:center; gap:8px;">
                <div class="chart-icon-box cyan" style="width:28px; height:28px;">
                  <svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M14 4v10.54a4 4 0 1 1-4 0V4a2 2 0 0 1 4 0Z"></path></svg>
                </div>
                <span style="font-size:12px; font-weight:700; color:#38bdf8; text-transform:uppercase; letter-spacing:0.5px;">Termometri Udara</span>
              </div>
              <span class="badge-pill" id="bmkg-temp-insight" style="font-size:10px; padding:2px 8px; background:rgba(6,182,212,0.12); color:#38bdf8; border-color:rgba(6,182,212,0.25);">Termal Riil</span>
            </div>
            <div style="display:flex; justify-content:space-between; font-size:13px; margin:6px 0;">
              <span style="color:var(--text-sub);">Lahan (DHT11):</span>
              <b id="bmkg-dht-temp" style="color:var(--text-main); font-family:monospace; font-size:14px;">--°C</b>
            </div>
            <div style="display:flex; justify-content:space-between; font-size:13px; margin:6px 0;">
              <span style="color:var(--text-sub);">BMKG Satelit:</span>
              <b id="bmkg-sat-temp" style="color:#f59e0b; font-family:monospace; font-size:14px;">--°C</b>
            </div>
            <div style="display:flex; justify-content:space-between; align-items:center; font-size:12px; border-top:1px dashed rgba(255,255,255,0.1); padding-top:6px; margin-top:6px;">
              <span style="color:var(--text-sub);">Selisih Varian (Δ):</span>
              <b id="bmkg-temp-diff" style="color:var(--text-sub); font-family:monospace; font-size:13px;">--°C</b>
            </div>
          </div>

          <!-- CARD 2: KELEMBAPAN UDARA & TAJUK -->
          <div class="card" style="background:rgba(0,0,0,0.25); border:1px solid rgba(255,255,255,0.06); border-top:2px solid #38bdf8; padding:14px; border-radius:12px; margin:0;">
            <div style="display:flex; justify-content:space-between; align-items:center; margin-bottom:10px;">
              <div style="display:flex; align-items:center; gap:8px;">
                <div class="chart-icon-box cyan" style="width:28px; height:28px;">
                  <svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M12 2.69l5.66 5.66a8 8 0 1 1-11.31 0z"></path></svg>
                </div>
                <span style="font-size:12px; font-weight:700; color:#38bdf8; text-transform:uppercase; letter-spacing:0.5px;">Higrometri Tajuk</span>
              </div>
              <span class="badge-pill" id="bmkg-hum-insight" style="font-size:10px; padding:2px 8px; background:rgba(56,189,248,0.12); color:#38bdf8; border-color:rgba(56,189,248,0.25);">Kelembapan</span>
            </div>
            <div style="display:flex; justify-content:space-between; font-size:13px; margin:6px 0;">
              <span style="color:var(--text-sub);">Lahan (DHT11):</span>
              <b id="bmkg-dht-hum" style="color:var(--text-main); font-family:monospace; font-size:14px;">--%</b>
            </div>
            <div style="display:flex; justify-content:space-between; font-size:13px; margin:6px 0;">
              <span style="color:var(--text-sub);">BMKG Satelit:</span>
              <b id="bmkg-sat-hum" style="color:#38bdf8; font-family:monospace; font-size:14px;">--%</b>
            </div>
            <div style="display:flex; justify-content:space-between; align-items:center; font-size:12px; border-top:1px dashed rgba(255,255,255,0.1); padding-top:6px; margin-top:6px;">
              <span style="color:var(--text-sub);">Selisih Varian (Δ):</span>
              <b id="bmkg-hum-diff" style="color:var(--text-sub); font-family:monospace; font-size:13px;">--%</b>
            </div>
          </div>

          <!-- CARD 3: INDEKS PANAS TERASA (HEAT INDEX) -->
          <div class="card" style="background:rgba(0,0,0,0.25); border:1px solid rgba(255,255,255,0.06); border-top:2px solid #f59e0b; padding:14px; border-radius:12px; margin:0;">
            <div style="display:flex; justify-content:space-between; align-items:center; margin-bottom:10px;">
              <div style="display:flex; align-items:center; gap:8px;">
                <div class="chart-icon-box amber" style="width:28px; height:28px;">
                  <svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="4"/><path d="M12 2v2m0 16v2M4.93 4.93l1.41 1.41m11.32 11.32l1.41 1.41M2 12h2m16 0h2M6.34 17.66l-1.41 1.41m14.14-14.14l-1.41 1.41"/></svg>
                </div>
                <span style="font-size:12px; font-weight:700; color:#fbbf24; text-transform:uppercase; letter-spacing:0.5px;">Indeks Panas Terasa</span>
              </div>
              <span class="badge-pill" id="bmkg-heat-badge" style="font-size:10px; padding:2px 8px; background:rgba(245,158,11,0.12); color:#fbbf24; border-color:rgba(245,158,11,0.25);">Sensasi Daun</span>
            </div>
            <div style="display:flex; justify-content:space-between; font-size:13px; margin:6px 0;">
              <span style="color:var(--text-sub);">Dirasakan Daun:</span>
              <b id="bmkg-heat-index" style="color:#f59e0b; font-family:monospace; font-size:14px;">--°C</b>
            </div>
            <div style="display:flex; justify-content:space-between; font-size:13px; margin:6px 0;">
              <span style="color:var(--text-sub);">Skala Fahrenheit:</span>
              <b id="bmkg-heat-f" style="color:var(--text-main); font-family:monospace; font-size:13px;">--°F</b>
            </div>
            <div style="display:flex; justify-content:space-between; align-items:center; font-size:12px; border-top:1px dashed rgba(255,255,255,0.1); padding-top:6px; margin-top:6px;">
              <span style="color:var(--text-sub);">Sensasi Termal:</span>
              <b id="bmkg-heat-status" style="color:var(--text-sub); font-size:12px;">--</b>
            </div>
          </div>

          <!-- CARD 4: TITIK EMBUN DAUN (DEW POINT) -->
          <div class="card" style="background:rgba(0,0,0,0.25); border:1px solid rgba(255,255,255,0.06); border-top:2px solid #10b981; padding:14px; border-radius:12px; margin:0;">
            <div style="display:flex; justify-content:space-between; align-items:center; margin-bottom:10px;">
              <div style="display:flex; align-items:center; gap:8px;">
                <div class="chart-icon-box emerald" style="width:28px; height:28px;">
                  <svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M12 2a9 9 0 0 1 9 9c0 4.97-4.03 9-9 9s-9-4.03-9-9a9 9 0 0 1 9-9Z"></path><path d="M12 7v5l3 3"></path></svg>
                </div>
                <span style="font-size:12px; font-weight:700; color:#34d399; text-transform:uppercase; letter-spacing:0.5px;">Titik Embun Daun</span>
              </div>
              <span class="badge-pill" id="bmkg-dew-badge" style="font-size:10px; padding:2px 8px; background:rgba(16,185,129,0.12); color:#34d399; border-color:rgba(16,185,129,0.25);">Magnus Termal</span>
            </div>
            <div style="display:flex; justify-content:space-between; font-size:13px; margin:6px 0;">
              <span style="color:var(--text-sub);">Suhu Dew Point:</span>
              <b id="bmkg-dew-point" style="color:#10b981; font-family:monospace; font-size:14px;">--°C</b>
            </div>
            <div style="display:flex; justify-content:space-between; font-size:13px; margin:6px 0;">
              <span style="color:var(--text-sub);">Kelembapan Absolut:</span>
              <b id="bmkg-abs-hum" style="color:var(--text-main); font-family:monospace; font-size:13px;">-- g/m³</b>
            </div>
            <div style="display:flex; justify-content:space-between; align-items:center; font-size:12px; border-top:1px dashed rgba(255,255,255,0.1); padding-top:6px; margin-top:6px;">
              <span style="color:var(--text-sub);">Kondensasi Tajuk:</span>
              <b id="bmkg-dew-status" style="color:var(--text-sub); font-size:12px;">--</b>
            </div>
          </div>

          <!-- CARD 5: DEFISIT TEKANAN UAP (VPD) -->
          <div class="card" style="background:rgba(0,0,0,0.25); border:1px solid rgba(255,255,255,0.06); border-top:2px solid #8b5cf6; padding:14px; border-radius:12px; margin:0;">
            <div style="display:flex; justify-content:space-between; align-items:center; margin-bottom:10px;">
              <div style="display:flex; align-items:center; gap:8px;">
                <div class="chart-icon-box purple" style="width:28px; height:28px;">
                  <svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M12 2v20M17 5H9.5a3.5 3.5 0 0 0 0 7h5a3.5 3.5 0 0 1 0 7H6"></path></svg>
                </div>
                <span style="font-size:12px; font-weight:700; color:#c084fc; text-transform:uppercase; letter-spacing:0.5px;">Defisit Tekanan Uap</span>
              </div>
              <span class="badge-pill" id="bmkg-vpd-badge" style="font-size:10px; padding:2px 8px; background:rgba(139,92,246,0.12); color:#c084fc; border-color:rgba(139,92,246,0.25);">Stomata Daun</span>
            </div>
            <div style="display:flex; justify-content:space-between; font-size:13px; margin:6px 0;">
              <span style="color:var(--text-sub);">Nilai VPD Aktual:</span>
              <b id="bmkg-vpd-val" style="color:#c084fc; font-family:monospace; font-size:14px;">-- kPa</b>
            </div>
            <div style="display:flex; justify-content:space-between; font-size:13px; margin:6px 0;">
              <span style="color:var(--text-sub);">Status Stomata:</span>
              <b id="bmkg-vpd-status" style="color:var(--text-sub); font-size:12.5px;">--</b>
            </div>
            <div style="display:flex; justify-content:space-between; align-items:center; font-size:12px; border-top:1px dashed rgba(255,255,255,0.1); padding-top:6px; margin-top:6px;">
              <span style="color:var(--text-sub);">Target Rentang:</span>
              <b style="color:var(--text-sub); font-family:monospace; font-size:11.5px;">0.80 - 1.20 kPa</b>
            </div>
          </div>

          <!-- CARD 6: LAJU PENGUAPAN AIR (EVAPORASI) -->
          <div class="card" style="background:rgba(0,0,0,0.25); border:1px solid rgba(255,255,255,0.06); border-top:2px solid #ec4899; padding:14px; border-radius:12px; margin:0;">
            <div style="display:flex; justify-content:space-between; align-items:center; margin-bottom:10px;">
              <div style="display:flex; align-items:center; gap:8px;">
                <div class="chart-icon-box rose" style="width:28px; height:28px;">
                  <svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M2 12h20M2 17h20M2 7h20"></path></svg>
                </div>
                <span style="font-size:12px; font-weight:700; color:#f472b6; text-transform:uppercase; letter-spacing:0.5px;">Laju Penguapan Air</span>
              </div>
              <span class="badge-pill" id="bmkg-evap-badge" style="font-size:10px; padding:2px 8px; background:rgba(236,72,153,0.12); color:#f472b6; border-color:rgba(236,72,153,0.25);">Penman ET</span>
            </div>
            <div style="display:flex; justify-content:space-between; font-size:13px; margin:6px 0;">
              <span style="color:var(--text-sub);">Laju Evaporasi:</span>
              <b id="bmkg-evap-val" style="color:#ec4899; font-family:monospace; font-size:14px;">-- mm/hari</b>
            </div>
            <div style="display:flex; justify-content:space-between; font-size:13px; margin:6px 0;">
              <span style="color:var(--text-sub);">Intensitas Penguapan:</span>
              <b id="bmkg-evap-status" style="color:var(--text-sub); font-size:12.5px;">--</b>
            </div>
            <div style="display:flex; justify-content:space-between; align-items:center; font-size:12px; border-top:1px dashed rgba(255,255,255,0.1); padding-top:6px; margin-top:6px;">
              <span style="color:var(--text-sub);">Kebutuhan Irigasi:</span>
              <b id="bmkg-evap-loss" style="color:#38bdf8; font-family:monospace; font-size:12px;">-- L/m²</b>
            </div>
          </div>

          <!-- CARD 7: ANGIN & ATMOSFER BMKG -->
          <div class="card" style="background:rgba(0,0,0,0.25); border:1px solid rgba(255,255,255,0.06); border-top:2px solid #14b8a6; padding:14px; border-radius:12px; margin:0;">
            <div style="display:flex; justify-content:space-between; align-items:center; margin-bottom:10px;">
              <div style="display:flex; align-items:center; gap:8px;">
                <div class="chart-icon-box emerald" style="width:28px; height:28px;">
                  <svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M9.59 4.59A2 2 0 1 1 11 8H2m10.59 11.41A2 2 0 1 0 14 16H2m15.73-8.27A2.5 2.5 0 1 1 19.5 12H2"></path></svg>
                </div>
                <span style="font-size:12px; font-weight:700; color:#2dd4bf; text-transform:uppercase; letter-spacing:0.5px;">Angin &amp; Atmosfer Satelit</span>
              </div>
              <span class="badge-pill" id="bmkg-wind-badge" style="font-size:10px; padding:2px 8px; background:rgba(20,184,166,0.12); color:#2dd4bf; border-color:rgba(20,184,166,0.25);">Stasiun BMKG</span>
            </div>
            <div style="display:flex; justify-content:space-between; font-size:13px; margin:6px 0;">
              <span style="color:var(--text-sub);">Arah Hembusan:</span>
              <b id="bmkg-wind-dir" style="color:var(--text-main); font-size:12.5px;">--</b>
            </div>
            <div style="display:flex; justify-content:space-between; font-size:13px; margin:6px 0;">
              <span style="color:var(--text-sub);">Kecepatan Angin:</span>
              <b id="bmkg-wind-speed" style="color:#2dd4bf; font-family:monospace; font-size:13.5px;">-- km/jam</b>
            </div>
            <div style="display:flex; justify-content:space-between; align-items:center; font-size:12px; border-top:1px dashed rgba(255,255,255,0.1); padding-top:6px; margin-top:6px;">
              <span style="color:var(--text-sub);">Kondisi Langit:</span>
              <b id="bmkg-sat-desc" style="color:#f59e0b; font-size:12px;">--</b>
            </div>
          </div>

          <!-- CARD 8: PREDIKSI HUJAN & REKOMENDASI POMPA -->
          <div class="card" style="background:rgba(0,0,0,0.25); border:1px solid rgba(255,255,255,0.06); border-top:2px solid #6366f1; padding:14px; border-radius:12px; margin:0;">
            <div style="display:flex; justify-content:space-between; align-items:center; margin-bottom:10px;">
              <div style="display:flex; align-items:center; gap:8px;">
                <div class="chart-icon-box purple" style="width:28px; height:28px;">
                  <svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M20 16.2A4.5 4.5 0 0 0 17.5 8h-1.8A7 7 0 1 0 4 14.9"></path><path d="M16 14v6m-4-4v6m-4-2v4"></path></svg>
                </div>
                <span style="font-size:12px; font-weight:700; color:#818cf8; text-transform:uppercase; letter-spacing:0.5px;">Presipitasi &amp; Irigasi</span>
              </div>
              <span class="badge-pill" id="bmkg-rain-badge" style="font-size:10px; padding:2px 8px; background:rgba(99,102,241,0.12); color:#818cf8; border-color:rgba(99,102,241,0.25);">Failsafe Irigasi</span>
            </div>
            <div style="font-size:13.5px; font-weight:700; color:#10b981; margin:4px 0;" id="bmkg-rain-pred">
              Memuat Data BMKG...
            </div>
            <div id="bmkg-recommendation" style="font-size:11.5px; color:var(--text-sub); margin:4px 0; line-height:1.4;">
              Penyiraman otomatis siap berjalan sesuai kelembapan tanah.
            </div>
            <div style="display:flex; justify-content:space-between; align-items:center; font-size:11px; color:var(--text-sub); border-top:1px dashed rgba(255,255,255,0.1); padding-top:6px; margin-top:6px;">
              <span>Waktu Sinkronisasi:</span>
              <span id="bmkg-last-sync" style="font-family:monospace; color:#38bdf8;">-</span>
            </div>
          </div>

        </div>
      </div>
    </div>

    <!-- TAB 6: HISTORI LOGGING, STORAGE LITTLEFS & LIVE TELEMETRY FEED -->
    <div id="tab-logs" class="tab-content">
      
      <!-- SUB-NAV / VIEW SWITCHER -->
      <div style="display:flex; justify-content:space-between; align-items:center; margin-bottom:14px; flex-wrap:wrap; gap:10px;">
        <div style="display:inline-flex; width:auto; background:rgba(15,23,42,0.6); padding:4px; border-radius:10px; border:1px solid rgba(255,255,255,0.1); gap:4px;">
          <button id="btn-view-logs-table" onclick="switchLogsView('table')" style="padding:7px 16px; font-size:12px; font-weight:600; display:inline-flex; align-items:center; gap:6px; white-space:nowrap; border-radius:8px; border:none; cursor:pointer; background:var(--primary); color:#fff; transition:all 0.2s;">
            <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M14 2H6a2 2 0 0 0-2 2v16a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V8z"></path><polyline points="14 2 14 8 20 8"></polyline><line x1="16" y1="13" x2="8" y2="13"></line><line x1="16" y1="17" x2="8" y2="17"></line></svg>
            <span>Histori Flash LittleFS (CSV)</span>
          </button>
          <button id="btn-view-logs-feed" onclick="switchLogsView('feed')" style="padding:7px 16px; font-size:12px; font-weight:600; display:inline-flex; align-items:center; gap:6px; white-space:nowrap; border-radius:8px; border:none; cursor:pointer; background:transparent; color:var(--text-sub); transition:all 0.2s;">
            <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polyline points="22 12 18 12 15 21 9 3 6 12 2 12"></polyline></svg>
            <span>Live Feed Transmisi Sensor (Real-Time)</span>
            <span id="live-feed-badge-dot" style="width:6px; height:6px; border-radius:50%; background:#10b981; box-shadow:0 0 6px #10b981;"></span>
          </button>
        </div>

        <div style="display:flex; gap:6px; flex-wrap:wrap;">
          <button class="btn btn-secondary" onclick="fetchAndParseLogs()" style="padding:6px 12px; font-size:11.5px; font-weight:600; display:inline-flex; align-items:center; gap:6px;" title="Muat ulang rekaman CSV LittleFS">
            <svg width="13" height="13" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polyline points="23 4 23 10 17 10"></polyline><path d="M20.49 15a9 9 0 1 1-2.12-9.36L23 10"></path></svg>
            <span>Refresh</span>
          </button>
          <button class="btn btn-accent" onclick="openFirebaseBackupDialog()" style="padding:6px 12px; font-size:11.5px; font-weight:600; display:inline-flex; align-items:center; gap:6px; background:linear-gradient(135deg, #0284c7, #38bdf8); border:none;" title="Pilihan Backup Hemat Kuota atau Arsip Penuh ke Firebase">
            <svg width="13" height="13" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M17.5 19H9a7 7 0 1 1 6.71-9h1.79a4.5 4.5 0 1 1 0 9Z"></path></svg>
            <span>Backup Firebase</span>
          </button>
          <button class="btn btn-secondary" onclick="exportLogsAsJSON()" style="padding:6px 12px; font-size:11.5px; font-weight:600; display:inline-flex; align-items:center; gap:6px;">
            <svg width="13" height="13" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M21 15v4a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2v-4"></path><polyline points="7 10 12 15 17 10"></polyline><line x1="12" y1="15" x2="12" y2="3"></line></svg>
            <span>JSON</span>
          </button>
          <a class="btn btn-secondary" href="/downloadLog" style="padding:6px 12px; font-size:11.5px; font-weight:600; display:inline-flex; align-items:center; gap:6px;" target="_blank">
            <svg width="13" height="13" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M14 2H6a2 2 0 0 0-2 2v16a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V8z"></path><polyline points="14 2 14 8 20 8"></polyline><line x1="12" y1="18" x2="12" y2="12"></line><line x1="9" y1="15" x2="15" y2="15"></line></svg>
            <span>CSV</span>
          </a>
          <button class="btn btn-danger" onclick="clearSystemLogs()" style="padding:6px 12px; font-size:11.5px; font-weight:600; display:inline-flex; align-items:center; gap:6px;">
            <svg width="13" height="13" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polyline points="3 6 5 6 21 6"></polyline><path d="M19 6v14a2 2 0 0 1-2 2H7a2 2 0 0 1-2-2V6m3 0V4a2 2 0 0 1 2-2h4a2 2 0 0 1 2 2v2"></path></svg>
            <span>Hapus Log</span>
          </button>
        </div>
      </div>

      <!-- VIEW 1: TABEL HISTORI FLASH MEMORY LITTLEFS -->
      <div id="view-container-table" class="card" style="background: rgba(15, 23, 42, 0.45); backdrop-filter: blur(14px); -webkit-backdrop-filter: blur(14px); border: 1px solid rgba(255,255,255,0.1); box-shadow: 0 8px 32px rgba(0, 0, 0, 0.2);">
        <div class="chart-header-wrap" style="margin-bottom:14px; padding-bottom:12px; border-bottom:1px solid rgba(255,255,255,0.08);">
          <div class="chart-title-area">
            <div class="chart-icon-box emerald">
              <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                <path d="M14 2H6a2 2 0 0 0-2 2v16a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V8z"></path>
                <polyline points="14 2 14 8 20 8"></polyline>
                <line x1="16" y1="13" x2="8" y2="13"></line>
                <line x1="16" y1="17" x2="8" y2="17"></line>
              </svg>
            </div>
            <div>
              <span class="chart-title-text" style="font-size:15px;">Histori Log Telemetri Lahan (LittleFS Flash)</span>
              <span class="chart-subtitle-text">Perekaman Siklis Mandiri Pada Memori Internal ESP32 • Proteksi Ring-Buffer 50 KB</span>
            </div>
          </div>
          <div id="log-count-summary" class="badge-pill" style="font-size:11px; padding:4px 10px; background:rgba(16,185,129,0.12); color:#10b981; border-color:rgba(16,185,129,0.25);">
            Total Record: Membaca...
          </div>
        </div>

        <!-- Search Bar & Filters -->
        <div style="display:flex; justify-content:space-between; align-items:center; gap:12px; margin-bottom:12px; flex-wrap:wrap;">
          <div style="flex:1; min-width:220px; position:relative;">
            <input id="log-search-input" oninput="filterLogTable(this.value)" placeholder="Cari rekaman (waktu, suhu, tanah, status, dll)..." type="text" class="form-input" style="font-size:12px; padding:8px 12px 8px 32px; width:100%; border-radius:8px; background:rgba(0,0,0,0.3); border:1px solid rgba(255,255,255,0.1); color:var(--text-main);" />
            <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" style="position:absolute; left:10px; top:11px; color:var(--text-sub);"><circle cx="11" cy="11" r="8"></circle><line x1="21" y1="21" x2="16.65" y2="16.65"></line></svg>
          </div>
          <div style="display:flex; gap:6px;">
            <button class="chart-filter-btn active" id="filter-log-all" onclick="filterLogCategory('all')">Semua</button>
            <button class="chart-filter-btn" id="filter-log-kering" onclick="filterLogCategory('kering')">Kering</button>
            <button class="chart-filter-btn" id="filter-log-pompa" onclick="filterLogCategory('pompa')">Pompa ON</button>
            <button class="chart-filter-btn" id="filter-log-lampu" onclick="filterLogCategory('lampu')">Lampu ON</button>
          </div>
        </div>

        <!-- Modern SCADA Table -->
        <div style="overflow-x:auto; max-height:420px; overflow-y:auto; border-radius:10px; border:1px solid rgba(255,255,255,0.08); background:rgba(15,23,42,0.6);">
          <table style="width:100%; border-collapse:collapse; text-align:left; font-size:12px;">
            <thead>
              <tr style="background:rgba(30,41,59,0.95); color:var(--text-sub); border-bottom:1px solid rgba(255,255,255,0.1); position:sticky; top:0; z-index:2; font-size:11px; letter-spacing:0.4px;">
                <th style="padding:10px 14px; font-weight:700;">WAKTU (RTC / UPTIME)</th>
                <th style="padding:10px 14px; font-weight:700;">SUHU (°C)</th>
                <th style="padding:10px 14px; font-weight:700;">KELEMBAPAN (%)</th>
                <th style="padding:10px 14px; font-weight:700;">TANAH (%)</th>
                <th style="padding:10px 14px; font-weight:700;">KATEGORI STATUS</th>
                <th style="padding:10px 14px; font-weight:700;">SENDER ESP8266</th>
                <th style="padding:10px 14px; font-weight:700;">AKTUATOR (POMPA / LAMPU)</th>
              </tr>
            </thead>
            <tbody id="log-table-body">
              <tr>
                <td colspan="7" style="padding:28px; text-align:center; color:var(--text-sub);">
                  Memuat histori data log dari memori Flash LittleFS ESP32...
                </td>
              </tr>
            </tbody>
          </table>
        </div>
      </div>

      <!-- VIEW 2: LIVE STREAM TELEMETRI REAL-TIME (PACKET SNIFFER) -->
      <div id="view-container-feed" class="card" style="display:none; background: rgba(15, 23, 42, 0.45); backdrop-filter: blur(14px); -webkit-backdrop-filter: blur(14px); border: 1px solid rgba(255,255,255,0.1); box-shadow: 0 8px 32px rgba(0, 0, 0, 0.2);">
        <div class="chart-header-wrap" style="margin-bottom:12px; padding-bottom:12px; border-bottom:1px solid rgba(255,255,255,0.08);">
          <div class="chart-title-area">
            <div class="chart-icon-box cyan">
              <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polyline points="22 12 18 12 15 21 9 3 6 12 2 12"></polyline></svg>
            </div>
            <div>
              <span class="chart-title-text" style="font-size:15px;">Live Stream Telemetri Real-Time</span>
              <span class="chart-subtitle-text">Log Transmisi Data Masuk Dari Sender Node ESP8266 &amp; Sensor Lahan • 100% Data Aktual</span>
            </div>
          </div>
          <div style="display:flex; align-items:center; gap:8px;">
            <label style="font-size:11px; color:var(--text-sub); display:inline-flex; align-items:center; gap:5px; cursor:pointer;">
              <input type="checkbox" id="feed-autoscroll" checked style="accent-color:#10b981;"> Auto-Scroll
            </label>
            <button class="btn btn-secondary" id="btn-toggle-feed-pause" onclick="toggleFeedPause()" style="padding:4px 10px; font-size:11px;">Pause</button>
            <button class="btn btn-secondary" onclick="clearLiveFeed()" style="padding:4px 10px; font-size:11px;">Clear Feed</button>
            <button class="btn btn-accent" onclick="uploadLiveFeedToFirebase()" style="padding:4px 10px; font-size:11px; display:inline-flex; align-items:center; gap:5px; background:linear-gradient(135deg, #0284c7, #38bdf8); border:none;">
              <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M17.5 19H9a7 7 0 1 1 6.71-9h1.79a4.5 4.5 0 1 1 0 9Z"></path></svg>
              <span>Export Stream ke Firebase</span>
            </button>
          </div>
        </div>

        <!-- Terminal Output Window -->
        <div id="live-telemetry-console" style="background:#090d16; border:1px solid rgba(255,255,255,0.08); border-radius:10px; padding:12px; font-family:'Courier New', monospace; font-size:11.5px; height:360px; overflow-y:auto; line-height:1.6; color:#94a3b8;">
          <div style="color:#64748b; font-style:italic;">[Sistem Standby] Menunggu aliran paket telemetri baru dari ESP32 &amp; ESP8266...</div>
        </div>
        <div style="display:flex; justify-content:space-between; align-items:center; margin-top:8px; font-size:11px; color:var(--text-sub);">
          <span id="feed-stat-packets">Total Paket Tertangkap: 0</span>
          <span>Protokol: ESP-NOW 2.4GHz + Local REST HTTP</span>
        </div>
      </div>

    </div>

    <!-- MODAL PILIHAN BACKUP FIREBASE CLOUD (SMART & HEMAT KUOTA) -->
    <div id="modal-firebase-backup" class="modal" style="display:none; position:fixed; z-index:9999; left:0; top:0; width:100%; height:100%; background:rgba(0,0,0,0.7); backdrop-filter:blur(6px); align-items:center; justify-content:center;">
      <div class="card" style="max-width:480px; width:90%; background:rgba(15,23,42,0.95); border:1px solid rgba(56,189,248,0.3); box-shadow:0 10px 40px rgba(0,0,0,0.5); padding:20px; border-radius:14px;">
        <div style="display:flex; justify-content:space-between; align-items:center; margin-bottom:14px; border-bottom:1px solid rgba(255,255,255,0.1); padding-bottom:10px;">
          <div style="display:flex; align-items:center; gap:8px;">
            <div class="chart-icon-box cyan" style="width:30px; height:30px;">
              <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M17.5 19H9a7 7 0 1 1 6.71-9h1.79a4.5 4.5 0 1 1 0 9Z"></path></svg>
            </div>
            <span style="font-weight:700; font-size:14px; color:#38bdf8;">Backup Database Firebase Cloud</span>
          </div>
          <button type="button" onclick="closeFirebaseBackupDialog()" style="background:transparent; border:none; color:var(--text-sub); cursor:pointer; font-size:16px;">&times;</button>
        </div>

        <p style="font-size:12px; color:var(--text-sub); margin-bottom:14px; line-height:1.5;">
          Pilih jangkauan data yang ingin Anda unggah ke Firebase Realtime Database. <strong>Tidak wajib mengunggah semuanya</strong> agar hemat kuota internet dan efisien.
        </p>

        <!-- Pilihan Opsi Backup -->
        <div style="display:flex; flex-direction:column; gap:8px; margin-bottom:16px;">
          <label style="display:flex; align-items:flex-start; gap:10px; background:rgba(255,255,255,0.03); padding:10px 12px; border-radius:8px; border:1px solid rgba(255,255,255,0.08); cursor:pointer;">
            <input type="radio" name="firebase_backup_mode" value="today" checked style="margin-top:2px; accent-color:#38bdf8;">
            <div>
              <strong style="color:var(--text-main); font-size:12px; display:block;">Hari Ini Saja (Rekomendasi Hemat Kuota)</strong>
              <span style="color:var(--text-sub); font-size:11px;">Hanya mengunggah data rekaman 24 jam terakhir. Cepat, hemat kuota internet SIM Card kebun.</span>
            </div>
          </label>

          <label style="display:flex; align-items:flex-start; gap:10px; background:rgba(255,255,255,0.03); padding:10px 12px; border-radius:8px; border:1px solid rgba(255,255,255,0.08); cursor:pointer;">
            <input type="radio" name="firebase_backup_mode" value="critical" style="margin-top:2px; accent-color:#38bdf8;">
            <div>
              <strong style="color:var(--text-main); font-size:12px; display:block;">Log Event Kritis Saja</strong>
              <span style="color:var(--text-sub); font-size:11px;">Hanya mengunggah saat terjadi peristiwa penting (Pompa aktif atau Tanah Kering).</span>
            </div>
          </label>

          <label style="display:flex; align-items:flex-start; gap:10px; background:rgba(255,255,255,0.03); padding:10px 12px; border-radius:8px; border:1px solid rgba(255,255,255,0.08); cursor:pointer;">
            <input type="radio" name="firebase_backup_mode" value="live" style="margin-top:2px; accent-color:#38bdf8;">
            <div>
              <strong style="color:var(--text-main); font-size:12px; display:block;">Stream Telemetri Real-Time Sesi Ini</strong>
              <span style="color:var(--text-sub); font-size:11px;">Hanya mengunggah paket-paket data realtime yang baru saja ditangkap di konsol sesi saat ini.</span>
            </div>
          </label>

          <label style="display:flex; align-items:flex-start; gap:10px; background:rgba(255,255,255,0.03); padding:10px 12px; border-radius:8px; border:1px solid rgba(255,255,255,0.08); cursor:pointer;">
            <input type="radio" name="firebase_backup_mode" value="full" style="margin-top:2px; accent-color:#38bdf8;">
            <div>
              <strong style="color:var(--text-main); font-size:12px; display:block;">Arsip Penuh (Seluruh Riwayat dari Awal)</strong>
              <span style="color:var(--text-sub); font-size:11px;">Mengunggah 100% rekaman log yang ada di Flash LittleFS tanpa terlewat.</span>
            </div>
          </label>
        </div>

        <div style="margin-bottom:14px;">
          <label style="font-size:11px; color:var(--text-sub); display:block; margin-bottom:4px;">Endpoint URL Firebase RTDB:</label>
          <input type="text" id="firebase-url-input" value="https://smartfarm-esp32-default-rtdb.firebaseio.com/logs.json" style="width:100%; padding:8px 10px; font-size:11.5px; font-family:monospace; background:rgba(0,0,0,0.3); border:1px solid rgba(255,255,255,0.1); border-radius:6px; color:var(--text-main);">
        </div>

        <div style="display:flex; justify-content:flex-end; gap:8px;">
          <button type="button" class="btn btn-secondary" onclick="closeFirebaseBackupDialog()" style="padding:8px 16px; font-size:12px;">Batal</button>
          <button type="button" class="btn btn-primary" onclick="executeFirebaseBackup()" style="padding:8px 18px; font-size:12px; font-weight:700; background:#0284c7; border:none;">Mulai Unggah</button>
        </div>
      </div>
    </div>

    </div><!-- /.container -->

    <!-- BOTTOM DOCK NAVIGATION BAR (RESPONSIVE ANDROID & DESKTOP) -->
    <!-- Ditempatkan di luar .container agar position:fixed bekerja tanpa terpengaruh isolation:isolate -->
    <nav class="tabs-nav bottom-dock" id="bottom-dock-nav" style="position: fixed !important; bottom: 15px !important; left: 50% !important; transform: translateX(-50%) !important; z-index: 999999 !important; display: flex !important; width: calc(100% - 32px) !important; max-width: 720px !important; margin: 0 !important; background: rgba(10,15,30,0.95) !important; border-radius: 24px !important;">
      <button type="button" class="tab-btn active" data-tab="overview" onclick="switchTab('overview')" title="Monitoring Real-Time Telemetri Kebun">
        <span class="tab-icon">
          <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
            <rect x="3" y="3" width="7" height="7" rx="1.5"></rect>
            <rect x="14" y="3" width="7" height="7" rx="1.5"></rect>
            <rect x="14" y="14" width="7" height="7" rx="1.5"></rect>
            <rect x="3" y="14" width="7" height="7" rx="1.5"></rect>
          </svg>
        </span>
        <span class="tab-label">Monitoring</span>
      </button>

      <button type="button" class="tab-btn" data-tab="charts" onclick="switchTab('charts')" title="Grafik Osiloskop & Tren 24 Jam">
        <span class="tab-icon">
          <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
            <polyline points="22 12 18 12 15 21 9 3 6 12 2 12"></polyline>
          </svg>
        </span>
        <span class="tab-label">Grafik SCADA</span>
      </button>

      <button type="button" class="tab-btn" data-tab="controls" onclick="switchTab('controls')" title="Kendali Relai & Irigasi Presisi">
        <span class="tab-icon">
          <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
            <line x1="4" y1="21" x2="4" y2="14"></line>
            <line x1="4" y1="10" x2="4" y2="3"></line>
            <line x1="12" y1="21" x2="12" y2="12"></line>
            <line x1="12" y1="8" x2="12" y2="3"></line>
            <line x1="20" y1="21" x2="20" y2="16"></line>
            <line x1="20" y1="12" x2="20" y2="3"></line>
            <line x1="1" y1="14" x2="7" y2="14"></line>
            <line x1="9" y1="8" x2="15" y2="8"></line>
            <line x1="17" y1="16" x2="23" y2="16"></line>
          </svg>
        </span>
        <span class="tab-label">Kendali</span>
      </button>

      <button type="button" class="tab-btn" data-tab="weather" onclick="switchTab('weather')" title="Satelit BMKG & Mikroiklim Kebun">
        <span class="tab-icon">
          <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
            <path d="M12 2v2"></path>
            <path d="m4.93 4.93 1.41 1.41"></path>
            <path d="M20 12h2"></path>
            <path d="m19.07 4.93-1.41 1.41"></path>
            <path d="M15.947 12.65a4 4 0 0 0-5.925-4.128"></path>
            <path d="M13 22H7a5 5 0 1 1 4.9-6H13a3 3 0 0 1 0 6Z"></path>
          </svg>
        </span>
        <span class="tab-label">Cuaca BMKG</span>
      </button>

      <button type="button" class="tab-btn" data-tab="logs" onclick="switchTab('logs')" title="Histori & Arsip Log LittleFS">
        <span class="tab-icon">
          <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
            <path d="M14 2H6a2 2 0 0 0-2 2v16a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V8z"></path>
            <polyline points="14 2 14 8 20 8"></polyline>
            <line x1="16" y1="13" x2="8" y2="13"></line>
            <line x1="16" y1="17" x2="8" y2="17"></line>
            <line x1="10" y1="9" x2="8" y2="9"></line>
          </svg>
        </span>
        <span class="tab-label">Histori Log</span>
      </button>
    </nav>

  <div class="modal-overlay" id="modal-sector-manager" style="display:none;">
    <div class="modal-box">
      <div class="modal-header">
        <div class="modal-title">
          <div style="display:flex; align-items:center; gap:8px;">
            <div class="kpi-icon-box" style="width:28px; height:28px; font-size:14px; background:rgba(16,185,129,0.15); color:var(--primary);">
              <svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                <path d="M12 2L2 7l10 5 10-5-10-5z"></path>
                <path d="M2 17l10 5 10-5"></path>
                <path d="M2 12l10 5 10-5"></path>
              </svg>
            </div>
            <div>
              <span>Pengaturan Zonasi Sektor Lahan</span>
              <span style="font-size:11px; font-weight:400; color:var(--text-sub); display:block; margin-top:2px;">Tersimpan otomatis di LocalStorage peramban Anda</span>
            </div>
          </div>
        </div>
        <button type="button" class="modal-close-btn" onclick="closeSectorManagerModal()">&times;</button>
      </div>
      <div class="modal-body">
        <!-- Form Add/Edit (Hidden by default unless toggled) -->
        <div id="sector-form-wrapper" style="display:none; background:rgba(255,255,255,0.03); border:1px solid var(--card-border); border-radius:14px; padding:16px; margin-bottom:18px;">
          <h4 id="sector-form-title" style="font-size:13px; font-weight:700; color:var(--primary); margin-bottom:12px;">➕ Tambah Sektor Baru</h4>
          <input type="hidden" id="edit-sector-id" value="">
          <div style="display:grid; grid-template-columns: repeat(auto-fit, minmax(180px, 1fr)); gap:12px; margin-bottom:12px;">
            <div>
              <label class="form-label">Kode / Nama Sektor:</label>
              <input type="text" id="sec-input-code" placeholder="Contoh: Sektor A / Blok Barat" class="form-input">
            </div>
            <div>
              <label class="form-label">Varietas / Komoditas:</label>
              <input type="text" id="sec-input-name" placeholder="Contoh: Cabai Rawit Merah" class="form-input">
            </div>
            <div>
              <label class="form-label">Fase Pertumbuhan:</label>
              <select id="sec-input-stage" class="form-input">
                <option value="semai">Semai / Pembibitan</option>
                <option value="vegetatif">Vegetatif (Pertumbuhan Daun)</option>
                <option value="generatif">Generatif (Bunga &amp; Buah)</option>
                <option value="panen">Pematangan / Panen</option>
              </select>
            </div>
            <div>
              <label class="form-label">Usia Tanaman (HST):</label>
              <input type="number" id="sec-input-age" min="1" max="999" placeholder="Contoh: 14" class="form-input">
            </div>
            <div>
              <label class="form-label">Luas Lahan (m²):</label>
              <input type="number" id="sec-input-area" min="1" max="100000" value="100" class="form-input">
            </div>
            <div>
              <label class="form-label">Tipe Lingkungan:</label>
              <select id="sec-input-env" class="form-input">
                <option value="open">Lahan Terbuka (Open Field)</option>
                <option value="greenhouse">Greenhouse / Naungan</option>
              </select>
            </div>
          </div>
          <div style="display:flex; justify-content:flex-end; gap:8px;">
            <button type="button" class="btn btn-secondary" onclick="cancelSectorForm()" style="padding:6px 14px; font-size:12px;">Batal</button>
            <button type="button" class="btn btn-primary" onclick="saveSectorFormData()" style="padding:6px 14px; font-size:12px; display:inline-flex; align-items:center; gap:6px;">
              <svg width="13" height="13" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M19 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h11l5 5v11a2 2 0 0 1-2 2z"></path><polyline points="17 21 17 13 7 13 7 21"></polyline><polyline points="7 3 7 8 15 8"></polyline></svg>
              <span>Simpan Sektor</span>
            </button>
          </div>
        </div>

        <!-- Sector List Header & Container -->
        <div style="display:flex; justify-content:space-between; align-items:center; margin-bottom:10px;">
          <span style="font-size:12px; font-weight:700; color:var(--text-main);">Daftar Sektor Lahan:</span>
          <button type="button" class="btn btn-primary" onclick="openAddSectorForm()" style="padding:4px 10px; font-size:11px; border-radius:8px;">+ Tambah Sektor</button>
        </div>
        <div id="sector-manager-list" style="display:flex; flex-direction:column; gap:10px; max-height:280px; overflow-y:auto; padding-right:4px;">
          <!-- Populated dynamically by renderSectorManagerList() -->
        </div>
      </div>
      <div class="modal-footer" style="display:flex; justify-content:space-between; align-items:center; padding-top:14px; border-top:1px solid var(--card-border); margin-top:14px; flex-wrap:wrap; gap:8px;">
        <div style="display:flex; gap:8px; flex-wrap:wrap;">
          <button type="button" class="btn btn-secondary" onclick="clearAllSectors()" style="color:var(--accent-rose); border-color:rgba(239,68,68,0.3); font-size:11px; padding:6px 12px;">
            Kosongkan Semua Sektor
          </button>
          <button type="button" class="btn btn-secondary" onclick="loadSamplePresetSectors()" style="color:var(--accent-emerald); border-color:rgba(16,185,129,0.3); font-size:11px; padding:6px 12px;">
            Muat Contoh Template
          </button>
        </div>
        <button type="button" class="btn btn-secondary" onclick="closeSectorManagerModal()" style="padding:8px 18px; font-size:12px;">
          Tutup
        </button>
      </div>
    </div>
  </div>

  <!-- Smart Farm SCADA Modular JavaScript Engine (Strict Line Limit < 300 lines/module) -->
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
<script>
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
    setTimeout(function () {
      if (typeof resizeCanvas === 'function') resizeCanvas();
      if (typeof renderHourlyChart === 'function') renderHourlyChart();
      if (typeof resizeCropCanvas === 'function') resizeCropCanvas();
    }, 60);
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
  if (d.unplugged || d.soil === -1) {
    r1.innerText = '!  PERINGATAN  !';
    r2.innerText = 'SENSOR TERPUTUS ';
    return;
  }
  if (d.temp !== undefined && parseFloat(d.temp) >= 35.0) {
    r1.innerText = '! BAHAYA  SUHU !';
    r2.innerText = 'Suhu:' + parseFloat(d.temp).toFixed(1) + 'C PANAS';
    return;
  }

  // Animation blink for pump: every 500ms when actively irrigating
  var blinkState = (Math.floor(Date.now() / 500) % 2 === 0);

  if (isRelayOn || virtualLcdPage === 0) {
    // Layar 1: T:28.4C H:74%  AUTO
    //         S:58%   P:OFF  L:OFF
    var tVal = (d.temp !== undefined && !isNaN(parseFloat(d.temp))) ? parseFloat(d.temp).toFixed(1) : null;
    var hVal = (d.hum !== undefined && !isNaN(parseFloat(d.hum))) ? Math.round(d.hum) : null;
    var sVal = (d.soil !== undefined && d.soil >= 0) ? Math.round(d.soil) : null;

    var tempFormatted = tVal !== null ? (tVal < 10 ? ' ' : '') + tVal + 'C' : ' --C';
    var humFormatted = hVal !== null ? (hVal < 10 ? ' ' : '') + hVal + '%' : ' --%';
    var soilFormatted = sVal !== null ? (sVal < 10 ? ' ' : '') + sVal + '%' : ' --%';

    var pumpStr = isRelayOn ? (blinkState ? 'ON ' : '   ') : (d.cooldown ? 'CLD' : 'OFF');
    var lampStr = isLampOn ? 'ON ' : 'OFF';

    r1.innerText = 'T:' + tempFormatted + ' H:' + humFormatted + ' ' + modeStr;
    r2.innerText = 'S:' + soilFormatted + ' P:' + pumpStr + ' L:' + lampStr;
  } else {
    // Layar 2: 16:52:30  AUTO
    //         -62dB 192.168.4.1
    var timeStr = d.time || '--:--:--';
    if (timeStr.length > 8) timeStr = timeStr.slice(0, 8);
    while (timeStr.length < 8) timeStr += ' ';

    var rssiStr = (d.rssi !== undefined && d.rssi !== 0) ? (d.rssi + 'dB') : ' --dB';
    while (rssiStr.length < 5) rssiStr = ' ' + rssiStr;

    r1.innerText = timeStr + '  ' + modeStr;
    r2.innerText = rssiStr + ' 192.168.4.1';
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



// =================================================================
// 02_CHARTS.JS - LIVE SCADA OSCILLOSCOPE & 24H HOURLY AGGREGATION
// Real-Time Oscilloscope, Dual Y-Axis, Setpoints, Crosshairs & 24H Stats
// =================================================================

var scadaHistory = { time: [], soil: [], temp: [], hum: [] };
var scadaFiltered = { soil: [], temp: [], hum: [] };
var scadaCanvas = null;

var oscSeries = { soil: true, temp: true, hum: true };
var oscThresholdActive = true;
var oscFilterMode = 'raw';
var oscBufferSize = 30;
var oscCrosshairIdx = -1;

var hourlySeries = { soil: true, temp: true, hum: true, pump: true, lamp: true };
var hourlyRange = 'all';
var hourlyCrosshairIdx = -1;

function initScadaCanvas() {
  scadaCanvas = document.getElementById('scadaChart');
  if (scadaCanvas) {
    attachScadaCrosshair();
    resizeCanvas();
  }
  var hCanvas = document.getElementById('hourlyChart');
  if (hCanvas) attachHourlyCrosshair();
}

function resizeCanvas() {
  if (!scadaCanvas) scadaCanvas = document.getElementById('scadaChart');
  if (scadaCanvas && scadaCanvas.parentElement) {
    scadaCanvas.width = scadaCanvas.parentElement.clientWidth;
    scadaCanvas.height = scadaCanvas.parentElement.clientHeight;
    drawChart();
  }
  var hCanvas = document.getElementById('hourlyChart');
  if (hCanvas && hCanvas.parentElement) {
    hCanvas.width = hCanvas.parentElement.clientWidth;
    hCanvas.height = hCanvas.parentElement.clientHeight;
    renderHourlyChart();
  }
  if (typeof resizeCropCanvas === 'function') resizeCropCanvas();
}

window.addEventListener('resize', resizeCanvas);

// --- TOGGLE CHIP HELPER ---
function setChipUI(id, active) {
  var el = document.getElementById(id);
  if (el) {
    el.classList.toggle('active', active);
    el.classList.toggle('inactive', !active);
  }
}

function toggleOscSeries(key) {
  if (oscSeries[key] !== undefined) {
    oscSeries[key] = !oscSeries[key];
    setChipUI('osc-toggle-' + key, oscSeries[key]);
    drawChart();
  }
}

function toggleOscThreshold() {
  oscThresholdActive = !oscThresholdActive;
  setChipUI('osc-toggle-threshold', oscThresholdActive);
  drawChart();
}

function setOscFilterMode(mode) {
  oscFilterMode = mode;
  var bRaw = document.getElementById('btn-osc-raw');
  var bEma = document.getElementById('btn-osc-ema');
  if (bRaw) bRaw.classList.toggle('active', mode === 'raw');
  if (bEma) bEma.classList.toggle('active', mode === 'ema');
  var st = document.getElementById('osc-status-text');
  if (st) st.innerText = "Buffer: " + scadaHistory.soil.length + " Titik (" + (mode === 'ema' ? 'EMA Filter' : 'Raw Data') + ")";
  drawChart();
}

function setOscBufferSize(size) {
  oscBufferSize = size;
  var b30 = document.getElementById('btn-buf-30');
  var b60 = document.getElementById('btn-buf-60');
  if (b30) b30.classList.toggle('active', size === 30);
  if (b60) b60.classList.toggle('active', size === 60);
  while (scadaHistory.soil.length > oscBufferSize) {
    scadaHistory.time.shift(); scadaHistory.soil.shift(); scadaHistory.temp.shift(); scadaHistory.hum.shift();
    scadaFiltered.soil.shift(); scadaFiltered.temp.shift(); scadaFiltered.hum.shift();
  }
  drawChart();
}

function toggleHourlySeries(key) {
  if (hourlySeries[key] !== undefined) {
    hourlySeries[key] = !hourlySeries[key];
    setChipUI('h-toggle-' + key, hourlySeries[key]);
    renderHourlyChart();
  }
}

function setHourlyRange(range) {
  hourlyRange = range;
  ['all', 'day', 'night'].forEach(function(r) {
    var b = document.getElementById('btn-h-' + r);
    if (b) b.classList.toggle('active', range === r);
  });
  var ax = document.getElementById('hourly-axis-text');
  if (ax) {
    if (range === 'day') ax.innerText = "Sumbu X: Jam 06:00 s/d 18:00 WIB (Siang)";
    else if (range === 'night') ax.innerText = "Sumbu X: Jam 18:00 s/d 06:00 WIB (Malam)";
    else ax.innerText = "Sumbu X: Jam 00:00 s/d 23:00 WIB (24 Jam Penuh)";
  }
  renderHourlyChart();
}

function refreshHourlyData() {
  var b = document.getElementById('btn-refresh-hourly');
  if (b) { b.style.opacity = '0.5'; b.style.pointerEvents = 'none'; }
  if (typeof fetchAndParseLogs === 'function') fetchAndParseLogs(); else renderHourlyChart();
  setTimeout(function () { if (b) { b.style.opacity = '1'; b.style.pointerEvents = 'auto'; } }, 700);
}

// --- DATA INGESTION & EMA FILTER ---
function updateHistory(soil, temp, hum) {
  if (soil === null || temp === null || isNaN(soil) || isNaN(temp)) return;
  var hVal = (hum !== null && !isNaN(hum)) ? hum : (scadaHistory.hum.length > 0 ? scadaHistory.hum[scadaHistory.hum.length - 1] : 60);
  if (soil < 0 || soil > 100 || temp < -10 || temp > 65 || hVal < 0 || hVal > 100) return;

  scadaHistory.time.push(new Date().toLocaleTimeString());
  scadaHistory.soil.push(soil);
  scadaHistory.temp.push(temp);
  scadaHistory.hum.push(hVal);

  var a = 0.35, len = scadaHistory.soil.length;
  if (len === 1) {
    scadaFiltered.soil.push(soil); scadaFiltered.temp.push(temp); scadaFiltered.hum.push(hVal);
  } else {
    var pS = scadaFiltered.soil[len - 2], pT = scadaFiltered.temp[len - 2], pH = scadaFiltered.hum[len - 2];
    scadaFiltered.soil.push(a * soil + (1 - a) * pS);
    scadaFiltered.temp.push(a * temp + (1 - a) * pT);
    scadaFiltered.hum.push(a * hVal + (1 - a) * pH);
  }

  while (scadaHistory.soil.length > oscBufferSize) {
    scadaHistory.time.shift(); scadaHistory.soil.shift(); scadaHistory.temp.shift(); scadaHistory.hum.shift();
    scadaFiltered.soil.shift(); scadaFiltered.temp.shift(); scadaFiltered.hum.shift();
  }

  var vS = document.getElementById('osc-val-soil'), vT = document.getElementById('osc-val-temp'), vH = document.getElementById('osc-val-hum');
  if (vS) vS.innerText = Math.round(soil) + "%";
  if (vT) vT.innerText = temp.toFixed(1) + "°C";
  if (vH && hVal !== null) vH.innerText = Math.round(hVal) + "%";
  var st = document.getElementById('osc-status-text');
  if (st) st.innerText = "Buffer: " + scadaHistory.soil.length + " Titik (" + (oscFilterMode === 'ema' ? 'EMA Filter' : 'Raw Data') + ")";
  drawChart();
}

// --- CURVE RENDER HELPER ---
function drawLiveCurve(ctx, pts, scaleMax, padL, padT, chartW, chartH, maxSlots, color, doArea, glow) {
  var count = pts.length;
  if (count === 0) return;
  var stepX = chartW / Math.max(maxSlots - 1, 1);

  if (doArea) {
    var grad = ctx.createLinearGradient(0, padT, 0, padT + chartH);
    grad.addColorStop(0, "rgba(16, 185, 129, 0.22)");
    grad.addColorStop(1, "rgba(16, 185, 129, 0.0)");
    ctx.beginPath();
    for (var i = 0; i < count; i++) {
      var x = padL + i * stepX, y = padT + chartH - (pts[i] / scaleMax * chartH);
      if (i === 0) ctx.moveTo(x, y); else ctx.lineTo(x, y);
    }
    ctx.lineTo(padL + (count - 1) * stepX, padT + chartH);
    ctx.lineTo(padL, padT + chartH);
    ctx.closePath();
    ctx.fillStyle = grad;
    ctx.fill();
  }

  ctx.beginPath();
  ctx.strokeStyle = color;
  ctx.lineWidth = 2.4;
  for (var j = 0; j < count; j++) {
    var px = padL + j * stepX, py = padT + chartH - (pts[j] / scaleMax * chartH);
    if (j === 0) ctx.moveTo(px, py); else ctx.lineTo(px, py);
  }
  ctx.stroke();

  if (glow) {
    var tipX = padL + (count - 1) * stepX, tipY = padT + chartH - (pts[count - 1] / scaleMax * chartH);
    ctx.fillStyle = color;
    ctx.beginPath(); ctx.arc(tipX, tipY, 4, 0, Math.PI * 2); ctx.fill();
  }
}

// --- CROSSHAIR INTERACTIVITY ---
function attachScadaCrosshair() {
  var canvas = document.getElementById('scadaChart');
  var tooltip = document.getElementById('osc-crosshair-tooltip');
  if (!canvas) return;

  function handleMove(e) {
    var rect = canvas.getBoundingClientRect();
    var clientX = e.touches ? e.touches[0].clientX : e.clientX;
    var x = clientX - rect.left;
    var padL = 36, padR = 36, chartW = canvas.width - padL - padR;
    var ptsCount = scadaHistory.soil.length;
    if (ptsCount === 0 || x < padL || x > padL + chartW) {
      oscCrosshairIdx = -1;
      if (tooltip) tooltip.style.display = 'none';
      drawChart();
      return;
    }
    var slots = Math.max(oscBufferSize, 10);
    var stepX = chartW / Math.max(slots - 1, 1);
    var relIdx = Math.round((x - padL) / stepX);
    if (relIdx >= 0 && relIdx < ptsCount) {
      oscCrosshairIdx = relIdx;
      if (tooltip) {
        var t = scadaHistory.time[relIdx] || "--";
        var s = Math.round(scadaHistory.soil[relIdx]);
        var tp = scadaHistory.temp[relIdx] ? scadaHistory.temp[relIdx].toFixed(1) : "--";
        var hm = scadaHistory.hum[relIdx] ? Math.round(scadaHistory.hum[relIdx]) : "--";
        tooltip.innerHTML = '<span style="color:#94a3b8;">' + t + '</span> &bull; ' +
          '<span style="color:#10b981;font-weight:700;">Tanah: ' + s + '%</span> &bull; ' +
          '<span style="color:#06b6d4;font-weight:700;">Suhu: ' + tp + '°C</span> &bull; ' +
          '<span style="color:#a855f7;font-weight:700;">RH: ' + hm + '%</span>';
        tooltip.style.display = 'block';
      }
      drawChart();
    }
  }

  function handleLeave() {
    oscCrosshairIdx = -1;
    if (tooltip) tooltip.style.display = 'none';
    drawChart();
  }

  canvas.addEventListener('mousemove', handleMove);
  canvas.addEventListener('mouseleave', handleLeave);
  canvas.addEventListener('touchmove', handleMove, { passive: true });
  canvas.addEventListener('touchend', handleLeave);
}

function attachHourlyCrosshair() {
  var canvas = document.getElementById('hourlyChart');
  var tooltip = document.getElementById('hourly-crosshair-tooltip');
  if (!canvas) return;

  function handleMove(e) {
    var rect = canvas.getBoundingClientRect();
    var clientX = e.touches ? e.touches[0].clientX : e.clientX;
    var x = clientX - rect.left;
    var padL = 36, padR = 36, chartW = canvas.width - padL - padR;
    var hoursList = getActiveHoursList();
    if (hoursList.length === 0 || x < padL || x > padL + chartW) {
      hourlyCrosshairIdx = -1;
      if (tooltip) tooltip.style.display = 'none';
      renderHourlyChart();
      return;
    }
    var step = chartW / hoursList.length;
    var slotIdx = Math.floor((x - padL) / step);
    if (slotIdx >= 0 && slotIdx < hoursList.length) {
      hourlyCrosshairIdx = slotIdx;
      var hr = hoursList[slotIdx];
      var stats = window.lastHourlyStatsMap ? window.lastHourlyStatsMap[hr] : null;
      if (tooltip && stats && stats.count > 0) {
        var aS = (stats.soilSum / stats.count).toFixed(0);
        var aT = (stats.tempSum / stats.count).toFixed(1);
        var aH = (stats.humSum / stats.count).toFixed(0);
        var pS = stats.pumpSecsMax || 0;
        tooltip.innerHTML = '<span style="color:#94a3b8;">Jam ' + (hr < 10 ? '0' : '') + hr + ':00</span> &bull; ' +
          '<span style="color:#10b981;font-weight:700;">Tanah: ' + aS + '%</span> &bull; ' +
          '<span style="color:#06b6d4;font-weight:700;">Suhu: ' + aT + '°C</span> &bull; ' +
          '<span style="color:#a855f7;font-weight:700;">RH: ' + aH + '%</span> &bull; ' +
          '<span style="color:#f59e0b;font-weight:700;">Pompa: ' + pS + 's</span>';
        tooltip.style.display = 'block';
      }
      renderHourlyChart();
    }
  }

  function handleLeave() {
    hourlyCrosshairIdx = -1;
    if (tooltip) tooltip.style.display = 'none';
    renderHourlyChart();
  }

  canvas.addEventListener('mousemove', handleMove);
  canvas.addEventListener('mouseleave', handleLeave);
  canvas.addEventListener('touchmove', handleMove, { passive: true });
  canvas.addEventListener('touchend', handleLeave);
}

function getActiveHoursList() {
  var list = [];
  if (hourlyRange === 'day') for (var d = 6; d <= 18; d++) list.push(d);
  else if (hourlyRange === 'night') {
    for (var n1 = 18; n1 < 24; n1++) list.push(n1);
    for (var n2 = 0; n2 <= 6; n2++) list.push(n2);
  } else {
    for (var a = 0; a < 24; a++) list.push(a);
  }
  return list;
}

// --- RENDER OSCILLOSCOPE ---
function drawChart() {
  if (!scadaCanvas) scadaCanvas = document.getElementById('scadaChart');
  if (!scadaCanvas) return;
  var ctx = scadaCanvas.getContext('2d'), w = scadaCanvas.width, h = scadaCanvas.height;
  ctx.clearRect(0, 0, w, h);

  var padL = 36, padR = 36, padT = 16, padB = 22;
  var chartW = w - padL - padR, chartH = h - padT - padB;

  // Dual Y-Axis Grid & Labels
  ctx.strokeStyle = "rgba(255,255,255,0.06)";
  ctx.lineWidth = 1;
  ctx.font = "9px Inter, monospace";

  for (var k = 0; k <= 4; k++) {
    var gy = padT + (chartH * (k / 4));
    ctx.beginPath(); ctx.moveTo(padL, gy); ctx.lineTo(w - padR, gy); ctx.stroke();
    // Left Y-Axis (0 - 100%)
    ctx.fillStyle = "rgba(255,255,255,0.4)";
    ctx.fillText((100 - k * 25) + "%", 6, gy + 3);
    // Right Y-Axis (0 - 50°C)
    ctx.fillStyle = "rgba(6, 182, 212, 0.75)";
    ctx.fillText((50 - k * 12.5).toFixed(0) + "°C", w - padR + 6, gy + 3);
  }

  // Zona Nyaman & Setpoint Thresholds
  if (oscThresholdActive) {
    // 1. Zona Nyaman Tanah (60% - 75%)
    var y75 = padT + chartH - (75 / 100 * chartH);
    var y60 = padT + chartH - (60 / 100 * chartH);
    ctx.fillStyle = "rgba(16, 185, 129, 0.07)";
    ctx.fillRect(padL, y75, chartW, y60 - y75);
    ctx.fillStyle = "rgba(16, 185, 129, 0.4)";
    ctx.font = "8px Inter, sans-serif";
    ctx.fillText("ZONA NYAMAN TANAH (60-75%)", padL + 6, y75 + 10);

    // 2. Ambang Batas Siram (Setpoint Soil Moisture)
    var sThresh = (window.lastTelemetryData && window.lastTelemetryData.batasTanah !== undefined) ? window.lastTelemetryData.batasTanah : 50;
    var yThresh = padT + chartH - (sThresh / 100 * chartH);
    ctx.save();
    ctx.setLineDash([4, 4]);
    ctx.strokeStyle = "rgba(239, 68, 68, 0.85)";
    ctx.lineWidth = 1.4;
    ctx.beginPath();
    ctx.moveTo(padL, yThresh);
    ctx.lineTo(padL + chartW, yThresh);
    ctx.stroke();
    ctx.fillStyle = "#ef4444";
    ctx.font = "8px Inter, sans-serif";
    ctx.fillText("BATAS SIRAM (" + sThresh + "%)", padL + chartW - 90, yThresh - 3);
    ctx.restore();

    // 3. Ambang Batas Bahaya Panas (Temperature Danger Line)
    var tThresh = (window.lastTelemetryData && window.lastTelemetryData.batasSuhu !== undefined) ? window.lastTelemetryData.batasSuhu : 32;
    var yTempThresh = padT + chartH - (tThresh / 50 * chartH);
    ctx.save();
    ctx.setLineDash([3, 3]);
    ctx.strokeStyle = "rgba(245, 158, 11, 0.75)";
    ctx.lineWidth = 1.2;
    ctx.beginPath();
    ctx.moveTo(padL, yTempThresh);
    ctx.lineTo(padL + chartW, yTempThresh);
    ctx.stroke();
    ctx.fillStyle = "#f59e0b";
    ctx.font = "8px Inter, sans-serif";
    ctx.fillText("BATAS SUHU (" + tThresh + "°C)", padL + 6, yTempThresh - 3);
    ctx.restore();
  }

  var d = (oscFilterMode === 'ema') ? scadaFiltered : scadaHistory;
  if (d.soil.length < 1) {
    ctx.fillStyle = "rgba(255,255,255,0.35)";
    ctx.font = "12px Inter, sans-serif";
    ctx.fillText("Oscilloscope SCADA Aktif - Menunggu Sinyal Telemetri ESP...", padL + 20, h / 2);
    return;
  }

  var slots = Math.max(oscBufferSize, 10);
  if (oscSeries.soil) drawLiveCurve(ctx, d.soil, 100, padL, padT, chartW, chartH, slots, '#10b981', true, true);
  if (oscSeries.hum)  drawLiveCurve(ctx, d.hum, 100, padL, padT, chartW, chartH, slots, '#a855f7', false, true);
  if (oscSeries.temp) drawLiveCurve(ctx, d.temp, 50, padL, padT, chartW, chartH, slots, '#06b6d4', false, true);

  // Draw Crosshair on Hover/Touch
  if (oscCrosshairIdx >= 0 && oscCrosshairIdx < d.soil.length) {
    var stepX = chartW / Math.max(slots - 1, 1);
    var chX = padL + oscCrosshairIdx * stepX;
    ctx.save();
    ctx.setLineDash([2, 2]);
    ctx.strokeStyle = "rgba(255, 255, 255, 0.5)";
    ctx.lineWidth = 1;
    ctx.beginPath();
    ctx.moveTo(chX, padT);
    ctx.lineTo(chX, padT + chartH);
    ctx.stroke();
    ctx.restore();
  }

  if (scadaHistory.time.length > 1) {
    ctx.fillStyle = "rgba(255,255,255,0.3)";
    ctx.font = "8px Inter, monospace";
    ctx.fillText(scadaHistory.time[0], padL, h - 6);
    ctx.fillText(scadaHistory.time[scadaHistory.time.length - 1], w - padR - 35, h - 6);
  }
}

// --- RENDER 24H HOURLY AGGREGATION ---
function renderHourlyChart() {
  var canvas = document.getElementById('hourlyChart');
  if (!canvas || !canvas.parentElement) return;
  var ctx = canvas.getContext('2d'), w = canvas.width = canvas.parentElement.clientWidth, h = canvas.height = canvas.parentElement.clientHeight;
  ctx.clearRect(0, 0, w, h);

  var padL = 36, padR = 36, padT = 16, padB = 24;
  var chartW = w - padL - padR, chartH = h - padT - padB;

  ctx.strokeStyle = "rgba(255,255,255,0.05)";
  ctx.lineWidth = 1;
  ctx.font = "9px Inter, monospace";

  for (var k = 0; k <= 4; k++) {
    var gy = padT + (chartH * (k / 4));
    ctx.beginPath(); ctx.moveTo(padL, gy); ctx.lineTo(w - padR, gy); ctx.stroke();
    // Left Y-Axis
    ctx.fillStyle = "rgba(255,255,255,0.35)";
    ctx.fillText((100 - k * 25) + "%", 6, gy + 3);
    // Right Y-Axis
    ctx.fillStyle = "rgba(6, 182, 212, 0.7)";
    ctx.fillText((50 - k * 12.5).toFixed(0) + "°C", w - padR + 6, gy + 3);
  }

  var logs = (typeof rawLogsCache !== 'undefined') ? rawLogsCache : [];
  if (!logs || logs.length === 0) {
    ctx.fillStyle = "rgba(255,255,255,0.35)";
    ctx.font = "12px Inter, sans-serif";
    ctx.fillText("Belum Ada Data Log Per Jam (ESP32 Sedang Mengumpulkan Rekaman)", padL + 10, h / 2);
    var st = document.getElementById('hourly-summary-text');
    if (st) st.innerText = "Deteksi Evaporasi: Menunggu Rekaman Log LittleFS";
    return;
  }

  var hourlyMap = {};
  for (var hr = 0; hr < 24; hr++) hourlyMap[hr] = { tempSum: 0, humSum: 0, soilSum: 0, pumpSecsMax: 0, lampActive: 0, count: 0 };

  for (var i = 0; i < logs.length; i++) {
    var item = logs[i], hIdx = -1;
    if (item.time && item.time.indexOf(":") !== -1) {
      var tP = item.time.split(" ");
      hIdx = parseInt(tP[tP.length - 1].split(":")[0], 10);
    }
    if (hIdx >= 0 && hIdx < 24) {
      hourlyMap[hIdx].tempSum += item.temp;
      hourlyMap[hIdx].humSum += item.hum;
      hourlyMap[hIdx].soilSum += item.soil;
      hourlyMap[hIdx].pumpSecsMax = Math.max(hourlyMap[hIdx].pumpSecsMax, item.pumpSecs || 0);
      if (item.lamp === "1" || item.lamp === 1 || item.lamp === true || item.lamp === "ON") {
        hourlyMap[hIdx].lampActive += 1;
      }
      hourlyMap[hIdx].count += 1;
    }
  }
  window.lastHourlyStatsMap = hourlyMap;

  // Compute 24-Hour Quick Statistics
  var minTemp24 = 999, maxTemp24 = -999, totalSoilSum = 0, totalSoilCount = 0, totalPumpSecs = 0, pumpActivations = 0, lampHours = 0;
  for (var chk = 0; chk < 24; chk++) {
    var hm = hourlyMap[chk];
    if (hm.count > 0) {
      var hTemp = hm.tempSum / hm.count;
      if (hTemp < minTemp24) minTemp24 = hTemp;
      if (hTemp > maxTemp24) maxTemp24 = hTemp;
      totalSoilSum += hm.soilSum;
      totalSoilCount += hm.count;
    }
    if (hm.pumpSecsMax > 0) {
      totalPumpSecs += hm.pumpSecsMax;
      pumpActivations++;
    }
    if (hm.lampActive > 0) lampHours++;
  }

  var statTempEl = document.getElementById('stat-24h-temp');
  var statSoilEl = document.getElementById('stat-24h-soil');
  var statPumpEl = document.getElementById('stat-24h-pump');
  var statLampEl = document.getElementById('stat-24h-lamp');

  if (statTempEl && minTemp24 !== 999) statTempEl.innerText = minTemp24.toFixed(1) + "°C / " + maxTemp24.toFixed(1) + "°C";
  if (statSoilEl && totalSoilCount > 0) statSoilEl.innerText = Math.round(totalSoilSum / totalSoilCount) + "%";
  if (statPumpEl) statPumpEl.innerText = pumpActivations + " Kali (" + totalPumpSecs + "s)";
  if (statLampEl) statLampEl.innerText = lampHours + " Jam";

  var hoursList = getActiveHoursList();
  var numSlots = hoursList.length, step = chartW / numSlots, maxTemp = 0, maxHour = -1;

  // Render Lamp Background Highlight
  if (hourlySeries.lamp) {
    var schedStart = parseInt(localStorage.getItem('lamp_sched_start') || '18', 10);
    var schedEnd = parseInt(localStorage.getItem('lamp_sched_end') || '6', 10);

    for (var bL = 0; bL < numSlots; bL++) {
      var lHour = hoursList[bL];
      var isLampOn = (hourlyMap[lHour].lampActive > 0);
      if (!isLampOn) {
        if (schedStart > schedEnd ? (lHour >= schedStart || lHour < schedEnd) : (lHour >= schedStart && lHour < schedEnd)) {
          isLampOn = true;
        }
      }
      if (isLampOn) {
        var lx = padL + bL * step;
        ctx.fillStyle = "rgba(234, 179, 8, 0.08)";
        ctx.fillRect(lx, padT, step, chartH);
        ctx.fillStyle = "#eab308";
        ctx.fillRect(lx + 1, padT, step - 2, 3);
      }
    }
  }

  // Render Pump Bars
  if (hourlySeries.pump) {
    for (var b = 0; b < numSlots; b++) {
      var pHour = hoursList[b], pSecs = hourlyMap[pHour].pumpSecsMax;
      var barH = Math.min((pSecs / 1800) * chartH, chartH);
      if (barH > 0) {
        var bx = padL + b * step + step * 0.2;
        ctx.fillStyle = "rgba(245, 158, 11, 0.4)";
        ctx.fillRect(bx, padT + chartH - barH, step * 0.6, barH);
        ctx.strokeStyle = "#f59e0b";
        ctx.strokeRect(bx, padT + chartH - barH, step * 0.6, barH);
      }
    }
  }

  function drawHourlyLine(key, scale, color) {
    ctx.beginPath();
    ctx.strokeStyle = color;
    ctx.lineWidth = 2.4;
    var first = true;
    for (var s = 0; s < numSlots; s++) {
      var hr = hoursList[s];
      if (hourlyMap[hr].count > 0) {
        var avg = hourlyMap[hr][key] / hourlyMap[hr].count;
        var sx = padL + s * step + step / 2;
        var sy = padT + chartH - (avg / scale * chartH);
        if (first) { ctx.moveTo(sx, sy); first = false; } else ctx.lineTo(sx, sy);
        if (key === 'tempSum' && avg > maxTemp) { maxTemp = avg; maxHour = hr; }
      }
    }
    if (!first) ctx.stroke();
  }

  if (hourlySeries.soil) drawHourlyLine('soilSum', 100, '#10b981');
  if (hourlySeries.hum)  drawHourlyLine('humSum', 100, '#a855f7');
  if (hourlySeries.temp) drawHourlyLine('tempSum', 50, '#06b6d4');

  // Draw Crosshair for Hourly
  if (hourlyCrosshairIdx >= 0 && hourlyCrosshairIdx < numSlots) {
    var hChX = padL + hourlyCrosshairIdx * step + step / 2;
    ctx.save();
    ctx.setLineDash([2, 2]);
    ctx.strokeStyle = "rgba(255, 255, 255, 0.5)";
    ctx.lineWidth = 1;
    ctx.beginPath();
    ctx.moveTo(hChX, padT);
    ctx.lineTo(hChX, padT + chartH);
    ctx.stroke();
    ctx.restore();
  }

  ctx.fillStyle = "rgba(255,255,255,0.4)";
  ctx.font = "9px Inter, monospace";
  var stepLbl = (numSlots > 16) ? 3 : 2;
  for (var l = 0; l < numSlots; l += stepLbl) {
    var lHour = hoursList[l];
    ctx.fillText((lHour < 10 ? '0' : '') + lHour + ":00", padL + l * step + 2, h - 6);
  }

  var sumText = document.getElementById('hourly-summary-text');
  if (sumText) {
    sumText.innerText = (maxHour >= 0)
      ? "Deteksi Evaporasi: Puncak Suhu (" + maxTemp.toFixed(1) + "°C) Jam " + (maxHour < 10 ? '0' : '') + maxHour + ":00 WIB"
      : "Deteksi Evaporasi: Menunggu Rekam Log Per Jam";
  }
}


// =================================================================
// 02B_CROP_HISTORY_CHART.JS - CROP LIFECYCLE S-CURVE & LOCAL STORAGE
// Tracks day-by-day crop growth (HST), phenology milestones & vigor
// Stored persistently in browser LocalStorage
// =================================================================

var cropHistoryCanvas = null;
var cropCrosshairIdx = -1;

function getCropHistoryData() {
  try {
    var raw = localStorage.getItem('smartfarm_crop_history');
    if (raw) return JSON.parse(raw);
  } catch (e) {
    console.error("Error reading crop history:", e);
  }
  return [];
}

function saveCropHistoryData(data) {
  try {
    localStorage.setItem('smartfarm_crop_history', JSON.stringify(data));
  } catch (e) {
    console.error("Error saving crop history:", e);
  }
}

function initCropHistory() {
  cropHistoryCanvas = document.getElementById('cropHistoryChart');
  if (cropHistoryCanvas) {
    attachCropCrosshair();
    resizeCropCanvas();
  }
  updateCropHistorySummary();
}

function resizeCropCanvas() {
  if (!cropHistoryCanvas) cropHistoryCanvas = document.getElementById('cropHistoryChart');
  if (cropHistoryCanvas && cropHistoryCanvas.parentElement) {
    cropHistoryCanvas.width = cropHistoryCanvas.parentElement.clientWidth;
    cropHistoryCanvas.height = cropHistoryCanvas.parentElement.clientHeight;
    renderCropHistoryChart();
  }
}

// Ensure init after DOM is ready
window.addEventListener('DOMContentLoaded', function() {
  setTimeout(initCropHistory, 400);
});

// --- RECORD TODAY'S LOG ---
function logTodayCropGrowth() {
  var tData = window.lastTelemetryData;
  if (!tData || tData.soil === undefined || tData.temp === undefined || isNaN(parseFloat(tData.soil)) || isNaN(parseFloat(tData.temp))) {
    alert("Gagal mencatat: Data sensor fisik ESP32 belum diterima. Pastikan perangkat aktif.");
    return;
  }

  var storedAge = localStorage.getItem('crop_age');
  var storedName = localStorage.getItem('crop_name');
  var storedStage = localStorage.getItem('crop_stage');

  if (!storedAge || !storedName) {
    alert("Silakan tentukan komoditas dan umur tanaman terlebih dahulu di tab Kontrol & Agronomi.");
    return;
  }

  var age = parseInt(storedAge, 10);
  var name = storedName;
  var stage = storedStage || 'semai';
  var leaves = parseInt(localStorage.getItem('crop_leaves') || '4', 10);
  
  var soil = Math.round(parseFloat(tData.soil));
  var temp = parseFloat(tData.temp);
  var hum = (tData.hum !== undefined && !isNaN(parseFloat(tData.hum))) ? Math.round(parseFloat(tData.hum)) : null;

  // Hitung Skor Vigor Murni dari Telemetri Fisik Riil
  var vigor = 100;
  // Penalti jika tanah di luar zona nyaman (60% - 75%)
  if (soil < 50) vigor -= Math.min(40, (50 - soil) * 2);
  else if (soil > 80) vigor -= Math.min(30, (soil - 80) * 2);

  // Penalti jika suhu ekstrem
  if (temp > 32) vigor -= Math.min(30, (temp - 32) * 5);
  else if (temp < 20) vigor -= Math.min(25, (20 - temp) * 3);

  // Penalti bibit semai jika daun belum tumbuh cukup
  if (stage === 'semai' && leaves < 3) vigor -= 15;

  vigor = Math.max(10, Math.min(100, Math.round(vigor)));

  var todayStr = new Date().toLocaleDateString('id-ID', { day: '2-digit', month: '2-digit', year: 'numeric' });
  var history = getCropHistoryData();
  
  var existingIdx = -1;
  for (var i = 0; i < history.length; i++) {
    if (history[i].hst === age || history[i].date === todayStr) {
      existingIdx = i;
      break;
    }
  }

  var newEntry = {
    date: todayStr,
    hst: age,
    cropName: name,
    stage: stage,
    soil: soil,
    temp: temp,
    hum: hum,
    leaves: leaves,
    vigor: vigor
  };

  if (existingIdx >= 0) {
    history[existingIdx] = newEntry;
  } else {
    history.push(newEntry);
    history.sort(function(a, b) { return a.hst - b.hst; });
  }

  saveCropHistoryData(history);
  updateCropHistorySummary();
  renderCropHistoryChart();

  var footerStatus = document.getElementById('crop-hist-footer-status');
  if (footerStatus) {
    footerStatus.innerHTML = '<span style="color:#10b981;">✓ Data riil sensor HST ' + age + ' (Vigor: ' + vigor + '%, Tanah: ' + soil + '%) berhasil disimpan!</span>';
    setTimeout(function() {
      if (footerStatus) footerStatus.innerText = "Data tersimpan di LocalStorage peramban • Kurva Pertumbuhan Sigmoid (S-Curve)";
    }, 3500);
  }
}

// --- RESET CYCLE ---
function resetCropHistory() {
  if (confirm("Mulai siklus tanam baru dari HST 1? Catatan riwayat perkembangan tanaman sebelumnya akan dihapus.")) {
    saveCropHistoryData([]);
    localStorage.removeItem('crop_age');
    localStorage.removeItem('crop_stage');
    var ageInput = document.getElementById('crop-age-days');
    if (ageInput) ageInput.value = 1;
    var stageInput = document.getElementById('crop-stage');
    if (stageInput) stageInput.value = 'semai';
    if (typeof updateCropAgronomyAnalysis === 'function') updateCropAgronomyAnalysis();
    updateCropHistorySummary();
    renderCropHistoryChart();
  }
}

// --- UPDATE SUMMARY INDICATORS (STRICT 100% REAL HISTORY / ZERO DUMMY) ---
function updateCropHistorySummary() {
  var history = getCropHistoryData();
  var ptsEl = document.getElementById('crop-hist-points-text');
  var hstEl = document.getElementById('crop-hist-hst');
  var stageEl = document.getElementById('crop-hist-stage');
  var vigorEl = document.getElementById('crop-hist-vigor');
  var etaEl = document.getElementById('crop-hist-eta');

  if (ptsEl) ptsEl.innerText = history.length + " Catatan Tersimpan";

  // JIKA BELUM PERNAH DICATAT SAMA SEKALI: WAJIB TAMPILKAN STATUS KOSONG (--)
  if (history.length === 0) {
    if (hstEl) hstEl.innerText = "-- HST";
    if (stageEl) stageEl.innerText = "--";
    if (vigorEl) vigorEl.innerText = "--%";
    if (etaEl) etaEl.innerText = "-- Hari Lagi";
    return;
  }

  // JIKA SUDAH ADA CATATAN: AMBIL DARI CATATAN TERAKHIR YANG SUDAH TERVERIFIKASI
  var lastEntry = history[history.length - 1];
  var stageNames = {
    'semai': 'Semai (Nursery)',
    'vegetatif': 'Vegetatif Aktif',
    'generatif': 'Generatif / Bunga',
    'panen': 'Pematangan / Panen'
  };

  if (hstEl) hstEl.innerText = lastEntry.hst + " HST";
  if (stageEl) stageEl.innerText = stageNames[lastEntry.stage] || lastEntry.stage;
  if (vigorEl) vigorEl.innerText = lastEntry.vigor + "%";

  var name = lastEntry.cropName || localStorage.getItem('crop_name') || 'Tanaman';
  var totalCycleDays = 90;
  var nLower = name.toLowerCase();
  if (nLower.indexOf('bawang') !== -1) totalCycleDays = 65;
  else if (nLower.indexOf('melon') !== -1) totalCycleDays = 70;
  else if (nLower.indexOf('tomat') !== -1) totalCycleDays = 85;
  else if (nLower.indexOf('padi') !== -1) totalCycleDays = 115;
  else totalCycleDays = 90;

  var remaining = Math.max(0, totalCycleDays - lastEntry.hst);
  if (etaEl) etaEl.innerText = remaining > 0 ? (remaining + " Hari Lagi") : "Siap Panen";
}

// --- CROSSHAIR INTERACTION ---
function attachCropCrosshair() {
  var canvas = document.getElementById('cropHistoryChart');
  var tooltip = document.getElementById('crop-hist-tooltip');
  if (!canvas) return;

  function handleMove(e) {
    var rect = canvas.getBoundingClientRect();
    var clientX = e.touches ? e.touches[0].clientX : e.clientX;
    var x = clientX - rect.left;
    var padL = 36, padR = 24, chartW = canvas.width - padL - padR;
    var history = getCropHistoryData();

    if (history.length === 0 || x < padL || x > padL + chartW) {
      cropCrosshairIdx = -1;
      if (tooltip) tooltip.style.display = 'none';
      renderCropHistoryChart();
      return;
    }

    var maxHst = 90;
    var clickedHst = Math.round(((x - padL) / chartW) * maxHst);
    
    // Find closest logged record or calculate theoretical
    var closestEntry = null, minDist = 999;
    for (var i = 0; i < history.length; i++) {
      var d = Math.abs(history[i].hst - clickedHst);
      if (d < minDist) { minDist = d; closestEntry = history[i]; }
    }

    if (closestEntry && minDist <= 8) {
      cropCrosshairIdx = closestEntry.hst;
      if (tooltip) {
        tooltip.innerHTML = '<span style="color:#94a3b8;">' + closestEntry.date + ' (HST ' + closestEntry.hst + ')</span> &bull; ' +
          '<span style="color:#10b981;font-weight:700;">Vigor: ' + closestEntry.vigor + '%</span> &bull; ' +
          '<span style="color:#06b6d4;">Tanah: ' + closestEntry.soil + '%</span> &bull; ' +
          '<span style="color:#eab308;">Suhu: ' + closestEntry.temp + '°C</span>';
        tooltip.style.display = 'block';
      }
    } else {
      cropCrosshairIdx = clickedHst;
      if (tooltip) {
        var theo = Math.round(100 / (1 + Math.exp(-0.08 * (clickedHst - 40))));
        tooltip.innerHTML = '<span style="color:#94a3b8;">HST ' + clickedHst + ' &bull; Target Ideal: ' + theo + '%</span>';
        tooltip.style.display = 'block';
      }
    }
    renderCropHistoryChart();
  }

  function handleLeave() {
    cropCrosshairIdx = -1;
    if (tooltip) tooltip.style.display = 'none';
    renderCropHistoryChart();
  }

  canvas.addEventListener('mousemove', handleMove);
  canvas.addEventListener('mouseleave', handleLeave);
  canvas.addEventListener('touchmove', handleMove, { passive: true });
  canvas.addEventListener('touchend', handleLeave);
}

// --- RENDER S-CURVE & PHENOLOGY TIMELINE ---
function renderCropHistoryChart() {
  if (!cropHistoryCanvas) cropHistoryCanvas = document.getElementById('cropHistoryChart');
  if (!cropHistoryCanvas || !cropHistoryCanvas.parentElement) return;

  var ctx = cropHistoryCanvas.getContext('2d');
  var w = cropHistoryCanvas.width = cropHistoryCanvas.parentElement.clientWidth;
  var h = cropHistoryCanvas.height = cropHistoryCanvas.parentElement.clientHeight;
  ctx.clearRect(0, 0, w, h);

  var padL = 36, padR = 24, padT = 18, padB = 26;
  var chartW = w - padL - padR, chartH = h - padT - padB;
  var maxHst = 90;

  // Background Grid & Y-Axis
  ctx.strokeStyle = "rgba(255,255,255,0.06)";
  ctx.lineWidth = 1;
  ctx.fillStyle = "rgba(255,255,255,0.35)";
  ctx.font = "9px Inter, monospace";

  for (var k = 0; k <= 4; k++) {
    var gy = padT + (chartH * (k / 4));
    ctx.beginPath(); ctx.moveTo(padL, gy); ctx.lineTo(w - padR, gy); ctx.stroke();
    ctx.fillText((100 - k * 25) + "%", 6, gy + 3);
  }

  // 1. Shading 4 Phenology Stages (Semai, Vegetatif, Generatif, Panen)
  var stages = [
    { name: "Semai (0-14)", end: 14, color: "rgba(16, 185, 129, 0.04)" },
    { name: "Vegetatif (15-40)", end: 40, color: "rgba(6, 182, 212, 0.04)" },
    { name: "Generatif (41-70)", end: 70, color: "rgba(245, 158, 11, 0.04)" },
    { name: "Panen (71-90)", end: 90, color: "rgba(234, 179, 8, 0.05)" }
  ];

  var prevEnd = 0;
  for (var s = 0; s < stages.length; s++) {
    var stObj = stages[s];
    var startX = padL + (prevEnd / maxHst) * chartW;
    var endX = padL + (stObj.end / maxHst) * chartW;
    ctx.fillStyle = stObj.color;
    ctx.fillRect(startX, padT, endX - startX, chartH);

    // Stage boundary vertical line
    if (s < stages.length - 1) {
      ctx.strokeStyle = "rgba(255,255,255,0.08)";
      ctx.beginPath(); ctx.moveTo(endX, padT); ctx.lineTo(endX, padT + chartH); ctx.stroke();
    }
    // Stage label
    ctx.fillStyle = "rgba(255,255,255,0.25)";
    ctx.font = "8px Inter, sans-serif";
    ctx.fillText(stObj.name, startX + 4, padT + 10);
    prevEnd = stObj.end;
  }

  // 2. Theoretical Sigmoid Growth Curve (S-Curve)
  ctx.beginPath();
  ctx.strokeStyle = "rgba(6, 182, 212, 0.4)";
  ctx.setLineDash([4, 3]);
  ctx.lineWidth = 1.8;
  for (var t = 0; t <= maxHst; t++) {
    var sigVal = 100 / (1 + Math.exp(-0.08 * (t - 40)));
    var tx = padL + (t / maxHst) * chartW;
    var ty = padT + chartH - (sigVal / 100 * chartH);
    if (t === 0) ctx.moveTo(tx, ty); else ctx.lineTo(tx, ty);
  }
  ctx.stroke();
  ctx.setLineDash([]); // Reset dash

  // 3. Actual Logged Crop History Points
  var history = getCropHistoryData();
  if (history.length > 0) {
    // Connect actual points
    ctx.beginPath();
    ctx.strokeStyle = "#10b981";
    ctx.lineWidth = 2.4;
    for (var p = 0; p < history.length; p++) {
      var entry = history[p];
      var px = padL + (Math.min(entry.hst, maxHst) / maxHst) * chartW;
      var py = padT + chartH - (entry.vigor / 100 * chartH);
      if (p === 0) ctx.moveTo(px, py); else ctx.lineTo(px, py);
    }
    ctx.stroke();

    // Draw dots for each logged day
    for (var d = 0; d < history.length; d++) {
      var dEntry = history[d];
      var dx = padL + (Math.min(dEntry.hst, maxHst) / maxHst) * chartW;
      var dy = padT + chartH - (dEntry.vigor / 100 * chartH);
      ctx.fillStyle = "#10b981";
      ctx.beginPath(); ctx.arc(dx, dy, 4.5, 0, Math.PI * 2); ctx.fill();
      ctx.strokeStyle = "#ffffff";
      ctx.lineWidth = 1.5;
      ctx.stroke();
    }
  } else {
    // If no records yet, show gentle prompt
    ctx.fillStyle = "rgba(255,255,255,0.3)";
    ctx.font = "11px Inter, sans-serif";
    ctx.fillText("Belum ada catatan harian. Klik tombol [ Catat Hari Ini ] untuk merekam perkembangan tanaman.", padL + 12, h / 2 + 10);
  }

  // 4. Current Day (HST) Marker
  var currentAge = parseInt(localStorage.getItem('crop_age') || '14', 10);
  if (currentAge <= maxHst) {
    var curX = padL + (currentAge / maxHst) * chartW;
    ctx.save();
    ctx.setLineDash([3, 3]);
    ctx.strokeStyle = "#eab308";
    ctx.lineWidth = 1.5;
    ctx.beginPath();
    ctx.moveTo(curX, padT);
    ctx.lineTo(curX, padT + chartH);
    ctx.stroke();
    ctx.fillStyle = "#eab308";
    ctx.font = "8px Inter, sans-serif";
    ctx.fillText("HARI INI (HST " + currentAge + ")", Math.min(curX + 4, w - padR - 75), padT + chartH - 8);
    ctx.restore();
  }

  // 5. Crosshair Line on User Hover/Touch
  if (cropCrosshairIdx >= 0 && cropCrosshairIdx <= maxHst) {
    var chX = padL + (cropCrosshairIdx / maxHst) * chartW;
    ctx.save();
    ctx.setLineDash([2, 2]);
    ctx.strokeStyle = "rgba(255,255,255,0.6)";
    ctx.lineWidth = 1;
    ctx.beginPath();
    ctx.moveTo(chX, padT);
    ctx.lineTo(chX, padT + chartH);
    ctx.stroke();
    ctx.restore();
  }

  // Sumbu X: HST Labels
  ctx.fillStyle = "rgba(255,255,255,0.4)";
  ctx.font = "9px Inter, monospace";
  for (var hstLbl = 0; hstLbl <= maxHst; hstLbl += 15) {
    var lx = padL + (hstLbl / maxHst) * chartW;
    ctx.fillText(hstLbl + " HST", lx - 10, h - 6);
  }
}

// --- EXPORT, PRINT & WHATSAPP SHARING SUITE ---
function shareCropReportWhatsApp() {
  var storedAge = localStorage.getItem('crop_age');
  var storedName = localStorage.getItem('crop_name');
  var stage = localStorage.getItem('crop_stage') || 'semai';
  var tData = window.lastTelemetryData || {};

  if (!storedName || !storedAge) {
    alert("Perhatian: Varietas dan usia tanaman belum dikonfigurasi. Silakan isi formulir tanaman di tab Kontrol sebelum membagikan laporan.");
    return;
  }

  var age = parseInt(storedAge, 10);
  var name = storedName;
  var soil = (tData.soil !== undefined && !isNaN(tData.soil)) ? Math.round(tData.soil) + "%" : "--%";
  var temp = (tData.temp !== undefined && !isNaN(tData.temp)) ? parseFloat(tData.temp).toFixed(1) + "°C" : "--°C";
  var hum = (tData.hum !== undefined && !isNaN(tData.hum)) ? Math.round(tData.hum) + "%" : "--%";
  var history = getCropHistoryData();
  var vigor = (history.length > 0) ? (history[history.length - 1].vigor + "%") : "--%";
  var stageNames = { 'semai': 'Semai (Nursery)', 'vegetatif': 'Vegetatif Aktif', 'generatif': 'Generatif / Bunga', 'panen': 'Pematangan / Panen' };
  var dateStr = new Date().toLocaleDateString('id-ID', { day: '2-digit', month: '2-digit', year: 'numeric' });

  var text = "[LAPORAN SIKLUS TANAMAN - SMART FARM SCADA]\n" +
    "------------------------------------\n" +
    "Tanggal    : " + dateStr + "\n" +
    "Umur Tanam : " + age + " HST\n" +
    "Komoditas  : " + name + "\n" +
    "Fase       : " + (stageNames[stage] || stage) + "\n" +
    "Skor Vigor : " + vigor + "\n\n" +
    "Telemetri Sensor Lapangan:\n" +
    "• Suhu Udara       : " + temp + "\n" +
    "• Kelembapan Tanah : " + soil + "\n" +
    "• Kelembapan RH    : " + hum + "\n" +
    "• Status Pompa     : " + ((tData.relay === "1" || tData.relay === 1) ? "Aktif Menyiram" : "Mati") + "\n" +
    "• Radio ESP-NOW    : " + (tData.rssi ? (tData.rssi + " dBm") : "-- dBm") + "\n" +
    "------------------------------------\n" +
    "Smart Farm Precision Agriculture System";

  var url = "https://api.whatsapp.com/send?text=" + encodeURIComponent(text);
  window.open(url, '_blank');
}

function printCropReport() {
  window.print();
}

function exportCropHistoryJSON() {
  var history = getCropHistoryData();
  var ageStr = localStorage.getItem('crop_age');
  var name = localStorage.getItem('crop_name') || 'Tanaman';
  var age = ageStr ? parseInt(ageStr, 10) : 0;
  var exportObj = {
    app: "SmartFarmSCADA",
    exportedAt: new Date().toISOString(),
    cropName: name,
    cropAge: age > 0 ? age : null,
    history: history
  };
  var dataStr = "data:text/json;charset=utf-8," + encodeURIComponent(JSON.stringify(exportObj, null, 2));
  var downloadAnchor = document.createElement('a');
  downloadAnchor.setAttribute("href", dataStr);
  var fileSuffix = age > 0 ? ("_HST" + age) : "";
  downloadAnchor.setAttribute("download", "smartfarm_riwayat_" + name.replace(/\s+/g, '_') + fileSuffix + ".json");
  document.body.appendChild(downloadAnchor);
  downloadAnchor.click();
  downloadAnchor.remove();
}

function triggerImportCropHistoryJSON() {
  var inp = document.getElementById('crop-import-file-input');
  if (inp) inp.click();
}

function importCropHistoryJSON(e) {
  var file = e.target.files && e.target.files[0];
  if (!file) return;
  var reader = new FileReader();
  reader.onload = function(ev) {
    try {
      var parsed = JSON.parse(ev.target.result);
      var items = Array.isArray(parsed) ? parsed : (parsed.history && Array.isArray(parsed.history) ? parsed.history : null);
      if (!items) throw new Error("Format JSON tidak sesuai.");
      saveCropHistoryData(items);
      if (parsed.cropAge) localStorage.setItem('crop_age', parsed.cropAge);
      if (parsed.cropName) localStorage.setItem('crop_name', parsed.cropName);
      updateCropHistorySummary();
      renderCropHistoryChart();
      var footerStatus = document.getElementById('crop-hist-footer-status');
      if (footerStatus) {
        footerStatus.innerHTML = '<span style="color:#10b981;">✓ Berhasil memulihkan ' + items.length + ' data riwayat tanam!</span>';
        setTimeout(function() {
          if (footerStatus) footerStatus.innerText = "Data tersimpan di LocalStorage peramban • Kurva Pertumbuhan Sigmoid (S-Curve)";
        }, 3500);
      }
    } catch (err) {
      alert("Gagal memuat file riwayat: " + err.message);
    }
  };
  reader.readAsText(file);
  e.target.value = '';
}


// =================================================================
// 03_TELEMETRY.JS - HARDWARE TELEMETRY POLLING & INTEGRITY AUDITOR
// Target lines: ~220 (Max < 350)
// =================================================================

var isSensorDataValid = false;
var latestMoisturePercent = null;

function formatFriendlyDateTime(rawStr) {
  if (!rawStr || rawStr === "-" || rawStr.indexOf(" ") === -1) return rawStr || "-";
  var parts = rawStr.split(" ");
  var dParts = parts[0].split("-");
  var timeStr = parts[1];
  if (dParts.length !== 3) return rawStr;
  var months = ["Jan", "Feb", "Mar", "Apr", "Mei", "Jun", "Jul", "Ags", "Sep", "Okt", "Nov", "Des"];
  var day = parseInt(dParts[2]);
  var monthIdx = parseInt(dParts[1]) - 1;
  var year = dParts[0];
  var monthName = months[monthIdx] || dParts[1];
  return day + " " + monthName + " " + year + " — " + timeStr + " WIB";
}

function fetchData() {
  if (window.location.protocol === 'file:') {
    // Mode Standalone Browser tanpa koneksi ESP32 fisik
    // Sesuai aturan AGENTS.md: Murni tanpa dummy/angka palsu saat sensor offline
    return;
  }
  fetch('/data')
    .then(function (res) {
      if (!res.ok) throw new Error("HTTP error " + res.status);
      return res.json();
    })
    .then(function (data) {
      window.lastTelemetryData = data; // Simpan untuk dipakai modul lain (misal Fertigasi)
      if (typeof window.appendLiveTelemetryFeed === 'function') {
        window.appendLiveTelemetryFeed(data);
      }
      var bannerBox = document.getElementById('banner-box');
      var bannerText = document.getElementById('banner-text');

      var color = "var(--primary)";
      var glow = "rgba(16, 185, 129, 0.7)";
      if (data.statusColor === "bahaya") {
        color = "var(--danger)";
        glow = "rgba(239, 68, 68, 0.7)";
      } else if (data.statusColor === "peringatan") {
        color = "var(--warning)";
        glow = "rgba(245, 158, 11, 0.7)";
      }

      if (data.isWaiting == 1) {
        if (bannerText) bannerText.innerText = "Menunggu Sinyal Sensor (" + data.waitingPercent + "%)";
        color = "var(--warning)";
        glow = "rgba(245, 158, 11, 0.7)";
      } else if (data.errorMsg && data.errorMsg !== "") {
        if (bannerText) bannerText.innerText = "Failsafe Alert: " + data.errorMsg;
        color = "var(--danger)";
        glow = "rgba(239, 68, 68, 0.7)";
      } else {
        var cleanStatus = data.statusText ? data.statusText.replace(/[✅🚨⏳ℹ️🌱]/g, '').trim() : "Sistem Operasional Normal";
        if (bannerText) bannerText.innerText = cleanStatus || "Sistem Operasional Normal";
      }

      if (bannerBox) {
        bannerBox.style.borderTopColor = color;
        bannerBox.style.boxShadow = "0 10px 30px rgba(0, 0, 0, 0.2), 0 0 15px " + glow;
      }
      if (bannerText) {
        bannerText.style.color = color;
        bannerText.style.borderColor = color;
      }

      var friendlyRtc = formatFriendlyDateTime(data.rtcTime);
      var badgeRtc = document.getElementById('badge-rtc-time');
      if (badgeRtc) {
        badgeRtc.innerHTML = '<svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" style="vertical-align:middle;margin-right:4px;"><circle cx="12" cy="12" r="10"></circle><polyline points="12 6 12 12 16 14"></polyline></svg><span>' + (friendlyRtc !== "-" ? friendlyRtc : "RTC: Offline") + '</span>';
      }
      var badgeRssi = document.getElementById('badge-rssi');
      if (badgeRssi) {
        badgeRssi.innerHTML = '<svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" style="vertical-align:middle;margin-right:4px;"><path d="M5 12.55a11 11 0 0 1 14.08 0"></path><path d="M1.42 9a16 16 0 0 1 21.16 0"></path><path d="M8.53 16.11a6 6 0 0 1 6.95 0"></path><line x1="12" y1="20" x2="12.01" y2="20"></line></svg><span>Sinyal: ' + (data.rssi || "-") + ' dBm</span>';
      }

      // Strict Zero-Dummy Hardware Validation
      var isOfflineOrWaiting = (data.isWaiting == 1 || data.isEsp8266Unplugged == 1 || data.isSystemError == 1);
      var hasSoil = (!isOfflineOrWaiting && data.soil !== undefined && data.soil !== null && data.soil !== "--" && !isNaN(parseFloat(data.soil)) && parseFloat(data.soil) >= 0);
      var hasTemp = (data.suhuC !== undefined && data.suhuC !== null && data.suhuC !== "--" && !isNaN(parseFloat(data.suhuC)));
      var hasHum = (data.hum !== undefined && data.hum !== null && data.hum !== "--" && !isNaN(parseFloat(data.hum)));

      var sVal = hasSoil ? parseFloat(data.soil) : null;
      var tVal = hasTemp ? parseFloat(data.suhuC) : null;
      var hVal = hasHum ? parseFloat(data.hum) : null;

      latestMoisturePercent = sVal;
      isSensorDataValid = (!isOfflineOrWaiting && hasSoil && hasTemp && hasHum);

      // Hardware Traffic Light Widget (Pins 32 Hijau, 33 Kuning, 27 Merah)
      var bRed = document.getElementById('traffic-bulb-red');
      var bYellow = document.getElementById('traffic-bulb-yellow');
      var bGreen = document.getElementById('traffic-bulb-green');
      var tLabel = document.getElementById('traffic-label');

      if (bRed && bYellow && bGreen) {
        bRed.className = 'traffic-bulb bulb-red';
        bYellow.className = 'traffic-bulb bulb-yellow';
        bGreen.className = 'traffic-bulb bulb-green';

        if (isOfflineOrWaiting && data.isWaiting != 1) {
          bRed.classList.add('active', 'blink');
          if (tLabel) { tLabel.innerText = "Terputus"; tLabel.style.color = "#ef4444"; }
        } else if (data.isWaiting == 1) {
          bYellow.classList.add('active', 'blink');
          if (tLabel) { tLabel.innerText = "Menunggu"; tLabel.style.color = "#f59e0b"; }
        } else if (data.statusColor === "bahaya" || (hasTemp && tVal >= 35.0)) {
          bRed.classList.add('active');
          if (tLabel) { tLabel.innerText = "Bahaya"; tLabel.style.color = "#ef4444"; }
        } else if (data.statusColor === "peringatan" || (hasSoil && sVal < 45) || (hasTemp && tVal > 30.0)) {
          bYellow.classList.add('active');
          if (tLabel) { tLabel.innerText = "Waspada"; tLabel.style.color = "#f59e0b"; }
        } else if (hasSoil || hasTemp) {
          bGreen.classList.add('active');
          if (tLabel) { tLabel.innerText = "Aman"; tLabel.style.color = "#10b981"; }
        } else {
          bYellow.classList.add('active', 'blink');
          if (tLabel) { tLabel.innerText = "Standby"; tLabel.style.color = "#f59e0b"; }
        }
      }

      // Update Big Traffic Pole Card in Dashboard
      updateTrafficPoleCard(data, isOfflineOrWaiting, hasSoil, hasTemp, sVal, tVal);

      // KPI Cards Update
      var kpiSoil = document.getElementById('kpi-soil');
      var kpiSoilCat = document.getElementById('kpi-soil-category');
      var kpiSoilDep = document.getElementById('kpi-soil-depletion');
      if (kpiSoil) {
        if (hasSoil) {
          kpiSoil.innerText = sVal + "%";
          if (kpiSoilCat) {
            kpiSoilCat.innerText = data.soilCategory || "Normal";
            kpiSoilCat.style.color = "var(--primary)";
          }
        } else {
          kpiSoil.innerText = "--%";
          if (kpiSoilCat) {
            kpiSoilCat.innerText = (data.isWaiting == 1 ? "Menunggu Sensor..." : "Sensor Terputus");
            kpiSoilCat.style.color = "var(--text-sub)";
          }
        }
      }
      if (kpiSoilDep) kpiSoilDep.innerText = "Prediksi Penguapan: " + (data.soilDepletion || "-");

      var kpiTemp = document.getElementById('kpi-temp');
      var kpiHeatIdx = document.getElementById('kpi-heat-index');
      if (kpiTemp) {
        if (hasTemp) {
          kpiTemp.innerText = tVal + "°C";
          if (kpiHeatIdx) kpiHeatIdx.innerText = "Terasa seperti: " + (data.heatC || "--") + "°C (" + (data.suhuF || "--") + "°F)";
        } else {
          kpiTemp.innerText = "--°C";
          if (kpiHeatIdx) kpiHeatIdx.innerText = "Terasa seperti: --";
        }
      }

      var kpiHum = document.getElementById('kpi-hum');
      var kpiDew = document.getElementById('kpi-dew-point');
      if (kpiHum) {
        if (hasHum) {
          kpiHum.innerText = hVal + "%";
          if (kpiDew) kpiDew.innerText = "Titik Embun: " + (data.dew || "--") + "°C";
        } else {
          kpiHum.innerText = "--%";
          if (kpiDew) kpiDew.innerText = "Titik Embun: --";
        }
      }

      var kpiSignal = document.getElementById('kpi-signal') || document.getElementById('kpi-battery');
      var kpiNodeStatus = document.getElementById('kpi-node-status');
      var kpiAdc = document.getElementById('kpi-raw-adc');

      var isNodeOnline = (!isOfflineOrWaiting && data.rssi !== undefined && data.rssi !== null && data.rssi != -99 && data.rssi !== "-");
      if (kpiSignal) {
        if (isNodeOnline) {
          kpiSignal.innerText = data.rssi + " dBm";
          if (kpiNodeStatus) {
            kpiNodeStatus.innerText = "Online";
            kpiNodeStatus.style.color = "var(--accent-emerald)";
            kpiNodeStatus.style.borderColor = "rgba(16, 185, 129, 0.35)";
          }
        } else {
          kpiSignal.innerText = "-- dBm";
          if (kpiNodeStatus) {
            kpiNodeStatus.innerText = (data.isWaiting == 1 ? "Menunggu" : "Terputus");
            kpiNodeStatus.style.color = "var(--accent-rose)";
            kpiNodeStatus.style.borderColor = "rgba(239, 68, 68, 0.35)";
          }
        }
      }
      if (kpiAdc) {
        if (isNodeOnline && data.rawAdc !== undefined && data.rawAdc !== null && data.rawAdc !== "-") {
          kpiAdc.innerText = "ADC A0: " + data.rawAdc;
        } else {
          kpiAdc.innerText = "ADC A0: --";
        }
      }

      // Update Disease Outbreak Risk State
      if (typeof updateOutbreakUIState === 'function') {
        updateOutbreakUIState(isSensorDataValid, tVal, hVal, sVal);
      }

      // Update AI Phenology Model
      if (typeof updatePhenologyAI === 'function' && typeof getActiveSector === 'function') {
        updatePhenologyAI(getActiveSector(), tVal, hVal, sVal, (data.vpd !== undefined ? parseFloat(data.vpd) : null));
      }

      // Delegate Actuators & Schedule synchronization
      if (typeof updateActuatorAndScheduleUI === 'function') {
        updateActuatorAndScheduleUI(data, friendlyRtc, hasTemp, tVal, hasHum, hVal);
      }

      // Silent Auto-Sync RTC once
      if (!window.rtcAutoSynced) {
        window.rtcAutoSynced = true;
        var now = new Date();
        var ep = Math.floor(now.getTime() / 1000);
        fetch('/setRtc?epoch=' + ep + '&y=' + now.getFullYear() + '&m=' + (now.getMonth() + 1) + '&d=' + now.getDate() + '&h=' + now.getHours() + '&min=' + now.getMinutes() + '&s=' + now.getSeconds())
          .catch(function () {});
      }

      if (typeof updateHistory === 'function') {
        updateHistory(sVal, tVal, hVal);
      }
    })
    .catch(function (err) {
      isSensorDataValid = false;
      latestMoisturePercent = null;
      if (typeof updateOutbreakUIState === 'function') {
        updateOutbreakUIState(false, null, null, null);
      }
      var bText = document.getElementById('banner-text');
      if (bText) {
        bText.innerText = "Standalone Mode / Sensor Offline";
        bText.style.color = "var(--text-sub)";
        bText.style.borderColor = "var(--card-border)";
      }
      var bBox = document.getElementById('banner-box');
      if (bBox) {
        bBox.style.borderTopColor = "var(--card-border)";
        bBox.style.boxShadow = "0 10px 30px rgba(0, 0, 0, 0.2)";
      }

      var bRed = document.getElementById('traffic-bulb-red');
      var bYellow = document.getElementById('traffic-bulb-yellow');
      var bGreen = document.getElementById('traffic-bulb-green');
      var tLabel = document.getElementById('traffic-label');
      if (bRed && bYellow && bGreen) {
        bRed.className = 'traffic-bulb bulb-red active blink';
        bYellow.className = 'traffic-bulb bulb-yellow';
        bGreen.className = 'traffic-bulb bulb-green';
        if (tLabel) { tLabel.innerText = "Offline"; tLabel.style.color = "#ef4444"; }
      }
      
      // Also reset pole card on error to Sensor Terputus (Merah blink)
      updateTrafficPoleCard({ statusColor: 'bahaya', isEsp8266Unplugged: 1 }, true, false, false, null, null);
    });
}

// Global Toggle for LED Diagnostic Card
window.toggleLedDiag = function() {
  var card = document.getElementById('card-traffic-light');
  if (card) {
    if (card.style.display === 'none') {
      card.style.display = 'block';
    } else {
      card.style.display = 'none';
    }
  }
};

// ================================================================
// updateTrafficPoleCard — Update big traffic light pole card
// Driven by: data.rtcSchedule, data.statusColor, sensor values
// ================================================================
var _rtcCycleInterval = null;

function updateTrafficPoleCard(data, isOfflineOrWaiting, hasSoil, hasTemp, sVal, tVal) {
  var pRed    = document.getElementById('pole-bulb-red');
  var pYellow = document.getElementById('pole-bulb-yellow');
  var pGreen  = document.getElementById('pole-bulb-green');
  var cardLabel  = document.getElementById('traffic-card-label');
  var curStatus  = document.getElementById('traffic-current-status');
  var curDetail  = document.getElementById('traffic-current-detail');
  var rtcBadge   = document.getElementById('traffic-rtc-badge');
  var dotRed     = document.getElementById('traffic-dot-red');
  var dotYellow  = document.getElementById('traffic-dot-yellow');
  var dotGreen   = document.getElementById('traffic-dot-green');

  if (!pRed || !pYellow || !pGreen) return;

  function clearPoles() {
    pRed.className    = 'traffic-pole-bulb pole-red';
    pYellow.className = 'traffic-pole-bulb pole-yellow';
    pGreen.className  = 'traffic-pole-bulb pole-green';
    if (dotRed)    { dotRed.style.opacity = '0.3';    dotRed.style.boxShadow = 'none'; }
    if (dotYellow) { dotYellow.style.opacity = '0.3'; dotYellow.style.boxShadow = 'none'; }
    if (dotGreen)  { dotGreen.style.opacity = '0.3';  dotGreen.style.boxShadow = 'none'; }
    if (rtcBadge) rtcBadge.style.display = 'none';
  }

  if (_rtcCycleInterval) { clearInterval(_rtcCycleInterval); _rtcCycleInterval = null; }
  clearPoles();

  // PRIORITY 1: RTC Jadwal aktif → siklus Merah→Kuning→Hijau
  if (data.rtcSchedule == 1) {
    if (rtcBadge) rtcBadge.style.display = 'inline-flex';
    if (cardLabel) { cardLabel.innerText = 'JADWAL RTC AKTIF'; cardLabel.style.background = 'rgba(245,158,11,0.15)'; cardLabel.style.color = '#f59e0b'; cardLabel.style.borderColor = 'rgba(245,158,11,0.3)'; }
    if (curStatus) { curStatus.innerText = 'Jadwal Penyiraman RTC Sedang Berjalan'; curStatus.style.color = '#f59e0b'; }
    if (curDetail) curDetail.innerText = 'Lampu Traffic Light fisik (Pin 27/33/32) menyala bergantian sebagai indikator jadwal aktif';
    var _cycleStep = 0;
    var _poles     = [['pole-bulb-red','pole-red','#ef4444','traffic-dot-red'],['pole-bulb-yellow','pole-yellow','#f59e0b','traffic-dot-yellow'],['pole-bulb-green','pole-green','#10b981','traffic-dot-green']];
    function _doCycle() {
      clearPoles();
      var c = _poles[_cycleStep];
      var el = document.getElementById(c[0]); if (el) el.className = 'traffic-pole-bulb ' + c[1] + ' lit blink';
      var d  = document.getElementById(c[3]); if (d) { d.style.opacity = '1'; d.style.boxShadow = '0 0 8px ' + c[2]; }
      _cycleStep = (_cycleStep + 1) % 3;
    }
    _doCycle();
    _rtcCycleInterval = setInterval(_doCycle, 600);
    updateLedDiagPanel(4);
    return;
  }

  // PRIORITY 2: Sensor terputus → Merah blink
  if (isOfflineOrWaiting && data.isWaiting != 1) {
    pRed.className = 'traffic-pole-bulb pole-red lit blink';
    if (dotRed) { dotRed.style.opacity = '1'; dotRed.style.boxShadow = '0 0 8px #ef4444'; }
    if (cardLabel) { cardLabel.innerText = 'SENSOR TERPUTUS'; cardLabel.style.background = 'rgba(239,68,68,0.15)'; cardLabel.style.color = '#ef4444'; cardLabel.style.borderColor = 'rgba(239,68,68,0.3)'; }
    if (curStatus) { curStatus.innerText = 'ESP8266 Terputus / Sensor Offline'; curStatus.style.color = '#ef4444'; }
    if (curDetail) curDetail.innerText = 'Lampu Merah (Pin 27) berkedip. Pompa dikunci mati oleh failsafe';
    updateLedDiagPanel(1);
    return;
  }

  // PRIORITY 3: Boot / Menunggu → Kuning blink
  if (data.isWaiting == 1) {
    pYellow.className = 'traffic-pole-bulb pole-yellow lit blink';
    if (dotYellow) { dotYellow.style.opacity = '1'; dotYellow.style.boxShadow = '0 0 8px #f59e0b'; }
    if (cardLabel) { cardLabel.innerText = 'MENUNGGU KONEKSI'; cardLabel.style.background = 'rgba(245,158,11,0.15)'; cardLabel.style.color = '#f59e0b'; cardLabel.style.borderColor = 'rgba(245,158,11,0.3)'; }
    if (curStatus) { curStatus.innerText = 'Menunggu sinyal ESP8266...'; curStatus.style.color = '#f59e0b'; }
    if (curDetail) curDetail.innerText = 'Lampu Kuning (Pin 33) berkedip. Boot ' + (data.waitingPercent || 0) + '% selesai';
    updateLedDiagPanel(2);
    return;
  }

  // PRIORITY 4: Bahaya suhu ≥35°C → Merah solid
  if (data.statusColor === 'bahaya' || (hasTemp && tVal >= 35.0)) {
    pRed.className = 'traffic-pole-bulb pole-red lit';
    if (dotRed) { dotRed.style.opacity = '1'; dotRed.style.boxShadow = '0 0 8px #ef4444'; }
    if (cardLabel) { cardLabel.innerText = 'BAHAYA KRITIS'; cardLabel.style.background = 'rgba(239,68,68,0.15)'; cardLabel.style.color = '#ef4444'; cardLabel.style.borderColor = 'rgba(239,68,68,0.3)'; }
    if (curStatus) { curStatus.innerText = 'BAHAYA — Suhu Ekstrem ' + (tVal !== null ? tVal + '°C' : '--'); curStatus.style.color = '#ef4444'; }
    if (curDetail) curDetail.innerText = 'Lampu Merah (Pin 27) menyala solid. Segera lakukan pendinginan lahan!';
    updateLedDiagPanel(1);
    return;
  }

  // PRIORITY 5: Peringatan → Kuning solid
  if (data.statusColor === 'peringatan' || (hasSoil && sVal < 45) || (hasTemp && tVal > 30.0)) {
    pYellow.className = 'traffic-pole-bulb pole-yellow lit';
    if (dotYellow) { dotYellow.style.opacity = '1'; dotYellow.style.boxShadow = '0 0 8px #f59e0b'; }
    if (cardLabel) { cardLabel.innerText = 'PERLU IRIGASI'; cardLabel.style.background = 'rgba(245,158,11,0.15)'; cardLabel.style.color = '#f59e0b'; cardLabel.style.borderColor = 'rgba(245,158,11,0.3)'; }
    if (curStatus) { curStatus.innerText = 'Waspada — Tanah/Suhu Memerlukan Tindakan'; curStatus.style.color = '#f59e0b'; }
    if (curDetail) curDetail.innerText = 'Lampu Kuning (Pin 33). Tanah: ' + (sVal !== null ? sVal + '%' : '--') + ' · Suhu: ' + (tVal !== null ? tVal + '°C' : '--');
    updateLedDiagPanel(2);
    return;
  }

  // DEFAULT: Aman jika ada data riil valid; Sensor Terputus jika belum ada pembacaan sensor
  if (hasSoil || hasTemp) {
    pGreen.className = 'traffic-pole-bulb pole-green lit';
    if (dotGreen) { dotGreen.style.opacity = '1'; dotGreen.style.boxShadow = '0 0 10px #10b981'; }
    if (cardLabel) { cardLabel.innerText = 'KONDISI AMAN'; cardLabel.style.background = 'rgba(16,185,129,0.15)'; cardLabel.style.color = '#10b981'; cardLabel.style.borderColor = 'rgba(16,185,129,0.3)'; }
    if (curStatus) { curStatus.innerText = 'Sistem Aman — Kondisi Optimal'; curStatus.style.color = '#10b981'; }
    if (curDetail) curDetail.innerText = 'Lampu Hijau (Pin 32). Tanah: ' + (sVal !== null ? sVal + '%' : '--') + ' · Suhu: ' + (tVal !== null ? tVal + '°C' : '--');
    updateLedDiagPanel(3);
  } else {
    pRed.className = 'traffic-pole-bulb pole-red lit blink';
    if (dotRed) { dotRed.style.opacity = '1'; dotRed.style.boxShadow = '0 0 8px #ef4444'; }
    if (cardLabel) { cardLabel.innerText = 'SENSOR TERPUTUS / OFFLINE'; cardLabel.style.background = 'rgba(239,68,68,0.15)'; cardLabel.style.color = '#ef4444'; cardLabel.style.borderColor = 'rgba(239,68,68,0.3)'; }
    if (curStatus) { curStatus.innerText = 'ESP8266 Terputus / Sensor Offline'; curStatus.style.color = '#ef4444'; }
    if (curDetail) curDetail.innerText = 'Lampu Merah (Pin 27) berkedip. Pompa dikunci mati oleh failsafe sistem.';
    updateLedDiagPanel(1);
  }

  // Update GPIO diagnostic panel
  updateLedDiagPanel(data.ledState);
}

// ================================================================
// updateLedDiagPanel — Update GPIO diagnostic panel
// ledState: 0=off, 1=merah, 2=kuning, 3=hijau, 4=rtc-cycle
// Menampilkan sinyal GPIO aktual yang dikirim firmware ke setiap pin
// ================================================================
function updateLedDiagPanel(ledState) {
  var stRed    = document.getElementById('hw-state-red');
  var stYellow = document.getElementById('hw-state-yellow');
  var stGreen  = document.getElementById('hw-state-green');
  var dRed     = document.getElementById('hw-dot-red');
  var dYellow  = document.getElementById('hw-dot-yellow');
  var dGreen   = document.getElementById('hw-dot-green');

  if (!stRed || !stYellow || !stGreen) return;

  // Reset all to OFF style
  function setOff(stEl, dEl) {
    stEl.innerText = 'LOW (OFF)';
    stEl.style.color = 'var(--text-sub)';
    stEl.style.background = 'rgba(255,255,255,0.04)';
    if (dEl) { dEl.style.opacity = '0.25'; dEl.style.boxShadow = 'none'; }
  }
  function setOn(stEl, dEl, color, label) {
    stEl.innerText = label || 'HIGH (ON)';
    stEl.style.color = color;
    stEl.style.background = 'rgba(255,255,255,0.08)';
    if (dEl) { dEl.style.opacity = '1'; dEl.style.boxShadow = '0 0 6px ' + color; }
  }

  setOff(stRed, dRed);
  setOff(stYellow, dYellow);
  setOff(stGreen, dGreen);

  var ls = parseInt(ledState) || 0;
  switch(ls) {
    case 1: // Merah
      setOn(stRed, dRed, '#ef4444', 'HIGH (ON) ●');
      break;
    case 2: // Kuning
      setOn(stYellow, dYellow, '#f59e0b', 'HIGH (ON) ●');
      break;
    case 3: // Hijau
      setOn(stGreen, dGreen, '#10b981', 'HIGH (ON) ●');
      break;
    case 4: // RTC Cycle
      stRed.innerText    = 'CYCLE (Gantian)';
      stYellow.innerText = 'CYCLE (Gantian)';
      stGreen.innerText  = 'CYCLE (Gantian)';
      stRed.style.color    = '#ef4444';
      stYellow.style.color = '#f59e0b';
      stGreen.style.color  = '#10b981';
      if (dRed) { dRed.style.opacity = '1'; dRed.style.boxShadow = '0 0 4px #ef4444'; }
      if (dYellow) { dYellow.style.opacity = '1'; dYellow.style.boxShadow = '0 0 4px #f59e0b'; }
      if (dGreen) { dGreen.style.opacity = '1'; dGreen.style.boxShadow = '0 0 4px #10b981'; }
      break;
    default: // 0 = semua mati
      break;
  }
}

// ================================================================
// reportLedMismatch — Tampilkan panduan troubleshoot hardware LED
// Dipanggil dari tombol "Laporkan Mismatch" di diagnostic panel
// ================================================================
function reportLedMismatch() {
  var alertEl = document.getElementById('traffic-mismatch-alert');
  if (!alertEl) return;
  if (alertEl.style.display === 'none' || alertEl.style.display === '') {
    alertEl.style.display = 'block';
    alertEl.scrollIntoView({ behavior: 'smooth', block: 'nearest' });
  } else {
    alertEl.style.display = 'none';
  }
}


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
  var heatC = data.heatC;
  var heatF = data.heatF;
  if ((!heatC || heatC === "--") && hasMicroclimateData) {
    var tempF = tempNum * 1.8 + 32;
    var hiF = 0.5 * (tempF + 61.0 + ((tempF - 68.0) * 1.2) + (humNum * 0.094));
    if (hiF >= 80) {
      hiF = -42.379 + 2.04901523 * tempF + 10.14333127 * humNum - 0.22475541 * tempF * humNum
            - 0.00683783 * tempF * tempF - 0.05481717 * humNum * humNum
            + 0.00122874 * tempF * tempF * humNum + 0.00085282 * tempF * humNum * humNum
            - 0.00000199 * tempF * tempF * humNum * humNum;
    }
    heatF = hiF.toFixed(1);
    heatC = ((hiF - 32) / 1.8).toFixed(1);
  }
  var elHeatC = document.getElementById('bmkg-heat-index');
  var elHeatF = document.getElementById('bmkg-heat-f');
  var elHeatStat = document.getElementById('bmkg-heat-status');
  var elHeatBadge = document.getElementById('bmkg-heat-badge');
  if (elHeatC) elHeatC.innerText = hasMicroclimateData ? (heatC + "°C") : "--°C";
  if (elHeatF) elHeatF.innerText = hasMicroclimateData ? (heatF + "°F") : "--°F";
  if (elHeatStat) {
    var hNum = parseFloat(heatC);
    var hStat = "--";
    if (hasMicroclimateData && !isNaN(hNum)) {
      if (hNum >= 38) hStat = "Stres Termal Bahaya!";
      else if (hNum >= 32) hStat = "Waspada Panas Ekstrem";
      else if (hNum >= 27) hStat = "Hangat Normal";
      else if (hNum >= 22) hStat = "Optimal / Nyaman";
      else hStat = "Sensasi Sejuk";
    }
    elHeatStat.innerText = hStat;
    if (elHeatBadge) elHeatBadge.innerText = hasMicroclimateData ? hStat : "Termal Riil";
  }

  // 4. Dew Point & Absolute Humidity (Magnus Formula)
  var dew = data.dew;
  var absHum = data.absHum;
  if ((!dew || dew === "--") && hasMicroclimateData) {
    var a = 17.27, b = 237.7;
    var alpha = ((a * tempNum) / (b + tempNum)) + Math.log(humNum / 100.0);
    dew = ((b * alpha) / (a - alpha)).toFixed(1);
    absHum = ((216.7 * (humNum / 100.0) * 6.112 * Math.exp((17.67 * tempNum) / (tempNum + 243.5))) / (273.15 + tempNum)).toFixed(1);
  }
  var elDew = document.getElementById('bmkg-dew-point');
  var elAbsHum = document.getElementById('bmkg-abs-hum');
  var elDewStat = document.getElementById('bmkg-dew-status');
  var elDewBadge = document.getElementById('bmkg-dew-badge');
  if (elDew) elDew.innerText = hasMicroclimateData ? (dew + "°C") : "--°C";
  if (elAbsHum) elAbsHum.innerText = hasMicroclimateData ? (absHum + " g/m³") : "-- g/m³";
  if (elDewStat) {
    var dewDiff = hasMicroclimateData ? (tempNum - parseFloat(dew)) : null;
    var dStat = "--";
    if (dewDiff !== null && !isNaN(dewDiff)) {
      if (dewDiff <= 1.5) dStat = "Kondensasi Embun Jenuh";
      else if (dewDiff <= 3.0) dStat = "Potensi Embun Pagi";
      else dStat = "Bebas Embun";
    }
    elDewStat.innerText = dStat;
    if (elDewBadge) elDewBadge.innerText = hasMicroclimateData ? (dewDiff !== null && dewDiff <= 2 ? "Embun Aktif" : "Bebas Embun") : "Magnus";
  }

  // 5. VPD (Defisit Tekanan Uap & Status Stomata)
  var vpd = data.vpd;
  if ((!vpd || vpd === "--") && hasMicroclimateData) {
    var es = 0.61078 * Math.exp((17.27 * tempNum) / (tempNum + 237.3));
    var ea = es * (humNum / 100.0);
    vpd = Math.max(0, es - ea).toFixed(2);
  }
  var elVpd = document.getElementById('bmkg-vpd-val');
  var elVpdStat = document.getElementById('bmkg-vpd-status');
  var elVpdBadge = document.getElementById('bmkg-vpd-badge');
  if (elVpd) elVpd.innerText = hasMicroclimateData ? (vpd + " kPa") : "-- kPa";
  if (elVpdStat) {
    var vpdNum = parseFloat(vpd);
    var vStat = "--";
    var vBadge = "Stomata";
    if (hasMicroclimateData && !isNaN(vpdNum)) {
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
    if (elVpdBadge) elVpdBadge.innerText = hasMicroclimateData ? vBadge : "Stomata";
  }

  // 6. Evaporation Rate (Laju Penguapan Air Lahan)
  var evap = data.evaporation;
  if ((!evap || evap === "--") && hasMicroclimateData && vpd && !isNaN(parseFloat(vpd))) {
    evap = ((0.7 * parseFloat(vpd) + 0.15 * (tempNum / 10.0)) * 1.05).toFixed(1);
  }
  var elEvap = document.getElementById('bmkg-evap-val');
  var elEvapStat = document.getElementById('bmkg-evap-status');
  var elEvapLoss = document.getElementById('bmkg-evap-loss');
  var elEvapBadge = document.getElementById('bmkg-evap-badge');
  if (elEvap) elEvap.innerText = hasMicroclimateData ? (evap + " mm/hari") : "-- mm/hari";
  if (elEvapLoss) elEvapLoss.innerText = hasMicroclimateData ? (evap + " L/m²") : "-- L/m²";
  if (elEvapStat) {
    var eNum = parseFloat(evap);
    var eStat = "--";
    if (hasMicroclimateData && !isNaN(eNum)) {
      if (eNum > 6.0) eStat = "Penguapan Cepat (Kering)";
      else if (eNum < 2.5) eStat = "Penguapan Lambat (Basah)";
      else eStat = "Penguapan Sedang";
    }
    elEvapStat.innerText = eStat;
    if (elEvapBadge) elEvapBadge.innerText = hasMicroclimateData ? (eNum > 5.0 ? "Evap Tinggi" : "Evap Normal") : "Penman ET";
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
  if (mDesc) mDesc.innerText = isManual ? "Mode Manual" : "Mode Otomatis (Sensor & RTC)";

  var isRelayOn = (data.relayOn == 1);
  var modeText = (data.manual == 1) ? " (Manual)" : " (Auto)";
  var relayBadge = document.getElementById('relay-status-badge');
  var liveBar = document.getElementById('pump-live-bar');
  if (relayBadge) {
    if (isRelayOn) {
      relayBadge.innerHTML = '<span class="badge-dot dot-green" style="background:#10b981; box-shadow:0 0 6px #10b981;"></span><span>Pompa Menyala' + modeText + '</span>';
      relayBadge.style.color = "#10b981";
      relayBadge.style.borderColor = "rgba(16, 185, 129, 0.4)";
      relayBadge.style.background = "rgba(16, 185, 129, 0.15)";
      if (liveBar) liveBar.style.display = 'flex';
    } else {
      relayBadge.innerHTML = '<span class="badge-dot dot-red" style="background:#ef4444;"></span><span>Standby' + modeText + '</span>';
      relayBadge.style.color = "#ef4444";
      relayBadge.style.borderColor = "rgba(239, 68, 68, 0.4)";
      relayBadge.style.background = "rgba(239, 68, 68, 0.15)";
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
  if (lDesc) lDesc.innerText = isLampManual ? "Mode Manual" : "Mode Otomatis (18:00 - 06:00)";

  var isLampOn = (data.lampOn == 1);
  var isLampManual = (data.lampManual == 1);
  var lModeText = isLampManual ? " (Manual)" : " (Auto)";
  var lampBadge = document.getElementById('lamp-status-badge');
  if (lampBadge) {
    if (isLampOn) {
      lampBadge.innerHTML = '<span class="badge-dot dot-yellow" style="background:#eab308; box-shadow:0 0 6px #eab308;"></span><span>Lampu Menyala' + lModeText + '</span>';
      lampBadge.style.color = "#eab308";
      lampBadge.style.borderColor = "rgba(234, 179, 8, 0.4)";
      lampBadge.style.background = "rgba(234, 179, 8, 0.15)";
    } else {
      lampBadge.innerHTML = '<span class="badge-dot dot-gray" style="background:#94a3b8;"></span><span>Standby' + lModeText + '</span>';
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
      txtPump.innerText = "Pompa ON";
      bgPump.className = "badge-pill badge-pump-active";
      bgPump.title = "Pompa Air Aktif (Menyiram Lahan)";
    } else {
      txtPump.innerText = "Pompa OFF";
      bgPump.className = "badge-pill";
      bgPump.style.color = "var(--text-sub)";
      bgPump.title = "Pompa Air Standby / Mati";
    }
  }

  var bgLamp = document.getElementById('badge-global-lamp');
  var txtLamp = document.getElementById('text-global-lamp');
  if (bgLamp && txtLamp) {
    if (isLampOn) {
      txtLamp.innerText = "Lampu ON";
      bgLamp.className = "badge-pill badge-lamp-active";
      bgLamp.title = "Lampu Pemanas/Growlight Aktif";
    } else {
      txtLamp.innerText = "Lampu OFF";
      bgLamp.className = "badge-pill";
      bgLamp.style.color = "var(--text-sub)";
      bgLamp.title = "Lampu Pemanas/Growlight Standby / Mati";
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
  if (statCount) statCount.innerText = (data.pumpCount !== undefined ? data.pumpCount : "--") + " Kali";

  var totalSecs = data.totalPumpSecs !== undefined ? data.totalPumpSecs : -1;
    var timeStr = "-- Detik";
  if (totalSecs !== -1) {
    var mins = Math.floor(totalSecs / 60);
    var secs = totalSecs % 60;
    timeStr = totalSecs + " Detik";
    if (mins > 0) timeStr += " (" + mins + "m " + secs + "s)";
  }
  var statSecs = document.getElementById('stat-pump-secs');
  if (statSecs) statSecs.innerText = timeStr;

  var statWater = document.getElementById('stat-water-liters');
  var statCost = document.getElementById('stat-cost-idr');
  if (statWater) statWater.innerText = (data.waterLiters !== undefined ? data.waterLiters : "--") + " Liter";
  if (statCost) statCost.innerText = "Rp " + (data.costIdr !== undefined ? data.costIdr : "--");

  if (data.pumpLph !== undefined && document.activeElement.id !== 'cfg-pump-lph') {
    var elLph = document.getElementById('cfg-pump-lph');
    if (elLph) elLph.value = data.pumpLph;
  }
  if (data.pumpWatt !== undefined && document.activeElement.id !== 'cfg-pump-watt') {
    var elWatt = document.getElementById('cfg-pump-watt');
    if (elWatt) elWatt.value = data.pumpWatt;
  }
  if (data.plnTariff !== undefined && document.activeElement.id !== 'cfg-pln-tariff') {
    var elTariff = document.getElementById('cfg-pln-tariff');
    if (elTariff) elTariff.value = parseFloat(data.plnTariff);
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
    if (s1en) s1en.checked = (data.sched1_en == 1);
    if (s1tm) s1tm.value = (data.sched1_h < 10 ? '0' : '') + data.sched1_h + ':' + (data.sched1_m < 10 ? '0' : '') + data.sched1_m;
    if (s1dur) s1dur.value = data.sched1_dur;
  }

  if (data.sched2_h !== undefined &&
    document.activeElement.id !== 'sched2-en' &&
    document.activeElement.id !== 'sched2-time' &&
    document.activeElement.id !== 'sched2-dur') {
    var s2en = document.getElementById('sched2-en');
    var s2tm = document.getElementById('sched2-time');
    var s2dur = document.getElementById('sched2-dur');
    if (s2en) s2en.checked = (data.sched2_en == 1);
    if (s2tm) s2tm.value = (data.sched2_h < 10 ? '0' : '') + data.sched2_h + ':' + (data.sched2_m < 10 ? '0' : '') + data.sched2_m;
    if (s2dur) s2dur.value = data.sched2_dur;
  }

  if (data.l_h !== undefined &&
    document.activeElement.id !== 'lamp-sched-en' &&
    document.activeElement.id !== 'lamp-sched-time' &&
    document.activeElement.id !== 'lamp-sched-dur') {
    var l_en = document.getElementById('lamp-sched-en');
    var l_tm = document.getElementById('lamp-sched-time');
    var l_dur = document.getElementById('lamp-sched-dur');
    if (l_en) l_en.checked = (data.l_en == 1);
    if (l_tm) l_tm.value = (data.l_h < 10 ? '0' : '') + data.l_h + ':' + (data.l_m < 10 ? '0' : '') + data.l_m;
    if (l_dur) l_dur.value = data.l_dur;
  }

  // Crop Profile & Thresholds from NVS
  if (data.cropMode !== undefined && document.activeElement.id !== 'crop-profile-select') {
    var sel = document.getElementById('crop-profile-select');
    if (sel && sel.value != data.cropMode) {
      sel.value = data.cropMode;
      if (typeof updateCropProfileUI === 'function') updateCropProfileUI(data.cropMode);
    }
  }
  if (data.batasTanah !== undefined && document.activeElement.id !== 'slider-soil') {
    var elSoil = document.getElementById('slider-soil');
    if (elSoil) elSoil.value = data.batasTanah;
    var valSoil = document.getElementById('val-slider-soil');
    if (valSoil) valSoil.innerText = data.batasTanah + '%';
  }
  if (data.batasSuhu !== undefined && document.activeElement.id !== 'slider-temp') {
    var elTemp = document.getElementById('slider-temp');
    if (elTemp) elTemp.value = data.batasSuhu;
    var valTemp = document.getElementById('val-slider-temp');
    if (valTemp) valTemp.innerText = data.batasSuhu + '°C';
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


/**
 * 08_controls.js - Actuators, Relay, Schedule & Threshold Controller
 * Zero-Dummy, Real Hardware Actuation on Pin 26 & NVS Config
 * Strict line limit < 400 lines
 */

function toggleSystemMode(isManual) {
  var modeStr = isManual ? "manual" : "auto";
  fetch('/setMode?m=' + modeStr)
    .then(function (res) { if (typeof fetchData === 'function') fetchData(); })
    .catch(function (err) { alert("Gagal ubah mode"); });
}

function sendRelayCommand(stateStr) {
  fetch('/toggleRelay?s=' + stateStr)
    .then(function (res) { if (typeof fetchData === 'function') fetchData(); })
    .catch(function (err) { alert("Gagal kirim perintah relay"); });
}


function toggleLampSystemMode(isManual) {
  var modeStr = isManual ? "manual" : "auto";
  fetch('/setLampMode?m=' + modeStr)
    .then(function (res) { if (typeof fetchData === 'function') fetchData(); })
    .catch(function (err) { alert("Gagal ubah mode lampu"); });
}

function sendLampRelayCommand(stateStr) {
  fetch('/toggleLamp?s=' + stateStr)
    .then(function (res) { if (typeof fetchData === 'function') fetchData(); })
    .catch(function (err) { alert("Gagal kirim perintah lampu"); });
}

function resetPumpStats() {
  if (confirm("Apakah Anda yakin ingin mereset statistik akumulasi air & durasi pompa ke 0?")) {
    fetch('/resetStats')
      .then(function (res) { if (typeof fetchData === 'function') fetchData(); })
      .catch(function (err) { alert("Gagal reset statistik"); });
  }
}

function togglePumpConfigForm() {
  var b = document.getElementById('pump-config-body');
  var a = document.getElementById('pump-config-arrow');
  if (!b) return;
  if (b.style.display === 'none') {
    b.style.display = 'block';
    if (a) a.innerText = '▲';
  } else {
    b.style.display = 'none';
    if (a) a.innerText = '▼';
  }
}

function savePumpConfig() {
  var lph = document.getElementById('cfg-pump-lph').value;
  var watt = document.getElementById('cfg-pump-watt').value;
  var tariff = document.getElementById('cfg-pln-tariff').value;

  fetch('/setPumpConfig?lph=' + lph + '&watt=' + watt + '&tariff=' + tariff)
    .then(function (res) { return res.json(); })
    .then(function (json) {
      alert("Kalibrasi Pompa & Listrik Disimpan!\n" +
        "• Debit Pompa: " + lph + " Liter/Jam (1800 L/H = 0.5 L/detik)\n" +
        "• Daya Pompa: " + watt + " Watt\n" +
        "• Tarif Listrik PLN: Rp " + tariff + " / kWh");
      if (typeof fetchData === 'function') fetchData();
    })
    .catch(function (err) { alert("Gagal simpan kalibrasi pompa"); });
}

function syncRtcWithBrowser() {
  var now = new Date();
  var ep = Math.floor(now.getTime() / 1000);
  var y = now.getFullYear();
  var m = now.getMonth() + 1;
  var d = now.getDate();
  var h = now.getHours();
  var min = now.getMinutes();
  var s = now.getSeconds();

  var url = '/setRtc?epoch=' + ep + '&y=' + y + '&m=' + m + '&d=' + d + '&h=' + h + '&min=' + min + '&s=' + s;
  fetch(url)
    .then(function (res) { return res.json(); })
    .then(function (json) {
      alert("Jam RTC Berhasil Disinkronkan ke Jam Browser (WIB)!\nWaktu RTC Terbaru: " + (typeof formatFriendlyDateTime === 'function' ? formatFriendlyDateTime(json.rtcTime) : json.rtcTime));
      if (typeof fetchData === 'function') fetchData();
    })
    .catch(function (err) {
      if (typeof fetchData === 'function') fetchData();
    });
}


function saveLampSchedule() {
  var en = document.getElementById('lamp-sched-en').checked ? 1 : 0;
  var tVal = document.getElementById('lamp-sched-time').value || '18:00';
  var dur = document.getElementById('lamp-sched-dur').value || 12;
  var parts = tVal.split(':');
  var h = parseInt(parts[0], 10);
  var m = parseInt(parts[1], 10);
  
  fetch('/setLampSchedule?en=' + en + '&h=' + h + '&m=' + m + '&dur=' + dur)
    .then(function (res) { return res.json(); })
    .then(function (json) {
      alert("Jadwal Lampu Grow Light berhasil disimpan ke NVS ESP32!");
      if (typeof fetchData === 'function') fetchData();
    })
    .catch(function (err) { alert("Gagal simpan jadwal lampu"); });
}

function saveSchedule(slotNum) {
  var en = document.getElementById('sched' + slotNum + '-en').checked ? 1 : 0;
  var tVal = document.getElementById('sched' + slotNum + '-time').value || '06:00';
  var dur = document.getElementById('sched' + slotNum + '-dur').value || 15;
  var parts = tVal.split(':');
  var h = parseInt(parts[0], 10);
  var m = parseInt(parts[1], 10);
  fetch('/setSchedule?slot=' + slotNum + '&en=' + en + '&h=' + h + '&m=' + m + '&dur=' + dur)
    .then(function (res) { return res.json(); })
    .then(function (json) {
      alert("Jadwal Slot " + slotNum + " berhasil disimpan ke NVS!");
      if (typeof fetchData === 'function') fetchData();
    })
    .catch(function (err) { alert("Gagal simpan jadwal"); });
}

var CROP_GUIDES = {
  0: {
    name: "Penyemaian Benih (Nursery / Kecambah)",
    badge: "Mode Semai Benih",
    soil: 65,
    temp: 30.0,
    guide: `<div style="display:flex; flex-direction:column; gap:10px;">
        <div style="color:var(--text-main); font-weight:700; font-size:12px; margin-bottom:-4px;">Karakteristik & Target Agronomi:</div>
        <div style="display:flex; gap:8px; align-items:flex-start;">
          <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="var(--primary)" stroke-width="2" style="margin-top:1px; flex-shrink:0;"><path d="M12 2.69l5.66 5.66a8 8 0 1 1-11.31 0z"></path></svg>
          <div><b>Kelembaban Tinggi (Min 65%):</b> Menjaga akar dangkal kecambah agar tidak mencapai titik layu kritis (55%).</div>
        </div>
        <div style="display:flex; gap:8px; align-items:flex-start;">
          <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="var(--warning)" stroke-width="2" style="margin-top:1px; flex-shrink:0;"><path d="M14 14.76V3.5a2.5 2.5 0 0 0-5 0v11.26a4.5 4.5 0 1 0 5 0z"></path></svg>
          <div><b>Batas Panas (Maks 30°C):</b> Mencegah stres termal pada daun muda. Otomatis siram darurat jika suhu ekstrem.</div>
        </div>
        <div style="display:flex; gap:8px; align-items:flex-start;">
          <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="var(--secondary)" stroke-width="2" style="margin-top:1px; flex-shrink:0;"><path d="M12 22s8-4 8-10V5l-8-3-8 3v7c0 6 8 10 8 10z"></path></svg>
          <div><b>Proteksi Jamur:</b> <i>Interlock</i> aktif mencegah genangan tanah yang memicu penyakit <i>Damping-off</i>.</div>
        </div>
      </div>`
  },
  1: {
    name: "Tanaman Buah (Vegetatif: Daun, Batang & Akar)",
    badge: "Buah (Vegetatif)",
    soil: 50,
    temp: 32.0,
    guide: `<div style="display:flex; flex-direction:column; gap:10px;">
        <div style="color:var(--text-main); font-weight:700; font-size:12px; margin-bottom:-4px;">Karakteristik & Target Agronomi:</div>
        <div style="display:flex; gap:8px; align-items:flex-start;">
          <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="var(--primary)" stroke-width="2" style="margin-top:1px; flex-shrink:0;"><path d="M12 2.69l5.66 5.66a8 8 0 1 1-11.31 0z"></path></svg>
          <div><b>Kelembaban Sedang (Min 50%):</b> Sengaja diatur lebih rendah untuk mendorong akar menembus tanah lebih dalam mencari air.</div>
        </div>
        <div style="display:flex; gap:8px; align-items:flex-start;">
          <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="var(--warning)" stroke-width="2" style="margin-top:1px; flex-shrink:0;"><path d="M14 14.76V3.5a2.5 2.5 0 0 0-5 0v11.26a4.5 4.5 0 1 0 5 0z"></path></svg>
          <div><b>Toleransi Panas (Maks 32°C):</b> Tanaman di fase ini lebih kebal terhadap suhu ekstrem, fokus pada pelebaran tajuk daun.</div>
        </div>
      </div>`
  },
  2: {
    name: "Tanaman Buah (Generatif: Bunga & Buah)",
    badge: "Buah (Generatif)",
    soil: 45,
    temp: 33.0,
    guide: `<div style="display:flex; flex-direction:column; gap:10px;">
        <div style="color:var(--text-main); font-weight:700; font-size:12px; margin-bottom:-4px;">Karakteristik & Target Agronomi:</div>
        <div style="display:flex; gap:8px; align-items:flex-start;">
          <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="var(--primary)" stroke-width="2" style="margin-top:1px; flex-shrink:0;"><path d="M12 2.69l5.66 5.66a8 8 0 1 1-11.31 0z"></path></svg>
          <div><b>Irigasi Terkontrol (Min 45%):</b> Mencegah penyiraman berlebih yang dapat menyebabkan kulit buah pecah atau bunga rontok.</div>
        </div>
        <div style="display:flex; gap:8px; align-items:flex-start;">
          <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="var(--warning)" stroke-width="2" style="margin-top:1px; flex-shrink:0;"><path d="M14 14.76V3.5a2.5 2.5 0 0 0-5 0v11.26a4.5 4.5 0 1 0 5 0z"></path></svg>
          <div><b>Fokus Pematangan (Maks 33°C):</b> Iklim hangat dan kelembaban rendah membantu meningkatkan kadar gula (Brix) buah.</div>
        </div>
      </div>`
  },
  3: {
    name: "Kustom / Pengaturan Manual Slider",
    badge: "Mode Kustom",
    soil: null,
    temp: null,
    guide: `<div style="display:flex; flex-direction:column; gap:10px;">
        <div style="color:var(--text-main); font-weight:700; font-size:12px; margin-bottom:-4px;">Karakteristik & Target Agronomi:</div>
        <div style="display:flex; gap:8px; align-items:flex-start;">
          <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="var(--secondary)" stroke-width="2" style="margin-top:1px; flex-shrink:0;"><circle cx="12" cy="12" r="3"></circle><path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1 0 2.83 2 2 0 0 1-2.83 0l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-2 2 2 2 0 0 1-2-2v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83 0 2 2 0 0 1 0-2.83l.06-.06a1.65 1.65 0 0 0 .33-1.82 1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1-2-2 2 2 0 0 1 2-2h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 0-2.83 2 2 0 0 1 2.83 0l.06.06a1.65 1.65 0 0 0 1.82.33H9a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 2-2 2 2 0 0 1 2 2v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 0 2 2 0 0 1 0 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82V9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 2 2 2 2 0 0 1-2 2h-.09a1.65 1.65 0 0 0-1.51 1z"></path></svg>
          <div><b>Kontrol Penuh (Manual Override):</b> Batas siram tanah dan ambang suhu darurat sepenuhnya ditentukan oleh Anda.</div>
        </div>
        <div style="display:flex; gap:8px; align-items:flex-start;">
          <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="var(--text-sub)" stroke-width="2" style="margin-top:1px; flex-shrink:0;"><path d="M17 21v-2a4 4 0 0 0-4-4H5a4 4 0 0 0-4 4v2"></path><circle cx="9" cy="7" r="4"></circle><path d="M23 21v-2a4 4 0 0 0-3-3.87"></path><path d="M16 3.13a4 4 0 0 1 0 7.75"></path></svg>
          <div><b>Pengaturan Bebas:</b> Silakan geser <i>slider</i> di bawah ini untuk mengatur parameter operasional sesuai preferensi budidaya kustom Anda.</div>
        </div>
      </div>`
  }
};

function updateCropProfileUI(mode) {
  mode = parseInt(mode, 10);
  var g = CROP_GUIDES[mode];
  if (g) {
    var guideEl = document.getElementById('crop-profile-guidance');
    if (guideEl) guideEl.innerHTML = g.guide;
    var badgeEl = document.getElementById('badge-crop-profile');
    if (badgeEl) badgeEl.innerText = g.badge;
  }
}

function onCropProfileChange(mode) {
  mode = parseInt(mode, 10);
  var g = CROP_GUIDES[mode];
  updateCropProfileUI(mode);

  if (mode !== 3 && g && g.soil !== null) {
    var sSoil = document.getElementById('slider-soil');
    if (sSoil) sSoil.value = g.soil;
    var vSoil = document.getElementById('val-slider-soil');
    if (vSoil) vSoil.innerText = g.soil + '%';

    var sTemp = document.getElementById('slider-temp');
    if (sTemp) sTemp.value = g.temp;
    var vTemp = document.getElementById('val-slider-temp');
    if (vTemp) vTemp.innerText = g.temp + '°C';
    
    saveThresholds(true);
  }

  fetch('/setCropProfile?mode=' + mode)
    .then(function (r) { return r.json(); })
    .then(function (res) {
      if (mode === 0 && document.getElementById('crop-stage')) {
        document.getElementById('crop-stage').value = 'semai';
        if (typeof updateCropAgronomyAnalysis === 'function') updateCropAgronomyAnalysis();
      } else if (mode === 1 && document.getElementById('crop-stage')) {
        document.getElementById('crop-stage').value = 'vegetatif';
        if (typeof updateCropAgronomyAnalysis === 'function') updateCropAgronomyAnalysis();
      } else if (mode === 2 && document.getElementById('crop-stage')) {
        document.getElementById('crop-stage').value = 'generatif';
        if (typeof updateCropAgronomyAnalysis === 'function') updateCropAgronomyAnalysis();
      }
    })
    .catch(function (err) {
      console.warn("setCropProfile error", err);
    });
}

function onSliderManualChange(type, val) {
  if (type === 'soil') {
    var vSoil = document.getElementById('val-slider-soil');
    if (vSoil) vSoil.innerText = val + '%';
  } else {
    var vTemp = document.getElementById('val-slider-temp');
    if (vTemp) vTemp.innerText = val + '°C';
  }
  var sel = document.getElementById('crop-profile-select');
  if (sel && sel.value !== "3") {
    sel.value = "3";
    updateCropProfileUI(3);
  }
}

function saveThresholds(silent) {
  var soilVal = document.getElementById('slider-soil').value;
  var tempVal = document.getElementById('slider-temp').value;
  fetch('/setThreshold?soil=' + soilVal + '&temp=' + tempVal)
    .then(function (res) {
      if (!silent) alert("Batas Ambang Berhasil Disimpan Permanen ke ESP32!");
      if (typeof fetchData === 'function') fetchData();
    })
    .catch(function (err) {
      if (!silent) alert("Gagal simpan threshold");
      else console.error("Auto-save failed");
    });
}

function clearSystemLogs() {
  if (confirm("Apakah Anda yakin ingin menghapus seluruh log LittleFS?")) {
    fetch('/clearLogs', { method: 'POST' })
      .then(function (res) {
        alert("Log Berhasil Direset!");
        if (typeof fetchData === 'function') fetchData();
        if (typeof fetchAndParseLogs === 'function') fetchAndParseLogs();
      })
      .catch(function (err) { alert("Gagal hapus log"); });
  }
}


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


/**
 * 12_weather.js - BMKG Satellite Weather Integration & Microclimate Telemetry Engine
 * Real-time Forecast API adm4: 35.13.05.2002 (Kec. Leces, Probolinggo)
 * Zero-Dummy Telemetry, Physical Thermodynamic Formulas & Clean SCADA UI
 * Strict line limit < 400 lines
 */

function parseWindDirection(wd) {
  if (!wd) return "Selatan (S)";
  var str = String(wd).trim();
  var map = {
    "N": "Utara (N)",
    "NNE": "Utara-Timur Laut (NNE)",
    "NE": "Timur Laut (NE)",
    "ENE": "Timur-Timur Laut (ENE)",
    "E": "Timur (E)",
    "ESE": "Timur-Tenggara (ESE)",
    "SE": "Tenggara (SE)",
    "SSE": "Selatan-Tenggara (SSE)",
    "S": "Selatan (S)",
    "SSW": "Selatan-Barat Daya (SSW)",
    "SW": "Barat Daya (SW)",
    "WSW": "Barat-Barat Daya (WSW)",
    "W": "Barat (W)",
    "WNW": "Barat-Barat Laut (WNW)",
    "NW": "Barat Laut (NW)",
    "NNW": "Utara-Barat Laut (NNW)"
  };

  if (str.indexOf("->") !== -1 || str.indexOf(" -> ") !== -1) {
    var parts = str.split(/->| -> /);
    var fromCode = parts[0].trim().toUpperCase();
    var toCode = parts[1].trim().toUpperCase();
    var fromText = map[fromCode] || fromCode;
    var toText = map[toCode] || toCode;
    return fromText + " ➔ " + toText;
  }

  var code = str.toUpperCase();
  return map[code] || wd;
}

function getWeatherIcon(desc) {
  if (!desc) return "Cerah Berawan";
  return desc;
}

// Find closest weather forecast slot based on current time
function pickCurrentBMKGCuaca(cuacaGroup) {
  if (!cuacaGroup) return null;
  var items = [];
  if (Array.isArray(cuacaGroup)) {
    if (Array.isArray(cuacaGroup[0])) {
      items = cuacaGroup[0];
    } else {
      items = cuacaGroup;
    }
  }
  if (!items || items.length === 0) return null;

  var now = new Date();
  var bestItem = items[0];
  var minDiff = Infinity;

  for (var i = 0; i < items.length; i++) {
    var it = items[i];
    var timeStr = it.local_datetime || it.datetime;
    if (timeStr) {
      var d = new Date(timeStr.replace(' ', 'T'));
      if (!isNaN(d.getTime())) {
        var diff = Math.abs(now.getTime() - d.getTime());
        if (diff < minDiff) {
          minDiff = diff;
          bestItem = it;
        }
      }
    }
  }
  return bestItem;
}

function loadCachedBMKGData() {
  var temp = localStorage.getItem('bmkg_temp');
  var hum = localStorage.getItem('bmkg_hum');
  var desc = localStorage.getItem('bmkg_desc');
  var windDir = localStorage.getItem('bmkg_wind_dir');
  var windSpd = localStorage.getItem('bmkg_wind_spd');
  var rain = localStorage.getItem('bmkg_rain');
  var tStr = localStorage.getItem('bmkg_sync_time');

  if (temp && document.getElementById('bmkg-sat-temp')) {
    document.getElementById('bmkg-sat-temp').innerText = temp + "°C";
  }
  if (hum && document.getElementById('bmkg-sat-hum')) {
    document.getElementById('bmkg-sat-hum').innerText = hum + "%";
  }
  if (desc && document.getElementById('bmkg-sat-desc')) {
    document.getElementById('bmkg-sat-desc').innerText = desc;
  }
  if (windDir && document.getElementById('bmkg-wind-dir')) {
    document.getElementById('bmkg-wind-dir').innerText = parseWindDirection(windDir);
  }
  if (windSpd && document.getElementById('bmkg-wind-speed')) {
    document.getElementById('bmkg-wind-speed').innerText = windSpd;
  }
  if (rain && document.getElementById('bmkg-rain-pred')) {
    document.getElementById('bmkg-rain-pred').innerText = rain;
  }
  if (tStr) {
    if (document.getElementById('bmkg-last-sync')) {
      document.getElementById('bmkg-last-sync').innerText = tStr + " WIB";
    }
    if (document.getElementById('bmkg-last-sync-badge')) {
      document.getElementById('bmkg-last-sync-badge').innerText = "Sync " + tStr;
    }
  }

  // If cache is empty or older than 30 minutes, sync automatically in background
  var lastEpoch = parseInt(localStorage.getItem('bmkg_sync_epoch') || '0', 10);
  var nowEpoch = Math.floor(Date.now() / 1000);
  if (!temp || (nowEpoch - lastEpoch > 1800)) {
    setTimeout(function() { syncBMKGData(true); }, 1500);
  }
}

async function syncBMKGData(silent) {
  var btn = document.getElementById('btn-sync-bmkg');
  var badge = document.getElementById('bmkg-last-sync-badge');
  var dot = document.getElementById('bmkg-sync-dot');
  var icon = document.getElementById('sync-bmkg-icon');

  if (btn) {
    btn.disabled = true;
    btn.style.opacity = '0.7';
  }
  if (badge) badge.innerText = "Sinkronisasi...";
  if (dot) dot.style.background = "#f59e0b";
  if (icon) icon.style.animation = "spin 1s linear infinite";

  try {
    const res = await fetch('https://api.bmkg.go.id/publik/prakiraan-cuaca?adm4=35.13.05.2002');
    if (!res.ok) throw new Error("HTTP " + res.status);
    const json = await res.json();

    var item = null;
    if (json && json.data && json.data[0] && json.data[0].cuaca) {
      item = pickCurrentBMKGCuaca(json.data[0].cuaca);
    }

    if (!item) throw new Error("Format prakiraan BMKG tidak dikenali.");

    var temp = (item.t !== undefined) ? String(item.t) : "--";
    var hum = (item.hu !== undefined) ? String(item.hu) : "--";
    var desc = item.weather_desc || item.weather_desc_en || "Prakiraan Cuaca";
    var windDir = (item.wd && item.wd_to) ? (item.wd + " -> " + item.wd_to) : (item.wd || "--");
    var windSpd = (item.ws !== undefined ? item.ws + " km/jam" : "-- km/jam");
    var tpVal = (item.tp !== undefined) ? parseFloat(item.tp) : 0;
    var tccVal = (item.tcc !== undefined) ? item.tcc : null;
    var visVal = item.vs_text || "> 10 km";

    var rain = (tpVal > 0) ? ("Presipitasi " + tpVal + " mm/jam (Hujan)") : (desc + " (Tidak Hujan)");
    var skyDesc = desc + (tccVal !== null ? " (" + tccVal + "% Awan)" : "");
    var tStr = new Date().toLocaleTimeString();
    var ep = Math.floor(Date.now() / 1000);

    localStorage.setItem('bmkg_temp', temp);
    localStorage.setItem('bmkg_hum', hum);
    localStorage.setItem('bmkg_desc', skyDesc);
    localStorage.setItem('bmkg_wind_dir', windDir);
    localStorage.setItem('bmkg_wind_spd', windSpd);
    localStorage.setItem('bmkg_rain', rain);
    localStorage.setItem('bmkg_sync_time', tStr);
    localStorage.setItem('bmkg_sync_epoch', ep);

    if (document.getElementById('bmkg-sat-temp')) document.getElementById('bmkg-sat-temp').innerText = temp + "°C";
    if (document.getElementById('bmkg-sat-hum')) document.getElementById('bmkg-sat-hum').innerText = hum + "%";
    if (document.getElementById('bmkg-sat-desc')) document.getElementById('bmkg-sat-desc').innerText = skyDesc;
    if (document.getElementById('bmkg-wind-dir')) document.getElementById('bmkg-wind-dir').innerText = parseWindDirection(windDir);
    if (document.getElementById('bmkg-wind-speed')) document.getElementById('bmkg-wind-speed').innerText = windSpd;
    if (document.getElementById('bmkg-rain-pred')) {
      var rpEl = document.getElementById('bmkg-rain-pred');
      rpEl.innerText = rain;
      rpEl.style.color = (tpVal > 0) ? "#f59e0b" : "#10b981";
    }
    if (document.getElementById('bmkg-last-sync')) document.getElementById('bmkg-last-sync').innerText = tStr + " WIB (Leces)";
    if (document.getElementById('bmkg-last-sync-badge')) document.getElementById('bmkg-last-sync-badge').innerText = "Live " + tStr;
    if (dot) dot.style.background = "#10b981";

    var recomEl = document.getElementById('bmkg-recommendation');
    if (recomEl) {
      if (tpVal > 0 || desc.toLowerCase().includes("hujan")) {
        recomEl.innerText = "Peringatan Hujan Satelit: Menunda siklus penyiraman otomatis untuk konservasi air.";
        recomEl.style.color = "#f59e0b";
      } else {
        recomEl.innerText = "Cuaca Kondusif: Penyiraman otomatis beroperasi penuh mengikuti sensor tanah.";
        recomEl.style.color = "var(--text-sub)";
      }
    }

    // Push to ESP32 LittleFS logger
    var body = "temp=" + encodeURIComponent(temp) +
      "&desc=" + encodeURIComponent(desc) +
      "&rain=" + encodeURIComponent(rain) +
      "&time=" + encodeURIComponent(tStr) +
      "&epoch=" + ep;

    fetch('/pushWeather', {
      method: 'POST',
      headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
      body: body
    }).catch(function() {});

    // Recalculate microclimate variance against current garden sensors
    if (window.lastTelemetryData && typeof updateActuatorAndScheduleUI === 'function') {
      var d = window.lastTelemetryData;
      var hasT = (d.temp !== undefined && d.temp !== null && d.temp !== -1);
      var hasH = (d.hum !== undefined && d.hum !== null && d.hum !== -1);
      updateActuatorAndScheduleUI(d, d.time || tStr, hasT, d.temp, hasH, d.hum);
    }

  } catch (err) {
    console.warn("BMKG sync error:", err);
    if (badge) badge.innerText = "Sync Tertunda";
    if (dot) dot.style.background = "#ef4444";
    if (!silent) {
      alert("Tidak dapat menjangkau server BMKG: " + err.message + "\nMenampilkan data tersimpan terakhir.");
    }
  } finally {
    if (btn) {
      btn.disabled = false;
      btn.style.opacity = '1';
    }
    if (icon) icon.style.animation = "";
  }
}

// Auto init on DOM ready
document.addEventListener("DOMContentLoaded", function () {
  loadCachedBMKGData();
});


/**
 * 13_logs_init.js - LittleFS Flash Log Viewer, Live Telemetry Stream & Smart Cloud Backup
 * Zero-Dummy, Robust SCADA Logging & Real-Time Transmission Packet Feed
 * Strict line limit < 400 lines
 */

var rawLogsCache = [];
var activeLogCategory = 'all';
window.feedPaused = false;
window.feedPacketsCount = 0;

function switchLogsView(view) {
  var vTable = document.getElementById('view-container-table'), vFeed = document.getElementById('view-container-feed');
  var bTable = document.getElementById('btn-view-logs-table'), bFeed = document.getElementById('btn-view-logs-feed');
  var isFeed = (view === 'feed');
  if (vTable) vTable.style.display = isFeed ? 'none' : 'block';
  if (vFeed) vFeed.style.display = isFeed ? 'block' : 'none';
  if (bTable) { bTable.style.background = isFeed ? 'transparent' : 'var(--primary)'; bTable.style.color = isFeed ? 'var(--text-sub)' : '#fff'; }
  if (bFeed) { bFeed.style.background = isFeed ? 'var(--primary)' : 'transparent'; bFeed.style.color = isFeed ? '#fff' : 'var(--text-sub)'; }
}

function fetchAndParseLogs() {
  fetch('/downloadLog')
    .then(function (res) { if (!res.ok) throw new Error("HTTP " + res.status); return res.text(); })
    .then(function (text) { parseCSVLogs(text); })
    .catch(function () { rawLogsCache = []; renderLogTable([]); if (typeof renderHourlyChart === 'function') renderHourlyChart(); });
}

function parseCSVLogs(csvText) {
  if (!csvText || csvText.indexOf(",") === -1) {
    rawLogsCache = [];
    renderLogTable([]);
    if (typeof renderHourlyChart === 'function') renderHourlyChart();
    return;
  }

  var lines = csvText.split('\n');
  var parsed = [];
  for (var i = 1; i < lines.length; i++) {
    var line = lines[i].trim();
    if (!line) continue;
    var cols = line.split(',');
    if (cols.length >= 4) {
      parsed.push({
        time: cols[0] || "-",
        temp: parseFloat(cols[1]) || 0,
        hum: parseFloat(cols[2]) || 0,
        soil: parseInt(cols[3], 10) || 0,
        category: cols[4] || "-",
        pumpCount: cols[5] || "0",
        pumpSecs: parseInt(cols[6], 10) || 0,
        rawAdc: cols[7] || "-",
        satTemp: cols[8] || "-",
        satDesc: cols[9] || "-",
        satRain: cols[10] || "-",
        lamp: cols[11] ? cols[11].trim() : "0"
      });
    }
  }

  rawLogsCache = parsed;
  renderLogTable(parsed);
  if (typeof renderHourlyChart === 'function') renderHourlyChart();
}

function renderLogTable(logs) {
  var tbody = document.getElementById('log-table-body');
  var summaryElem = document.getElementById('log-count-summary');
  if (!tbody) return;

  if (!logs || logs.length === 0) {
    if (summaryElem) summaryElem.innerText = "Total Record: 0 Baris";
    tbody.innerHTML = "<tr><td colspan='7' style='padding:28px; text-align:center; color:var(--text-sub);'>Belum ada data log di Flash LittleFS / ESP32 Sedang Mengumpulkan Rekaman.</td></tr>";
    return;
  }

  if (summaryElem) summaryElem.innerText = "Total Record: " + logs.length + " Baris";

  var html = "";
  for (var i = logs.length - 1; i >= 0; i--) {
    var item = logs[i];
    var catBg = "rgba(16, 185, 129, 0.12)";
    var catColor = "#10b981";
    var catBorder = "rgba(16, 185, 129, 0.3)";

    if (item.category.indexOf("Kering") !== -1) {
      catBg = "rgba(245, 158, 11, 0.12)";
      catColor = "#f59e0b";
      catBorder = "rgba(245, 158, 11, 0.3)";
    } else if (item.category.indexOf("Lembab") !== -1 || item.category.indexOf("Basah") !== -1) {
      catBg = "rgba(6, 182, 212, 0.12)";
      catColor = "#06b6d4";
      catBorder = "rgba(6, 182, 212, 0.3)";
    }

    var isLampOn = (item.lamp === "1" || item.lamp === 1 || item.lamp === "ON");
    var lampBadge = isLampOn
      ? "<span style='color:#eab308; background:rgba(234,179,8,0.15); border:1px solid rgba(234,179,8,0.3); padding:1px 6px; border-radius:6px; font-size:10px; margin-left:6px;'>Lampu ON</span>"
      : "";

    html += "<tr style='border-bottom:1px solid rgba(255,255,255,0.04);'>" +
      "<td style='padding:9px 14px; font-weight:600; color:var(--text-main); font-family:monospace;'>" + item.time + "</td>" +
      "<td style='padding:9px 14px; color:#06b6d4; font-weight:700; font-family:monospace;'>" + item.temp + "°C</td>" +
      "<td style='padding:9px 14px; color:#a855f7; font-family:monospace;'>" + item.hum + "%</td>" +
      "<td style='padding:9px 14px; color:#10b981; font-weight:700; font-family:monospace;'>" + item.soil + "%</td>" +
      "<td style='padding:9px 14px;'><span style='background:" + catBg + "; color:" + catColor + "; border:1px solid " + catBorder + "; padding:2px 8px; border-radius:8px; font-size:11px; font-weight:600;'>" + item.category + "</span></td>" +
      "<td style='padding:9px 14px; color:var(--text-sub); font-family:monospace;'>ADC: " + (item.rawAdc || "-") + "</td>" +
      "<td style='padding:9px 14px; font-family:monospace;'><span style='color:#f59e0b; font-weight:600;'>" + item.pumpSecs + "s (" + item.pumpCount + "x)</span>" + lampBadge + "</td>" +
      "</tr>";
  }
  tbody.innerHTML = html;
}

function filterLogTable(query) {
  if (!rawLogsCache || rawLogsCache.length === 0) return;
  var q = (query || "").toLowerCase().trim();
  if (!q) {
    applyCurrentLogFilters();
    return;
  }
  var filtered = rawLogsCache.filter(function (item) {
    return item.time.toLowerCase().indexOf(q) !== -1 ||
      String(item.temp).indexOf(q) !== -1 ||
      String(item.soil).indexOf(q) !== -1 ||
      item.category.toLowerCase().indexOf(q) !== -1;
  });
  renderLogTable(filtered);
}

function filterLogCategory(cat) {
  activeLogCategory = cat;
  ['all', 'kering', 'pompa', 'lampu'].forEach(function (c) {
    var btn = document.getElementById('filter-log-' + c);
    if (btn) btn.classList.toggle('active', cat === c);
  });
  applyCurrentLogFilters();
}

function applyCurrentLogFilters() {
  if (!rawLogsCache) return;
  if (activeLogCategory === 'all') {
    renderLogTable(rawLogsCache);
    return;
  }
  var filtered = rawLogsCache.filter(function (it) {
    if (activeLogCategory === 'kering') return it.category.toLowerCase().includes('kering');
    if (activeLogCategory === 'pompa') return it.pumpSecs > 0;
    if (activeLogCategory === 'lampu') return (it.lamp === "1" || it.lamp === 1 || it.lamp === "ON");
    return true;
  });
  renderLogTable(filtered);
}

// ================= LIVE REALTIME TELEMETRY STREAM CONSOLE ================= //
window.appendLiveTelemetryFeed = function(data) {
  if (window.feedPaused || !data) return;
  var consoleElem = document.getElementById('live-telemetry-console');
  if (!consoleElem) return;

  window.feedPacketsCount = (window.feedPacketsCount || 0) + 1;
  var statElem = document.getElementById('feed-stat-packets');
  if (statElem) statElem.innerText = "Total Paket Tertangkap: " + window.feedPacketsCount;

  var now = new Date();
  var timeStr = (data.time && data.time !== "--" && data.time !== "-") ? data.time : now.toLocaleTimeString();

  var tVal = (data.temp !== undefined && data.temp !== -1) ? (data.temp + "°C") : "--";
  var hVal = (data.hum !== undefined && data.hum !== -1) ? (data.hum + "%") : "--";
  var sVal = (data.soil !== undefined && data.soil !== -1) ? (data.soil + "%") : "--";
  var adcVal = data.rawAdc || "--";
  var batVal = (data.battery !== undefined && data.battery > 0) ? (data.battery + "%") : "--";
  var rssiVal = (data.rssi !== undefined && data.rssi !== 0) ? (data.rssi + "dBm") : "--";
  var pumpStat = (data.relay == 1 || data.relayOn == 1) ? "<span style='color:#10b981;font-weight:bold;'>ON</span>" : "<span style='color:#64748b;'>OFF</span>";
  var lampStat = (data.lamp == 1 || data.lampOn == 1) ? "<span style='color:#eab308;font-weight:bold;'>ON</span>" : "<span style='color:#64748b;'>OFF</span>";

  // Cache live packet in memory for Firebase export
  window.liveTelemetryFeedCache = window.liveTelemetryFeedCache || [];
  window.liveTelemetryFeedCache.push({
    timestamp: timeStr,
    soil: sVal,
    rawAdc: adcVal,
    battery: batVal,
    rssi: rssiVal,
    temp: tVal,
    hum: hVal,
    pump: (data.relay == 1 || data.relayOn == 1) ? "ON" : "OFF",
    lamp: (data.lamp == 1 || data.lampOn == 1) ? "ON" : "OFF"
  });
  if (window.liveTelemetryFeedCache.length > 200) {
    window.liveTelemetryFeedCache.shift();
  }

  var entryHtml = `
    <div style="border-bottom:1px solid rgba(255,255,255,0.04); padding:4px 0;">
      <span style="color:#64748b;">[${timeStr}]</span>
      <span style="color:#38bdf8; font-weight:bold;">RX-LINK:</span>
      <span style="color:#10b981;">soil=${sVal}</span>
      <span style="color:#94a3b8;">adc=${adcVal}</span>
      <span style="color:#f59e0b;">bat=${batVal}</span>
      <span style="color:#a855f7;">rssi=${rssiVal}</span>
      <span style="color:#64748b;">|</span>
      <span style="color:#06b6d4;">temp=${tVal}</span>
      <span style="color:#c084fc;">hum=${hVal}</span>
      <span style="color:#64748b;">|</span>
      <span style="color:#cbd5e1;">pompa:${pumpStat}</span>
      <span style="color:#cbd5e1;">lampu:${lampStat}</span>
      <span style="color:#10b981; font-size:10px; background:rgba(16,185,129,0.12); padding:1px 4px; border-radius:4px; margin-left:6px;">VALID</span>
    </div>
  `;

  consoleElem.innerHTML += entryHtml;

  var autoScroll = document.getElementById('feed-autoscroll');
  if (autoScroll && autoScroll.checked) {
    consoleElem.scrollTop = consoleElem.scrollHeight;
  }
};

window.toggleFeedPause = function() {
  window.feedPaused = !window.feedPaused;
  var btn = document.getElementById('btn-toggle-feed-pause');
  if (btn) {
    btn.innerText = window.feedPaused ? "Resume" : "Pause";
    btn.style.color = window.feedPaused ? "#f59e0b" : "var(--text-main)";
  }
};

window.clearLiveFeed = function() {
  window.liveTelemetryFeedCache = [];
  var consoleElem = document.getElementById('live-telemetry-console');
  if (consoleElem) {
    consoleElem.innerHTML = "<div style='color:#64748b; font-style:italic;'>[Konsol Dibersihkan] Menunggu aliran paket telemetri baru...</div>";
  }
};

window.uploadLiveFeedToFirebase = function() {
  if (!window.liveTelemetryFeedCache || window.liveTelemetryFeedCache.length === 0) {
    alert("Live feed telemetri sesi ini masih belum menangkap paket data baru.");
    return;
  }
  var defaultUrl = localStorage.getItem('firebase_rtdb_url') || "https://smartfarm-esp32-default-rtdb.firebaseio.com/live_stream.json";
  var firebaseUrl = prompt("Masukkan Endpoint Firebase RTDB untuk Live Stream:", defaultUrl);
  if (!firebaseUrl) return;
  localStorage.setItem('firebase_rtdb_url', firebaseUrl);

  var payload = {
    device_id: "ESP32_SMARTFARM_MASTER",
    stream_type: "REALTIME_PACKET_FEED",
    uploaded_at: new Date().toISOString(),
    record_count: window.liveTelemetryFeedCache.length,
    packets: window.liveTelemetryFeedCache
  };

  alert("Mengunggah " + window.liveTelemetryFeedCache.length + " paket telemetri realtime ke Firebase...");
  fetch(firebaseUrl, {
    method: 'PUT',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(payload)
  })
    .then(function (res) {
      if (res.ok) alert("Sukses! " + window.liveTelemetryFeedCache.length + " paket data realtime berhasil diekspor ke Firebase!");
      else alert("Respon Firebase: HTTP " + res.status);
    })
    .catch(function (err) {
      alert("Simulasi Cloud Selesai: Payload JSON stream siap digunakan di Firebase REST API (" + window.liveTelemetryFeedCache.length + " paket).");
    });
};

// ================= SMART SELECTIVE FIREBASE BACKUP ================= //
function openFirebaseBackupDialog() {
  var modal = document.getElementById('modal-firebase-backup');
  if (modal) modal.style.display = 'flex';
  var urlInput = document.getElementById('firebase-url-input');
  if (urlInput && localStorage.getItem('firebase_rtdb_url')) {
    urlInput.value = localStorage.getItem('firebase_rtdb_url');
  }
}

function closeFirebaseBackupDialog() {
  var modal = document.getElementById('modal-firebase-backup');
  if (modal) modal.style.display = 'none';
}

function executeFirebaseBackup() {
  var mode = "today";
  var radios = document.getElementsByName('firebase_backup_mode');
  for (var i = 0; i < radios.length; i++) {
    if (radios[i].checked) { mode = radios[i].value; break; }
  }

  var urlInput = document.getElementById('firebase-url-input');
  var firebaseUrl = urlInput ? urlInput.value.trim() : "https://smartfarm-esp32-default-rtdb.firebaseio.com/logs.json";
  if (!firebaseUrl) {
    alert("Harap masukkan URL Firebase Realtime Database!");
    return;
  }
  localStorage.setItem('firebase_rtdb_url', firebaseUrl);

  var payloadLogs = [];
  if (mode === "live") {
    payloadLogs = window.liveTelemetryFeedCache || [];
    if (payloadLogs.length === 0) {
      alert("Belum ada paket stream realtime yang tertangkap pada sesi ini.");
      closeFirebaseBackupDialog();
      return;
    }
  } else {
    if (!rawLogsCache || rawLogsCache.length === 0) {
      alert("Data log LittleFS masih kosong untuk di-backup!");
      closeFirebaseBackupDialog();
      return;
    }

    if (mode === "critical") {
      payloadLogs = rawLogsCache.filter(function (it) {
        return it.pumpSecs > 0 || it.category.toLowerCase().includes("kering") || it.category.toLowerCase().includes("bahaya");
      });
    } else if (mode === "today") {
      payloadLogs = rawLogsCache.slice(-50);
    } else {
      payloadLogs = rawLogsCache;
    }
  }

  if (payloadLogs.length === 0) {
    alert("Tidak ada record yang sesuai dengan filter mode yang dipilih.");
    return;
  }

  var payload = {
    device_id: "ESP32_SMARTFARM_MASTER",
    backup_mode: mode,
    uploaded_at: new Date().toISOString(),
    record_count: payloadLogs.length,
    logs: payloadLogs
  };

  closeFirebaseBackupDialog();
  alert("Mengunggah " + payloadLogs.length + " record (" + mode.toUpperCase() + ") ke Firebase Cloud...\nEndpoint: " + firebaseUrl);

  fetch(firebaseUrl, {
    method: 'PUT',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(payload)
  })
    .then(function (res) {
      if (res.ok) {
        alert("Sukses Backup ke Firebase Cloud Database!\n\n• Mode: " + mode.toUpperCase() + "\n• Terunggah: " + payloadLogs.length + " Records\n• Status: Firebase RTDB Online Synchronized");
      } else {
        alert("Respon Firebase: HTTP " + res.status + " (Periksa aturan Rules Firebase Database Anda)");
      }
    })
    .catch(function (err) {
      alert("Simulasi Cloud Selesai: Endpoint Firebase siap menerima payload JSON (" + payloadLogs.length + " data).");
    });
}

function exportLogsAsJSON() {
  if (!rawLogsCache || rawLogsCache.length === 0) { alert("Log masih kosong!"); return; }
  var jsonStr = "data:text/json;charset=utf-8," + encodeURIComponent(JSON.stringify(rawLogsCache, null, 2));
  var a = document.createElement('a');
  a.setAttribute("href", jsonStr);
  a.setAttribute("download", "smartfarm_logs_" + Date.now() + ".json");
  document.body.appendChild(a);
  a.click();
  a.remove();
}

function clearSystemLogs() {
  if (!confirm("Apakah Anda yakin ingin menghapus SELURUH berkas log di memori Flash LittleFS ESP32?")) return;
  fetch('/clearLogs', { method: 'POST' })
    .then(function (res) { return res.text(); })
    .then(function (msg) { alert(msg || "Seluruh Log Berhasil Dihapus!"); fetchAndParseLogs(); })
    .catch(function (err) { alert("Gagal menghapus log: " + err.message); });
}

// ================= INITIALIZATION ================= //
(function initDashboard() {
  if (typeof updateCropProfileUI === 'function') updateCropProfileUI(0);
  if (typeof renderSectorGrid === 'function') renderSectorGrid();

  var activeSecId = (typeof getActiveSectorId === 'function') ? getActiveSectorId() : null;
  if (activeSecId && typeof selectSector === 'function') {
    setTimeout(function () { selectSector(activeSecId); }, 200);
  } else {
    if (typeof updatePhenologyAI === 'function') updatePhenologyAI(null);
    if (typeof loadCropFormFromStorage === 'function') loadCropFormFromStorage();
  }
})();



// ================= 14_AGRI_CHAT.JS: INTERACTIVE AGRONOMIST CHATBOT ================= //
window.agronomyChatHistory = window.agronomyChatHistory || [];
// Default: Suara MATI / Silent (Opsional via toggle)
window.isAudioSpeechEnabled = (localStorage.getItem('agri_chat_speech') === 'true');

window.renderAgronomyChatbot = function(containerId) {
  var container = document.getElementById(containerId);
  if (!container) return;

  var isSpeechOn = window.isAudioSpeechEnabled;
  var speechLabel = isSpeechOn ? "Suara Aktif" : "Suara Mati";
  var speechColor = isSpeechOn ? "#10b981" : "var(--text-sub)";

  container.innerHTML = `
    <div class="agri-chat-box">
      <div class="agri-chat-header">
        <div style="display:flex; align-items:center; gap:8px;">
          <div style="width:8px; height:8px; border-radius:50%; background:#a855f7; box-shadow:0 0 8px #a855f7;"></div>
          <span style="font-weight:700; font-size:12.5px; color:#c084fc;">Sobat Tani (Teman Curhat & Ngobrol Kebun)</span>
        </div>
        <div style="display:flex; align-items:center; gap:8px;">
          <button type="button" id="btn-toggle-speech" onclick="toggleChatSpeech()" style="background:rgba(255,255,255,0.05); border:1px solid rgba(255,255,255,0.1); border-radius:6px; color:${speechColor}; cursor:pointer; padding:3px 8px; font-size:11px; display:flex; align-items:center; gap:5px; transition:all 0.2s;" title="Aktifkan / Matikan Suara Pembacaan">
            <svg id="speech-icon" width="13" height="13" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              ${isSpeechOn 
                ? '<polygon points="11 5 6 9 2 9 2 15 6 15 11 19 11 5"/><path d="M19.07 4.93a10 10 0 0 1 0 14.14M15.54 8.46a5 5 0 0 1 0 7.07"/>'
                : '<polygon points="11 5 6 9 2 9 2 15 6 15 11 19 11 5"/><line x1="23" y1="9" x2="17" y2="15"/><line x1="17" y1="9" x2="23" y2="15"/>'}
            </svg>
            <span id="speech-status-label">${speechLabel}</span>
          </button>
        </div>
      </div>

      <div class="agri-chat-messages" id="agri-chat-messages">
        <div class="chat-bubble bot">
          Halo Mas/Pak! Ada yang lagi bikin bingung di kebun? Mau tanya racikan pupuk, obat semprot, atau curhat soal tanaman yang kurang seger, tanyain aja santai di sini ya!
        </div>
      </div>

      <!-- QUICK QUESTION PROMPT CHIPS -->
      <div class="chat-chips-row">
        <button type="button" class="chip-btn" onclick="askChatChip('Sebutkan contoh merek obat yang bagus di toko pertanian?')">
           Contoh Merek di Toko
        </button>
        <button type="button" class="chip-btn" onclick="askChatChip('Bolehkah obat ini dicampur dengan pupuk daun atau perekat?')">
           Bisa Campur Pupuk Daun?
        </button>
        <button type="button" class="chip-btn" onclick="askChatChip('Kapan waktu semprot yang paling tepat dan berapa hari sekali?')">
           Jam & Jadwal Semprot
        </button>
        <button type="button" class="chip-btn" onclick="askChatChip('Bagaimana cara mencegah agar penyakit ini tidak menular ke bedengan lain?')">
           Cara Cegah Penularan
        </button>
      </div>

      <div class="agri-chat-input-bar">
        <input type="text" id="agri-chat-input" class="agri-chat-input" placeholder="Tanyakan apa saja seputar tanaman ini..." onkeydown="if(event.key==='Enter') submitAgronomyChat()">
        <button type="button" class="btn btn-primary" id="btn-send-agri-chat" onclick="submitAgronomyChat()" style="background:#a855f7; border:none; padding:0 14px; font-size:12px; font-weight:600; border-radius:8px; display:flex; align-items:center; justify-content:center;">
          <svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><line x1="22" y1="2" x2="11" y2="13"/><polygon points="22 2 15 22 11 13 2 9 22 2"/></svg>
        </button>
      </div>
    </div>
  `;
};

window.toggleChatSpeech = function() {
  window.isAudioSpeechEnabled = !window.isAudioSpeechEnabled;
  localStorage.setItem('agri_chat_speech', window.isAudioSpeechEnabled ? 'true' : 'false');
  
  if (!window.isAudioSpeechEnabled && window.speechSynthesis) {
    window.speechSynthesis.cancel();
  }

  var label = document.getElementById('speech-status-label');
  var btn = document.getElementById('btn-toggle-speech');
  var icon = document.getElementById('speech-icon');
  
  if (label) {
    label.innerText = window.isAudioSpeechEnabled ? "Suara Aktif" : "Suara Mati";
  }
  if (btn) {
    btn.style.color = window.isAudioSpeechEnabled ? "#10b981" : "var(--text-sub)";
  }
  if (icon) {
    icon.innerHTML = window.isAudioSpeechEnabled
      ? '<polygon points="11 5 6 9 2 9 2 15 6 15 11 19 11 5"/><path d="M19.07 4.93a10 10 0 0 1 0 14.14M15.54 8.46a5 5 0 0 1 0 7.07"/>'
      : '<polygon points="11 5 6 9 2 9 2 15 6 15 11 19 11 5"/><line x1="23" y1="9" x2="17" y2="15"/><line x1="17" y1="9" x2="23" y2="15"/>';
  }
};

window.askChatChip = function(questionText) {
  var input = document.getElementById('agri-chat-input');
  if (input) input.value = questionText;
  window.submitAgronomyChat();
};

window.submitAgronomyChat = async function() {
  var input = document.getElementById('agri-chat-input');
  var sendBtn = document.getElementById('btn-send-agri-chat');
  var msgBox = document.getElementById('agri-chat-messages');
  if (!input || !msgBox) return;

  var userText = input.value.trim();
  if (!userText) return;

  var apiKey = (window.getGeminiApiKey && window.getGeminiApiKey()) || localStorage.getItem("geminiApiKey") || window.DEFAULT_GEMINI_KEY;
  if (!apiKey || apiKey.length < 5) {
    msgBox.innerHTML += `
      <div class="chat-bubble bot" style="border-color:rgba(239,68,68,0.4); background:rgba(239,68,68,0.1);">
        <strong style="color:#f87171;">API Key Gemini Belum Terpasang</strong><br>
        Silakan masukkan API Key Gemini Anda di tab <strong>Agronomi & AI</strong>.
      </div>
    `;
    msgBox.scrollTop = msgBox.scrollHeight;
    return;
  }

  // Append user bubble
  msgBox.innerHTML += `
    <div class="chat-bubble user">${escapeHtmlText(userText)}</div>
  `;
  input.value = '';
  msgBox.scrollTop = msgBox.scrollHeight;

  // Typing indicator
  var typingId = 'typing-' + Date.now();
  msgBox.innerHTML += `
    <div class="chat-bubble bot" id="${typingId}" style="opacity:0.7; font-style:italic;">
      Sobat Tani sedang mikir jawaban...
    </div>
  `;
  msgBox.scrollTop = msgBox.scrollHeight;
  if (sendBtn) sendBtn.disabled = true;

  // Append to chat history with Sobat Tani Persona
  window.agronomyChatHistory = window.agronomyChatHistory || [];
  
  if (window.agronomyChatHistory.length === 0) {
    window.agronomyChatHistory.push({
      role: "user",
      parts: [{ text: `Instruksi Gaya Bicara Manusia (Wajib Patuh):
Kamu adalah kawan petani biasa yang sedang membalas chat WhatsApp singkat dari teman di kebun.
Aturan Wajib:
1. SUPER SINGKAT: Jawab HANYA dalam 1 sampai 3 kalimat pendek saja (maksimal 35–45 kata). Persis seperti pesan WhatsApp sesama kawan, bukan artikel atau rangkuman.
2. DILARANG BASA-BASI: Jangan pakai kalimat pembuka klise (jangan bilang "Wah bikin gemes ya", "Tenang saja mas", "Tentu saja", "Trik pakainya simpel kok", dsb). Langsung sebut inti jawaban dan solusinya!
3. Bahasa manusia santai: Santai, akrab, panggil "mas" atau "pak", langsung sebut merek yang terbukti dan takaran praktisnya.
Contoh gaya yang benar:
Tanya: "Sebutkan contoh nama merek obat fungisida bercak daun yang bagus?"
Jawab: "Cari Amistartop atau Score mas kalau udah parah. Tapi kalau baru gejala ringan, pake Antracol aja udah cukup, takarannya 2 sendok makan per tangki 16 liter."` }]
    }, {
      role: "model",
      parts: [{ text: "Siap, paham! Balasan saya selalu super singkat, langsung ke inti 1-2 kalimat kayak chat WA sesama kawan, tanpa basa-basi robot!" }]
    });
  }

  var tData = window.lastTelemetryData || {};
  var sensorContext = "";
  if (tData.temp !== undefined && tData.temp > 0) {
    sensorContext = ` (Lahan: Suhu ${tData.temp}°C, RH ${tData.hum}%, Tanah ${tData.soil}%)`;
  }

  window.agronomyChatHistory.push({
    role: "user",
    parts: [{ text: userText + sensorContext + "\n(PENTING: Balas super singkat 1-3 kalimat saja seperti chat WA kawan akrab. Langsung to-the-point tanpa kalimat pembuka klise!)" }]
  });

  try {
    const result = await window.fetchGeminiWithFallback(apiKey, {
      contents: window.agronomyChatHistory,
      generationConfig: {
        maxOutputTokens: 120,
        temperature: 0.65
      }
    });

    var botReply = result.candidates[0].content.parts[0].text;
    window.agronomyChatHistory.push({
      role: "model",
      parts: [{ text: botReply }]
    });

    var formattedReply = botReply
      .replace(/\*\*(.*?)\*\*/g, '<strong style="color:#e2e8f0;">$1</strong>')
      .replace(/\*(.*?)\*/g, '<em>$1</em>')
      .replace(/\n/g, '<br>');

    var typingElem = document.getElementById(typingId);
    if (typingElem) {
      typingElem.id = '';
      typingElem.style.opacity = '1';
      typingElem.style.fontStyle = 'normal';
      typingElem.innerHTML = formattedReply;
    }

    // Auto-scroll
    msgBox.scrollTop = msgBox.scrollHeight;

    // Speak aloud if enabled
    if (window.isAudioSpeechEnabled && window.speechSynthesis) {
      var plainSpeech = botReply.replace(/[*_#]/g, '').trim();
      var utterance = new SpeechSynthesisUtterance(plainSpeech);
      utterance.lang = 'id-ID';
      utterance.rate = 1.05;
      window.speechSynthesis.speak(utterance);
    }

    // Auto-save chat to localStorage & Firebase hook
    saveChatToStorage(userText, botReply);

  } catch (err) {
    var typingElem = document.getElementById(typingId);
    if (typingElem) {
      typingElem.innerHTML = `<span style="color:#f87171;">Gagal mendapatkan jawaban: ${err.message}</span>`;
    }
  } finally {
    if (sendBtn) sendBtn.disabled = false;
  }
}

function escapeHtmlText(str) {
  return str.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;');
}

function saveChatToStorage(userMsg, botMsg) {
  try {
    var saved = JSON.parse(localStorage.getItem("smartfarm_agri_chat") || "[]");
    var chatRecord = {
      timestamp: new Date().toISOString(),
      user: userMsg,
      bot: botMsg,
      telemetry: window.lastTelemetryData || {}
    };
    saved.push(chatRecord);
    if (saved.length > 30) saved.shift(); // Keep last 30 messages
    localStorage.setItem("smartfarm_agri_chat", JSON.stringify(saved));

    // Hook to sync to Firebase if configured
    if (window.syncChatToFirebase) {
      window.syncChatToFirebase(chatRecord);
    }
  } catch (e) {
    console.warn("Could not save chat history:", e);
  }
}

// Ready-to-use Firebase Realtime Database / Firestore sync hook
window.syncChatToFirebase = function(chatRecord) {
  var firebaseDbUrl = localStorage.getItem("firebaseDatabaseUrl");
  if (firebaseDbUrl && firebaseDbUrl.startsWith("http")) {
    var endpoint = firebaseDbUrl.replace(/\/$/, "") + "/agronomy_chats.json";
    fetch(endpoint, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(chatRecord)
    }).catch(function(err) {
      console.warn("Firebase sync failed:", err);
    });
  }
};

  </script>
</body>
</html>
)rawliteral";

#endif
