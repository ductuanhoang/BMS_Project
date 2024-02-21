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
#include <string.h>
#include <math.h>
#include <stdlib.h>     /* abs */
int LOGI(const char *Format, ...);

const char *byte2bitstr(uint8_t b);

uint32_t uptime(void);

#endif // COMMON_H_
