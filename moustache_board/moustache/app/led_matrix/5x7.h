#ifndef __5X7
#define __5X7

#include "stm32f0xx.h"
#include "types.h"
#define ARRAY_SIZE(arr)         (sizeof(arr) / sizeof(arr[0]))

//these are defines for controling 5x7 led matrix
//it is supposed to have some kind grayscale
//part of led zeppelin project
//j3qq4hch@gmail.com
#ifndef SYSCLK_HZ
#define SYSCLK_HZ           48000000
#endif

#define MATRIX_SCAN_TIME_uS     10000 
#define GRAYSCALE_STEPS         255 //number of grayscales
#define MAX_BRIGHTNESS          GRAYSCALE_STEPS
#define COL_NUM                 7
#define ROW_NUM                 5
#define TOTAL_PIXELS            ROW_NUM*COL_NUM              
#define ROW_SCAN_TIME_uS        (u32)(MATRIX_SCAN_TIME_uS/COL_NUM)

#define MATRIX_RES_Y            ROW_NUM    
#define MATRIX_RES_X            COL_NUM
#define MAX_BAR_VALUE           (MAX_BRIGHTNESS*MATRIX_RES_Y) 

#define TIMER_TICK_LEN          (ROW_SCAN_TIME_uS/GRAYSCALE_STEPS)


#define COL1_pin        GPIO_Pin_4
#define COL1_port       GPIOB   
#define COL1_rcc        RCC_AHBPeriph_GPIOB

#define COL2_pin        GPIO_Pin_10
#define COL2_port       GPIOB   
#define COL2_rcc        RCC_AHBPeriph_GPIOB

#define COL3_pin        GPIO_Pin_5
#define COL3_port       GPIOB   
#define COL3_rcc        RCC_AHBPeriph_GPIOB

#define COL4_pin        GPIO_Pin_9
#define COL4_port       GPIOB   
#define COL4_rcc        RCC_AHBPeriph_GPIOB

#define COL5_pin        GPIO_Pin_1
#define COL5_port       GPIOF
#define COL5_rcc        RCC_AHBPeriph_GPIOF

#define COL6_pin        GPIO_Pin_8
#define COL6_port       GPIOB   
#define COL6_rcc        RCC_AHBPeriph_GPIOB

#define COL7_pin        GPIO_Pin_0
#define COL7_port       GPIOF   
#define COL7_rcc        RCC_AHBPeriph_GPIOF

//////////////////////////////////////////////////

#define ROW1_pin        GPIO_Pin_3
#define ROW1_port       GPIOA
#define ROW1_rcc        RCC_AHBPeriph_GPIOA
#define ROW1_AF         GPIO_PinSource3

#define ROW2_pin        GPIO_Pin_3
#define ROW2_port       GPIOB   
#define ROW2_rcc        RCC_AHBPeriph_GPIOB
#define ROW2_AF         GPIO_PinSource3

#define ROW3_pin        GPIO_Pin_15
#define ROW3_port       GPIOA
#define ROW3_rcc        RCC_AHBPeriph_GPIOA
#define ROW3_AF         GPIO_PinSource15

#define ROW4_pin        GPIO_Pin_2
#define ROW4_port       GPIOA
#define ROW4_rcc        RCC_AHBPeriph_GPIOA
#define ROW4_AF         GPIO_PinSource2

#define ROW5_pin        GPIO_Pin_10
#define ROW5_port       GPIOA
#define ROW5_rcc        RCC_AHBPeriph_GPIOA
#define ROW5_AF         GPIO_PinSource10

#define ALL_ROWS_OFF()  ((ROW1_port->BSRR|=ROW1_pin), (ROW2_port->BSRR|=ROW2_pin), (ROW3_port->BSRR|=ROW3_pin), (ROW4_port->BSRR|=ROW4_pin), (ROW5_port->BSRR|=ROW5_pin))
#define ALL_ROWS_ON()   ((ROW1_port->BRR|=ROW1_pin), (ROW2_port->BRR|=ROW2_pin), (ROW3_port->BRR|=ROW3_pin), (ROW4_port->BRR|=ROW4_pin), (ROW5_port->BRR|=ROW5_pin))

///////////////////////////////////////////////////

#define PWM_CHANNEL_0    (TIM2->CCR4)
#define PWM_CHANNEL_1    (TIM2->CCR2)
#define PWM_CHANNEL_2    (TIM2->CCR1)
#define PWM_CHANNEL_3    (TIM2->CCR3)
#define PWM_CHANNEL_4    (TIM1->CCR3)

#ifndef NULL
 #define NULL    0
#endif


typedef struct
{
u16             pin;
GPIO_TypeDef *  port;
u32             rcc;
u8              af;
}pin_init_t;

typedef struct
{
u16             pin;
GPIO_TypeDef *  port;
} pin_t;

typedef u8 frame_t[TOTAL_PIXELS];


typedef enum {STRAIGHT = 0, REVERSE = !STRAIGHT} matrix_mode_t;

void led_matrix_init                    (void);
void led_matrix_isr_handler             (void);
void led_matrix_timers_init             (void);
void led_matrix_gpio_rows_init          (void);
void led_matrix_gpio_columns_init       (void);
void do_nothing                         (void);
void led_matrix_set_frame               (frame_t frame);

u8 led_matrix_open                      (matrix_mode_t mode, 
                                         void(*scan_complete_handler)(void),
                                         frame_t first_frame);
void led_matrix_set_scan_handler        (void(*scan_complete_handler)(void));
void led_matrix_set_mode                (matrix_mode_t mode);
void led_matrix_close                   (void);
void led_matrix_putpixel                (int x, int y, u16 br,frame_t fr);


u8 led_matrix_putchar(int x, 
                      int y, 
                      char c, 
                      u16 br,
                      frame_t fr);

void led_matrix_putstring(int x, 
                          int y, 
                          char *c, //string
                          u16 len, //lenght
                          u16 br,
                          frame_t fr);

void led_matrix_clear(frame_t fr);
u16 led_matrix_get_string_len(char *c, u16 len);
void led_matrix_scroll_text     (char* text,u16 len, u16 timeout , u16 br);
void led_matrix_putbars         (u16* values, u8 values_len,frame_t fr);
#endif