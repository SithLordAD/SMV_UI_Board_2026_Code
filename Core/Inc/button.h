#ifndef BUTTON_H
#define BUTTON_H

#include "smv_board_enums.h"
#include "stm32f4xx_hal.h"
#include "smv_canbus.h"


/* Debounce time in milliseconds */
#define BUTTON_DEBOUNCE_MS 50

/* struct to hold all the data per button */
typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;

    GPIO_PinState last_raw_state;   // last immediate read
    GPIO_PinState stable_state;     // debounced stable state

    uint32_t last_debounce_time;

    uint32_t *counter;              // optional debug counter (can be NULL)
    enum UIMessage msg;              // CAN message enum

} PushButton;

/* Initialize button state */
void Button_Init(PushButton *btn);

/* Update button and send ON/OFF over CAN */
void Button_UpdateCAN(PushButton *btn, CANBUS *can);

#endif

