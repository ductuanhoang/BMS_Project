/**
 * @file ui_common.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-12-05
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef UI_COMMON_H
#define UI_COMMON_H

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "nextion.h"
#include "nextion_text.h"
#include "nextion_button.h"
#include "nextion_page.h"
#include "nextion_visible.h"
#include "nextion_picture.h"
#include "nextion_comboBox.h"

#include "battery_manager.h"
#include <stdint.h>
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define BATTERY_ID_MAX_LENGTH (30)
#define BATTERY_VOLTAGE_MAX_LENGTH (10)
#define BATTERY_SOC_MAX_LENGTH (20)

typedef enum
{
    E_BATTERY_NORMAL,
    E_BATTERY_WARNING,
    E_BATTERY_ERROR,
    E_BATTERY_NOT_CONNECTED,
} e_pic_battery_type;

typedef enum
{
    E_BATTERY_CELL_NORMAL,
    E_BATTERY_CELL_WARNING,
    E_BATTERY_CELL_ERROR
} e_pic_cell_type;

typedef enum
{
    E_UI_SCREEN_STARTUP = -1,
    E_UI_SCREEN_MAIN,
    E_UI_SCREEN_TOTAL,
    E_UI_SCREEN_CELLS,
    E_UI_SCREEN_ALERTS,
    E_UI_SCREEN_SETTINGS,
    E_UI_SCREEN_SETTINGS_2,
    E_UI_SCREEN_SETTINGS_3,
    E_UI_SCREEN_PASSWORD,
    E_UI_SCREEN_POP_UP_WARNING,
    E_UI_SCREEN_POP_UP_ERROR
} e_ui_screen_type;
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef struct
{
    e_ui_screen_type pageId;
    uint8_t battery_selected;
} nextion_ui_t;


typedef struct
{
    char battery_ID[BATTERY_ID_MAX_LENGTH];
    char battery_volt[BATTERY_VOLTAGE_MAX_LENGTH];
    char battery_soc[BATTERY_SOC_MAX_LENGTH];
} ui_battery_info_t;

extern ui_battery_info_t ui_battery_info[12];
extern nextion_ui_t nextion_ui;
/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/

void ui_common_init(void);

/**
 * @brief 
 * 
 * @return uint8_t 
 */
uint8_t event_change_pageID(void);

void set_pageID(uint8_t pageId);

e_ui_screen_type get_pageID(void);

void set_battery_selected(uint8_t selected);

uint8_t get_battery_selected(void);

#endif /* UI_COMMON_H */