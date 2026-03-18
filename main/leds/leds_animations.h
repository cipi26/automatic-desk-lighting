#pragma once

#include "led_animation_types.h"

typedef struct {
  led_ctx_t *ctx;
  anim_sequence_fade_state_t left_side;
  anim_sequence_fade_state_t right_side;
  anim_status_t status;
} anim_startup_state_t;

void anim_startup_init(anim_startup_state_t *state, led_ctx_t *ctx, led_hsv_t color);
void anim_startup_tick(anim_startup_state_t *state);