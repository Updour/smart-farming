import re

with open('esp32_server_receiver.ino', 'r') as f:
    code = f.read()

# 1. Remove LittleFS logic from handleSetThreshold
old_threshold_func = """void handleSetThreshold() {
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
}"""

new_threshold_func = """void handleSetThreshold() {
  if (server.hasArg("soil") && server.hasArg("temp")) {
    batasTanah = server.arg("soil").toInt();
    batasSuhu = server.arg("temp").toFloat();
    server.send(200, "text/plain", "Threshold Berhasil Diubah Sementara!");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}"""
code = code.replace(old_threshold_func, new_threshold_func)

# 2. Remove LittleFS load from setup()
old_setup_load = """  // Load Config
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
  }"""
code = code.replace(old_setup_load, "")

with open('esp32_server_receiver.ino', 'w') as f:
    f.write(code)
print("Removed LittleFS logic!")
