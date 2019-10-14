/******************************************************************************

 @file  spl_extint.c

 @brief This file contains the interface to the external interrupt service.

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
#include "M051Series.h"
#include "spl_config.h"
#include "spl_extint.h"

#if (SPL_EXTINT_EN > 0)
/**************************************************************************************************
 *                                         CONSTANTS
 **************************************************************************************************/

/**************************************************************************************************
 *                                      GLOBAL VARIABLES
 **************************************************************************************************/

#if (SPL_EXTINT0_EN > 0)
extern void SPL_EXTINT_INT0_CALLBACK(void);
#endif

#if (SPL_EXTINT1_EN > 0)
extern void SPL_EXTINT_INT1_CALLBACK(void);
#endif

extern void spl_extint_init( void )
{
#if (SPL_EXTINT0_EN > 0)
    NVIC_EnableIRQ(EINT0_IRQn);
#endif

#if (SPL_EXTINT1_EN > 0)
    NVIC_EnableIRQ(EINT1_IRQn);
#endif
}

extern void spl_extint_config( uint8_t pin, uint8_t trig )
{
    uint32_t int_attr = 0;

    switch( trig )
    {
        case SPL_EXTINT_TRIG_RISE:
            int_attr = GPIO_INT_RISING;
        break;

        case SPL_EXTINT_TRIG_FALL:
            int_attr = GPIO_INT_FALLING;
        break;

        case (SPL_EXTINT_TRIG_RISE + SPL_EXTINT_TRIG_FALL):
            int_attr = GPIO_INT_BOTH_EDGE;
        break;

        default:
            int_attr = 0;
        break;
    }
    
    switch (pin)
    {
#if (SPL_EXTINT0_EN > 0)
        case SPL_EXTINT_INT_0:
            if(int_attr)
                GPIO_EnableInt(P3, 2, int_attr);
            else
                GPIO_DisableInt(P3, 2);
        break;
#endif

#if (SPL_EXTINT1_EN > 0)
        case SPL_EXTINT_INT_1:
            if(int_attr)
                GPIO_EnableInt(P3, 3, int_attr);
            else
                GPIO_DisableInt(P3, 3);
        break;
#endif
    }
}

extern void spl_extint_deinit( void )
{
#if (SPL_EXTINT0_EN > 0)
    NVIC_DisableIRQ(EINT0_IRQn);
#endif

#if (SPL_EXTINT1_EN > 0)
    NVIC_DisableIRQ(EINT1_IRQn);
#endif
}

#if (SPL_EXTINT0_EN > 0)
extern void spl_extint_int0_isr( void )
{
    /* Clear P3.2 interrupt flag */
    GPIO_CLR_INT_FLAG(P3, BIT2);
    SPL_EXTINT_INT0_CALLBACK();
}
#endif

#if (SPL_EXTINT1_EN > 0)
extern void spl_extint_int1_isr( void )
{
    /* Clear P3.3 interrupt flag */
    GPIO_CLR_INT_FLAG(P3, BIT3);
    SPL_EXTINT_INT1_CALLBACK();
}
#endif

#endif /* (SPL_EXTINT_EN > 0) */
/**************************************************************************************************
**************************************************************************************************/

