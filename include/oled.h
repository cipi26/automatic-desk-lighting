#pragma once

namespace oled {
  enum class Action : uint8_t { Next, Enter, Back };

  void init();
  void tick();
  void onAction(Action a);
}