/**
  ******************************************************************************
  * @file    hal_oweeprom.c 
  * @author  Wentao SUN
  * @brief   1-wire EEPROM read/write API implementations.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2019 Pencil Development Ltd.</center></h2>
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "osal.h"
#include "spl.h"

#include "hal_config.h"
#include "hal_assert.h"
#include "hal_oweeprom.h"

#include "main.h"

#include <string.h>
#include "stringx.h"
#include "bufmgr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/




/**
  * @brief  hal_oweeprom_init
  *         Function Description 1
  *         Function Description 2
  * @param  None
  * @retval None
  */
extern void  hal_oweeprom_init( void )
{
    
}

extern int8_t  hal_oweeprom_rom_read( HAL_OWEEPROM_ROM_CODE_t *rom_code )
{
    return HAL_OWEEPROM_ERR_NONE;
}


/************************ (C) COPYRIGHT Pencil Development Ltd. *****END OF FILE****/
