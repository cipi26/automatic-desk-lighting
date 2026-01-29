#include <Arduino.h>
#include <WiFi.h>
#include "led_config.h"
#include "led_core.h"
#include "dev_temp.h"
#include "dev_humid.h"
#include "dev_ledstrip_smooth.h"

#define DHT_PIN 18

DHT climateSensor(DHT_PIN, DHT11);
CRGB leds[NUM_LEDS];
volatile LEDState ledstate = {
  0, 0,         // currentH, targetH
  0, 0,         // currentS, targetS
  0, 0,         // currentV, targetV (both 0 = off at startup)
  false, false, // currentPower, targetPower
  0             // animationMode
};

void setup()
{
  Serial.begin(115200);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);

  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();

  // Disable WiFi power saving for better responsiveness
  homeSpan.setWifiBegin([](const char *ssid, const char *pwd) {
    WiFi.begin(ssid, pwd);
    WiFi.setSleep(false);
  });

  // Log connection events for debugging
  homeSpan.setConnectionCallback([](int count) {
    Serial.printf("*** WiFi connection event #%d - RSSI: %d dBm\n", count, WiFi.RSSI());
  });

  // Enable WiFi rescan for better handling of extenders/mesh networks
  homeSpan.enableWiFiRescan();

  homeSpan.setStatusPin(2);
  homeSpan.begin(Category::Bridges, "Desk Bridge");

  new SpanAccessory();  
    new Service::AccessoryInformation();
      new Characteristic::Identify(); 

  // new SpanAccessory();
  //   new Service::AccessoryInformation();
  //     new Characteristic::Identify();
  //   new DEV_TEMP();
  //   new DEV_HUMID();

  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify();
      new Characteristic::Name("LED Strip");
    new DEV_LEDSTRIP_SMOOTH();

  homeSpan.autoPoll();
}

void loop()
{
  transitionColors();
  delay(1);  // Yield to FreeRTOS scheduler - prevents Core 0 watchdog timeout
}