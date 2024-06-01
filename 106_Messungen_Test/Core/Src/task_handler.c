#include "task_handler.h"
#include "main.h"

//-----------------------------------------------------------------------------------------------------------------------
/* VARIABLES ***********************************************************************************************************/
uint8_t handler_state;
uint8_t prev_handler_state;
uint16_t time_val = 0;
char msg[20];
uint8_t px_ofs1 = 11;

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

		ADC_Select_CH1();
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, 20); // timeout 20ms
		adc_raw_values1.poti1_V = HAL_ADC_GetValue(&hadc1);
		adc_values1.poti1_Vf = (3.3*(float)adc_raw_values1.poti1_V)/4095;
		HAL_ADC_Stop(&hadc1);

//		ADC_Select_CH2();
//		HAL_ADC_Start(&hadc1);
//		HAL_ADC_PollForConversion(&hadc1, 20); // timeout 20ms
//		adc_raw_values1.poti2_V = HAL_ADC_GetValue(&hadc1);
//		adc_values1.poti2_Vf = (3.3*(float)adc_raw_values1.poti2_V)/4095;
//		HAL_ADC_Stop(&hadc1);

//		ADC_Select_CH15();
//		HAL_ADC_Start(&hadc1);
//		HAL_ADC_PollForConversion(&hadc1, 20); // timeout 20ms
//		adc_raw_values1.bnc1_V = HAL_ADC_GetValue(&hadc1);
//		adc_values1.bnc1_Vf = (3.3*(float)adc_raw_values1.bnc1_V)/4095;
//		HAL_ADC_Stop(&hadc1);

		HAL_ADC_Start(&hadc2);
		HAL_ADC_PollForConversion(&hadc2, 20); // timeout 20ms
		adc_raw_values1.bnc2_V = HAL_ADC_GetValue(&hadc2);
		adc_values1.bnc2_Vf = (3.3*(float)adc_raw_values1.bnc2_V)/4095;
		HAL_ADC_Stop(&hadc2);

		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_4);
		// Print ADC Value in Console
		sprintf(msg, "Poti1: %2.2fV \r\n", adc_values1.poti1_Vf);
		HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
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
