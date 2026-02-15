#pragma once

namespace clock_manager
{
  void tick();
  bool isReady();
  bool getClock(tm &t);
  const char* getDayName();
  const char* getMonthName();
  const char* getFormattedDate();
  const char* getFormattedTime();
}