/**
 * @file ui_cells_alert_screen.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-22
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef UI_CELLS_ALERT_H
#define UI_CELLS_ALERT_H

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
 * @param battery_selected 
 */
void ui_cell_alert_screen_update(uint8_t battery_selected);

/**
 * @brief 
 * 
 * @param button 
 * @param len 
 */
void ui_cell_alert_screen_check_button_table(const char *button, uint16_t len);

#endif /* UI_CELLS_ALERT_H */