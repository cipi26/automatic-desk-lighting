#ifndef DEV_LEDSTRIP_SMOOTH_h
#define DEV_LEDSTRIP_SMOOTH_h

#include <HomeSpan.h>
#include <FastLED.h>
#include "led_config.h"

struct DEV_LEDSTRIP_SMOOTH : Service::LightBulb {
  SpanCharacteristic *power;
  SpanCharacteristic *H;
  SpanCharacteristic *S;
  SpanCharacteristic *V;

  DEV_LEDSTRIP_SMOOTH() : Service::LightBulb() {
    power = new Characteristic::On();
    H = new Characteristic::Hue(0);
    S = new Characteristic::Saturation(0);
    V = new Characteristic::Brightness(100);
    V->setRange(5, 100, 1);
  }

  boolean update() {

    if(power->updated()) {
      ledstate.targetPower = power->getNewVal();

      if(ledstate.targetPower) 
        ledstate.targetV = V->getVal();
      else ledstate.targetV = 0;
    }

    if (H->updated()) {
      ledstate.targetH = H->getNewVal<float>();
    }

    if (S->updated()) {
      ledstate.targetS = S->getNewVal<float>();
    }

    if (V->updated()) {
      ledstate.targetV = V->getNewVal<float>();
    }

    return true;
  }
};

#endif