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
#ifndef __SPL_ADC_H__
#define __SPL_ADC_H__


#include <stdint.h>
#include "spl_config.h"

#if (SPL_ADC_CH0_EN > 0)
#define     SPL_ADC_CH_0                 0
#endif
#if (SPL_ADC_CH1_EN > 0)
#define     SPL_ADC_CH_1                 1
#endif
#if (SPL_ADC_CH2_EN > 0)
#define     SPL_ADC_CH_2                 2
#endif
#if (SPL_ADC_CH3_EN > 0)
#define     SPL_ADC_CH_3                 3
#endif
#if (SPL_ADC_CH4_EN > 0)
#define     SPL_ADC_CH_4                 4
#endif
#if (SPL_ADC_CH5_EN > 0)
#define     SPL_ADC_CH_5                 5
#endif
#if (SPL_ADC_CH6_EN > 0)
#define     SPL_ADC_CH_6                 6
#endif
#if (SPL_ADC_CH7_EN > 0)
#define     SPL_ADC_CH_7                 7
#endif
#if (SPL_ADC_BANDGAP_EN > 0)
#define     SPL_ADC_CH_BANDGAP           8
#endif

#define SPL_ADC_VALUE_MAX                4095

#if (SPL_ADC_BANDGAP_EN > 0)
#define     SPL_ADC_BANDGAP_VOLTAGE(bandgap_value)  ((double)bandgap_value*3072/4096)
#endif

extern void     spl_adc_init( void );

#if (SPL_ADC_BANDGAP_EN > 0)
extern uint16_t spl_adc_get_bandgap( void );
#endif

extern uint16_t spl_adc_read( uint8_t ch );


#endif /* __SPL_ADC_H__ */

