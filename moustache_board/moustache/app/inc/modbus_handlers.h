#ifndef __MODBUS_HANDLERS
#define __MODBUS_HANDLERS

#include "types.h"
#include "pwm_app.h"
#include "modbus.h"


#define PWM_START_ADR           10
#define PWM_END_ADR             PWM_START_ADR  + PWM_APP_COPIES
#define STEP_START_ADR          PWM_END_ADR  + 1
#define STEP_END_ADR            STEP_START_ADR + PWM_APP_COPIES
#define PERIOD_START_ADR        STEP_END_ADR   + 1
#define PERIOD_END_ADR          PERIOD_START_ADR + PWM_APP_COPIES

#define ID_START_ADR            100
#define ID_END_ADR              ID_START_ADR + 6          

u8 app_step_access              (u8 mode, u16 adr, u16 * val);
u8 app_period_access            (u8 mode, u16 adr, u16 * val);
u8 app_pwm_access               (u8 mode, u16 adr, u16 * val);
u8 adc_access                   (u8 mode, u16 adr, u16 * val);
u8 pwm_period_access            (u8 mode, u16 adr, u16 * val);
u8 id_access                    (u8 mode, u16 adr, u16 * val);
u8 modbus_adr_acceess           (u8 mode, u16 adr, u16 * val);
u8 version_access               (u8 mode, u16 adr, u16 * val);
u8 serial_settings_access       (u8 mode, u16 adr, u16 * val);

#endif