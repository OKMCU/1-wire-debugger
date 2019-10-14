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
#ifndef __BUFMGR_H__
#define __BUFMGR_H__

#include "stdint.h"
#include "stdmacro.h"


#define FIFO_BUF_FLUSH( head, tail )                st( head = 0; tail = 0; )
#define FIFO_BUF_INC_INDEX( index, size )           index = (index+1)>=size ? 0:(index+1)
#define FIFO_BUF_PUT( byte, head, p_buf, size )     st( p_buf[head] = byte; FIFO_BUF_INC_INDEX(head, size); )
#define FIFO_BUF_GET( p_byte, tail, p_buf, size )   st( *(p_byte) = p_buf[tail]; FIFO_BUF_INC_INDEX(tail, size); )
#define FIFO_BUF_EMPTY( head, tail )                (head==tail)
#define FIFO_BUF_FULL( head, tail, size )           (((head+1)>=size?0:(head+1))==tail)

typedef struct fifo_buf_t {
    uint8_t *buf;
    uint8_t size;
	uint8_t head;
	uint8_t tail;
} FIFO_BUF_t;

extern void    fifo_buf_flush( FIFO_BUF_t *p_fifo );
extern void    fifo_buf_put  ( FIFO_BUF_t *p_fifo, uint8_t byte );
extern uint8_t fifo_buf_get  ( FIFO_BUF_t *p_fifo );
extern uint8_t fifo_buf_full ( const FIFO_BUF_t *p_fifo );
extern uint8_t fifo_buf_empty( const FIFO_BUF_t *p_fifo );

#endif

