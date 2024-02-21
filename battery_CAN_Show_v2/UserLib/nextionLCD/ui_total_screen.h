/**
 * @file ui_local_screen.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef UI_LOCAL_SCREEN_
#define UI_LOCAL_SCREEN_

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
void ui_total_screen_check_button_table(const char *message, uint16_t len);

/**
 * @brief update component state of total screen
 * 
 */
void ui_total_screen_update(void);
#endif /* UI_LOCAL_SCREEN_ */