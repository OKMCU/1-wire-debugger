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
#ifndef __SPL_I2C_H__
#define __SPL_I2C_H__


#include <stdint.h>

#define SPL_I2C_ERR_NONE    0
#define SPL_I2C_ERR_NACK    1   //No ACK
#define SPL_I2C_ERR_ARB     2   //Arbitration Lost
#define SPL_I2C_ERR_BE      3   //Bus error


extern void     spl_i2c_init( void );
extern void     spl_i2c_open( void );
extern uint8_t  spl_i2c_busy( void );
extern uint8_t  spl_i2c_start( void );
extern uint8_t  spl_i2c_restart( void );
extern uint8_t  spl_i2c_wr_addr( uint8_t addr );
extern uint8_t  spl_i2c_wr_data( uint8_t byte );
extern uint8_t  spl_i2c_rd_data( uint8_t ack, uint8_t *p_data );
extern uint8_t  spl_i2c_stop( void );
extern void     spl_i2c_close( void );
extern void     spl_i2c_deinit( void );

#endif /* __SPL_I2C_H__ */

