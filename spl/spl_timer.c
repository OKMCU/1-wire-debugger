/******************************************************************************

 @file  spl_mcu.c

 @brief This file contains the interface to the Drivers Service.

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 
 *****************************************************************************/

/**************************************************************************************************
 *                                            INCLUDES
 **************************************************************************************************/
#include "Common.h"
#include "Function_Define.h"
#include "N76E003.h"
#include "SFR_Macro.h"
#include "spl_config.h"
#include "spl_timer.h"
#include "spl_sysclk.h"

#if SPL_TIMER_EN > 0
/**************************************************************************************************
 *                                         CONSTANTS
 **************************************************************************************************/

/**************************************************************************************************
 *                                      GLOBAL VARIABLES
 **************************************************************************************************/

/**************************************************************************************************
 *                                      FUNCTIONS
 **************************************************************************************************/
extern void spl_timer_init( void )
{
#if SPL_TIMER_SYSTICK_EN > 0
    uint16_t reload;
#endif

#if SPL_TIMER_SYSTICK_EN > 0
#if SPL_SYSCLK_TRIM_EN > 0
    int32_t s32tmp;
    s32tmp = (int32_t)SPL_SYSCLK + (int32_t)spl_sysclk_get_hirc()*40000;    //calculate accurate SYS_CLOCK
    reload = UINT16_MAX - (s32tmp/SPL_TIMER_SYSTICK_FREQ) + 1;
#else
    reload = UINT16_MAX - (SPL_SYSCLK/SPL_TIMER_SYSTICK_FREQ) + 1;
#endif
    
    TIMER2_DIV_1;
    TIMER2_Auto_Reload_Delay_Mode;

    RCMP2L = (uint8_t)(reload & 0xFF);
    RCMP2H = (uint8_t)(reload >> 8);
    TL2 = 0;
    TH2 = 0;

    set_ET2;                                    // Enable Timer2 interrupt
    set_TR2;                                    // Timer2 run
#endif//SPL_TIMER_SYSTICK_EN > 0
}

extern void spl_timer_deinit( void )
{
#if SPL_TIMER_SYSTICK_EN > 0
    clr_TR2;
    clr_ET2;
#endif
}

#endif //SPL_TIMER_EN > 0
/**************************************************************************************************
**************************************************************************************************/

