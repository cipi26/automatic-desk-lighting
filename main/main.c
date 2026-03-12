#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "leds/leds.h"
#include <stdio.h>

void app_main(void) {
  leds_init();
  xTaskCreate(leds_task, "leds_task", 4096, NULL, 5,
              NULL);
}
