#include <FastLED.h>
#include <led_strip.h>

CRGB leds[NUM_LEDS];

namespace led_strip {

  void init()
  {
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);

    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
  }

}