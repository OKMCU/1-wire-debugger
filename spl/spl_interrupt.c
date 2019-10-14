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
#include "osal.h"

#if (SPL_UART_EN > 0)
#if (SPL_UART0_EN > 0)
extern void spl_uart0_isr( void );
extern void UART0_IRQHandler(void) interrupt 4  {spl_uart0_isr();}
#endif
#if (SPL_UART1_EN > 0)
extern void spl_uart1_isr( void );
extern void UART1_IRQHandler(void)  {spl_uart1_isr();}
#endif
#endif

#if (SPL_EXTINT_EN > 0)
#if (SPL_EXTINT0_EN > 0)
extern void spl_extint_int0_isr( void );
void EINT0_IRQHandler(void)         {spl_extint_int0_isr();}
#endif
#if (SPL_EXTINT1_EN > 0)
extern void spl_extint_int1_isr( void );
void EINT1_IRQHandler(void)         {spl_extint_int1_isr();}
#endif
#endif

#if (SPL_TIMER_EN > 0)
#if (SPL_TIMER0_EN > 0)
extern void SPL_TIMER0_CALLBACK( void );
void Timer0_ISR (void) interrupt 1  {SPL_TIMER0_CALLBACK();}
#endif//(SPL_TIMER0_EN > 0)
#if (SPL_TIMER1_EN > 0)
extern void SPL_TIMER1_CALLBACK( void );
void Timer1_ISR (void) interrupt 3  {SPL_TIMER1_CALLBACK();}
#endif//(SPL_TIMER1_EN > 0)
#if (SPL_TIMER_SYSTICK_EN > 0)
extern void SPL_TIMER_SYSTICK_CALLBACK( void );
void Timer2_ISR (void) interrupt 5  {clr_TF2; SPL_TIMER_SYSTICK_CALLBACK();}
#endif//(SPL_TIMER_SYSTICK_EN > 0)
#endif//(SPL_TIMER_EN > 0)


