/**
 * @file sw_id.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-09-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef SW_ID_H
#define SW_ID_H

#include <stdint.h>
typedef enum
{
    E_INVALID = 0,
    E_SW_ID_01,
    E_SW_ID_02,
    E_SW_ID_03,
    E_SW_ID_04,

    E_SW_ID_05,
    E_SW_ID_06,
    E_SW_ID_07,
    E_SW_ID_08,

    E_SW_ID_09,
    E_SW_ID_10,
    E_SW_ID_11,
    E_SW_ID_12,

    E_SW_ID_13,
    E_SW_ID_14,
    E_SW_ID_15,
    E_SW_ID_16,
    E_SW_ID_17,
    E_SW_ID_18,
    E_SW_ID_19,
    E_SW_ID_20,
    E_SW_ID_21,
    E_SW_ID_22,
    E_SW_ID_23,
    E_SW_ID_24,
} e_switch_position;
uint16_t sw_id_detect(void);

#endif /* SW_ID_H */
