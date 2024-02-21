/**
 * @file ui_settings_screen_2.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-12-22
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
#include "ui_settings_screen_2.h"
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/
#define NUMBER_BUTTON_UI_SETTINGS (4)
#define NUMBER_COMPONENT_UI_SETTINGS (4)
/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
typedef enum
{
    E_SETTING_GET_NUMBER_BATTERY = 0,
    E_SETTING_GET_CAN_BAUD,
    E_SETTING_GET_CAN_PROFILE,
    E_SETTING_GET_ALARM_SOC,
    E_SETTING_GET_DONE
} e_get_param_value;

/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/
static void save_user_setting(void);
static void reset_parameters(void);
static void ui_setting_screen_2_param_come(const char *message, uint16_t len);
uint8_t update_only_2_times = 0;
bool update_get_data = false;
uint8_t state_get_param = E_SETTING_GET_NUMBER_BATTERY;
/***********************************************************************************************************************
 * Exported global variables and functions (to be accessed by other files)
 ***********************************************************************************************************************/
/**
 * @brief button use in main ui
 *
 */
const char ui_setting_2_button_table[NUMBER_BUTTON_UI_SETTINGS][15] = {
    "Setting2Back",
    "Setting2Cancel",
    "Setting2Save"};

/**
 * @brief component in the main ui
 *
 */
const char ui_setting_2_components[NUMBER_COMPONENT_UI_SETTINGS][15] = {
    "Setting2Number", // number of battery supporting
    "Setting2Baud",   // CAN bus speed
    "Setting2Name",   // user name
    "Setting2Alarm"}; // percentage battery soc

/***********************************************************************************************************************
 * Imported global variables and functions (from other files)
 ***********************************************************************************************************************/
/**
 * @brief
 *
 * @param message
 * @param len
 */
void ui_setting_screen_2_check_button_table(const char *message, uint16_t len)
{
    // if (update_get_data == false)
    // {
    LOGI("check button screen 2 %s", message);
    for (size_t i = 0; i < NUMBER_BUTTON_UI_SETTINGS; i++)
    {
        if (strcmp(message, ui_setting_2_button_table[i]) == 0)
        {
            LOGI("button pressed: %s", ui_setting_2_button_table[i]);
            if (i == 0) // SettingBack
            {
                reset_parameters();
                set_pageID(E_UI_SCREEN_SETTINGS);
            }
            else if (i == 1) // SettingCancel
            {
                reset_parameters();
                set_pageID(E_UI_SCREEN_SETTINGS);
            }
            else if (i == 2) // SettingSave
            {
                // save settings and go back to settings screen
                LOGI("get and save settings");
                HAL_Delay(100);
                // save_user_setting();
                update_get_data = true;
            }
            break;
        }
    }
    if (update_get_data == true)
    {
        ui_setting_screen_2_param_come(message, len);
    }
}

/**
 * @brief call in loop to update
 *
 */
void ui_setting_screen_get_data(void)
{
    if (update_get_data == true)
        save_user_setting();
}
/**
 * @brief update component state of total screen
 *
 */
void ui_setting_screen_2_update(void)
{
    update_only_2_times++;
    if (update_only_2_times > 3)
        return;

    uint8_t label[30];
    LOGI("set here");
    snprintf(label, sizeof(label), "%d", battery_settings.num_battery_supports);
    nextion_setText(label, ui_setting_2_components[0]);

    snprintf(label, sizeof(label), "%d Kbs", battery_settings.can_bus_baud_rate);
    nextion_setText(label, ui_setting_2_components[1]);

    snprintf(label, sizeof(label), "%s", battery_settings.profile_name);
    nextion_setText(label, ui_setting_2_components[2]);

    snprintf(label, sizeof(label), "%d%%", battery_settings.soc_trigger_alarm);
    nextion_setText(label, ui_setting_2_components[3]);
}

/***********************************************************************************************************************
 * static functions
 ***********************************************************************************************************************/
/**
 * @brief
 *
 * @param message
 * @param len
 */
static void ui_setting_screen_2_param_come(const char *message, uint16_t len)
{
    LOGI("get param come: %d -- message = %s --update_get_data %d", state_get_param, message, update_get_data);
    uint8_t buffer[4];
    if (message[0] != 0x70)
        return;

    switch (state_get_param)
    {
    case E_SETTING_GET_NUMBER_BATTERY:
        LOGI("NUMBER_BATTERY: %s", message + 1);
        state_get_param = E_SETTING_GET_CAN_BAUD;
        battery_settings.num_battery_supports = atoi(message + 1);
        /* code */
        break;
    case E_SETTING_GET_CAN_BAUD:
        LOGI("CAN_BAUD: %s", message + 1);
        battery_settings.can_bus_baud_rate = atoi(message + 1);
        state_get_param = E_SETTING_GET_CAN_PROFILE;
        break;
    case E_SETTING_GET_CAN_PROFILE:
        LOGI("CAN_PROFILE: %s", message + 1);
        snprintf(battery_settings.profile_name, sizeof(battery_settings.profile_name), "%s", message + 1);
        state_get_param = E_SETTING_GET_ALARM_SOC;
        break;
    case E_SETTING_GET_ALARM_SOC:
        LOGI("ALARM_SOC: %s", message + 1);
        battery_settings.soc_trigger_alarm = atoi(message + 1);
        state_get_param = E_SETTING_GET_DONE;
        break;
    default:
        break;
    }
}

static void save_user_setting(void)
{
    // 70 31 32 35 20 4B 62 73 FF FF FF
    // p125 Kbs\xFF\xFF\xFF
    switch (state_get_param)
    {
    case E_SETTING_GET_NUMBER_BATTERY:
        LOGI("E_SETTING_GET_NUMBER_BATTERY");
        nextion_comboBox_get_text(ui_setting_2_components[0]);
        /* code */
        break;
    case E_SETTING_GET_CAN_BAUD:
        LOGI("E_SETTING_GET_CAN_BAUD");

        nextion_comboBox_get_text(ui_setting_2_components[1]);
        /* code */
        break;
    case E_SETTING_GET_CAN_PROFILE:
        LOGI("E_SETTING_GET_CAN_PROFILE");

        nextion_comboBox_get_text(ui_setting_2_components[2]);
        /* code */
        break;
    case E_SETTING_GET_ALARM_SOC:
        LOGI("E_SETTING_GET_ALARM_SOC");
        nextion_comboBox_get_text(ui_setting_2_components[3]);
        /* code */
        break;
    case E_SETTING_GET_DONE:
        LOGI("E_SETTING_GET_DONE");
        reset_parameters();
        set_pageID(E_UI_SCREEN_SETTINGS);
        break;
    default:
        break;
    }
}

/**
 * @brief Construct a new reset parameters object
 *
 */
static void reset_parameters(void)
{
    update_only_2_times = 0;
    update_get_data = false;
    state_get_param = 0;
}
/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/