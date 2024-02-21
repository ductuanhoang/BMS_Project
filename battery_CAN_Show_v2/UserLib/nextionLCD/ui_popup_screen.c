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
#define NUMBER_BUTTON_UI_SETTINGS (2)
#define NUMBER_COMPONENT_UI_SETTINGS (4)
/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/
extern const char content_report_alarm[16][50];
extern const char content_report_protect[16][50];

extern const char content_protect_txt[16][15];
extern const char content_alarm_txt[16][15];
/***********************************************************************************************************************
 * Exported global variables and functions (to be accessed by other files)
 ***********************************************************************************************************************/
/**
 * @brief button use in main ui
 *
 */
const char ui_popup_button_table[NUMBER_BUTTON_UI_SETTINGS][14] = {
    "PopUpExit",
    "PopUp"};

/**
 * @brief component in the main ui
 *
 */
const char ui_popup_components[NUMBER_COMPONENT_UI_SETTINGS][14] = {
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
void ui_popup_screen_check_button_table(const char *message, uint16_t len)
{
    for (size_t i = 0; i < NUMBER_BUTTON_UI_SETTINGS; i++)
    {
        if (strcmp(message, ui_popup_button_table[i]) == 0)
        {
            LOGI("button pressed: %s", ui_popup_button_table[i]);
            // go to the new page
            if (i == 0)
                set_pageID(E_UI_SCREEN_MAIN);
            return;
        }
    }
}

/**
 * @brief
 *
 */
void ui_popup_warning_screen_update(void)
{
    char buffer[30];
    uint8_t *label_report;
    uint8_t index_alarm;
    label_report = (char *)malloc(100);
    snprintf(buffer, sizeof(buffer), "WARNING %s", totalBattery.alarm_message);
    for (size_t i = 0; i < 16; i++)
    {
        if (strcmp(totalBattery.alarm_message, content_alarm_txt[i]) == 0)
            snprintf(label_report, 100, "%s", content_report_alarm[i]);
    }
    nextion_setText(buffer, "t2");
    nextion_setText(label_report, "t1");
    free(label_report);
}

/**
 * @brief update component state of total screen
 *
 */
void ui_popup_error_screen_update(void)
{
    char buffer[30];
    uint8_t *label_report;
    uint8_t index_error;
    label_report = (char *)malloc(100);
    for (size_t i = 0; i < 16; i++)
    {
        if (strcmp(totalBattery.error_message, content_protect_txt[i]) == 0)
            snprintf(label_report, 100, "%s", content_report_protect[i]);
    }
    
    snprintf(buffer, sizeof(buffer), "ALERT %s", totalBattery.error_message);
    nextion_setText(buffer, "t2");
    nextion_setText(label_report, "t1");
    free(label_report);
}
/***********************************************************************************************************************
 * static functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/