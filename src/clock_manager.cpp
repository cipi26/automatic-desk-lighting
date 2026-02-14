#include <time.h>
#include <Arduino.h>
#include "wifi_manager.h"

namespace
{
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
}