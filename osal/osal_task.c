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
#include "osal_task.h"
#include "osal_config.h"
#include "osal_port.h"

#include "stdstr.h"

extern FLASH OSAL_TASK_t osal_task_list[];

#if ( OSAL_EVENT_MAX == 32 )
uint32_t osal_event_list[ OSAL_TASK_MAX ];
#elif ( OSAL_EVENT_MAX == 16 )
uint16_t osal_event_list[ OSAL_TASK_MAX ];
#elif ( OSAL_EVENT_MAX == 8 )
uint8_t  osal_event_list[ OSAL_TASK_MAX ];
#else
#error "OSAL_EVENT_MAX must be 8, 16 or 32"
#endif


extern void    osal_task_init   ( void )
{
    mem_set( osal_event_list, 0, sizeof(osal_event_list) );
}

extern void osal_event_set   ( uint8_t task_id, uint8_t event_id )
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
    
    //OSAL_ASSERT( task_id < OSAL_TASK_MAX);
    //OSAL_ASSERT( event_id < OSAL_EVENT_MAX );
    //OSAL_ASSERT( osal_task_list[task_id] != NULL );
    //while( task_id  >= OSAL_TASK_MAX 
    //     ||event_id >= OSAL_EVENT_MAX
    //     ||osal_task_list[task_id] == NULL );
    
    event = ( 1 << event_id );
    
    OSAL_ENTER_CRITICAL();
    osal_event_list[task_id] |= event;
    OSAL_EXIT_CRITICAL();
}

extern void osal_event_clr   ( uint8_t task_id, uint8_t event_id )
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
    
    //OSAL_ASSERT( task_id < OSAL_TASK_MAX);
    //OSAL_ASSERT( event_id < OSAL_EVENT_MAX );
    //OSAL_ASSERT( osal_task_list[task_id] != NULL );
    //while( task_id  >= OSAL_TASK_MAX 
    //     ||event_id >= OSAL_EVENT_MAX
    //     ||osal_task_list[task_id] == NULL );
    
    event = ( 1 << event_id );
    event = ~event;
    
    OSAL_ENTER_CRITICAL();
    osal_event_list[task_id] &= event;
    OSAL_EXIT_CRITICAL();
    
}




















