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
#include "stdint.h"
#include "bufmgr.h"

#ifndef FIFO_BUF_API_EN
#define FIFO_BUF_API_EN 1
#endif

#if FIFO_BUF_API_EN > 0
extern void    fifo_buf_flush( FIFO_BUF_t *p_fifo )
{
	p_fifo->head = 0;
	p_fifo->tail = 0;
}

extern void    fifo_buf_put  ( FIFO_BUF_t *p_fifo, uint8_t byte )
{
	uint8_t head;
	
	head = p_fifo->head;
	p_fifo->buf[head++] = byte;
	if( head >= p_fifo->size )
		head = 0;
	p_fifo->head = head;
}

extern uint8_t fifo_buf_get  ( FIFO_BUF_t *p_fifo )
{
	uint8_t tail;
	uint8_t byte;
	
	tail = p_fifo->tail;
	byte = p_fifo->buf[tail++];
	if( tail >= p_fifo->size )
		tail = 0;
	p_fifo->tail = tail;
	return byte;
}

extern uint8_t fifo_buf_full ( const FIFO_BUF_t *p_fifo )
{
	uint8_t head;
	head = p_fifo->head;
	head++;
	if( head >= p_fifo->size )
		head = 0;
	return ( head == p_fifo->tail );
}

extern uint8_t fifo_buf_empty( const FIFO_BUF_t *p_fifo )
{
	return ( p_fifo->tail == p_fifo->head );
}
#endif

