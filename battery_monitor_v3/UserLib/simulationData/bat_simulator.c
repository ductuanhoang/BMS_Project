/**
 * @file bms_can_trans.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-09-25
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

#include "main.h"
#include "stm32f1xx_hal_gpio.h"
#include "sw_id.h"
#include "bms_can_trans.h"
#include "bms_board_config.h"
#include "bat_simulator.h"
#include "interface.h"
#include "registers.h"
#include "common.h"
#include "bms.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h" // Include cJSON header
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
static void battery_message_callback(uint8_t *message, uint16_t len);
static uint8_t battery_message_event = 0;
static uint8_t json_buffer_message[UART_COMMAND_MAX_LENGTH];
static uint16_t json_buffer_message_length = 0;
simulate_message_t simulate_data;
static char buffer_sub_token_1[20];
/***********************************************************************************************************************
 * Exported global variables and functions (to be accessed by other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Imported global variables and functions (from other files)
 ***********************************************************************************************************************/

/**
 * @brief
 *  const char *jsonString = "{\"battery_id\":0,\"cell_voltage\":[\"12.3\",\"12\",\"12\",\"12\",\"12\",\"12\",\"12\",\"12\",\"12\",\"12\",\"12\",\"12\"],\"current\":\"12\",\"soc\":\"200\",\"temperature\":[\"12\",\"12\",\"12\"]}";
 * @param jsonString
 * @param data
 */
void parseJsonMessage(const char *jsonString, simulate_message_t *data, uint16_t len)
{
    cJSON *json_root = cJSON_Parse(jsonString);
    LOGI("json_root %s", jsonString);
    //    cJSON *json_root = cJSON_ParseWithLength(jsonString, strlen(jsonString));
    cJSON *json_current = NULL;
    cJSON *json_id = NULL;
    cJSON *temperature_array = NULL;
    cJSON *soc = NULL;
    cJSON *cell = NULL; // each cell has 3 elements
    if (json_root != NULL)
    {
        // Extract data from the parsed JSON
        json_id = cJSON_GetObjectItem(json_root, "battery_id");
        if (json_id != NULL)
            data->battery_id = (json_id)->valueint;

        json_current = cJSON_GetObjectItem(json_root, "current");
        if (json_current != NULL)
            data->current = (json_current)->valuedouble;

        soc = cJSON_GetObjectItem(json_root, "soc");
        if (soc != NULL)
            data->soc = (soc)->valuedouble;

        temperature_array = cJSON_GetObjectItem(json_root, "temperature");
        // // LOGI("temperature_array = %d", cJSON_GetArraySize(temperature_array));
        if (temperature_array != NULL)
        {
            LOGI("wrong");
            for (size_t i = 0; i < cJSON_GetArraySize(temperature_array); i++)
            {
                data->temperature[i] = cJSON_GetArrayItem(temperature_array, i)->valuedouble;
            }
        }
        // get cell 1
        cell = cJSON_GetObjectItem(json_root, "cell1");
        if (cell != NULL)
        {
            LOGI("cell1");
            for (size_t i = 0; i < cJSON_GetArraySize(cell); i++)
                data->cell_voltage[i + 0] = cJSON_GetArrayItem(cell, i)->valuedouble;
        }
        // get cell 2
        cell = cJSON_GetObjectItem(json_root, "cell2");
        if (cell != NULL)
        {
            LOGI("cell2");
            for (size_t i = 0; i < cJSON_GetArraySize(cell); i++)
                data->cell_voltage[i + 3] = cJSON_GetArrayItem(cell, i)->valuedouble;
        }
        // get cell 3
        cell = cJSON_GetObjectItem(json_root, "cell3");
        if (cell != NULL)
        {
            LOGI("cell3");
            for (size_t i = 0; i < cJSON_GetArraySize(cell); i++)
                data->cell_voltage[i + 6] = cJSON_GetArrayItem(cell, i)->valuedouble;
        }
        // get cell 4
        cell = cJSON_GetObjectItem(json_root, "cell4");
        if (cell != NULL)
        {
            LOGI("cell4");
            for (size_t i = 0; i < cJSON_GetArraySize(cell); i++)
                data->cell_voltage[i + 9] = cJSON_GetArrayItem(cell, i)->valuedouble;
        }
    }
    else
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        LOGI("error getting json type: [%s]", error_ptr);
    }
    // cJSON_Delete(json_current);
    // cJSON_Delete(json_id);
    cJSON_Delete(json_root);
}

/**
 * @brief
 *
 * @param message
 * @param data
 */
void parserSubToken(char *message, simulate_message_t *data)
{
    char *buffer = strtok(message, ",");
    int temp_index = 0;
    while (buffer != NULL && temp_index < 3)
    {
        data->temperature[temp_index++] = atof(buffer);
        buffer = strtok(NULL, ",");
    }
}
/**
 * @brief
 * "{\"data\":[123]  [1.2]   [80.1]  [3.2,3.3,3.4]  [12.3,11.8,13.2,12.0,11.5,12.6,11.9,12.8,13.1,11.7,12.2,12.4]}";
 *              id  current    soc     temperature                           cells voltage
 * @param message
 * @param data
 */
void parseNonJsonMessage(char *message, simulate_message_t *data, uint16_t len)
{
    char *token = strtok(message, "[]");

    int count = 0;
    char *subtoken;

    while (token != NULL)
    {
        if (count == 1)
        {
            data->battery_id = atoi(token);
        }
        else if (count == 2)
        {
            data->current = atof(token);
        }
        else if (count == 3)
        {
            data->soc = atof(token);
        }
        else if (count == 4)
        {
            sprintf(buffer_sub_token_1, "%s", token);
        }
        else if (count == 5)
        {
            // Process the cell_voltage array [12.3,11.8,13.2,12.0,11.5,12.6,11.9,12.8,13.1,11.7,12.2,12.4]
            char *subtoken = strtok(token, ",");
            int voltage_index = 0;
            while (subtoken != NULL && voltage_index < 15)
            {
                data->cell_voltage[voltage_index++] = atof(subtoken);
                subtoken = strtok(NULL, ",");
            }
        }

        token = strtok(NULL, "[]");
        count++;
    }
}
void battery_simulator_init(void)
{
    // init callback function
    userUartJsonCommandCallback(battery_message_callback);
    memset(&simulate_data, 0, sizeof(simulate_message_t));
}

/**
 *  "{\"data\":[123][1.2][80.1][3.2,3.3,3.4][12.3,11.8,13.2,12.0,11.5,12.6,11.9,12.8,13.1,11.7,12.2,12.4]}";
 *
 * @brief call function to in loop function
 *
 */
void battery_simulator_process(void)
{
    if (battery_message_event == 1)
    {
        // LOGI("process call = %d\n", battery_message_event);
        // parseJsonMessage(json_buffer_message, &data, json_buffer_message_length);
        parseNonJsonMessage(json_buffer_message, &simulate_data, json_buffer_message_length);
        parserSubToken(buffer_sub_token_1,&simulate_data);
        simulate_data.pack_voltage = 0;
        memset(buffer_sub_token_1, 0x00, sizeof(buffer_sub_token_1));
        // Access the parsed data as needed
        LOGI("Battery ID: %d\n", simulate_data.battery_id);
        LOGI("Current: %f\n", simulate_data.current);
        LOGI("State of Charge (SOC): %2.2f\n", simulate_data.soc);
        for (size_t i = 0; i < 3; i++)
        {
            LOGI("temperature [%d] = %2.2f", i, simulate_data.temperature[i]);
        }
        for (size_t i = 0; i < 15; i++)
        {
            simulate_data.pack_voltage = simulate_data.pack_voltage + simulate_data.cell_voltage[i];
            LOGI("cell [%d] = %2.2f", i, simulate_data.cell_voltage[i]);
        }
        // freen memory
        memset(json_buffer_message, 0, sizeof(json_buffer_message));
        json_buffer_message_length = 0;
        battery_message_event = 0;
        // LOGI("process done\n");
    }
}
/**
 * @brief
 *
 * @param message
 * @param len
 */
static void battery_message_callback(uint8_t *message, uint16_t len)
{
    LOGI("test %d, len = %d", battery_message_event, len);
    if (battery_message_event == 0)
    {
        for (size_t i = 0; i < len - 2; i++)
        {
            json_buffer_message[i] = message[i];
        }
        json_buffer_message_length = len - 2;
        battery_message_event = 1;
    }
}
/***********************************************************************************************************************
 * static functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/
