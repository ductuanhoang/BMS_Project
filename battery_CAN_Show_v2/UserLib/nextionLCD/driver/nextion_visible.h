/**
 * @file nextion_visible.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef NEXTION_VISIBLE_H
#define NEXTION_VISIBLE_H

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <stdbool.h>

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
#define USER_COLOR_RED (45283)
#define USER_COLOR_GREEN (1352)
#define USER_COLOR_YELLOW (65225)
/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/

/**
 * @brief set visibility of the nextion component
 * 
 * @param id 
 * @param visible 
 */
void nextion_setVisible(const char *id, bool visible);

/**
 * @brief
 *
 * @param id
 * @param color
 */
void nextion_setColorText(const char *id, uint32_t color);

/**
 * @brief
 *
 * @param id
 * @param color
 */
void nextion_setColorButton(const char *id, uint32_t color);

/**
 * @brief 
 * 
 * @param baud 
 */
void nextion_setBaud(uint32_t baud);

#endif /* NEXTION_VISIBLE_H */