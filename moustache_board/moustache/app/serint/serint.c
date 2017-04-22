#include "serint.h"
#include <string.h>

///////////////////////////////////////////////////////////////////////
//usart 1 hw description
const USART_hw_desc_t usart1_hw_desc = 
{
  GPIO_Pin_10,           //u16 rx_pin;
  GPIO_PinSource10,      
  GPIOA,                //GPIO_TypeDef * rx_port
  RCC_AHBPeriph_GPIOA,  //u32 rx_rcc;
  GPIO_AF_1,
  
  GPIO_Pin_9,           //u16 tx_pin
  GPIO_PinSource9,      
  GPIOA,                //GPIO_TypeDef * tx_port
  RCC_AHBPeriph_GPIOA,  //u32 tx_rcc;
  GPIO_AF_1,
  
  USART1,                       //USART_TypeDef * usart
  RCC_APB2Periph_USART1,        
  
  USART1_IRQn                   //usart_IRQn
};

///////////////////////////////////////////////////

const USART_hw_desc_t usart2_hw_desc = 
{
  GPIO_Pin_10,           //u16 rx_pin;
  GPIO_PinSource10,      
  GPIOA,                //GPIO_TypeDef * rx_port
  RCC_AHBPeriph_GPIOA,  //u32 rx_rcc;
  GPIO_AF_1,
  
  GPIO_Pin_9,           //u16 tx_pin
  GPIO_PinSource9,      
  GPIOA,                //GPIO_TypeDef * tx_port
  RCC_AHBPeriph_GPIOA,  //u32 tx_rcc;
  GPIO_AF_1,
  
  USART1,                       //USART_TypeDef * usart
  RCC_APB2Periph_USART1,        
  
  USART1_IRQn                   //usart_IRQn
};

////////////////////////////////////////////////////////////////////////

void usart_hw_init(const USART_hw_desc_t * d)
{
GPIO_InitTypeDef GPIO_InitStructure;
RCC_AHBPeriphClockCmd(d->rx_rcc | d->tx_rcc, ENABLE);

GPIO_InitStructure.GPIO_Pin     = d->tx_pin;
GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF;
GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_10MHz;
GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
GPIO_Init(d->rx_port, &GPIO_InitStructure);

GPIO_InitStructure.GPIO_Pin = d->rx_pin;
GPIO_Init(d->rx_port, &GPIO_InitStructure);

GPIO_PinAFConfig(d->rx_port, d->rx_af_pinsource, d->rx_af);
GPIO_PinAFConfig(d->tx_port, d->tx_af_pinsource, d->tx_af);

if(d->usart ==  USART1) 
{
RCC_APB2PeriphClockCmd(d->usart_rcc,ENABLE);
}
else
{
RCC_APB1PeriphClockCmd(d->usart_rcc,ENABLE);
}

NVIC_InitTypeDef NVIC_InitStructure;
NVIC_InitStructure.NVIC_IRQChannel = d->usart_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);
}



void usart_sendbuf(USART_TypeDef* USARTx, const u8 *buf,u16 len)
{
  while(len)
  {
  USARTx->TDR = *buf++;
  while((USARTx->ISR & USART_FLAG_TXE) == 0){}
  len--;
  }
  while(USARTx->ISR & USART_FLAG_TC == 0){}
}




void serint_init        (serint_desc_t* desc,
			 char* name,
			 const USART_hw_desc_t * hw_desc,
			 void(*presend_func)(const u8* data, u16 len),
			 void(*aftersend_func)(const u8* data, u16 len),
			 void(*port_open_func)(void),
			 void(*port_close_func)(void))
{	
	desc->name              = name;
	desc->hw_desc           = hw_desc;
	desc->presend_func	= presend_func;
	desc->aftersend_func    = aftersend_func;
	desc->port_open_func    = port_open_func;
	desc->port_close_func   = port_close_func;
        if(desc->hw_desc!=NULL){ usart_hw_init(desc->hw_desc);}
}


ErrorStatus serint_open(serint_desc_t* desc,
                        USART_InitTypeDef * settings,
                        u8* inbuf,
                        u16 buf_len)
{
if(desc->opened_flag !=0) return ERROR;
desc->opened_flag = 1;
desc->inbuf = inbuf;
desc->inbuf_len = buf_len;
desc->usart_settings = settings;

if(desc->port_open_func!=NULL) desc->port_open_func();
if(desc->hw_desc!=NULL)
{
  USART_Init(desc->hw_desc->usart, desc->usart_settings);
  serint_rx_cmd(desc,ENABLE);
}
return SUCCESS;
}


void serint_close(serint_desc_t* desc)
{
desc->opened_flag = 0;
serint_rx_cmd(desc,DISABLE);
if(desc->port_close_func!=NULL) desc->port_close_func();
}



void serint_rx_cmd(serint_desc_t* desc,FunctionalState NewState)
{
  if(desc->hw_desc == NULL) return;
USART_Cmd(desc->hw_desc->usart, NewState);
USART_ITConfig(desc->hw_desc->usart, USART_IT_RXNE|USART_IT_IDLE , NewState);
//USART_ITConfig(desc->hw_desc->usart, USART_IT_RXNE, NewState);
}

void serint_send(serint_desc_t* desc,const u8* data,u16 len)
{
if(desc->presend_func!=NULL) desc->presend_func(data, len);
if(desc->hw_desc!=NULL)
{
  usart_sendbuf(desc->hw_desc->usart,data,len);
}
  if(desc->aftersend_func!=NULL)desc->aftersend_func(data, len);
}

void serint_flush_inbuf		(serint_desc_t* desc)
{
memset(desc->inbuf,0x00,desc->in_counter);
desc->in_counter = 0;
}

///////////////////////////////////////////////////////////////

void serint_rx_isr(serint_desc_t* desc)
{
u32 status = desc->hw_desc->usart->ISR;
if(status & USART_FLAG_RXNE)
{
desc->receiving_flag = 1;
u8 c = desc->hw_desc->usart->RDR;              // to avoid warnings
desc->inbuf[desc->in_counter] = c;
desc->in_counter++;
if(desc->in_counter == desc->inbuf_len)
{
desc->in_counter = 0;           //to avoid overflow
desc->receiving_flag = 0;       //to avoid infinite waiting of transmission complete
}
//return;
}
if(status & USART_FLAG_IDLE)
{
desc->receiving_flag = 0;
USART_ClearFlag(desc->hw_desc->usart, USART_ICR_IDLECF);
//return;
}
if(status & USART_FLAG_ORE)
{
USART_ClearFlag(desc->hw_desc->usart, USART_ICR_ORECF);
}

}
