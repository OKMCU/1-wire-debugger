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
#include "osal_msg.h"
#include "osal_config.h"
#include "osal_port.h"
#include "osal_task.h"
#include "stdint.h"

#if (OSAL_MSG_EN > 0)

typedef struct osal_msg_t {
    struct osal_msg_t *p_node_prev;
    struct osal_msg_t *p_node_next;
    uint16_t len;
    uint8_t  task_id;
    uint8_t  type;
} OSAL_MSG_t;

static OSAL_MSG_t *p_msg_list_head;
static OSAL_MSG_t *p_msg_list_tail;

static OSAL_MSG_t *osal_msg_list_find( OSAL_MSG_t *p_node )
{
    OSAL_MSG_t *p_node_match;
    
    if( p_msg_list_head == NULL || p_msg_list_tail == NULL )
    {
        return NULL;
    }
    
    //find the timer at the tail
    p_node_match = p_msg_list_head;
    while( p_node != p_node_match )
    {
        p_node_match = p_node_match->p_node_next;
        if( p_node_match == NULL )
            return NULL;
    }
    
    return p_node_match;
}

static void osal_msg_list_del( OSAL_MSG_t *p_node )
{
    OSAL_ASSERT( p_msg_list_head != NULL && p_msg_list_tail != NULL );
    
    if( p_node->p_node_prev == NULL &&
        p_node->p_node_next == NULL )
    {
        p_msg_list_head = NULL;
        p_msg_list_tail = NULL;
    }
    else
    {
        if( p_node->p_node_prev != NULL )
        {
            p_node->p_node_prev->p_node_next = p_node->p_node_next;
            if( p_node->p_node_prev->p_node_next == NULL )
            {
                p_msg_list_tail = p_node->p_node_prev;
            }
        }

        if( p_node->p_node_next != NULL )
        {
            p_node->p_node_next->p_node_prev = p_node->p_node_prev;
            if( p_node->p_node_next->p_node_prev == NULL )
            {
                p_msg_list_head = p_node->p_node_next;
            }
        }
    }
}

static void osal_msg_list_add( OSAL_MSG_t *p_node_new )
{
    p_node_new->p_node_next = NULL;
    if( p_msg_list_tail )
    {
        OSAL_ASSERT( p_msg_list_head != NULL );
        //append the new timer to the tail
        p_node_new->p_node_prev = p_msg_list_tail;
        p_msg_list_tail->p_node_next = p_node_new;
        p_msg_list_tail = p_node_new;
    }
    else
    {
        OSAL_ASSERT( p_msg_list_head == NULL );
        p_node_new->p_node_prev = NULL;
        p_msg_list_head = p_node_new;
        p_msg_list_tail = p_node_new;
    }
}



extern void     osal_msg_init         ( void )
{
    p_msg_list_head = NULL;
    p_msg_list_tail = NULL;
}

extern void *osal_msg_create ( uint16_t len )
{
    OSAL_MSG_t *p_node_new;
    void *p_msg;

    OSAL_ASSERT( len > 0 );
    
    p_node_new = (OSAL_MSG_t *)osal_mem_alloc( sizeof(OSAL_MSG_t) + len );

    if( p_node_new == NULL )
        return NULL;
    
    p_msg = (void *)( (uint8_t *)p_node_new + sizeof( OSAL_MSG_t ) );
    
    p_node_new->len = len;
    p_node_new->type = 0;
    
    return p_msg;
}

extern void osal_msg_delete ( void *p_msg )
{
    OSAL_MSG_t *p_node;

    OSAL_ASSERT( p_msg != NULL );

    p_node = (OSAL_MSG_t *)((uint8_t *)p_msg - sizeof(OSAL_MSG_t));
    
    if( osal_msg_list_find( p_node ) != NULL )
    {
        //kick it out of the list
        osal_msg_list_del( p_node );
    }
    
    //free the RAM of the msg node
    osal_mem_free( p_node );
}

extern void osal_msg_send ( void *p_msg, uint8_t task_id )
{
    OSAL_MSG_t *p_node;
    
    OSAL_ASSERT( p_msg != NULL );
    p_node = (OSAL_MSG_t *)((uint8_t *)p_msg - sizeof(OSAL_MSG_t));
    OSAL_ASSERT( osal_msg_list_find( p_node ) == NULL );
    
    p_node->task_id = task_id;
    osal_msg_list_add( p_node );
    osal_event_set( task_id, OSAL_TASK_EVENT_MSG );
}

extern void osal_msg_fwrd ( void *p_msg, uint8_t task_id )
{
    OSAL_MSG_t *p_node;
    
    OSAL_ASSERT( p_msg != NULL );
    p_node = (OSAL_MSG_t *)((uint8_t *)p_msg - sizeof(OSAL_MSG_t));
    OSAL_ASSERT( osal_msg_list_find( p_node ) != NULL );
    p_node->task_id = task_id;
    osal_event_set( task_id, OSAL_TASK_EVENT_MSG );
}

extern void *osal_msg_recv ( uint8_t task_id )
{
    OSAL_MSG_t *p_node_match;
    void *p_msg;
    
    if( p_msg_list_head == NULL || p_msg_list_tail == NULL )
        return NULL;

    //find the timer at the tail
    p_node_match = p_msg_list_head;
    while( p_node_match->task_id != task_id )
    {
        p_node_match = p_node_match->p_node_next;
        if( p_node_match == NULL )
            return NULL;
    }

    p_msg = (void *)((uint8_t *)p_node_match + sizeof(OSAL_MSG_t));
    return p_msg;
}

extern uint16_t osal_msg_len ( void *p_msg )
{
    OSAL_MSG_t *p_node;
    
    OSAL_ASSERT( p_msg != NULL );
    p_node = (OSAL_MSG_t *)((uint8_t *)p_msg - sizeof(OSAL_MSG_t));
    
    return p_node->len;
}

extern uint8_t osal_msg_get_type ( void *p_msg )
{
    OSAL_MSG_t *p_node;
    
    OSAL_ASSERT( p_msg != NULL );
    p_node = (OSAL_MSG_t *)((uint8_t *)p_msg - sizeof(OSAL_MSG_t));
    
    return p_node->type;
}

extern void osal_msg_set_type     ( void *p_msg, uint8_t type )
{
    OSAL_MSG_t *p_node;
    
    OSAL_ASSERT( p_msg != NULL );
    p_node = (OSAL_MSG_t *)((uint8_t *)p_msg - sizeof(OSAL_MSG_t));
    
    p_node->type = type;
}

#endif


