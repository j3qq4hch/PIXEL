#ifndef __LE_TABLE
#define __LE_TABLE

#include "stm32f10x.h"


typedef struct 
{
u8 red;
u8 green;
u8 blue;
}
ws2812_t;


void timer_init(void);

void ws2812_2_pwm(ws2812_t * w, u16 * buf);
void ws2812_send(ws2812_t * w, u16 n);

void DMA_Configuration(void);
void dma_half_isr(void);
void dma_complete_isr(void);

void table_dma_isr(void);
void ws2812_test  (ws2812_t w);
void dma_test_init(void);
void NVIC_config  (void);
void ws_init      (void);
void adc_init     (void);
void sensor_poll  (void);

#endif