
#ifndef PT_UART_H_
#define PT_UART_H_

#include "scheduler.h"
#include "uart_int.h"



typedef struct
{
	struct pt pt;
	uart_desc_t a;
	timer_t t;
}pt_uart_desc_t;


void pt_uart_send(pt_uart_desc_t* d, u8 * data, u16 datalen);

void pt_uart_send(pt_uart_desc_t* d, u8 * data, u16 datalen)
{
//uart_start_tx(uart->, u8 * data, u16 datalen);
}

#endif /* PT_UART_H_ */
