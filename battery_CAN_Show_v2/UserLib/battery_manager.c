/**
 * @file battery_manager.c
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
#include "battery_manager.h"
#include "CANparser.h"
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
Battery_info_t battery_info[BATTERY_MAX_PACK + 1];
Total_Battery_info_t totalBattery;
Battery_Settings_info_t battery_settings;

static char *battery_warrning_get_name(bit_alarm_t alarm_flag);
/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/
const char batter_warring_name[16][10] = {
    "NO FAULTS",
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
    "A15"};
const char batter_error_name[16][10] = {
    "NO FAULTS",
    "E1",
    "E2",
    "E3",
    "E4",
    "E5",
    "E6",
    "E7",
    "E8",
    "E9",
    "E10",
    "E11",
    "E12",
    "E13",
    "E14",
    "E15"};
/***********************************************************************************************************************
 * Exported global variables and functions (to be accessed by other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Imported global variables and functions (from other files)
 ***********************************************************************************************************************/
void battery_manager_init(void)
{
    // Initialize
    totalBattery.time_hours = 0;
    totalBattery.time_minutes = 0;
    totalBattery.time_seconds = 0;

    battery_settings.can_bus_baud_rate = CAN_BUS_BAUD;
    battery_settings.num_battery_supports = BATTERY_MAX_PACK;
    battery_settings.soc_trigger_alarm = SOC_TRIGGER_ALARM;
    sprintf(battery_settings.profile_name, "%s", PROFILE_NAME_1);
}

/**
 * @brief
 *
 */
void battery_time_life(void)
{
    totalBattery.time_seconds++;
    if (totalBattery.time_seconds == 60)
    {
        totalBattery.time_seconds = 0;
        totalBattery.time_minutes++;
    }
    if (totalBattery.time_minutes == 60)
    {
        totalBattery.time_minutes = 0;
        totalBattery.time_hours++;
    }
}

void battery_total_getValue(void)
{
    uint32_t voltage = 0;
    uint32_t current = 0;
    uint32_t soc = 0;
    uint8_t totalPack = 0;
    for (size_t index = 1; index <= BATTERY_MAX_PACK; index++)
    {
        if (battery_info[index].active == true)
        {
            totalPack++;
            voltage += battery_info[index].pack_volt;
            current += battery_info[index].current;
            soc = battery_info[index].pack_soc;
        }
    }
    totalBattery.voltage = voltage;
    totalBattery.current = current;
    totalBattery.soc = soc;
    totalBattery.totalPack = totalPack;
}

/**
 * @brief
 *
 */
void battery_manager_set_error_name(void)
{
    char *message = NULL;
    for (size_t index = 1; index <= BATTERY_MAX_PACK; index++)
    {
        if (battery_info[index].active == true)
        {
            if (battery_info[index].alert.value != 0)
            {
                // battery_warrning_get_name(battery_info[index].alert);
            }
            else if (battery_info[index].alarm.value != 0)
            {
                message = battery_warrning_get_name(battery_info[index].alarm);
                snprintf(battery_info[index].alarm_error_message,
                         sizeof(battery_info[index].alarm_error_message), "%s",
                         message);
            }
            else
                snprintf(battery_info[index].alarm_error_message,
                         sizeof(battery_info[index].alarm_error_message), "%s",
                         batter_warring_name[0]);
        }
    }
    free(message);
}
// Bit 15	Reserved
// Bit 14	Reserved
// Bit 13	Reserved
// Bit 12	Reserved
// Bit 11	Module Over Voltage Protect
// Bit 10	Module Under Voltage Protect
// Bit 9	Discharge Over Current Protect
// Bit 8	Charge Over Current Protect
// Bit 7	Discharge Cell Over Temperature Protect
// Bit 6	Discharge Cell Under Temperature Protect
// Bit 5	Charge Cell Over Temperature Protect
// Bit 4	Charge Cell Under Temperature Protect
// Bit 3	Charge system Over Voltage Protect
// Bit 2	Discharge system Under Voltage Protect
// Bit 1	Single Cell Over Voltage Protect
// Bit 0	Single Cell Under Voltage Protect
/**
 * @brief check battery status protect flag
 * call in loop check
 *
 */
void battery_error_check(void)
{
}

// Bit 15	Reserved
// Bit 14	Reserved
// Bit 13	Reserved
// Bit 12	Reserved
// Bit 11	Module high voltage alarm (Flag if any cell gets above 3.70V)
// Bit 10	Module low voltage alarm (Flag if any cell goes below 3.00V)
// Bit 9	Discharge over current alarm (Flag if discharge current is over discharge current limit in 0x4220)
// Bit 8	Charge over current alarm (Flag if charge current is over charge current limit in 0x4220)
// Bit 7	Discharge Cell High Temperature Alarm (Flag if any sensor goes above 65 degrees)
// Bit 6	Discharge Cell Low Temperature Alarm (Flag if any sensor goes below 0 degrees)
// Bit 5	Charge Cell High Temperature Alarm (Flag if any sensor goes above 65 degrees)
// Bit 4	Charge Cell Low Temperature Alarm (Flag if any sensor goes below 5 degrees)
// Bit 3	Charge system High Voltage Alarm (Flag if voltage goes 1V above Charge Cutoff Voltage in 0x4220)
// Bit 2	Discharge system Low Voltage Alarm (Flag if any cell goes below 3.00V)
// Bit 1	Single Cell High Voltage Alarm (Flag if any cell gets above 3.70V)
// Bit 0	Single Cell Low Voltage Alarm (Flag if any cell goes below 3.00V)

/**
 * @brief check battery status alarm flag
 * call in loop check
 *
 */
void battery_warrning_check(void)
{
    uint8_t cell_alarm_active_high_volt = 0;
    uint8_t cell_alarm_active_low_volt = 0;
    for (size_t index = 0; index < CAN_ID_HEADER_MAX; index++)
    {
        if (battery_info[index].active == true)
        {
            // TODO: bit 0
            // Single Cell Low Voltage Alarm (Flag if any cell goes below 3.00V)
            for (uint8_t i = 0; i < BATTERY_CELLS; i++)
            {
                if ((battery_info[index].cell[i].voltage / 100.0) < 3.0)
                    battery_info[index].cell[i].alarm.bits.bit_0 = 1;
                else
                    battery_info[index].cell[i].alarm.bits.bit_0 = 0;

                // Single Cell High Voltage Alarm (Flag if any cell gets above 3.70V)
                if ((battery_info[index].cell[i].voltage / 100.0) > 3.70)
                    battery_info[index].cell[i].alarm.bits.bit_1 = 1;
                else
                    battery_info[index].cell[i].alarm.bits.bit_1 = 0;

                if (battery_info[index].cell[i].alarm.bits.bit_0 != 0)
                    cell_alarm_active_low_volt = 1;

                if (battery_info[index].cell[i].alarm.bits.bit_1 != 0)
                    cell_alarm_active_high_volt = 1;
            }
            // TODO: bit 10
            // TODO: bit 11
            if (cell_alarm_active_low_volt == 1)
                battery_info[index].alarm.bits.bit_10 = 1; // Done
            else
                battery_info[index].alarm.bits.bit_10 = 0; // Done

            if (cell_alarm_active_high_volt == 1)
                battery_info[index].alarm.bits.bit_11 = 1; // Done
            else
                battery_info[index].alarm.bits.bit_11 = 0; // Done

            for (uint8_t j = 0; j < BATTERY_TEMP_SENSORS; j++)
            {
                // TODO: bit 4
                if (battery_info[index].temperature[j] / 100.0 > 65.0)
                {
                    battery_info[index].alarm.bits.bit_4 = 1; // Done
                    break;
                }
                else
                    battery_info[index].alarm.bits.bit_4 = 0; // done
            }

            for (uint8_t j = 0; j < BATTERY_TEMP_SENSORS; j++)
            {
                // TODO: bit 5
                // Charge Cell High Temperature Alarm (Flag if any sensor goes above 65 degrees)
                if (battery_info[index].temperature[j] / 100.0 > 65.0)
                {
                    battery_info[index].alarm.bits.bit_5 = 1;
                    break;
                }
                else
                    battery_info[index].alarm.bits.bit_5 = 0;
            }

            for (uint8_t j = 0; j < BATTERY_TEMP_SENSORS; j++)
            {
                // TODO: bit 6
                if (battery_info[index].temperature[j] / 100.0 < 0.0)
                {
                    battery_info[index].alarm.bits.bit_6 = 1;
                    break;
                }
                else
                    battery_info[index].alarm.bits.bit_6 = 0;
            }
            for (uint8_t j = 0; j < BATTERY_TEMP_SENSORS; j++)
            {
                // TODO: bit 7
                if (battery_info[index].temperature[j] / 100.0 > 65.0)
                {
                    battery_info[index].alarm.bits.bit_7 = 1;
                    break;
                }
                else
                    battery_info[index].alarm.bits.bit_7 = 0;
            }
        }
        // TODO: bit 8
        // TODO: bit 9
        battery_info[index].alarm.bits.reserved = 0;
    }
}

/**
 * @brief
 *
 * @return uint16_t
 */
uint16_t battery_manager_get_max_alarms()
{
    uint16_t index = 0;
    uint16_t max_alarm = 0;
    for (size_t i = 0; i < CAN_ID_HEADER_MAX; i++)
    {
        if (battery_info[i].alarm.value > max_alarm)
        {
            max_alarm = battery_info[i].alarm.value;
            return i;
        }
    }
    return index;
}

/**
 * @brief
 *
 * @return uint16_t
 */
uint16_t battery_manager_get_max_error()
{
    uint16_t index = 0;
    uint16_t max_error = 0;
    for (size_t i = 0; i < CAN_ID_HEADER_MAX; i++)
    {
        if (battery_info[i].alert.value > max_error)
        {
            max_error = battery_info[i].alert.value;
            return i;
        }
    }
    return index;
}

void battery_action_process(void)
{
}
/***********************************************************************************************************************
 * static functions
 ***********************************************************************************************************************/
static char *battery_warrning_get_name(bit_alarm_t alarm_flag)
{
    // Allocate memory for a string
    char *alarm_name = (char *)malloc(12 * sizeof(char)); // 12 is just an example size

    // Check if memory allocation was successful
    if (alarm_name == NULL)
    {
        return alarm_name;
    }
    if (alarm_flag.value == 0)
        strcpy(alarm_name, batter_warring_name[0]);
    for (size_t i = 0; i < 15; i++)
    {
        uint8_t bit = (alarm_flag.value >> i) & 0x01;
        if (bit == 1)
        {
            strcpy(alarm_name, batter_warring_name[i]);
            return alarm_name;
        }
    }

    // Return the allocated memory
    return alarm_name;
}
/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/