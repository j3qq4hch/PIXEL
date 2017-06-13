#include "bsp.h"

static GPIO_InitTypeDef GPIO_InitStruct; //this is used to init all gpios

static inline void clock_init(void)
{
RCC_APB2PeriphClockCmd  (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO,   ENABLE);
RCC_APB1PeriphClockCmd  (RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4, ENABLE);
RCC_AHBPeriphClockCmd   (RCC_AHBPeriph_DMA1,  ENABLE);
RCC_APB2PeriphClockCmd  (RCC_APB2Periph_ADC1 , ENABLE);
}


static inline void gpio_init(void)
{

GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;

GPIO_InitStruct.GPIO_Pin   = LED_Pin;
GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

GPIO_InitStruct.GPIO_Pin   = TABLE_CLK_Pin;
GPIO_Init(TABLE_CLK_Port, &GPIO_InitStruct);

GPIO_InitStruct.GPIO_Pin   = TABLE_DATA_Pin;
GPIO_Init(TABLE_DATA_Port, &GPIO_InitStruct);

}

//static inline void NVIC_init(void)
//{
//NVIC_InitTypeDef   NVIC_InitStructure;
//NVIC_InitStructure.NVIC_IRQChannel              = USART1_IRQn; 
//NVIC_InitStructure.NVIC_IRQChannelCmd           = ENABLE; 
//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 0;
//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//
//NVIC_Init(&NVIC_InitStructure);
//}
////

void board_init(void)
{
  clock_init();
  gpio_init(); 
//  NVIC_init();
//dbgu_init();
//gsmu_init();
//NVIC_init();
}
//






