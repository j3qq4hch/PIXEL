#ifndef __BLINKER_THREAD
#define __BLINKER_THREAD

#include "pwm.h"
#include "scheduler.h"
typedef struct  
{
timer_t timer;
u16 period;
}blinker_data_t;

typedef struct  
{
  timer_t timer;
  u16 period;
  u8 from;
  u8 to;
  u16 br_from;
  u16 br_to;
}animation_data_t;

typedef struct  
{
timer_t timer;
u16 current_val;
u16 desired_val;
u16 step;
u16 period;
const pwm_ch_t * pwm;
}pwm_thread_context_t;


extern thread_t pwm1_thread;
extern thread_t pwm2_thread;
extern thread_t pwm3_thread;
extern thread_t pwm4_thread;
extern thread_t pwm5_thread;
extern thread_t pwm6_thread;

#endif

