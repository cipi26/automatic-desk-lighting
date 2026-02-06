#ifndef LED_CONFIG_h
#define LED_CONFIG_h

#include <FastLED.h>

#define LED_PIN 5
#define NUM_LEDS 60

// LED transition constants (outside struct to avoid volatile access overhead)
constexpr unsigned long LED_UPDATE_INTERVAL = 10;
constexpr float LED_TRANSITION_SPEED = 0.05;

extern CRGB leds[];

// struct LEDState {
//   float currentH, targetH;
//   float currentS, targetS;
//   float currentV, targetV;
//   bool currentPower, targetPower;
//   int animationMode;
// };

// extern volatile LEDState ledstate;

#endif