
#include "modbus_handlers.h"
#include "version.h"

extern pwm_master_param_t pwm_param[];
extern u16 pwm_period_ticks;

u8 app_pwm_access (u8 mode, u16 adr, u16 * val)
{
  u8 i = adr - PWM_START_ADR;
  if(mode == REG_READ)
  {
    *val = pwm_param[i].desired_value;
    return NO_ERROR;
  }  
  else
  {
  if(*val > pwm_get_period()) return ILLEGAL_DATA_VALUE;
  pwm_param[i].desired_value = *val;
  return NO_ERROR;
  }
  } 

u8 app_step_access (u8 mode, u16 adr, u16 * val)
{
  u8 i = adr - STEP_START_ADR;
  if(mode == REG_READ)
  {
    *val = pwm_param[i].step;
    return NO_ERROR;
  }  
  else
  {
  if(*val > 0xFF) return ILLEGAL_DATA_VALUE; //0xFF is maximal value for step
  pwm_param[i].step = *val;
  return NO_ERROR;
  }
} 


u8 app_period_access (u8 mode, u16 adr, u16 * val)
{
  u8 i = adr - PERIOD_START_ADR;
  if(mode == REG_READ)
  {
    *val = pwm_param[i].period;
    return NO_ERROR;
  }  
  else
  {
  if(*val > 0xFF) return ILLEGAL_DATA_VALUE; //0xFF is maximal value for step
  pwm_param[i].period = *val;
  return NO_ERROR;
  }
} 

u8 version_access(u8 mode, u16 adr, u16 * val)
{
  if(mode == REG_READ)
  {
  *val = (HW_ID << 8) + SW_VER;
  }
  else
  {
  return ILLEGAL_FUNCTION;
  }
  return NO_ERROR;
}

u8 pwm_period_access(u8 mode, u16 adr, u16 * val)
{
  if(mode == REG_READ)
  {
  *val = pwm_period_ticks;
  }
  else
  {
  if(*val == 0) return ILLEGAL_DATA_VALUE;
  pwm_period_ticks = *val;
  pwm_init();
  //i should restart timers here //
  }
  return NO_ERROR;
}  

u8 serial_settings_access (u8 mode, u16 adr, u16 * val)
{
  if(mode == REG_READ)
  {
    *val = 0xBEEF;
  }
  return NO_ERROR;
}


u8 adc_access(u8 mode, u16 adr, u16 * val)
{
  if(mode == REG_READ)
  {
      *val = 0xDEAD;
  }
  else
  {
  return ILLEGAL_FUNCTION; // you cannot write to ADC register, read-only
  }
  return NO_ERROR;
}

u8 id_access(u8 mode, u16 adr, u16 * val)
{
  return NO_ERROR;
}

//////////////////////////////////

u8 modbus_adr_acceess   (u8 mode, u16 adr, u16 * val)
{
  if(mode == REG_READ)
  {
    *val = modbus_get_adr();
  }
  else
  {
    if(*val > 254) return ILLEGAL_FUNCTION;
    modbus_set_adr((u8)*val);
  }
  return NO_ERROR;
}