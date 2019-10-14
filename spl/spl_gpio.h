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
#ifndef __SPL_GPIO_H__
#define __SPL_GPIO_H__
#include "stdint.h"
#include "Common.h"
#include "Function_Define.h"
#include "N76E003.h"
#include "SFR_Macro.h"

#define SPL_GPIO_PORT_0         P0
#define SPL_GPIO_PORT_1         P1
#define SPL_GPIO_PORT_3         P3

#define SPL_GPIO_PIN_P00        P00
#define SPL_GPIO_PIN_P01        P01
#define SPL_GPIO_PIN_P02        P02
#define SPL_GPIO_PIN_P03        P03
#define SPL_GPIO_PIN_P04        P04
#define SPL_GPIO_PIN_P05        P05
#define SPL_GPIO_PIN_P06        P06
#define SPL_GPIO_PIN_P07        P07
#define SPL_GPIO_PIN_P10        P10
#define SPL_GPIO_PIN_P11        P11
#define SPL_GPIO_PIN_P12        P12
#define SPL_GPIO_PIN_P13        P13
#define SPL_GPIO_PIN_P14        P14
#define SPL_GPIO_PIN_P15        P15
#define SPL_GPIO_PIN_P16        P16
#define SPL_GPIO_PIN_P17        P17
#define SPL_GPIO_PIN_P20        P20
#define SPL_GPIO_PIN_P30        P30

#define SPL_GPIO_SET_MODE_P00_QUASI()               P0M1&=~SET_BIT0;P0M2&=~SET_BIT0
#define SPL_GPIO_SET_MODE_P01_QUASI()               P0M1&=~SET_BIT1;P0M2&=~SET_BIT1
#define SPL_GPIO_SET_MODE_P02_QUASI()               P0M1&=~SET_BIT2;P0M2&=~SET_BIT2
#define SPL_GPIO_SET_MODE_P03_QUASI()               P0M1&=~SET_BIT3;P0M2&=~SET_BIT3
#define SPL_GPIO_SET_MODE_P04_QUASI()               P0M1&=~SET_BIT4;P0M2&=~SET_BIT4
#define SPL_GPIO_SET_MODE_P05_QUASI()               P0M1&=~SET_BIT5;P0M2&=~SET_BIT5
#define SPL_GPIO_SET_MODE_P06_QUASI()               P0M1&=~SET_BIT6;P0M2&=~SET_BIT6
#define SPL_GPIO_SET_MODE_P07_QUASI()               P0M1&=~SET_BIT7;P0M2&=~SET_BIT7
#define SPL_GPIO_SET_MODE_P10_QUASI()               P1M1&=~SET_BIT0;P1M2&=~SET_BIT0
#define SPL_GPIO_SET_MODE_P11_QUASI()               P1M1&=~SET_BIT1;P1M2&=~SET_BIT1
#define SPL_GPIO_SET_MODE_P12_QUASI()               P1M1&=~SET_BIT2;P1M2&=~SET_BIT2
#define SPL_GPIO_SET_MODE_P13_QUASI()               P1M1&=~SET_BIT3;P1M2&=~SET_BIT3
#define SPL_GPIO_SET_MODE_P14_QUASI()               P1M1&=~SET_BIT4;P1M2&=~SET_BIT4
#define SPL_GPIO_SET_MODE_P15_QUASI()               P1M1&=~SET_BIT5;P1M2&=~SET_BIT5
#define SPL_GPIO_SET_MODE_P16_QUASI()               P1M1&=~SET_BIT6;P1M2&=~SET_BIT6
#define SPL_GPIO_SET_MODE_P17_QUASI()               P1M1&=~SET_BIT7;P1M2&=~SET_BIT7
#define SPL_GPIO_SET_MODE_P30_QUASI()               P3M1&=~SET_BIT0;P3M2&=~SET_BIT0
#define SPL_GPIO_SET_MODE_P00_OUTPUT()              P0M1&=~SET_BIT0;P0M2|=SET_BIT0
#define SPL_GPIO_SET_MODE_P01_OUTPUT()              P0M1&=~SET_BIT1;P0M2|=SET_BIT1
#define SPL_GPIO_SET_MODE_P02_OUTPUT()              P0M1&=~SET_BIT2;P0M2|=SET_BIT2
#define SPL_GPIO_SET_MODE_P03_OUTPUT()              P0M1&=~SET_BIT3;P0M2|=SET_BIT3
#define SPL_GPIO_SET_MODE_P04_OUTPUT()              P0M1&=~SET_BIT4;P0M2|=SET_BIT4
#define SPL_GPIO_SET_MODE_P05_OUTPUT()              P0M1&=~SET_BIT5;P0M2|=SET_BIT5
#define SPL_GPIO_SET_MODE_P06_OUTPUT()              P0M1&=~SET_BIT6;P0M2|=SET_BIT6
#define SPL_GPIO_SET_MODE_P07_OUTPUT()              P0M1&=~SET_BIT7;P0M2|=SET_BIT7
#define SPL_GPIO_SET_MODE_P10_OUTPUT()              P1M1&=~SET_BIT0;P1M2|=SET_BIT0
#define SPL_GPIO_SET_MODE_P11_OUTPUT()              P1M1&=~SET_BIT1;P1M2|=SET_BIT1
#define SPL_GPIO_SET_MODE_P12_OUTPUT()              P1M1&=~SET_BIT2;P1M2|=SET_BIT2
#define SPL_GPIO_SET_MODE_P13_OUTPUT()              P1M1&=~SET_BIT3;P1M2|=SET_BIT3
#define SPL_GPIO_SET_MODE_P14_OUTPUT()              P1M1&=~SET_BIT4;P1M2|=SET_BIT4
#define SPL_GPIO_SET_MODE_P15_OUTPUT()              P1M1&=~SET_BIT5;P1M2|=SET_BIT5
#define SPL_GPIO_SET_MODE_P16_OUTPUT()              P1M1&=~SET_BIT6;P1M2|=SET_BIT6
#define SPL_GPIO_SET_MODE_P17_OUTPUT()              P1M1&=~SET_BIT7;P1M2|=SET_BIT7
#define SPL_GPIO_SET_MODE_P30_OUTPUT()              P3M1&=~SET_BIT0;P3M2|=SET_BIT0
#define SPL_GPIO_SET_MODE_P00_INPUT()               P0M1|=SET_BIT0;P0M2&=~SET_BIT0
#define SPL_GPIO_SET_MODE_P01_INPUT()               P0M1|=SET_BIT1;P0M2&=~SET_BIT1
#define SPL_GPIO_SET_MODE_P02_INPUT()               P0M1|=SET_BIT2;P0M2&=~SET_BIT2
#define SPL_GPIO_SET_MODE_P03_INPUT()               P0M1|=SET_BIT3;P0M2&=~SET_BIT3
#define SPL_GPIO_SET_MODE_P04_INPUT()               P0M1|=SET_BIT4;P0M2&=~SET_BIT4
#define SPL_GPIO_SET_MODE_P05_INPUT()               P0M1|=SET_BIT5;P0M2&=~SET_BIT5
#define SPL_GPIO_SET_MODE_P06_INPUT()               P0M1|=SET_BIT6;P0M2&=~SET_BIT6
#define SPL_GPIO_SET_MODE_P07_INPUT()               P0M1|=SET_BIT7;P0M2&=~SET_BIT7
#define SPL_GPIO_SET_MODE_P10_INPUT()               P1M1|=SET_BIT0;P1M2&=~SET_BIT0
#define SPL_GPIO_SET_MODE_P11_INPUT()               P1M1|=SET_BIT1;P1M2&=~SET_BIT1
#define SPL_GPIO_SET_MODE_P12_INPUT()               P1M1|=SET_BIT2;P1M2&=~SET_BIT2
#define SPL_GPIO_SET_MODE_P13_INPUT()               P1M1|=SET_BIT3;P1M2&=~SET_BIT3
#define SPL_GPIO_SET_MODE_P14_INPUT()               P1M1|=SET_BIT4;P1M2&=~SET_BIT4
#define SPL_GPIO_SET_MODE_P15_INPUT()               P1M1|=SET_BIT5;P1M2&=~SET_BIT5
#define SPL_GPIO_SET_MODE_P16_INPUT()               P1M1|=SET_BIT6;P1M2&=~SET_BIT6
#define SPL_GPIO_SET_MODE_P17_INPUT()               P1M1|=SET_BIT7;P1M2&=~SET_BIT7
#define SPL_GPIO_SET_MODE_P30_INPUT()               P3M1|=SET_BIT0;P3M2&=~SET_BIT0
#define SPL_GPIO_SET_MODE_P00_OD()                  P0M1|=SET_BIT0;P0M2|=SET_BIT0
#define SPL_GPIO_SET_MODE_P01_OD()                  P0M1|=SET_BIT1;P0M2|=SET_BIT1
#define SPL_GPIO_SET_MODE_P02_OD()                  P0M1|=SET_BIT2;P0M2|=SET_BIT2
#define SPL_GPIO_SET_MODE_P03_OD()                  P0M1|=SET_BIT3;P0M2|=SET_BIT3
#define SPL_GPIO_SET_MODE_P04_OD()                  P0M1|=SET_BIT4;P0M2|=SET_BIT4
#define SPL_GPIO_SET_MODE_P05_OD()                  P0M1|=SET_BIT5;P0M2|=SET_BIT5
#define SPL_GPIO_SET_MODE_P06_OD()                  P0M1|=SET_BIT6;P0M2|=SET_BIT6
#define SPL_GPIO_SET_MODE_P07_OD()                  P0M1|=SET_BIT7;P0M2|=SET_BIT7
#define SPL_GPIO_SET_MODE_P10_OD()                  P1M1|=SET_BIT0;P1M2|=SET_BIT0
#define SPL_GPIO_SET_MODE_P11_OD()                  P1M1|=SET_BIT1;P1M2|=SET_BIT1
#define SPL_GPIO_SET_MODE_P12_OD()                  P1M1|=SET_BIT2;P1M2|=SET_BIT2
#define SPL_GPIO_SET_MODE_P13_OD()                  P1M1|=SET_BIT3;P1M2|=SET_BIT3
#define SPL_GPIO_SET_MODE_P14_OD()                  P1M1|=SET_BIT4;P1M2|=SET_BIT4
#define SPL_GPIO_SET_MODE_P15_OD()                  P1M1|=SET_BIT5;P1M2|=SET_BIT5
#define SPL_GPIO_SET_MODE_P16_OD()                  P1M1|=SET_BIT6;P1M2|=SET_BIT6
#define SPL_GPIO_SET_MODE_P17_OD()                  P1M1|=SET_BIT7;P1M2|=SET_BIT7
#define SPL_GPIO_SET_MODE_P30_OD()                  P3M1|=SET_BIT0;P3M2|=SET_BIT0

#define spl_gpio_write_pin( pin, value )      pin = value
#define spl_gpio_read_pin( pin )              pin
#define spl_gpio_write_port( port, value )    port = value
#define spl_gpio_read_port( port )            port


#endif /* __SPL_GPIO_H__ */

