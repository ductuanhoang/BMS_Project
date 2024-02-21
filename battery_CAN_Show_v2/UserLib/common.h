/**
 * @file common.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-08-08
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef COMMON_H_
#define COMMON_H_
#include "main.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h> /* abs */


#define BATTERY_CELLS (15U)
#define BATTERY_TEMP_SENSORS (3U)
#define BATTERY_MAX_PACK (12U)

#define PROFILE_NAME_1 "DEYE"
#define PROFILE_NAME_2 "VICTRON"
#define SOC_TRIGGER_ALARM 100
#define CAN_BUS_BAUD 500 // 500 kbs

typedef union
{
    uint16_t value;
    struct
    {
        uint8_t bit_0 : 1;
        uint8_t bit_1 : 1;
        uint8_t bit_2 : 1;
        uint8_t bit_3 : 1;

        uint8_t bit_4 : 1;
        uint8_t bit_5 : 1;
        uint8_t bit_6 : 1;
        uint8_t bit_7 : 1;

        uint8_t bit_8 : 1;
        uint8_t bit_9 : 1;
        uint8_t bit_10 : 1;
        uint8_t bit_11 : 1;
        uint8_t reserved : 4;
    } bits;
} bit_alert_t;

typedef union
{
    uint16_t value;
    struct
    {
        uint8_t bit_0 : 1;
        uint8_t bit_1 : 1;
        uint8_t bit_2 : 1;
        uint8_t bit_3 : 1;

        uint8_t bit_4 : 1;
        uint8_t bit_5 : 1;
        uint8_t bit_6 : 1;
        uint8_t bit_7 : 1;

        uint8_t bit_8 : 1;
        uint8_t bit_9 : 1;
        uint8_t bit_10 : 1;
        uint8_t bit_11 : 1;
        uint8_t reserved : 4;
    } bits;
} bit_alarm_t;

typedef struct
{
    uint16_t voltage;
    bit_alarm_t alarm;
} cell_data_t;

typedef struct
{
    uint32_t id;
    char alarm_error_message[20];
    cell_data_t cell[BATTERY_CELLS];
    int16_t current;
    int16_t temperature[BATTERY_TEMP_SENSORS];
    uint16_t pack_volt;
    uint16_t pack_soc;
    uint16_t time;
    uint16_t time_counting_down;
    bit_alert_t alert;
    bit_alarm_t alarm;
    bool active;
} Battery_info_t;

typedef struct
{
    uint8_t profile_name[10];
    uint8_t num_battery_supports;
    uint32_t can_bus_baud_rate;
    uint8_t soc_trigger_alarm;
} Battery_Settings_info_t;

typedef struct
{
    uint32_t time_hours;
    uint8_t time_minutes;
    uint8_t time_seconds;
    int32_t current;
    float voltage;
    float power;
    uint16_t soc;
    uint8_t totalPack;
    char error_message[20];
    char alarm_message[20];
} Total_Battery_info_t;

extern Total_Battery_info_t totalBattery;

extern Battery_info_t battery_info[BATTERY_MAX_PACK + 1];

extern Battery_Settings_info_t battery_settings;

int LOGI(const char *Format, ...);

void LOG_COMMAND(uint8_t *buf, uint16_t len);

const char *byte2bitstr(uint8_t b);

uint32_t uptime(void);

#endif // COMMON_H_
