/******************************************************************************

 @file  

 @brief 

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 
 *****************************************************************************/
 
#include "osal.h"
#include "spl.h"

#include "hal_config.h"
#include "hal_assert.h"
#include "hal_cli.h"

#include "main.h"

#include "stdstr.h"
#include "stringx.h"
#include "bufmgr.h"

#if (HAL_CLI_EN > 0)

//#define HAL_CLI_RX_DATA_BLK_SIZE    8

//static void *tx_fifo;
//#define IS_CHAR(x)  ( x > 0 && x <= 127 )
static char rx_buf[HAL_CLI_RX_BUF_SIZE];
static uint8_t rx_buf_head;
static uint8_t rx_buf_tail;

extern void hal_cli_init( void )
{   
    FIFO_BUF_FLUSH( rx_buf_head, rx_buf_tail );
}

extern uint8_t hal_cli_rx_len( void )
{
    return FIFO_BUF_EMPTY(rx_buf_head, rx_buf_tail) ? 0 : 1;
}

extern void hal_cli_putchar(char c)
{
    spl_uart_txd( SPL_UART_PORT_0, (uint8_t)c );
}

extern char hal_cli_getchar( void )
{
    char c;
    if( !FIFO_BUF_EMPTY(rx_buf_head, rx_buf_tail) )
    {
        FIFO_BUF_GET( &c, rx_buf_tail, rx_buf, HAL_CLI_RX_BUF_SIZE );
        return c;
    }
    return '\0';
}

#if (HAL_CLI_PRINT_EN > 0)
extern void hal_cli_print_str(const char *s)
{
    while(*s)
    {
        hal_cli_putchar(*s++);
    }
}

extern void hal_cli_print_sint(int32_t num)
{
    char str[SINT_STR_LEN_MAX];
    uint8_t len;
    uint8_t i;
    
    len = tostr_sint(num, str);
    
    for(i = 0; i < len; i++)
    {
        hal_cli_putchar(str[i]);
    }
    
}

extern void hal_cli_print_uint(uint32_t num)
{
    char str[UINT_STR_LEN_MAX];
    uint8_t len;
    uint8_t i;
    
    len = tostr_uint(num, str);
    
    for(i = 0; i < len; i++)
    {
        hal_cli_putchar(str[i]);
    }
}

extern void hal_cli_print_hex8(uint8_t num)
{
    char str[HEX8_STR_LEN_MAX];
    uint8_t len;
    uint8_t i;
    
    len = tostr_hex8(num, str);
    
    for(i = 0; i < len; i++)
    {
        hal_cli_putchar(str[i]);
    }
}


extern void hal_cli_print_hex16(uint16_t num)
{
    char str[HEX16_STR_LEN_MAX];
    uint8_t len;
    uint8_t i;
    
    len = tostr_hex16(num, str);
    
    for(i = 0; i < len; i++)
    {
        hal_cli_putchar(str[i]);
    }
}

extern void hal_cli_print_hex32(uint32_t num)
{
    char str[HEX32_STR_LEN_MAX];
    uint8_t len;
    uint8_t i;
    
    len = tostr_hex32(num, str);
    
    for(i = 0; i < len; i++)
    {
        hal_cli_putchar(str[i]);
    }
}
#endif

extern void hal_cli_driver_handle_rxne( void )
{
    char c;
    
    while( !spl_uart_rxd_empty(SPL_UART_PORT_0) )
    {
        c = (char)spl_uart_rxd( SPL_UART_PORT_0 );
        if( !FIFO_BUF_FULL(rx_buf_head, rx_buf_tail, HAL_CLI_RX_BUF_SIZE) )
        {
            FIFO_BUF_PUT( c, rx_buf_head, rx_buf, HAL_CLI_RX_BUF_SIZE );
            osal_event_set( TASK_ID_APP_CLI, TASK_EVT_APP_CLI_RX_CHAR );
        }
        else
        {
            FIFO_BUF_FLUSH( rx_buf_head, rx_buf_tail );
            osal_event_set( TASK_ID_APP_CLI, TASK_EVT_APP_CLI_RX_OVF );
        }
    }
}

extern void spl_uart0_callback( uint8_t event )
{
    switch (event)
    {
        case SPL_UART_ISR_EVT_RXD:
            osal_event_set( TASK_ID_HAL_DRIVER_BASIC, TASK_EVT_HAL_DRIVER_BASIC_CLI_RXNE );
        break;

        //case SPL_UART_ISR_EVT_TXD_EMPTY:
        //    osal_event_set( TASK_ID_HAL_DRIVER_BASIC, TASK_EVT_HAL_DRIVER_BASIC_CLI_TXE );
        //break;
        
        case SPL_UART_ISR_EVT_RXD_FULL:
            osal_event_set( TASK_ID_APP_CLI, TASK_EVT_APP_CLI_RX_OVF );
        break;

        default:
        break;
    }
    
}

#endif /* (HAL_CLI_EN > 0) */

