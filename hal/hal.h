/******************************************************************************

 @file  hal.h

 @brief Describe the purpose and contents of the file.

 Group: 
 Target Device: 

 ******************************************************************************

 ******************************************************************************
 Release Name: 
 Release Date: 
 *****************************************************************************/

#ifndef __HAL_H__
#define __HAL_H__

#include "hal_config.h"

#if HAL_ASSERT_EN > 0
#include "hal_assert.h"
#endif

#include "hal_drivers.h"
#include "hal_mcu.h"

#if HAL_CLI_EN > 0
#include "hal_cli.h"
#endif


#if HAL_KEY_EN > 0
#include "hal_key.h"
#endif

#if HAL_OWEEPROM_EN > 0
#include "hal_oweeprom.h"
#endif

/* ------------------------------------------------------------------------------------------------
 *                                           Macros
 * ------------------------------------------------------------------------------------------------
 */

/* ------------------------------------------------------------------------------------------------
 *                                          Prototypes
 * ------------------------------------------------------------------------------------------------
 */

/**************************************************************************************************
 */

/**************************************************************************************************
 *                                        FUNCTIONS - API
 **************************************************************************************************/
//extern void hal_init( void );

#endif

