#include <FastLED.h>
#include <led_strip.h>

namespace
{
  CRGB leds[NUM_LEDS];

  bool power = false, targetPower = false;
  uint8_t brightness = 0;

  void powerOn()
  {
    static uint32_t last = 0;

    if (millis() - last >= 10)
    {
      if (brightness != 255)
        brightness = (brightness > 255 - 10) ? 255 : brightness + 10;
      else
        power = true;

      FastLED.setBrightness(brightness);
      FastLED.show();

      last = millis();
    }
  }

  void powerOff()
  {
    static uint32_t last = 0;

    if (millis() - last >= 10)
    {
      if (brightness != 0)
        brightness = (brightness < 10) ? 0 : brightness - 10;
      else
        power = false;

      FastLED.setBrightness(brightness);
      FastLED.show();

      last = millis();
    }
  }
}

namespace led_strip
{

  void init()
  {
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);

    fill_solid(leds, NUM_LEDS, CRGB::White);
    FastLED.setBrightness(0);
    FastLED.show();
  }

  void tick()
  {
    if (power != targetPower)
    {
      if (targetPower)
        powerOn();
      else
        powerOff();
    }
  }

  void togglePower()
  {
    targetPower = !power;
  }

}