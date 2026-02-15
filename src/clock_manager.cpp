#include <time.h>
#include <Arduino.h>
#include "wifi_manager.h"

namespace
{
  const char *DAY_NAMES[]{"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  const char *MONTH_NAMES[]{"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

  bool ntpStarted = false;
  bool clockReady = false;
  uint32_t lastCheck = 0;
  uint32_t lastLog = 0;
}

namespace clock_manager
{
  void tick()
  {
    if (clockReady)
      return;

    if (!wifi_manager::isConnected())
      return;

    if (!ntpStarted)
    {
      configTime(0, 0, "ro.pool.ntp.org", "0.ro.pool.ntp.org", "1.ro.pool.ntp.org");
      setenv("TZ", "EET-2EEST,M3.5.0/3,M10.5.0/4", 1);
      tzset();
      ntpStarted = true;
    }

    if (millis() - lastCheck < 500)
      return;
    lastCheck = millis();

    tm t;
    if (getLocalTime(&t, 10))
    {
      clockReady = true;
      Serial.println("[Clock] Synced");
    }
    else if (millis() - lastLog > 2000)
    {
      lastLog = millis();
      Serial.println("[Clock] Syncing..");
    }

    if (clockReady)
      Serial.printf("[Clock] %04d-%02d-%02d %02d:%02d:%02d\n",
                    t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
                    t.tm_hour, t.tm_min, t.tm_sec);
  }

  bool isReady()
  {
    return clockReady;
  }

  bool getClock(tm &t)
  {
    if (!clockReady)
      return false;

    return getLocalTime(&t, 10);
  }

  const char *getDayName()
  {
    if (!clockReady)
      return "---";
    tm t;
    if (getLocalTime(&t, 10))
      return DAY_NAMES[t.tm_wday];
    return "---";
  }

  const char *getMonthName()
  {
    if (!clockReady)
      return "---";
    tm t;
    if (getLocalTime(&t, 10))
      return MONTH_NAMES[t.tm_mon];
    return "---";
  }

  const char *getFormattedDate()
  {
    static char buf[16];
    tm t;
    if (getClock(t))
    {
      snprintf(buf, 16, "%s, %s %d", DAY_NAMES[t.tm_wday], MONTH_NAMES[t.tm_mon], t.tm_mday);
      return buf;
    }
    return "---";
  }

  const char *getFormattedTime()
  {
    static char buf[6];
    tm t;
    if (getClock(t))
    {
      snprintf(buf, 6, "%02d:%02d", t.tm_hour, t.tm_min);
      return buf;
    }
    return "--:--";
  }

}