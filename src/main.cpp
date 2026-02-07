#include <Arduino.h>
#include "led_strip.h"
#include "wifi_manager.h"
#include "ota.h"

void setup()
{
  Serial.begin(115200);

  led_strip::init();
  wifi_manager::init();
  
}

void loop()
{
  ota::tick();
  delay(1);
}