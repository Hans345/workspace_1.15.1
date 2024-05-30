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
#define NMOS1 GPIOA
#define NMOS1_PIN GPIO_PIN_8
#define NMOS2 GPIOA
#define NMOS2_PIN GPIO_PIN_7
#define NMOS3 GPIOA
#define NMOS3_PIN GPIO_PIN_9
#define NMOS4 GPIOB
#define NMOS4_PIN GPIO_PIN_14

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
