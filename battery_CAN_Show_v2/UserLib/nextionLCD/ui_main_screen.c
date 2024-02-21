/**
 * @file ui_main_screen.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-12-06
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
#include "ui_main_screen.h"
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
#define NUMBER_BUTTON_UI_MAIN (3)
#define NUMBER_COMPONENT_UI_MAIN (8)

typedef enum
{
    E_0_AMP,
    E_5_AMP,
} e_main_gaue_picture;
/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/
static void update_error_id(void);
static void update_ui_main_value(void);
static void update_ui_main_gauge(void);

/***********************************************************************************************************************
 * Exported global variables and functions (to be accessed by other files)
 ***********************************************************************************************************************/

/**
 * @brief button use in main ui
 *
 */
const char ui_main_button_table[NUMBER_BUTTON_UI_MAIN][14] = {
    "Main_monitor",
    "Main_settings",
    "Main_Status"};

/**
 * @brief component in the main ui
 *
 */
const char ui_main_components[NUMBER_COMPONENT_UI_MAIN][14] = {
    "mainAmp",
    "mainPercent",
    "mainPower",
    "mainVoltage",
    "mainTimer",
    "mainStaNormal",
    "mainStaErr",
    "mainStaErr1"};
/***********************************************************************************************************************
 * Imported global variables and functions (from other files)
 ***********************************************************************************************************************/

/**
 * @brief
 *
 * @param message
 * @param len
 */
void ui_main_screen_check_button_table(const char *message, uint16_t len)
{
    for (size_t i = 0; i < NUMBER_BUTTON_UI_MAIN; i++)
    {
        if (strcmp(message, ui_main_button_table[i]) == 0)
        {
            LOGI("button pressed: %s", ui_main_button_table[i]);
            // go to the new page
            if (i == 0)
                set_pageID(E_UI_SCREEN_TOTAL);
            else if (i == 1)
                set_pageID(E_UI_SCREEN_SETTINGS);
            else if (i == 2)
            {

                set_pageID(E_UI_SCREEN_POP_UP_WARNING);
            }
            return;
        }
    }
}

/**
 * @brief update component state of total screen
 *
 */
void ui_main_screen_update(void)
{
    update_ui_main_gauge();
    update_ui_main_value();
    update_error_id();
}
/***********************************************************************************************************************
 * static functions
 ***********************************************************************************************************************/

/**
 * @brief update component picture as amp
 *
 */
uint16_t test_pic = 27;
static void update_ui_main_gauge(void)
{
    // component picture mainGauge
    int current = 0;
    uint16_t current_picture = test_pic;
    nextion_setPicNumber("mainGauge", current_picture);
    test_pic++;
    if (test_pic == 32)
        test_pic = 27;
}

static void update_ui_main_value(void)
{
    uint8_t label_value[BATTERY_ID_MAX_LENGTH];

    memset(label_value, 0, sizeof(label_value));
    snprintf(label_value, BATTERY_ID_MAX_LENGTH, "%0.1f AMPS", totalBattery.current / 100.0);
    nextion_setText(label_value, ui_main_components[0]);

    memset(label_value, 0, sizeof(label_value));
    snprintf(label_value, BATTERY_ID_MAX_LENGTH, "%0.0f", totalBattery.soc / 100.0);
    nextion_setText(label_value, ui_main_components[1]);

    memset(label_value, 0, sizeof(label_value));
    snprintf(label_value, BATTERY_ID_MAX_LENGTH, "%3.1fkW", totalBattery.power);
    nextion_setText(label_value, ui_main_components[2]);

    memset(label_value, 0, sizeof(label_value));
    snprintf(label_value, BATTERY_ID_MAX_LENGTH, "%0.1fV", totalBattery.voltage / 100.0);
    nextion_setText(label_value, ui_main_components[3]);

    memset(label_value, 0, sizeof(label_value));
    snprintf(label_value, 50, "%d:%d:%d", totalBattery.time_hours, totalBattery.time_minutes, totalBattery.time_seconds);
    nextion_setText(label_value, ui_main_components[4]);
}

/**
 * @brief
 *
 */
static void update_error_id(void)
{
    uint16_t alarm_index = 0;
    uint16_t error_index = 0;

    alarm_index = battery_manager_get_max_alarms();
    error_index = battery_manager_get_max_error();
    if (error_index != 0)
    {
        snprintf(totalBattery.error_message, sizeof(totalBattery.error_message), "%s", battery_info[error_index].alarm_error_message);
        // set value text error message
        nextion_setText(battery_info[error_index].alarm_error_message, "mainStaErr1");

        // set pic
        nextion_button_change_pic("mainStaErr", 25);
        // set visible state of mainStaErr1 and mainStaErr
        nextion_setVisible("mainStaErr", true);
        nextion_setVisible("mainStaErr1", true);
        nextion_setVisible("mainStaNormal", false);
        nextion_setColorText("mainStaErr1", USER_COLOR_RED);
    }
    else if (alarm_index != 0)
    {
        // set pic
        nextion_button_change_pic("mainStaErr", 26);
        // set visible state of mainStaErr1 and mainStaErr
        nextion_setVisible("mainStaErr", true);
        nextion_setVisible("mainStaErr1", true);
        nextion_setVisible("mainStaNormal", false);
        // set value text error message
        snprintf(totalBattery.alarm_message, sizeof(totalBattery.alarm_message), "%s", battery_info[alarm_index].alarm_error_message);
        nextion_setText(battery_info[alarm_index].alarm_error_message, "mainStaErr1");
        // set text color alarm
        nextion_setColorText("mainStaErr1", USER_COLOR_YELLOW);
    }
    else
    {
        nextion_setVisible("mainStaErr", false);
        nextion_setVisible("mainStaErr1", false);
        nextion_setVisible("mainStaNormal", true);
    }
}

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/