
#include "controller.h"
#include "buttons/buttons.h"
#include "esp_log.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "portmacro.h"

void controller_task(void *pvParameters) {
  uint32_t notification_value;
  while (1) {
    if (xTaskNotifyWait(0, ULONG_MAX, &notification_value, portMAX_DELAY) ==
        pdTRUE) {
      if (notification_value == BTN_SELECT)
        ESP_LOGI("CONTROLLER", "Select button clicked");
    }
  }
}