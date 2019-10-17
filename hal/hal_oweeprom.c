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

uint8_t dallas_crc8(const uint8_t * p_data, const uint16_t size)
{
    uint8_t crc = 0;
    uint8_t j, inbyte, mix;
    uint16_t i;
    
    for ( i = 0; i < size; ++i )
    {
        inbyte = p_data[i];
        for ( j = 0; j < 8; ++j )
        {
            mix = (crc ^ inbyte) & 0x01;
            crc >>= 1;
            if ( mix ) crc ^= 0x8C;
            inbyte >>= 1;
        }
    }
    return crc;
}


/**
  * @brief  hal_oweeprom_init
  *         Function Description 1
  *         Function Description 2
  * @param  None
  * @retval None
  */
extern void  hal_oweeprom_init( void )
{
    spl_owbus_init();
}

extern int8_t  hal_oweeprom_rom_read( HAL_OWEEPROM_ROM_CODE_t *rom_code )
{
    uint8_t buf[8];
    uint8_t crc;
    
    if( spl_owbus_std_reset() < 0 ) 
        return -HAL_OWEEPROM_ERR_DEV_NOT_FOUND;

    buf[0] = 0x33;
    
    spl_owbus_std_write( &buf, 1 );
    spl_owbus_std_read( buf, 8 );
    
    rom_code->crc_code      = buf[0];
    rom_code->serial_num[0] = buf[1];
    rom_code->serial_num[1] = buf[2];
    rom_code->serial_num[2] = buf[3];
    rom_code->serial_num[3] = buf[4];
    rom_code->serial_num[4] = buf[5];
    rom_code->serial_num[5] = buf[6];
    rom_code->family_code   = buf[7];

    crc = dallas_crc8( buf, 7 );
    if( crc != rom_code->crc_code )
        return -HAL_OWEEPROM_ERR_CRC;
    
    return HAL_OWEEPROM_ERR_NONE;
}


/************************ (C) COPYRIGHT Pencil Development Ltd. *****END OF FILE****/
