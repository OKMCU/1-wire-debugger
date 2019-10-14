#include "osal.h"
#include "spl.h"

#include "hal_config.h"
#include "hal_assert.h"
#include "hal_key.h"

#include "main.h"

#include <string.h>
#include "stringx.h"
#include "bufmgr.h"

#if (HAL_KEY_EN > 0)

#define HAL_KEY_STATE_PRESSED   0
#define HAL_KEY_STATE_RELEASED  1

#define HAL_KEY_IO_INIT()       //st(SPL_GPIO_SET_MODE_P06_QUASI(); SPL_GPIO_SET_MODE_P07_QUASI();)

#if ( HAL_KEY_MAX > 0 )
#define HAL_STATE_KEY0()        HAL_KEY_STATE_RELEASED//spl_gpio_read_pin(SPL_GPIO_PIN_P06)
#endif
#if ( HAL_KEY_MAX > 1 )
#define HAL_STATE_KEY1()        HAL_KEY_STATE_RELEASED//spl_gpio_read_pin(SPL_GPIO_PIN_P07)
#endif
#if ( HAL_KEY_MAX > 2 )
#define HAL_STATE_KEY2()        
#endif
#if ( HAL_KEY_MAX > 3 )
#define HAL_STATE_KEY3()        
#endif
#if ( HAL_KEY_MAX > 4 )
#define HAL_STATE_KEY4()        
#endif
#if ( HAL_KEY_MAX > 5 )
#define HAL_STATE_KEY5()        
#endif
#if ( HAL_KEY_MAX > 6 )
#define HAL_STATE_KEY6()        
#endif
#if ( HAL_KEY_MAX > 7 )
#define HAL_STATE_KEY7()        
#endif

extern void     hal_key_init ( void )
{
    HAL_KEY_IO_INIT();
}

extern uint8_t  hal_key_read ( void )
{
    uint8_t key;

    key = 0;
    
#if ( HAL_KEY_MAX > 0 )
    if( HAL_STATE_KEY0() == HAL_KEY_STATE_PRESSED ) key |= BV(HAL_KEY_0);
#endif
#if ( HAL_KEY_MAX > 1 )
    if( HAL_STATE_KEY1() == HAL_KEY_STATE_PRESSED ) key |= BV(HAL_KEY_1);
#endif
#if ( HAL_KEY_MAX > 2 )
    if( HAL_STATE_KEY2() == HAL_KEY_STATE_PRESSED ) key |= BV(HAL_KEY_2);
#endif
#if ( HAL_KEY_MAX > 3 )
    if( HAL_STATE_KEY3() == HAL_KEY_STATE_PRESSED ) key |= BV(HAL_KEY_3);
#endif
#if ( HAL_KEY_MAX > 4 )
    if( HAL_STATE_KEY4() == HAL_KEY_STATE_PRESSED ) key |= BV(HAL_KEY_4);
#endif
#if ( HAL_KEY_MAX > 5 )
    if( HAL_STATE_KEY5() == HAL_KEY_STATE_PRESSED ) key |= BV(HAL_KEY_5);
#endif
#if ( HAL_KEY_MAX > 6 )
    if( HAL_STATE_KEY6() == HAL_KEY_STATE_PRESSED ) key |= BV(HAL_KEY_6);
#endif
#if ( HAL_KEY_MAX > 7 )
    if( HAL_STATE_KEY7() == HAL_KEY_STATE_PRESSED ) key |= BV(HAL_KEY_7);
#endif

    return key;
}

#endif /* (HAL_KEY_EN > 0) */

