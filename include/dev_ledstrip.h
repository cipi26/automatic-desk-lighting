#ifndef DEV_LEDSTRIP_h
#define DEV_LEDSTRIP_h

#include <HomeSpan.h>
#include <FastLED.h>
#include "led_config.h"

struct DEV_LEDSTRIP : Service::LightBulb {
  SpanCharacteristic *power;
  SpanCharacteristic *H;
  SpanCharacteristic *S;
  SpanCharacteristic *V;

  DEV_LEDSTRIP() : Service::LightBulb() {
    power = new Characteristic::On();
    H = new Characteristic::Hue(0);
    S = new Characteristic::Saturation(0);
    V = new Characteristic::Brightness(100);
    V->setRange(5, 100, 1);
  }

  boolean update() {
    boolean p;
    float v, s, h;
    bool changed = false;

    h = H->getVal<float>();
    s = S->getVal<float>();
    v = V->getVal<float>();
    p=power->getVal();

    if(power->updated())
    {
      p=power->getNewVal();

      if(p)
        fill_solid(leds, NUM_LEDS, CHSV(h*255/360, s*2.55, v*2.55));
      else fill_solid(leds, NUM_LEDS, CRGB::Black);

      FastLED.show();
    }

    if (H->updated()) {
      h = H->getNewVal<float>();
      changed = true;
    }

    if (S->updated()) {
      s = S->getNewVal<float>();
      changed = true;
    }

    if (V->updated()) {
      v = V->getNewVal();
      changed = true;
    }

    if(changed)
    {
      fill_solid(leds, NUM_LEDS, CHSV(h*255/360,s*2.55,v*2.55));
      FastLED.show();
      changed = false;
    }

    return true;
  
  }

};

#endif