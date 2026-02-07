#pragma once

#include <FastLED.h>

inline constexpr u_int8_t LED_PIN = 5;
inline constexpr u_int16_t NUM_LEDS = 60;

extern CRGB leds[];

namespace led_strip {
  void init();
}

// constexpr unsigned long LED_UPDATE_INTERVAL = 10;
// constexpr float LED_TRANSITION_SPEED = 0.05;
