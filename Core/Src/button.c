#include "button.h"
#include "smv_canbus.h"
#include "stdbool.h"

#define BUTTON_DEBOUNCE_MS 50 /* 50ms debounce */

/*
 * method to initialize button states
 *
 */
void Button_Init(PushButton *btn) {
  GPIO_PinState state = HAL_GPIO_ReadPin(btn->port, btn->pin);

  btn->last_raw_state = state;
  btn->stable_state = state;
  btn->last_debounce_time = HAL_GetTick();
}

/*
 * method to detect button pushes and send accordingly over can
 */
void Button_UpdateCAN(PushButton *btn, CANBUS *can) {
  GPIO_PinState raw_state = HAL_GPIO_ReadPin(btn->port, btn->pin);
  uint32_t now = HAL_GetTick();

  if (raw_state != btn->last_raw_state) {
    btn->last_raw_state = raw_state;
    btn->last_debounce_time = now;
  }

  if ((now - btn->last_debounce_time) >= BUTTON_DEBOUNCE_MS) {
    if (btn->stable_state != raw_state) {
      btn->stable_state = raw_state;

      if (btn->type == BTN_TYPE_MOMENTARY) {
        *btn->counter = (raw_state == GPIO_PIN_RESET) ? 1 : 0;
        can->send(can, (double)(*btn->counter), btn->msg);

      } else if (btn->type == BTN_TYPE_BLINK) {
        if (raw_state != GPIO_PIN_RESET) {
          // only send 0 if blink was actually active
          if (btn->blink_state) {
            btn->blink_state = false;
            can->send(can, 0.0, btn->msg);
          }
        }
      }
    }
  }

  // blink tick — only runs while button is held
  if (btn->type == BTN_TYPE_BLINK && btn->stable_state == GPIO_PIN_RESET) {
    if (now - btn->last_blink_time >= btn->blink_interval_ms) {
      btn->blink_state = !btn->blink_state;
      btn->last_blink_time = now;
      can->send(can, btn->blink_state ? 1.0 : 0.0, btn->msg);
    }
  }
}