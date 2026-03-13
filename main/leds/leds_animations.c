#include "leds/leds_animations.h"
#include "esp_log.h"
#include "led_animation_api.h"
#include "led_animation_types.h"

anim_startup_state_t anim_startup_init(led_ctx_t *ctx, led_hsv_t color) {
  anim_startup_state_t startup_state = {.ctx = ctx};

  anim_sequence_init(ctx, &startup_state.left_side, 29, 0, 20, color);
  anim_sequence_init(ctx, &startup_state.right_side, 30, 59, 20, color);

  return startup_state;
}

anim_status_t anim_startup_tick(anim_startup_state_t *state) {
  anim_status_t left_side_state =
      anim_sequence_tick(state->ctx, &state->left_side);
  anim_status_t right_side_state =
      anim_sequence_tick(state->ctx, &state->right_side);

  if (left_side_state == ANIM_UPDATED || right_side_state == ANIM_UPDATED) {
    anim_refresh(state->ctx);
    return ANIM_UPDATED;
  }
  if (left_side_state == ANIM_DONE && right_side_state == ANIM_DONE)
    return ANIM_DONE;
  return ANIM_WAITING;
}