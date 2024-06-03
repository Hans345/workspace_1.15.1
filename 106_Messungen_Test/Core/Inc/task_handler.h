#ifndef __TASK_HANDLER_H__
#define __TASK_HANDLER_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "stm32g4xx_hal.h"
#include "ST7735.h"
#include "GFX_FUNCTIONS.h"

//-----------------------------------------------------------------------------------------------------------------------
// Strukturen für ADC Messungen
typedef struct {
	uint16_t poti1_V;
	uint16_t poti2_V;
	uint16_t bnc1_V;
	uint16_t bnc2_V;
}adc_raw_values;

typedef struct {
	float poti1_Vf;
	float poti2_Vf;
	float bnc1_Vf;
	float bnc2_Vf;
}adc_values;

//-----------------------------------------------------------------------------------------------------------------------

enum {
	IDLE = 0, ADC_Values
};

//-----------------------------------------------------------------------------------------------------------------------

extern uint8_t handler_state;

//-----------------------------------------------------------------------------------------------------------------------

void handler_task(void);
void set_handler_state(uint8_t state);
void increase_handler_state(void);
uint8_t get_handler_state(void);
void inc_handler_currentTime(uint16_t dT);
adc_values* get_adc_values(void);

#endif // __TASK_HANDLER_H__
