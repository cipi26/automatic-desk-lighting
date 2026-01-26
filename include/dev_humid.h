#ifndef DEV_HUMID_h
#define DEV_HUMID_h

#include <HomeSpan.h>
#include <DHT.h>

extern DHT climateSensor;

struct DEV_HUMID : Service::HumiditySensor{
  SpanCharacteristic *humidity;
  SpanCharacteristic *status;
  unsigned long lastRead = 0;

  DEV_HUMID() : Service::HumiditySensor() {
    humidity = new Characteristic::CurrentRelativeHumidity(50);
    status = new Characteristic::StatusActive(true);
  }

  void loop()
  {
    if(millis() - lastRead < 2000)
      return;
    
    lastRead = millis();

    float h = climateSensor.readHumidity();

    if(!isnan(h))
    {
      status->setVal(true);
      humidity->setVal(h);
    } else status->setVal(false);
  }
};

#endif