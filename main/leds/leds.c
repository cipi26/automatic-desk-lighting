#include "esp_err.h"
#include "esp_log.h"
#include "freertos/idf_additions.h"
#include "freertos/task.h"
#include "led_animation.h"
#include "led_animation_types.h"
#include "led_strip_rmt.h"

#define NUM_LEDS 60

static const char *TAG = "LEDS";
static const led_hsv_t initialColor = {.h = 271, .s = 255, .v = 240};

static led_strip_handle_t strip;

static led_strip_config_t cfg = {.strip_gpio_num = 5,
                                 .max_leds = NUM_LEDS,
                                 .led_model = LED_MODEL_WS2812,
                                 .color_component_format =
                                     LED_STRIP_COLOR_COMPONENT_FMT_GRB};

static led_strip_rmt_config_t rmt_cfg = {.resolution_hz = 10 * 1000 * 1000};

static led_hsv_t LEDS[NUM_LEDS];

static led_ctx_t ctx = {
    .nr_leds = NUM_LEDS, .strip = &strip, .framebuffer = LEDS};

static led_state_t state = {.power = LED_POWER_ON,
                            .shutdown_anim_status = ANIM_DONE,
                            .startup_anim_status = ANIM_WAITING};

void leds_init(void) {
  ESP_ERROR_CHECK(led_strip_new_rmt_device(&cfg, &rmt_cfg, &strip));
  ESP_LOGI(TAG, "LED strip initialized successfully");
}

void leds_task(void *pvParameters) {
  fill_leds(&ctx, NUM_LEDS, (led_hsv_t){.h = 0, .s = 0, .v = 0});

  anim_sequence_state_t left_side;
  anim_sequence_init(&ctx, &left_side, 29, 0, 20, initialColor);

  anim_sequence_state_t right_side;
  anim_sequence_init(&ctx, &right_side, 30, 59, 20, initialColor);

  while (1) {
    if (state.startup_anim_status != ANIM_DONE) {
      state.startup_anim_status = anim_sequence_tick(&ctx, &left_side);
      anim_sequence_tick(&ctx, &right_side);
      if (state.startup_anim_status == ANIM_UPDATED)
        anim_refresh(&ctx);
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}