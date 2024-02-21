/**
 * @file ui_total_screen.c
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
#include "ui_total_screen.h"
#include "ui_common.h"
#include "common.h"
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
#define NUMBER_BUTTON_UI_TOTAL (13)
#define NUMBER_BUTTON_CELLS_UI_TOTAL (12)
/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/
static void update_titles(void);
static void nextion_upload_total_screen(void);
static void update_ui_battery(Battery_info_t info, uint8_t ui_battery_info_id);
static void ui_total_screen_check_alert_error(Battery_info_t info);
/***********************************************************************************************************************
 * Exported global variables and functions (to be accessed by other files)
 ***********************************************************************************************************************/
// this is id of each battery in the common monitor
const char battery_ID_1[8][8] = {"ComID1", "ComVol1", "ComSoc1"};
const char battery_ID_2[8][8] = {"ComID2", "ComVol2", "ComSoc2"};
const char battery_ID_3[8][8] = {"ComID3", "ComVol3", "ComSoc3"};
const char battery_ID_4[8][8] = {"ComID4", "ComVol4", "ComSoc4"};

const char battery_ID_5[8][8] = {"ComID5", "ComVol5", "ComSoc5"};
const char battery_ID_6[8][8] = {"ComID6", "ComVol6", "ComSoc6"};
const char battery_ID_7[8][8] = {"ComID7", "ComVol7", "ComSoc7"};
const char battery_ID_8[8][8] = {"ComID8", "ComVol8", "ComSoc8"};

const char battery_ID_9[8][8] = {"ComID9", "ComVol9", "ComSoc9"};
const char battery_ID_10[8][9] = {"ComID10", "ComVol10", "ComSoc10"};
const char battery_ID_11[8][9] = {"ComID11", "ComVol11", "ComSoc11"};
const char battery_ID_12[8][9] = {"ComID12", "ComVol12", "ComSoc12"};

const char battery_title[1][12] = {"ComTotalBat"};

const char ui_total_button_table[NUMBER_BUTTON_UI_TOTAL][10] = {
    "TotalBack",
    "Com_bt1", // 1
    "Com_bt2",
    "Com_bt3",
    "Com_bt4",
    "Com_bt5",
    "Com_bt6",
    "Com_bt7",
    "Com_bt8",
    "Com_bt9",
    "Com_bt10",
    "Com_bt11",
    "Com_bt12"}; // 13
const char ui_total_button_images[NUMBER_BUTTON_CELLS_UI_TOTAL][10] = {
    "bt0",
    "bt1",
    "bt2",
    "bt3",
    "bt4",
    "bt5",
    "bt6",
    "bt7",
    "bt8",
    "bt9",
    "bt10",
    "bt11"};
/**
 * @brief
 *
 * @param message
 * @param len
 */
void ui_total_screen_check_button_table(const char *message, uint16_t len)
{
    LOGI("check button %s", message);
    for (size_t i = 0; i < NUMBER_BUTTON_UI_TOTAL; i++)
    {
        if (strcmp(message, ui_total_button_table[i]) == 0)
        {
            LOGI("button pressed: %s", ui_total_button_table[i]);
            // go to the new page
            if (i == 0)
                set_pageID(E_UI_SCREEN_MAIN);
            else
            {
                set_battery_selected(i);
                set_pageID(E_UI_SCREEN_CELLS);
            }
            return;
        }
    }
}

/**
 * @brief update component state of total screen
 *
 */
void ui_total_screen_update(void)
{
    update_titles();
    nextion_upload_total_screen();
}
/***********************************************************************************************************************
 * Imported global variables and functions (from other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * static functions
 ***********************************************************************************************************************/
static void update_titles(void)
{
    uint8_t label[BATTERY_ID_MAX_LENGTH];
    snprintf(label, BATTERY_ID_MAX_LENGTH, "%d/%d", totalBattery.totalPack, BATTERY_MAX_PACK);
    nextion_setText(label, battery_title[0]); // update battery id label
    if (totalBattery.totalPack == BATTERY_MAX_PACK)
        nextion_setColorText(battery_title[0], USER_COLOR_GREEN);
    else
        nextion_setColorText(battery_title[0], USER_COLOR_RED);
}

static void update_available_pack(uint8_t battery, bool enabled)
{
    switch (battery)
    {
    case 1:
        if (enabled == false)
        {
            nextion_button_change_pic("bt0", 14); // disable battery
            nextion_button_enable("bt0", false);  // enable battery
            nextion_setVisible(battery_ID_1[0], false);
            nextion_setVisible(battery_ID_1[1], false);
            nextion_setVisible(battery_ID_1[2], false);
        }
        else
        {
            // nextion_button_change_pic("bt0", 13);
            nextion_button_enable("bt0", true); // enable battery
            nextion_setVisible(battery_ID_1[0], true);
            nextion_setVisible(battery_ID_1[1], true);
            nextion_setVisible(battery_ID_1[2], true);
        }
        break;
    case 2:
        if (enabled == false)
        {
            nextion_setVisible(battery_ID_2[0], false);
            nextion_setVisible(battery_ID_2[1], false);
            nextion_setVisible(battery_ID_2[2], false);
            nextion_button_change_pic("bt1", 14); // disable battery
            nextion_button_enable("bt1", false);  // enable battery
        }
        else
        {
            nextion_setVisible(battery_ID_2[0], true);
            nextion_setVisible(battery_ID_2[1], true);
            nextion_setVisible(battery_ID_2[2], true);
            // nextion_button_change_pic("bt1", 13);
            nextion_button_enable("bt1", true); // enable battery
        }
        break;
    case 3:
        if (enabled == false)
        {
            nextion_setVisible(battery_ID_3[0], false);
            nextion_setVisible(battery_ID_3[1], false);
            nextion_setVisible(battery_ID_3[2], false);
            nextion_button_change_pic("bt2", 14); // disable battery
            nextion_button_enable("bt2", false);  // enable battery
        }
        else
        {
            nextion_setVisible(battery_ID_3[0], true);
            nextion_setVisible(battery_ID_3[1], true);
            nextion_setVisible(battery_ID_3[2], true);
            // nextion_button_change_pic("bt2", 13);
            nextion_button_enable("bt2", true); // enable battery
        }
        break;
    case 4:
        if (enabled == false)
        {
            nextion_setVisible(battery_ID_4[0], false);
            nextion_setVisible(battery_ID_4[1], false);
            nextion_setVisible(battery_ID_4[2], false);
            nextion_button_change_pic("bt3", 14); // disable battery
            nextion_button_enable("bt3", false);  // enable battery
        }
        else
        {
            nextion_setVisible(battery_ID_4[0], true);
            nextion_setVisible(battery_ID_4[1], true);
            nextion_setVisible(battery_ID_4[2], true);
            // nextion_button_change_pic("bt3", 13);
            nextion_button_enable("bt3", true); // enable battery
        }
        break;
    case 5:
        if (enabled == false)
        {
            nextion_setVisible(battery_ID_5[0], false);
            nextion_setVisible(battery_ID_5[1], false);
            nextion_setVisible(battery_ID_5[2], false);
            nextion_button_change_pic("bt4", 14); // disable battery
            nextion_button_enable("bt4", false);  // enable battery
        }
        else
        {
            nextion_setVisible(battery_ID_5[0], true);
            nextion_setVisible(battery_ID_5[1], true);
            nextion_setVisible(battery_ID_5[2], true);
            // nextion_button_change_pic("bt4", 13);
            nextion_button_enable("bt4", true); // enable battery
        }
        break;
    case 6:
        if (enabled == false)
        {
            nextion_setVisible(battery_ID_6[0], false);
            nextion_setVisible(battery_ID_6[1], false);
            nextion_setVisible(battery_ID_6[2], false);
            nextion_button_change_pic("bt5", 14); // disable battery
            nextion_button_enable("bt5", false);  // enable battery
        }
        else
        {
            nextion_setVisible(battery_ID_6[0], true);
            nextion_setVisible(battery_ID_6[1], true);
            nextion_setVisible(battery_ID_6[2], true);
            // nextion_button_change_pic("bt5", 13);
            nextion_button_enable("bt5", true); // enable battery
        }
        break;
    case 7:
        if (enabled == false)
        {
            nextion_setVisible(battery_ID_7[0], false);
            nextion_setVisible(battery_ID_7[1], false);
            nextion_setVisible(battery_ID_7[2], false);
            nextion_button_change_pic("bt6", 14); // disable battery
            nextion_button_enable("bt6", false);  // enable battery
        }
        else
        {
            nextion_setVisible(battery_ID_7[0], true);
            nextion_setVisible(battery_ID_7[1], true);
            nextion_setVisible(battery_ID_7[2], true);
            // nextion_button_change_pic("bt6", 13);
            nextion_button_enable("bt6", true); // enable battery
        }
        break;
    case 8:
        if (enabled == false)
        {
            nextion_setVisible(battery_ID_8[0], false);
            nextion_setVisible(battery_ID_8[1], false);
            nextion_setVisible(battery_ID_8[2], false);
            nextion_button_change_pic("bt7", 14); // disable battery
            nextion_button_enable("bt7", false);  // enable battery
        }
        else
        {
            nextion_setVisible(battery_ID_8[0], true);
            nextion_setVisible(battery_ID_8[1], true);
            nextion_setVisible(battery_ID_8[2], true);
            // nextion_button_change_pic("bt7", 13);
            nextion_button_enable("bt7", true); // enable battery
        }
        break;
    case 9:
        if (enabled == false)
        {
            nextion_setVisible(battery_ID_9[0], false);
            nextion_setVisible(battery_ID_9[1], false);
            nextion_setVisible(battery_ID_9[2], false);
            nextion_button_change_pic("bt8", 14); // disable battery
            nextion_button_enable("bt8", false);  // enable battery
        }
        else
        {
            nextion_setVisible(battery_ID_9[0], true);
            nextion_setVisible(battery_ID_9[1], true);
            nextion_setVisible(battery_ID_9[2], true);
            // nextion_button_change_pic("bt8", 13);
            nextion_button_enable("bt8", true); // enable battery
        }
        break;
    case 10:
        if (enabled == false)
        {
            nextion_setVisible(battery_ID_10[0], false);
            nextion_setVisible(battery_ID_10[1], false);
            nextion_setVisible(battery_ID_10[2], false);
            nextion_button_change_pic("bt9", 14); // disable battery
            nextion_button_enable("bt9", false);  // enable battery
        }
        else
        {
            nextion_setVisible(battery_ID_10[0], true);
            nextion_setVisible(battery_ID_10[1], true);
            nextion_setVisible(battery_ID_10[2], true);
            // nextion_button_change_pic("bt9", 13);
            nextion_button_enable("bt9", true); // enable battery
        }
        break;
    case 11:

        if (enabled == false)
        {
            nextion_setVisible(battery_ID_11[0], false);
            nextion_setVisible(battery_ID_11[1], false);
            nextion_setVisible(battery_ID_11[2], false);
            nextion_button_change_pic("bt10", 14); // disable battery
            nextion_button_enable("bt10", false);  // enable battery
        }
        else
        {
            nextion_setVisible(battery_ID_11[0], true);
            nextion_setVisible(battery_ID_11[1], true);
            nextion_setVisible(battery_ID_11[2], true);
            // nextion_button_change_pic("bt10", 13);
            nextion_button_enable("bt10", true); // enable battery
        }
        break;
    case 12:
        if (enabled == false)
        {
            nextion_setVisible(battery_ID_12[0], false);
            nextion_setVisible(battery_ID_12[1], false);
            nextion_setVisible(battery_ID_12[2], false);
            nextion_button_change_pic("bt11", 14); // disable battery
            nextion_button_enable("bt11", false);  // enable battery
        }
        else
        {
            nextion_setVisible(battery_ID_12[0], true);
            nextion_setVisible(battery_ID_12[1], true);
            nextion_setVisible(battery_ID_12[2], true);
            // nextion_button_change_pic("bt11", 13);
            nextion_button_enable("bt11", true); // enable battery
        }
        break;
    default:

        break;
    }
}

static void nextion_upload_total_screen(void)
{
    for (size_t i = 0; i <= BATTERY_MAX_PACK; i++)
    {
        if (battery_info[i].active)
        {
            update_available_pack(i, true);
            update_ui_battery(battery_info[i], i);
            ui_total_screen_check_alert_error(battery_info[i]);
        }
        else
        {
            update_available_pack(i, false);
        }
    }
}

/**
 * @brief
 *
 * @param info
 * @param ui_battery_info_id
 */
static void update_ui_battery(Battery_info_t info, uint8_t ui_battery_info_id)
{
    uint8_t label_id[BATTERY_ID_MAX_LENGTH];
    uint8_t label_voltage[BATTERY_VOLTAGE_MAX_LENGTH];
    uint8_t label_soc[BATTERY_SOC_MAX_LENGTH];

    snprintf(label_id, BATTERY_ID_MAX_LENGTH, "ID: %d", info.id);
    snprintf(label_voltage, BATTERY_VOLTAGE_MAX_LENGTH, "%0.2fV", info.pack_volt / 100.0);
    snprintf(label_soc, BATTERY_SOC_MAX_LENGTH, "SOC %0.2f%%", info.pack_soc / 100.0);

    switch (ui_battery_info_id)
    {
    case 1:
        nextion_setText(label_id, battery_ID_1[0]);      // update battery id label
        nextion_setText(label_voltage, battery_ID_1[1]); // update battery voltage label
        nextion_setText(label_soc, battery_ID_1[2]);     // update battery soc label
        break;
    case 2:
        nextion_setText(label_id, battery_ID_2[0]);      // update battery id label
        nextion_setText(label_voltage, battery_ID_2[1]); // update battery voltage label
        nextion_setText(label_soc, battery_ID_2[2]);     // update battery soc label
        break;
    case 3:
        nextion_setText(label_id, battery_ID_3[0]);      // update battery id label
        nextion_setText(label_voltage, battery_ID_3[1]); // update battery voltage label
        nextion_setText(label_soc, battery_ID_3[2]);     // update battery soc label
        break;
    case 4:
        nextion_setText(label_id, battery_ID_4[0]);      // update battery id label
        nextion_setText(label_voltage, battery_ID_4[1]); // update battery voltage label
        nextion_setText(label_soc, battery_ID_4[2]);     // update battery soc label
        break;
    case 5:
        nextion_setText(label_id, battery_ID_5[0]);      // update battery id label
        nextion_setText(label_voltage, battery_ID_5[1]); // update battery voltage label
        nextion_setText(label_soc, battery_ID_5[2]);     // update battery soc label
        break;
    case 6:
        nextion_setText(label_id, battery_ID_6[0]);      // update battery id label
        nextion_setText(label_voltage, battery_ID_6[1]); // update battery voltage label
        nextion_setText(label_soc, battery_ID_6[2]);     // update battery soc label
        break;
    case 7:
        nextion_setText(label_id, battery_ID_7[0]);      // update battery id label
        nextion_setText(label_voltage, battery_ID_7[1]); // update battery voltage label
        nextion_setText(label_soc, battery_ID_7[2]);     // update battery soc label
        break;
    case 8:
        nextion_setText(label_id, battery_ID_8[0]);      // update battery id label
        nextion_setText(label_voltage, battery_ID_8[1]); // update battery voltage label
        nextion_setText(label_soc, battery_ID_8[2]);     // update battery soc label
        break;
    case 9:
        nextion_setText(label_id, battery_ID_9[0]);      // update battery id label
        nextion_setText(label_voltage, battery_ID_9[1]); // update battery voltage label
        nextion_setText(label_soc, battery_ID_9[2]);     // update battery soc label
        break;
    case 10:
        nextion_setText(label_id, battery_ID_10[0]);      // update battery id label
        nextion_setText(label_voltage, battery_ID_10[1]); // update battery voltage label
        nextion_setText(label_soc, battery_ID_10[2]);     // update battery soc label
        break;
    case 11:
        nextion_setText(label_id, battery_ID_11[0]);      // update battery id label
        nextion_setText(label_voltage, battery_ID_11[1]); // update battery voltage label
        nextion_setText(label_soc, battery_ID_11[2]);     // update battery soc label
        break;
    case 12:
        nextion_setText(label_id, battery_ID_12[0]);      // update battery id label
        nextion_setText(label_voltage, battery_ID_12[1]); // update battery voltage label
        nextion_setText(label_soc, battery_ID_12[2]);     // update battery soc label
        break;
    default:
        LOGI("Unsupported battery = %d\r\n", ui_battery_info_id);
        break;
    }
}

/**
 * @brief
 *
 * @param info
 */
static void ui_total_screen_check_alert_error(Battery_info_t info)
{
    uint8_t label[BATTERY_ID_MAX_LENGTH];
    // update common parameter
    if (info.alert.value != 0)
    {
        nextion_button_change_pic(ui_total_button_images[info.id - 1], 47);
    }
    else if (info.alarm.value != 0)
    {
        // display alarm image
        nextion_button_change_pic(ui_total_button_images[info.id - 1], 48);
    }
    else
    {
        nextion_button_change_pic(ui_total_button_images[info.id - 1], 13);
    }
    
}

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/