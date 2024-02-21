/**
 * @file ui_local_screen.c
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
#include "common.h"
#include "ui_common.h"
#include "ui_cells_screen.h"
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
#define NUMBER_BUTTON_UI_CELLS (2)
/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/
static void cell_screen_update(Battery_info_t info);
static void cell_screen_check_alert_error(Battery_info_t info);
/***********************************************************************************************************************
 * Exported global variables and functions (to be accessed by other files)
 ***********************************************************************************************************************/
const char ui_cells_screen_table[2][12] = {"Local_Back", // back to the total screen
                                           "Local_bt1"}; // goto popup alert

// this is id of each battery in the local battery monitor
const char local_battery_monitor_common[8][15] = {"localID",     // 0
                                                  "localAlert",  // 1
                                                  "localAmp",    // 2
                                                  "localVolt",   // 3
                                                  "localSoc",    // 4
                                                  "localTemp1",  // 5
                                                  "localTemp2",  // 6
                                                  "localTemp3"}; // 7

const char local_battery_monitor_cells[15][15] = {"localCell1",
                                                  "localCell2",
                                                  "localCell3",
                                                  "localCell4",
                                                  "localCell5",
                                                  "localCell6",
                                                  "localCell7",
                                                  "localCell8",
                                                  "localCell9",
                                                  "localCell10",
                                                  "localCell11",
                                                  "localCell12",
                                                  "localCell13",
                                                  "localCell14",
                                                  "localCell15"};

const char local_battery_monitor_battery[3][10] = {"b5",
                                                   "b6",
                                                   "b7"};
/***********************************************************************************************************************
 * Imported global variables and functions (from other files)
 ***********************************************************************************************************************/

/**
 * @brief
 *
 * @param battery_selected
 */
void ui_cell_screen_update(uint8_t battery_selected)
{
    cell_screen_update(battery_info[battery_selected]);

    cell_screen_check_alert_error(battery_info[battery_selected]);
}

/**
 * @brief
 *
 * @param button
 * @param len
 */
void ui_cell_screen_check_button_table(const char *button, uint16_t len)
{
    LOGI("check button %s", button);
    for (size_t i = 0; i < NUMBER_BUTTON_UI_CELLS; i++)
    {
        if (strcmp(button, ui_cells_screen_table[i]) == 0)
        {
            LOGI("button pressed: %s", ui_cells_screen_table[i]);
            // go to the new page
            if (i == 0)
                set_pageID(E_UI_SCREEN_TOTAL);
            else if (i == 1)
                set_pageID(E_UI_SCREEN_ALERTS);
            return;
        }
    }
}

/**
 * @brief update battery status of each battery in the local battery monitor
 *
 * @param battery_id
 */
static void cell_screen_update(Battery_info_t info)
{
    uint8_t label[BATTERY_ID_MAX_LENGTH];
    // update common parameter
    // snprintf(label, BATTERY_ID_MAX_LENGTH, "BATTERY ID: %d", info.id);
    snprintf(label, BATTERY_ID_MAX_LENGTH, "BATTERY ID: %d", info.id);
    nextion_setText(label, local_battery_monitor_common[0]); // update battery id label

    memset(label, 0, sizeof(label));
    snprintf(label, BATTERY_ID_MAX_LENGTH, "%0.2fA", info.current / 100.0);
    nextion_setText(label, local_battery_monitor_common[2]); // update battery id label

    memset(label, 0, sizeof(label));
    snprintf(label, BATTERY_ID_MAX_LENGTH, "%0.2fV", info.pack_volt / 100.0);
    nextion_setText(label, local_battery_monitor_common[3]); //

    memset(label, 0, sizeof(label));
    snprintf(label, BATTERY_ID_MAX_LENGTH, "%0.1f%%", info.pack_soc / 100.0);
    nextion_setText(label, local_battery_monitor_common[4]); //

    memset(label, 0, sizeof(label));
    snprintf(label, BATTERY_ID_MAX_LENGTH, "%0.1f°C", info.temperature[0] / 100.0);
    nextion_setText(label, local_battery_monitor_common[5]); //

    memset(label, 0, sizeof(label));
    snprintf(label, BATTERY_ID_MAX_LENGTH, "%0.1f°C", info.temperature[1] / 100.0);
    nextion_setText(label, local_battery_monitor_common[6]); //

    memset(label, 0, sizeof(label));
    snprintf(label, BATTERY_ID_MAX_LENGTH, "%0.1f°C", info.temperature[2] / 100.0);
    nextion_setText(label, local_battery_monitor_common[7]); //

    // update cells voltage
    for (size_t i = 0; i < BATTERY_CELLS; i++)
    {
        memset(label, 0, sizeof(label));
        snprintf(label, BATTERY_ID_MAX_LENGTH, "%0.2fV", info.cell[i].voltage / 100.0);
        nextion_setText(label, local_battery_monitor_cells[i]); //
    }
}

/**
 * @brief
 *
 * @param info
 */
static void cell_screen_check_alert_error(Battery_info_t info)
{
    uint8_t label[BATTERY_ID_MAX_LENGTH];
    // update common parameter
    if (info.alert.value != 0)
    {
        // display alert
        nextion_button_change_pic("bt0", 4);
    }
    else if (info.alarm.value != 0)
    {
        // display alarm
        if (info.alarm_error_message != NULL)
            snprintf(label, BATTERY_ID_MAX_LENGTH, "%s", info.alarm_error_message);
        else
            snprintf(label, BATTERY_ID_MAX_LENGTH, "%s", "NO FAULTS");
        nextion_setText(label, local_battery_monitor_common[1]); // localAlert
        nextion_button_change_pic("bt0", 5);
        // display cell status
        for (size_t index = 0; index < BATTERY_CELLS; index++)
        {
            if (info.cell[index].alarm.value != 0)
                nextion_button_change_pic(local_battery_monitor_cells[index], 50);
            else
                nextion_button_change_pic(local_battery_monitor_cells[index], 3);
        }
        // display temperature status
        if ((info.alarm.bits.bit_4 == 1) ||
            (info.alarm.bits.bit_5 == 1) ||
            (info.alarm.bits.bit_6 == 1) ||
            (info.alarm.bits.bit_7 == 1))
        {
            nextion_button_change_pic(local_battery_monitor_battery[0], 55);
            nextion_button_change_pic(local_battery_monitor_battery[1], 53);
            nextion_button_change_pic(local_battery_monitor_battery[2], 51);
        }
        else
        {
            nextion_button_change_pic(local_battery_monitor_battery[0], 8);
            nextion_button_change_pic(local_battery_monitor_battery[1], 8);
            nextion_button_change_pic(local_battery_monitor_battery[2], 8);
        }
    }
    else
    {
        snprintf(label, BATTERY_ID_MAX_LENGTH, "%s", "NO FAULTS");
        nextion_setText(label, local_battery_monitor_common[1]); // localAlert
        nextion_button_change_pic("bt0", 12);
        // normal
    }
}
/***********************************************************************************************************************
 * static functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/