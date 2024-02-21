/**
 * @file bms_can_trans.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-09-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef BMS_CAN_TRANS_H_
#define BMS_CAN_TRANS_H_
#include "common.h"
#include "bms.h"
#include "bat_simulator.h"
/**
 * @brief 
 * 
 * @param bms 
 */
void bms_can_packet_message(Bms *bms, uint8_t board_id, uint64_t timestamp);

/**
 * @brief 
 * 
 * @param simulate_data 
 */
void bms_can_packet_simulation_message(simulate_message_t *simulate_data);
#endif // BMS_CAN_TRANS_H_
