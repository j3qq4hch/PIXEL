#include "cli.h"
#include "cmd_handlers.h"

#define CLI_BUFLEN      128
#define CLI_RESPLEN     128

static serint_desc_t* interface = NULL;
static char inbuf[CLI_BUFLEN];
static char response[CLI_RESPLEN];

static USART_InitTypeDef usart_settings = 
{
9600,
USART_WordLength_8b,
USART_StopBits_1,
USART_Parity_No,
USART_Mode_Rx | USART_Mode_Tx,
USART_HardwareFlowControl_None
};

#ifdef CLI_HELP_FOR_COMMANDS
static const cli_map_t cli_map[] = 
{
  {"cli",       cli_cli_cmd_handler,            "shows brief CLI info"},
  {"help",      cli_help_cmd_handler,           "shows brief help for cmd"},
  {"name",      cli_name_cmd_handler,           "shows name of the device"},
  {"lis",       cli_lis_cmd_handler,            "shows accel readings"},
  {"lisreg",    cli_lisreg_cmd_handler,         "accel register operation"},
  {"listhrsh",  cli_listhrsh_cmd_handler,       "sets thrsh for clicks"},
  {"lislat",    cli_lislat_cmd_handler,         "shows specified font char"},
  {"listime",   cli_listime_cmd_handler,        "shows specified font char"},
  {"liswin",    cli_liswin_cmd_handler,         "shows specified font char"},
  {"test",      cli_test_cmd_handler,           "test"},
  {"char",      cli_char_cmd_handler,           "shows specified font char"},
  {"pixel",     cli_pixel_cmd_handler,          "puts pixel in specified place"},
  {"boot",      cli_boot_cmd_handler,           "starts DFU bootloader"}
};

#else
static const cli_map_t cli_map[] = 
{
  {"cli",       cli_cli_cmd_handler},
  {"name",      cli_name_cmd_handler}
};
#endif

void cli_init(serint_desc_t* d)
{
interface = d;
serint_open(interface,
            &usart_settings,
            (u8*)inbuf,
            CLI_BUFLEN);
}

static u16 processed = 0;
void cli(void)
{
while(interface->receiving_flag){} // wait till receive everything
while(processed < interface->in_counter)
{
  if(interface->inbuf[processed] == CLI_CMD_TERMINATION_SYMBOL)
  {
  u16 resplen = 0;
  cli_cmd_process  ((char*)interface->inbuf,     //command string
                     processed,                  //command string length
                     response,                   //char buffer for response
                     &resplen,
                     cli_map,                    //cli command map
                     ARRAY_SIZE(cli_map),        //cli command map length
                     0);                         //nesting flag. Is used if response must include command
  
  serint_send(interface, (u8*)response, resplen);
  serint_flush_inbuf(interface);
  processed = 0;
  }
  processed++;
  if(processed == CLI_BUFLEN)  // to avoid overflow
  {
  serint_flush_inbuf(interface);
  processed = 0;
  }
}
}

serint_desc_t* get_cli_interface(void)
{
return interface ;
}