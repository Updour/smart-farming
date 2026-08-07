import re
with open('esp32_server_receiver.ino', 'r') as f:
    code = f.read()

# 1. Replace const float batasSuhu
code = code.replace("const float batasSuhu = 30.0;  // Batas Hangat", "float batasSuhu = 30.0;  // Batas Hangat\nint batasTanah = 45;     // Batas Tanah Minimal")

# 2. Replace all hardcoded 45 with batasTanah
code = re.sub(r'moisture >= 45\)', 'moisture >= batasTanah)', code)
code = re.sub(r'latestMoisturePercent < 45\)', 'latestMoisturePercent < batasTanah)', code)

# 3. Add handleSetThreshold before handleSetMode
handler_code = """
void handleSetThreshold() {
  if (server.hasArg("soil") && server.hasArg("temp")) {
    batasTanah = server.arg("soil").toInt();
    batasSuhu = server.arg("temp").toFloat();
    
    File f = LittleFS.open("/config.txt", "w");
    if (f) {
      f.println(String(batasTanah) + "," + String(batasSuhu));
      f.close();
    }
    server.send(200, "text/plain", "Threshold Berhasil Disimpan!");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

void handleSetMode() {"""
code = code.replace("void handleSetMode() {", handler_code)

# 4. Add server.on("/setThreshold")
code = code.replace('server.on("/setMode", handleSetMode);', 'server.on("/setMode", handleSetMode);\n  server.on("/setThreshold", handleSetThreshold);')

# 5. Add loading config in setup()
setup_load = """
  // Load Config
  if (LittleFS.exists("/config.txt")) {
    File f = LittleFS.open("/config.txt", "r");
    if (f) {
      String line = f.readStringUntil('\\n');
      int commaIdx = line.indexOf(',');
      if (commaIdx > 0) {
        batasTanah = line.substring(0, commaIdx).toInt();
        batasSuhu = line.substring(commaIdx + 1).toFloat();
      }
      f.close();
    }
  }

  dht.begin();"""
code = code.replace("dht.begin();", setup_load)

# 6. Expose config to JSON payload in handleData()
# Search for String json = "{";
# Replace with String json = "{";\n  json += "\"batasSuhu\":\"" + String(batasSuhu) + "\",";\n  json += "\"batasTanah\":\"" + String(batasTanah) + "\",";
code = code.replace('String json = "{";', 'String json = "{";\n  json += "\\"batasSuhu\\":\\"" + String(batasSuhu) + "\\",";\n  json += "\\"batasTanah\\":\\"" + String(batasTanah) + "\\",";')

with open('esp32_server_receiver.ino', 'w') as f:
    f.write(code)
print("Patched!")
