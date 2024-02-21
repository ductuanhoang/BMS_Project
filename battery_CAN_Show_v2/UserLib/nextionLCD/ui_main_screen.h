/**
 * @file ui_main_screen.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-12-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef UI_MAIN_SCREEN_H
#define UI_MAIN_SCREEN_H

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
void ui_main_screen_check_button_table(const char *message, uint16_t len);

/**
 * @brief update component state of total screen
 *
 */
void ui_main_screen_update(void);

#endif /* UI_MAIN_SCREEN_H */