#ifndef __SCHEDULER
#define __SCHEDULER

//this is small and basic scheduler base on protothreads for using with small and simple projects

#include "types.h"
#include "pt.h"

typedef struct
{
  u32 start_time;
  u32 interval;
} timer_t;


typedef struct
{
  char * name;
  char (*func)(struct pt *pt, void * data); 
  struct pt pt; 
  void * context;
}thread_t;


u8 timer_expired(timer_t* timer);
void timer_set(timer_t* timer, u32 interval);


#define PT_TIMER_WAIT(pt, timer_p, interval)	\
do{					    	\
  timer_set(timer_p, interval);			\
  PT_WAIT_UNTIL(pt,timer_expired(timer_p));	\
}while(0)


static inline void thread_spin(thread_t * t)
{
t->func(&t->pt,t->context);
}


#endif
