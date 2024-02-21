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
#define BT_ALARM_Pin GPIO_PIN_0
#define BT_ALARM_GPIO_Port GPIOA
#define BT_SILENCE_Pin GPIO_PIN_1
#define BT_SILENCE_GPIO_Port GPIOA
#define SPI1_CS2_Pin GPIO_PIN_0
#define SPI1_CS2_GPIO_Port GPIOB
#define SPI1_HL_Pin GPIO_PIN_1
#define SPI1_HL_GPIO_Port GPIOB
#define CAN_INT_Pin GPIO_PIN_10
#define CAN_INT_GPIO_Port GPIOB
#define FLASH_WP_Pin GPIO_PIN_11
#define FLASH_WP_GPIO_Port GPIOB
#define LED_BLUE_Pin GPIO_PIN_12
#define LED_BLUE_GPIO_Port GPIOB
#define LED_GREEN_Pin GPIO_PIN_13
#define LED_GREEN_GPIO_Port GPIOB
#define LED_RED_Pin GPIO_PIN_14
#define LED_RED_GPIO_Port GPIOB
#define CTR_RELAY_2_Pin GPIO_PIN_15
#define CTR_RELAY_2_GPIO_Port GPIOA
#define CTR_RELAY_1_Pin GPIO_PIN_3
#define CTR_RELAY_1_GPIO_Port GPIOB
#define SIMU_CAN_TX_Pin GPIO_PIN_4
#define SIMU_CAN_TX_GPIO_Port GPIOB
#define SIMU_CAN_RX_Pin GPIO_PIN_5
#define SIMU_CAN_RX_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
