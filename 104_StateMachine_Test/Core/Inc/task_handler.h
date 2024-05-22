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
    IDLE,
	LEDOFF,
    LEDON,
    LEDBLINK
};

//-----------------------------------------------------------------------------------------------------------------------

extern uint8_t handler_state;

//-----------------------------------------------------------------------------------------------------------------------

void set_handler_state(uint8_t state);
uint8_t get_handler_state(void);
void handler_task(void);
void handler_rt_task(void);
void set_handler_timeout(uint32_t handler_time);

#endif // __TASK_HANDLER_H__
