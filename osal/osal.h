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
#ifndef __OSAL_H__
#define __OSAL_H__



#include "osal_config.h"
#include "osal_comdef.h"
#include "osal_port.h"
#include "osal_task.h"
#include "osal_system.h"

#if (OSAL_FIFO_EN > 0)
#include "osal_fifo.h"
#endif

#if (OSAL_TIMER_EN > 0)
#include "osal_timer.h"
#endif

#if (OSAL_MSG_EN > 0)
#include "osal_msg.h"
#endif

#endif //__OSAL_H__

