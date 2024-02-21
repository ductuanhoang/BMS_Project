/*
 * template.c
 *
 *  Created on:
 *      Author:
 */

/***********************************************************************************************************************
 * Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes <System Includes>
 ***********************************************************************************************************************/
#include "common.h"
#include "nextion.h"
#include "uart.h"
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/
static void nextion_receive_message(uint8_t *message, uint16_t len);
extern UART_HandleTypeDef huart1;
/***********************************************************************************************************************
 * Exported global variables and functions (to be accessed by other files)
 ***********************************************************************************************************************/
uint8_t nextion_receive_buffer[NEXTION_UART_MAX_LENGTH];
uint8_t nextion_receive_len = 0;
static uint8_t new_event_button = 0;
/***********************************************************************************************************************
 * Imported global variables and functions (from other files)
 ***********************************************************************************************************************/
void nextion_init(void)
{
    userUartCallback(nextion_receive_message);
    userUartDMAInit();
}

/**
 * @brief
 *
 */
extern void nextion_process_button(uint8_t *message, uint16_t len);
void nextion_process_event(void)
{
    if (new_event_button)
    {
        nextion_process_button(nextion_receive_buffer, nextion_receive_len);
        new_event_button = 0;
        // clear buffer
        nextion_receive_len = 0;
        memset(nextion_receive_buffer, 0x00, sizeof(nextion_receive_buffer));
    }
}
/***********************************************************************************************************************
 * static functions
 ***********************************************************************************************************************/

/**
 * @brief
 *
 * @param message
 * @param len
 */
static void nextion_receive_message(uint8_t *message, uint16_t len)
{
    // process  of message received from
    //    memcpy(nextion_receive_buffer, message, len);
    if (new_event_button == 0)
    {
        for (size_t i = 0; i < len - 3; i++)
        {
            nextion_receive_buffer[i] = message[i];
            
        }
        nextion_receive_len = len - 3;
        // set event new button handler
        new_event_button = 1;
    }
}

/**
 * @brief
 *
 * @param message
 * @param len
 */
void nextion_send_message(uint8_t *message, uint16_t len)
{
    // LOGI("nextion_send_message: %s -- len = %d", message, len);
    userUartDMASendMessage((uint8_t *)message, len);
    uint8_t end_message[2];
    end_message[0] = 0xFF; //
    end_message[1] = 0xFF; //
    userUartDMASendMessage((uint8_t *)end_message, 1);
    userUartDMASendMessage((uint8_t *)end_message, 1);
    userUartDMASendMessage((uint8_t *)end_message, 1);
}

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/