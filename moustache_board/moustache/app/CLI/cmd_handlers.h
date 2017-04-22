#ifndef __CMD_HANDLERS
#define __CMD_HANDLERS

#include "cpm.h"

cmd_retval_t cli_name_cmd_handler       (char *cmd, u16 cmdlen, char *response, u16 *response_len);
cmd_retval_t cli_lis_cmd_handler        (char *cmd, u16 cmdlen, char *response, u16 *response_len);
cmd_retval_t cli_lisreg_cmd_handler     (char *cmd, u16 cmdlen, char *response, u16 *response_len);
cmd_retval_t cli_test_cmd_handler       (char *cmd, u16 cmdlen, char *response, u16 *response_len);
cmd_retval_t cli_char_cmd_handler       (char *cmd, u16 cmdlen, char *response, u16 *response_len);
cmd_retval_t cli_listhrsh_cmd_handler   (char *cmd, u16 cmdlen, char *response, u16 *response_len);
cmd_retval_t cli_lislat_cmd_handler     (char *cmd, u16 cmdlen, char *response, u16 *response_len);
cmd_retval_t cli_listime_cmd_handler    (char *cmd, u16 cmdlen, char *response, u16 *response_len);
cmd_retval_t cli_liswin_cmd_handler     (char *cmd, u16 cmdlen, char *response, u16 *response_len);
cmd_retval_t cli_pixel_cmd_handler      (char *cmd, u16 cmdlen, char *response, u16 *response_len);
cmd_retval_t cli_boot_cmd_handler       (char *cmd, u16 cmdlen, char *response, u16 *response_len);
#endif