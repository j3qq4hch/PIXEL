/*
 * uart_int.c
 *
 *  Created on: 22 марта 2017 г.
 *      Author: j3qq4hch
 */

#include "uart_int.h"
#include <string.h>
#include "bsp.h"   
   
#define UART_RXNE_bm            BIT0
#define UART_ORE_bm             BIT1
#define UART_TC_bm              BIT2
#define UART_RTO_bm             BIT3
#define UART_RX_COMPLETE_bm     BIT4

void uart_isr(uart_desc_t * d)
{ 
  u32 ISR = d->instance->ISR;
  if(ISR & USART_FLAG_RXNE)
  {
    if(d->rx_buf)
    {
      d->rx_buf[d->rx_counter++] = (u8)d->instance->RDR;
      if(d->rx_counter >= d->bytes2read)
      {
        xTaskNotifyFromISR( d->owner_task, UART_RX_COMPLETE_bm, eSetBits, NULL);
      }
    }
      else
    {
    if(d->rx_fifo.buf)
      {
        fifo_push(&d->rx_fifo,(u8)d->instance->RDR);
      }
    }
  }

  if(ISR & USART_FLAG_ORE)
  {
  USART_ReceiveData(d->instance);
  USART_ClearFlag(d->instance, USART_FLAG_ORE);
  } 
 
  if(ISR & USART_FLAG_RTO)      
  {
  USART_ClearFlag(d->instance, USART_FLAG_RTO);
  xTaskNotifyFromISR( d->owner_task, UART_RTO_bm, eSetBits, NULL);
  }
}

void uart_open(uart_desc_t * d, u8 * rx_buf, u16 rx_buflen, char* format_str, u32 baudrate)
{
  
        fifo_init(&d->rx_fifo, rx_buf, rx_buflen);
	 
        USART_InitTypeDef USART_InitStructure;
	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_Init(d->instance, &USART_InitStructure);
	
        USART_SetReceiverTimeOut(d->instance, 22);
        USART_ReceiverTimeOutCmd(d->instance, ENABLE);
        
        USART_ITConfig(d->instance, USART_IT_RTO, ENABLE);
        USART_ITConfig(d->instance, USART_IT_RXNE, ENABLE);
	USART_Cmd(d->instance, ENABLE);
        d->owner_task =  xTaskGetCurrentTaskHandle();
}

////////////////////////////////////////////////////////////////////////////



void uart_send_str(uart_desc_t * d, char * str)
{
uart_send(d, (u8 *) str, strlen(str));
}



void uart_send(uart_desc_t * d, u8 * data, u16 datalen)
{
  if(datalen)
  {
  while(datalen--)
  {
    while(USART_GetFlagStatus(d->instance,USART_FLAG_TXE) == RESET){}
    USART_SendData(d->instance, *data++);
  }
  }
}


u16 uart_read(uart_desc_t * d, u8 * data, u16 datalen, TickType_t timeout_ticks)
{
 u16 b = fifo_pop_buf(&d->rx_fifo, data, datalen); // check fifo first
 datalen -= b;
 
 if(datalen)
 {
    d->rx_counter = 0; 
    d->bytes2read = datalen;
    d->rx_buf = data + b;
    
    while(1)
      {
      u32 event = 0;
      if(xTaskNotifyWait(UART_RTO_bm, 0, &event, timeout_ticks ) == pdFALSE)
        {
          break;
        }
      if(event & UART_RX_COMPLETE_bm)
        { 
          break; 
        }
      }
 }
 d->rx_buf = NULL;
 d->bytes2read = 0;
 
 return b + d->rx_counter;;
}


u16 uart_read_block(uart_desc_t * d, u8 * data, u16 maxdatalen, TickType_t timeout_ticks)
{
    d->rx_counter       = 0; 
    d->bytes2read       = maxdatalen;
    d->rx_buf           = data;
    
    while(1)
      {
      u32 event = 0;
      if(xTaskNotifyWait(UART_RTO_bm, 0, &event, timeout_ticks ) == pdFALSE)
        {
          break;
        }
      if(event & (UART_RX_COMPLETE_bm | UART_RTO_bm))
        { 
          break; 
        }
      }
    
 d->rx_buf = NULL;
 d->bytes2read = 0; 
 return d->rx_counter;
}