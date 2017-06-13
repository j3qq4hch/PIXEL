
#include "bsp.h"
#include "Freertos.h"
#include "task.h"
#include "pwm.h"

#include "settings.h"
#include "modbus_app.h"

void blinker    (void *p);

u16 blink_period = 100;
extern modbus_app_param_t modbus_app_param ;


#define LED_DRV_FREQ    150000
#define TIMER_FREQ      48000000

#define PERIOD_TICKS TIMER_FREQ / LED_DRV_FREQ

int main(void)
{ 

  board_init();
  u32 resolution = PERIOD_TICKS;
  
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT  ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL ;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
GPIO_Init(GPIOA, &GPIO_InitStructure);
while(1)
{
 GPIO_SetBits(GPIOA,  GPIO_Pin_10);
 for(u8 i = 0; i< 0xFF; i++){}
 GPIO_ResetBits(GPIOA,  GPIO_Pin_10);
for(u8 i = 0; i< 0xFF; i++){}
}

  
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL ;
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_2); 


  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  TIM_Cmd(TIM1, DISABLE); 
 
  TIM_TimeBaseStructure.TIM_Prescaler           = 0;
  TIM_TimeBaseStructure.TIM_CounterMode         = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period              = PERIOD_TICKS;
  TIM_TimeBaseStructure.TIM_ClockDivision       = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter   = 0;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  TIM_OCInitStructure.TIM_OCMode        = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState   = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse         = 0;
  TIM_OCInitStructure.TIM_OCPolarity    = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState   = TIM_OCIdleState_Set;
    
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);
  
  TIM_Cmd(TIM1, ENABLE);
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
  
  TIM_SetCompare3(TIM1, PERIOD_TICKS);  

  while(1){}
  
  
  xTaskCreate(blinker,                    /* Function that implements the task. */
              "blink",                    /* Name */
              configMINIMAL_STACK_SIZE,   /* Stack size in words, not bytes. */
              ( void * ) &blink_period,   /* Parameter passed into the task. */
              tskIDLE_PRIORITY + 1,       /* Priority at which the task is created. */
              NULL );  


//  xTaskCreate(modbus_app,                     /* Function that implements the task. */
//              "modbus",                       /* Name */
//              configMINIMAL_STACK_SIZE * 2,   /* Stack size in words, not bytes. */
//              ( void * ) &modbus_app_param,   /* Parameter passed into the task. */
//              tskIDLE_PRIORITY + 1,           /* Priority at which the task is created. */
//              NULL );  
  
  vTaskStartScheduler();
}



void blinker(void *p)
{

while(1)
{
LED_toggle();  
vTaskDelay( 100 / portTICK_PERIOD_MS );
}
}


