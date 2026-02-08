#include <WiFi.h>
#include <Preferences.h>

static Preferences preferences;

static void onWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info)
{
  switch (event)
  {
  case ARDUINO_EVENT_WIFI_STA_CONNECTED:
    Serial.println("[WiFi] Connected");
    break;

  case ARDUINO_EVENT_WIFI_STA_GOT_IP:
    Serial.printf("[WiFi] IP: %s\n", WiFi.localIP().toString().c_str());
    break;

  case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
    Serial.printf("[WiFi] Disconnected (reason=%d)\n", info.wifi_sta_disconnected.reason);
    break;

  case ARDUINO_EVENT_WIFI_STA_LOST_IP:
    Serial.println("[WiFi] Lost IP");
    break;

  default:
    break;
  }
}

namespace wifi_manager
{
  void init()
  {
    preferences.begin("credentials", true);
    String ssid = preferences.getString("ssid", "");
    String pass = preferences.getString("password", "");

    preferences.end();

    if (ssid.isEmpty() || pass.isEmpty())
    {
      Serial.println("[WiFi] Missing credentials");
      return;
    }

    WiFi.onEvent(onWiFiEvent);
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);

    // Connect to WiFi network
    WiFi.begin(ssid.c_str(), pass.c_str());
    Serial.printf("[WiFi] Connecting to %d..", ssid.c_str());
  }

  bool isConnected()
  {
    return WiFi.status() == WL_CONNECTED;
  }
}