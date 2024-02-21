/**
 * @file ui_process.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-12-05
 *
 * @copyright Copyright (c) 2023
 *
 */
/***********************************************************************************************************************
 * Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes <System Includes>
 ***********************************************************************************************************************/
#include "ui_process.h"
#include "ui_common.h"
#include "common.h"

// ui screen specific
#include "ui_main_screen.h"
#include "ui_total_screen.h"
#include "ui_cells_screen.h"
#include "ui_settings_screen.h"
#include "ui_settings_screen_2.h"
#include "ui_cells_alert_screen.h"
#include "ui_popup_screen.h"
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
#define MAX_UI_SCREEN_SUPPORTED (10)
/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/
const char ui_name[MAX_UI_SCREEN_SUPPORTED][17] = {
    "ui_main",
    "ui_total",
    "ui_cell",
    "ui_report",
    "ui_settings",
    "ui_settings_2",
    "ui_admin",
    "ui_settings_3",
    "popup_warning",
    "popup_error",
};
/***********************************************************************************************************************
 * Exported global variables and functions (to be accessed by other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Imported global variables and functions (from other files)
 ***********************************************************************************************************************/

void ui_process_updatePage(void)
{
    // check change page action
    if (event_change_pageID() == 1)
    {
        // change page action
        LOGI("goto page %s", ui_name[get_pageID()]);
        nextion_setPage(ui_name[get_pageID()]);
    }
}
/**
 * @brief
 *
 */
void nextion_update(void)
{
    // update page date
    switch (get_pageID())
    {
    case E_UI_SCREEN_MAIN:
        ui_main_screen_update();
        break;
    case E_UI_SCREEN_TOTAL:
        ui_total_screen_update();
        break;
    case E_UI_SCREEN_CELLS:
        ui_cell_screen_update(get_battery_selected());
        break;
    case E_UI_SCREEN_ALERTS:
        ui_cell_alert_screen_update(get_battery_selected());
        break;
    case E_UI_SCREEN_SETTINGS:
        ui_setting_screen_update();
        break;
    case E_UI_SCREEN_SETTINGS_2:
        ui_setting_screen_2_update();
        break;
    case E_UI_SCREEN_SETTINGS_3:
        break;
    case E_UI_SCREEN_POP_UP_WARNING:
        ui_popup_warning_screen_update();
        break;
    case E_UI_SCREEN_POP_UP_ERROR:
        ui_popup_error_screen_update();
        break;
    default:
        break;
    }
    // check popup screen action
}

/**
 * @brief
 *
 */
void ui_get_data(void)
{
    ui_setting_screen_get_data();
}
/**
 * @brief call in loop function
 *
 */
void ui_progress_event_handler(void)
{
    nextion_process_event();
}
/**
 * @brief process all button press events
 *
 * @param message
 * @param len
 */
void nextion_process_button(uint8_t *message, uint16_t len)
{
    ui_cell_screen_check_button_table(message, len);
    ui_total_screen_check_button_table(message, len);
    ui_main_screen_check_button_table(message, len);
    ui_setting_screen_check_button_table(message, len);
    ui_setting_screen_2_check_button_table(message, len);
    ui_popup_screen_check_button_table(message, len);
    ui_cell_alert_screen_check_button_table(message, len);
    // switch (get_pageID())
    // {
    // case E_UI_SCREEN_MAIN:
    //     ui_main_screen_check_button_table(message, len);
    //     break;
    // case E_UI_SCREEN_TOTAL:
    //     ui_total_screen_check_button_table(message, len);
    //     break;
    // case E_UI_SCREEN_CELLS:
    //     ui_cell_screen_check_button_table(message, len);
    //     break;
    // case E_UI_SCREEN_SETTINGS:
    //     break;
    // case E_UI_SCREEN_POP_UP_WARNING:
    //     break;
    // case E_UI_SCREEN_POP_UP_ERROR:
    //     break;
    // default:
    //     break;
    // }
}

/***********************************************************************************************************************
 * static functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/