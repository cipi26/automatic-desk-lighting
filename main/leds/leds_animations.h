#pragma once

#include "led_animation_types.h"

typedef struct {
  led_ctx_t *ctx;
  anim_sequence_state_t left_side;
  anim_sequence_state_t right_side;
} anim_startup_state_t;

anim_startup_state_t anim_startup_init(led_ctx_t *ctx, led_hsv_t color);
anim_status_t anim_startup_tick(anim_startup_state_t *state);