#pragma once

#include "led_animation_types.h"
#include <stdint.h>

void anim_sequence_init(led_ctx_t *ctx, anim_sequence_state_t *state,
                        int8_t start_index, int8_t end_index,
                        uint32_t interval_ms, led_hsv_t color);

void anim_sequence_tick(led_ctx_t *ctx, anim_sequence_state_t *state);

void anim_refresh(led_ctx_t *ctx);

void fill_leds(led_ctx_t *ctx, uint8_t num_leds, led_hsv_t color);

void fade_leds_init(anim_fade_leds_state_t *state, led_ctx_t *ctx,
                    uint8_t num_leds, led_hsv_t color, uint32_t interval_ms);

void fade_leds_tick(anim_fade_leds_state_t *state);