#include <U8g2lib.h>
#include <Wire.h>
#include "oled.h"

//temporary
#include "led_strip.h"

namespace
{
  U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

  constexpr int NUM_ITEMS = 6;
  const char *menu_items[NUM_ITEMS] = {"Dashboard", "Park Sensor", "Turbo Gauge", "Toggle LED", "Brightness", "Fireworks"};
  uint8_t currentIndex = 0;
  bool update = true;

  void nextIndex(uint8_t &idx, uint8_t count)
  {
    idx = (idx + 1) % count;
    update = true;
  }
}

namespace oled
{
  void init()
  {
    Wire.begin();
    u8g2.begin();
  }

  void onAction(Action a)
  {
    switch (a)
    {
    case Action::Next:
      nextIndex(currentIndex, NUM_ITEMS);
      break;
    case Action::Enter:
      if(currentIndex == 3)
        led_strip::togglePower();
    }
  }

  void tick()
  {

    if(update)
    {
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_7x14_tr);
      u8g2.drawStr(26, 15, menu_items[(currentIndex - 1 + NUM_ITEMS) % NUM_ITEMS]);

      u8g2.setFont(u8g2_font_7x14B_tr);
      u8g2.drawStr(26, 37, menu_items[currentIndex]);

      u8g2.drawRFrame(0, 22, 128, 21, 4);

      u8g2.setFont(u8g2_font_7x14_tr);
      u8g2.drawStr(26, 59, menu_items[(currentIndex + 1 + NUM_ITEMS) % NUM_ITEMS]);

      u8g2.sendBuffer();

      update = false;
    }
  }
}
