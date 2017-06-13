#ifndef __CPM
#define __CPM

#include "types.h"

#define ARRAY_SIZE(arr)         (sizeof(arr) / sizeof(arr[0]))
#define ISUSABLECHAR(x)         ((x) >= '"' && (x) <= '~')
#define ISPRINTABLE(x)          ((x) >  ' ' && (x) <= '~')

#define CLI_VERSION     "1.0"
#define CLI_HELP_FOR_COMMANDS                 //uncomment this if some brief HELP or description for each command is needed
#define CLI_RESP_INCLUDES_CMD           //uncomment this if response must include command itself


typedef enum 
{	
	CLI_cmd_OK,
	CLI_cmd_UNKNOWN,
	CLI_cmd_INVALID_PASS,
	CLI_cmd_INVALID_PARAMETERS,
	CLI_cmd_FAIL,
} cmd_retval_t;

typedef struct 
{
  cmd_retval_t retval;
  char* msg;
} cli_retval_map_t;

typedef struct
{
char* cmd;
cmd_retval_t (*cmd_handler)(char *cmd, u16 cmdlen, char *response, u16 *response_len); 
#ifdef CLI_HELP_FOR_COMMANDS
char* help;
#endif
}cli_map_t;



cmd_retval_t cli_cmd_process  (char *cmd,                 //command string
                               u16 cmd_len,               //command string length
                               char *response,            //char buffer for response
                               u16  *response_len,        
                               const cli_map_t * map,     //cli command map
                               u16 map_size,              //cli command map length
                               u8 nest_flag);              //nesting flag. Is used if response must include command

u16 cli_process(char *cmd,           //command string
                 u16 cmd_len,        //command string length
                 char *response);    //response buffer

u16          cli_retval_getmesage(cmd_retval_t retval,char* msg);

cmd_retval_t cli_help_cmd_handler       (char *cmd, u16 cmdlen, char *response, u16 *response_len);
cmd_retval_t cli_cli_cmd_handler        (char *cmd, u16 cmdlen, char *response, u16 *response_len);

#endif