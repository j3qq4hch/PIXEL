#include "scheduler.h"
#include "time_service.h"


u8 timer_expired(timer_t* timer)
{
  if( get_uptime() >= timer->start_time + timer->interval) 
    {
    return 1;
    }
  else
    {
    return 0;
    }
}

void timer_set(timer_t* timer, u32 interval)
{
  timer->interval = interval;
  timer->start_time = get_uptime();
}




