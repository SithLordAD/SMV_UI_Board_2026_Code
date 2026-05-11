#ifndef BUTTON_H
#define BUTTON_H

#include "smv_board_enums.h"
#include "smv_canbus.h"
#include "stdbool.h"
#include "stm32f4xx_hal.h"

#define BUTTON_DEBOUNCE_MS 50

typedef enum {
  BTN_TYPE_MOMENTARY,
  BTN_TYPE_BLINK,
} ButtonType;

typedef struct {
  GPIO_TypeDef *port;
  uint16_t pin;

  GPIO_PinState last_raw_state;
  GPIO_PinState stable_state;

  uint32_t last_debounce_time;

  uint32_t *counter;
  enum UIMessage msg;

  ButtonType type;
  bool blink_state;           // current blink on/off
  uint32_t last_blink_time;   // tracks blink interval
  uint32_t blink_interval_ms; // 0 for momentary buttons
} PushButton;

void Button_Init(PushButton *btn);
void Button_UpdateCAN(PushButton *btn, CANBUS *can);

#endif