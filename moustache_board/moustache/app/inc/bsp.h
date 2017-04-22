#ifndef __BSP
#define __BSP

#include "stm32f0xx.h"
#include "types.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#define LED_port        GPIOA
#define LED_pin         GPIO_Pin_11

#define LED_on()        LED_port->BSRR |= LED_pin        
#define LED_off()       LED_port->BRR  |= LED_pin
#define LED_toggle()     ((LED_port->ODR & LED_pin) ? (LED_off()) : (LED_on()))

//////////////////////////////////////////////
// hardware REDE management
#define REDE_port       GPIOA
#define REDE_pin        GPIO_Pin_1
#define REDE_pinsource	GPIO_PinSource1

#define TX_port         GPIOA
#define TX_pin          GPIO_Pin_2
#define TX_pinsource	GPIO_PinSource2

#define RX_port         GPIOA
#define RX_pin          GPIO_Pin_3
#define RX_pinsource	GPIO_PinSource3

////////////////////////////////////////////////

#define PWM1_gpio_port            GPIOA
#define PWM1_gpio_pin             GPIO_Pin_10
#define PWM1_gpio_pinsource       GPIO_PinSource10
#define PWM1_gpio_pin_af          GPIO_AF_0
//tim1_CH3

#define PWM2_gpio_port            GPIOA
#define PWM2_gpio_pin             GPIO_Pin_9
#define PWM2_gpio_pinsource       GPIO_PinSource9
#define PWM2_gpio_pin_af          GPIO_AF_0
//tim1_CH2

#define PWM3_gpio_port            GPIOA
#define PWM3_gpio_pin             GPIO_Pin_8
#define PWM3_gpio_pinsource       GPIO_PinSource8
#define PWM3_gpio_pin_af          GPIO_AF_0
//tim3_CH1

#define LED1_CURRENT_SENSE_port         GPIOA
#define LED1_CURRENT_SENSE_pin          GPIO_Pin_6
//AIN6

#define LED2_CURRENT_SENSE_port         GPIOA
#define LED2_CURRENT_SENSE_pin          GPIO_Pin_7
//AIN7

#define LED3_CURRENT_SENSE_port         GPIOB
#define LED3_CURRENT_SENSE_pin          GPIO_Pin_0
//AIN8

#define VIN_SENSE_port                  GPIOA
#define VIN_SENSE_pin                   GPIO_Pin_5
//AIN5

#define TEMP_SENSE_port                  GPIOA
#define TEMP_SENSE_pin                   GPIO_Pin_4
//AIN4

////////////////////////////////////////////////

#define CS_port         GPIOA
#define CS_pin          GPIO_Pin_15

#define CS_OUT_port     GPIOA
#define CS_OUT_pin      GPIO_Pin_12

#define CLOCK_port      GPIOB
#define CLOCK_pin       GPIO_Pin_3

#define MISO_port       GPIOB
#define MISO_pin        GPIO_Pin_4

#define MOSI_port       GPIOB
#define MOSI_pin        GPIO_Pin_5

/////////////////////////////////////////////////

void board_init(void);
#endif
