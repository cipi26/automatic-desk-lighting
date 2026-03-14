#include "led_animation_api.h"
#include "freertos/projdefs.h"
#include "led_animation_types.h"
#include "led_strip.h"
#include <stdint.h>
#include <stdlib.h>

static const uint8_t gamma_lut[256] = {
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,
    2,   2,   3,   3,   3,   3,   3,   4,   4,   4,   4,   5,   5,   5,   5,
    6,   6,   6,   6,   7,   7,   7,   8,   8,   8,   9,   9,   9,   10,  10,
    11,  11,  11,  12,  12,  13,  13,  13,  14,  14,  15,  15,  16,  16,  17,
    17,  18,  18,  19,  19,  20,  20,  21,  22,  22,  23,  23,  24,  25,  25,
    26,  26,  27,  28,  28,  29,  30,  30,  31,  32,  33,  33,  34,  35,  35,
    36,  37,  38,  39,  39,  40,  41,  42,  43,  43,  44,  45,  46,  47,  48,
    49,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,
    63,  64,  65,  66,  67,  68,  69,  70,  71,  73,  74,  75,  76,  77,  78,
    79,  81,  82,  83,  84,  85,  87,  88,  89,  90,  91,  93,  94,  95,  97,
    98,  99,  100, 102, 103, 105, 106, 107, 109, 110, 111, 113, 114, 116, 117,
    119, 120, 121, 123, 124, 126, 127, 129, 130, 132, 133, 135, 137, 138, 140,
    141, 143, 145, 146, 148, 149, 151, 153, 154, 156, 158, 159, 161, 163, 165,
    166, 168, 170, 172, 173, 175, 177, 179, 181, 182, 184, 186, 188, 190, 192,
    194, 196, 197, 199, 201, 203, 205, 207, 209, 211, 213, 215, 217, 219, 221,
    223, 225, 227, 229, 231, 234, 236, 238, 240, 242, 244, 246, 248, 251, 253,
    255};

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

  state->ctx = ctx;
  state->last_tick = pdTICKS_TO_MS(xTaskGetTickCount());
  state->current = start_index;
  state->start = start_index;
  state->end = end_index;
  state->status = ANIM_WAITING;
  state->direction = start_index <= end_index ? LEFT_TO_RIGHT : RIGHT_TO_LEFT;
  state->color = color;
  state->interval_ms = interval_ms;
}

void anim_sequence_tick(anim_sequence_state_t *state) {
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

    state->ctx->framebuffer[state->current] = state->color;

    state->current += 1 * (state->direction == LEFT_TO_RIGHT ? 1 : -1);

    state->status = ANIM_UPDATED;
    return;
  }
  state->status = ANIM_WAITING;
}

static uint8_t compute_gamma(uint8_t value) { return gamma_lut[value]; }

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

void anim_sequence_fade_init(led_ctx_t *ctx, anim_sequence_fade_state_t *state,
                             int8_t start_index, int8_t end_index,
                             uint32_t interval_ms, uint8_t interval_brightness,
                             uint8_t brightness_step_size, led_hsv_t color) {
  if (start_index >= ctx->nr_leds)
    start_index = ctx->nr_leds - 1;
  if (end_index >= ctx->nr_leds)
    end_index = ctx->nr_leds - 1;

  if (start_index < 0)
    start_index = 0;
  if (end_index < 0)
    end_index = 0;

  state->ctx = ctx;
  state->last_tick = pdTICKS_TO_MS(xTaskGetTickCount());
  state->start_limit = start_index;
  state->end_limit = end_index;
  state->start = start_index;
  state->end = start_index;
  state->status = ANIM_WAITING;
  state->direction = start_index <= end_index ? LEFT_TO_RIGHT : RIGHT_TO_LEFT;
  state->color = color;
  state->interval_ms = interval_ms;
  if (interval_brightness == 0)
    interval_brightness = 245;
  state->interval_brightness = interval_brightness;
  if (brightness_step_size == 0)
    brightness_step_size = 10;
  state->brightness_step_size = brightness_step_size;
}

void anim_sequence_fade_tick(anim_sequence_fade_state_t *state) {
  if (state->status == ANIM_DONE)
    return;

  uint32_t now = pdTICKS_TO_MS(xTaskGetTickCount());

  if (now - state->last_tick >= state->interval_ms) {
    state->last_tick = now;
    uint8_t to_brightness = state->color.v;

    for (int i = state->start;
         state->direction == LEFT_TO_RIGHT ? i <= state->end : i >= state->end;
         i += (int)state->direction) {
      // if ((state->direction == LEFT_TO_RIGHT && state->current > state->end)
      // || (state->direction == RIGHT_TO_LEFT && state->current < state->end))
      // {
      //   state->current += 1 * (state->direction == LEFT_TO_RIGHT ? -1 : 1);
      // }

      uint8_t from_brightness = state->ctx->framebuffer[i].v;

      if (abs(from_brightness - to_brightness) > 0) {
        if (from_brightness >= state->interval_brightness && i == state->end &&
            (state->direction == LEFT_TO_RIGHT ? state->end < state->end_limit
                                               : state->end > state->end_limit))
          state->end += 1 * (int)state->direction;

        state->ctx->framebuffer[i].h = state->color.h;
        state->ctx->framebuffer[i].s = state->color.s;

        if (abs(to_brightness - from_brightness) <
            state->brightness_step_size) {
          state->ctx->framebuffer[i].v = to_brightness;
          state->status = ANIM_UPDATED;
        } else if (from_brightness < to_brightness) {
          state->ctx->framebuffer[i].v += state->brightness_step_size;
          state->status = ANIM_UPDATED;
          // ESP_LOGI("FADE_SEQ", "LED %d updated to %d brightness", i,
          //  state->ctx->framebuffer[i].v);
        } else {
          state->ctx->framebuffer[i].v -= state->brightness_step_size;
          state->status = ANIM_UPDATED;
        }
      } else if (i == state->start && (state->direction == LEFT_TO_RIGHT
                                           ? state->start < state->end_limit
                                           : state->start > state->end_limit))
        state->start += 1 * (int)state->direction;
    }

    if (state->status == ANIM_WAITING) {
      state->status = ANIM_DONE;
      return;
    }
  }
}