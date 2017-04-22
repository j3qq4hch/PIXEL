#include "pwm.h"
#include "bsp.h"

static inline void TIM_OCInit(TIM_TypeDef* TIMx, u8 ch, TIM_OCInitTypeDef* TIM_OCInitStruct);

u16 pwm_period_ticks = 1024;

const pwm_ch_t pwm_array[] =
{
  {PWM1_gpio_port, PWM1_gpio_pin, PWM1_gpio_pinsource, PWM1_gpio_pin_af, TIM14, 1},
  {PWM2_gpio_port, PWM2_gpio_pin, PWM2_gpio_pinsource, PWM2_gpio_pin_af, TIM3,  1},
  {PWM3_gpio_port, PWM3_gpio_pin, PWM3_gpio_pinsource, PWM3_gpio_pin_af, TIM3,  2},
  {PWM4_gpio_port, PWM4_gpio_pin, PWM4_gpio_pinsource, PWM4_gpio_pin_af, TIM3,  4},
  {PWM5_gpio_port, PWM5_gpio_pin, PWM5_gpio_pinsource, PWM5_gpio_pin_af, TIM1,  2},
  {PWM6_gpio_port, PWM6_gpio_pin, PWM6_gpio_pinsource, PWM6_gpio_pin_af, TIM1,  3},
};




static inline void TIM_OCInit(TIM_TypeDef* TIMx, 
                              u8 ch, 
                              TIM_OCInitTypeDef* TIM_OCInitStruct)
{
if(ch == 1) TIM_OC1Init(TIMx, TIM_OCInitStruct);
if(ch == 2) TIM_OC2Init(TIMx, TIM_OCInitStruct);
if(ch == 3) TIM_OC3Init(TIMx, TIM_OCInitStruct);
if(ch == 4) TIM_OC4Init(TIMx, TIM_OCInitStruct);
}

static inline void TIM_SetCompare(TIM_TypeDef* TIMx, u8 ch, uint32_t Compare)
{
if(ch == 1) TIM_SetCompare1(TIMx, Compare);
if(ch == 2) TIM_SetCompare2(TIMx, Compare);
if(ch == 3) TIM_SetCompare3(TIMx, Compare);
if(ch == 4) TIM_SetCompare4(TIMx, Compare);
}

////////////////////////////////////////////

void pwm_setval(const pwm_ch_t* pwm, u32 val)
{
  TIM_SetCompare(pwm->TIM, pwm->TIM_ch, val);
}

/////////////////////////////////////////////

static void pwm_init_ch(const pwm_ch_t* pwm)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  TIM_Cmd(pwm->TIM, DISABLE); 
  
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  
  GPIO_InitStructure.GPIO_Pin = pwm->GPIO_Pin;
  GPIO_Init(pwm->GPIO_port, &GPIO_InitStructure);
  GPIO_PinAFConfig(pwm->GPIO_port, pwm->GPIO_PinSource, pwm->GPIO_AF); 
  
  TIM_TimeBaseStructure.TIM_Prescaler           = 0;
  TIM_TimeBaseStructure.TIM_CounterMode         = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period              = pwm_period_ticks;
  TIM_TimeBaseStructure.TIM_ClockDivision       = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter   = 0;

  TIM_TimeBaseInit(pwm->TIM, &TIM_TimeBaseStructure);

  TIM_OCInitStructure.TIM_OCMode        = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState   = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse         = 0;
  TIM_OCInitStructure.TIM_OCPolarity    = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState   = TIM_OCIdleState_Set;
  
  TIM_OCInit(pwm->TIM, pwm->TIM_ch,  &TIM_OCInitStructure);
  TIM_Cmd(pwm->TIM, ENABLE);
  TIM_CtrlPWMOutputs(pwm->TIM, ENABLE);
}

void pwm_init(void)
{
  for(u8 i = 0; i < ARRAY_SIZE(pwm_array); i++)
  {
  pwm_init_ch(&pwm_array[i]);
  }
}

u16 pwm_get_period(void)
{
return pwm_period_ticks;
}