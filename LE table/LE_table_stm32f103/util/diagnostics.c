#include "diagnostics.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "bsp.h"

#include "xprintf.h"
#include <stdarg.h>

static SemaphoreHandle_t semphr;
static void diag_out_char(unsigned char c)
{
  while(USART_GetFlagStatus(DBG_UART, USART_FLAG_TXE)!=SET){};
  USART_SendData(DBG_UART, c);
}

void diag_init(void)
{
semphr = xSemaphoreCreateMutex();
xSemaphoreGive(semphr);

  USART_InitTypeDef USART_InitStructure;
  USART_StructInit(&USART_InitStructure);
  USART_InitStructure.USART_BaudRate = 115200;
  USART_Init(DBG_UART, &USART_InitStructure);
  USART_Cmd (DBG_UART, ENABLE);
  xfunc_out = diag_out_char;
}

void xvprintf (const char* fmt, va_list arp);
void diag_printf (char const * fmt,	...)
{
        xSemaphoreTake( semphr, portMAX_DELAY);
        va_list arp;
	va_start(arp, fmt);
	xvprintf(fmt, arp);
	va_end(arp);        
        while(USART_GetFlagStatus(DBG_UART, USART_FLAG_TC)!=SET){};
        xSemaphoreGive(semphr);
}

void diag_write(char* str,u16 len)
{
xSemaphoreTake( semphr, portMAX_DELAY);
while(len--)
{
diag_out_char(*str++);
}
while(USART_GetFlagStatus(DBG_UART, USART_FLAG_TC)!=SET){};
xSemaphoreGive(semphr);
}

void   delay(u32 i)
{
  while(i--){}
}
