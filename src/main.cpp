#include <Arduino.h>
#include "led_strip.h"
#include "wifi_manager.h"
#include "serial_console.h"
#include "oled.h"
#include "buttons.h"

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
  buttons::init();
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
  buttons::tick();
  led_strip::tick();

  for (buttons::Event e = buttons::popEvent(); e != buttons::Event::None; e = buttons::popEvent())
  {
    if (e == buttons::Event::NavClick)
      oled::onAction(oled::Action::Next);
    else if(e == buttons::Event::SelectClick)
      oled::onAction(oled::Action::Enter);
  }

  delay(1);
}