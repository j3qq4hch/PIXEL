#ifndef __FLASH_ACCESS
#define __FLASH_ACCESS

#include "stm32f0xx.h"
#include "types.h"


void flash_init         (u32 start_adr);
void flash_write_byte   (u8 b);
void flash_finalize     (void);
u8   flash_read_byte    (u8 * b);

#endif