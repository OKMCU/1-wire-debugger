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
#if APP_CLI_CMD_OWDEV_EN > 0
static void app_cli_cmd_owdev    ( char *p_arg );
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
#if APP_CLI_CMD_OWDEV_EN > 0
    { "owdev", app_cli_cmd_owdev },
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
    p_arg = p_arg;
    
    hal_cli_print_str( "TEST\r\n\r\n" );
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

#if APP_CLI_CMD_OWDEV_EN > 0
static void app_cli_cmd_owdev_print_err( int8_t err )
{
    hal_cli_print_str( "Error!" );
    if( err == -HAL_OWDEV_ERR_DEV_NOT_FOUND )
    {
        hal_cli_print_str( "No devices found on 1-wire bus.\r\n" );
        return;
    }

    if( err == -HAL_OWDEV_ERR_BUS_BUSY )
    {
        hal_cli_print_str( "1-wire bus pulled low for a long time.\r\n" );
        return;
    }

    if( err == -HAL_OWDEV_ERR_CRC )
    {
        hal_cli_print_str( "CRC check failed.\r\n" );
        return;
    }
}

static void app_cli_cmd_owdev_print_rom_code( const HAL_OWDEV_ROM_CODE_t *rom_code )
{
    hal_cli_print_str( "Family Code: 0x" );
    hal_cli_print_hex8( rom_code->family_code );
    hal_cli_print_str( "\r\n" );

    hal_cli_print_str( "Serial Num: 0x" );
    hal_cli_print_hex8( rom_code->serial_num[0] );
    hal_cli_print_hex8( rom_code->serial_num[1] );
    hal_cli_print_hex8( rom_code->serial_num[2] );
    hal_cli_print_hex8( rom_code->serial_num[3] );
    hal_cli_print_hex8( rom_code->serial_num[4] );
    hal_cli_print_hex8( rom_code->serial_num[5] );
    hal_cli_print_str( "\r\n" );

    hal_cli_print_str( "CRC Code: 0x" );
    hal_cli_print_hex8( rom_code->crc_code );
    hal_cli_print_str( "\r\n" );
}

static void app_cli_cmd_owdev    ( char *p_arg )
{
    uint8_t buf[128];
    uint8_t rom_cmd = 0x00;
    uint16_t txlen = 0;
    uint16_t rxlen = 0;
    uint8_t i;
    int8_t err;
    char *token = NULL;
    HAL_OWDEV_ROM_CODE_t rom_code = {0};
    uint32_t num = 0;

    token = str_tok_r( p_arg, " ", &p_arg);
    if     ( str_cmp("-R", token) == 0 || str_cmp("-readrom", token) == 0 )
        rom_cmd = HAL_OWDEV_READ_ROM;
    else if( str_cmp("-M", token) == 0 || str_cmp("-matchrom", token) == 0 )
        rom_cmd = HAL_OWDEV_MATCH_ROM;
    else if( str_cmp("-S", token) == 0 || str_cmp("-skiprom", token) == 0 )
        rom_cmd = HAL_OWDEV_SKIP_ROM;
    else if( str_cmp("-OS", token) == 0 || str_cmp("-odskiprom", token) == 0 )
        rom_cmd = HAL_OWDEV_OD_SKIP_ROM;
    else if( str_cmp("-OM", token) == 0 || str_cmp("-odmatchrom", token) == 0 )
        rom_cmd = HAL_OWDEV_OD_MATCH_ROM;
    else if( str_cmp("-I", token) == 0 || str_cmp("-init", token) == 0 )
    {
        err = hal_owdev_init();
        if( err < 0 )
        {
            app_cli_cmd_owdev_print_err( err );
        }
        else
        {
            hal_cli_print_str( "OK! " );
            hal_cli_print_str( "1-wire bus init done. Slave device(s) detected. Standard speed.\r\n" );
        }
        goto check_useless_argument;
    }

    switch ( rom_cmd )
    {
        case HAL_OWDEV_READ_ROM:
        break;
        
        case HAL_OWDEV_MATCH_ROM:
        case HAL_OWDEV_OD_MATCH_ROM:
        {
            for( i = 0; i < 8; i++ )
            {
                token = str_tok_r( NULL, " ", &p_arg);
                if( token == NULL )
                {
                    break;
                }

                if( hexstr2uint( token, &num ) == 0 )
                {
                    break;
                }

                if( num > 0xFF )
                {
                    hal_cli_print_str( "Error! " );
                    hal_cli_print_str( "Illegal characters found in ROM code: " );
                    hal_cli_print_str( token );
                    hal_cli_print_str( "\r\n" );
                    return;
                }

                buf[i] = (uint8_t)num;
            }

            if( i != 8 )
            {
                hal_cli_print_str( "Error! " );
                hal_cli_print_str( "Please specify 8-byte ROM code in hex format.\r\n" );
                return;
            }
            
            rom_code.family_code    = buf[0];
            rom_code.serial_num[5]  = buf[1];
            rom_code.serial_num[4]  = buf[2];
            rom_code.serial_num[3]  = buf[3];
            rom_code.serial_num[2]  = buf[4];
            rom_code.serial_num[1]  = buf[5];
            rom_code.serial_num[0]  = buf[6];
            rom_code.crc_code       = buf[7];
            app_cli_cmd_owdev_print_rom_code( &rom_code );
        }
        
        case HAL_OWDEV_SKIP_ROM:
        case HAL_OWDEV_OD_SKIP_ROM:
        {
            token = str_tok_r( NULL, " ", &p_arg);
            if( str_cmp("-w", token) == 0 || str_cmp("-write", token) == 0 )
            {
                for( i = 0; i < sizeof(buf); i++ )
                {
                    token = str_tok_r( NULL, " ", &p_arg);
                    if( token == NULL )
                    {
                        if( i == 0 )
                        {
                            hal_cli_print_str( "Error! " );
                            hal_cli_print_str( "Please specify data to write.\r\n" );
                            return;
                        }
                        break;
                    }

                    if( hexstr2uint( token, &num ) == 0 )
                    {
                        break;
                    }

                    if( num > 0xFF )
                    {
                        hal_cli_print_str( "Error! " );
                        hal_cli_print_str( "Illegal data value: " );
                        hal_cli_print_str( token );
                        hal_cli_print_str( "\r\n" );
                        return;
                    }

                    buf[i] = (uint8_t)num;
                    txlen++;
                }
            }

            if( token != NULL )
            {
                if( str_cmp("-r", token) == 0 || str_cmp("-read", token) == 0 )
                {
                    token = str_tok_r( NULL, " ", &p_arg);
                    if( decstr2uint( token, &num ) == 0 )
                    {
                        hal_cli_print_str( "Error! " );
                        hal_cli_print_str( "Illegal read data length: " );
                        hal_cli_print_str( token );
                        hal_cli_print_str( "\r\n" );
                        return;
                    }

                    if( num == 0 || num >= sizeof(buf) )
                    {
                        hal_cli_print_str( "Error! " );
                        hal_cli_print_str( "Read data length should between 0 and " );
                        hal_cli_print_uint( sizeof(buf) );
                        hal_cli_print_str( "\r\n" );
                        return;
                    }

                    rxlen = num;
                }
                else
                {
                    hal_cli_print_str( "Error!" );
                    hal_cli_print_str( " Unknown command option: " );
                    hal_cli_print_str( token );
                    hal_cli_print_str( "\r\n" );
                    return;
                }
            }
        }
        break;
        
        default:
        {
            hal_cli_print_str( "Error! ");
            hal_cli_print_str( "Please specify ROM command.\r\n" );
            return;
        }
        break;
    }

    switch ( rom_cmd )
    {
        case HAL_OWDEV_READ_ROM:
            err = hal_owdev_rom_read( &rom_code );
            app_cli_cmd_owdev_print_rom_code( &rom_code );
        break;

        case HAL_OWDEV_MATCH_ROM:
            err = hal_owdev_rom_match( &rom_code );
        break;
        
        case HAL_OWDEV_OD_MATCH_ROM:
            err = hal_owdev_rom_odmatch( &rom_code );
        break;

        case HAL_OWDEV_SKIP_ROM:
            err = hal_owdev_rom_skip();
        break;
        
        case HAL_OWDEV_OD_SKIP_ROM:
            err = hal_owdev_rom_odskip();
        break;
    }
    
    if( err < 0 )
    {
        app_cli_cmd_owdev_print_err( err );
        goto check_useless_argument;
    }

    if( txlen )
    {
        hal_owdev_mem_write( buf, txlen );
    }

    if( rxlen )
    {
        hal_owdev_mem_read( buf, rxlen );
        
    }

    if( txlen )
    {
        hal_cli_print_str( "Totally write " );
        hal_cli_print_uint( txlen );
        hal_cli_print_str( " bytes.\r\n" );
    }

    if( rxlen )
    {
        hal_cli_print_str( "Totally read " );
        hal_cli_print_uint( rxlen );
        hal_cli_print_str( " bytes:\r\n" );
        
        for( i = 0; i < rxlen; i++ )
        {
            hal_cli_print_str( "0x" );
            hal_cli_print_hex8( buf[i] );
            hal_cli_print_str( " " );
        }
        hal_cli_print_str( " \r\n" );
    }
    
check_useless_argument:
    token = str_tok_r( NULL, " ", &p_arg);
    if( token )
    {
        hal_cli_print_str( "Warning! Ignored arguments: " );
        hal_cli_print_str( token );
        while( token )
        {
            token = str_tok_r( NULL, " ", &p_arg);
            if( token )
            {
                hal_cli_print_str( " " );
                hal_cli_print_str( token );
            }
        }
        hal_cli_print_str( "\r\n" );
    }
}
#endif


#endif //APP_CLI_EN > 0
/**************************************************************************************************
**************************************************************************************************/

