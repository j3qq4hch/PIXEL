#include "bsp.h"

static void led_init(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 GPIO_InitStructure.GPIO_Pin = LED_pin;
 GPIO_Init(LED_port, &GPIO_InitStructure);
}


static void rcc_init(void)
{
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);
  RCC_AHBPeriphClockCmd (RCC_AHBPeriph_GPIOA  | RCC_AHBPeriph_GPIOB,   ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14 | RCC_APB1Periph_TIM3,   ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1  | RCC_APB2Periph_USART1, ENABLE);
}


static void rs485_hw_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_PinAFConfig(REDE_port, REDE_pinsource,	GPIO_AF_1 );
	GPIO_PinAFConfig(TX_port, TX_pinsource,		GPIO_AF_1);
	GPIO_PinAFConfig(RX_port, RX_pinsource,		GPIO_AF_1);

	GPIO_InitStructure.GPIO_Pin = TX_pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(TX_port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = REDE_pin;
	GPIO_Init(REDE_port, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = RX_pin;
        GPIO_Init(RX_port, &GPIO_InitStructure);

	USART_DECmd(USART1,  ENABLE);
	USART_SetDEAssertionTime(USART1, 4);
	USART_SetDEDeassertionTime(USART1, 4);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


static void adc_init(void)
{

}


void board_init(void)
{
	rcc_init();
	led_init();
//	adc_init();
//	rs485_hw_init();
}

