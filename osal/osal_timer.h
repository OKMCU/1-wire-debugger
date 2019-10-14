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
#ifndef __OSAL_TIMER_H__
#define __OSAL_TIMER_H__

#include "stdint.h"
#include "osal_config.h"

#if (OSAL_TIMER_TIMEOUT_MAX == UINT8_MAX)
typedef uint8_t  osal_timer_timeout_t;
#elif (OSAL_TIMER_TIMEOUT_MAX == UINT16_MAX)
typedef uint16_t osal_timer_timeout_t;
#elif (OSAL_TIMER_TIMEOUT_MAX == UINT32_MAX)
typedef uint32_t osal_timer_timeout_t;
#else
#error "OSAL_TIMER_TIMEOUT_MAX must be UINT8_MAX or UINT16_MAX or UINT32_MAX"
#endif
extern void     osal_timer_init         ( void );
extern void     osal_timer_get_time     ( uint32_t *p_sec, uint16_t *p_ms );
extern void     osal_timer_set_time     ( uint32_t sec, uint16_t ms );

extern void     osal_timer_event_create ( uint8_t task_id, uint8_t event_id, osal_timer_timeout_t timeout_ms );
extern void     osal_timer_event_update ( uint8_t task_id, uint8_t event_id, osal_timer_timeout_t timeout_ms );
extern void     osal_timer_event_delete ( uint8_t task_id, uint8_t event_id );
extern osal_timer_timeout_t osal_timer_event_query  ( uint8_t task_id, uint8_t event_id );


#endif //__OSAL_TIMER_H__

