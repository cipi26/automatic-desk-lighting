#include <OneButton.h>
#include "buttons.h"

namespace {
  constexpr uint8_t PIN_NAV = 19;
  constexpr uint8_t PIN_SELECT = 18;

  OneButton navButton(PIN_NAV, true);
  OneButton selectButton(PIN_SELECT, true);

  constexpr uint8_t QSIZE = 8;
  buttons::Event q[QSIZE];
  uint8_t head = 0;
  uint8_t tail = 0;

  void push(buttons::Event e)
  {
    uint8_t next = (head+1) % QSIZE;
    if(next == tail) return;
    q[head] = e;
    head = next;
  }

  void onNavClick() { push(buttons::Event::NavClick); }
  void onNavLong() { push(buttons::Event::NavLongPress); }
  void onSelectClick() { push(buttons::Event::SelectClick); }
}

namespace buttons {
  void init()
  {
    pinMode(PIN_NAV, INPUT_PULLUP);
    pinMode(PIN_SELECT, INPUT_PULLUP);

    navButton.attachClick(onNavClick);
    navButton.attachLongPressStart(onNavLong);

    selectButton.attachClick(onSelectClick);
  }

  void tick()
  {
    navButton.tick();
    selectButton.tick();
  }

  Event popEvent()
  {
    if(head == tail) return Event::None;
    Event e = q[tail];
    tail = (tail + 1) % QSIZE;
    return e;
  }
}