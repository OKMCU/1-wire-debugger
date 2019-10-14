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
 
#ifndef __HAL_CLI_H__
#define __HAL_CLI_H__


#include "stdint.h"
#include "hal_config.h"

extern void hal_cli_init         ( void );
extern uint8_t hal_cli_rx_len    ( void );
extern void hal_cli_putchar      ( char c );
extern char hal_cli_getchar      ( void );
#if (HAL_CLI_PRINT_EN > 0)
extern void hal_cli_print_str    ( const char *s );
extern void hal_cli_print_sint   ( int32_t num );
extern void hal_cli_print_uint   ( uint32_t num );
extern void hal_cli_print_hex8   ( uint8_t num );
extern void hal_cli_print_hex16  ( uint16_t num );
extern void hal_cli_print_hex32  ( uint32_t num );
#endif

#endif /* __HAL_CLI_H__ */

