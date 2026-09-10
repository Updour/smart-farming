#!/usr/bin/env python3
"""
build_web.py
Script otomatis untuk membundel modul web SCADA Smart Farm:
1. src_web/html/*.html -> index.html (Modular markup)
2. src_web/js/*.js     -> Bundel PROGMEM in-memory (Setiap modul JS max < 400 baris)
3. index.html + style.css + JS -> index.h (C++ PROGMEM WebServer ESP32)
Catatan: app.js di root dipertahankan sebagai manifest loader ringan (~50 baris).
"""

import os
import sys
import glob
import re

def build():
    base_dir = os.path.dirname(os.path.abspath(__file__))
    src_html_dir = os.path.join(base_dir, 'src_web', 'html')
    src_js_dir = os.path.join(base_dir, 'src_web', 'js')
    
    html_file = os.path.join(base_dir, 'index.html')
    css_file = os.path.join(base_dir, 'style.css')
    output_header = os.path.join(base_dir, 'index.h')

    # 1. Assembling Modular HTML if directory exists
    if os.path.exists(src_html_dir):
        html_parts = sorted(glob.glob(os.path.join(src_html_dir, '*.html')))
        if html_parts:
            print(f"Menggabungkan {len(html_parts)} modul HTML...")
            assembled_html = []
            for part in html_parts:
                with open(part, 'r', encoding='utf-8') as f:
                    assembled_html.append(f.read())
            with open(html_file, 'w', encoding='utf-8') as f:
                f.write("\n".join(assembled_html))
            print(f"   -> Terkumpul ke index.html ({os.path.getsize(html_file):,} bytes)")

    # 2. Assembling Modular JS in-memory (preserving modular files < 400 lines)
    combined_js = ""
    if os.path.exists(src_js_dir):
        js_parts = sorted(glob.glob(os.path.join(src_js_dir, '*.js')))
        if js_parts:
            print(f"Membaca {len(js_parts)} modul JavaScript (< 400 baris per file)...")
            assembled_js = []
            for part in js_parts:
                with open(part, 'r', encoding='utf-8') as f:
                    content = f.read()
                    lines = content.count('\n') + 1
                    base_name = os.path.basename(part)
                    print(f"   • {base_name}: {lines} baris")
                    assembled_js.append(content)
            combined_js = "\n\n".join(assembled_js)
            print(f"   -> Total bundle in-memory: {len(combined_js):,} bytes ({len(assembled_js)} modul)")

    if not os.path.exists(html_file) or not os.path.exists(css_file):
        print("Error: File sumber index.html atau style.css tidak ditemukan!")
        sys.exit(1)

    with open(html_file, 'r', encoding='utf-8') as f:
        html = f.read()

    with open(css_file, 'r', encoding='utf-8') as f:
        css = f.read()

    # Inisialisasi JS jika tidak ada modul src_web/js
    if not combined_js:
        js_file = os.path.join(base_dir, 'app.js')
        if os.path.exists(js_file):
            with open(js_file, 'r', encoding='utf-8') as f:
                combined_js = f.read()

    # Ganti tag link css dengan inline style
    css_tag = '<link rel="stylesheet" href="style.css">'
    inline_css = f"<style>\n{css}\n  </style>"
    if css_tag in html:
        html = html.replace(css_tag, inline_css)
    else:
        html = html.replace('</head>', f"{inline_css}\n</head>")

    # Ganti tag modular script / app.js dengan inline script PROGMEM
    inline_js = f"<script>\n{combined_js}\n  </script>"
    # Hapus semua tag script individual
    html = re.sub(r'<script\s+src=["\']src_web/js/[^"\']+["\']></script>', '', html)
    html = re.sub(r'<script\s+src=["\']app\.js["\']></script>', '', html)
    # Masukkan inline script tepat sebelum </body>
    html = html.replace('</body>', f"{inline_js}\n</body>")

    # Format output index.h
    header_content = f'''#ifndef INDEX_H
#define INDEX_H

#include <Arduino.h>

const char index_html[] PROGMEM = R"rawliteral(
{html.strip()}
)rawliteral";

#endif
'''

    with open(output_header, 'w', encoding='utf-8') as f:
        f.write(header_content)

    print(f"SUKSES: Web dashboard berhasil dikompilasi ke {output_header}")
    print(f"Ukuran file index.h: {os.path.getsize(output_header):,} bytes")

if __name__ == '__main__':
    build()
