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

pwm_sin_mod *pwmPtr;

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
		break;
		//--------------------------------------------------------------------------------------
	case SINUS_MOD:
		// Reset current time
		time_val = 0;
		// LED2 ON
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
		// Update Display
		fillScreen(BLACK);
		ST7735_WriteString(0, 0, "State: SINUS_MOD", Font_7x10, GREEN, BLACK);
		drawFastHLine(0, px_ofs1, 128, WHITE);
		// Start Timer17 for updating values on Display
		HAL_TIM_Base_Start_IT(&htim17);
		// Start Timer16 for LED blinking
		HAL_TIM_Base_Start_IT(&htim16);
		// ADC1 & ADC2 mit DMA starten
		HAL_ADC_Start_DMA(&hadc1, AD_RES_BUFFER_ADC1, 3);
		HAL_ADC_Start_DMA(&hadc2, AD_RES_BUFFER_ADC2, 1);
		// Set PWM Values
		set_pwm_values(fPWM, f0, A0);
		// PWM Timer mit DMA starten
		HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1,
				(uint32_t*) pwmPtr->ccr_arr, pwmPtr->NrOfEl);
		HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_2,
				(uint32_t*) pwmPtr->ccr_arr, pwmPtr->NrOfEl);
		HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
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
	case SINUS_MOD:
		// ON2
		if (time_val > 250) {
			HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
			time_val = 0;
		}
		// ADC
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_4);

		adc_raw_values1.poti1_V = (AD_RES_BUFFER_ADC1[0] << 4); // Map 12 Bit to 16 Bit values
		adc_values1.poti1_Vf = (3.3 * (float) adc_raw_values1.poti1_V) / 65535;
		adc_raw_values1.poti2_V = (AD_RES_BUFFER_ADC1[1] << 4);
		adc_values1.poti2_Vf = (3.3 * (float) adc_raw_values1.poti2_V) / 65535;
		adc_raw_values1.bnc1_V = (AD_RES_BUFFER_ADC1[2] << 4);
		adc_values1.bnc1_Vf = (3.3 * (float) adc_raw_values1.bnc1_V) / 65535;

		adc_raw_values1.bnc2_V = (AD_RES_BUFFER_ADC2[0] << 4);
		adc_values1.bnc2_Vf = (3.3 * (float) adc_raw_values1.bnc2_V) / 65535;

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
	case SINUS_MOD:
		// Start Timer17 for updating values on Display
		HAL_TIM_Base_Stop_IT(&htim17);
		// Start Timer16 for LED blinking
		HAL_TIM_Base_Stop_IT(&htim16);
		// ADC1 & ADC2 mit DMA stoppen
		HAL_ADC_Stop_DMA(&hadc1);
		HAL_ADC_Stop_DMA(&hadc2);
		// PWM Timer mit DMA stoppen
		HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
		HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_2);
		HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
		// dynamisch allozierter Speicher freigeben
		free(pwmPtr);
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

adc_values* get_adc_values(void) {
	return &adc_values1;
}

void set_pwm_values(const uint32_t fPWM, const uint32_t f0, const float A0) {
	// Char array für ausgabe
	char msg_console1[80];
	// Speicher dynamisch allozieren
	pwmPtr = malloc(
			sizeof(pwm_sin_mod) + (uint32_t) (fPWM / f0) * sizeof(uint32_t));
	pwmPtr->NrOfEl = (uint32_t) (fPWM / f0);
	// Struktur mit Werten füllen
	const float pi = M_PI;
	float dRad = (2 * pi) / (pwmPtr->NrOfEl - 1);	// Winkelschritt [rad]
	float sin_val = 0;
	float posOffset0 = 1;
	float posOffsetCurr = 0;
	float norm0 = ((TIM1->ARR) / 2) + 1;
	float normCurr = 0;
	uint32_t i = 1;
	for (i = 0; i < pwmPtr->NrOfEl; i++) {
		if (i == 0) {
			pwmPtr->ccr_arr[i] = (uint32_t) norm0;
		} else if (i == 1) {
			sin_val = A0 * sinf(i * dRad);
			posOffsetCurr = posOffset0 + sin_val;
			normCurr = norm0 * posOffsetCurr;
			pwmPtr->ccr_arr[i] = (uint32_t) normCurr;
		} else {
			sin_val = A0 * sinf(i * dRad);
			posOffsetCurr = posOffset0 + sin_val;
			normCurr = norm0 * posOffsetCurr;
			pwmPtr->ccr_arr[i] = (uint32_t) normCurr;
		}
		// Daten ausgeben
		sprintf(msg_console1, "pwmPtr->ccr_arr[%lu]: %3lu\r\n", i,
				pwmPtr->ccr_arr[i]);
		HAL_UART_Transmit(&huart2, (uint8_t*) msg_console1,
				strlen(msg_console1), HAL_MAX_DELAY);
	}
}
