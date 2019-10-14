/******************************************************************************

 @file  spl_config.h

 @brief 

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 
 *****************************************************************************/

#ifndef __SPL_CONFIG_H__
#define __SPL_CONFIG_H__

#include "stdint.h"

#define SPL_MCU_XTAL_EN                     0
#define SPL_UART_EN                         1
#define SPL_UART0_EN                        1
#define SPL_UART1_EN                        0
#define SPL_GPIO_EN                         1
#define SPL_I2C_EN                          0
#define SPL_I2C_MASTER_EN                   0
#define SPL_I2C_SLAVE_EN                    0
#define SPL_EXTINT_EN                       0
#define SPL_EXTINT0_EN                      0
#define SPL_EXTINT1_EN                      0
#define SPL_ADC_EN                          0
#define SPL_ADC_CH0_EN                      0
#define SPL_ADC_CH1_EN                      0
#define SPL_ADC_CH2_EN                      0
#define SPL_ADC_CH3_EN                      0
#define SPL_ADC_CH4_EN                      0
#define SPL_ADC_CH5_EN                      0
#define SPL_ADC_CH6_EN                      0
#define SPL_ADC_CH7_EN                      0
#define SPL_ADC_BANDGAP_EN                  0
#define SPL_SYSCLK_EN                       1
#define SPL_SYSCLK_TRIM_EN                  0
#define SPL_SYSCLK_CLKO_EN                  1
#define SPL_TIMER_EN                        1
#define SPL_TIMER_SYSTICK_EN                1
#define SPL_TIMER0_EN                       0
#define SPL_TIMER1_EN                       0

#if (SPL_UART_EN > 0)
#if (SPL_UART0_EN > 0)
#define SPL_UART0_BAUDRATE                   9600
#define SPL_UART0_RX_CACHE_SIZE              8
#define SPL_UART0_TX_CACHE_SIZE              0
#define SPL_UART0_CALLBACK                   spl_uart0_callback
#endif

#if (SPL_UART1_EN > 0)
#define SPL_UART1_BAUDRATE                   9600
#define SPL_UART1_RX_CACHE_SIZE              0
#define SPL_UART1_TX_CACHE_SIZE              0
#define SPL_UART1_CALLBACK                   spl_uart1_callback
#endif
#endif

#if (SPL_I2C_EN > 0)
#if (SPL_I2C_MASTER_EN > 0)
#define SPL_I2C_CLOCK                       100000
#endif
#if (SPL_I2C_SLAVE_EN > 0)
#define SPL_I2C_SLAVE_ADDR                  0x4A
#define SPL_I2C_CALLBACK_ADDRW              spl_i2c_callback_addrw
#define SPL_I2C_CALLBACK_ADDRR              spl_i2c_callback_addrr
#define SPL_I2C_CALLBACK_RXD                spl_i2c_callback_rxd
#define SPL_I2C_CALLBACK_TXD                spl_i2c_callback_txd
#define SPL_I2C_CALLBACK_FRMEND             spl_i2c_callback_frmend
#endif
#endif

#if (SPL_EXTINT_EN > 0)
#if (SPL_EXTINT0_EN > 0)
#define SPL_EXTINT_INT0_CALLBACK            spl_extint_int0_callback
#endif
#if (SPL_EXTINT1_EN > 0)
#define SPL_EXTINT_INT1_CALLBACK            spl_extint_int1_callback
#endif
#endif

#if (SPL_SYSCLK_EN > 0)
#define SPL_SYSCLK_TRIM_BASE                15      //0 == 16.00 MHz, about 40kHz per step
#define SPL_SYSCLK                          (16000000L+40000L*SPL_SYSCLK_TRIM_BASE)
#if (SPL_SYSCLK_CLKO_EN > 0)
#define SPL_SYSCLK_CLKO_DEF_LVL             0       //should be 0 or 1
#endif //(SPL_SYSCLK_CLKO_EN > 0)
#else  //(SPL_SYSCLK_EN > 0)
#define SPL_SYSCLK                          16000000L
#endif //(SPL_SYSCLK_EN > 0)

#if (SPL_TIMER_EN > 0)
#if (SPL_TIMER_SYSTICK_EN > 0)
#define SPL_TIMER_SYSTICK_FREQ              1000
#define SPL_TIMER_SYSTICK_CALLBACK          osal_timer_systick_inc
#endif //(SPL_TIMER_SYSTICK_EN > 0)
#if (SPL_TIMER0_EN > 0)
#define SPL_TIMER0_CALLBACK                 
#endif //(SPL_TIMER0_EN > 0)
#if (SPL_TIMER1_EN > 0)
#define SPL_TIMER1_CALLBACK                 
#endif //(SPL_TIMER1_EN > 0)
#endif //(SPL_TIMER_EN > 0)

/**************************************************************************************************
 *                                        FUNCTIONS - API
 **************************************************************************************************/

#endif

