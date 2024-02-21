/*
 * Copyright (c) The Libre Solar Project Contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "common.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
//#include "usb_device.h"
extern UART_HandleTypeDef huart1;
//extern USBD_HandleTypeDef hUsbDeviceFS;

int LOGI(const char *Format, ...)
{
    uint8_t buff[512 + 1] = {0};
    va_list args;
    va_start(args, Format);
    uint16_t len = vsprintf((char *)buff, Format, args);
    buff[len] = '\r';
    buff[len + 1] = '\n';
    HAL_UART_Transmit(&huart1, (uint8_t *)buff, len + 2, 500);
    ////    USBD_CDC_SetTxBuffer(&hUsbDeviceFS, (uint8_t *)buff, len);
//    USBD_CDC_TransmitPacket(&hUsbDeviceFS);
    va_end(args);
    return -1;
}

const char *byte2bitstr(uint8_t b)
{
    static char str[9];

    str[0] = '\0';
    for (int z = 128; z > 0; z >>= 1)
    {
        strcat(str, ((b & z) == z) ? "1" : "0");
    }

    return str;
}

uint32_t uptime(void)
{
    return (HAL_GetTick());
}