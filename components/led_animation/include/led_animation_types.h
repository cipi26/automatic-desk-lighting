#pragma once

#include "led_strip_types.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct {
  uint16_t h;
  uint8_t s, v;
} led_hsv_t;

typedef enum {
  LED_POWER_ON,
  LED_POWER_OFF
} led_power_state_t;

typedef struct {
  led_strip_handle_t *strip;
  uint8_t nr_leds;
  led_hsv_t *framebuffer;
} led_ctx_t;

typedef enum { ANIM_DONE, ANIM_WAITING, ANIM_UPDATED } anim_status_t;

typedef struct {
  led_power_state_t power;
} led_state_t;

typedef enum { LEFT_TO_RIGHT = 1, RIGHT_TO_LEFT = -1 } anim_direction_t;


typedef struct {
  led_ctx_t *ctx;
  int8_t start, end, current;
  led_hsv_t color;
  anim_direction_t direction;
  uint32_t interval_ms;
  uint32_t last_tick;
  anim_status_t status;
} anim_sequence_state_t;

typedef struct {
  led_ctx_t *ctx;
  uint8_t num_leds;
  led_hsv_t color;
  uint32_t interval_ms;
  uint32_t last_tick;
  anim_status_t status;
} anim_fade_leds_state_t;

typedef struct {
  led_ctx_t *ctx;
  int8_t start, end, start_limit, end_limit;
  led_hsv_t color;
  anim_direction_t direction;
  uint32_t interval_ms;
  uint8_t brightness_step_size;
  uint8_t interval_brightness;
  uint32_t last_tick;
  anim_status_t status;
} anim_sequence_fade_state_t;