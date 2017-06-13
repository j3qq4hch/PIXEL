

#include "cli.h"
#include "io.h"

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "bsp.h"

#include "cmd_handlers.h"


///#ifdef CLI_HELP_FOR_COMMANDS

//static const cli_map_t cli_map[] = 
//{
//  {"cli",       cli_cli_cmd_handler,            "shows brief CLI info"},
//  {"help",      cli_help_cmd_handler,           "shows brief help for cmd"},
//  {"name",      cli_name_cmd_handler,           "shows name of the device"},
//};
//
//#else
static const cli_map_t cli_map[] = 
{
  {"cli",       cli_cli_cmd_handler},
  {"name",      cli_name_cmd_handler}
};
//#endif


static buffer_t  buf;
static u8 d[128];

void cli(void* p)
{
 
 buf.data = d;
 buf.counter = 0;
 buf.buflen = sizeof(d);
 u16 counter = 0;
 io_descriptor_t i = io_open((char*)p);
 io_writeln(i,"CLI",0);

while(1)
{
io_readln (i, &buf, portMAX_DELAY);
counter = 0;
cli_cmd_process  ((char*)buf.data,                //command string
                 buf.counter,             //command string length
                 (char*)buf.data,                //char buffer for response
                 &counter,                //response len
                 cli_map,                //cli command map
                 ARRAY_SIZE(cli_map),     //cli command map length
                 0);                      //nesting flag. Is used if response must include command
buf.counter = counter;
io_write(i, buf.data,buf.counter,NULL);
buffer_discard(&buf);
}

}

