#ifndef __MISC
#define __MISC

#include "stm32f0xx.h"
#include "types.h"


#define I2C_IN_USE              I2C2
#define I2C_BUS                 I2C_IN_USE
#define I2C_rcc                 RCC_APB1Periph_I2C2

#define I2C_SDA_pin             GPIO_Pin_14
#define I2C_SDA_port            GPIOB
#define I2C_SDA_rcc             RCC_AHBPeriph_GPIOB
#define I2C_SDA_AF_source       GPIO_PinSource14

#define I2C_SCL_pin             GPIO_Pin_13
#define I2C_SCL_port            GPIOB
#define I2C_SCL_rcc             RCC_AHBPeriph_GPIOB
#define I2C_SCL_AF_source       GPIO_PinSource13


u16  i2c_readbuf        (u8 adr, u8 reg, u16 len ,u8* buf);
void i2c_gpio_init      (void);
void i2c_init           (void);

//////////////////////////////////////////////





// Вспомогательные переменные
typedef enum _I2C_Direction {I2C_Transmitter=0, I2C_Receiver=1} I2C_Direction;

/*
Битовые смещения для настройки разных регистров
Чтобы не использовать в коде магические числа, и код был читаемым
Например:
I2C->CR2 |= 1<<I2C_OFFSET_CR2_NBYTES;	// Здесь понятно, что передаётся 1 байт
I2C->CR2 |= 1<<16;						// Здесь возникает путаница, то ли 1 байт, то ли 16
*/
#define I2C_OFFSET_TIMINGR_SCLL		0
#define I2C_OFFSET_TIMINGR_SCLH		8
#define I2C_OFFSET_TIMINGR_SDADEL	16
#define I2C_OFFSET_TIMINGR_SCLDEL	20
#define I2C_OFFSET_TIMINGR_PRESC	28
#define I2C_OFFSET_CR2_NBYTES		16


////////////////////////////////////////////////
//    Секция прототипов локальных функций     //
////////////////////////////////////////////////
void i2c_start (void);
void i2c_stop (void);
u8 i2c_write_transaction (u8 Adress, u8 Register, u8 *Data, u8 Size);
u8 i2c_read_transaction (u8 Adress, u8 Register, u8 *Data, u8 Size);
void i2c_start_direction_adress_size (I2C_Direction Direction, u8 Adress, u8 Size);


#endif