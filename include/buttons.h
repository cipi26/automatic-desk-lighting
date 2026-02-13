#pragma once
#include <OneButton.h>

namespace buttons {
  enum class Event: uint8_t {
    None,
    NavClick,
    NavLongPress,
    SelectClick,
  };

  void init();
  void tick();
  Event popEvent();
}