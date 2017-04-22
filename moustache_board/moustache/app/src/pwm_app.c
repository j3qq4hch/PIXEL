
#include "Freertos.h"
#include "task.h"
#include "pwm_app.h"


void pwm_master(void *param)
{
  pwm_master_param_t * p = param;
 
  u16 value = 0; 
  u32 event = 0;

  while(1)
  {
   if(value != p->desired_value)
    {   
      if(p->step)
        { 
          if(value > p->desired_value)
            {
              ((value - p->desired_value) > p->step) ? (value -= p->step) : (value = p->desired_value);
            }
          
          if(value < p->desired_value)
            {
              ((p->desired_value - value) > p->step) ? (value += p->step) : (value = p->desired_value);
            }
        }
      else
        {
        value = p->desired_value;
        }
    pwm_setval(p->pwm, value);
    vTaskDelay( p->period / portTICK_PERIOD_MS );
    }
   else
    {
    taskYIELD();
    } 
  }
}



