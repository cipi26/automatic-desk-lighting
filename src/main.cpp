#include <Arduino.h>
#include <WiFi.h>
#include "led_config.h"
// #include "led_core.h"

#define DHT_PIN 18

// DHT climateSensor(DHT_PIN, DHT11);
CRGB leds[NUM_LEDS];
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


}

void loop()
{
  // transitionColors();
  delay(1);  // Yield to FreeRTOS scheduler - prevents Core 0 watchdog timeout
}