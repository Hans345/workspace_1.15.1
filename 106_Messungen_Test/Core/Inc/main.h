/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_EXTI13_Pin GPIO_PIN_13
#define B1_EXTI13_GPIO_Port GPIOC
#define B1_EXTI13_EXTI_IRQn EXTI15_10_IRQn
#define Stg2_NMOS1_Pin GPIO_PIN_2
#define Stg2_NMOS1_GPIO_Port GPIOC
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define Stg1_NMOS2_Pin GPIO_PIN_7
#define Stg1_NMOS2_GPIO_Port GPIOA
#define Stg2_NMOS4_Pin GPIO_PIN_5
#define Stg2_NMOS4_GPIO_Port GPIOC
#define Tast1_EXTI1_Pin GPIO_PIN_1
#define Tast1_EXTI1_GPIO_Port GPIOB
#define Tast1_EXTI1_EXTI_IRQn EXTI1_IRQn
#define Tast2_EXTI2_Pin GPIO_PIN_2
#define Tast2_EXTI2_GPIO_Port GPIOB
#define Tast2_EXTI2_EXTI_IRQn EXTI2_IRQn
#define Stg1_NMOS4_Pin GPIO_PIN_14
#define Stg1_NMOS4_GPIO_Port GPIOB
#define Stg3_NMOS1_Pin GPIO_PIN_6
#define Stg3_NMOS1_GPIO_Port GPIOC
#define Stg3_NMOS3_Pin GPIO_PIN_7
#define Stg3_NMOS3_GPIO_Port GPIOC
#define Stg1_NMOS1_Pin GPIO_PIN_8
#define Stg1_NMOS1_GPIO_Port GPIOA
#define Stg1_NMOS3_Pin GPIO_PIN_9
#define Stg1_NMOS3_GPIO_Port GPIOA
#define Stg2_NMOS3_Pin GPIO_PIN_11
#define Stg2_NMOS3_GPIO_Port GPIOA
#define Stg3_NMOS2_Pin GPIO_PIN_10
#define Stg3_NMOS2_GPIO_Port GPIOC
#define Stg3_NMOS4_Pin GPIO_PIN_11
#define Stg3_NMOS4_GPIO_Port GPIOC
#define Stg2_NMOS2_Pin GPIO_PIN_9
#define Stg2_NMOS2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */