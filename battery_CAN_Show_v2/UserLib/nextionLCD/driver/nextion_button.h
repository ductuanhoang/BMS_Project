/**
 * @file nextion_button.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef NEXTION_BUTTON_H
#define NEXTION_BUTTON_H

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "common.h"

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
 * @brief set picture of the button
 *
 * @param name
 * @param pic
 */
void nextion_button_change_pic(const char *name, const uint8_t pic);

/**
 * @brief set enable/disable touch button
 * 
 * @param name 
 * @param enabled 
 */
void nextion_button_enable(const char *name, bool enabled);

#endif /* NEXTION_BUTTON_H */