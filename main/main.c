/******************************************************************************

 @file  main.c

 @brief 

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 
 *****************************************************************************/

#include "osal.h"
#include "hal.h"
#include "app.h"
#include "main.h"

#include "stdstr.h"

FLASH OSAL_TASK_t osal_task_list[OSAL_TASK_MAX] = {
    hal_task_driver_basic,

#if APP_CLI_EN > 0
    app_task_cli,
#else
    NULL,
#endif

#if APP_KEY_EN > 0
    app_task_key,
#else
    NULL,
#endif
    app_task_main,
};

int main( void )
{
    /* Disable Interrupts */
    OSAL_ENTER_CRITICAL();
    
    /* Initialize the OS's vars */
    osal_system_init();
    
    /* Initialize mcu and devices */
    hal_driver_init();
    
    /* Create HAL Tasks */
    //osal_task_create( hal_task_driver_basic, TASK_ID_HAL_DRIVER_BASIC );
    
    /* Create APP Tasks */
    //osal_task_create( app_task_cli, TASK_ID_APP_CLI );
#if APP_CLI_EN > 0
    app_task_cli_init();
#endif

#if APP_KEY_EN > 0
    app_task_key_init();
#endif

    //osal_task_create( app_task_main, TASK_ID_APP_MAIN );
    app_task_main_init();

    /* Enable Interrupts */
    OSAL_EXIT_CRITICAL();
    /* Start OSAL */
    osal_system_start(); // No Return from here

    return 0;

}

