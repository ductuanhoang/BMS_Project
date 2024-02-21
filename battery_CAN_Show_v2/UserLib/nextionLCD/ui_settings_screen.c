/**
 * @file ui_settings_screen.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-12-07
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
#include "ui_settings_screen.h"
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/
#define NUMBER_BUTTON_UI_SETTINGS (4)
#define NUMBER_COMPONENT_UI_SETTINGS (4)
/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/
static void setting_reset_param(void);

/***********************************************************************************************************************
 * Exported global variables and functions (to be accessed by other files)
 ***********************************************************************************************************************/
/**
 * @brief button use in main ui
 *
 */
const char ui_setting_button_table[NUMBER_BUTTON_UI_SETTINGS][14] = {
    "SettingBack",
    "SettingReset",
    "SettingEdit",
    "SettingAdmin"};

/**
 * @brief component in the main ui
 *
 */
const char ui_setting_components[NUMBER_COMPONENT_UI_SETTINGS][14] = {
    "SettingNumber",
    "SettingBaud",
    "SettingName",
    "SettingAlarm"};

/***********************************************************************************************************************
 * Imported global variables and functions (from other files)
 ***********************************************************************************************************************/
/**
 * @brief
 *
 * @param message
 * @param len
 */
void ui_setting_screen_check_button_table(const char *message, uint16_t len)
{
    for (size_t i = 0; i < NUMBER_BUTTON_UI_SETTINGS; i++)
    {
        if (strcmp(message, ui_setting_button_table[i]) == 0)
        {
            LOGI("button pressed: %s", ui_setting_button_table[i]);
            if (i == 0) // SettingBack
                set_pageID(E_UI_SCREEN_MAIN);
            else if (i == 1) // SettingReset
            {
                setting_reset_param();
            }
            else if (i == 2) // SettingEdit
                set_pageID(E_UI_SCREEN_SETTINGS_2);
            else if (i == 3) // SettingAdmin
                set_pageID(E_UI_SCREEN_PASSWORD);
            return;
        }
    }
}

/**
 * @brief update component state of total screen
 *
 */
void ui_setting_screen_update(void)
{
    uint8_t label[30];
    snprintf(label, sizeof(label), "%d", battery_settings.num_battery_supports);
    nextion_setText(label, ui_setting_components[0]);

    snprintf(label, sizeof(label), "%d Kbs", battery_settings.can_bus_baud_rate);
    nextion_setText(label, ui_setting_components[1]);

    snprintf(label, sizeof(label), "%s", battery_settings.profile_name);
    nextion_setText(label, ui_setting_components[2]);

    snprintf(label, sizeof(label), "%d%%", battery_settings.soc_trigger_alarm);
    nextion_setText(label, ui_setting_components[3]);
}
/***********************************************************************************************************************
 * static functions
 ***********************************************************************************************************************/
static void setting_reset_param(void)
{
    battery_settings.can_bus_baud_rate = CAN_BUS_BAUD;
    battery_settings.num_battery_supports = BATTERY_MAX_PACK;
    battery_settings.soc_trigger_alarm = SOC_TRIGGER_ALARM;
    sprintf(battery_settings.profile_name, "%s", PROFILE_NAME_1);
}
/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/