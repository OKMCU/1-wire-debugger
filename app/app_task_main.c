/******************************************************************************

 @file  app_task_main.c

 @brief 

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 2016-06-09 06:57:09
 *****************************************************************************/

/**************************************************************************************************
 * INCLUDES
 **************************************************************************************************/
#include "osal.h"
#include "hal.h"
#include "app.h"

#include "main.h"
/**************************************************************************************************
 * TYPES
 **************************************************************************************************/

/**************************************************************************************************
 * CONSTANTS
 **************************************************************************************************/


/**************************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************************/
extern void app_task_main_init( void )
{
    osal_event_set( TASK_ID_APP_MAIN, TASK_EVT_APP_MAIN_POR );
}


extern void app_task_main ( uint8_t task_id, uint8_t event_id )
{
    task_id = task_id;
    
    switch (event_id)
    {
        case TASK_EVT_APP_MAIN_POR:
        {
            app_event_main_por();
        }
        break;


        case TASK_EVT_APP_MAIN_OSAL_EXCEPTION:
        {
            app_event_main_osal_exception();
        }
        break;

        case TASK_EVT_APP_MAIN_HAL_EXCEPTION:
        {
            app_event_main_hal_exception();
        }
        break;

        case TASK_EVT_APP_MAIN_APP_EXCEPTION:
        {
            app_event_main_app_exception();
        }
        break;
        
        case TASK_EVT_APP_MAIN_IDLE:
        {
            app_event_main_idle();
        }
        break;
        
        default:
            APP_ASSERT_FORCED();
        break;
    }
}



/**************************************************************************************************
**************************************************************************************************/

