#ifndef DEV_TEMP_h
#define DEV_TEMP_h

#include <HomeSpan.h>
#include <DHT.h>

extern DHT climateSensor;

struct DEV_TEMP : Service::TemperatureSensor{
  SpanCharacteristic *temperature;
  SpanCharacteristic *status;
  unsigned long lastRead = 0;

  DEV_TEMP() : Service::TemperatureSensor() {
    temperature = new Characteristic::CurrentTemperature(20.0);
    status = new Characteristic::StatusActive(true);
  }

  void loop()
  {
    if(millis() - lastRead < 2000)
      return;
    
    lastRead = millis();

    float t = climateSensor.readTemperature();

    if(!isnan(t))
    {
      status->setVal(true);
      temperature->setVal(t);
    } else status->setVal(false);
  }
};
#endif