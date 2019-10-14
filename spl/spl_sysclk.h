/******************************************************************************

 @file  spl_sysclk.h

 @brief 

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 18-DEC-12 16:00:00
 *****************************************************************************/

#ifndef __SPL_SYSCLK_H__
#define __SPL_SYSCLK_H__

#include "stdint.h"
#include "spl_config.h"

#if (SPL_SYSCLK_EN > 0)
extern void   spl_sysclk_init( void );

#if (SPL_SYSCLK_TRIM_EN > 0)
extern void     spl_sysclk_set_hirc( int8_t trim );
extern int8_t   spl_sysclk_get_hirc( void );
#endif //(SPL_SYSCLK_TRIM_EN > 0)

#if (SPL_SYSCLK_CLKO_EN > 0)
extern void   spl_sysclk_output_enable( void );
extern void   spl_sysclk_output_disable( void );
#endif //(SPL_SYSCLK_CLKO_EN > 0)

#endif //(SPL_SYSCLK_EN > 0)

#endif //__SPL_SYSCLK_H__

