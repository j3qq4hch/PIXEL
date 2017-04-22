#ifndef MODBUS_APP_H_INCLUDED
#define MODBUS_APP_H_INCLUDED

#include "types.h"

typedef struct
{
u8 adr;
u32 baudrate;
char format[4];
}modbus_app_param_t;

void modbus_app(void * p);

#endif /* MODBUS_APP_H_INCLUDED */
