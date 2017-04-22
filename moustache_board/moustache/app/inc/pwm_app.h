#ifndef __PWM_APP
#define __PWM_APP

#include "pwm.h"

#define PWM_APP_COPIES  6

typedef struct 
{
pwm_ch_t *      pwm;
u8              step;
u8              period;
u16             desired_value;
}pwm_master_param_t;
 
void pwm_master(void *param);
#endif