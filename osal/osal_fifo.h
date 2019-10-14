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
#ifndef __OSAL_FIFO_H__
#define __OSAL_FIFO_H__

#include "stdint.h"

extern void *osal_fifo_create(void);
extern void osal_fifo_delete(void *fifo);
extern uint8_t *osal_fifo_put(void *fifo, uint8_t byte);
extern uint32_t osal_fifo_len(void *fifo);
extern uint8_t osal_fifo_get(void *fifo);

#endif /* __OSAL_FIFO_H__ */

