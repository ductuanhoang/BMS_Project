/**
 * @file CANparser.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-10-09
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
#include "CANparser.h"
#include "SerialMessage.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

#include "sys_command.h"
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/
static void user_serial_receive_message(uint8_t *message, uint16_t len);
uint8_t user_json_serial_receive_buffer[UART_COMMAND_MAX_LENGTH];
uint8_t user_json_serial_receive_buffer_size = 0;
uint8_t user_json_serial_event = 0;
int serial_ParserCommandHandler(uint8_t *message, uint16_t message_len);
/***********************************************************************************************************************
 * Exported global variables and functions (to be accessed by other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Imported global variables and functions (from other files)
 ***********************************************************************************************************************/
/**
 *
 */
void SerialMessageRegister()
{
    userUartJsonCommandCallback(user_serial_receive_message);
}

/**
 * @brief call this function in loop
 *
 */
void SerialMessageProcessMessage(void)
{
    if (user_json_serial_event)
    {
        user_json_serial_event = 0;
        serial_ParserCommandHandler(user_json_serial_receive_buffer,
                                    user_json_serial_receive_buffer_size);
        user_json_serial_receive_buffer_size = 0;
        memset(user_json_serial_receive_buffer, 0x00, sizeof(user_json_serial_receive_buffer));
    }
}
/**
 * @brief
 *
 */
void serialMessageSend(void)
{
    // uint8_t message[256] = "{\"bat_id\":1,\"cells\":[15,16,17], \"temp\":[123,456], \"soc\":100,\"soc\":100, \"err\":8}";
    uint8_t *message;
    for (size_t i = 1; i < 11; i++)
    {
        message = (uint8_t *)malloc(256 * sizeof(uint8_t)); // Allocate memory for the array
        if (message == NULL)
        {
            LOGI("Memory allocation failed!\n");
            return;
        }
        sprintf(message, "{\"battery_id\":%d,\"cells\":[%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d], \"temperature\":[%d,%d,%d], \"current\":%d,\"soc\":%d, \"err\":8, \"status\":%d}\r\n",
                i,
                battery_info[i].cell[0].voltage, battery_info[i].cell[1].voltage, battery_info[i].cell[2].voltage,
                battery_info[i].cell[3].voltage, battery_info[i].cell[4].voltage, battery_info[i].cell[5].voltage,
                battery_info[i].cell[6].voltage, battery_info[i].cell[7].voltage, battery_info[i].cell[8].voltage,
                battery_info[i].cell[9].voltage, battery_info[i].cell[10].voltage, battery_info[i].cell[11].voltage,
                battery_info[i].cell[12].voltage, battery_info[i].cell[13].voltage, battery_info[i].cell[14].voltage,
                battery_info[i].temperature[0], battery_info[i].temperature[1], battery_info[i].temperature[2],
                battery_info[i].current,
                battery_info[i].pack_soc,
                battery_info[i].active);
        LOG_COMMAND(message, strlen(message));
        free(message);
    }
}

// https://docs.google.com/spreadsheets/d/1qbJindxINeALQaSMDtJ9Mz8mke8QkbLv8wckIjf6AgE/edit#gid=0
/**
 * @brief
 *
 * @param message
 * @param message_len
 *
 * {"command":"calib","current_load":40}
 * *{"command":"calib","reponse":true,"current_load":40}
 * *{"command":"stop", "password": "123456789"}
 * *{"command":"enable","password": "123456789"}
 * *{"command":"test", "data":"1234123"}
 * {"command":"simulate","data":0x01"}
 */

#define COMMAND_CALIB "calib"
#define COMMAND_TEST "test"
#define COMMAND_SIMULATE "simulate"

int serial_ParserCommandHandler(uint8_t *message, uint16_t message_len)
{
    int ret = -1;
    const cJSON *resolution = NULL;
    const cJSON *resolutions = NULL;
    const cJSON *command = NULL;
    const cJSON *stop = NULL;
    const cJSON *enable = NULL;
    const cJSON *test = NULL;
    // uint8_t test_message[] = "{\"command\":\"calib\",\"current_load\":41}";
    // parse the JSON data
    // cJSON *json = cJSON_ParseWithLength(test_message, strlen(test_message));
    cJSON *json = cJSON_ParseWithLength(message, message_len);
    if (json == NULL)
    {
        LOGI("message doesn't json format: %s with len = %d\n", message, message_len);
        return -1;
    }
    LOGI("json command \"%s\" with len = %d\n", message, message_len);
    command = cJSON_GetObjectItemCaseSensitive(json, "command");
    if (cJSON_IsString(command) && (command->valuestring != NULL))
    {
        LOGI("command: %s", command->valuestring);
    }

    cJSON_Delete(json);
    return ret;
}

/**
 * @brief
 *
 * @param command_type
 */
void serial_PacketMessage(e_type_command command_type)
{
    switch (command_type)
    {
    case E_OK:
        /* code */
        break;
    case E_CALIB_REPONSE:
        break;
    default:
        break;
    }
}
/***********************************************************************************************************************
 * static functions
 ***********************************************************************************************************************/

/**
 * @brief
 *
 * @param command
 * @param message
 */
static void process_command(const char *command, const char *message)
{
}

/**
 * @brief
 *
 * @param message
 * @param len
 */
static void user_serial_receive_message(uint8_t *message, uint16_t len)
{
    if (user_json_serial_event == 0)
    {
        for (size_t i = 0; i < len - 2; i++)
        {
            user_json_serial_receive_buffer[i] = message[i];
        }
        user_json_serial_event = 1;
        user_json_serial_receive_buffer_size = len - 2;
    }
}
/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/