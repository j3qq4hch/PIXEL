#include "dbg.h"
#include "io.h"
#include "stm32f0xx.h"
#include "bsp.h"


static  USART_InitTypeDef     settings = 
{
.USART_BaudRate                 = 115200,
.USART_WordLength               = USART_WordLength_8b,
.USART_StopBits                 = USART_StopBits_1,
.USART_Parity                   = USART_Parity_No,
.USART_Mode                     = USART_Mode_Tx|USART_Mode_Rx,
.USART_HardwareFlowControl      = USART_HardwareFlowControl_None,
};


void pipe_test(void)
{
  
  GPIO_SetBits(GSM_PWR_EN_Port, GSM_PWR_EN_Pin);
  
  for(u32 i = 0; i < 1000000; i++){}
  
  GPIO_ResetBits(GSM_PWR_EN_Port,  GSM_PWR_EN_Pin);
  GPIO_ResetBits(GSM_PWR_KEY_Port, GSM_PWR_KEY_Pin);
  

  for(u32 i = 0; i < 5000000; i++){}

  
  GPIO_SetBits(GSM_PWR_KEY_Port, GSM_PWR_KEY_Pin);  
  GPIO_ResetBits(LED1_GPIO_Port, LED1_Pin);
  GPIO_ResetBits(LED2_GPIO_Port, LED2_Pin);

  USART_Ini t(DBG_UART, &settings);
  USART_ITConfig(DBG_UART, USART_IT_RXNE, ENABLE); 
  USART_Cmd(DBG_UART, ENABLE);
 
  USART_Init(GSM_UART, &settings);
  USART_ITConfig(GSM_UART, USART_IT_RXNE, ENABLE); 
  USART_Cmd(GSM_UART, ENABLE);

  while(1){} 
}