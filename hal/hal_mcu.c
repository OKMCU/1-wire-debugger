/******************************************************************************

 @file  hal.c

 @brief Describe the purpose and contents of the file.

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 
 *****************************************************************************/


/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "osal.h"
#include "spl.h"

#include "hal_config.h"
#include "hal_assert.h"
#include "hal_mcu.h"

/* ------------------------------------------------------------------------------------------------
 *                                       Local Prototypes
 * ------------------------------------------------------------------------------------------------
 */

extern void hal_mcu_init( void )
{
    spl_mcu_init();

#if (SPL_SYSCLK_EN > 0)
    spl_sysclk_init();
#if (SPL_SYSCLK_CLKO_EN > 0)
    spl_sysclk_output_enable();
#endif
#endif

#if (SPL_UART0_EN > 0)
    spl_uart_init( SPL_UART_PORT_0 );
    spl_uart_open( SPL_UART_PORT_0 );
#endif

#if (SPL_UART1_EN > 0)
    spl_uart_init( SPL_UART_PORT_1 );
    spl_uart_open( SPL_UART_PORT_1 );
#endif

#if (SPL_I2C_MASTER_EN > 0 || SPL_I2C_SLAVE_EN > 0)
    spl_i2c_init();
    spl_i2c_open();
#endif

#if (SPL_EXTINT_EN > 0)
    spl_extint_init();
#endif

#if (SPL_TIMER_EN > 0)
    spl_timer_init();
#endif

#if (SPL_ADC_EN > 0)
    spl_adc_init();
#endif
}

#if HAL_MCU_RESET_EN > 0
extern void hal_mcu_reset( void )
{
    spl_mcu_reset();
}
#endif //HAL_MCU_RESET_EN > 0

#if HAL_MCU_DELAYUS_EN > 0
extern void hal_mcu_delayus( uint32_t us)
{
    us = us;
    spl_mcu_delayus(us);
}
#endif //HAL_MCU_DELAYUS_EN > 0

#if HAL_MCU_HSI_TRIM_EN > 0
extern int8_t hal_mcu_hsi_trim_set( int8_t trim )
{
    if( trim > HAL_MCU_TRIM_MAX )
        trim = HAL_MCU_TRIM_MAX;
    else if( trim < HAL_MCU_TRIM_MIN )
        trim = HAL_MCU_TRIM_MIN;

#if (SPL_UART0_EN > 0)
    spl_uart_close( SPL_UART_PORT_0 );
    spl_uart_deinit( SPL_UART_PORT_0 );    
#endif

#if (SPL_UART1_EN > 0)
    spl_uart_close( SPL_UART_PORT_1 );
    spl_uart_deinit( SPL_UART_PORT_1 );    
#endif

#if (SPL_TIMER_EN > 0)
    spl_timer_deinit();
#endif

    spl_sysclk_set_hirc( trim );

#if (SPL_UART0_EN > 0)
    spl_uart_init( SPL_UART_PORT_0 );
    spl_uart_open( SPL_UART_PORT_0 );
#endif

#if (SPL_UART1_EN > 0)
    spl_uart_init( SPL_UART_PORT_1 );
    spl_uart_open( SPL_UART_PORT_1 );
#endif

#if (SPL_TIMER_EN > 0)
    spl_timer_init();
#endif

    return trim;
}

extern int8_t hal_mcu_hsi_trim_get( void )
{
    return spl_sysclk_get_hirc();
}
#endif //HAL_MCU_TRIM_EN > 0

/**************************************************************************************************
*/
