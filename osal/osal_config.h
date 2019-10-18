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
#ifndef __OSAL_CONFIG_H__
#define __OSAL_CONFIG_H__
#include "stdint.h"


#define     OSAL_ASSERT_EN          1
#define     OSAL_FIFO_EN            0
#define     OSAL_MSG_EN             0
#define     OSAL_TIMER_EN           0
#define     OSAL_TIMER_STATIC_EN    0
#define     OSAL_TIMER_MAX          8           //meaningless if OSAL_TIMER_STATIC_EN == 0
#define     OSAL_TIMER_TIMEOUT_MAX  UINT32_MAX  //should be UINT8_MAX or UINT16_MAX or UINT32_MAX
#define     OSAL_MEM_EN             0

#define     OSAL_TASK_MAX           4
#define     OSAL_EVENT_MAX          8      //must be 8, 16 or 32
#define     OSAL_FIFO_PAGE_SIZE     64





#endif //__OSAL_CONFIG_H__

