#include <Arduino.h>
#include "led_strip.h"
#include "wifi_manager.h"
#include "serial_console.h"
#include "oled.h"

#ifdef APP_ENV_PROD
#include "ota.h"
#endif

#ifdef APP_ENV_DEV
#include "dev_ota.h"
#endif

void setup()
{
  Serial.begin(115200);

  led_strip::init();
  wifi_manager::init();
  oled::init();
}

void loop()
{
#ifdef APP_ENV_PROD
  ota::tick();
#endif

#ifdef APP_ENV_DEV
  dev_ota::tick();
  serial_console::tick();
#endif

  oled::tick();

  delay(1);
}