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
#ifndef __SPL_UART_H__
#define __SPL_UART_H__


#include "stdint.h"

#define     SPL_UART_PORT_0                 0
#define     SPL_UART_PORT_1                 1

#define     SPL_UART_ISR_EVT_RXD            1
#define     SPL_UART_ISR_EVT_RXD_FULL       2
//#define     SPL_UART_ISR_EVT_TXD            3
//#define     SPL_UART_ISR_EVT_TXD_EMPTY      4

extern void     spl_uart_init      ( uint8_t port );
extern void     spl_uart_open      ( uint8_t port );
extern void     spl_uart_txd       ( uint8_t port, uint8_t tx_byte );
extern uint8_t  spl_uart_rxd       ( uint8_t port );
extern uint8_t  spl_uart_txd_full  ( uint8_t port );
extern uint8_t  spl_uart_rxd_empty ( uint8_t port );
extern void     spl_uart_close     ( uint8_t port );
extern void     spl_uart_deinit    ( uint8_t port );


#endif /* __SPL_UART_H__ */

