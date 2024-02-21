/**
 * @file sw_id.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-09-19
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
#include "bms_board_config.h"
#include "interface.h"
#include "registers.h"
#include "common.h"
#include "bms.h"
#include <string.h>

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

typedef union
{
    uint8_t data[2];
    struct
    {
        uint8_t bit_0 : 1;
        uint8_t bit_1 : 1;
        uint8_t bit_2 : 1;
        uint8_t bit_3 : 1;

        uint8_t bit_4 : 1;
        uint8_t bit_5 : 1;
        uint8_t bit_6 : 1;
        uint8_t bit_7 : 1;

        uint8_t bit_8 : 1;
        uint8_t bit_9 : 1;
        uint8_t bit_10 : 1;
        uint8_t bit_11 : 1;
        uint8_t un_use_1 : 2;
        uint8_t un_use_2 : 2;
    } bits;
} sw_id_t;

/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables and functions (to be accessed by other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Imported global variables and functions (from other files)
 ***********************************************************************************************************************/

uint16_t sw_id_detect(void)
{
    sw_id_t sw_id;
    uint16_t id_device = E_INVALID;
    uint8_t seg_1 = 0x00;
    uint8_t seg_2 = 0x00;
    uint8_t seg_3 = 0x00;

    sw_id.bits.bit_0 = HAL_GPIO_ReadPin(SW_1_GPIO_Port, SW_1_Pin);
    sw_id.bits.bit_1 = HAL_GPIO_ReadPin(SW_2_GPIO_Port, SW_2_Pin);
    sw_id.bits.bit_2 = HAL_GPIO_ReadPin(SW_3_GPIO_Port, SW_3_Pin);
    sw_id.bits.bit_3 = HAL_GPIO_ReadPin(SW_4_GPIO_Port, SW_4_Pin);

    sw_id.bits.bit_4 = HAL_GPIO_ReadPin(SW_5_GPIO_Port, SW_5_Pin);
    sw_id.bits.bit_5 = HAL_GPIO_ReadPin(SW_6_GPIO_Port, SW_6_Pin);
    sw_id.bits.bit_6 = HAL_GPIO_ReadPin(SW_7_GPIO_Port, SW_7_Pin);
    sw_id.bits.bit_7 = HAL_GPIO_ReadPin(SW_8_GPIO_Port, SW_8_Pin);

    sw_id.bits.bit_8 = HAL_GPIO_ReadPin(SW_9_GPIO_Port, SW_9_Pin);
    sw_id.bits.bit_9 = HAL_GPIO_ReadPin(SW_10_GPIO_Port, SW_10_Pin);
    sw_id.bits.bit_10 = HAL_GPIO_ReadPin(SW_11_GPIO_Port, SW_11_Pin);
    sw_id.bits.bit_11 = HAL_GPIO_ReadPin(SW_12_GPIO_Port, SW_12_Pin);

    sw_id.bits.un_use_1 = 0x00;
    sw_id.bits.un_use_2 = 0x00;

    seg_1 = sw_id.data[0] & 0x0F;
    seg_2 = sw_id.data[0] >> 4;

    seg_3 = sw_id.data[1] & 0x0F;

    if ((seg_1 != 0) & (seg_2 == 0x00) & (seg_3 == 0x00))
    {
        if ((seg_1 == 0x01))
            id_device = E_SW_ID_01;
        else if ((seg_1 == 0x03))
            id_device = E_SW_ID_02;
        else if ((seg_1 == 0x07))
            id_device = E_SW_ID_03;
        else if ((seg_1 == 0x0F))
            id_device = E_SW_ID_04;
    }

    if ((seg_1 == 0x0F) && (seg_2 != 0x00) && (seg_3 == 0x00))
    {
        if ((seg_2 == 0x01))
            id_device = E_SW_ID_05;
        else if ((seg_2 == 0x03))
            id_device = E_SW_ID_06;
        else if ((seg_2 == 0x07))
            id_device = E_SW_ID_07;
        else if ((seg_2 == 0x0F))
            id_device = E_SW_ID_08;
    }

    if ((seg_1 == 0x0F) && (seg_2 == 0x0F) && (seg_3 != 0x00))
    {
        if ((seg_3 == 0x01))
            id_device = E_SW_ID_09;
        else if ((seg_3 == 0x03))
            id_device = E_SW_ID_10;
        else if ((seg_3 == 0x07))
            id_device = E_SW_ID_11;
        else if ((seg_3 == 0x0F))
            id_device = E_SW_ID_12;
    }

    if ((seg_1 != 0x0F) & (seg_2 == 0x0F) & (seg_3 == 0x0F))
    {
        if ((seg_1 == 0x0E))
            id_device = E_SW_ID_13;
        else if ((seg_1 == 0x0C))
            id_device = E_SW_ID_14;
        else if ((seg_1 == 0x08))
            id_device = E_SW_ID_15;
        else if ((seg_1 == 0x00))
            id_device = E_SW_ID_16;
    }

    if ((seg_1 == 0x00) & (seg_2 != 0x0F) & (seg_3 == 0x0F))
    {
        if ((seg_2 == 0x0E))
            id_device = E_SW_ID_17;
        else if ((seg_2 == 0x0C))
            id_device = E_SW_ID_18;
        else if ((seg_2 == 0x08))
            id_device = E_SW_ID_19;
        else if ((seg_2 == 0x00))
            id_device = E_SW_ID_20;
    }

    if ((seg_1 == 0x00) & (seg_2 == 0x00) & (seg_3 != 0x0F))
    {
        if ((seg_3 == 0x0E))
            id_device = E_SW_ID_21;
        else if ((seg_3 == 0x0C))
            id_device = E_SW_ID_22;
        else if ((seg_3 == 0x08))
            id_device = E_SW_ID_23;
        else if ((seg_3 == 0x00))
            id_device = E_SW_ID_24;
    }
    return id_device;
}

/***********************************************************************************************************************
 * static functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/