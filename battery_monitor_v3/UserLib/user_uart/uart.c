/**
 * @file uart.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-12-04
 *
 * @copyright Copyright (c) 2023
 *
 */
/***********************************************************************************************************************
 * Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes <System Includes>
 ***********************************************************************************************************************/
#include "uart.h"
#include "main.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h> /* abs */
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
extern UART_HandleTypeDef huart1;

/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/

uint8_t rxBuffer_huart2[2];
uint8_t rxBufferHUART2[UART_COMMAND_MAX_LENGTH];
uint16_t rxBufferHUART2_size = 0;
receivedCallback_t UartJsonCommandCallback = NULL;
/***********************************************************************************************************************
 * Exported global variables and functions (to be accessed by other files)
 ***********************************************************************************************************************/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == huart1.Instance)
    {
        rxBufferHUART2[rxBufferHUART2_size] = rxBuffer_huart2[0];
        rxBuffer_huart2[0] = 0x00;
        if (rxBufferHUART2_size >= UART_COMMAND_MAX_LENGTH ||
            ((rxBufferHUART2[rxBufferHUART2_size] == 0x0A) && (rxBufferHUART2[rxBufferHUART2_size - 1] == 0x0D))) // \r\n
        {
            if (UartJsonCommandCallback != NULL)
                UartJsonCommandCallback(rxBufferHUART2, rxBufferHUART2_size + 1);
            rxBufferHUART2_size = 0;
            memset(rxBufferHUART2, 0x00, UART_COMMAND_MAX_LENGTH);
        }
        else
            rxBufferHUART2_size++;
        HAL_UART_Receive_IT(&huart1, (uint8_t *)rxBuffer_huart2, 1);
    }
}
/***********************************************************************************************************************
 * Imported global variables and functions (from other files)
 ***********************************************************************************************************************/

/**
 * @brief register call back function
 *
 * @param callback
 */
void userUartJsonCommandCallback(receivedCallback_t callback)
{
    if (callback != NULL)
        UartJsonCommandCallback = callback;
}
/**
 * @brief init uart nextion lcd dma
 *
 */
void userUartDMAInit(void)
{
    LOGI("userUartDMAInit");
    HAL_UART_Receive_IT(&huart1, rxBuffer_huart2, 1);
}

/**
 * @brief
 *
 * @param message
 * @param len
 */
void userUartDMASendMessage(const uint8_t *message, uint16_t len)
{
    HAL_UART_Transmit(&huart1, message, len, 50);
}
/***********************************************************************************************************************
 * static functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/
