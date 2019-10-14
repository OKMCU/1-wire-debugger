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
#include "osal_timer.h"
#include "osal_config.h"
#include "osal_port.h"
#include "osal_task.h"
#include "stdint.h"
#include "stdstr.h"

#if (OSAL_TIMER_EN > 0)

#if (OSAL_TIMER_STATIC_EN == 0)
typedef struct osal_timer_t {
    struct osal_timer_t *p_timer_prev;
    struct osal_timer_t *p_timer_next;
    void ( *p_fxn )( void * );
    void *p_arg;
    osal_timer_timeout_t timeout;
} OSAL_TIMER_t;
#else
typedef struct osal_timer_t {
    osal_timer_timeout_t timeout;
    uint8_t task_id;
    uint8_t event_id;
} OSAL_TIMER_t;
#endif //(OSAL_TIMER_STATIC_EN == 0)

#if (OSAL_TIMER_STATIC_EN == 0)
static OSAL_TIMER_t *p_timers_head;
static OSAL_TIMER_t *p_timers_tail;
#else
static OSAL_TIMER_t osal_timer_list[OSAL_TIMER_MAX];
#endif //(OSAL_TIMER_STATIC_EN == 0)

static uint32_t time_sec;
static uint16_t time_ms;
static uint8_t prev_systick;
static uint8_t osal_systick;

#if (OSAL_TIMER_STATIC_EN == 0)
#if (OSAL_ASSERT_EN > 0)
static OSAL_TIMER_t *osal_timer_list_find( OSAL_TIMER_t *p_timer )
{
    OSAL_TIMER_t *p_timer_match;
    
    OSAL_ASSERT( p_timer != NULL );
    if( p_timers_head == NULL )
        return NULL;

    //find the timer at the tail
    p_timer_match = p_timers_head;
    while( p_timer != p_timer_match )
    {
        p_timer_match = p_timer_match->p_timer_next;
        if( p_timer_match == NULL )
            return NULL;
    }
    
    return p_timer_match;
}
#endif

static void osal_timer_list_del( OSAL_TIMER_t *p_timer )
{
    OSAL_ASSERT( p_timer != NULL );
    OSAL_ASSERT( p_timers_head != NULL );
    
    if( p_timer->p_timer_prev == NULL &&
        p_timer->p_timer_next == NULL )
    {
        p_timers_head = NULL;
        p_timers_tail = NULL;
    }
    else
    {
        if( p_timer->p_timer_prev != NULL )
        {
            p_timer->p_timer_prev->p_timer_next = p_timer->p_timer_next;
            if( p_timer->p_timer_prev->p_timer_next == NULL )
            {
                p_timers_tail = p_timer->p_timer_prev;
            }
        }

        if( p_timer->p_timer_next != NULL )
        {
            p_timer->p_timer_next->p_timer_prev = p_timer->p_timer_prev;
            if( p_timer->p_timer_next->p_timer_prev == NULL )
            {
                p_timers_head = p_timer->p_timer_next;
            }
        }
    }
}

static void osal_timer_list_add( OSAL_TIMER_t *p_timer_new )
{
    p_timer_new->p_timer_next = NULL;
    if( p_timers_tail )
    {
        //append the new timer to the tail
        p_timer_new->p_timer_prev = p_timers_tail;
        p_timers_tail->p_timer_next = p_timer_new;
        p_timers_tail = p_timer_new;
    }
    else
    {
        p_timer_new->p_timer_prev = NULL;
        OSAL_ASSERT( p_timers_head == NULL );
        p_timers_head = p_timer_new;
        p_timers_tail = p_timer_new;
    }
}

static void osal_timer_event_kernel ( void *p_arg )
{
    uint16_t u16tmp;
    uint8_t task_id;
    uint8_t event_id;

    u16tmp = (uint16_t)((uint32_t)p_arg);
    task_id = HI_UINT16(u16tmp);
    event_id = LO_UINT16(u16tmp);
    osal_event_set(task_id, event_id);
}

static OSAL_TIMER_t *osal_timer_event_find( uint8_t task_id, uint8_t event_id )
{
    OSAL_TIMER_t *p_timer_match;
    void *p_arg;
    
    if( p_timers_head == NULL )
        return NULL;

    //find the timer at the tail
    p_timer_match = p_timers_head;
    p_arg = (void *)BUILD_UINT16( event_id, task_id );
    while( p_timer_match->p_fxn != osal_timer_event_kernel || p_timer_match->p_arg != p_arg )
    {
        p_timer_match = p_timer_match->p_timer_next;
        if( p_timer_match == NULL )
            return NULL;
    }
    
    return p_timer_match;
}

static void    *osal_timer_cback_create ( void ( *p_fxn )( void * ), void *p_arg, osal_timer_timeout_t timeout_ms )
{
    OSAL_TIMER_t *p_timer_new;

    OSAL_ASSERT( p_fxn != NULL );
    OSAL_ASSERT( timeout_ms != 0 );

    p_timer_new = osal_mem_alloc( sizeof(OSAL_TIMER_t) );
    if( p_timer_new != NULL )
    {
        p_timer_new->p_fxn = p_fxn;
        p_timer_new->p_arg = p_arg;
        p_timer_new->timeout = timeout_ms;
        osal_timer_list_add( p_timer_new );
    }
    
    return p_timer_new;
}

//static void osal_timer_cback_update ( void *timer_id, osal_timer_timeout_t timeout_ms )
//{
//    OSAL_ASSERT( timer_id != NULL );
//    OSAL_ASSERT( timeout_ms != 0 );
//    OSAL_ASSERT( osal_timer_list_find( (OSAL_TIMER_t *)timer_id ) != NULL );
//    
//    ((OSAL_TIMER_t *)timer_id)->timeout = timeout_ms;
//}

static void osal_timer_cback_delete ( void *timer_id )
{
    OSAL_ASSERT( timer_id != NULL );
    OSAL_ASSERT( osal_timer_list_find( (OSAL_TIMER_t *)timer_id ) != NULL );
    
    osal_timer_list_del( (OSAL_TIMER_t *)timer_id );
    osal_mem_free( timer_id );
}

//static uint32_t osal_timer_cback_query  ( void *timer_id )
//{
//    OSAL_ASSERT( timer_id != NULL );
//    OSAL_ASSERT( osal_timer_list_find( (OSAL_TIMER_t *)timer_id ) != NULL );
//    
//    return ( (OSAL_TIMER_t *)timer_id )->timeout;
//}
#else

#if (OSAL_TIMER_MAX >= UINT8_MAX)
static uint16_t
#else
static uint8_t  
#endif //(OSAL_TIMER_MAX >= UINT8_MAX)
osal_timer_event_find( uint8_t task_id, uint8_t event_id )
{
#if (OSAL_TIMER_MAX >= UINT8_MAX)
    uint16_t timer_id;
#else
    uint8_t  timer_id;
#endif //(OSAL_TIMER_MAX >= UINT8_MAX)
    for( timer_id = 0; timer_id < OSAL_TIMER_MAX; timer_id++ )
    {
        if( osal_timer_list[timer_id].timeout > 0 )
        {
            if( osal_timer_list[timer_id].task_id  == task_id &&
                osal_timer_list[timer_id].event_id == event_id )
            {
                break;
            }
        }
    }
    
    return timer_id;
}
#endif //(OSAL_TIMER_STATIC_EN == 0)

extern void     osal_timer_init         ( void )
{
#if (OSAL_TIMER_STATIC_EN == 0)
    p_timers_head = NULL;
    p_timers_tail = NULL;
#else
    mem_set( osal_timer_list, 0, sizeof(osal_timer_list) );
#endif
    time_sec = 0;
    time_ms = 0;
    prev_systick = 0;
    osal_systick = 0;
}

extern void     osal_timer_update       ( void )
{
#if (OSAL_TIMER_STATIC_EN == 0)
    OSAL_TIMER_t *p_timer_curr;
    OSAL_TIMER_t *p_timer_next;
    OSAL_TIMER_t *p_timer_head;
    OSAL_TIMER_t *p_timer_tail;
    void (*p_fxn)( void * );
    void *p_arg;
#else
#if (OSAL_TIMER_MAX >= UINT8_MAX)
    uint16_t timer_id;
#else
    uint8_t  timer_id;
#endif//(OSAL_TIMER_MAX >= UINT8_MAX)
#endif//(OSAL_TIMER_STATIC_EN == 0)

    uint8_t curr_systick;
    uint8_t delta_systick;
    
    OSAL_ENTER_CRITICAL();
    curr_systick = osal_systick;
    OSAL_EXIT_CRITICAL();

    if( curr_systick != prev_systick )
    {
        delta_systick = ( curr_systick > prev_systick ) ? ( curr_systick - prev_systick ) : ( UINT8_MAX - prev_systick + curr_systick );
        prev_systick = curr_systick;
        
        time_ms += delta_systick;
        if( time_ms >= 1000 )
        {
            time_ms -= 1000;
            time_sec++;
        }

#if (OSAL_TIMER_STATIC_EN == 0)
        if( p_timers_head )
        {
            OSAL_ASSERT( p_timers_tail != NULL );
            //save the head and tail to local
            p_timer_head = p_timers_head;
            p_timer_tail = p_timers_tail;
            
            p_timer_curr = NULL;
            p_timer_next = p_timer_head;
            while( p_timer_curr != p_timer_tail )
            {
                p_timer_curr = p_timer_next;
                p_timer_next = p_timer_next->p_timer_next;
                p_timer_curr->timeout = ( p_timer_curr->timeout >= delta_systick ) ? ( p_timer_curr->timeout - delta_systick ) : 0;
                if( p_timer_curr->timeout == 0 )
                {
                    p_fxn = p_timer_curr->p_fxn;
                    p_arg = p_timer_curr->p_arg;
                    osal_timer_list_del( p_timer_curr );
                    osal_mem_free( p_timer_curr );
                    p_fxn( p_arg );
                }
            }
        }
#else
        for( timer_id = 0; timer_id < OSAL_TIMER_MAX; timer_id++ )
        {
            if( osal_timer_list[timer_id].timeout )
            {
                osal_timer_list[timer_id].timeout = ( osal_timer_list[timer_id].timeout >= delta_systick ) ? (osal_timer_list[timer_id].timeout - delta_systick) : 0;
                if( osal_timer_list[timer_id].timeout == 0 )
                {
                    osal_event_set( osal_timer_list[timer_id].task_id, osal_timer_list[timer_id].event_id );
                }
            }
        }
#endif
    }
}

extern void     osal_timer_get_time     ( uint32_t *p_sec, uint16_t *p_ms )
{
    if( p_sec )
        *p_sec = time_sec;
    if( p_ms )
        *p_ms = time_ms;
}

extern void     osal_timer_set_time     ( uint32_t sec, uint16_t ms )
{
    time_sec = sec;
    time_ms = ms;
}

extern void osal_timer_event_create ( uint8_t task_id, uint8_t event_id, osal_timer_timeout_t timeout_ms )
{
#if (OSAL_TIMER_STATIC_EN == 0)
    OSAL_TIMER_t *p_timer_match;
#else
#if (OSAL_TIMER_MAX >= UINT8_MAX)
    uint16_t timer_id;
#else
    uint8_t  timer_id;
#endif//(OSAL_TIMER_MAX >= UINT8_MAX)
#endif

    OSAL_ASSERT( task_id < OSAL_TASK_MAX );
    OSAL_ASSERT( event_id < OSAL_EVENT_MAX );
    OSAL_ASSERT( timeout_ms != 0 );
    
#if (OSAL_TIMER_STATIC_EN == 0)
    p_timer_match = osal_timer_event_find( task_id, event_id );
    if( p_timer_match )
    {
        //if found, update it
        p_timer_match->timeout = timeout_ms;
    }
    else
    {
        //if not found, create it
        p_timer_match = (void *)osal_timer_cback_create( osal_timer_event_kernel, (void *)BUILD_UINT16( event_id, task_id ), timeout_ms);
    }
    OSAL_ASSERT( p_timer_match != NULL );
#else
    timer_id = osal_timer_event_find( task_id, event_id );
    if( timer_id != OSAL_TIMER_MAX )
    {
        //if found, update it
        osal_timer_list[timer_id].timeout = timeout_ms;
    }
    else
    {
        //find a free timer_id
        for( timer_id = 0; timer_id < OSAL_TIMER_MAX; timer_id++ )
        {
            if( osal_timer_list[timer_id].timeout == 0 )
            {
                osal_timer_list[timer_id].timeout = timeout_ms;
                osal_timer_list[timer_id].task_id = task_id;
                osal_timer_list[timer_id].event_id = event_id;
                break;
            }
        }

        OSAL_ASSERT( timer_id != OSAL_TIMER_MAX );
    }
#endif

}

extern void osal_timer_event_update ( uint8_t task_id, uint8_t event_id, osal_timer_timeout_t timeout_ms )
{
#if (OSAL_TIMER_STATIC_EN == 0)
    OSAL_TIMER_t *p_timer_match;
#else
#if (OSAL_TIMER_MAX >= UINT8_MAX)
    uint16_t timer_id;
#else
    uint8_t  timer_id;
#endif//(OSAL_TIMER_MAX >= UINT8_MAX)
#endif

    OSAL_ASSERT( task_id < OSAL_TASK_MAX);
    OSAL_ASSERT( event_id < OSAL_EVENT_MAX );
    OSAL_ASSERT( timeout_ms != 0 );

#if (OSAL_TIMER_STATIC_EN == 0)
    p_timer_match = osal_timer_event_find( task_id, event_id );
    if( p_timer_match )
    {
        p_timer_match->timeout = timeout_ms;
    }
#else
    timer_id = osal_timer_event_find( task_id, event_id );
    if( timer_id != OSAL_TIMER_MAX )
    {
        osal_timer_list[timer_id].timeout = timeout_ms;
    }
#endif

}

extern void osal_timer_event_delete ( uint8_t task_id, uint8_t event_id )
{
#if (OSAL_TIMER_STATIC_EN == 0)
    OSAL_TIMER_t *p_timer_match;
#else
#if (OSAL_TIMER_MAX >= UINT8_MAX)
    uint16_t timer_id;
#else
    uint8_t  timer_id;
#endif//(OSAL_TIMER_MAX >= UINT8_MAX)
#endif

    OSAL_ASSERT( task_id < OSAL_TASK_MAX);
    OSAL_ASSERT( event_id < OSAL_EVENT_MAX );
    
#if (OSAL_TIMER_STATIC_EN == 0)
    p_timer_match = osal_timer_event_find( task_id, event_id );

    if( p_timer_match )
    {
        osal_timer_cback_delete( p_timer_match );
    }
    else
    {
        osal_event_clr( task_id, event_id );
    }
#else
    timer_id = osal_timer_event_find( task_id, event_id );
    if( timer_id != OSAL_TIMER_MAX )
    {
        osal_timer_list[timer_id].timeout = 0;
    }
    else
    {
        osal_event_clr( task_id, event_id );
    }
#endif
}

extern uint32_t osal_timer_event_query  ( uint8_t task_id, uint8_t event_id )
{
#if (OSAL_TIMER_STATIC_EN == 0)
    OSAL_TIMER_t *p_timer_match;
#else
#if (OSAL_TIMER_MAX >= UINT8_MAX)
    uint16_t timer_id;
#else
    uint8_t  timer_id;
#endif//(OSAL_TIMER_MAX >= UINT8_MAX)
#endif

    OSAL_ASSERT( task_id < OSAL_TASK_MAX);
    OSAL_ASSERT( event_id < OSAL_EVENT_MAX );

#if (OSAL_TIMER_STATIC_EN == 0)
    p_timer_match = osal_timer_event_find( task_id, event_id );
    if( p_timer_match )
    {
        return p_timer_match->timeout;
    }
#else
    timer_id = osal_timer_event_find( task_id, event_id );
    if( timer_id != OSAL_TIMER_MAX )
    {
        return osal_timer_list[timer_id].timeout;
    }
#endif
    
    return 0;
}

extern void osal_timer_systick_inc( void )
{
    osal_systick++;
}

#endif /* (OSAL_TIMER_EN > 0) */











