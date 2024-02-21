/**
 * @file ui_settings_screen_2.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-22
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef UI_SETTINGS_SCREEN_2_H
#define UI_SETTINGS_SCREEN_2_H

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
void ui_setting_screen_2_check_button_table(const char *message, uint16_t len);

/**
 * @brief update component state of total screen
 *
 */
void ui_setting_screen_2_update(void);

/**
 * @brief call in loop to update
 *
 */
void ui_setting_screen_get_data(void);

#endif /* UI_SETTINGS_SCREEN_H */