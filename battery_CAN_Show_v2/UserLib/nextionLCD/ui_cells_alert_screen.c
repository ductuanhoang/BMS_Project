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

static uint8_t ui_alert_protect_id = 0;
/***********************************************************************************************************************
 * Exported global variables and functions (to be accessed by other files)
 ***********************************************************************************************************************/
const char ui_cells_report_screen_table[2][14] = {"Report_Back",
                                                  "Report_Next"}; // goto popup alert
const char ui_cells_report_id[2][10] = {
    "localID",
    "header"}; // goto popup alert

const char header_content_table[2][15] = {"Alert Status",
                                          "Protect Status"};

// this is id of each battery in the local battery monitor
const char content_txt[8][15] = {"contentID1",  // 0
                                 "contentID2",  // 1
                                 "contentID3",  // 2
                                 "contentID4",  // 3
                                 "contentID5",  // 4
                                 "contentID6",  // 5
                                 "contentID7",  // 6
                                 "contentID8"}; // 7

// this is id of each battery in the local battery monitor
const char report_txt_id[8][15] = {"reportID1",  // 0
                                   "reportID2",  // 1
                                   "reportID3",  // 2
                                   "reportID4",  // 3
                                   "reportID5",  // 4
                                   "reportID6",  // 5
                                   "reportID7",  // 6
                                   "reportID8"}; // 7

const char content_alarm_txt[16][15] = {
    "A0",
    "A1",
    "A2",
    "A3",
    "A4",
    "A5",
    "A6",
    "A7",
    "A8",
    "A9",
    "A10",
    "A11",
    "A12",
    "A13",
    "A14",
    "A15",
};

const char content_protect_txt[16][15] = {
    "P0",
    "P1",
    "P2",
    "P3",
    "P4",
    "P5",
    "P6",
    "P7",
    "P8",
    "P9",
    "P10",
    "P11",
    "P12",
    "P13",
    "P14",
    "P15",
};

const char content_report_alarm[16][50] = {
    "Single Cell Low Voltage Alarm",
    "Single Cell High Voltage Alarm",
    "Discharge system Low Voltage Alarm",
    "Charge system High Voltage Alarm",
    "Charge Cell Low Temperature Alarm",
    "Charge Cell High Temperature Alarm",
    "Discharge Cell Low Temperature Alarm",
    "Discharge Cell High Temperature Alarm",
    "Charge over current alarm",
    "Discharge over current alarm",
    "Module low voltage alarm",
    "Module high voltage alarm",
    "", // Reserved
    "",
    "",
    "",
};

const char content_report_protect[16][50] = {
    "Single Cell Under Voltage Protect",
    "Single Cell Over Voltage Protect",
    "Discharge system Under Voltage Protect",
    "Charge system Over Voltage Protect",
    "Charge Cell Under Temperature Protect",
    "Charge Cell Over Temperature Protect",
    "Discharge Cell Under Temperature Protect",
    "Discharge Cell Over Temperature Protect",
    "Charge Over Current Protect",
    "Discharge Over Current Protect",
    "Module Under Voltage Protect",
    "Module Over Voltage Protect",
    "", // Reserved
    "",
    "",
    "",
};

/***********************************************************************************************************************
 * Imported global variables and functions (from other files)
 ***********************************************************************************************************************/

/**
 * @brief
 *
 * @param battery_selected
 */
void ui_cell_alert_screen_update(uint8_t battery_selected)
{
    cell_screen_update(battery_info[battery_selected]);
}

/**
 * @brief
 *
 * @param button
 * @param len
 */
void ui_cell_alert_screen_check_button_table(const char *button, uint16_t len)
{
    LOGI("check button %s", button);
    for (size_t i = 0; i < NUMBER_BUTTON_UI_CELLS; i++)
    {
        if (strcmp(button, ui_cells_report_screen_table[i]) == 0)
        {
            LOGI("button pressed: %s", ui_cells_report_screen_table[i]);
            // go to the new page
            if (i == 0) // Report_Back
            {
                if (ui_alert_protect_id == 0)
                    set_pageID(E_UI_SCREEN_CELLS);
                if (ui_alert_protect_id > 0)
                    ui_alert_protect_id--;
            }
            else if (i == 1) // Report_Next
            {
                ui_alert_protect_id++;
                if (ui_alert_protect_id >= 3)
                    ui_alert_protect_id = 3;
            }
            LOGI("ui_alert_protect_id: %d", ui_alert_protect_id);
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
    uint8_t label_report[100];
    // update common parameter
    // snprintf(label, BATTERY_ID_MAX_LENGTH, "BATTERY ID: %d", info.id);
    memset(label, 0, sizeof(label));
    snprintf(label, BATTERY_ID_MAX_LENGTH, "BATTERY ID: %d", info.id);
    nextion_setText(label, ui_cells_report_id[0]); // update battery id label

    // update header
    if (ui_alert_protect_id < 2)
    {
        memset(label, 0, sizeof(label));
        snprintf(label, BATTERY_ID_MAX_LENGTH, "%s", header_content_table[0]);
        nextion_setText(label, ui_cells_report_id[1]); // update header
    }
    else
    {
        memset(label, 0, sizeof(label));
        snprintf(label, BATTERY_ID_MAX_LENGTH, "%s", header_content_table[1]);
        nextion_setText(label, ui_cells_report_id[1]); // update header
    }

    // hiden button when id = 3
    if (ui_alert_protect_id == 3)
        nextion_setVisible(ui_cells_report_screen_table[1], false);
    else
        nextion_setVisible(ui_cells_report_screen_table[1], true);
    // update content table
    if (ui_alert_protect_id == 0)
    {
        for (size_t i = 0; i < 8; i++)
        {
            memset(label, 0, sizeof(label));
            snprintf(label, BATTERY_ID_MAX_LENGTH, "%s", content_alarm_txt[i]);
            nextion_setText(label, report_txt_id[i]); // update header

            memset(label, 0, sizeof(label));
            snprintf(label_report, 100, "%s", content_report_alarm[i]);
            nextion_setText(label_report, content_txt[i]);
        }
    }
    else if (ui_alert_protect_id == 1)
    {
        for (size_t i = 8; i < 16; i++)
        {
            memset(label, 0, sizeof(label));
            snprintf(label, BATTERY_ID_MAX_LENGTH, "%s", content_alarm_txt[i]);
            nextion_setText(label, report_txt_id[i - 8]); // update header

            memset(label, 0, sizeof(label));
            snprintf(label_report, 100, "%s", content_report_alarm[i]);
            nextion_setText(label_report, content_txt[i - 8]);
        }
    }
    else if (ui_alert_protect_id == 2)
    {
        for (size_t i = 0; i < 8; i++)
        {
            memset(label, 0, sizeof(label));
            snprintf(label, BATTERY_ID_MAX_LENGTH, "%s", content_protect_txt[i]);
            nextion_setText(label, report_txt_id[i]); // update header

            memset(label, 0, sizeof(label));
            snprintf(label_report, 100, "%s", content_report_protect[i]);
            nextion_setText(label_report, content_txt[i]);
        }
    }
    else if (ui_alert_protect_id == 3)
    {
        for (size_t i = 8; i < 16; i++)
        {
            memset(label, 0, sizeof(label));
            snprintf(label, BATTERY_ID_MAX_LENGTH, "%s", content_protect_txt[i]);
            nextion_setText(label, report_txt_id[i - 8]); // update header

            memset(label, 0, sizeof(label));
            snprintf(label_report, 100, "%s", content_report_protect[i]);
            nextion_setText(label_report, content_txt[i - 8]);
        }
    }
}

/***********************************************************************************************************************
 * static functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/