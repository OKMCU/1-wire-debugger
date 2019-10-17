/**
  ******************************************************************************
  * @file    hal_owdev.h
  * @author  Wentao SUN
  * @brief   1-wire device operation APIs.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2019 Pencil Development Ltd.</center></h2>
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_OWDEV_H__
#define __HAL_OWDEV_H__

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "hal_config.h"

/* Exported constants --------------------------------------------------------*/

#define HAL_OWDEV_ERR_NONE               0         
#define HAL_OWDEV_ERR_DEV_NOT_FOUND      1  // hardware device not found
#define HAL_OWDEV_ERR_BUS_BUSY           2  // 1-wire bus keeps busy for a long time
#define HAL_OWDEV_ERR_CRC                3  // CRC check failed

#define HAL_OWDEV_READ_ROM               0x33
#define HAL_OWDEV_MATCH_ROM              0x55
#define HAL_OWDEV_SEARCH_ROM             0xF0
#define HAL_OWDEV_SKIP_ROM               0xCC
#define HAL_OWDEV_RESUME                 0xA5
#define HAL_OWDEV_OD_SKIP_ROM            0x3C
#define HAL_OWDEV_OD_MATCH_ROM           0x69

/* Exported types ------------------------------------------------------------*/
typedef struct {
    uint8_t family_code;
    uint8_t serial_num[6];
    uint8_t crc_code;
} HAL_OWDEV_ROM_CODE_t;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

extern int8_t hal_owdev_init( void );
extern int8_t hal_owdev_rom_read( HAL_OWDEV_ROM_CODE_t *rom_code );
extern int8_t hal_owdev_rom_match( const HAL_OWDEV_ROM_CODE_t *rom_code );
extern int8_t hal_owdev_rom_skip( void );
extern int8_t hal_owdev_rom_odskip( void );
extern int8_t hal_owdev_rom_odmatch( const HAL_OWDEV_ROM_CODE_t *rom_code );
extern void   hal_owdev_mem_write( const uint8_t *p_data, uint8_t len );
extern void   hal_owdev_mem_read( uint8_t *p_data, uint8_t len );

#endif /* __HAL_OWBUS_H__ */

/********** (C) COPYRIGHT 2019 Pencil Development Ltd. *****END OF FILE****/

