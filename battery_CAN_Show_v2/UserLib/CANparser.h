/**
 * @file CANparser.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-10-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef CAN_PARSER_H
#define CAN_PARSER_H
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>     /* abs */
#include <stdint.h>
#include <stdbool.h>
#include "common.h"

#define CAN_ID_HEADER_MIN (01U)
#define CAN_ID_HEADER_MAX (12U)

/**
 * @brief 
 * 
 * @param can_id 
 * @param message 
 */
void CANParser_message(uint16_t can_id, uint8_t *message);

/**
 * @brief 
 * 
 * @param index 
 * @param info 
 * @param sub_index 
 */
void CANParser_setBatteryInfo(uint8_t index, uint8_t *info, uint8_t sub_index);

/**
 * @brief 
 * 
 */
void CANParser_showBatteryInfo(void);

/**
 * @brief 
 * 
 * @param time_scale 
 */
void CANParser_checkBatteryStatus(uint16_t time_scale);
#endif // CAN_PARSER_H
