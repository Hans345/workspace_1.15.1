#include "task_handler.h"

//-----------------------------------------------------------------------------------------------------------------------
/* VARIABLES ***********************************************************************************************************/
uint8_t handler_state;
uint8_t prev_handler_state;
uint16_t time_val = 0;

void enter_handler_state(int state) {
	switch (state) {
	//--------------------------------------------------------------------------------------
	case IDLE:
		// LED2 OFF
		HAL_GPIO_WritePin(LD2, LD2_PIN, GPIO_PIN_RESET);
		handler_state = IDLE;
		// Update Display
		fillScreen(BLACK);
		ST7735_WriteString(0, 0, "State: IDLE", Font_7x10, GREEN, BLACK);
		break;
		//--------------------------------------------------------------------------------------
	case LEDON:
		// LED2 ON
		HAL_GPIO_WritePin(LD2, LD2_PIN, GPIO_PIN_SET);
		// Update Display
		fillScreen(BLACK);
		ST7735_WriteString(0, 0, "State: LEDON", Font_7x10, GREEN, BLACK);
		break;
		//--------------------------------------------------------------------------------------
	case LEDBLINK:
		// Reset current time
		time_val = 0;
		// Update Display
		fillScreen(BLACK);
		ST7735_WriteString(0, 0, "State: LEDBLINK", Font_7x10, GREEN, BLACK);
		break;
		//--------------------------------------------------------------------------------------
	default:
		break;
		//--------------------------------------------------------------------------------------
	}
}

//-----------------------------------------------------------------------------------------------------------------------

void exec_handler_state(int state) {
	switch (state) {
	//--------------------------------------------------------------------------------------
	case IDLE:

		break;
		//--------------------------------------------------------------------------------------
	case LEDON:

		break;
		//--------------------------------------------------------------------------------------
	case LEDBLINK:
		// LEDBLINK
		if (time_val > 250) {
			HAL_GPIO_TogglePin(LD2, LD2_PIN);
			time_val = 0;
		}
		break;
		//--------------------------------------------------------------------------------------
	default:
		break;
		//--------------------------------------------------------------------------------------
	}
}

//-----------------------------------------------------------------------------------------------------------------------

void leave_handler_state(int state) {
	switch (state) {
	//--------------------------------------------------------------------------------------
	case IDLE:
		break;
		//--------------------------------------------------------------------------------------
	case LEDON:
		break;
		//--------------------------------------------------------------------------------------
	case LEDBLINK:
		break;
		//--------------------------------------------------------------------------------------
	default:
		break;
		//--------------------------------------------------------------------------------------
	}
}

//-----------------------------------------------------------------------------------------------------------------------

void handler_task(void) {
	exec_handler_state(handler_state);
}

void set_handler_state(uint8_t state) {
	prev_handler_state = handler_state;
	handler_state = state;
	leave_handler_state(prev_handler_state);
	enter_handler_state(state);
}

uint8_t get_handler_state(void) {
	return handler_state;
}

void inc_handler_currentTime(uint16_t dT) {
	time_val = time_val + dT;
}
