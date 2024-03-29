/******************************************************************************

 @file  app_task_main.h

 @brief 

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 2016-06-09 06:57:09
 *****************************************************************************/
#ifndef __APP_TASK_MAIN_H__
#define __APP_TASK_MAIN_H__


/**************************************************************************************************
 * INCLUDES
 **************************************************************************************************/

#include "stdint.h"
#include "app_config.h"

/**************************************************************************************************
 * TYPEDEF
 **************************************************************************************************/

/**************************************************************************************************
 * CONSTANTS
 **************************************************************************************************/

/**************************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************************/

/**************************************************************************************************
 * FUNCTIONS - API
 **************************************************************************************************/
extern void app_task_main_init( void );
extern void app_task_main ( uint8_t task_id, uint8_t event_id );

#endif

/**************************************************************************************************
**************************************************************************************************/
