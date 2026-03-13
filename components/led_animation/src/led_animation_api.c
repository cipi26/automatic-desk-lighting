#include "esp_err.h"
#include "freertos/projdefs.h"
#include "led_animation_types.h"
#include "led_strip.h"
#include <stdint.h>

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
  state->direction = start_index <= end_index ? LEFT_TO_RIGHT : RIGHT_TO_LEFT;
  state->color = color;
  state->interval_ms = interval_ms;
}

anim_status_t anim_sequence_tick(led_ctx_t *ctx, anim_sequence_state_t *state) {
  uint32_t now = pdTICKS_TO_MS(xTaskGetTickCount());

  if (now - state->last_tick >= state->interval_ms) {
    state->last_tick = now;

    if ((state->direction == LEFT_TO_RIGHT && state->current > state->end) ||
        (state->direction == RIGHT_TO_LEFT && state->current < state->end))
      return ANIM_DONE;

    ctx->framebuffer[state->current] = state->color;

    state->current += 1 * (state->direction == LEFT_TO_RIGHT ? 1 : -1);

    return ANIM_UPDATED;
  }
  return ANIM_WAITING;
}

void anim_refresh(led_ctx_t *ctx) {
  for (int i = 0; i < ctx->nr_leds; i++) {
    led_strip_set_pixel_hsv(*ctx->strip, i, ctx->framebuffer[i].h,
                            ctx->framebuffer[i].s, ctx->framebuffer[i].v);
  }
  led_strip_refresh(*ctx->strip);
}

void fill_leds(led_ctx_t *ctx, uint8_t num_leds, led_hsv_t color) {
  for (int i = 0; i < num_leds; i++)
    ctx->framebuffer[i] = color;
  anim_refresh(ctx);
}