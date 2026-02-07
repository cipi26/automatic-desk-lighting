#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "ota.h"
#include <ArduinoJson.h>

namespace ota {
  void tick()
  {
    static uint32_t last = 0;

    if(millis() - last >= 3600000)
    {
      WiFiClientSecure client;
      HTTPClient http;
      JsonDocument doc;
  
      client.setInsecure();
      http.begin(client, manifest_url);

      http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
      http.setTimeout(8000);

      int httpCode = http.GET();

      if(httpCode != 200)
      {
        http.end();
        last = millis();
        return;
      }

      String payload = http.getString();
      http.end();
      DeserializationError err = deserializeJson(doc, payload);

      if(err)
      {
        last = millis();
        return;
      }

      int build = doc["build"] | 0;
      const char* url = doc["url"] | "";
      const char* sha = doc["sha256"] | "";

      Serial.print("Build: ");
      Serial.println(build);
      Serial.print("Firmware URL: ");
      Serial.println(url);
      Serial.print("sha256: ");
      Serial.println(sha);

      last = millis();
    }
  }
}