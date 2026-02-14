#pragma once

namespace oled {
  enum class Action : uint8_t { Down, Select, Back };
  enum class Screen : uint8_t
  {
    Dashboard,
    Menu,
    Colors,
    Animations,
    Timer,
    WiFi,
    COUNT
  };
  enum class MenuOption : uint8_t
  {
    Power,
    Colors,
    Animations,
    Timer,
    WiFi,
    Purple,
    WarmWhite,
    Static,
    COUNT
  };

  void init();
  void tick();
  void onAction(Action a);
  Screen getCurrentScreen();
  MenuOption getCurrentOption();
  void returnToDashboard();
}