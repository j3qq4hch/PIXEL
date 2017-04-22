#ifndef __MISC
#define __MISC

#include "stm32f0xx.h"

#define I2C_IN_USE              I2C2
#define I2C_rcc                 RCC_APB1Periph_I2C2

#define I2C_SDA_pin             GPIO_Pin_14
#define I2C_SDA_port            GPIOB
#define I2C_SDA_rcc             RCC_AHBPeriph_GPIOB
#define I2C_SDA_AF_source       GPIO_PinSource14

#define I2C_SCL_pin             GPIO_Pin_13
#define I2C_SCL_port            GPIOB
#define I2C_SCL_rcc             RCC_AHBPeriph_GPIOB
#define I2C_SCL_AF_source       GPIO_PinSource13

void i2c_gpio_init(void);
void i2c_init(void);
#endif