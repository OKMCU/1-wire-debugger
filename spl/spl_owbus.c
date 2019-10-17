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

#define DLY_TIME_STD_RSTL   0xDF93                                             // 500us, 480~640us, Reset-Low Time
#define DLY_TIME_STD_PDMSP  (UINT16_MAX-(uint16_t)( 65L*SPL_SYSCLK/1000000L))  //  65us,  60~75us,  Presence-Detect Sampling Time
#define DLY_TIME_STD_PDL    (UINT16_MAX-(uint16_t)(240L*SPL_SYSCLK/1000000L))  // 240us,  60~240us, Presence-Detect Low Time (Max)
#define DLY_TIME_STD_RDL    (UINT16_MAX-(uint16_t)(  5L*SPL_SYSCLK/1000000L))  //   5us,  5~(15-X)us, Read-Low Time (Min)
#define DLY_TIME_STD_RDH    (UINT16_MAX-(uint16_t)( 95L*SPL_SYSCLK/1000000L))  //  95us,  (tRL+X)~15us, Read Sampling Time (Max)
#define DLY_TIME_STD_W0L    (UINT16_MAX-(uint16_t)( 80L*SPL_SYSCLK/1000000L))  //  80us,  W0L+W0H <= 120, Write 0 low time
#define DLY_TIME_STD_W0H    (UINT16_MAX-(uint16_t)( 20L*SPL_SYSCLK/1000000L))  //  20us,  W0L+W0H <= 120, Write 0 high time
#define DLY_TIME_STD_W1L    (UINT16_MAX-(uint16_t)(  5L*SPL_SYSCLK/1000000L))  //   5us,  1~15us, W1L+W1H <= 120, Write 1 low time
#define DLY_TIME_STD_W1H    (UINT16_MAX-(uint16_t)( 95L*SPL_SYSCLK/1000000L))  //  95us,  W1L+W1H <= 120, Write 1 high time

#define DLY_TIME_OVD_RSTL   (UINT16_MAX-(uint16_t)( 60L*SPL_SYSCLK/1000000L))  //  60us,  48~80us, Reset-Low Time
#define DLY_TIME_OVD_PDMSP  (UINT16_MAX-(uint16_t)( 10L*SPL_SYSCLK/1000000L))  //  10us,   6~30us, Presence-Detect Sampling Time
#define DLY_TIME_OVD_PDL    (UINT16_MAX-(uint16_t)( 24L*SPL_SYSCLK/1000000L))  //  24us,   8~24us, Presence-Detect Low Time (Max)
#define DLY_TIME_OVD_RDL    (UINT16_MAX-(uint16_t)(  1*SPL_SYSCLK/1000000L))   //   1us,  1~(2-X)us, Read-Low Time (Min)
#define DLY_TIME_OVD_RDH    (UINT16_MAX-(uint16_t)( 11L*SPL_SYSCLK/1000000L))  //  11us,  (tRL+X)~2us, Read Sampling Time (Max)
#define DLY_TIME_OVD_W0L    (UINT16_MAX-(uint16_t)(  8L*SPL_SYSCLK/1000000L))  //   8us,  W0L+W0H <= 16, Write 0 low time
#define DLY_TIME_OVD_W0H    (UINT16_MAX-(uint16_t)(  4L*SPL_SYSCLK/1000000L))  //   4us,  W0L+W0H <= 16, Write 0 high time
#define DLY_TIME_OVD_W1L    (UINT16_MAX-(uint16_t)(  1L*SPL_SYSCLK/1000000L))  //   1us,  1~2us, W1L+W1H <= 16, Write 1 low time
#define DLY_TIME_OVD_W1H    (UINT16_MAX-(uint16_t)( 11L*SPL_SYSCLK/1000000L))  //  11us,  W1L+W1H <= 16, Write 1 high time

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
    if( TF0 ) return -SPL_OWBUS_ERR_BUS_BUSY;

    // wait for bus pulled low by slave device
    while( !TF0 && spl_gpio_read_pin(OW_GPIO) );
    if( TF0 ) return -SPL_OWBUS_ERR_NO_PRS;

    // wait for slave release 1-wire bus
    clr_TR0;
    clr_TF0;
    TH0 = HI_UINT16( DLY_TIME_STD_PDL );
    TL0 = LO_UINT16( DLY_TIME_STD_PDL );
    set_TR0;
    while( !TF0 && spl_gpio_read_pin(OW_GPIO) == 0 );
    if( TF0 ) return -SPL_OWBUS_ERR_BUS_BUSY;

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
            byte >>= 1;
            if( spl_gpio_read_pin(OW_GPIO) )  byte |= 0x80;
            while( !TF0 );
        }
        p_buf[i] = byte;
    }
}

extern int8_t  spl_owbus_ovd_reset( void )
{
    // pull 1-wire bus low for reset
    clr_TR0;
    clr_TF0;
    TH0 = HI_UINT16( DLY_TIME_OVD_RSTL );
    TL0 = LO_UINT16( DLY_TIME_OVD_RSTL );
    spl_gpio_write_pin( OW_GPIO, 0 );
    set_TR0;
    while( !TF0 );

    // set sampling presence-detection timeout
    clr_TR0;
    clr_TF0;
    TH0 = HI_UINT16( DLY_TIME_OVD_PDMSP );
    TL0 = LO_UINT16( DLY_TIME_OVD_PDMSP );
    set_TR0;
    
    // release 1-wire bus
    spl_gpio_write_pin( OW_GPIO, 1 );

    // wait for bus pull high
    while( !TF0 && spl_gpio_read_pin(OW_GPIO) == 0 );
    if( TF0 ) return -SPL_OWBUS_ERR_BUS_BUSY;

    // wait for bus pulled low by slave device
    while( !TF0 && spl_gpio_read_pin(OW_GPIO) );
    if( TF0 ) return -SPL_OWBUS_ERR_NO_PRS;

    // wait for slave release 1-wire bus
    clr_TR0;
    clr_TF0;
    TH0 = HI_UINT16( DLY_TIME_OVD_PDL );
    TL0 = LO_UINT16( DLY_TIME_OVD_PDL );
    set_TR0;
    while( !TF0 && spl_gpio_read_pin(OW_GPIO) == 0 );
    if( TF0 ) return -SPL_OWBUS_ERR_BUS_BUSY;

    clr_TR0;
    clr_TF0;

    return SPL_OWBUS_ERR_NONE;

}

extern void    spl_owbus_ovd_write( const uint8_t *p_buf, uint16_t len )
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
                TH0 = HI_UINT16( DLY_TIME_OVD_W1L );
                TL0 = LO_UINT16( DLY_TIME_OVD_W1L );
                spl_gpio_write_pin( OW_GPIO, 0 );
                set_TR0;
                while( !TF0 );
                
                clr_TR0;
                clr_TF0;
                TH0 = HI_UINT16( DLY_TIME_OVD_W1H );
                TL0 = LO_UINT16( DLY_TIME_OVD_W1H );
                spl_gpio_write_pin( OW_GPIO, 1 );
                set_TR0;
                while( !TF0 );
            }
            else
            {
                clr_TR0;
                clr_TF0;
                TH0 = HI_UINT16( DLY_TIME_OVD_W0L );
                TL0 = LO_UINT16( DLY_TIME_OVD_W0L );
                spl_gpio_write_pin( OW_GPIO, 0 );
                set_TR0;
                while( !TF0 );
                
                clr_TR0;
                clr_TF0;
                TH0 = HI_UINT16( DLY_TIME_OVD_W0H );
                TL0 = LO_UINT16( DLY_TIME_OVD_W0H );
                spl_gpio_write_pin( OW_GPIO, 1 );
                set_TR0;
                while( !TF0 );
            }
            byte >>= 1;
        }
    }

}

extern void    spl_owbus_ovd_read( uint8_t *p_buf, uint16_t len )
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
            TH0 = HI_UINT16( DLY_TIME_OVD_RDL );
            TL0 = LO_UINT16( DLY_TIME_OVD_RDL );
            spl_gpio_write_pin( OW_GPIO, 0 );
            set_TR0;
            while( !TF0 );
            spl_gpio_write_pin( OW_GPIO, 1 );

            clr_TR0;
            clr_TF0;
            TH0 = HI_UINT16( DLY_TIME_OVD_RDH );
            TL0 = LO_UINT16( DLY_TIME_OVD_RDH );
            set_TR0;
            nop();nop();nop();
            byte >>= 1;
            if( spl_gpio_read_pin(OW_GPIO) )  byte |= 0x80;
            while( !TF0 );
        }
        p_buf[i] = byte;
    }

}

/**************************************************************************************************
*/
