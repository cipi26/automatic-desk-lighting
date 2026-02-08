#include <Arduino.h>
#include <WiFi.h>

static String cmdLine;

static void handleCommand(String cmd)
{
  cmd.trim();
  cmd.toLowerCase();

  if(cmd == "wifi.status")
    Serial.printf("WiFi status=%d ip=%s\n", WiFi.status(), WiFi.localIP().toString().c_str());

}

namespace serial_console {
  void tick()
  {
    while(Serial.available()) {
      char c = (char) Serial.read();
      if(c == '\r' ) continue;
      if(c == '\n') {
        handleCommand(cmdLine);
        cmdLine = "";
      } else if(cmdLine.length() < 120)
      {
        cmdLine += c;
      }
    }
  }
}