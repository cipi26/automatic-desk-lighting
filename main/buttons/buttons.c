#include "buttons.h"
#include "driver/gpio.h"
#include "freertos/idf_additions.h"

TaskHandle_t controller_handle = NULL;

gpio_config_t btn_conf = {.pin_bit_mask =
                              (1ULL << GPIO_NUM_18) | (1ULL << GPIO_NUM_19),
                          .mode = GPIO_MODE_INPUT,
                          .pull_up_en = GPIO_PULLUP_ENABLE,
                          .intr_type = GPIO_INTR_NEGEDGE};

static void IRAM_ATTR select_button_isr(void *arg) {
  xTaskNotifyFromISR(controller_handle, BTN_SELECT, eSetBits, NULL);
}

void buttons_init(TaskHandle_t notify_task) {
  controller_handle = notify_task;
  gpio_config(&btn_conf);
  gpio_install_isr_service(0);

  gpio_isr_handler_add(GPIO_NUM_18, select_button_isr, NULL);
}