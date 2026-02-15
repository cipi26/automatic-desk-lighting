#include <Arduino.h>

namespace
{
  constexpr uint8_t LED_PIN = 2;
  constexpr uint16_t FAST_BLINK_INTERVAL = 200;

  bool fastBlink = false;
  uint8_t times = 0;
}

namespace board_led
{
  void init()
  {
    pinMode(LED_PIN, OUTPUT);
  }

  void tick()
  {
    static uint16_t last = 0;
    if (fastBlink)
      if (millis() - last >= FAST_BLINK_INTERVAL)
      {
        digitalWrite(LED_PIN, LOW);
        last = millis();
      }
  }
}