
#include "flash_access.h"

/*
some stuff to overcome stm32f0 natural flash access limitations to be able to write flash in 1 byte chunks
to make stream operations with flash possible

It is important to mention it is NOT REENTRANT and thus should be used with care
*/

static u32 flash_adr = 0;
static u16 val = 0;
static u8 flag = 0;

void flash_init(u32 start_adr)
{
flash_adr  = start_adr;
flag = 0;
val  = 0;
}

void flash_write_byte(u8 b)
{
  if(!flag)
  {
  flag = 1;
  val = b;
  }
  else
  {
  flag = 0;
  val |= b<<8;
  FLASH_ProgramHalfWord(flash_adr, val);
  flash_adr += 2;  
  }
}

void flash_finalize(void)
{
  if(flag){flash_write_byte(0);}
}

u8 flash_read_byte(u8 * b)
{
  *b = *(u8*)flash_adr++;
  return 0;
}