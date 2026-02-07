#include <WiFi.h>
#include <Preferences.h>

static Preferences preferences;

namespace wifi_manager {
  void init()
  {
    preferences.begin("credentials", true);
    String ssid = preferences.getString("ssid", "none");
    String pass = preferences.getString("password", "none");
    
    preferences.end();

    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);

    // Connect to WiFi network
    WiFi.begin(ssid.c_str(), pass.c_str());
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.printf("\nConnected. IP: %s\n", WiFi.localIP().toString().c_str());
  }
}