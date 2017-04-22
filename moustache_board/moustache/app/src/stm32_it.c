


/* Includes ------------------------------------------------------------------*/
#include "stm32_it.h"
#include "stm32f0xx.h"
#include "uart_int.h"
#include "bsp.h"


void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
 
  while(1) 
  {
  u32 zz = 300000;
  while(zz){zz--;}
  LED_toggle();
  }  
}
  
/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f072.s).                                            */
/******************************************************************************/
void TIM2_IRQHandler(void)
{
}


void TIM3_IRQHandler(void)
{
}

void ADC1_IRQHandler(void)
{
}

//extern uart_desc_t RS485;
void USART1_IRQHandler(void)
{
//uart_isr(&RS485);
}

void DMA1_Channel1_IRQHandler(void)
{
 
}