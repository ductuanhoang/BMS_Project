/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "registers.h"
#include "bms_board_config.h"
#include "bms.h"
#include "common.h"
#include "interface.h"
#include "sw_id.h"
#include "bms_can_trans.h"
#include "uart.h"
#include "bat_simulator.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan;

I2C_HandleTypeDef hi2c1;

IWDG_HandleTypeDef hiwdg;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

bool datacheck = false;
CAN_FilterTypeDef sFilterConfig; // declare CAN filter structure
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_CAN_Init(void);
static void MX_IWDG_Init(void);
/* USER CODE BEGIN PFP */
void user_battery_calculate_soc(Bms *bms);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define defive_ID 0x106
#define defive_ID_check 0x104
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_CAN_Init();
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */
  HAL_IWDG_Refresh(&hiwdg);
  HAL_CAN_Start(&hcan);
  battery_simulator_init();

  // Activate the notification
  HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO1_MSG_PENDING);
  // bms configuration
  Bms bms;
  bms.status.soc = 15;
#ifdef SECOND_BMS
  Bms bms_2;
#endif /* SECOND_BMS */
  bms.conf.shunt_res_mOhm = BOARD_SHUNT_RESISTOR;
  bms.conf.nominal_capacity_Ah = BOARD_BATTERY_CAPACITY;
  bms.conf.bal_idle_current = 0;
  bms.hi2c = hi2c1;
  bms.conf.thermistor_beta = 3950;
#ifdef SECOND_BMS
  bms_2.conf.shunt_res_mOhm = BOARD_SHUNT_RESISTOR;
  bms_2.conf.nominal_capacity_Ah = BOARD_BATTERY_CAPACITY;
  bms_2.conf.bal_idle_current = 0;
  bms_2.hi2c = hi2c2;
  bms_2.conf.thermistor_beta = 3950;
#endif

  while (bms_init_hardware(&bms) != 0)
  {
    HAL_GPIO_WritePin(RESET_BQ_GPIO_Port, RESET_BQ_Pin, GPIO_PIN_SET);
    LOGI("BMS hardware initialization failed pack 1, retrying in 1s");
    HAL_IWDG_Refresh(&hiwdg);
    HAL_Delay(1000);
  }
  HAL_IWDG_Refresh(&hiwdg);
  HAL_GPIO_WritePin(RESET_BQ_GPIO_Port, RESET_BQ_Pin, GPIO_PIN_RESET);
#ifdef SECOND_BMS
  while (bms_init_hardware(&bms_2) != 0)
  {
    LOGI("BMS hardware initialization failed pack 2, retrying in 1s");
    HAL_Delay(1000);
  }
#endif

  bq769x0_write_byte(&bms.hi2c, BQ769X0_CELLBAL1, 0x00);
  HAL_Delay(100);
  bq769x0_write_byte(&bms.hi2c, BQ769X0_CELLBAL2, 0x00);
  HAL_Delay(100);
  bq769x0_write_byte(&bms.hi2c, BQ769X0_CELLBAL3, 0x00);
  HAL_Delay(100);

#ifdef SECOND_BMS
  bq769x0_write_byte(&bms_2.hi2c, BQ769X0_CELLBAL1, 0x00);
  HAL_Delay(100);
  bq769x0_write_byte(&bms_2.hi2c, BQ769X0_CELLBAL2, 0x00);
  HAL_Delay(100);
  bq769x0_write_byte(&bms_2.hi2c, BQ769X0_CELLBAL3, 0x00);
  HAL_Delay(100);
#endif

  LOGI("BQ769X0_CELLBAL1 pack 1 = %x \r\n", bq769x0_read_byte(&bms.hi2c, BQ769X0_CELLBAL1));
  LOGI("BQ769X0_CELLBAL1 pack 1 = %x \r\n", bq769x0_read_byte(&bms.hi2c, BQ769X0_CELLBAL2));
  LOGI("BQ769X0_CELLBAL1 pack 1 = %x \r\n", bq769x0_read_byte(&bms.hi2c, BQ769X0_CELLBAL3));
#ifdef SECOND_BMS
  LOGI("BQ769X0_CELLBAL1 pack 2 = %x \r\n", bq769x0_read_byte(&bms_2.hi2c, BQ769X0_CELLBAL1));
  LOGI("BQ769X0_CELLBAL1 pack 2 = %x \r\n", bq769x0_read_byte(&bms_2.hi2c, BQ769X0_CELLBAL2));
  LOGI("BQ769X0_CELLBAL1 pack 2 = %x \r\n", bq769x0_read_byte(&bms_2.hi2c, BQ769X0_CELLBAL3));
#endif
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    // clear wdg interrupt
    static uint32_t clear_wdog_prev = 0;
    if (HAL_GetTick() - clear_wdog_prev >= 1000)
    {
      clear_wdog_prev = HAL_GetTick();
      user_battery_calculate_soc(&bms);
      HAL_IWDG_Refresh(&hiwdg);
    }
    // toggle led state
    static uint32_t update_led_prev = 0;
    if (HAL_GetTick() - update_led_prev >= 1000)
    {
      update_led_prev = HAL_GetTick();
      bms_update(&bms);
      HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);
    }

    // process command json command
    static uint32_t json_command_prev = 0;
    if (HAL_GetTick() - json_command_prev >= 200)
    {
      json_command_prev = HAL_GetTick();
      battery_simulator_process();
    }
    // update battery status
    static uint32_t update_battery_prev = 0;
    if (HAL_GetTick() - update_battery_prev >= 1000)
    {
      update_battery_prev = HAL_GetTick();
      LOGI("sw_id_detect() = %d", sw_id_detect());
      if (sw_id_detect() == E_SW_ID_24) // no config
        bms_can_packet_simulation_message(&simulate_data);
      else
        bms_can_packet_message(&bms, sw_id_detect(), NULL);
    }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief CAN Initialization Function
 * @param None
 * @retval None
 */
static void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 18;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_2TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */
  CAN_FilterTypeDef canfilterconfig;

  canfilterconfig.FilterActivation = CAN_FILTER_ENABLE;
  canfilterconfig.FilterBank = 0; // which filter bank to use from the assigned ones
  canfilterconfig.FilterFIFOAssignment = CAN_FILTER_FIFO1;
  canfilterconfig.FilterIdHigh = 0;
  canfilterconfig.FilterIdLow = 0;
  canfilterconfig.FilterMaskIdHigh = 0;
  canfilterconfig.FilterMaskIdLow = 0x0000;
  canfilterconfig.FilterMode = CAN_FILTERMODE_IDMASK;
  canfilterconfig.FilterScale = CAN_FILTERSCALE_32BIT;
  canfilterconfig.SlaveStartFilterBank = 0; // doesn't matter in single can controllers

  HAL_CAN_ConfigFilter(&hcan, &canfilterconfig);

  /* USER CODE END CAN_Init 2 */
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */
}

/**
 * @brief IWDG Initialization Function
 * @param None
 * @retval None
 */
static void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_64;
  hiwdg.Init.Reload = 3124;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */
}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
  userUartDMAInit();
  /* USER CODE END USART1_Init 2 */
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RESET_BQ_GPIO_Port, RESET_BQ_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_LED_Pin */
  GPIO_InitStruct.Pin = USER_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USER_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SW_12_Pin SW_11_Pin SW_10_Pin SW_9_Pin
                           SW_8_Pin */
  GPIO_InitStruct.Pin = SW_12_Pin | SW_11_Pin | SW_10_Pin | SW_9_Pin | SW_8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : SW_7_Pin SW_6_Pin SW_5_Pin SW_4_Pin
                           SW_3_Pin SW_2_Pin SW_1_Pin */
  GPIO_InitStruct.Pin = SW_7_Pin | SW_6_Pin | SW_5_Pin | SW_4_Pin | SW_3_Pin | SW_2_Pin | SW_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : RESET_BQ_Pin */
  GPIO_InitStruct.Pin = RESET_BQ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RESET_BQ_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
// void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
// {
//     HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &RxHeader, RxData);
//     if (RxHeader.StdId == defive_ID_check)
//         datacheck = 1;
// }
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define CLAMP(val, low, high) (((val) <= (low)) ? (low) : MIN(val, high))
void user_battery_calculate_soc(Bms *bms)
{
  static float coulomb_counter_mAs = 0;
  if (bms->status.pack_voltage >= 52.5)
  {
    bms->status.soc = 100;
  }
  else
  {
    coulomb_counter_mAs += bms->status.pack_current * 1000;
    float soc_delta = coulomb_counter_mAs / (bms->conf.nominal_capacity_Ah * 3.6e4F);

    if (soc_delta > 0.1F || soc_delta < -0.1F)
    {
      // only update SoC after significant changes to maintain higher resolution
      float soc_tmp = bms->status.soc + soc_delta;
      bms->status.soc = CLAMP(soc_tmp, 0.0F, 100.0F);
      coulomb_counter_mAs = 0;
    }
  }
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
