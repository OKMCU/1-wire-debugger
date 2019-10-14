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
#include "osal_config.h"
#include "osal_port.h"
#include "osal_fifo.h"

#include "stdstr.h"

#if (OSAL_FIFO_EN > 0)

typedef struct FIFOPage {
    uint8_t buf[OSAL_FIFO_PAGE_SIZE];
    uint16_t head;
    uint16_t tail;
    struct FIFOPage *nextPage;
} FIFOPage_t;

typedef struct {
    uint32_t datalen;
    FIFOPage_t *headPage;
    FIFOPage_t *tailPage;
} FIFOHandle_t;

extern void *osal_fifo_create(void)
{
    void *handle;
    
    handle = osal_mem_alloc(sizeof(FIFOHandle_t));
    if(handle != NULL)
    {
        mem_set(handle, 0x00, sizeof(FIFOHandle_t));
    }
    
    return handle;
}

extern void osal_fifo_delete(void *fifo)
{
    FIFOHandle_t *fifoHandle = NULL;
    FIFOPage_t *fifoPageSav = NULL;
    fifoHandle = (FIFOHandle_t *)fifo;
    
    while(fifoHandle->headPage != NULL)
    {
        fifoPageSav = fifoHandle->headPage->nextPage;
        osal_mem_free(fifoHandle->headPage);
        fifoHandle->headPage = fifoPageSav;
    }
    
    osal_mem_free(fifoHandle);
}

extern uint8_t *osal_fifo_put(void *fifo, uint8_t byte)
{
    uint8_t *pos;
    FIFOHandle_t *fifoHandle = NULL;
    FIFOPage_t *fifoPage = NULL;
    fifoHandle = (FIFOHandle_t *)fifo;

    if(fifoHandle->tailPage == NULL)
    {
        OSAL_ASSERT(fifoHandle->headPage == NULL);
        fifoPage = osal_mem_alloc(sizeof(FIFOPage_t));
        if(fifoPage == NULL)
        {
            return NULL;
        }
        fifoPage->head = 0;
        fifoPage->tail = 0;
        fifoPage->nextPage = NULL;
        fifoHandle->headPage = fifoPage;
        fifoHandle->tailPage = fifoPage;
    }

    if(fifoHandle->tailPage->tail >= OSAL_FIFO_PAGE_SIZE)
    {
        OSAL_ASSERT(fifoHandle->tailPage->nextPage == NULL);
        fifoPage = osal_mem_alloc(sizeof(FIFOPage_t));
        if(fifoPage == NULL)
        {
            return NULL;
        }
        fifoPage->head = 0;
        fifoPage->tail = 0;
        fifoPage->nextPage = NULL;
        fifoHandle->tailPage->nextPage = fifoPage;
        fifoHandle->tailPage = fifoPage;
        
    }

    pos = fifoHandle->tailPage->buf + fifoHandle->tailPage->tail;
    *pos = byte;
    fifoHandle->tailPage->tail++;
    fifoHandle->datalen++;

    return pos;
    
}

extern uint32_t osal_fifo_len(void *fifo)
{
    return ((FIFOHandle_t *)fifo)->datalen;
}

extern uint8_t osal_fifo_get(void *fifo)
{
    uint8_t u8tmp = 0;
    FIFOHandle_t *fifoHandle = NULL;
    FIFOPage_t *fifoPage = NULL;
    fifoHandle = (FIFOHandle_t *)fifo;

    if(fifoHandle->datalen > 0)
    {
        u8tmp = fifoHandle->headPage->buf[fifoHandle->headPage->head++];
        fifoHandle->datalen--;
        if(fifoHandle->headPage->head == fifoHandle->headPage->tail)
        {
            fifoPage = fifoHandle->headPage->nextPage;
            osal_mem_free(fifoHandle->headPage);
            fifoHandle->headPage = fifoPage;
            if(fifoHandle->headPage == NULL)
            {
                fifoHandle->tailPage = NULL;
            }
        }
    }

    return u8tmp;
    
}

#endif /* (OSAL_FIFO_EN > 0) */

