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

#ifndef __SPL_EXTINT_H__
#define __SPL_EXTINT_H__


#define SPL_EXTINT_TRIG_NONE    0x00
#define SPL_EXTINT_TRIG_RISE    0x01
#define SPL_EXTINT_TRIG_FALL    0x02

#define SPL_EXTINT_INT_0        0
#define SPL_EXTINT_INT_1        1

extern void spl_extint_init( void );
extern void spl_extint_config( uint8_t int_x, uint8_t trig );
extern void spl_extint_deinit( void );

#endif

