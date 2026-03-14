#include "led_animation_api.h"
#include "esp_err.h"
#include "freertos/projdefs.h"
#include "led_animation_types.h"
#include "led_strip.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

void anim_sequence_init(led_ctx_t *ctx, anim_sequence_state_t *state,
                        int8_t start_index, int8_t end_index,
                        uint32_t interval_ms, led_hsv_t color) {
  if (start_index >= ctx->nr_leds)
    start_index = ctx->nr_leds - 1;
  if (end_index >= ctx->nr_leds)
    end_index = ctx->nr_leds - 1;

  if (start_index < 0)
    start_index = 0;
  if (end_index < 0)
    end_index = 0;

  state->last_tick = pdTICKS_TO_MS(xTaskGetTickCount());
  state->current = start_index;
  state->start = start_index;
  state->end = end_index;
  state->status = ANIM_WAITING;
  state->direction = start_index <= end_index ? LEFT_TO_RIGHT : RIGHT_TO_LEFT;
  state->color = color;
  state->interval_ms = interval_ms;
}

void anim_sequence_tick(led_ctx_t *ctx, anim_sequence_state_t *state) {
  if (state->status == ANIM_DONE)
    return;

  uint32_t now = pdTICKS_TO_MS(xTaskGetTickCount());

  if (now - state->last_tick >= state->interval_ms) {
    state->last_tick = now;

    if ((state->direction == LEFT_TO_RIGHT && state->current > state->end) ||
        (state->direction == RIGHT_TO_LEFT && state->current < state->end)) {
      state->status = ANIM_DONE;
      return;
    }

    ctx->framebuffer[state->current] = state->color;

    state->current += 1 * (state->direction == LEFT_TO_RIGHT ? 1 : -1);

    state->status = ANIM_UPDATED;
    return;
  }
  state->status = ANIM_WAITING;
}

static uint8_t compute_gamma(uint8_t value) {
  return round(255 * pow(value / 255.0, 2.2));
}

void anim_refresh(led_ctx_t *ctx) {
  for (int i = 0; i < ctx->nr_leds; i++) {
    led_strip_set_pixel_hsv(*ctx->strip, i, ctx->framebuffer[i].h,
                            ctx->framebuffer[i].s,
                            compute_gamma(ctx->framebuffer[i].v));
  }
  led_strip_refresh(*ctx->strip);
}

void fill_leds(led_ctx_t *ctx, uint8_t num_leds, led_hsv_t color) {
  for (int i = 0; i < num_leds; i++)
    ctx->framebuffer[i] = color;
  anim_refresh(ctx);
}

static const int step_size = 5;

void fade_leds_init(anim_fade_leds_state_t *state, led_ctx_t *ctx,
                    uint8_t num_leds, led_hsv_t color, uint32_t interval_ms) {
  if (num_leds > ctx->nr_leds)
    num_leds = ctx->nr_leds;
  state->num_leds = num_leds;
  state->color = color;
  state->interval_ms = interval_ms;
  state->status = ANIM_WAITING;
  state->last_tick = pdTICKS_TO_MS(xTaskGetTickCount());
  state->ctx = ctx;
}

void fade_leds_tick(anim_fade_leds_state_t *state) {
  if (state->status == ANIM_DONE)
    return;

  uint32_t now = pdTICKS_TO_MS(xTaskGetTickCount());

  if (now - state->last_tick >= state->interval_ms) {
    state->last_tick = now;
    uint8_t to_brightness = state->color.v;

    for (int i = 0; i < state->num_leds; i++) {
      uint8_t from_brightness = state->ctx->framebuffer[i].v;

      if (abs(from_brightness - to_brightness) > 0) {
        state->ctx->framebuffer[i].h = state->color.h;
        state->ctx->framebuffer[i].s = state->color.s;

        if (abs(to_brightness - from_brightness) < step_size) {
          state->ctx->framebuffer[i].v = to_brightness;
          state->status = ANIM_UPDATED;
        } else if (from_brightness < to_brightness)
          state->ctx->framebuffer[i].v += step_size,
              state->status = ANIM_UPDATED;
        else
          state->ctx->framebuffer[i].v -= step_size,
              state->status = ANIM_UPDATED;
      }
    }
    if (state->status == ANIM_WAITING) {
      state->status = ANIM_DONE;
      return;
    }

    if (state->status == ANIM_UPDATED) {
      anim_refresh(state->ctx);
    }
  }
  state->status = ANIM_WAITING;
}