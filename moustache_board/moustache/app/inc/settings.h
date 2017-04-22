#ifndef __SETTINGS
#define __SETTINGS

#include "types.h"
/*
This module defines a set of parameters that must be kept in non-volatile memory and stores them in the somewhere in internal MCU flash
For that FLASH_PAGE_SIZE and FLASH_SETTINGS_PAGE must be defined for your particular MCU 
You should not store anything else in that flash memory area
*/
#define FLASH_PAGE_SIZE         0x400
#define FLASH_SETTINGS_PAGE     15
///////////////////////////////////////
#define FLASH_START_ADDR        0x08000000
#define SETTINGS_START_ADR      (FLASH_START_ADDR + (FLASH_SETTINGS_PAGE * FLASH_PAGE_SIZE))


#define TYPE_U8          (1<<8)
#define TYPE_U16         (2<<8)
#define TYPE_U32         (3<<8)
#define TYPE_BYTE_ARRAY  (4<<8)

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))



#define DISABLE_SYSTICK()       SysTick->CTRL  &= ~SysTick_CTRL_ENABLE_Msk;  
#define ENABLE_SYSTICK()        SysTick->CTRL  |= SysTick_CTRL_ENABLE_Msk;


typedef struct
{
char * name;  
u16 type;
void * val_p;                   
const void * default_val_p; 
}setting_t ;


void    settings_load(void);
void    settings_save(void);
void    settings_load_dflt(void);

#endif