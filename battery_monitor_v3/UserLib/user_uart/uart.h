/**
 * @file uart.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-12-04
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef USER_UART_H
#define USER_UART_H

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <stdint.h>
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
typedef void (*receivedCallback_t)(uint8_t *, uint16_t);
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
#define UART_COMMAND_MAX_LENGTH (128)
/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/
/**
 * @brief init uart nextion lcd dma
 *
 */
void userUartDMAInit(void);

/**
 * @brief
 *
 * @param message
 * @param len
 */
void userUartDMASendMessage(const uint8_t *message, uint16_t len);

/**
 * @brief register call back function
 * 
 * @param callback 
 */
void userUartCallback(receivedCallback_t callback);

/**
 * @brief register call back function
 *
 * @param callback
 */
void userUartJsonCommandCallback(receivedCallback_t callback);

#endif /* USER_UART_H */
