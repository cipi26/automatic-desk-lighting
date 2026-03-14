#include "esp_err.h"
#include "esp_log.h"
#include "freertos/idf_additions.h"
#include "freertos/task.h"
#include "led_animation_api.h"
#include "led_animation_types.h"
#include "led_strip_rmt.h"
#include "leds/leds_animations.h"

#define NUM_LEDS 60

static const char *TAG = "LEDS";
static const led_hsv_t initialColor = {.h = 271, .s = 255, .v = 255};

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

static led_state_t state = {.power = LED_POWER_ON};

void leds_init(void) {
  ESP_ERROR_CHECK(led_strip_new_rmt_device(&cfg, &rmt_cfg, &strip));
  ESP_LOGI(TAG, "LED strip initialized successfully");
}

void leds_task(void *pvParameters) {
  fill_leds(&ctx, NUM_LEDS, (led_hsv_t){.h = 0, .s = 0, .v = 0});

  vTaskDelay(pdMS_TO_TICKS(1000));

  anim_startup_state_t startup_state = {0};
  // anim_startup_init(&startup_state, &ctx, initialColor);

  anim_fade_leds_state_t fade_state = {0};
  // fade_leds_init(&fade_state, &ctx, NUM_LEDS, initialColor, 20);

  anim_sequence_fade_state_t sequence_fade_state = {0};
  anim_sequence_fade_init(&ctx, &sequence_fade_state, 0, 59, 0, 5, 5, initialColor);

  while (1) {
    anim_startup_tick(&startup_state);

    fade_leds_tick(&fade_state);

    anim_sequence_fade_tick(&sequence_fade_state);
    if (sequence_fade_state.status == ANIM_UPDATED) {
      anim_refresh(&ctx);
      sequence_fade_state.status = ANIM_WAITING;
    }

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}