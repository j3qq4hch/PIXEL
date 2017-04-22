

#include "modbus_app.h"
#include "Freertos.h"
#include "task.h"
#include "uart_int.h"
#include "modbus.h"

extern uart_desc_t RS485;
static u8 modbus_request[64];
static u8 modbus_response[64];

void modbus_app(void * p)
{
 uart_desc_t * uart = &RS485;
 modbus_app_param_t * param  = p;
 modbus_set_adr(param->adr);
 uart_open(uart, NULL, 0, param->format, param->baudrate);
 u16 datalen = 0;
 while(1)
 {
  datalen = uart_read_block(uart, modbus_request, sizeof(modbus_request), 2000 / portTICK_PERIOD_MS );
  if(datalen)
  {
    datalen = modbus_process_request(modbus_request, datalen, modbus_response, sizeof(modbus_response));
    uart_send(uart, modbus_response, datalen);
  }
}

}
