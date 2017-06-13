#ifndef __DIAGNOSTICS
#define __DIAGNOSTICS

#include "types.h"


void diag_init(void);
void diag_printf (const char*	fmt,	...);
void diag_write(char* str,      u16  len);

void   delay(u32 i);

#endif