#ifndef __TASK_HANDLER_H__
#define __TASK_HANDLER_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "stm32g4xx_hal.h"
#include "ST7735.h"
#include "GFX_FUNCTIONS.h"

//-----------------------------------------------------------------------------------------------------------------------
/* PIN DEFINES *********************************************************************************************************/
#define LD2 GPIOA
#define LD2_PIN  GPIO_PIN_5
#define B1 GPIOC
#define B1_PIN GPIO_PIN_13
// NMOS
#define Stg1_NMOS1 GPIOA
#define Stg1_NMOS1_PIN GPIO_PIN_8
#define Stg1_NMOS2 GPIOA
#define Stg1_NMOS2_PIN GPIO_PIN_7
#define Stg1_NMOS3 GPIOA
#define Stg1_NMOS3_PIN GPIO_PIN_9
#define Stg1_NMOS4 GPIOB
#define Stg1_NMOS4_PIN GPIO_PIN_14

#define Stg2_NMOS1 GPIOC
#define Stg2_NMOS1_PIN GPIO_PIN_2
#define Stg2_NMOS2 GPIOB
#define Stg2_NMOS2_PIN GPIO_PIN_9
#define Stg2_NMOS3 GPIOA
#define Stg2_NMOS3_PIN GPIO_PIN_11
#define Stg2_NMOS4 GPIOC
#define Stg2_NMOS4_PIN GPIO_PIN_5

#define Stg3_NMOS1 GPIOC
#define Stg3_NMOS1_PIN GPIO_PIN_6
#define Stg3_NMOS2 GPIOC
#define Stg3_NMOS2_PIN GPIO_PIN_10
#define Stg3_NMOS3 GPIOC
#define Stg3_NMOS3_PIN GPIO_PIN_7
#define Stg3_NMOS4 GPIOC
#define Stg3_NMOS4_PIN GPIO_PIN_11

//-----------------------------------------------------------------------------------------------------------------------

enum {
	IDLE = 0, ON1, ON2
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
