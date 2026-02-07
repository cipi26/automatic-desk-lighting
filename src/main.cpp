#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>
#include "led_config.h"
// #include "led_core.h"

#define DHT_PIN 18

// DHT climateSensor(DHT_PIN, DHT11);
CRGB leds[NUM_LEDS];
Preferences preferences;
// volatile LEDState ledstate = {
//   0, 0,         // currentH, targetH
//   0, 0,         // currentS, targetS
//   0, 0,         // currentV, targetV (both 0 = off at startup)
//   false, false, // currentPower, targetPower
//   0             // animationMode
// };

void setup()
{
  Serial.begin(115200);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);

  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();

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

void loop()
{
  // transitionColors();
  delay(1);  // Yield to FreeRTOS scheduler - prevents Core 0 watchdog timeout
}