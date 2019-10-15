/**
  ******************************************************************************
  * @file    hal_oweeprom.h
  * @author  Wentao SUN
  * @brief   1-wire EEPROM read/write APIs.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2019 Pencil Development Ltd.</center></h2>
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_OWEEPROM_H__
#define __HAL_OWEEPROM_H__

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "hal_config.h"

/* Exported constants --------------------------------------------------------*/
#define HAL_OWEEPROM_SIZE             2560      // bytes, (80 pages, 32 bytes/page)
#define HAL_OWEEPROM_SCRPAD_SIZE      32        // bytes

#define HAL_OWEEPROM_ERR_NONE         0         
#define HAL_OWEEPROM_ERR_HWDEV        1         // hardware device access error
#define HAL_OWEEPROM_ERR_CRC          2

/* Exported types ------------------------------------------------------------*/
typedef struct {
    uint8_t family_code;
    uint8_t serial_num[6];
    uint8_t crc_code;
} HAL_OWEEPROM_ROM_CODE_t;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

extern void  hal_oweeprom_init( void );

extern int8_t  hal_oweeprom_rom_read( HAL_OWEEPROM_ROM_CODE_t *rom_code );
//extern int8_t hal_oweeprom_rom_skip( void );
//extern int8_t hal_oweeprom_rom_ovrdrv_skip( void );
//extern int8_t hal_oweeprom_rom_match( void );
//extern int8_t hal_oweeprom_rom_ovrdrv_match( void );
//extern int8_t hal_oweeprom_rom_resume( void );
//extern int8_t hal_oweeprom_rom_search( OW_EEPROM_ROM_CODE_t **rom_code, uint16_t num_max );
//extern int8_t hal_oweeprom_scrpad_write( uint16_t address, const uint8_t *p_buf, uint8_t size );
//extern int8_t hal_oweeprom_scrpad_read( uint16_t *address, uint8_t *p_buf, uint8_t size );
//extern int8_t hal_oweeprom_scrpad_copy( uint16_t address, uint8_t es_code );
//extern int8_t hal_oweeprom_memory_read( uint16_t address, uint8_t *p_buf, uint16_t len );
//extern int8_t hal_oweeprom_memory_ext_read( uint16_t address, uint8_t *p_buf, uint16_t len );

#endif /* __HAL_OWEEPROM_H__ */

/********** (C) COPYRIGHT 2019 Pencil Development Ltd. *****END OF FILE****/

