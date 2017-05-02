
#ifndef __PWM
#define __PWM


#include "stm32f0xx.h"
#include "types.h"


typedef struct
{
GPIO_TypeDef*   GPIO_port;
uint16_t        GPIO_Pin;
uint8_t         GPIO_PinSource;
uint8_t         GPIO_AF;

TIM_TypeDef* TIM;
u8 TIM_ch;
}pwm_ch_t;

static void pwm_init_ch(const pwm_ch_t* pwm);
void pwm_setval(const pwm_ch_t* pwm, u32 val);
void pwm_init(void);
u16  pwm_get_period(void);
void led_driver_init(void);

#endif