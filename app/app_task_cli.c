/******************************************************************************

 @file  app_task_cli.c

 @brief This file contains the command line interface service.

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 2016-06-09 06:57:09
 *****************************************************************************/

/**************************************************************************************************
 * INCLUDES
 **************************************************************************************************/
#include "osal.h"
#include "hal.h"
#include "app.h"

#include "main.h"

#include "stdstr.h"
#include "stringx.h"

#if APP_CLI_EN > 0
/**************************************************************************************************
 * CONSTANTS
 **************************************************************************************************/
#define CLI_CMD_LEN_MAX         128
#define ASCII_VALUE_BACKSPACE   127

/**************************************************************************************************
 * TYPES
 **************************************************************************************************/
typedef struct cli_cmd_t {
    char str[CLI_CMD_LEN_MAX];
    uint8_t len;
} CLI_CMD_t;

/**************************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************************/
static CLI_CMD_t cli_cmd;

extern void app_task_cli_init ( void )
{
    cli_cmd.len = 0;
}



static void app_task_cli_handle_rx_char( char c)
{
    if( c != '\r' && c != '\n' && c != 0x7F )
    {
        if( cli_cmd.len < CLI_CMD_LEN_MAX - 1 )
        {
            cli_cmd.str[cli_cmd.len++] = c;
            hal_cli_putchar( c );
        }
    }
    else if( c == '\r' )
    {
        hal_cli_putchar( c );
        hal_cli_putchar( '\n' );
        
        if( cli_cmd.len )
        {
            cli_cmd.str[cli_cmd.len] = '\0';
            app_event_cli_process_cmd( cli_cmd.str );
            cli_cmd.len = 0;
        }
    }
    else if( c == ASCII_VALUE_BACKSPACE )
    {
        hal_cli_putchar( c );
        if( cli_cmd.len > 0 )
        {
            cli_cmd.len--;
        }
    }
}

/**************************************************************************************************
 * @fn      app_task_cli
 *
 * @brief   app_task_cli
 *
 * @param   task_id - CLI Task Id
 *          events - events
 *
 * @return  None
 **************************************************************************************************/
void app_task_cli( uint8_t task_id, uint8_t event_id )
{
    task_id = task_id;
    
    switch ( event_id )
    {
        case TASK_EVT_APP_CLI_RX_CHAR:
        {
            while( hal_cli_rx_len() )
            {
                app_task_cli_handle_rx_char( hal_cli_getchar() );
            }
        }
        break;

        case TASK_EVT_APP_CLI_RX_OVF:
        {

        }
        break;
        
        default:
            APP_ASSERT_FORCED();
        break;
    }
}

#endif
/**************************************************************************************************
**************************************************************************************************/

