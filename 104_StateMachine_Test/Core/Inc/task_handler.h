#ifndef __TASK_HANDLER_H__
#define __TASK_HANDLER_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "stm32g4xx_hal.h"

//-----------------------------------------------------------------------------------------------------------------------
/****** PIN DEFINES ******/
#define LD2 GPIOA
#define LD2_PIN  GPIO_PIN_5
#define B1 GPIOC
#define B1_PIN GPIO_PIN_13

//-----------------------------------------------------------------------------------------------------------------------

enum{
    IDLE = 0,
    LEDON,
    LEDBLINK
};

//-----------------------------------------------------------------------------------------------------------------------

extern uint8_t handler_state;

//-----------------------------------------------------------------------------------------------------------------------

void handler_task(void);
void set_handler_state(uint8_t state);
void increase_handler_state(void);
uint8_t get_handler_state(void);
void inc_handler_currentTime(uint16_t dT);

#endif // __TASK_HANDLER_H__
