/**
 * @file battery_manager.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef BATTERY_MANAGER_H
#define BATTERY_MANAGER_H

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/


/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/


/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/

void battery_manager_init(void);

/**
 * @brief 
 * 
 */
void battery_time_life(void);

void battery_total_getValue(void);

/**
 * @brief check battery status alarm flag
 * call in loop check
 * 
 */
void battery_warrning_check(void);

/**
 * @brief check battery status protect flag
 * call in loop check
 * 
 */
void battery_error_check(void);

/**
 * @brief
 *
 */
void battery_manager_set_error_name(void);


/**
 * @brief
 *
 * @return uint16_t
 */
uint16_t battery_manager_get_max_alarms();


/**
 * @brief
 *
 * @return uint16_t
 */
uint16_t battery_manager_get_max_error();

#endif /* BATTERY_MANAGER_H */