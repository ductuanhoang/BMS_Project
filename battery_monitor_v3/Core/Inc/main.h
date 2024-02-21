/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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
#define USER_LED_Pin GPIO_PIN_13
#define USER_LED_GPIO_Port GPIOC
#define SW_12_Pin GPIO_PIN_3
#define SW_12_GPIO_Port GPIOA
#define SW_11_Pin GPIO_PIN_4
#define SW_11_GPIO_Port GPIOA
#define SW_10_Pin GPIO_PIN_5
#define SW_10_GPIO_Port GPIOA
#define SW_9_Pin GPIO_PIN_6
#define SW_9_GPIO_Port GPIOA
#define SW_8_Pin GPIO_PIN_7
#define SW_8_GPIO_Port GPIOA
#define SW_7_Pin GPIO_PIN_0
#define SW_7_GPIO_Port GPIOB
#define SW_6_Pin GPIO_PIN_1
#define SW_6_GPIO_Port GPIOB
#define SW_5_Pin GPIO_PIN_10
#define SW_5_GPIO_Port GPIOB
#define SW_4_Pin GPIO_PIN_11
#define SW_4_GPIO_Port GPIOB
#define SW_3_Pin GPIO_PIN_12
#define SW_3_GPIO_Port GPIOB
#define SW_2_Pin GPIO_PIN_13
#define SW_2_GPIO_Port GPIOB
#define SW_1_Pin GPIO_PIN_14
#define SW_1_GPIO_Port GPIOB
#define RESET_BQ_Pin GPIO_PIN_8
#define RESET_BQ_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
