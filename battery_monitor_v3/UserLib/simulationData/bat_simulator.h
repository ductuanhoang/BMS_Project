/**
 * @file bat_simulator.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-01-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef BATTERY_SIMULATION_H_
#define BATTERY_SIMULATION_H_

/**
 * @brief 
 * 
 */
typedef struct
{
    uint16_t battery_id;
    float cell_voltage[15];
    float current;
    float temperature[3];
    float soc;
    float pack_voltage;
} simulate_message_t;

extern simulate_message_t simulate_data;
/**
 * @brief 
 * 
 */
void battery_simulator_init(void);

/**
 * @brief call function to in loop function
 *
 */
void battery_simulator_process(void);
#endif // BATTERY_SIMULATION_H_
