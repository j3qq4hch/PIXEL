#ifndef  __TIME_SERVICE
#define  __TIME_SERVICE
#include "types.h"


void time_service_init(void);
void ts_delay(u16 mS);
void time_service_isr(void);
u32 get_uptime(void);
#endif 
