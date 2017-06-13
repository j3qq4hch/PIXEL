#include "cmd_handlers.h"
#include <stdio.h>

cmd_retval_t cli_name_cmd_handler(char *cmd, u16 cmdlen, char *response, u16 *response_len)
{
  *response_len = sprintf(response,"DAS V1");
  return CLI_cmd_OK;
}