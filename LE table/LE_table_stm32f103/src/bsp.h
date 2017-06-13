#include "stm32f10x.h"

#define LED_Pin                GPIO_Pin_13
#define LED_GPIO_Port          GPIOC

#define TABLE_CLK_Port          GPIOB
#define TABLE_CLK_Pin           GPIO_Pin_10

#define TABLE_DATA_Port          GPIOB
#define TABLE_DATA_Pin           GPIO_Pin_1

#define TABLE_CLK_TICK()        (GPIO_SetBits(TABLE_CLK_Port, TABLE_CLK_Pin), GPIO_ResetBits(TABLE_CLK_Port, TABLE_CLK_Pin))

#define DBG_UART                USART1
#define DBG_TX_Pin              GPIO_Pin_9
#define DBG_TX_GPIO_Port        GPIOA

#define DBG_RX_Pin              GPIO_Pin_10
#define DBG_RX_GPIO_Port        GPIOA

#define SENS1_Pin       GPIO_Pin_4
#define SENS1_Port      GPIOA
//adc_in4


#define SENS2_Pin       GPIO_Pin_5
#define SENS2_Port      GPIOA
//adc_in5


#define SENS3_Pin       GPIO_Pin_6
#define SENS3_Port      GPIOA
//adc_in6


#define SENS4_Pin       GPIO_Pin_7
#define SENS4_Port      GPIOA
//adc_in7

void board_init(void);

static inline void GPIO_toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
(GPIOx->ODR & GPIO_Pin) ? (GPIOx->BRR = GPIO_Pin) : (GPIOx->BSRR = GPIO_Pin);
}





