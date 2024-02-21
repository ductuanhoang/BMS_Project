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
#include "interface.h"
#include "registers.h"
#include "common.h"
#include "bms.h"
#include <string.h>
#include "bat_simulator.h"

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
#define CURRENT_OFFSET (300)     // Current offset is 300A
#define TEMPERATURE_OFFSET (273) // 273 degree
/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/
extern CAN_HandleTypeDef hcan;
CAN_TxHeaderTypeDef TxHeader; // declare a specific header for message transmittions
CAN_RxHeaderTypeDef RxHeader; // declare header for message reception

/***********************************************************************************************************************
 * Exported global variables and functions (to be accessed by other files)
 ***********************************************************************************************************************/

uint8_t TxData[8];
uint32_t TxMailbox;
/***********************************************************************************************************************
 * Imported global variables and functions (from other files)
 ***********************************************************************************************************************/
void bms_can_packet_message(Bms *bms, uint8_t board_id, uint64_t timestamp)
{
    uint8_t cell_id = 0;
    uint16_t cell_volt[15];
    uint16_t battery_current;
    uint16_t temperature[3];
    uint16_t pack_volt;
    uint16_t pack_soc;
    // convert data to uint16
    for (int i = 0; i < (sizeof(cell_volt) / sizeof(uint16_t)); i++)
    {
        cell_volt[i] = (uint16_t)((bms->status.cell_voltages[i]) * 100);
    }
    // bms->status.pack_current = -3.5;
    battery_current = (uint16_t)(bms->status.pack_current * 100 + CURRENT_OFFSET * 100);
    // bms->status.bat_temps[0] = -27.5;
    for (int i = 0; i < (sizeof(temperature) / sizeof(uint16_t)); i++)
    {
        temperature[i] = (uint16_t)(bms->status.bat_temps[i] * 100 + TEMPERATURE_OFFSET * 100);
    }
    pack_volt = (uint16_t)((bms->status.pack_voltage) * 100);

    pack_soc = (uint16_t)((bms->status.soc) * 100);
    // add TxData

    // report packet via CAN packet
    for (size_t i = 0; i < 6; i++)
    {
        cell_id = 0x0A + i;
        TxHeader.DLC = 8; // data length
        TxHeader.IDE = CAN_ID_STD;
        TxHeader.RTR = CAN_RTR_DATA;
        TxHeader.StdId = board_id << 4 | cell_id; // ID
        if (cell_id < 0x0E)
        {
            TxData[0] = cell_volt[i * 4 + 0] >> 8;
            TxData[1] = cell_volt[i * 4 + 0] & 0xFF;

            TxData[2] = cell_volt[i * 4 + 1] >> 8;
            TxData[3] = cell_volt[i * 4 + 1] & 0xFF;

            TxData[4] = cell_volt[i * 4 + 2] >> 8;
            TxData[5] = cell_volt[i * 4 + 2] & 0xFF;

            if (cell_id == 0x0D)
            {
                TxData[6] = battery_current >> 8;
                TxData[7] = battery_current & 0xFF;
            }
            else
            {
                TxData[6] = cell_volt[i * 4 + 3] >> 8;
                TxData[7] = cell_volt[i * 4 + 3] & 0xFF;
            }
        }
        else if (cell_id == 0x0F)
        {
            LOGI("config cell id %x", cell_id);
            TxData[0] = pack_soc >> 8;
            TxData[1] = pack_soc & 0xFF;

            TxData[2] = pack_soc >> 8;
            TxData[3] = pack_soc & 0xFF;
            TxData[4] = 0x00;
            TxData[5] = 0x00;
            TxData[6] = 0x00;
            TxData[7] = 0x00;
        }
        else if (cell_id == 0x0E)
        {
            LOGI("config cell id %x", cell_id);
            TxData[0] = temperature[0] >> 8;
            TxData[1] = temperature[0] & 0xFF;
            TxData[2] = temperature[1] >> 8;
            TxData[3] = temperature[1] & 0xFF;
            TxData[4] = temperature[2] >> 8;
            TxData[5] = temperature[2] & 0xFF;
            TxData[6] = pack_volt >> 8;
            TxData[7] = pack_volt & 0xFF;
        }
        // if (cell_id == 0x0E)
        // {
        //     LOGI("cell_volt[0] = %d\r\n", cell_volt[0]);
        //     LOGI("TxData[0] = %x\r\n", TxData[0]);
        //     LOGI("TxData[1] = %x\r\n", TxData[1]);
        //     LOGI("temperature[2] = %2.2f\r\n", temperature[2] / 100.0);
        //     LOGI("megers = %d\r\n", (TxData[0] << 8) | TxData[1]);
        // }
        LOGI("send id = %x\r\n", TxHeader.StdId);
        HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox);
    }
}

void bms_can_packet_simulation_message(simulate_message_t *simulate_data)
{
    uint8_t cell_id = 0;
    uint16_t cell_volt[15];
    uint16_t battery_current;
    uint16_t temperature[3];
    uint16_t pack_volt;
    uint16_t pack_soc;
    // convert data to uint16
    for (int i = 0; i < (sizeof(cell_volt) / sizeof(uint16_t)); i++)
    {
        cell_volt[i] = (uint16_t)((simulate_data->cell_voltage[i]) * 100);
    }
    // bms->status.pack_current = -3.5;
    battery_current = (uint16_t)(simulate_data->current * 100 + CURRENT_OFFSET * 100);
    // bms->status.bat_temps[0] = -27.5;
    for (int i = 0; i < (sizeof(temperature) / sizeof(uint16_t)); i++)
    {
        temperature[i] = (uint16_t)(simulate_data->temperature[i] * 100 + TEMPERATURE_OFFSET * 100);
    }
    pack_volt = (uint16_t)((simulate_data->pack_voltage) * 100);

    pack_soc = (uint16_t)((simulate_data->soc) * 100);
    // add TxData

    // report packet via CAN packet
    for (size_t i = 0; i < 6; i++)
    {
        cell_id = 0x0A + i;
        TxHeader.DLC = 8; // data length
        TxHeader.IDE = CAN_ID_STD;
        TxHeader.RTR = CAN_RTR_DATA;
        TxHeader.StdId = simulate_data->battery_id << 4 | cell_id; // ID
        if (cell_id < 0x0E)
        {
            TxData[0] = cell_volt[i * 4 + 0] >> 8;
            TxData[1] = cell_volt[i * 4 + 0] & 0xFF;

            TxData[2] = cell_volt[i * 4 + 1] >> 8;
            TxData[3] = cell_volt[i * 4 + 1] & 0xFF;

            TxData[4] = cell_volt[i * 4 + 2] >> 8;
            TxData[5] = cell_volt[i * 4 + 2] & 0xFF;

            if (cell_id == 0x0D)
            {
                TxData[6] = battery_current >> 8;
                TxData[7] = battery_current & 0xFF;
            }
            else
            {
                TxData[6] = cell_volt[i * 4 + 3] >> 8;
                TxData[7] = cell_volt[i * 4 + 3] & 0xFF;
            }
        }
        else if (cell_id == 0x0F)
        {
            LOGI("config cell id %x", cell_id);
            TxData[0] = pack_soc >> 8;
            TxData[1] = pack_soc & 0xFF;

            TxData[2] = pack_soc >> 8;
            TxData[3] = pack_soc & 0xFF;
            TxData[4] = 0x00;
            TxData[5] = 0x00;
            TxData[6] = 0x00;
            TxData[7] = 0x00;
        }
        else if (cell_id == 0x0E)
        {
            LOGI("config cell id %x", cell_id);
            TxData[0] = temperature[0] >> 8;
            TxData[1] = temperature[0] & 0xFF;
            TxData[2] = temperature[1] >> 8;
            TxData[3] = temperature[1] & 0xFF;
            TxData[4] = temperature[2] >> 8;
            TxData[5] = temperature[2] & 0xFF;
            TxData[6] = pack_volt >> 8;
            TxData[7] = pack_volt & 0xFF;
        }
        // if (cell_id == 0x0E)
        // {
        //     LOGI("cell_volt[0] = %d\r\n", cell_volt[0]);
        //     LOGI("TxData[0] = %x\r\n", TxData[0]);
        //     LOGI("TxData[1] = %x\r\n", TxData[1]);
        //     LOGI("temperature[2] = %2.2f\r\n", temperature[2] / 100.0);
        //     LOGI("megers = %d\r\n", (TxData[0] << 8) | TxData[1]);
        // }
        LOGI("send id simulate = %x -- simulate id  = %d\r\n", TxHeader.StdId, simulate_data->battery_id );
        HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox);
    }
}
/***********************************************************************************************************************
 * static functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/