#include "le_table.h"
#include "bsp.h"

#define TABLE_PCB_NUM   2 //

#define SYS_FREQ_HZ     72000000
#define TIMER_PERIOD    90


#define WS_Port          GPIOB
#define WS_Pin           GPIO_Pin_0

#define DMA_CHANNEL     DMA1_Channel2
#define DMA_IT_TC_FLAG  DMA1_IT_TC2
#define DMA_IT_HT_FLAG  DMA1_IT_HT2
#define DMA_NVIC_IRQ    DMA1_Channel2_IRQn
#define TIMER           TIM3


#define SENSOR_POLL_PERIOD_uS   500     

#define ADC_TIMER       TIM4
#define ADC_DMA_CHANNEL DMA1_Channel1
#define ADC_DMA_IT_TC_FLAG  DMA1_IT_TC1
#define ADC_DMA_IT_HT_FLAG  DMA1_IT_HT1

#define LOW_DURATION    (TIMER_PERIOD * 10 / 35)
#define HIGH_DURATION   (LOW_DURATION * 2)

#define BUFLEN          48


static u16 ws_buf[BUFLEN];
static u8 finish_flag = 0;
static u8 points_2_send = 0;
static ws2812_t * ws_array = {0};

static u16 sensor_data[TABLE_PCB_NUM * 4];
static u16 sensor_counter  = 0;
////////////////////////////////////////////////////////////

static DMA_InitTypeDef dma = 
{
	.DMA_PeripheralBaseAddr = 0,
	.DMA_MemoryBaseAddr = 0,
	.DMA_DIR = DMA_DIR_PeripheralDST,
	.DMA_BufferSize = 8,
	.DMA_PeripheralInc = DMA_PeripheralInc_Disable,
	.DMA_MemoryInc = DMA_MemoryInc_Enable,
	.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord,
	.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord,
	.DMA_Mode = DMA_Mode_Circular,//DMA_Mode_Normal
	.DMA_Priority = DMA_Priority_High,
	.DMA_M2M = DMA_M2M_Disable
};

static GPIO_InitTypeDef gpio = 
{
.GPIO_Mode  = GPIO_Mode_AF_PP,
.GPIO_Speed = GPIO_Speed_50MHz,
.GPIO_Pin   = GPIO_Pin_0
};
  
static TIM_TimeBaseInitTypeDef  timebase = 
{
  .TIM_Prescaler = 0,
  .TIM_CounterMode = TIM_CounterMode_Up,
  .TIM_Period = TIMER_PERIOD,
  .TIM_ClockDivision = 0,
  .TIM_RepetitionCounter = 0
};
  
  
TIM_OCInitTypeDef  pwm_init = 
  {
  .TIM_OCMode = TIM_OCMode_PWM1,
  .TIM_OutputState = TIM_OutputState_Enable,
  .TIM_OutputNState = TIM_OutputNState_Enable,
  .TIM_Pulse = 0,
  .TIM_OCPolarity  = TIM_OCPolarity_High,
  .TIM_OCNPolarity = TIM_OCNPolarity_High,
  .TIM_OCIdleState = TIM_OCIdleState_Set,
  .TIM_OCNIdleState = TIM_OCIdleState_Reset
  };

  
NVIC_InitTypeDef   NVIC_InitStructure =
  {
  .NVIC_IRQChannel              = DMA_NVIC_IRQ,
  .NVIC_IRQChannelCmd           = ENABLE, 
  .NVIC_IRQChannelPreemptionPriority    = 0,
  .NVIC_IRQChannelSubPriority   = 0,
  };

////////////////////////////////////////////////////////////
  
void ws_init(void)
{  
  GPIO_Init(GPIOB, &gpio);  
  //GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE); 
  
  dma.DMA_PeripheralBaseAddr = (uint32_t)&TIMER->CCR3; //this must be corrected in case of any changes 
  dma.DMA_MemoryBaseAddr = (uint32_t)(ws_buf),
  DMA_Init      (DMA_CHANNEL, &dma);
  DMA_ITConfig  (DMA_CHANNEL, DMA_IT_TC | DMA_IT_HT, ENABLE); 
  
  TIM_TimeBaseInit      (TIMER, &timebase);
  TIM_OC3Init           (TIMER, &pwm_init);  
  TIM_ARRPreloadConfig  (TIMER, ENABLE);
  TIM_OC3PreloadConfig  (TIMER, TIM_OCPreload_Enable);
  TIM_DMACmd            (TIMER, TIM_DMA_CC3,ENABLE);
  TIM_GenerateEvent     (TIMER, TIM_EventSource_Update);
  TIM_CtrlPWMOutputs    (TIMER, ENABLE);
  TIM_Cmd               (TIMER, ENABLE);
  NVIC_Init             (&NVIC_InitStructure);
}

void ws2812_2_pwm(ws2812_t * w, u16 * buf)
{
u8 mask = 0x80;
if(w)
{
for(u8 i=0; i < 8; i++ )
{ 
  (w->green   & mask) ? (buf[i]      = HIGH_DURATION ) : (buf[i]      = LOW_DURATION);
  (w->red     & mask) ? (buf[i+8]    = HIGH_DURATION ) : (buf[i+8]    = LOW_DURATION);
  (w->blue    & mask) ? (buf[i+16]   = HIGH_DURATION ) : (buf[i+16]   = LOW_DURATION);
   mask >>=1;
}
}
else
{
  for(u8 i=0; i < 24; i++ )
  {
    buf[i] = 0;
  }
  }
}

//sends N ws2812 pixels 
void ws2812_send(ws2812_t * w, u16 n)
{
  
if(!points_2_send)
  {   
    ws_array  = w;
    points_2_send = n;
    ws2812_2_pwm(0,ws_buf);
    ws2812_2_pwm(ws_array++, ws_buf + 24);
    points_2_send--;    
    finish_flag = 0;
    DMA_SetCurrDataCounter(DMA1_Channel2, 48);
    DMA1_Channel1->CMAR = (u32)ws_buf;
    DMA_Cmd(DMA_CHANNEL, ENABLE);
  }
}


void table_dma_isr(void)
{
  u16 * p = 0;
  
  if(DMA_GetITStatus(DMA_IT_HT_FLAG) == SET)
  {
  p = ws_buf;
  DMA_ClearITPendingBit(DMA_IT_HT_FLAG);
  }
  
  if(DMA_GetITStatus(DMA_IT_TC_FLAG) == SET)
  {
  p = ws_buf+24;
  DMA_ClearITPendingBit(DMA_IT_TC_FLAG);
  }

  if(points_2_send)
  {
  ws2812_2_pwm(ws_array++, p);
  points_2_send--;
  }
  else 
  {
    if(finish_flag )
    {
      DMA_Cmd(DMA_CHANNEL, DISABLE); 
    }
    else
    {
       ws2812_2_pwm(0, p);
      finish_flag = 1;
    }
  }
}


u16 adc_buf[4] = {0xAAAA,0xBBBB, 0xCCCC, 0xDDDD};


void adc_isr(void);
void adc_isr(void)
{
GPIO_toggle(LED_GPIO_Port, LED_Pin);
GPIO_toggle(LED_GPIO_Port, LED_Pin);
}
  
void ADC_DMA_init(void);

ADC_InitTypeDef ADC_InitStructure = 
{
.ADC_ContinuousConvMode = DISABLE,
.ADC_DataAlign = ADC_DataAlign_Right,
.ADC_Mode = ADC_Mode_Independent,
.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T4_CC4,
.ADC_Mode = ADC_Mode_Independent,
.ADC_ScanConvMode = ENABLE,
.ADC_NbrOfChannel = 4,
};


void ADC_DMA_init(void)
{
  RCC_ADCCLKConfig(RCC_PCLK2_Div6);
  
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
  NVIC_Init(&NVIC_InitStructure);
 
  DMA_ITConfig  (DMA1_Channel1, DMA_IT_TC, ENABLE); 
    
  dma.DMA_BufferSize = 4;
  dma.DMA_DIR = DMA_DIR_PeripheralSRC;
  dma.DMA_M2M = DMA_M2M_Disable;
  dma.DMA_MemoryBaseAddr = (uint32_t)adc_buf;
  dma.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
  dma.DMA_Mode = DMA_Mode_Circular;
  dma.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
  dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  dma.DMA_Priority = DMA_Priority_High;
  DMA_Init(DMA1_Channel1, &dma);
  DMA_Cmd(DMA1_Channel1 , ENABLE ) ;

  gpio.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  gpio.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &gpio);
  
  ADC_Init(ADC1, &ADC_InitStructure);
    
  ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_13Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_13Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 3, ADC_SampleTime_13Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 4, ADC_SampleTime_13Cycles5);
  ADC_Cmd(ADC1 , ENABLE ) ;
  ADC_DMACmd(ADC1 , ENABLE ) ;
  ADC_ExternalTrigConvCmd (ADC1, ENABLE);
  ADC_ResetCalibration(ADC1);
  
  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));
    
  
  timebase.TIM_Prescaler = 3;
  timebase.TIM_Period = SYS_FREQ_HZ / ( 2000 * 4); //this is to have events every 500 uS
  TIM_TimeBaseInit      (ADC_TIMER, &timebase);
  
  pwm_init.TIM_OCMode           = TIM_OCMode_PWM1,
  pwm_init.TIM_OutputState      = TIM_OutputState_Enable,
  pwm_init.TIM_OutputNState     = TIM_OutputNState_Disable,
  pwm_init.TIM_Pulse            = timebase.TIM_Period / 2; 
  TIM_OC4Init(ADC_TIMER, &pwm_init);  
 
  TIM_ARRPreloadConfig  (ADC_TIMER, ENABLE);
  TIM_OC4PreloadConfig  (ADC_TIMER, TIM_OCPreload_Enable);
  TIM_GenerateEvent     (ADC_TIMER, TIM_EventSource_Update);
}

static u8 SENSOR_POLL_COMPLETE_FLAG = 0;

void sensor_poll(void)
{
sensor_counter = 0;
SENSOR_POLL_COMPLETE_FLAG = 0;
GPIO_SetBits(TABLE_DATA_Port, TABLE_DATA_Pin);
TABLE_CLK_TICK();
GPIO_ResetBits(TABLE_DATA_Port, TABLE_DATA_Pin);

TIM_Cmd(ADC_TIMER, ENABLE);

while(!SENSOR_POLL_COMPLETE_FLAG){}
asm("NOP");
}


void adc_dma_isr(void);




void adc_dma_isr(void)
{
  
  TABLE_CLK_TICK();
  sensor_data[sensor_counter++] = adc_buf[0];
  sensor_data[sensor_counter++] = adc_buf[1];
  sensor_data[sensor_counter++] = adc_buf[2];
  sensor_data[sensor_counter++] = adc_buf[3];
  
  if(sensor_counter >= TABLE_PCB_NUM * 4)
  {
    SENSOR_POLL_COMPLETE_FLAG = 1;
    TIM_Cmd(ADC_TIMER, DISABLE);
  }
  
  DMA_ClearITPendingBit(ADC_DMA_IT_TC_FLAG);
}
