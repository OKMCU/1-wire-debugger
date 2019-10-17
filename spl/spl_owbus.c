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
#include "spl_owbus.h"

#define OW_GPIO             SPL_GPIO_PIN_P30
#define OW_GPIO_DBG         SPL_GPIO_PIN_P05

#define DLY_TIME_STD_RSTL   0xDF93 //(UINT16_MAX-(uint16_t)(500L*SPL_SYSCLK/1000000L))  // 500us, 480~640us, Reset-Low Time
#define DLY_TIME_STD_PDMSP  (UINT16_MAX-(uint16_t)( 65L*SPL_SYSCLK/1000000L))  //  65us,  60~75us,  Presence-Detect Sampling Time
#define DLY_TIME_STD_PDL    (UINT16_MAX-(uint16_t)(240L*SPL_SYSCLK/1000000L))  // 240us,  60~240us, Presence-Detect Low Time (Max)
#define DLY_TIME_STD_RDL    (UINT16_MAX-(uint16_t)(  5L*SPL_SYSCLK/1000000L))  //   5us,  5~(15-X)us, Read-Low Time (Min)
#define DLY_TIME_STD_RDH    (UINT16_MAX-(uint16_t)( 95L*SPL_SYSCLK/1000000L))  //  95us,  (tRL+X)~15us, Read Sampling Time (Max)
#define DLY_TIME_STD_W0L    (UINT16_MAX-(uint16_t)( 80L*SPL_SYSCLK/1000000L))  //  80us,  W0L+W0H <= 120, Write 0 low time
#define DLY_TIME_STD_W0H    (UINT16_MAX-(uint16_t)( 20L*SPL_SYSCLK/1000000L))  //  20us,  W0L+W0H <= 120, Write 0 high time
#define DLY_TIME_STD_W1L    (UINT16_MAX-(uint16_t)(  5L*SPL_SYSCLK/1000000L))  //   5us,  1~15us, W1L+W1H <= 120, Write 1 low time
#define DLY_TIME_STD_W1H    (UINT16_MAX-(uint16_t)( 95L*SPL_SYSCLK/1000000L))  //  95us,  W1L+W1H <= 120, Write 1 high time

/*************************************************************************************************
 * Local Prototypes
 *************************************************************************************************
 */

extern void    spl_owbus_init( void )
{
    TMOD &= 0xF0;
    TMOD |= 0x01;
    set_T0M;                                    // Use Fsys as Timer 0 input clock
    //clr_CT_T0;                                // Use Fsys as Timer 0 input clock
    //clr_GATE_T0;                              // TR0 controls Timer 0 run/stop, regardless of INT0 logic level
    clr_TR0;                                    // Stop Timer 0
    TL0 = 0x00;                                 // Clear Timer 0
    TH0 = 0x00;                                 // Clear Timer 0

    SPL_GPIO_SET_MODE_P30_QUASI();              // P3.0 is 1-wire port
    SPL_GPIO_SET_MODE_P05_QUASI();              // P0.5 is debug port
    spl_gpio_write_pin( OW_GPIO, 1 );           // Output 1 by default
}

extern int8_t  spl_owbus_std_reset( void )
{
    // pull 1-wire bus low for reset
    clr_TR0;
    clr_TF0;
    TH0 = HI_UINT16( DLY_TIME_STD_RSTL );
    TL0 = LO_UINT16( DLY_TIME_STD_RSTL );
    spl_gpio_write_pin( OW_GPIO, 0 );
    set_TR0;
    while( !TF0 );

    // set sampling presence-detection timeout
    clr_TR0;
    clr_TF0;
    TH0 = HI_UINT16( DLY_TIME_STD_PDMSP );
    TL0 = LO_UINT16( DLY_TIME_STD_PDMSP );
    set_TR0;
    
    // release 1-wire bus
    spl_gpio_write_pin( OW_GPIO, 1 );

    // wait for bus pull high
    while( !TF0 && spl_gpio_read_pin(OW_GPIO) == 0 );
    if( TF0 ) return -SPL_OWBUS_ERR_BUSERR;

    // wait for bus pulled low by slave device
    while( !TF0 && spl_gpio_read_pin(OW_GPIO) );
    if( TF0 ) return -SPL_OWBUS_ERR_NOPRS;

    // wait for slave release 1-wire bus
    clr_TR0;
    clr_TF0;
    TH0 = HI_UINT16( DLY_TIME_STD_PDL );
    TL0 = LO_UINT16( DLY_TIME_STD_PDL );
    set_TR0;
    while( !TF0 && spl_gpio_read_pin(OW_GPIO) == 0 );
    if( TF0 ) return -SPL_OWBUS_ERR_BUSERR;

    clr_TR0;
    clr_TF0;

    return SPL_OWBUS_ERR_NONE;
}

extern void    spl_owbus_std_write( const uint8_t *p_buf, uint16_t len )
{
    uint16_t i;
    uint8_t j;
    uint8_t byte;
    
    for( i = 0; i < len; i++ )
    {
        byte = p_buf[i];
        for( j = 0; j < 8; j++ )
        {
            if( byte & 0x01 )
            {
                clr_TR0;
                clr_TF0;
                TH0 = HI_UINT16( DLY_TIME_STD_W1L );
                TL0 = LO_UINT16( DLY_TIME_STD_W1L );
                spl_gpio_write_pin( OW_GPIO, 0 );
                set_TR0;
                while( !TF0 );
                
                clr_TR0;
                clr_TF0;
                TH0 = HI_UINT16( DLY_TIME_STD_W1H );
                TL0 = LO_UINT16( DLY_TIME_STD_W1H );
                spl_gpio_write_pin( OW_GPIO, 1 );
                set_TR0;
                while( !TF0 );
            }
            else
            {
                clr_TR0;
                clr_TF0;
                TH0 = HI_UINT16( DLY_TIME_STD_W0L );
                TL0 = LO_UINT16( DLY_TIME_STD_W0L );
                spl_gpio_write_pin( OW_GPIO, 0 );
                set_TR0;
                while( !TF0 );
                
                clr_TR0;
                clr_TF0;
                TH0 = HI_UINT16( DLY_TIME_STD_W0H );
                TL0 = LO_UINT16( DLY_TIME_STD_W0H );
                spl_gpio_write_pin( OW_GPIO, 1 );
                set_TR0;
                while( !TF0 );
            }
            byte >>= 1;
        }
    }
}

extern void    spl_owbus_std_read( uint8_t *p_buf, uint16_t len )
{
    uint16_t i;
    uint8_t j;
    uint8_t byte;

    for( i = 0; i < len; i++ )
    {
        byte = 0;
        for( j = 0; j < 8; j++ )
        {
            clr_TR0;
            clr_TF0;
            TH0 = HI_UINT16( DLY_TIME_STD_RDL );
            TL0 = LO_UINT16( DLY_TIME_STD_RDL );
            spl_gpio_write_pin( OW_GPIO, 0 );
            set_TR0;
            while( !TF0 );
            spl_gpio_write_pin( OW_GPIO, 1 );

            clr_TR0;
            clr_TF0;
            TH0 = HI_UINT16( DLY_TIME_STD_RDH );
            TL0 = LO_UINT16( DLY_TIME_STD_RDH );
            set_TR0;
            nop();nop();nop();
            spl_gpio_toggle_pin( OW_GPIO_DBG );
            byte >>= 1;
            if( spl_gpio_read_pin(OW_GPIO) )  byte |= 0x80;
            while( !TF0 );
        }
        p_buf[len-i-1] = byte;
    }
}

//extern int8_t  spl_owbus_ovd_reset( void )
//{
//    
//}
//
//extern void    spl_owbus_ovd_write( const uint8_t *p_buf, uint16_t len )
//{
//    
//}
//
//extern void    spl_owbus_ovd_read( uint8_t *p_buf, uint16_t len )
//{
//    
//}

/**************************************************************************************************
*/
