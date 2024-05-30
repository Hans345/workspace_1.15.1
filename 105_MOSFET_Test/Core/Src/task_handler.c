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
		// Set all MOSFETs LOW
		HAL_GPIO_WritePin(NMOS1, NMOS1_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(NMOS2, NMOS2_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(NMOS3, NMOS3_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(NMOS4, NMOS4_PIN, GPIO_PIN_RESET);
		break;
		//--------------------------------------------------------------------------------------
	case ON1:
		// LED2 ON
		HAL_GPIO_WritePin(LD2, LD2_PIN, GPIO_PIN_SET);
		// Update Display
		fillScreen(BLACK);
		ST7735_WriteString(0, 0, "State: ON1", Font_7x10, GREEN, BLACK);
		// NMOS1, NMOS4: HIGH
		HAL_GPIO_WritePin(NMOS1, NMOS1_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(NMOS4, NMOS4_PIN, GPIO_PIN_SET);
		// NMOS2, NMOS3: LOW
		HAL_GPIO_WritePin(NMOS2, NMOS2_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(NMOS3, NMOS3_PIN, GPIO_PIN_RESET);
		break;
		//--------------------------------------------------------------------------------------
	case ON2:
		// Reset current time
		time_val = 0;
		// Update Display
		fillScreen(BLACK);
		ST7735_WriteString(0, 0, "State: ON2", Font_7x10, GREEN, BLACK);
		// NMOS1, NMOS4: LOW
		HAL_GPIO_WritePin(NMOS1, NMOS1_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(NMOS4, NMOS4_PIN, GPIO_PIN_RESET);
		// NMOS2, NMOS3: HIGH
		HAL_GPIO_WritePin(NMOS2, NMOS2_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(NMOS3, NMOS3_PIN, GPIO_PIN_SET);
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
	case ON1:

		break;
		//--------------------------------------------------------------------------------------
	case ON2:
		// ON2
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
	case ON1:
		break;
		//--------------------------------------------------------------------------------------
	case ON2:
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
