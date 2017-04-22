#ifndef __LIS302DL
#define __LIS302DL

#include "types.h"
#include "bitdef.h"

#define INT1_pin        GPIO_Pin_8
#define INT1_port       GPIOA
#define INT1_rcc        RCC_AHBPeriph_GPIOA
#define INT1_EXTI_port  EXTI_PortSourceGPIOA
#define INT1_EXTI_pin   EXTI_PinSource8
#define INT1_EXTI_LINE  EXTI_Line8


#define INT2_pin        GPIO_Pin_15
#define INT2_port       GPIOB   
#define INT2_rcc        RCC_AHBPeriph_GPIOB
#define INT2_EXTI_port  EXTI_PortSourceGPIOB
#define INT2_EXTI_pin   EXTI_PinSource15
#define INT2_EXTI_LINE  EXTI_Line15

#define USE_I2C

#ifdef USE_I2C
#define LIS_I2C_ADR     0x38 //SDO must be connected to GND
#endif

///Register map
#define LIS302DL_WHO_AM_I               0x0F
#define LIS302DL_CTRL_REG1              0x20
#define LIS302DL_CTRL_REG2              0x21
#define LIS302DL_CTRL_REG3              0x22
#define LIS302DL_STATUS_REG             0x27
#define LIS302DL_OUT_X                  0x29
#define LIS302DL_OUT_Y                  0x2B
#define LIS302DL_OUT_Z                  0x2D
#define LIS302DL_CLICK_CFG              0x38
#define LIS302DL_CLICK_SRC              0x39
#define LIS302DL_CLICK_THSY_X           0x3B
#define LIS302DL_CLICK_THSZ             0x3C
#define LIS302DL_CLICK_timelimit        0x3D
#define LIS302DL_CLICK_latency          0x3E
#define LIS302DL_CLICK_window           0x3F

////Bit definitions for registers
// CTRL_REG1
#define LIS302DL_XEN            BIT0
#define LIS302DL_YEN            BIT1
#define LIS302DL_ZEN            BIT2
#define LIS302DL_STM            BIT3
#define LIS302DL_STP            BIT4
#define LIS302DL_FS             BIT5
#define LIS302DL_PD             BIT6
#define LIS302DL_DR             BIT7

// CTRL_REG2
#define LIS302DL_HP_COEFF1      BIT0
#define LIS302DL_HP_COEFF2      BIT1
#define LIS302DL_HP_FF_WU1      BIT2
#define LIS302DL_HP_FF_WU2      BIT3
#define LIS302DL_FDS 	        BIT4
#define LIS302DL_BOOT           BIT5
#define LIS302DL_SIM	        BIT6


// CTRL_REG3
#define LIS302DL_I1CFG0         BIT0
#define LIS302DL_I1CFG1         BIT1
#define LIS302DL_I1CFG2         BIT2
#define LIS302DL_I2CFG0         BIT3
#define LIS302DL_I2CFG1         BIT4
#define LIS302DL_I2CFG2         BIT5
#define LIS302DL_PPOD           BIT6
#define LIS302DL_IHL            BIT7


// LIS302DL_CLICK_CFG
#define LIS_SINGLE_X    BIT0
#define LIS_DOUBLE_X    BIT1
#define LIS_SINGLE_Y    BIT2
#define LIS_DOUBLE_Y    BIT3
#define LIS_SINGLE_Z    BIT4
#define LIS_DOUBLE_Z    BIT5
#define LIS_LIR         BIT6

// LIS302DL_CLICK_SRC
#define SINGLE_CLICK_X   BIT0
#define DOUBLE_CLICK_X   BIT1
#define SINGLE_CLICK_Y   BIT2
#define DOUBLE_CLICK_Y   BIT3
#define SINGLE_CLICK_Z   BIT4
#define DOUBLE_CLICK_Z   BIT5
#define INTERRUPT_ACTIVE BIT6

/////////////////////////////////////////////////////



typedef union{
   struct {
          u8 Xen:1;
          u8 Yen:1;
          u8 Zen:1;
          u8 STM:1;
          u8 STP:1;
          u8 FS:1;
          u8 PD:1;
          u8 DR:1;
    } bits;
    u8 byte;
} ctrl_reg1_t;


typedef union{
   struct {
          u8 HP_coeff1  :1;
          u8 HP_coeff2  :1;
          u8 HP_FF_WU1  :1;
          u8 HP_FF_WU2  :1;
          u8 FDS        :1;
          u8 reserved   :1;
          u8 BOOT       :1;
          u8 SIM        :1;
    } bits;
    u8 byte;
} ctrl_reg2_t;



#define ICFG_OFF        0
#define ICFG_FF_WU_1    1
#define ICFG_FF_WU_2    2
#define ICFG_FF_WU_1_2  3
#define ICFG_DATA_READY 4
#define ICFG_CLICK_INT  7



typedef union{
   struct {
           u8 I1CFG:3;
           u8 I2CFG:3;     
           u8 IHL:1;
           u8 PP_OD:1;
         } bits;
    u8 byte;
} ctrl_reg3_t;

typedef union{
   struct {
           u8 XDA       :1;
           u8 YDA       :1;     
           u8 ZDA       :1;
           u8 ZYXDA     :1;
           u8 XOR       :1;
           u8 YOR       :1;
           u8 ZOR       :1;
           u8 ZYXOR     :1;
          } bits;
    u8 byte;
} status_reg_t;


//typedef union status_

typedef union lis_ctrl_reg
{
ctrl_reg1_t ctrlreg1;
ctrl_reg2_t ctrlreg2;
ctrl_reg3_t ctrlreg3;
status_reg_t statusreg;

}lis_ctrl_reg_t;

u8	lis302d_get_id(void);
void	lis302d_regwrite(u8 adr, u8 data);
u8      lis302d_regread(u8 adr);
void    lis_get_readings(s8* readings);

void    lis302d_init(void(*single_click_handler)(void),
                     void(*double_click_handler)(void),
                     u8 thrshX,u8 thrshY,u8 thrshZ,s8* isr_readings);

void    lis302d_gpio_int_config(void);
void    lis302d_set_click_handler(void(*handler)(void));
void    lis302d_set_int2_handler(void(*handler)(void));
void    lis302d_set_int1_handler(void(*handler)(void));
void    lis302d_set_thresholds(u8 x,u8 y, u8 z);
void    do_nothing      (void);
void    EXTI4_15_IRQHandler(void);



#endif
