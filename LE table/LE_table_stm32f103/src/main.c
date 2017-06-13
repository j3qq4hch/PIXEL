


#include "bsp.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdlib.h>


#include "ver.h"
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "le_table.h"

#define FRAME_LEN       32
#define DELAY_MS        2


void _delay_ms(u32 d);
void ADC_DMA_init(void);
  ws2812_t frame[FRAME_LEN] = {0};
  void frame_update(void);

  u8 flagster = 1;
  int main(void)
{
  SystemInit();
  board_init();
  SysTick_Config(72000);

  ADC_DMA_init();
   while(1)
  {   
    sensor_poll();
  //  frame_update();
  //  ws2812_send(frame,FRAME_LEN);
    }
  }


static u8 flag[FRAME_LEN] = {0};

u32 uptime = 0;
void _delay_ms(u32 d)
{
  d +=uptime;
  while(uptime < d){}
}




