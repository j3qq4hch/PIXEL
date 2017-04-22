
#include "time_service.h"
#include "stm32f0xx.h"

//time service time step is 1mS
//all delay functions are in mS

static u32 uptime = 0;

void time_service_init(void)
{
  SysTick_Config(SystemCoreClock / 1000);
}

void ts_delay(u16 mS)
{
u32 time = uptime + mS;
while(time > uptime){}
}

void time_service_isr(void)
{
uptime++;
}

u32 get_uptime(void)
{
return uptime;
}