#include "buttons/buttons.h"
#include "controller/controller.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "leds/leds.h"
#include <stdio.h>


void app_main(void) {
  TaskHandle_t controller_handle = NULL;

  leds_init();
  xTaskCreate(leds_task, "leds_task", 4096, NULL, 5, NULL);
  xTaskCreate(controller_task, "controller_task", 2048, NULL, 5,
              &controller_handle);
  
  buttons_init(controller_handle);
}
