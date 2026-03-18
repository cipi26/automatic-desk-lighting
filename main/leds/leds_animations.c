#include "leds/leds_animations.h"
#include "esp_log.h"
#include "led_animation_api.h"
#include "led_animation_types.h"

void anim_startup_init(anim_startup_state_t *state, led_ctx_t *ctx,
                       led_hsv_t color) {
  state->status = ANIM_WAITING;
  state->ctx = ctx;

  anim_sequence_fade_init(ctx, &state->left_side, 29, 0, 20, 10, 10, color);
  anim_sequence_fade_init(ctx, &state->right_side, 30, 59, 20, 10, 10, color);
}

void anim_startup_tick(anim_startup_state_t *state) {
  if (state->status == ANIM_DONE)
    return;

  anim_sequence_fade_tick(&state->left_side);
  anim_sequence_fade_tick(&state->right_side);

  if (state->left_side.status == ANIM_UPDATED ||
      state->right_side.status == ANIM_UPDATED) {
    anim_refresh(state->ctx);
  }

  if (state->left_side.status == ANIM_DONE &&
      state->right_side.status == ANIM_DONE)
    state->status = ANIM_DONE;
}