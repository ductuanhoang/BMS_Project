/**
 * @file SerialMessage.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-10-27
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef SERIAL_MESSAGE_H
#define SERIAL_MESSAGE_H
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h> /* abs */
#include <stdint.h>
#include <stdbool.h>


typedef enum
{
    E_OK = 0,
    E_CALIB_REPONSE,
    E_ENABLE_LOGGING,
    E_DISABLE_LOGGING,
    E_TEST
} e_type_command;

#define SERIAL_COMMAND_CALIB "calib"
#define SERIAL_COMMAND_STOP "stop"
#define SERIAL_COMMAND_enable "enable"

/**
 * @brief call this function in loop
 *
 */
void SerialMessageProcessMessage(void);

/**
 * @brief
 *
 */
void serialMessageSend(void);

// https://docs.google.com/spreadsheets/d/1qbJindxINeALQaSMDtJ9Mz8mke8QkbLv8wckIjf6AgE/edit#gid=0
/**
 * @brief
 *
 * @param message
 * @param message_len
 */
int serial_ParserMessageHandler(uint8_t *message, uint16_t message_len);

/**
 * @brief 
 * 
 * @param command_type 
 */
void serial_PacketMessage(e_type_command command_type);


#endif // SERIAL_MESSAGE_H
