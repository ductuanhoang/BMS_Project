/**
 * @file ui_settings_screen.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef UI_SETTINGS_SCREEN_H
#define UI_SETTINGS_SCREEN_H

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <stdint.h>

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
 * @brief
 *
 * @param message
 * @param len
 */
void ui_setting_screen_check_button_table(const char *message, uint16_t len);

/**
 * @brief update component state of total screen
 *
 */
void ui_setting_screen_update(void);

#endif /* UI_SETTINGS_SCREEN_H */