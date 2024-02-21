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
#include "CANparser.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/
#define BATTERY_TIME_OUT_MS 3
#define CURRENT_OFFSET (300)     // Current offset is 3000A
#define TEMPERATURE_OFFSET (273) // 273 degree
#define CAN_HEADER_PREFIX 0x5A
/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/
static void CANParser_clearBatteryInfo(uint8_t index);
/***********************************************************************************************************************
 * Exported global variables and functions (to be accessed by other files)
 ***********************************************************************************************************************/
// Battery_info_t battery_info[BATTERY_MAX_PACK + 1];
/***********************************************************************************************************************
 * Imported global variables and functions (from other files)
 ***********************************************************************************************************************/

/**
 * @brief
 *
 * @param can_id
 * @param message
 */
void CANParser_message(uint16_t can_id, uint8_t *message)
{
    uint8_t can_id_header = 0;
    uint8_t can_id_sub = 0;
    can_id_header = can_id >> 4;
    can_id_sub = can_id & 0x00F;
    if ((can_id_header >= CAN_ID_HEADER_MIN) && (can_id_header <= CAN_ID_HEADER_MAX) && (can_id_sub >= 0x00) && (can_id_sub <= 0x0F))
    {
        CANParser_setBatteryInfo(can_id_header, message, can_id_sub);
        battery_info[can_id_header].time_counting_down = BATTERY_TIME_OUT_MS;
        battery_info[can_id_header].id = can_id_header;
        // LOGI("battery_info[index] = %2.2f", (message[6] << 8 | message[7]) / 100.0); // cell voltage 1
    }
}

void CANParser_checkBatteryStatus(uint16_t time_scale)
{
    for (size_t index = 0; index <= BATTERY_MAX_PACK; index++)
    {
        if (battery_info[index].time_counting_down == 0)
        {
            // clear batter info
            CANParser_clearBatteryInfo(index);
            battery_info[index].active = 0;
        }
        else
        {
            battery_info[index].active = 1;
            battery_info[index].time_counting_down = battery_info[index].time_counting_down - 1;
        }
    }
}
/**
 * @brief
 *
 * @param index
 * @param info
 * @param sub_index
 */
void CANParser_setBatteryInfo(uint8_t index, uint8_t *info, uint8_t sub_index)
{
    // set voltage cells
    // LOGI("battery_info[index] = %2.2f", (info[0] << 8 | info[1]) / 100.0); // cell voltage 1

    switch (sub_index)
    {
    case 0x0A:
        battery_info[index].cell[0].voltage = info[0] << 8 | info[1]; // cell voltage 1
        battery_info[index].cell[1].voltage = info[2] << 8 | info[3]; // cell voltage 2
        battery_info[index].cell[2].voltage = info[4] << 8 | info[5]; // cell voltage 3
        battery_info[index].cell[3].voltage = info[6] << 8 | info[7]; // cell voltage 4
        break;
    case 0x0B:
        battery_info[index].cell[4].voltage = info[0] << 8 | info[1]; // cell voltage 5
        battery_info[index].cell[5].voltage = info[2] << 8 | info[3]; // cell voltage 6
        battery_info[index].cell[6].voltage = info[4] << 8 | info[5]; // cell voltage 7
        battery_info[index].cell[7].voltage = info[6] << 8 | info[7]; // cell voltage 8
        break;
    case 0x0C:
        battery_info[index].cell[8].voltage = info[0] << 8 | info[1];  // cell voltage 9
        battery_info[index].cell[9].voltage = info[2] << 8 | info[3];  // cell voltage 10
        battery_info[index].cell[10].voltage = info[4] << 8 | info[5]; // cell voltage 11
        battery_info[index].cell[11].voltage = info[6] << 8 | info[7]; // cell voltage 12
        break;
    case 0x0D:
        battery_info[index].cell[12].voltage = info[0] << 8 | info[1];                    // cell voltage 13
        battery_info[index].cell[13].voltage = info[2] << 8 | info[3];                    // cell voltage 14
        battery_info[index].cell[14].voltage = info[4] << 8 | info[5];                    // cell voltage 15
        battery_info[index].current = (info[6] << 8 | info[7]) - CURRENT_OFFSET * 100; // current pack
        break;
    case 0x0E:
        battery_info[index].temperature[0] = (info[0] << 8 | info[1]) - TEMPERATURE_OFFSET * 100; //
        battery_info[index].temperature[1] = (info[2] << 8 | info[3]) - TEMPERATURE_OFFSET * 100;
        battery_info[index].temperature[2] = (info[4] << 8 | info[5]) - TEMPERATURE_OFFSET * 100;
        battery_info[index].pack_volt = info[6] << 8 | info[7];
        break;
    case 0x0F:
        battery_info[index].pack_soc = info[0] << 8 | info[1]; //
        battery_info[index].time = info[2] << 8 | info[3];
        break;
    default:
        break;
    }
}

/**
 * @brief
 *
 * @param index
 */
void CANParser_showBatteryInfo(void)
{
    for (size_t index = 0; index <= CAN_ID_HEADER_MAX; index++)
    {
        if (battery_info[index].active == true)
        {
            /* code */
            LOGI("---------------- info of :%d ----------------", index);
            for (uint8_t i = 0; i < BATTERY_CELLS; i++)
            {
                LOGI("cell %d = %2.2f", i, battery_info[index].cell[i].voltage / 100.0);
            }
            for (uint8_t i = 0; i < BATTERY_TEMP_SENSORS; i++)
            {
                LOGI("temp %d = %2.2f", i, battery_info[index].temperature[i] / 100.0);
            }
            LOGI("current pack = %2.2f", battery_info[index].current / 100.0);
            LOGI("voltage pack = %2.2f", battery_info[index].pack_volt / 100.0);
            LOGI("soc pack = %d ", battery_info[index].pack_soc);

            LOGI("-------------------- end --------------------");
        }
    }
}
/***********************************************************************************************************************
 * static functions
 ***********************************************************************************************************************/
static void CANParser_clearBatteryInfo(uint8_t index)
{
    for (uint8_t i = 0; i < BATTERY_CELLS; i++)
    {
        battery_info[index].cell[i].voltage = 0;
    }
    for (uint8_t i = 0; i < BATTERY_TEMP_SENSORS; i++)
    {
        battery_info[index].temperature[i] = 0;
    }
    battery_info[index].current = 0;
    battery_info[index].pack_volt = 0;
    battery_info[index].pack_soc = 0;
}
/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/