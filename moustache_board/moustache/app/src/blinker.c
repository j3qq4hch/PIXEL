#include "blinker.h"
#include "bsp.h"
#include "pwm.h"

//////////////////////////////////////////////

static PT_THREAD(blink(struct pt *pt, void * data));
static PT_THREAD(animation(struct pt *pt, void * data));
static PT_THREAD(linear_breathe_thread(struct pt *pt, void * data));

extern const pwm_ch_t pwm_array[];

pwm_thread_context_t breath_context1 = {{0}, 0, 0, 1, 5, &pwm_array[0]};
pwm_thread_context_t breath_context2 = {{0}, 0, 0, 2, 5, &pwm_array[1]};
pwm_thread_context_t breath_context3 = {{0}, 0, 0, 3, 5, &pwm_array[2]};
pwm_thread_context_t breath_context4 = {{0}, 0, 0, 4, 5, &pwm_array[3]};
pwm_thread_context_t breath_context5 = {{0}, 0, 0, 5, 5, &pwm_array[4]};
pwm_thread_context_t breath_context6 = {{0}, 0, 0, 6, 5, &pwm_array[5]};


thread_t pwm1_thread = {"pwm1", linear_breathe_thread, 0, &breath_context1};
thread_t pwm2_thread = {"pwm2", linear_breathe_thread, 0, &breath_context2};
thread_t pwm3_thread = {"pwm3", linear_breathe_thread, 0, &breath_context3};
thread_t pwm4_thread = {"pwm4", linear_breathe_thread, 0, &breath_context4};
thread_t pwm5_thread = {"pwm5", linear_breathe_thread, 0, &breath_context5};
thread_t pwm6_thread = {"pwm6", linear_breathe_thread, 0, &breath_context6};

timer_t z = {0};

static PT_THREAD(blink(struct pt *pt, void * context))
{
  PT_BEGIN(pt);
  while(1)
    {
    LED_toggle();
    timer_set(&z,100);
    PT_WAIT_UNTIL(pt,timer_expired(&z));
//    PT_TIMER_WAIT(pt,&z,p->period);
    }
  PT_END(pt);
}

thread_t blinker_thread = {"blink", blink, 0, NULL};

//////////////////////////////////////////////

extern const pwm_ch_t pwm_array[];

static PT_THREAD(animation(struct pt *pt, void * data))
{

  PT_BEGIN(pt);
  animation_data_t * p = data;
  p->from = 0;
  p->to = p->from + 1;
  
  while(1)
    {    
      p->br_from = get_pwm_period();
      p->br_to = 0;  
      while(p->br_from)
      {
      pwm_setval(&pwm_array[p->from], p->br_from);
      pwm_setval(&pwm_array[p->to], p->br_to);
      PT_TIMER_WAIT(pt,&p->timer,p->period);
      p->br_from--;
      p->br_to++;
      }
      pwm_setval(&pwm_array[p->from], p->br_from);
      pwm_setval(&pwm_array[p->to], p->br_to);
      p->from++;
      p->to++;
      if(p->to >= 6) p->to = 0;
      if(p->from >= 6) p->from = 0;
    }
  PT_END(pt);
}

//////////////////////////////////////////////

thread_t anim_thread = {"anim", animation, 0, NULL};

static PT_THREAD(linear_breathe_thread(struct pt *pt, void * data))
{
	PT_BEGIN(pt);
	pwm_thread_context_t* p = data;
	while(1)
		{
		while(p->current_val < get_pwm_period())
		{
			p->current_val += p->step;
			pwm_setval(p->pwm, p->current_val);
			PT_TIMER_WAIT(pt,&p->timer,p->period);
		}

		while(p->current_val > 0)
		{
		if(p->current_val < p->step)
			{
			p->current_val = 0;
			}
		else
			{
			p->current_val -= p->step;
			}
		pwm_setval(p->pwm, p->current_val);
		PT_TIMER_WAIT(pt,&p->timer,p->period);
		}
	}
	  PT_END(pt);
}