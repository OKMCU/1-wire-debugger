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
#include "Common.h"
#include "Function_Define.h"
#include "N76E003.h"
#include "SFR_Macro.h"
#include "spl_config.h"
#include "spl_gpio.h"
#include "spl_uart.h"
#include "spl_mcu.h"
#include "spl_sysclk.h"

#include "bufmgr.h"
#include "stdstr.h"

#if SPL_UART_EN > 0
#if SPL_UART0_EN > 0
extern void SPL_UART0_CALLBACK( uint8_t event );
#endif

#if SPL_UART1_EN > 0
extern void SPL_UART1_CALLBACK( uint8_t event );
#endif

#if SPL_UART0_EN > 0
static uint8_t uart0_rx_head;
static uint8_t uart0_rx_tail;
static uint8_t uart0_rx_cache[SPL_UART0_RX_CACHE_SIZE];
//static uint8_t uart0_tx_cache[SPL_UART0_TX_CACHE_SIZE];
#endif

#if SPL_UART1_EN > 0
static uint8_t uart1_rx_head;
static uint8_t uart1_rx_tail;
static uint8_t uart1_rx_cache[SPL_UART1_RX_CACHE_SIZE];
//static uint8_t uart1_tx_cache[SPL_UART1_TX_CACHE_SIZE];
#endif

extern void     spl_uart_init      ( uint8_t port )
{
#if ( SPL_SYSCLK_EN > 0 && SPL_SYSCLK_TRIM_EN > 0 )
    int32_t s32_tmp;
#endif

    if( port == SPL_UART_PORT_0 )
    {
        FIFO_BUF_FLUSH( uart0_rx_head, uart0_rx_tail );
        
        SPL_GPIO_SET_MODE_P06_QUASI();         //Setting UART pin as Quasi mode for transmit
        SPL_GPIO_SET_MODE_P07_QUASI();         //Setting UART pin as Quasi mode for transmit    
        
        SCON = 0x50;            //UART0 Mode1,REN=1,TI=1
        set_SMOD;               //UART0 Double Rate Enable
        T3CON &= 0xF8;          //T3PS2=0,T3PS1=0,T3PS0=0(Prescale=1)
        set_BRCK;               //UART0 baud rate clock source = Timer3

#if ( SPL_SYSCLK_EN == 0 || SPL_SYSCLK_TRIM_EN == 0 )
        RH3    = HIBYTE(65536 - (SPL_SYSCLK/16/SPL_UART0_BAUDRATE)-1);
        RL3    = LOBYTE(65536 - (SPL_SYSCLK/16/SPL_UART0_BAUDRATE)-1);
#else
        s32_tmp = (int32_t)SPL_SYSCLK + (int32_t)spl_sysclk_get_hirc()*40000;    //calculate accurate SYS_CLOCK
        s32_tmp /= (int32_t)16*SPL_UART0_BAUDRATE;
        s32_tmp = 65536 - 1 - s32_tmp;
        RH3    = HIBYTE((uint16_t)s32_tmp);
        RL3    = LOBYTE((uint16_t)s32_tmp);
#endif
        return;
    }
    
    
}

extern void spl_uart_open ( uint8_t port )
{
    if( port == SPL_UART_PORT_0 )
    {
        set_TR3;            //Trigger Timer3
        //set_TI;             //For printf function must setting TI = 1
        set_ES;             //enable interrupt
        return;
    }
}

extern void spl_uart_txd( uint8_t port, uint8_t byte )
{
    if( port == SPL_UART_PORT_0 )
    {
        SBUF = byte;
        while(!TI);
        TI = 0;
        set_ES;
    }
}

extern uint8_t  spl_uart_rxd      ( uint8_t port )
{
    uint8_t u8tmp;
    
#if SPL_UART0_EN > 0
    if( port == SPL_UART_PORT_0 )
    {
        spl_mcu_disable_irq();
        FIFO_BUF_GET( &u8tmp, uart0_rx_tail, uart0_rx_cache, SPL_UART0_RX_CACHE_SIZE );
        spl_mcu_enable_irq();
        return u8tmp;
    }
#endif

#if SPL_UART1_EN > 0
    if( port == SPL_UART_PORT_1 )
    {
        spl_mcu_disable_irq();
        FIFO_BUF_GET( &u8tmp, uart1_rx_tail, uart1_rx_cache, SPL_UART1_RX_CACHE_SIZE );
        spl_mcu_enable_irq();
        return u8tmp;
    }
#endif

    return 0;

}

extern uint8_t  spl_uart_txd_full  ( uint8_t port )
{
    //pending mode, never full
    port = port;
    return 0;
}

extern uint8_t  spl_uart_rxd_empty ( uint8_t port )
{
    uint8_t u8tmp;
#if SPL_UART0_EN > 0
    if( port == SPL_UART_PORT_0 )
    {
        spl_mcu_disable_irq();
        u8tmp = FIFO_BUF_EMPTY( uart0_rx_head, uart0_rx_tail );
        spl_mcu_enable_irq();
        return u8tmp;
    }
#endif

#if SPL_UART1_EN > 0
    if( port == SPL_UART_PORT_1 )
    {
        spl_mcu_disable_irq();
        u8tmp = FIFO_BUF_EMPTY( uart1_rx_head, uart1_rx_tail );
        spl_mcu_enable_irq();
        return u8tmp;
    }
#endif

    return 0;
}

extern void spl_uart_close( uint8_t port )
{
#if SPL_UART0_EN > 0
    if( port == SPL_UART_PORT_0 )
    {
        clr_TR3;
        clr_TI;
        clr_ES;
        return;
    }
#endif

#if SPL_UART1_EN > 0
    if(port == SPL_UART_PORT_1)
    {
        return;
    }
#endif
}

extern void spl_uart_deinit( uint8_t port )
{
#if SPL_UART0_EN > 0
    if( port == SPL_UART_PORT_0 )
    {
        FIFO_BUF_FLUSH( uart0_rx_head, uart0_rx_tail );
        return;
    }
#endif

#if SPL_UART1_EN > 0
    if( port == SPL_UART_PORT_1 )
    {
        FIFO_BUF_FLUSH( uart1_rx_head, uart1_rx_tail );
        return;
    }
#endif
}

#if SPL_UART0_EN > 0
extern void spl_uart0_isr( void )
{
	uint8_t u8tmp;
    
    if( RI )
    {
        clr_RI;
        /* Get the character from UART Buffer */
        u8tmp = SBUF;
        if( !FIFO_BUF_FULL(uart0_rx_head, uart0_rx_tail, SPL_UART0_RX_CACHE_SIZE) )
        {
            FIFO_BUF_PUT( u8tmp, uart0_rx_head, uart0_rx_cache, SPL_UART0_RX_CACHE_SIZE );
            SPL_UART0_CALLBACK( SPL_UART_ISR_EVT_RXD );
        }
        else
        {
            FIFO_BUF_FLUSH( uart0_rx_head, uart0_rx_tail );
            SPL_UART0_CALLBACK( SPL_UART_ISR_EVT_RXD_FULL );
        }   
    }
    
    
    if( TI )
    {
        clr_ES;
    }
}
#endif

#if SPL_UART1_EN > 0
extern void spl_uart1_isr( void )
{
	uint8_t u8tmp;
    
    if( RI )
    {
        clr_RI;
        /* Get the character from UART Buffer */
        u8tmp = SBUF;
        if( !FIFO_BUF_FULL(uart1_rx_head, uart1_rx_tail, SPL_UART1_RX_CACHE_SIZE) )
        {
            FIFO_BUF_PUT( u8tmp, uart1_rx_head, uart1_rx_cache, SPL_UART1_RX_CACHE_SIZE );
            SPL_UART1_CALLBACK( SPL_UART_ISR_EVT_RXD );
        }
        else
        {
            FIFO_BUF_FLUSH( uart1_rx_head, uart1_rx_tail );
            SPL_UART1_CALLBACK( SPL_UART_ISR_EVT_RXD_FULL );
        }   
    }
    
    
    if( TI )
    {
        clr_ES;
    }
}
#endif
#endif

