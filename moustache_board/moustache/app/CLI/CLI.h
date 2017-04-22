#include "cpm.h"
#include "serint.h"

#define CLI_CMD_TERMINATION_SYMBOL      '\r'    //symbol which points to the end of command

void cli_init(serint_desc_t* desc);
void cli(void);
serint_desc_t* get_cli_interface(void);