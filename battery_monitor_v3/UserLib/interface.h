/**
 * @file interface.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-08-04
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef INTERFACE_H
#define INTERFACE_H

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "main.h"
#include <stdint.h>
#include <stdbool.h>

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/


/**
 * Initialization of bq769x0 IC
 *
 * - Determines I2C address
 * - Sets ALERT pin interrupt
 */
int bq769x0_init();

/**
 * Writes one byte to bq769x0 IC
 */
void bq769x0_write_byte(I2C_HandleTypeDef *hi2c, uint8_t reg_addr, uint8_t data);

/**
 * Read 1 byte from bq769x0 IC
 */
uint8_t bq769x0_read_byte(I2C_HandleTypeDef *hi2c, uint8_t reg_addr);

/**
 * Read 16-bit word (two bytes) from bq769x0 IC
 *
 * @returns the (unsigned) word or -1 in case of CRC error
 */
int32_t bq769x0_read_word(I2C_HandleTypeDef *hi2c, uint8_t reg_addr);

/**
 * \returns status of the alert pin
 */
bool bq769x0_alert_flag();

/**
 * Reset alert pin interrupt flag
 */
void bq769x0_alert_flag_reset();

/**
 * Returns the time when the interrupt was triggered
 */
// time_t bq769x0_alert_timestamp();

#endif /* INTERFACE_H */