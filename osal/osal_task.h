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
#ifndef __OSAL_TASK_H__
#define __OSAL_TASK_H__

#include "stdint.h"

extern void    osal_task_init       ( void );
extern void    osal_event_set       ( uint8_t task_id, uint8_t event_id );
extern void    osal_event_clr       ( uint8_t task_id, uint8_t event_id );




#endif //__OSAL_TASK_H__

