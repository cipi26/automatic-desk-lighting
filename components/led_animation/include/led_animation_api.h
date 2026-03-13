#pragma once

#include "led_animation_types.h"
#include <stdint.h>

void anim_sequence_init(led_ctx_t *ctx, anim_sequence_state_t *state,
                        uint8_t start_index, uint8_t end_index,
                        uint32_t interval_ms, led_hsv_t color);

anim_status_t anim_sequence_tick(led_ctx_t *ctx, anim_sequence_state_t *state);

void anim_refresh(led_ctx_t *ctx);

void fill_leds(led_ctx_t *ctx, uint8_t num_leds, led_hsv_t color);