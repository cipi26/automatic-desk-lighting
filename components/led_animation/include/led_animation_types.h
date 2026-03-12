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

typedef enum { ANIM_WAITING, ANIM_UPDATED, ANIM_DONE } anim_status_t;

typedef struct {
  led_power_state_t power;
  anim_status_t startup_anim_status;
  anim_status_t shutdown_anim_status;
} led_state_t;

typedef enum { LEFT_TO_RIGHT, RIGHT_TO_LEFT } anim_direction_t;


typedef struct {
  uint8_t start, end, current;
  led_hsv_t color;
  anim_direction_t direction;
  uint32_t interval_ms;
  uint32_t last_tick;
} anim_sequence_state_t;