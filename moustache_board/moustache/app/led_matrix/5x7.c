#include "5x7.h"
#include "bitdef.h"
#include "5x7_font.h"
#include "uptime.h"
#include <string.h>
static const pin_init_t col_pins[COL_NUM] =
{
 {COL1_pin,      COL1_port,    COL1_rcc,        0},
 {COL2_pin,      COL2_port,    COL2_rcc,        0},
 {COL3_pin,      COL3_port,    COL3_rcc,        0},
 {COL4_pin,      COL4_port,    COL4_rcc,        0},
 {COL5_pin,      COL5_port,    COL5_rcc,        0},
 {COL6_pin,      COL6_port,    COL6_rcc,        0},
 {COL7_pin,      COL7_port,    COL7_rcc,        0},
};

static const pin_init_t row_pins[ROW_NUM] =
{
  {ROW1_pin,      ROW1_port,    ROW1_rcc,       ROW1_AF},
  {ROW2_pin,      ROW2_port,    ROW2_rcc,       ROW2_AF},
  {ROW3_pin,      ROW3_port,    ROW3_rcc,       ROW3_AF},
  {ROW4_pin,      ROW4_port,    ROW4_rcc,       ROW4_AF},
  {ROW5_pin,      ROW5_port,    ROW5_rcc,       ROW5_AF}
};


static const pin_t col_scan_straight[COL_NUM] =
{
  {COL7_pin,      COL7_port},
  {COL6_pin,      COL6_port},
  {COL5_pin,      COL5_port},
  {COL4_pin,      COL4_port},
  {COL3_pin,      COL3_port},
  {COL2_pin,      COL2_port},
  {COL1_pin,      COL1_port}
};

static const pin_t col_scan_reverse[COL_NUM] =
{
 {COL1_pin,      COL1_port},
 {COL2_pin,      COL2_port},
 {COL3_pin,      COL3_port},
 {COL4_pin,      COL4_port},
 {COL5_pin,      COL5_port},
 {COL6_pin,      COL6_port},
 {COL7_pin,      COL7_port}
};

volatile u32 * row_scan_pwm_straight[]=
{
 &PWM_CHANNEL_0,
 &PWM_CHANNEL_1,
 &PWM_CHANNEL_2,
 &PWM_CHANNEL_3,
 &PWM_CHANNEL_4
};

volatile u32 *   row_scan_pwm_reverse[]=
{
 &PWM_CHANNEL_4,
 &PWM_CHANNEL_3, 
 &PWM_CHANNEL_2,
 &PWM_CHANNEL_1,
 &PWM_CHANNEL_0,
};

volatile u32** row_scan;
static const pin_t*  col_scan;
void(*scan_complete)(void);
static u8 opened_flag = 0;
static u8* frame;
static u8 col = 0;

void led_matrix_isr_handler(void)
{
col_scan[col].port->BRR |= col_scan[col].pin; //turn of column
col++;
if(col == COL_NUM)
{
  col = 0;
  scan_complete();
}

for(u8 row = 0; row < ROW_NUM; row++)
{
*row_scan[row] = frame[col+row*COL_NUM];
}
col_scan[col].port->BSRR |= col_scan[col].pin;
TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}


void led_matrix_init(void)
{
led_matrix_gpio_columns_init();
led_matrix_gpio_rows_init();
led_matrix_timers_init();
}

//////////////////////////////////////

void led_matrix_set_mode(matrix_mode_t mode)
{
if(mode == REVERSE)
  {
  row_scan      = row_scan_pwm_reverse;
  col_scan      = col_scan_reverse;
  }
  else
  {
  row_scan      = row_scan_pwm_straight;
  col_scan      = col_scan_straight;
  }
}

//////////////////////////////////////////

void led_matrix_gpio_columns_init(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

for(u8 i=0;i<COL_NUM;i++)
{
RCC_AHBPeriphClockCmd(col_pins[i].rcc,ENABLE);
GPIO_InitStructure.GPIO_Pin  = col_pins[i].pin;
GPIO_Init(col_pins[i].port, &GPIO_InitStructure);
col_pins[i].port->BRR |= col_pins[i].pin;
}
}

///////////////////////////////////////////////////////////

void led_matrix_gpio_rows_init(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
for(u8 i=0;i<ROW_NUM;i++)
{
RCC_AHBPeriphClockCmd(row_pins[i].rcc,ENABLE);
GPIO_InitStructure.GPIO_Pin  = row_pins[i].pin;
GPIO_Init(row_pins[i].port, &GPIO_InitStructure);
GPIO_PinAFConfig(row_pins[i].port, row_pins[i].af, GPIO_AF_2); 
}
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

void led_matrix_timers_init(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,   ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,   ENABLE);
  
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

/////////////////////////////////////////////////
  
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  u32 prescaler = TIMER_TICK_LEN*SYSCLK_HZ/1000000; 
  TIM_TimeBaseStructure.TIM_Prescaler           = (uint16_t)(prescaler - 1); 
  TIM_TimeBaseStructure.TIM_Period              = GRAYSCALE_STEPS; 
  TIM_TimeBaseStructure.TIM_ClockDivision       = 0;
  TIM_TimeBaseStructure.TIM_CounterMode         = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /////////////////////////////////////////////////
  
  TIM_OCInitTypeDef     TIM_OCInitStructure;
  TIM_OCInitStructure.TIM_OCMode        = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState   = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState  = TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_OCPolarity    = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState   = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_Pulse         = 0;
  
  TIM_OC1Init(TIM2, &TIM_OCInitStructure);
  TIM_OC2Init(TIM2, &TIM_OCInitStructure);
  TIM_OC3Init(TIM2, &TIM_OCInitStructure);
  TIM_OC4Init(TIM2, &TIM_OCInitStructure);
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);
  
  TIM_CtrlPWMOutputs(TIM2, ENABLE); 
  TIM_CtrlPWMOutputs(TIM1, ENABLE); 
  
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

static void do_nothing(void){}

//////////////////////////////////////////////////////////////

void led_matrix_set_frame(u8* new_frame)
{
  frame = new_frame;
}

/////////////////////////////////////////////////////////////
//returns 0 if everything is OK
//1 if already opened
u8 led_matrix_open(matrix_mode_t mode, 
                   void(*scan_complete_handler)(void),
                   frame_t first_frame)
{
if(opened_flag) return 1;
if(scan_complete_handler == NULL)scan_complete_handler  = do_nothing;

led_matrix_set_mode             (mode); 
led_matrix_set_scan_handler     (scan_complete_handler);
led_matrix_set_frame            (first_frame);

TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
TIM_Cmd(TIM2, ENABLE);
TIM_Cmd(TIM1, ENABLE);
opened_flag = 1;
return 0;
}

void led_matrix_set_scan_handler(void(*scan_complete_handler)(void))
{
scan_complete = scan_complete_handler;
}

void led_matrix_close(void)
{
opened_flag = 0;
TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
TIM_Cmd(TIM2, ENABLE);
TIM_Cmd(TIM1, ENABLE);
}

///////////////////////////////////////////////////////////////////

void led_matrix_putpixel(int x, int y, u16 br,frame_t fr)
{
  if(!opened_flag) return; //not to modify invalid frame
  if((x >= COL_NUM)||(y >= ROW_NUM)||(x<0)||(y<0)) return;  
  if(fr == NULL) fr=frame;
  fr[x + y*COL_NUM] = br;
}

////////////////////////////////////////////////////////////////////
///this is for faster FFT visualization.
//values is 

void led_matrix_putbars(u16* values, u8 values_len,frame_t fr)
{
  if(!opened_flag) return; //not to modify invalid frame
  if(fr == NULL) fr=frame;
  if(values_len > MATRIX_RES_X) values_len = MATRIX_RES_X;
  for(u8 x = 0; x < values_len ; x++)
  {
    u16 z = values[x];
    if(z > MAX_BAR_VALUE) z = MAX_BAR_VALUE;
    for(u8 y = 0; y < MATRIX_RES_Y;y++)
    {  
      u16 br = z;
      if(br > MAX_BRIGHTNESS) br = MAX_BRIGHTNESS;
      fr[x + y*COL_NUM] = br;
      z -= br;
    }
  }
}

//returns length of a char 
//returns 0 if 



void led_matrix_clear(frame_t fr)
{
if(fr==NULL) fr = frame;
memset(frame,0x00,sizeof(frame_t));
}


u8 led_matrix_putchar ( int x, 
                        int y, 
                        char c, 
                        u16 br,
                        frame_t fr)
{
c -= ' ';
u8 width =  font_pos[c+1] - font_pos[c];
u8 pos =  font_pos[c];
if((x+width < 0)||(x > MATRIX_RES_X)||(x >MATRIX_RES_Y)) return width; //to speed everything up a little bit
for(u8 i=0; i < width; i++)
{
  if (font_data[pos] & BIT0) led_matrix_putpixel(x, y, br, fr);
  if (font_data[pos] & BIT1) led_matrix_putpixel(x, y+1, br, fr);
  if (font_data[pos] & BIT2) led_matrix_putpixel(x, y+2, br, fr);
  if (font_data[pos] & BIT3) led_matrix_putpixel(x, y+3, br, fr);
  if (font_data[pos] & BIT4) led_matrix_putpixel(x, y+4, br, fr);
  x++;
  pos++;
}
return width;
}

void led_matrix_putstring(int x, 
                          int y, 
                          char *c, //string
                          u16 len, //lenght
                          u16 br,
                          frame_t fr)
{
  while(len)
  {  
  x+=led_matrix_putchar(x,y,*c,br,fr);
  x++; //intercharacter interval
  c++;
  len--;
  }
  }


u16 led_matrix_get_string_len(char *c, u16 len)
{
u16 l = 0;
  while(len)
  {  
  l+=led_matrix_putchar(-1,-1,*c,0xFF,NULL);
  l++; 
  c++;
  len--;
  }
return l;
}

void led_matrix_scroll_text(char* text,u16 len, u16 timeout,u16 br )
{

  int k = 0 - led_matrix_get_string_len(text,len);
  
  for(int z = MATRIX_RES_X; z > k; z--)
  {

led_matrix_clear(NULL);
led_matrix_putstring(z, 
                     0, 
                     text, 
                     len, 
                     br,
                     NULL);
  delay_ms__(timeout);
  }
}