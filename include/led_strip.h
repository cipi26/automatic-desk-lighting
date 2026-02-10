#pragma once

#include <FastLED.h>

inline constexpr u_int8_t LED_PIN = 5;
inline constexpr u_int16_t NUM_LEDS = 60;

namespace led_strip {
  void init();
}
