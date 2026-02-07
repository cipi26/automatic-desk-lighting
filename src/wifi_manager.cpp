#include <WiFi.h>
#include <Preferences.h>

static Preferences preferences;

namespace wifi_manager {
  void init()
  {
    preferences.begin("credentials", true);
    String ssid = preferences.getString("ssid", "");
    String pass = preferences.getString("password", "");
    
    preferences.end();

    if(ssid == "" || pass == "")
    {
      Serial.println("Invalid WiFi credentials");
      return;
    }

    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);

    // Connect to WiFi network
    WiFi.begin(ssid.c_str(), pass.c_str());
    Serial.println("Connecting to ");
    Serial.print(ssid.c_str());
  }

  int isConnected()
  {
    return WiFi.status() == WL_CONNECTED;
  }
}