#pragma once

/**
 * OTA implementation using ArduinoOTA
 * Works while Mac and ESP32 connected on the same WiFi
 * Development only
 */
namespace dev_ota 
{
  void init();
  void tick();
}