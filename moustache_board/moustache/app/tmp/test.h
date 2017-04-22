#ifndef __TEST
#define __TEST

#include "stm32f0xx.h"


#define TEST1_pin        GPIO_Pin_6
#define TEST1_port       GPIOB   
#define TEST1_rcc        RCC_AHBPeriph_GPIOB


#define TEST2_pin        GPIO_Pin_7
#define TEST2_port       GPIOB   
#define TEST2_rcc        RCC_AHBPeriph_GPIOB

 
#define TEST1_toggle() ((TEST1_port->ODR  & TEST1_pin)!=0)  ? (TEST1_port->BRR  = TEST1_pin) : ( TEST1_port->BSRR = TEST1_pin)
#define TEST2_toggle() ((TEST2_port->ODR  & TEST2_pin)!=0)  ? (TEST2_port->BRR  = TEST2_pin) : ( TEST2_port->BSRR = TEST2_pin)


void test(void);
void test_pins_init(void);
#endif