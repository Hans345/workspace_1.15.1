#include "task_handler.h"
#include "main.h"

//-----------------------------------------------------------------------------------------------------------------------
/* VARIABLES ***********************************************************************************************************/
uint8_t handler_state;
uint8_t prev_handler_state;
uint16_t time_val = 0;	// 0.1s Zeitschritt
uint8_t px_ofs1 = 11;	// Offset in pixel

adc_raw_values adc_raw_values1;
adc_values adc_values1;

void enter_handler_state(int state) {
	switch (state) {
	//--------------------------------------------------------------------------------------
	case IDLE:
		// LED2 OFF
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
		handler_state = IDLE;
		// Update Display
		fillScreen(BLACK);
		ST7735_WriteString(0, 0, "State: IDLE", Font_7x10, GREEN, BLACK);
		drawFastHLine(0, px_ofs1, 128, WHITE);
		// ADC
		break;
		//--------------------------------------------------------------------------------------
	case ADC_Values:
		// Reset current time
		time_val = 0;
		// LED2 ON
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
		// Update Display
		fillScreen(BLACK);
		ST7735_WriteString(0, 0, "State: ADC_Values", Font_7x10, GREEN, BLACK);
		drawFastHLine(0, px_ofs1, 128, WHITE);
		// Start Timer17 for updating values on Display
		HAL_TIM_Base_Start_IT(&htim17);
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
	case ADC_Values:
		// ON2
		if (time_val > 250) {
			HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
			time_val = 0;
		}
		// ADC
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_4);

		adc_raw_values1.poti1_V = (AD_RES_BUFFER_ADC1[0] << 4); // Map 12 Bit to 16 Bit values
		adc_values1.poti1_Vf = (3.3*(float)adc_raw_values1.poti1_V)/65535;
	    adc_raw_values1.poti2_V = (AD_RES_BUFFER_ADC1[1] << 4);
	    adc_values1.poti2_Vf = (3.3*(float)adc_raw_values1.poti2_V)/65535;
	    adc_raw_values1.bnc1_V = (AD_RES_BUFFER_ADC1[2] << 4);
	    adc_values1.bnc1_Vf = (3.3*(float) adc_raw_values1.bnc1_V)/65535;

	    adc_raw_values1.bnc2_V = (AD_RES_BUFFER_ADC2[0] << 4);
	    adc_values1.bnc2_Vf = (3.3*(float) adc_raw_values1.bnc2_V)/65535;

		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_4);
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
	case ADC_Values:
		// Stop Timer17 for updating values on Display
		HAL_TIM_Base_Start_IT(&htim17);
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

adc_values* get_adc_values(void)
{
	return &adc_values1;
}
