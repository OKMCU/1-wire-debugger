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
#ifndef __STRINGX_H__
#define __STRINGX_H__

#include "stdint.h"



#define SINT_STR_LEN_MAX    11
#define UINT_STR_LEN_MAX    10
#define HEX8_STR_LEN_MAX    2
#define HEX16_STR_LEN_MAX   4
#define HEX32_STR_LEN_MAX   8

extern uint8_t hexstr2uint(const char *str, uint32_t *num);
extern uint8_t decstr2uint(const char *str, uint32_t *num);
extern uint8_t decstr2sint(const char *str, int32_t *num);
extern uint8_t tostr_sint(int32_t num, char *str);
extern uint8_t tostr_uint(uint32_t num, char *str);
extern uint8_t tostr_hex8(uint8_t num, char *str);
extern uint8_t tostr_hex16(uint16_t num, char *str);
extern uint8_t tostr_hex32(uint32_t num, char *str);
extern char *str_starts_with(char *str1, const char *str2);
extern char *str_ends_with(char *str1, const char *str2);

#endif

