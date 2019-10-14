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
#include "osal.h"
#include "stdint.h"

#if (OSAL_TIMER_EN > 0)
extern void osal_timer_update( void );
#endif /* (OSAL_TIMER_EN > 0) */

extern FLASH OSAL_TASK_t osal_task_list[];

#if ( OSAL_EVENT_MAX == 32 )
extern uint32_t osal_event_list[];
#elif ( OSAL_EVENT_MAX == 16 )
extern uint16_t osal_event_list[];
#elif ( OSAL_EVENT_MAX == 8 )
extern uint8_t  osal_event_list[];
#else
#error "OSAL_EVENT_MAX must be 8, 16 or 32"
#endif

extern void osal_system_init ( void )
{
#if (OSAL_MEM_EN > 0)
    osal_mem_init();
#endif /* (OSAL_MEM_EN > 0) */

#if (OSAL_TIMER_EN > 0)
    osal_timer_init();
#endif /* (OSAL_TIMER_EN > 0) */
    osal_task_init();

#if ( OSAL_MSG_EN > 0 )
    osal_msg_init();
#endif /* ( OSAL_MSG_EN > 0 ) */
}

extern void osal_system_start( void )
{

#if (OSAL_EVENT_MAX == 32)
    uint32_t event;
#elif (OSAL_EVENT_MAX == 16)
    uint16_t event;
#elif (OSAL_EVENT_MAX == 8)
    uint8_t  event;
#else
#error "OSAL_EVENT_MAX must be 8, 16 or 32"
#endif

    uint8_t event_id;
    uint8_t index;
    
    for(;;)
    {
#if (OSAL_TIMER_EN > 0)
        osal_timer_update();
#endif /* (OSAL_TIMER_EN > 0) */
        
        for( index = 0; index < OSAL_TASK_MAX; index++ )
        {
            OSAL_ENTER_CRITICAL();
            event = osal_event_list[index];
            OSAL_EXIT_CRITICAL();
            
            if( event )
            {
                for( event_id = 0; event_id < OSAL_EVENT_MAX; event_id++ )
                {
                    if( event & BV( event_id ) )
                    {
                        break;
                    }
                }

                event = BV( event_id );
                event = ~event;
                
                OSAL_ENTER_CRITICAL();
                osal_event_list[index] &= event;
                OSAL_EXIT_CRITICAL();
                
                osal_task_list[index]( index, event_id );
                break;
            }
        }
    }
}





















