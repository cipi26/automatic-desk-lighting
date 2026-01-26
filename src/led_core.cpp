#ifndef LED_CORE_cpp
#define LED_CORE_cpp

#include <FastLED.h>
#include "led_config.h"

void transitionColors() {
    static unsigned long lastUpdate = 0;
    unsigned long now = millis();

    // Only update at specified interval for consistent frame rate
    // Early return BEFORE creating volatile references to avoid unnecessary memory access
    if (now - lastUpdate < LED_UPDATE_INTERVAL) {
      return;
    }
    lastUpdate = now;

    // Create volatile references AFTER early return check
    volatile float& currentH = ledstate.currentH;
    volatile float& currentS = ledstate.currentS;
    volatile float& currentV = ledstate.currentV;
    volatile float& targetH = ledstate.targetH;
    volatile float& targetS = ledstate.targetS;
    volatile float& targetV = ledstate.targetV;
    volatile bool& currentPower = ledstate.currentPower;
    volatile bool& targetPower = ledstate.targetPower;

    bool needsUpdate = false;

    // Handle power state
    if (currentPower != targetPower) {
      currentPower = targetPower;
      needsUpdate = true;
    }

    // Smooth interpolation for Hue (special handling for circular values)
    if (abs(currentH - targetH) > 0.1) {
      float diff = targetH - currentH;

      // Handle wrap-around for hue (0-360 degrees)
      if (diff > 180) diff -= 360;
      if (diff < -180) diff += 360;

      currentH += diff * LED_TRANSITION_SPEED;
      if (currentH < 0) currentH += 360;
      if (currentH > 360) currentH -= 360;
      needsUpdate = true;
    }

    // Smooth interpolation for Saturation
    if (abs(currentS - targetS) > 0.1) {
      currentS += (targetS - currentS) * LED_TRANSITION_SPEED;
      needsUpdate = true;
    }

    // Smooth interpolation for Brightness
    if (abs(currentV - targetV) > 0.1) {
      currentV += (targetV - currentV) * LED_TRANSITION_SPEED;
      needsUpdate = true;
    }

    // Update LEDs if values changed
    if (needsUpdate) {
      if (currentPower && currentV > 0) {
        fill_solid(leds, NUM_LEDS, CHSV(currentH * 255 / 360, currentS * 2.55, currentV * 2.55));
      } else {
        fill_solid(leds, NUM_LEDS, CRGB::Black);
      }

      FastLED.show();
    }
  }

#endif