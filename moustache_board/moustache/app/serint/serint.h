#ifndef __SERINT
#define __SERINT

#include "stm32f0xx.h"
#include "types.h"


typedef struct
{
u16             rx_pin;
u16             rx_af_pinsource;
GPIO_TypeDef *  rx_port;
u32             rx_rcc;
u8              rx_af;

u16             tx_pin;
u16             tx_af_pinsource;
GPIO_TypeDef *  tx_port;
u32             tx_rcc;
u8              tx_af;

USART_TypeDef * usart;
u32             usart_rcc;  //rcc bit
u8              usart_IRQn; //interrupt vector

}USART_hw_desc_t;

typedef struct
{	
	char *name; 
	const USART_hw_desc_t *hw_desc;
	USART_InitTypeDef * usart_settings;
	
	u8* inbuf;
	u16 inbuf_len;
	volatile u16 in_counter;
	
	char opened_flag;
	volatile char receiving_flag;
	
	void(*port_open_func)(void);
	void(*port_close_func)(void);
	void(*presend_func)     (const u8* data, u16 len);
	void(*aftersend_func)   (const u8* data, u16 len);
} serint_desc_t;

void usart_hw_init      (const USART_hw_desc_t * d);
void usart_sendbuf      (USART_TypeDef* USARTx, const u8 *buf,u16 len);

void serint_init        (serint_desc_t* desc,
			 char* name,
			 const USART_hw_desc_t * hw_desc,
			 void(*presend_func)    (const u8* data, u16 len),
			 void(*aftersend_func)  (const u8* data, u16 len),
			 void(*port_open_func)(void),
			 void(*port_close_func)(void));

ErrorStatus serint_open(serint_desc_t* desc,
                        USART_InitTypeDef * settings,
                        u8* inbuf,
                        u16 buf_len);
void serint_close               (serint_desc_t* desc);
void serint_send                (serint_desc_t* desc,const u8* data,u16 len);
void serint_flush_inbuf		(serint_desc_t* desc);
void serint_rx_cmd              (serint_desc_t* desc,FunctionalState NewState);
void serint_wait_receive	(serint_desc_t* desc);
void serint_rx_isr              (serint_desc_t* desc);
#endif