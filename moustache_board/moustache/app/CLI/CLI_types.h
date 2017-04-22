
#ifndef __CLI_types
#define __CLI_types
#include "stm32f0xx.h"
//#include "interfaces.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

typedef enum {       
              PASS_OK,
              PASS_INVALID,
             } pass_check_flag_type;

typedef enum {
            CLI_cmd_OK,
            CLI_cmd_UNKNOWN,
            CLI_cmd_INVALID_PASS,
            CLI_cmd_INVALID_PARAMETERS,
            CLI_cmd_FAIL,
            } cmd_return_value_type;

typedef struct{
              cmd_return_value_type cmd_return_value;
              char* message;
              } CLI_return_value_message_map_struct;


typedef cmd_return_value_type(*CLI_cmd_handler_func_type)(char *cmd,         
                                                          char *response, 
                                                          u16 *response_len, 
                                                          pass_check_flag_type pass_check_flag);

typedef struct {
                 char *cmd;               // command name
                 CLI_cmd_handler_func_type cmd_handler; 
                 char *help;              // command help
               } cmd_func_map_struct;


#endif