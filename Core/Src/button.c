#include "button.h"
#include "smv_canbus.h"

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

  /* If raw state changed, reset debounce timer */
  if (raw_state != btn->last_raw_state) {
    btn->last_raw_state = raw_state;
    btn->last_debounce_time = now;
  }

  /* If input has remained stable long enough */
  if ((now - btn->last_debounce_time) >= BUTTON_DEBOUNCE_MS) {
    /* If debounced state changed */
    if (btn->stable_state != raw_state) {
      btn->stable_state = raw_state;

      *btn->counter = (raw_state == GPIO_PIN_RESET) ? 1 : 0; /* sets ON/OFF */

      can->send(can, (double)(*btn->counter), btn->msg);
    }
  }
}
