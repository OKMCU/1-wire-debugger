/*************************************************************************************************

 @file  hal_owbus.c

 @brief 1-wire bus API implementations.

 Group: 
 Target Device: 

 *************************************************************************************************
 

 *************************************************************************************************
 Release Name: 
 Release Date: 
 *************************************************************************************************/


/*************************************************************************************************
 * Includes
 *************************************************************************************************
 */
#include "osal.h"
#include "spl.h"

#include "hal_config.h"
#include "hal_assert.h"
#include "hal_owbus.h"

#define OW_GPIO             SPL_GPIO_PIN_P30

#define DLY_TIME_STD_RSTL   (UINT16_MAX-(uint16_t)(500L*SPL_SYSCLK/1000000L))  // 500us, 480-640us, Reset-Low Time
#define DLY_TIME_STD_MSP    (UINT16_MAX-(uint16_t)( 65L*SPL_SYSCLK/1000000L))  //  65us,  60-75us,  Presence-Detect Sampling Time



/*************************************************************************************************
 * Local Prototypes
 *************************************************************************************************
 */

extern void    hal_owbus_init( void )
{
    
    set_T0M;                                    // Use Fsys as Timer 0 input clock
    clr_CT_T0;                                  // Use Fsys as Timer 0 input clock
    clr_GATE_T0;                                // TR0 controls Timer 0 run/stop, regardless of INT0 logic level
    clr_TR0;                                    // Stop Timer 0
    TL0 = 0x00;                                 // Clear Timer 0
    TH0 = 0x00;                                 // Clear Timer 0

    SPL_GPIO_SET_MODE_P30_QUASI();              // P3.0 is 1-wire port
    spl_gpio_write_pin( OW_GPIO, 1 );           // Output 1 by default
}

extern int8_t  hal_owbus_std_reset( void )
{
    clr_TR0;
    clr_TF0;
    TH0 = HI_UINT16( DLY_TIME_STD_RSTL );
    TL0 = LO_UINT16( DLY_TIME_STD_RSTL );
    spl_gpio_write_pin( OW_GPIO, 0 );
    set_TR0;
    while( !TF0 );
    spl_gpio_write_pin( OW_GPIO, 1 );
    
    clr_TR0;
    clr_TF0;
    TH0 = HI_UINT16( DLY_TIME_STD_MSP );
    TL0 = LO_UINT16( DLY_TIME_STD_MSP );
    set_TR0;
    while( !TF0 );

    if( spl_gpio_read_pin(OW_GPIO) )
    {
        return -HAL_OWBUS_ERR_NOPRS;
    }

    clr_TR0;
    clr_TF0;
    while( !spl_gpio_read_pin(OW_GPIO) );

    return HAL_OWBUS_ERR_NONE;
}

extern void    hal_owbus_std_write( const uint8_t *p_buf, uint16_t len )
{
    
}

extern void    hal_owbus_std_read( uint8_t *p_buf, uint16_t len )
{
    
}

extern int8_t  hal_owbus_ovd_reset( void )
{
    
}

extern void    hal_owbus_ovd_write( const uint8_t *p_buf, uint16_t len )
{
    
}

extern void    hal_owbus_ovd_read( uint8_t *p_buf, uint16_t len )
{
    
}

/**************************************************************************************************
*/
