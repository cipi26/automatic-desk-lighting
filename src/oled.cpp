#include <U8g2lib.h>
#include <Wire.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

namespace oled {
  void init()
  {
    Wire.begin();
    u8g2.begin();
  }

  void tick()
  {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_7x14_tr);
    u8g2.drawStr(26, 15, "Dashboard");

    u8g2.setFont(u8g2_font_7x14B_tr);
    u8g2.drawStr(26, 37, "Park Sensor");

    u8g2.drawRFrame(0, 22, 128, 21, 6);

    u8g2.setFont(u8g2_font_7x14_tr);
    u8g2.drawStr(26, 59, "Turbo Gauge");
    
    u8g2.sendBuffer();
  }
}
