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
#include "Common.h"
#include "Function_Define.h"
#include "N76E003.h"
#include "SFR_Macro.h"
#include "spl_config.h"
#include "spl_adc.h"

#if (SPL_ADC_EN > 0)

#if (SPL_ADC_BANDGAP_EN > 0)
static xdata uint16_t bandgap_value;
#endif

#if (SPL_ADC_BANDGAP_EN > 0)
static void ADC_Bypass (void)    // The first three times convert should be bypass, to improve accuracy
{ 
    uint8_t ozc; 
    for ( ozc=0; ozc<0x03; ozc++ ) 
    { 
        clr_ADCF;
        set_ADCS; 
        while(ADCF == 0); 
    } 
} 
#endif

extern void     spl_adc_init( void )
{
#if (SPL_ADC_BANDGAP_EN > 0)
    uint8_t bandgap_hi, bandgap_lo;
    //set_IAPEN;
    TA=0xAA;TA=0x55;CHPCON|=SET_BIT0;
    IAPAL = 0x0C; 
    IAPAH = 0x00; 
    IAPCN = 0x04;
    //set_IAPGO;
    TA=0xAA;TA=0x55;IAPTRG|=SET_BIT0;
    bandgap_hi = IAPFD;
    IAPAL = 0x0D;
    IAPAH = 0x00; 
    IAPCN = 0x04;
    //set_IAPGO;
    TA=0xAA;TA=0x55;IAPTRG|=SET_BIT0;
    bandgap_lo = IAPFD;
    bandgap_lo &= 0x0F;
    //clr_IAPEN;
    TA=0xAA;TA=0x55;CHPCON&=~SET_BIT0;
    bandgap_value = (bandgap_hi<<4) | bandgap_lo;
#endif
}

#if (SPL_ADC_BANDGAP_EN > 0)
extern uint16_t spl_adc_get_bandgap( void )
{
    return bandgap_value;
}
#endif

extern uint16_t spl_adc_read( uint8_t ch )
{
    switch( ch )
    {
#if (SPL_ADC_CH0_EN > 0)
        case SPL_ADC_CH_0:
            Enable_ADC_AIN0;
        break;
#endif
#if (SPL_ADC_CH1_EN > 0)
        case SPL_ADC_CH_1:
            Enable_ADC_AIN1;
        break;
#endif
#if (SPL_ADC_CH2_EN > 0)
        case SPL_ADC_CH_2:
            Enable_ADC_AIN2;
        break;
#endif
#if (SPL_ADC_CH3_EN > 0)
        case SPL_ADC_CH_3:
            Enable_ADC_AIN3;
        break;
#endif
#if (SPL_ADC_CH4_EN > 0)
        case SPL_ADC_CH_4:
            Enable_ADC_AIN4;
        break;
#endif
#if (SPL_ADC_CH5_EN > 0)
        case SPL_ADC_CH_5:
            Enable_ADC_AIN5;
        break;
#endif
#if (SPL_ADC_CH6_EN > 0)
        case SPL_ADC_CH_6:
            Enable_ADC_AIN6;
        break;
#endif
#if (SPL_ADC_CH7_EN > 0)
        case SPL_ADC_CH_7:
            Enable_ADC_AIN7;
        break;
#endif
#if (SPL_ADC_BANDGAP_EN > 0)
        default:
            Enable_ADC_BandGap;
            ADC_Bypass();
        break;
#endif
    }
    
    clr_ADCF;
	set_ADCS;
    while(ADCF == 0);
    return ((uint16_t)ADCRH << 4) | (uint16_t)(ADCRL&0x0F);//ADCRH[11:4] stores MSB, ADCRL[3:0] stores LSB;

}


#endif //(SPL_ADC_EN > 0)

