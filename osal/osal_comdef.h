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
#ifndef __OSAL_COMDEF_H__
#define __OSAL_COMDEF_H__

#include "stdint.h"
#include "stdmacro.h"
#include "osal_config.h"
#include "osal_port.h"

/**************************** CONSTANTs ***************************/
#define     OSAL_VERSION                    "V1.0.0"
#if (OSAL_MSG_EN > 0)
#define     OSAL_TASK_EVENT_MSG             0
#endif
#define     OSAL_TASK_ID_NONE               0xFF

/**************************** TYPEDEFs ****************************/
typedef void ( *OSAL_TASK_t )( uint8_t task_id, uint8_t event_id );


#if ( OSAL_ASSERT_EN == 0 )
#define OSAL_ASSERT(expr)                        
#define OSAL_ASSERT_FORCED()
#define OSAL_ASSERT_STATEMENT(statement)
#define OSAL_ASSERT_DECLARATION(declaration)
#else
#define OSAL_ASSERT(expr)                        st( if (!( expr )) osal_assert_handler(); )
#define OSAL_ASSERT_FORCED()                     osal_assert_handler()
#define OSAL_ASSERT_STATEMENT(statement)         st( statement )
#define OSAL_ASSERT_DECLARATION(declaration)     declaration
#endif


#endif //__OSAL_COMDEF_H__
