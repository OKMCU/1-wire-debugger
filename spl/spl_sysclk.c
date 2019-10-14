/******************************************************************************

 @file  spl_sysclk.c

 @brief 

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
#include "spl_sysclk.h"

#include "stdint.h"

#if (SPL_SYSCLK_EN > 0)
/**************************************************************************************************
 * CONSTANTS
 **************************************************************************************************/

/**************************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************************/
#if (SPL_SYSCLK_TRIM_EN > 0)
static xdata int8_t hirc_trim_value;
static xdata int16_t hircmap_fac_default;
#endif

#if (SPL_SYSCLK_TRIM_BASE != 0 || SPL_SYSCLK_TRIM_EN > 0)
static int16_t spl_systick_get_hircmap( void )
{
    uint8_t hircmap0;
    uint8_t hircmap1;

    //BIT_TMP = EA; EA = 0;
    TA=0xAA;
    TA=0x55;
    CHPCON|=SET_BIT0;
    IAPAL = 0x30;
    IAPAH = 0x00;
    IAPCN = READ_UID;
    TA=0xAA;
    TA=0x55;
    IAPTRG|=SET_BIT0;
    hircmap0 = IAPFD;
    IAPAL = 0x31;
    IAPAH = 0x00;
    TA=0xAA;
    TA=0x55;
    IAPTRG|=SET_BIT0 ;
    hircmap1 = IAPFD;
    TA=0xAA;
    TA=0x55;
    CHPCON&=~SET_BIT0;
    //EA = BIT_TMP;

    return ((hircmap0<<1) | (hircmap1&0x01));
}

static void spl_sysclk_set_rctrim( int16_t rctrim )
{
    uint8_t data rctrim0;
    uint8_t data rctrim1;

    rctrim &= 0x01FF;   //9bit
    rctrim0 = rctrim>>1;
    rctrim1 = rctrim&0x01;
    
    TA=0XAA;
    TA=0X55;
    RCTRIM0 = rctrim0;
    TA=0XAA;
    TA=0X55;
    RCTRIM1 = rctrim1;
}
#endif //(SPL_SYSCLK_TRIM_BASE != 0 || SPL_SYSCLK_TRIM_EN > 0)

extern void   spl_sysclk_init( void )
{
#if (SPL_SYSCLK_TRIM_BASE != 0 || SPL_SYSCLK_TRIM_EN > 0)
    int16_t hircmap;
    hircmap = spl_systick_get_hircmap();
#endif

#if (SPL_SYSCLK_TRIM_BASE != 0)
    spl_sysclk_set_rctrim( hircmap - SPL_SYSCLK_TRIM_BASE );
#endif

#if (SPL_SYSCLK_TRIM_EN > 0)
    hircmap_fac_default = hircmap;
    hirc_trim_value = 0;
#endif
    
#if (SPL_SYSCLK_CLKO_EN > 0)
    P11 = SPL_SYSCLK_CLKO_DEF_LVL;
    P11_PushPull_Mode;
#endif
}

#if (SPL_SYSCLK_TRIM_EN > 0)
extern void spl_sysclk_set_hirc( int8_t trim )
{   
    spl_sysclk_set_rctrim( hircmap_fac_default - SPL_SYSCLK_TRIM_BASE - trim );
    hirc_trim_value = trim;
}

extern int8_t spl_sysclk_get_hirc( void )
{
    return hirc_trim_value;
}
#endif //(SPL_SYSCLK_TRIM_EN > 0)

#if (SPL_SYSCLK_CLKO_EN > 0)
extern void   spl_sysclk_output_enable( void )
{
    P11 = 1;
    set_CLOEN;
}

extern void   spl_sysclk_output_disable( void )
{
    P11 = SPL_SYSCLK_CLKO_DEF_LVL;
    clr_CLOEN;
}
#endif //(SPL_SYSCLK_CLKO_EN > 0)

#endif //(SPL_SYSCLK_EN > 0)

/**************************************************************************************************
**************************************************************************************************/

