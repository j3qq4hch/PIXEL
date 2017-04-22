
#include <string.h>
#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "bitdef.h"
#include "types.h"
#include "5x7.h"
#include "test.h"
#include "uptime.h"
#include "lis302dl.h"
#include "misc.h"

USB_CORE_HANDLE  USB_Device_dev ;

int main(void)
{
  RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI48);
  RCC_ClocksTypeDef c;
  RCC_GetClocksFreq(&c);
  SysTick_Config(SYSTICK_PERIOD);
  led_matrix_init();  
  lis302d_init(NULL, NULL);
  USBD_Init(&USB_Device_dev,
            &USR_desc, 
            &USBD_CDC_cb, 
            &USR_cb);
test_init();    
while (1)
  {
 test_animation();
  }
} 


void i2c_init(void)
{ 
  RCC_APB1PeriphClockCmd(I2C_rcc, ENABLE); 
  I2C_InitTypeDef  I2C_InitStructure;

  I2C_InitStructure.I2C_Mode                    = I2C_Mode_I2C;
  I2C_InitStructure.I2C_AnalogFilter            = I2C_AnalogFilter_Enable;
  I2C_InitStructure.I2C_DigitalFilter           = 0x00;
  I2C_InitStructure.I2C_OwnAddress1             = 0x00;
  I2C_InitStructure.I2C_Ack                     = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress     = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_Timing                  = 0x00210507; // WHAT A FUCK!?

  I2C_Init      (I2C_IN_USE, &I2C_InitStructure);
  I2C_Cmd       (I2C_IN_USE, ENABLE);  
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void i2c_gpio_init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
  RCC_AHBPeriphClockCmd(I2C_SDA_rcc | I2C_SCL_rcc, ENABLE);
  
  
  GPIO_PinAFConfig(I2C_SDA_port, I2C_SDA_AF_source, GPIO_AF_1);
  GPIO_PinAFConfig(I2C_SCL_port, I2C_SCL_AF_source, GPIO_AF_1);
  
  GPIO_InitStructure.GPIO_Pin   = I2C_SCL_pin;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(I2C_SCL_port, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = I2C_SCL_pin;
  GPIO_Init(I2C_SCL_port, &GPIO_InitStructure);
}
