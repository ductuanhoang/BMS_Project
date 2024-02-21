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
#include "main.h"
#include "common.h"
#include "CANparser.h"
#include "CANsend.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/
#define ID_0x4210 (0x4210)
#define ID_0x4220 (0x4220)
#define ID_0x4230 (0x4230)
#define ID_0x4240 (0x4240)
#define ID_0x4250 (0x4250)
#define ID_0x4260 (0x4260)
#define ID_0x4270 (0x4270)
#define ID_0x4280 (0x4280)
#define ID_0x4290 (0x4290)
#define ID_0x7310 (0x7310)
#define ID_0x7320 (0x7320)
/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
extern CAN_TxHeaderTypeDef TxHeader;
extern CAN_HandleTypeDef hcan;
/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/
static uint8_t TxData[8];
static uint32_t TxMailbox;

/***********************************************************************************************************************
 * Exported global variables and functions (to be accessed by other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Imported global variables and functions (from other files)
 ***********************************************************************************************************************/

void CANsend_inverter_message(void)
{
    // ID_0x4210
    TxHeader.DLC = 8; // data length
    TxHeader.IDE = CAN_ID_EXT;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.ExtId = ID_0x4210;

    uint16_t total_voltage = 0;
    uint16_t total_current = 0;
    uint16_t temperature = 0;
    uint8_t soc = 0;
    for (size_t index = 0; index <= CAN_ID_HEADER_MAX; index++)
    {
        if (battery_info[index].active == true)
        {
            total_voltage += battery_info[index].pack_volt;
            for (uint8_t i = 0; i < BATTERY_CELLS; i++)
            {
                LOGI("cell %d = %2.2f", i, battery_info[index].cell[i].voltage / 100.0);
            }
            for (uint8_t i = 0; i < BATTERY_TEMP_SENSORS; i++)
            {
                LOGI("temp %d = %2.2f", i, battery_info[index].temperature[i] / 100.0);
            }
        }
        total_current = battery_info[1].current;
        temperature = battery_info[1].temperature[1]; // Avr temperature we have 24 packs and each pack has 3 temperatures sensor. For now we can use battery_info 1 temperature. Sensor 2
    }
    total_voltage = total_voltage / 10;
    total_current = total_current / 10 - 3000;
    TxData[1] = total_voltage >> 8;
    TxData[0] = total_voltage & 0xFF;

    TxData[3] = total_current >> 8;
    TxData[2] = total_current & 0xFF;

    temperature = temperature / 10 - 100;
    TxData[5] = temperature >> 8;
    TxData[4] = temperature & 0xFF;

    TxData[7] = soc;
    TxData[6] = 100;

    HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox);
    // ID_0x4220
    uint16_t Charge_Cutoff_Voltage = 0;
    uint16_t Disharge_Cutoff_Voltage = 0;
    uint16_t MAX_Charge_Current = 0;
    uint16_t MAX_Discharge_Current = 0;

    TxHeader.ExtId = ID_0x4220;
    TxData[1] = Charge_Cutoff_Voltage >> 8;
    TxData[0] = Charge_Cutoff_Voltage & 0xFF;

    TxData[3] = Disharge_Cutoff_Voltage >> 8;
    TxData[2] = Disharge_Cutoff_Voltage & 0xFF;

    TxData[5] = MAX_Charge_Current >> 8;
    TxData[4] = MAX_Charge_Current & 0xFF;

    TxData[7] = MAX_Discharge_Current >> 8;
    TxData[6] = MAX_Discharge_Current & 0xFF;

    HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox);

    // ID_0x4230
    uint16_t MAX_Single_Cell_Voltage = 0;
    uint16_t MIN_Single_Cell_Voltage = 0;
    uint16_t Highest_Cell_Voltage_Number = 0;
    uint16_t Lowest_Cell_Voltage_Number = 0;

    TxHeader.ExtId = ID_0x4230;
    TxData[1] = MAX_Single_Cell_Voltage >> 8;
    TxData[0] = MAX_Single_Cell_Voltage & 0xFF;

    TxData[3] = MIN_Single_Cell_Voltage >> 8;
    TxData[2] = MIN_Single_Cell_Voltage & 0xFF;

    TxData[5] = Highest_Cell_Voltage_Number >> 8;
    TxData[4] = Highest_Cell_Voltage_Number & 0xFF;

    TxData[7] = Lowest_Cell_Voltage_Number >> 8;
    TxData[6] = Lowest_Cell_Voltage_Number & 0xFF;
    HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox);

    // ID_0x4240
    uint16_t MAX_Single_Battery_Temperature = 0;
    uint16_t MIN_Single_Battery_Temperature = 0;
    uint16_t Highest_Battery_Temperature = 0;
    uint16_t Lowest_Battery_Temperature = 0;

    TxHeader.ExtId = ID_0x4240;

    TxData[1] = MAX_Single_Battery_Temperature >> 8;
    TxData[0] = MAX_Single_Battery_Temperature & 0xFF;

    TxData[3] = MIN_Single_Battery_Temperature >> 8;
    TxData[2] = MIN_Single_Battery_Temperature & 0xFF;

    TxData[5] = Highest_Battery_Temperature >> 8;
    TxData[7] = Highest_Battery_Temperature & 0xFF;

    TxData[7] = Lowest_Battery_Temperature >> 8;
    TxData[6] = Lowest_Battery_Temperature & 0xFF;
    HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox);
    // ID_0x7320
    TxHeader.ExtId = ID_0x7320;

    TxData[0] = 'M';
    TxData[1] = 'P';

    TxData[2] = 'S';
    TxData[3] = 0x00;

    TxData[4] = 'B';
    TxData[5] = 'A';

    TxData[6] = 'T';
    TxData[7] = 0x00;

    HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox);
}
/***********************************************************************************************************************
 * static functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/