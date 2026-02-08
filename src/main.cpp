#include <Arduino.h>
#include "led_strip.h"
#include "wifi_manager.h"

#ifdef APP_ENV_PROD
#include "ota.h"
#endif

void setup()
{
  Serial.begin(115200);

  led_strip::init();
  wifi_manager::init();
}

void loop()
{
  #ifdef APP_ENV_PROD
  ota::tick();
  #endif

  delay(1);
}