#ifndef __UART_INT
#define __UART_INT

#include "stm32f0xx_usart.h"
#include "fifo.h"
#include "Freertos.h"
#include "semphr.h"
#include "task.h"
typedef struct
{
	char* str;
	uint32_t word_length;
	uint32_t stop_bits;
	uint32_t flow_control;
}uart_format_map_t;

typedef struct
{
  char* name;
  USART_TypeDef * instance;
  fifo_t rx_fifo;
  u8 * rx_buf;
  u16 rx_counter;
  u16 bytes2read;
  
  u8 rx_flag;
  
  u8 * tx_p;
  u16 tx_counter;
  u8 tx_flag;
  TaskHandle_t owner_task;
}uart_desc_t;

void uart_isr(uart_desc_t * d);
void uart_open(uart_desc_t * d, u8 * rx_buf, u16 rx_buflen, char* format_str, u32 baudrate);
void uart_send(uart_desc_t * d,  u8 * data, u16 datalen);
void uart_get_rx_datalen(uart_desc_t * d);

u16 uart_read_block(uart_desc_t * d, u8 * data, u16 max_datalen, TickType_t xTicksToWait);
void uart_send_str(uart_desc_t * d, char * str);
u16 uart_read(uart_desc_t * d, u8 * data, u16 datalen, TickType_t timeout_ticks);
#endif
