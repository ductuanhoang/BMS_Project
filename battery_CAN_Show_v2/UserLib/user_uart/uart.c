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
#include "common.h"
#include "uart.h"
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

extern DMA_HandleTypeDef hdma_usart1_rx;
/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/

uint8_t rxBuffer[2];
uint8_t rxBuffer_huart2[2];
uint8_t rxBufferTotal[NEXTION_UART_MAX_LENGTH + 5];
uint8_t rxBufferHUART2[UART_COMMAND_MAX_LENGTH];
uint8_t size = 0;
uint8_t rxBufferHUART2_size = 0;
receivedCallback_t receivedCallback = NULL;
receivedCallback_t UartJsonCommandCallback = NULL;
/***********************************************************************************************************************
 * Exported global variables and functions (to be accessed by other files)
 ***********************************************************************************************************************/

// void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
// {
//     HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rxBuffer, NEXTION_UART_MAX_LENGTH);
//     __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
//     size = Size;
//     if (receivedCallback != NULL)
//         receivedCallback(rxBuffer, size);
//     memset(rxBuffer, 0x00, NEXTION_UART_MAX_LENGTH);
// }

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == huart1.Instance)
    {
        rxBufferTotal[size] = rxBuffer[0];
        rxBuffer[0] = 0x00;
        if (size >= 3)
        {
            if (size >= NEXTION_UART_MAX_LENGTH ||
                ((rxBufferTotal[size] == 0xFF) && (rxBufferTotal[size - 1] == 0xFF) && (rxBufferTotal[size - 2] == 0xFF)))
            {
                if (size == 3) // noise buffer
                {
                    size = 0;
                    memset(rxBufferTotal, 0x00, NEXTION_UART_MAX_LENGTH);
                }
                else
                {
                    if (receivedCallback != NULL)
                        receivedCallback(rxBufferTotal, size + 1);
                    size = 0;
                    memset(rxBufferTotal, 0x00, NEXTION_UART_MAX_LENGTH);
                }
            }
            else
                size++;
        }
        else
            size++;
        HAL_UART_Receive_IT(&huart1, (uint8_t *)rxBuffer, 1);
    }
    else if (huart->Instance == huart2.Instance)
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
        HAL_UART_Receive_IT(&huart2, (uint8_t *)rxBuffer_huart2, 1);
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
void userUartCallback(receivedCallback_t callback)
{
    if (callback != NULL)
        receivedCallback = callback;
}

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
    HAL_UART_Receive_IT(&huart1, rxBuffer, 1);
    HAL_UART_Receive_IT(&huart2, rxBuffer_huart2, 1);
    // HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rxBuffer, NEXTION_UART_MAX_LENGTH);
    // __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
}

/**
 * @brief
 *
 * @param message
 * @param len
 */
void userUartDMASendMessage(const uint8_t *message, uint16_t len)
{
    // LOGI("userUartDMASendMessage = %s -- %d", message, len);
    HAL_UART_Transmit(&huart1, message, len, 50);
}
/***********************************************************************************************************************
 * static functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/