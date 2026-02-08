#include <ArduinoOTA.h>

static uint32_t last_ota_time = 0;
static bool otaStarted = false;

namespace dev_ota
{
  void init()
  {
    if(otaStarted) return;

    ArduinoOTA.setHostname("ESP32");
    ArduinoOTA
        .onStart([]()
                 {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
      } else {  // U_SPIFFS
        type = "filesystem";
      }

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("[OTA] Start updating " + type); })

        .onEnd([]()
               { Serial.println("\n[OTA] End"); })

        .onProgress([](unsigned int progress, unsigned int total)
                    {
      if (millis() - last_ota_time > 500) {
        Serial.printf("[OTA] Progress: %u%%\n", (progress / (total / 100)));
        last_ota_time = millis();
      } })

        .onError([](ota_error_t error)
                 {
      Serial.printf("[OTA] Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) {
        Serial.println("[OTA] Auth Failed");
      } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("[OTA] Begin Failed");
      } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("[OTA] Connect Failed");
      } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("[OTA] Receive Failed");
      } else if (error == OTA_END_ERROR) {
        Serial.println("[OTA] End Failed");
      } });

    ArduinoOTA.begin();
    otaStarted = true;
  }

  void tick()
  {
    ArduinoOTA.handle();
  }
}