/**
 * @file bms_board_config.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-08-04
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef BMS_BOARD_CONFIG_H_
#define BMS_BOARD_CONFIG_H_

#define BME_BOARD_ADDRESS 0x08

#define BOARD_NUM_CELLS_MAX 15
#define BOARD_NUM_THERMISTORS_MAX 3

#define BOARD_MAX_CURRENT 50
#define BOARD_SHUNT_RESISTOR 0.38 // 2.25//22

#define BOARD_BATTERY_CAPACITY 100 // 100 Ah

#endif // BMS_BOARD_CONFIG_H_
