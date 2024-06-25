#include "task_handler.h"
#include "main.h"

//-----------------------------------------------------------------------------------------------------------------------
/* Private Variables ***************************************************************************************************/
// State Machine
uint8_t handler_state;
uint8_t prev_handler_state;
uint16_t time_val = 0;	// 0.1s Zeitschritt

// Display
const uint8_t next_line = 12;	// Offset in pixel

// ADC
adc_raw_values adc_raw_values1;
adc_values adc_values1;
// Arrays für ADC, DMA schreibt direkt in diese Arrays
uint32_t AD_RES_BUFFER_ADC1[3];
uint32_t AD_RES_BUFFER_ADC2[1];

// PWM Modulation
pwm_sin_mod *pwmPtr_m1p;
pwm_sin_mod *pwmPtr_m1n;
pwm_sin_mod *pwmPtr_m2p;
const int stufen = 2;
const int level = 2;
const uint32_t fPWM = 20000;	// Schaltfrequenz in [Hz]
const uint32_t f0 = 500;		// Zu modulierende Frequenz [Hz]
const float A0 = 1.0;			// Zu modulierende Amplitude [0...1.5]

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
		drawFastHLine(0, next_line, 128, WHITE);
		break;
		//--------------------------------------------------------------------------------------
	case SINUS_MOD:
		// Reset current time
		time_val = 0;
		// LED2 ON
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
		// Update Display
		set_display(stufen, level);
		// Start Timer17 for updating values on Display
		HAL_TIM_Base_Start_IT(&htim17);
		// Start Timer16 for LED blinking
		HAL_TIM_Base_Start_IT(&htim16);
		// ADC1 & ADC2 mit DMA starten
		HAL_ADC_Start_DMA(&hadc1, AD_RES_BUFFER_ADC1, 3);
		HAL_ADC_Start_DMA(&hadc2, AD_RES_BUFFER_ADC2, 1);
		// Set PWM Values
		set_pwm_values(stufen, level);
		// PWM Timer mit DMA starten
		start_pwm(stufen, level);
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
		stopp_pwm(stufen, level);
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

void set_pwm_values(const int stufen, const int level) {
	// Char array für ausgabe
	char msg_console[80] = "";
	// Konstanten
	const float pi = M_PI;
	uint32_t sz = (uint32_t)(fPWM/f0);
	const float dRad = (2 * pi) / (sz - 1);	// Winkelschritt [rad]
	const float norm0 = ((TIM1->ARR) / 2) + 1;
	// Variablen
	uint32_t i = 0;
	float sin_val = 0;
	float posOffset0 = 1;
	float posOffsetCurr_1 = 0;
	float normCurr_1 = 0;
	// Speicher dynamisch allozieren
	switch (stufen) {
	case 1:
		switch (level) {
		case 2:
			pwmPtr_m1p = malloc(sizeof(pwm_sin_mod) + sz * sizeof(uint32_t));
			pwmPtr_m1p->NrOfEl = sz;
			// Struktur mit Werten füllen
			for (i = 0; i < sz; i++) {
				if (i == 0) {
					pwmPtr_m1p->ccr_arr[i] = (uint32_t) norm0;
				} else {
					sin_val = A0 * sinf(i * dRad);
					posOffsetCurr_1 = posOffset0 + sin_val;
					normCurr_1 = norm0 * posOffsetCurr_1;
					pwmPtr_m1p->ccr_arr[i] = (uint32_t) normCurr_1;
				}
				// Dynamisch allozierter Speicher mit PuTTY Konsole ausgeben
				sprintf(msg_console,"pwmPtr_m1p->ccr_arr[%lu]: %3lu\r\n",i, pwmPtr_m1p->ccr_arr[i]);
				// PuTTY Ausgabe
				HAL_UART_Transmit(&huart2, (uint8_t*) msg_console, strlen(msg_console), HAL_MAX_DELAY);
			}
			break;
		case 3:
			pwmPtr_m1p = malloc(sizeof(pwm_sin_mod) + sz * sizeof(uint32_t));
			pwmPtr_m1p->NrOfEl = sz;
			pwmPtr_m1n = malloc(sizeof(pwm_sin_mod) + sz * sizeof(uint32_t));
			pwmPtr_m1n->NrOfEl = sz;
			// Variablen
			float posOffsetCurr_2 = 0;
			float normCurr_2 = 0;
			// Struktur mit Werten füllen
			for (i = 0; i < sz; i++) {
				if (i == 0) {
					// m1+, m1-
					pwmPtr_m1p->ccr_arr[i] = (uint32_t) norm0;
					pwmPtr_m1n->ccr_arr[i] = (uint32_t) norm0;
				} else {
					// m1+
					sin_val = A0 * sinf(i * dRad);
					posOffsetCurr_1 = posOffset0 + sin_val;
					normCurr_1 = norm0 * posOffsetCurr_1;
					pwmPtr_m1p->ccr_arr[i] = (uint32_t) normCurr_1;
					// m1-
					posOffsetCurr_2 = posOffset0 - sin_val; // Sinusreferenz für 3-Stufen hier MINUS!
					normCurr_2 = norm0 * posOffsetCurr_2;
					pwmPtr_m1n->ccr_arr[i] = (uint32_t) normCurr_2;
				}
				// Dynamisch allozierter Speicher mit PuTTY Konsole ausgeben
				sprintf(msg_console,"pwmPtr_m1p[%lu]: %3lu  |  pwmPtr_m1n[%lu]: %3lu\r\n",
						i, pwmPtr_m1p->ccr_arr[i], i, pwmPtr_m1n->ccr_arr[i]);
				// PuTTY Ausgabe
				HAL_UART_Transmit(&huart2, (uint8_t*) msg_console, strlen(msg_console), HAL_MAX_DELAY);
			}
			break;
		default:
			sprintf(msg_console, "Error: Stufen 1-4 und Level 2-3 moeglich!\n");
			break;
		}
		break;
	case 2:
		switch (level){
		case 2:
			pwmPtr_m1p = malloc(sizeof(pwm_sin_mod) + sz * sizeof(uint32_t));
			pwmPtr_m1p->NrOfEl = sz;
			pwmPtr_m2p = malloc(sizeof(pwm_sin_mod) + sz * sizeof(uint32_t));
			pwmPtr_m2p->NrOfEl = sz;
			// Struktur mit Werten füllen
			for (i = 0; i < sz; i++) {
				if (i == 0) {
					pwmPtr_m1p->ccr_arr[i] = (uint32_t) norm0*stufen;
					if(norm0*stufen - TIM1->ARR < 0)
					{
						pwmPtr_m2p->ccr_arr[i] = 0;
					}
					else{
						pwmPtr_m2p->ccr_arr[i] = (uint32_t) norm0*stufen - TIM1->ARR;
					}
				} else {
					sin_val = A0 * sinf(i * dRad);
					posOffsetCurr_1 = posOffset0 + sin_val;
					normCurr_1 = norm0 * posOffsetCurr_1;
					pwmPtr_m1p->ccr_arr[i] = (uint32_t) normCurr_1*stufen;
					if(normCurr_1*stufen - TIM1->ARR < 0)
					{
						pwmPtr_m2p->ccr_arr[i] = 0;
					}
					else{
						pwmPtr_m2p->ccr_arr[i] = (uint32_t) normCurr_1*stufen - TIM1->ARR;
					}
				}
				// Dynamisch allozierter Speicher mit PuTTY Konsole ausgeben
				sprintf(msg_console,"pwmPtr_m1p->ccr_arr[%lu]: %3lu | pwmPtr_m2p->ccr_arr[%lu]: %3lu\r\n",
						i, pwmPtr_m1p->ccr_arr[i], i, pwmPtr_m2p->ccr_arr[i]);
				// PuTTY Ausgabe
				HAL_UART_Transmit(&huart2, (uint8_t*) msg_console, strlen(msg_console), HAL_MAX_DELAY);
			}
			break;
		case 3:
			break;
		default:
			sprintf(msg_console, "Error: Stufen 1-4 und Level 2-3 moeglich!\n");
			break;
		}
		break;
	case 3:
		switch (level){
		case 2:
			break;
		case 3:
			break;
		default:
			sprintf(msg_console, "Error: Stufen 1-4 und Level 2-3 moeglich!\n");
			break;
		}
		break;
	case 4:
		switch (level){
		case 2:
			break;
		case 3:
			break;
		default:
			sprintf(msg_console, "Error: Stufen 1-4 und Level 2-3 moeglich!\n");
			break;
		}
		break;
	default:
		sprintf(msg_console, "Error: Stufen 1-4 und Level 2-3 moeglich!\n");
		break;
	}
}

void stopp_pwm(const int stufen, const int level) {
	// Variablen
	char msg_console[80] = ""; // array für Ausgabe
	uint16_t gpio_state = GPIO_PIN_SET;
	switch(stufen){
	case 1:

		// PWM Stoppen ohne Kurzschluss
		while (gpio_state != GPIO_PIN_RESET) {
			gpio_state = HAL_GPIO_ReadPin(TIM1_CH1N_NMOS2_LS_GPIO_Port,
			TIM1_CH1N_NMOS2_LS_Pin);
		}
		// Stop PWM NMOS1 & NMOS2
		HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);

		while (gpio_state != GPIO_PIN_RESET) {
			gpio_state = HAL_GPIO_ReadPin(TIM1_CH2N_NMOS4_LS_GPIO_Port,
			TIM1_CH2N_NMOS4_LS_Pin);
		}
		// Stop PWM NMOS3 & NMOS4
		HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
		HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_2);

		// dynamisch allozierter Speicher freigeben
		switch(level){
		case 2:
			free(pwmPtr_m1p);
			break;
		case 3:
			free(pwmPtr_m1p);
			free(pwmPtr_m1n);
			break;
		default:
			sprintf(msg_console, "Error: Stufen 1-4 und Level 2-3 moeglich!\n");
			break;
		}
		break;
	case 2:

		// PWM Stoppen ohne Kurzschluss
		while (gpio_state != GPIO_PIN_RESET) {
			gpio_state = HAL_GPIO_ReadPin(TIM1_CH1N_NMOS2_LS_GPIO_Port,
			TIM1_CH1N_NMOS2_LS_Pin);
		}
		// Stop PWM NMOS1 & NMOS2
		HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);

		while (gpio_state != GPIO_PIN_RESET) {
			gpio_state = HAL_GPIO_ReadPin(TIM1_CH2N_NMOS4_LS_GPIO_Port,
			TIM1_CH2N_NMOS4_LS_Pin);
		}
		// Stop PWM NMOS3 & NMOS4
		HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
		HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_2);

		// PWM Stoppen ohne Kurzschluss
		while (gpio_state != GPIO_PIN_RESET) {
			gpio_state = HAL_GPIO_ReadPin(TIM1_CH3N_NMOS2_LS_GPIO_Port,
			TIM1_CH3N_NMOS2_LS_Pin);
		}
		// Stop PWM NMOS1 & NMOS2
		HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);
		HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_3);

		while (gpio_state != GPIO_PIN_RESET) {
			gpio_state = HAL_GPIO_ReadPin(TIM1_CH4N_NMOS4_LS_GPIO_Port,
			TIM1_CH4N_NMOS4_LS_Pin);
		}
		// Stop PWM NMOS3 & NMOS4
		HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_4);
		HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_4);

		// dynamisch allozierter Speicher freigeben
		switch(level){
		case 2:
			free(pwmPtr_m1p);
			free(pwmPtr_m2p);
			break;
		case 3:
			break;
		default:
			sprintf(msg_console, "Error: Stufen 1-4 und Level 2-3 moeglich!\n");
			break;
		}
		break;
	case 3:
		break;
	case 4:
	default:
		sprintf(msg_console, "Error: Stufen 1-4 und Level 2-3 moeglich!\n");
		break;
	}
	// PuTTY Ausgabe
	HAL_UART_Transmit(&huart2, (uint8_t*) msg_console, strlen(msg_console), HAL_MAX_DELAY);
}

void start_pwm(const int stufen, const int level)
{
	// Variablen
	char msg_console[80] = ""; // array für Ausgabe

	switch (stufen) {
	case 1:
		switch (level) {
		case 2:
			// TIM1_CH1 & CH1N
			HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1,
					(uint32_t*) pwmPtr_m1p->ccr_arr, pwmPtr_m1p->NrOfEl);
			HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
			// TIM1 CH2 & CH2N
			HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_2,
					(uint32_t*) pwmPtr_m1p->ccr_arr, pwmPtr_m1p->NrOfEl);
			HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
			break;
		case 3:
			// TIM1 CH1 & CH1N
			HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1,
					(uint32_t*) pwmPtr_m1p->ccr_arr, pwmPtr_m1p->NrOfEl);
			HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
			// TIM1 CH2 & CH2N
			HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_2,
					(uint32_t*) pwmPtr_m1n->ccr_arr, pwmPtr_m1n->NrOfEl);
			HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
			break;
		default:
			sprintf(msg_console, "Error: Stufen 1-4 und Level 2-3 moeglich!\n");
			break;
		}
		break;
	case 2:
		switch (level) {
		case 2:
			// TIM1_CH1 & CH1N
			HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1,
					(uint32_t*) pwmPtr_m1p->ccr_arr, pwmPtr_m1p->NrOfEl);
			HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
			// TIM1 CH2 & CH2N
			HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_2,
					(uint32_t*) pwmPtr_m1p->ccr_arr, pwmPtr_m1p->NrOfEl);
			HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
			// TIM1_CH3 & CH3N
			HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_3,
					(uint32_t*) pwmPtr_m2p->ccr_arr, pwmPtr_m2p->NrOfEl);
			HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
			// TIM1 CH4 & CH4N
			HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_4,
					(uint32_t*) pwmPtr_m2p->ccr_arr, pwmPtr_m2p->NrOfEl);
			HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_4);
			break;
		case 3:
			break;
		default:
			sprintf(msg_console, "Error: Stufen 1-4 und Level 2-3 moeglich!\n");
			break;
		}
		break;
	case 3:
		break;
	case 4:
	default:
		sprintf(msg_console, "Error: Stufen 1-4 und Level 2-3 moeglich!\n");
		break;
	}
	// PuTTY Ausgabe
	HAL_UART_Transmit(&huart2, (uint8_t*) msg_console, strlen(msg_console), HAL_MAX_DELAY);
}

void set_display(const int stufen, const int level){
	// Ausgabe Display
	char msg_display[20];
	fillScreen(BLACK);

	ST7735_WriteString(0, 0, "State: SINUS", Font_7x10, GREEN, BLACK);
	drawFastHLine(0, next_line, 128, WHITE);

	sprintf(msg_display, "Stufen: %u", stufen);
	ST7735_WriteString(0, 2*next_line, msg_display, Font_7x10, GREEN, BLACK);

	sprintf(msg_display, "Level:  %u", level);
	ST7735_WriteString(0, 3*next_line, msg_display, Font_7x10, GREEN, BLACK);
}

uint8_t get_next_line(void){
	return next_line;
}

uint32_t get_f0(void) {
	return f0;
}

float get_A0(void) {
	return A0;
}

int get_level(void) {
	return level;
}

int get_stufen(void) {
	return stufen;
}
