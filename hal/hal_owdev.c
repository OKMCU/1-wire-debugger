/**
  ******************************************************************************
  * @file    hal_owdev.c 
  * @author  Wentao SUN
  * @brief   1-wire device operation API implementations.
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
#include "hal_owdev.h"

#include "main.h"

#include <string.h>
#include "stringx.h"
#include "bufmgr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t od_flag = 0; //overdrive flag
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

static uint8_t dallas_crc8(const uint8_t * p_data, const uint16_t size)
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
  * @brief  hal_owdev_init
  *         Master Tx a reset pulse and detect slave presence signal
  *         Function Description 2
  * @param  None
  * @retval Returned value can be one of the following values:
  *         @arg @ref  HAL_OWDEV_ERR_NONE(0)         
  *         @arg @ref -HAL_OWDEV_ERR_DEV_NOT_FOUND(-1)
  *         @arg @ref -HAL_OWDEV_ERR_BUS_BUSY(-2)
  */
extern int8_t hal_owdev_init( void )
{
    od_flag = 0;
    spl_owbus_init();
    return spl_owbus_std_reset();
}

/**
  * @brief  hal_owdev_rom_read
  *         Master Tx a reset pulse and detect slave presence signal
  *         and read 64-bit ROM code from slave.
  * @param  rom_code - buffer to store ROM code read from slave
  * @retval Returned value can be one of the following values:
  *         @arg @ref  HAL_OWDEV_ERR_NONE(0)         
  *         @arg @ref -HAL_OWDEV_ERR_DEV_NOT_FOUND(-1)
  *         @arg @ref -HAL_OWDEV_ERR_BUS_BUSY(-2)
  *         @arg @ref -HAL_OWDEV_ERR_CRC(-3)
  */
extern int8_t hal_owdev_rom_read( HAL_OWDEV_ROM_CODE_t *rom_code )
{
    uint8_t buf[8];
    uint8_t tmp;
     int8_t err;

    err = spl_owbus_std_reset();
    if( err < 0 ) return err;

    buf[0] = HAL_OWDEV_READ_ROM;
    
    spl_owbus_std_write( &buf, 1 );
    spl_owbus_std_read( buf, sizeof(buf) );
    
    rom_code->crc_code      = buf[0];
    rom_code->serial_num[5] = buf[1];
    rom_code->serial_num[4] = buf[2];
    rom_code->serial_num[3] = buf[3];
    rom_code->serial_num[2] = buf[4];
    rom_code->serial_num[1] = buf[5];
    rom_code->serial_num[0] = buf[6];
    rom_code->family_code   = buf[7];

    tmp = buf[0]; buf[0] = buf[7]; buf[0] = tmp;
    tmp = buf[1]; buf[1] = buf[6]; buf[1] = tmp;
    tmp = buf[2]; buf[2] = buf[5]; buf[2] = tmp;
    tmp = buf[3]; buf[3] = buf[4]; buf[3] = tmp;

    tmp = dallas_crc8( buf, 7 );
    if( tmp != rom_code->crc_code )
        return -HAL_OWDEV_ERR_CRC;
    
    return HAL_OWDEV_ERR_NONE;
}

extern int8_t hal_owdev_rom_match( const HAL_OWDEV_ROM_CODE_t *rom_code )
{
    uint8_t buf[9];
     int8_t err;
     
    buf[0] = HAL_OWDEV_MATCH_ROM;
    buf[1] = rom_code->family_code;
    buf[2] = rom_code->serial_num[0];
    buf[3] = rom_code->serial_num[1];
    buf[4] = rom_code->serial_num[2];
    buf[5] = rom_code->serial_num[3];
    buf[6] = rom_code->serial_num[4];
    buf[7] = rom_code->serial_num[5];
    buf[8] = rom_code->crc_code;
    
    err = spl_owbus_std_reset();
    if( err < 0 ) return err;
    spl_owbus_std_write( buf, sizeof(buf) );
    return HAL_OWDEV_ERR_NONE;
}

extern int8_t hal_owdev_rom_skip( void )
{
    int8_t err;
    uint8_t rom_cmd = HAL_OWDEV_SKIP_ROM;

    err = spl_owbus_std_reset();
    if( err < 0 ) return err;
    spl_owbus_std_write( &rom_cmd, 1 );
    return HAL_OWDEV_ERR_NONE;
}

extern int8_t hal_owdev_rom_odskip( void )
{
    int8_t err;
    uint8_t rom_cmd = HAL_OWDEV_OD_SKIP_ROM;
    
    err = spl_owbus_std_reset();
    if( err < 0 ) return err;
    spl_owbus_std_write( &rom_cmd, 1 );
    od_flag = 1;
    return HAL_OWDEV_ERR_NONE;
}

extern int8_t hal_owdev_rom_odmatch( const HAL_OWDEV_ROM_CODE_t *rom_code )
{
    uint8_t buf[9];
     int8_t err;

    buf[0] = HAL_OWDEV_OD_MATCH_ROM;
    buf[1] = rom_code->family_code;
    buf[2] = rom_code->serial_num[0];
    buf[3] = rom_code->serial_num[1];
    buf[4] = rom_code->serial_num[2];
    buf[5] = rom_code->serial_num[3];
    buf[6] = rom_code->serial_num[4];
    buf[7] = rom_code->serial_num[5];
    buf[8] = rom_code->crc_code;
    
    err = spl_owbus_std_reset();
    if( err < 0 ) return err;
    spl_owbus_std_write( &buf[0], 1 );
    spl_owbus_ovd_write( &buf[1], 8 );
    od_flag = 1;
    return HAL_OWDEV_ERR_NONE;
}


extern void   hal_owdev_mem_write( const uint8_t *p_data, uint8_t len )
{
    if( od_flag )
        spl_owbus_ovd_write( p_data, len );
    else
        spl_owbus_std_write( p_data, len );
}

extern void   hal_owdev_mem_read( uint8_t *p_data, uint8_t len )
{
    if( od_flag )
        spl_owbus_ovd_read( p_data, len );
    else
        spl_owbus_std_read( p_data, len );
}


/************************ (C) COPYRIGHT Pencil Development Ltd. *****END OF FILE****/
