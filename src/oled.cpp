#include <U8g2lib.h>
#include <Wire.h>
#include "oled.h"
#include "clock_manager.h"

using namespace oled;

namespace
{
  U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

  struct MenuOptionData
  {
    const uint8_t *icon;
    const char *name;
    Screen parentScreen;
  };

  struct ScreenHistoryData
  {
    Screen screen;
    uint8_t index;
  };

  const MenuOptionData MENU_OPTIONS[] = {
      {
          nullptr,
          "Power",
          Screen::Menu,
      },
      {nullptr, "Colors", Screen::Menu},
      {nullptr, "Animations", Screen::Menu},
      {nullptr, "Timer", Screen::Menu},
      {nullptr, "WiFi", Screen::Menu},
      {nullptr, "Purple", Screen::Colors},
      {nullptr, "Warm White", Screen::Colors},
      {nullptr, "Static", Screen::Animations},
  };

  ScreenHistoryData screenHistory[5];
  uint8_t historyDepth = 0;

  Screen currentScreen = Screen::Dashboard;
  MenuOption currentMenuOptions[10];
  uint8_t currentIndex = 0, optionsCount = 0;
  uint8_t lastMin = 255;

  bool render = true;

  void ScrollMenu()
  {
    if (!optionsCount)
      return;
    currentIndex = (currentIndex + 1) % optionsCount;
    render = true;
  }

  void buildCurrentOptions()
  {
    optionsCount = 0;
    currentIndex = 0;
    for (uint8_t i = 0; i < (uint8_t)MenuOption::COUNT; i++)
      if (MENU_OPTIONS[i].parentScreen == currentScreen)
        currentMenuOptions[optionsCount++] = (MenuOption)i;
  }

  void pushScreen(Screen screen)
  {
    if (historyDepth < 5)
    {
      screenHistory[historyDepth].screen = currentScreen;
      screenHistory[historyDepth++].index = currentIndex;
    }
    currentScreen = screen;
    buildCurrentOptions();
    render = true;
  }

  void popScreen()
  {
    if (historyDepth > 0)
    {
      currentScreen = screenHistory[--historyDepth].screen;
      buildCurrentOptions();
      currentIndex = screenHistory[historyDepth].index;
    }
    else
    {
      currentScreen = Screen::Dashboard;
    }
    render = true;
  }

  void centerString(u8g2_uint_t y, const char *s)
  {
    int w = u8g2.getStrWidth(s);
    int x = (128 - w) / 2;

    u8g2.drawStr(x, y, s);
  }

  void renderMenu()
  {
    uint8_t prevIndex = (currentIndex - 1 + optionsCount) % optionsCount;
    uint8_t nextIndex = (currentIndex + 1 + optionsCount) % optionsCount;

    u8g2.clearBuffer();
    if (optionsCount <= 2 && prevIndex < currentIndex || optionsCount > 2)
    {
      u8g2.setFont(u8g2_font_profont15_tr);
      u8g2.drawStr(26, 15, MENU_OPTIONS[(uint8_t)currentMenuOptions[prevIndex]].name);
    }

    u8g2.setFont(u8g2_font_profont17_tr);
    u8g2.drawStr(26, 37, MENU_OPTIONS[(uint8_t)currentMenuOptions[currentIndex]].name);

    u8g2.drawRFrame(0, 22, 128, 21, 4);

    if (optionsCount <= 2 && nextIndex > currentIndex || optionsCount > 2)
    {
      u8g2.setFont(u8g2_font_profont15_tr);
      u8g2.drawStr(26, 59, MENU_OPTIONS[(uint8_t)currentMenuOptions[nextIndex]].name);
    }

    u8g2.sendBuffer();
  }

  void renderDashboard()
  {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_logisoso24_tn);

    int ascend = u8g2.getAscent();
    int descend = u8g2.getDescent();
    int h = ascend - descend;
    int y = (64 - h) / 2 + ascend;

    centerString(y, clock_manager::getFormattedTime());

    u8g2.setFont(u8g2_font_prospero_bold_nbp_tr);
    centerString(y + 20, clock_manager::getFormattedDate());

    u8g2.setFont(u8g2_font_profont12_tr);
    centerString(u8g2.getAscent() - u8g2.getDescent(), "Purple - ON");

    u8g2.sendBuffer();
  }

  void onSelectActionByOption(MenuOption option)
  {
    switch (option)
    {
    case MenuOption::Colors:
      pushScreen(Screen::Colors);
      break;

    case MenuOption::Animations:
      pushScreen(Screen::Animations);
      break;

    case MenuOption::Timer:
      pushScreen(Screen::Timer);
      break;

    case MenuOption::WiFi:
      pushScreen(Screen::WiFi);
      break;
    }
  }

  void renderControl()
  {
    switch (currentScreen)
    {
    case Screen::Dashboard:
      tm t;
      if (clock_manager::getClock(t))
        if (lastMin != t.tm_min)
        {
          lastMin = t.tm_min;
          render = true;
        }
    }
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
    case Action::Down:
      ScrollMenu();
      break;
    case Action::Back:
      popScreen();
      break;
    case Action::Select:
      if (getCurrentScreen() == Screen::Dashboard)
        pushScreen(Screen::Menu);
      else
        onSelectActionByOption(currentMenuOptions[currentIndex]);
      break;
    }
  }

  Screen getCurrentScreen()
  {
    return currentScreen;
  }

  MenuOption getCurrentOption()
  {
    return currentMenuOptions[currentIndex];
  }

  void returnToDashboard()
  {
    currentScreen = Screen::Dashboard;
    historyDepth = 0;
    render = true;
  }

  void tick()
  {
    renderControl();

    if (!render)
      return;

    render = false;

    switch (currentScreen)
    {
    case Screen::Dashboard:
      renderDashboard();
      break;

    case Screen::WiFi:
      u8g2.clearBuffer();
      u8g2.setDrawColor(1);
      u8g2.drawBox(0, 54, 128, 10); // Black bar
      u8g2.setDrawColor(0);         // White text on black
      u8g2.setFont(u8g2_font_profont10_tr);
      u8g2.drawStr(2, 62, "Purple - ON");
      u8g2.setDrawColor(1); // Reset
      u8g2.sendBuffer();
      break;

    default:
      if (optionsCount)
        renderMenu();
      else
      {
        u8g2.clearBuffer();
        u8g2.sendBuffer();
      }
    }
  }
}
