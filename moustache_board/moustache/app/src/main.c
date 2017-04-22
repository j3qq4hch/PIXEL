
#include "bsp.h"
#include "Freertos.h"
#include "task.h"

#include "uart_int.h"


#include "settings.h"
#include "modbus_app.h"

void blinker    (void *p);

u16 blink_period = 100;
extern modbus_app_param_t modbus_app_param ;


int main(void)
{ 
//settings_load();
  board_init();
        
  xTaskCreate(blinker,                    /* Function that implements the task. */
              "blink",                    /* Name */
              configMINIMAL_STACK_SIZE,   /* Stack size in words, not bytes. */
              ( void * ) &blink_period,   /* Parameter passed into the task. */
              tskIDLE_PRIORITY + 1,       /* Priority at which the task is created. */
              NULL );  


//  xTaskCreate(modbus_app,                     /* Function that implements the task. */
//              "modbus",                       /* Name */
//              configMINIMAL_STACK_SIZE * 2,   /* Stack size in words, not bytes. */
//              ( void * ) &modbus_app_param,   /* Parameter passed into the task. */
//              tskIDLE_PRIORITY + 1,           /* Priority at which the task is created. */
//              NULL );  
  
  vTaskStartScheduler();
}



void blinker(void *p)
{

while(1)
{
LED_toggle();  
vTaskDelay( 100 / portTICK_PERIOD_MS );
}
}


