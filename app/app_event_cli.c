/******************************************************************************

 @file  app_event_cli.c

 @brief This file contains the command line interface events handlers.

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

#include "stdstr.h"
#include "stringx.h"

#if APP_CLI_EN > 0
/**************************************************************************************************
 * TYPE DEFINES
 **************************************************************************************************/

typedef struct cli_cmd_list_t {
    char *cmd;
    void ( *p_fxn )( char *p_arg );
} CLI_CMD_LIST_t;

 /**************************************************************************************************
 * LOCAL API DECLARATION
 **************************************************************************************************/
#if APP_CLI_CMD_TEST_EN > 0
static void app_cli_cmd_test         ( char *p_arg );
#endif
#if APP_CLI_CMD_SYSCLK_INC_EN > 0
static void app_cli_cmd_sysclk_inc   ( char *p_arg );
#endif
#if APP_CLI_CMD_SYSCLK_DEC_EN > 0
static void app_cli_cmd_sysclk_dec   ( char *p_arg );
#endif
/**************************************************************************************************
 * CONSTANTS
 **************************************************************************************************/

/**************************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************************/
static FLASH CLI_CMD_LIST_t cmdList[] = {
#if APP_CLI_CMD_TEST_EN > 0
    { "test", app_cli_cmd_test },
#endif
#if APP_CLI_CMD_SYSCLK_INC_EN > 0
    { "+",    app_cli_cmd_sysclk_inc },
#endif
#if APP_CLI_CMD_SYSCLK_DEC_EN > 0
    { "-",    app_cli_cmd_sysclk_dec },
#endif
};



extern void app_event_cli_process_cmd( char *s )
{
    char *p_arg = NULL;
    char *cmd;
    uint16_t i;
    
    cmd = str_tok_r( s, " ", &p_arg);

    for ( i = 0; i < sizeof(cmdList)/sizeof(CLI_CMD_LIST_t); i++ )
    {
        if( str_cmp( cmd, cmdList[i].cmd ) == 0 )
        {
            if( cmdList[i].p_fxn != NULL )
            {
                cmdList[i].p_fxn( p_arg );
            }
            break;
        }
    }

    if( i == sizeof(cmdList)/sizeof(CLI_CMD_LIST_t) )
    {
        hal_cli_print_str( "Bad command.\r\n\r\n" );
    }
}

#if APP_CLI_CMD_TEST_EN > 0
static void app_cli_cmd_test( char *p_arg )
{
    static HAL_OWEEPROM_ROM_CODE_t rom_code;
    uint8_t err;
    
    p_arg = p_arg;
    
    hal_cli_print_str( "TEST\r\n\r\n" );

    err = hal_oweeprom_rom_read( &rom_code );
    if( err < 0 )
    {
        hal_cli_print_str( "Failed to access 1-wire EEPROM\r\n\r\n" );
        return;
    }
    
    hal_cli_print_str( "Family Code: 0x" );
    hal_cli_print_hex8( rom_code.family_code );
    hal_cli_print_str( "\r\n" );

    hal_cli_print_str( "Serial Num: 0x" );
    hal_cli_print_hex8( rom_code.serial_num[0] );
    hal_cli_print_hex8( rom_code.serial_num[1] );
    hal_cli_print_hex8( rom_code.serial_num[2] );
    hal_cli_print_hex8( rom_code.serial_num[3] );
    hal_cli_print_hex8( rom_code.serial_num[4] );
    hal_cli_print_hex8( rom_code.serial_num[5] );
    hal_cli_print_str( "\r\n" );

    hal_cli_print_str( "CRC Code: 0x" );
    hal_cli_print_hex8( rom_code.crc_code );
    hal_cli_print_str( "\r\n" );
}
#endif

#if APP_CLI_CMD_SYSCLK_INC_EN > 0
static void app_cli_cmd_sysclk_inc( char *p_arg )
{
    int8_t trim;

    p_arg = p_arg;
    
    trim = hal_mcu_hsi_trim_get();
    trim++;
    trim = hal_mcu_hsi_trim_set( trim );
    hal_cli_print_str( "TRIM to " );
    hal_cli_print_sint( trim );
    hal_cli_print_str( ".\r\n" );
}
#endif

#if APP_CLI_CMD_SYSCLK_INC_EN > 0
static void app_cli_cmd_sysclk_dec( char *p_arg )
{
    int8_t trim;

    p_arg = p_arg;
    
    trim = hal_mcu_hsi_trim_get();
    trim--;
    trim = hal_mcu_hsi_trim_set( trim );
    hal_cli_print_str( "TRIM to " );
    hal_cli_print_sint( trim );
    hal_cli_print_str( ".\r\n" );
}
#endif
#endif //APP_CLI_EN > 0
/**************************************************************************************************
**************************************************************************************************/

