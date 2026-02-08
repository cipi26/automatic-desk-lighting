#include <Arduino.h>
#include "led_strip.h"
#include "wifi_manager.h"

#ifdef APP_ENV_PROD
#include "ota.h"
#endif

#ifdef APP_ENV_DEV
#include "dev_ota.h"
#endif

void setup()
{
  Serial.begin(115200);

#ifdef APP_ENV_DEV
  uint32_t t0 = millis();
  while (!Serial && millis() - t0 < 1500)
  {
    delay(10);
  }
#endif

  led_strip::init();
  wifi_manager::init();
}

void loop()
{
#ifdef APP_ENV_PROD
  ota::tick();
#endif

#ifdef APP_ENV_DEV
  dev_ota::tick();
#endif

  delay(1);
}