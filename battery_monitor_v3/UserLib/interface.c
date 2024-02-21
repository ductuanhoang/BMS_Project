/**
 * @file interface.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-08-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "main.h"

#include "bms_board_config.h"
#include "interface.h"
#include "registers.h"
#include "common.h"
#include "bms.h"
#include <string.h>

extern I2C_HandleTypeDef hi2c1;

#define BMS_I2C hi2c1
#define BMS_I2C_TIMEOUT 100

int adc_gain;   // factory-calibrated, read out from chip (uV/LSB)
int adc_offset; // factory-calibrated, read out from chip (mV)

// indicates if a new current reading or an error is available from BMS IC
static bool alert_interrupt_flag;
// static time_t alert_interrupt_timestamp;

#ifndef UNIT_TEST

static int i2c_address = BME_BOARD_ADDRESS;
static bool crc_enabled; // determined automatically

// // The bq769x0 drives the ALERT pin high if the SYS_STAT register contains
// // a new value (either new CC reading or an error)
// static void bq769x0_alert_isr(const struct device *port, struct gpio_callback *cb,
//                               gpio_port_pins_t pins)
// {
//     alert_interrupt_timestamp = uptime();
//     alert_interrupt_flag = true;
// }

static const uint8_t crc8_ccitt_small_table[16] = {
    0x00, 0x07, 0x0e, 0x09, 0x1c, 0x1b, 0x12, 0x15,
    0x38, 0x3f, 0x36, 0x31, 0x24, 0x23, 0x2a, 0x2d};

uint8_t crc8_ccitt(uint8_t val, const void *buf, size_t cnt)
{
    size_t i;
    const uint8_t *p = buf;

    for (i = 0; i < cnt; i++)
    {
        val ^= p[i];
        val = (val << 4) ^ crc8_ccitt_small_table[val >> 4];
        val = (val << 4) ^ crc8_ccitt_small_table[val >> 4];
    }
    return val;
}

void bq769x0_write_byte(I2C_HandleTypeDef *hi2c, uint8_t reg_addr, uint8_t data)
{
    uint8_t buf[4] = {i2c_address << 1, // slave address incl. R/W bit required for CRC calculation
                      reg_addr, data};

    if (crc_enabled)
    {
        buf[3] = crc8_ccitt(0, buf, 3);
        // LOGI("crc8 = 0x%x --- buf[2] = 0x%x --- buf[1] = 0x%x--- buf[0] = 0x%x\r\n", buf[3], buf[2], buf[1], buf[0]);
        HAL_I2C_Master_Transmit(&hi2c1, i2c_address << 1, buf + 1, 3, 1000);
    }
    else
    {
        HAL_I2C_Master_Transmit(&hi2c1, i2c_address << 1, buf + 1, 2, 1000);
    }
}

uint8_t bq769x0_read_byte(I2C_HandleTypeDef *hi2c,uint8_t reg_addr)
{
    uint8_t buf[3];

    // i2c_write(i2c_dev, &reg_addr, 1, i2c_address);
    HAL_I2C_Master_Transmit(&hi2c1, i2c_address << 1, &reg_addr, 1, 1000);

    if (crc_enabled)
    {
        // CRC is calculated over the slave address (incl. R/W bit) and data
        buf[0] = (i2c_address << 1) | 1U;
        do
        {
            HAL_I2C_Master_Receive(&hi2c1, i2c_address << 1, buf + 1, 2, 1000);
        } while (crc8_ccitt(0, buf, 2) != buf[2]);

        return buf[1];
    }
    else
    {
        HAL_I2C_Master_Receive(&hi2c1, i2c_address << 1, buf + 1, 1, 1000);
        return buf[0];
    }
}

int32_t bq769x0_read_word(I2C_HandleTypeDef *hi2c,uint8_t reg_addr)
{
    uint8_t buf[5];

    // write starting register
    // i2c_write(i2c_dev, &reg_addr, 1, i2c_address);
    HAL_I2C_Master_Transmit(hi2c, i2c_address << 1, &reg_addr, 1, 1000);
    if (crc_enabled)
    {
        // CRC is calculated over the slave address (incl. R/W bit) and data
        buf[0] = (i2c_address << 1) | 1U;
        // i2c_read(i2c_dev, buf + 1, 4, i2c_address);
        HAL_I2C_Master_Receive(hi2c, i2c_address << 1, buf + 1, 4, 1000);

        if (crc8_ccitt(0, buf, 2) != buf[2])
        {
            return -1;
        }

        // CRC of subsequent bytes only considering data
        if (crc8_ccitt(0, buf + 3, 1) != buf[4])
        {
            return -1;
        }

        return buf[1] << 8 | buf[3];
    }
    else
    {
        // i2c_read(i2c_dev, buf, 2, i2c_address);
        HAL_I2C_Master_Receive(hi2c, i2c_address << 1, buf, 2, 1000);
        return buf[0] << 8 | buf[1];
    }
}

// automatically find out address and CRC setting
// automatically find out address and CRC setting
static int determine_address_and_crc(Bms *bms)
{
    i2c_address = 0x08;
    crc_enabled = 1;
    bq769x0_write_byte(&bms->hi2c, BQ769X0_CC_CFG, 0x19);
    if (bq769x0_read_byte(&bms->hi2c, BQ769X0_CC_CFG) == 0x19)
    {
        return 0;
    }
    return -1;
}

int bq769x0_init(Bms *bms)
{
    // gpio_pin_configure_dt(&alert, GPIO_INPUT);
    // gpio_init_callback(&alert_cb, bq769x0_alert_isr, BIT(alert.pin));
    // gpio_add_callback(alert.port, &alert_cb);
    LOGI("BMS bq769x0_init\r\n");
    int ret = HAL_I2C_IsDeviceReady(&bms->hi2c, i2c_address << 1, 3, 5);
    if (ret != HAL_OK) /* No ACK Received At That Address */
    {
        return -1;
    }
    LOGI("BMS device ready\r\n");

    alert_interrupt_flag = true; // init with true to check and clear errors at start-up

    int err = determine_address_and_crc(bms);
    if (!err)
    {
        // initial settings for bq769x0
        bq769x0_write_byte(&bms->hi2c, BQ769X0_SYS_CTRL1, 0x18); // switch external thermistor and ADC on
        bq769x0_write_byte(&bms->hi2c, BQ769X0_SYS_CTRL2, 0x40); // switch CC_EN on

        // get ADC offset and gain
        adc_offset = (signed int)bq769x0_read_byte(&bms->hi2c, BQ769X0_ADCOFFSET);                                                                // 2's complement
        adc_gain = 365 + (((bq769x0_read_byte(&bms->hi2c, BQ769X0_ADCGAIN1) & 0x0C) << 1) | ((bq769x0_read_byte(&bms->hi2c, BQ769X0_ADCGAIN2) & 0xE0) >> 5)); // uV/LSB
        LOGI("adc_offset_1 = %d, gain = %d\r\n", adc_offset, adc_gain);
    }
    else
    {
        LOGI("BMS communication error");
        return err;
    }

    return 0;
}

#endif // UNIT_TEST

bool bq769x0_alert_flag()
{
    return alert_interrupt_flag;
}

void bq769x0_alert_flag_reset()
{
    alert_interrupt_flag = false;
}

// time_t bq769x0_alert_timestamp()
// {
//     return alert_interrupt_timestamp;
// }
