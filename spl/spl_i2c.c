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
#include "spl_i2c.h"

#include "bufmgr.h"

#if (SPL_I2C_EN > 0)

extern void     spl_i2c_init( void )
{
    P13_OpenDrain_Mode;					// Modify SCL pin to Open drain mode. don't forget the pull high resister in circuit
    P14_OpenDrain_Mode;					// Modify SDA pin to Open drain mode. don't forget the pull high resister in circuit
    P13 = 1;
    P14 = 1;
}

extern void     spl_i2c_open( void )
{
    /* Set I2C clock rate */
    I2CLK = SPL_SYSCLK/SPL_I2C_CLOCK/4 - 1;
    /* Enable I2C */
    set_I2CEN;
}

extern uint8_t  spl_i2c_busy( void )
{
    return !(I2STAT == 0xF8);
}

#if (SPL_I2C_MASTER_EN > 0)
extern uint8_t  spl_i2c_start( void )
{
    set_STA;
    clr_SI;
    while (!SI);
    if (I2STAT == 0x08)
        return SPL_I2C_ERR_NONE;
    return SPL_I2C_ERR_BE;
}


extern uint8_t  spl_i2c_restart( void )
{
    set_STA;
    clr_SI;
    while (!SI);
    if (I2STAT == 0x10)
        return SPL_I2C_ERR_NONE;
    return SPL_I2C_ERR_BE;
}

extern uint8_t  spl_i2c_wr_addr( uint8_t addr )
{
    uint8_t status;

    I2DAT = addr;
    clr_STA;                                /* Clear STA and Keep SI value in I2CON */
    clr_SI;
    while (!SI);
    
    status = I2STAT;
    
    if(addr & 0x01)
    {
        if(status == 0x40)
            return SPL_I2C_ERR_NONE;
        if(status == 0x48)
            return SPL_I2C_ERR_NACK;
    }
    else
    {
        if(status == 0x18)
            return SPL_I2C_ERR_NONE;
        if(status == 0x20)
            return SPL_I2C_ERR_NACK;
    }

    if(status == 0x38 ||
       status == 0x68 ||
       status == 0x78 ||
       status == 0xB0)
            return SPL_I2C_ERR_ARB;

    return SPL_I2C_ERR_BE;
}

extern uint8_t  spl_i2c_wr_data( uint8_t byte )
{
    uint8_t status;

    I2DAT = byte;
    clr_SI;
    while (!SI);
    
    status = I2STAT;
    
    if(status == 0x28)
        return SPL_I2C_ERR_NONE;
    if(status == 0x30)
        return SPL_I2C_ERR_NACK;
    if(status == 0x38)
        return SPL_I2C_ERR_ARB;

    return SPL_I2C_ERR_BE;
}

extern uint8_t  spl_i2c_rd_data( uint8_t ack, uint8_t *p_data )
{
    uint8_t status;
    uint8_t byte;
    
    if(ack)
    {
        set_AA;                             /* Set Assert Acknowledge Control Bit */
    }
    else
    {
        clr_AA;
    }
    clr_SI;
    
    while (!SI);
    status = I2STAT;
    byte = I2DAT;
    
    if(p_data)
        *p_data = byte;
    
    if(ack)
    {
        if(status == 0x50)
            return SPL_I2C_ERR_NONE;
    }
    else
    {
        if(status == 0x58)
            return SPL_I2C_ERR_NONE;
    }
    if(status == 0x38)
        return SPL_I2C_ERR_ARB;

    return SPL_I2C_ERR_BE;
}

extern uint8_t  spl_i2c_stop( void )
{
    uint8_t status;

    clr_SI;
    set_STO;

    while(STO)
    {
        if(I2STAT == 0x00)
            return SPL_I2C_ERR_BE;
    }
    
    status = I2STAT;
    
    if(status == 0xF8)
        return SPL_I2C_ERR_NONE;

    return SPL_I2C_ERR_BE;
}
#endif

extern void     spl_i2c_close( void )
{
    clr_I2CEN;
    clr_SI;
    clr_STO;
    clr_STA;
    clr_AA;
}

extern void     spl_i2c_deinit( void )
{
    P13_Quasi_Mode;
    P14_Quasi_Mode;
}

#if (SPL_I2C_SLAVE_EN > 0)
extern void spl_i2c_isr  ( void )
{
    uint32_t u32Status;
    
    u32Status = I2C_GET_STATUS( I2C0 );

    if( I2C_GET_TIMEOUT_FLAG( I2C0 ) )
    {
        /* Clear I2C0 Timeout Flag */
        I2C_ClearTimeoutFlag( I2C0 );
    }
    else
    {
        if( u32Status == 0x60 )                       /* Own SLA+W has been receive; ACK has been return */
        {
            SPL_I2C0_CALLBACK_ADDRW();
            I2C_SET_CONTROL_REG( I2C0, I2C_I2CON_SI_AA );
        }
        else if( u32Status == 0x80 )                 /* Previously address with own SLA address
                                                       Data has been received; ACK has been returned*/
        {
            SPL_I2C0_CALLBACK_RXD((uint8_t)I2C_GET_DATA(I2C0));
            I2C_SET_CONTROL_REG( I2C0, I2C_I2CON_SI_AA );
        }
        else if( u32Status == 0xA8 )                  /* Own SLA+R has been receive; ACK has been return */
        {
            SPL_I2C0_CALLBACK_ADDRR();
            I2C_SET_DATA(I2C0, SPL_I2C0_CALLBACK_TXD()); //put the first byte into the buffer
            I2C_SET_CONTROL_REG( I2C0, I2C_I2CON_SI_AA );
        }
        else if( u32Status == 0xB8 )                 /* Data byte or last data in I2CDAT has been transmitted
                                                        ACK has been received */
        {
            I2C_SET_DATA(I2C0, SPL_I2C0_CALLBACK_TXD());//previous byte is transmitted successfully, 
                                                        //put the next byte into the buffer
            I2C_SET_CONTROL_REG( I2C0, I2C_I2CON_SI_AA );
        }
        else if( u32Status == 0xC0 )                 /* Data byte or last data in I2CDAT has been transmitted
                                                       Not ACK has been received */
        {
            I2C_SET_CONTROL_REG( I2C0, I2C_I2CON_SI_AA );
        }
        else if( u32Status == 0x88 )                 /* Previously addressed with own SLA address; NOT ACK has
                                                       been returned */
        {
            I2C_SET_CONTROL_REG( I2C0, I2C_I2CON_SI_AA );
        }
        else if( u32Status == 0xA0 )                 /* A STOP or repeated START has been received while still
                                                       addressed as Slave/Receiver*/
        {
            SPL_I2C0_CALLBACK_FRMEND();
            I2C_SET_CONTROL_REG( I2C0, I2C_I2CON_SI_AA );
        }
        else
        {
            /* TO DO */
            //HAL_ASSERT_FORCED();
        }
    }
}
#endif
#endif /* (SPL_I2C_EN > 0) */

