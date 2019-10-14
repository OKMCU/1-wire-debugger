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

#ifndef __SPL_MCU_H__
#define __SPL_MCU_H__

#include "Common.h"
#include "Function_Define.h"
#include "N76E003.h"
#include "SFR_Macro.h"

#include "stdmacro.h"

#define spl_mcu_disable_irq()    EA = 0
#define spl_mcu_enable_irq()     EA = 1
#define spl_mcu_reset()          st( TA = 0xAA; TA = 0x55; EA = 0; TA=0xAA; TA=0x55; CHPCON|=SET_BIT7; )
#define spl_mcu_delayus(us)      

extern void spl_mcu_init( void );

#endif

