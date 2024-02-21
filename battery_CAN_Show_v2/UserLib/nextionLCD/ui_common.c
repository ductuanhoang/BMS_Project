/**
 * @file ui_common.c
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
#include "ui_common.h"
#include "common.h"
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables and functions (to be accessed by other files)
 ***********************************************************************************************************************/
ui_battery_info_t ui_battery_info[12];
nextion_ui_t nextion_ui;
static e_ui_screen_type oldPageId;
/***********************************************************************************************************************
 * Imported global variables and functions (from other files)
 ***********************************************************************************************************************/
void ui_common_init(void)
{
    nextion_init();
    nextion_ui.pageId = E_UI_SCREEN_STARTUP;
    oldPageId = nextion_ui.pageId;
    set_pageID(E_UI_SCREEN_MAIN);
    nextion_ui.battery_selected = 0;
}

void set_battery_selected(uint8_t selected)
{
    nextion_ui.battery_selected = selected;
}

uint8_t get_battery_selected(void)
{
    return (nextion_ui.battery_selected);
}

/**
 * @brief
 *
 * @return uint8_t
 */
uint8_t event_change_pageID(void)
{
    if (oldPageId != nextion_ui.pageId)
    {
        oldPageId = nextion_ui.pageId;
        return 1;
    }

    return 0;
}
void set_pageID(uint8_t pageId)
{
    nextion_ui.pageId = pageId;
}

e_ui_screen_type get_pageID(void)
{
    return nextion_ui.pageId;
}

/***********************************************************************************************************************
 * static functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/