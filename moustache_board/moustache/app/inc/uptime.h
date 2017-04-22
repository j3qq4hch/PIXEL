
#ifndef __UPTIME
#define __UPTIME

#include "stm32f0xx.h"
#include "types.h"


#define         TICK_LEN_IN_uS   1000
#define         TICK_LEN_IN_mS   TICK_LEN_IN_uS/1000

#ifndef SYSCLK
#define         SYSCLK           48000000
#endif
#define         SYSTICK_PERIOD  (SYSCLK/TICK_LEN_IN_uS) //HSE VALUE is 16



extern volatile u32 uptime_counter;

void delay_ms__(u32 delay);
u32 get_uptime_ticks(void);
#endif