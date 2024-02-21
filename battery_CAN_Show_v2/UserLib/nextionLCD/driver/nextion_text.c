/**
 * @file nextion_text.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-12-04
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
#include "nextion.h"
#include "nextion_text.h"
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables and functions (to be accessed by other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Imported global variables and functions (from other files)
 ***********************************************************************************************************************/

/**
 * @brief
 *
 * @param text
 * @param name
 */
void nextion_setText(const char *text, const char *name)
{
    char cmd[100];
    memset(cmd, 0x00, 100);
    uint16_t len = 0;
    len = snprintf(cmd, 100, "%s.txt=\"%s\"", name, text);
    nextion_send_message(cmd, len);
}

/**
 * @brief 
 * 
 * @param text 
 * @param name 
 * @return char* 
 */
char *nextion_getText(const char *text, const char *name)
{
    char *textContent = (char *)malloc(20 * sizeof(char));
    if(textContent == NULL)
    {
        LOGI("Memory allocation failed!\n");
    }
    
    return textContent;
}
/***********************************************************************************************************************
 * static functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/