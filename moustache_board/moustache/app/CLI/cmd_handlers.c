#include "cmd_handlers.h"
#include "lis302dl.h"
#include "5x7.h"
#include <stdio.h>
#include "test.h"
#include "5x7_font.h"
#include "uptime.h"
#include "bootloader.h"
cmd_retval_t cli_name_cmd_handler(char *cmd, u16 cmdlen, char *response, u16 *response_len)
{
  *response_len = sprintf(response,"Wattmeter");
  return CLI_cmd_OK;
}

///////////////////////////////////////////////

cmd_retval_t cli_lis_cmd_handler(char *cmd, u16 cmdlen, char *response, u16 *response_len)
{
  s8 r[6] = {0,0,0,0,0,0};
  lis_get_readings(r);
  *response_len = sprintf(response, "%i %i %i", r[1],r[3],r[5]);
  return CLI_cmd_OK;
}


cmd_retval_t cli_lisreg_cmd_handler(char *cmd, u16 cmdlen, char *response, u16 *response_len)
{
  u8 reg = 0;
  u8 val = 0;
  u8 k = sscanf(cmd,"%hhu %hhu",&reg,&val);
  if(k==2)
  {
  lis302d_regwrite(reg,val);
  return CLI_cmd_OK;
  }
  if(k==1)
  {
    val = lis302d_regread(reg);
    *response_len = sprintf(response, "%i = %i",reg,val);
    return CLI_cmd_OK;
  }
  return CLI_cmd_INVALID_PARAMETERS;
}

cmd_retval_t cli_test_cmd_handler(char *cmd, u16 cmdlen, char *response, u16 *response_len)
{
   return CLI_cmd_OK;
}

///////////////

cmd_retval_t cli_char_cmd_handler(char *cmd, u16 cmdlen, char *response, u16 *response_len)
{
char c = 0;
if(sscanf(cmd,"%c",&c)!=1)       return CLI_cmd_INVALID_PARAMETERS;
if((c<'a')||(c>'z'))            return CLI_cmd_INVALID_PARAMETERS;

//led_matrix_set_frame(char2font(c));
delay_ms__(1000);
return CLI_cmd_OK;
}

cmd_retval_t cli_listhrsh_cmd_handler(char *cmd, u16 cmdlen, char *response, u16 *response_len)
{
  u8 x,y,z;
 u8 k = sscanf(cmd,"%hhu %hhu %hhu", &x,&y,&z);
 if(k==3) 
 {
   lis302d_set_thresholds(x,y,z);
 }
 else
  {
   z = lis302d_regread(LIS302DL_CLICK_THSY_X);
   y = z>>4;
   x = z & 0x0F;
   z = lis302d_regread(LIS302DL_CLICK_THSZ);
   *response_len = sprintf(response,"Thresholds: x = %i y = %i z= %i",x,y,z);
   return CLI_cmd_OK;
  }
 
  return CLI_cmd_OK;
}

cmd_retval_t cli_lislat_cmd_handler(char *cmd, u16 cmdlen, char *response, u16 *response_len)
{
  u8 lat = 0;
  if(sscanf(cmd,"%hhu",&lat)==1)
  {
  lis302d_regwrite(LIS302DL_CLICK_latency,lat);
  }
  else
  {
  lat = lis302d_regread(LIS302DL_CLICK_latency);
  *response_len = sprintf(response,"Latency=%i",lat);

  }
  return CLI_cmd_OK;
}

cmd_retval_t cli_listime_cmd_handler(char *cmd, u16 cmdlen, char *response, u16 *response_len)
{
  u8 lat = 0;
  if(sscanf(cmd,"%hhu",&lat)==1)
  {
  lis302d_regwrite(LIS302DL_CLICK_timelimit,lat);
  }
  else
  {
  lat = lis302d_regread(LIS302DL_CLICK_timelimit);
  *response_len = sprintf(response,"Time=%i",lat);
  }
  return CLI_cmd_OK;
}


cmd_retval_t cli_liswin_cmd_handler     (char *cmd, u16 cmdlen, char *response, u16 *response_len)
{
  u8 lat = 0;
  if(sscanf(cmd,"%hhu",&lat)==1)
  {
  lis302d_regwrite(LIS302DL_CLICK_window,lat);
  }
  else
  {
  lat = lis302d_regread(LIS302DL_CLICK_window);
  *response_len = sprintf(response,"Window=%i",lat);
  }
  return CLI_cmd_OK;
}

cmd_retval_t cli_pixel_cmd_handler(char *cmd, u16 cmdlen, char *response, u16 *response_len)
{
u8 x,y;
u16 b;

u8 k =sscanf(cmd,"%hhu %hhu %hu",&x,&y,&b);
if(k==3)
{
led_matrix_putpixel(x,y,b,NULL);
return CLI_cmd_OK;
}
if(k==2)
{
b=0xFF;
led_matrix_putpixel(x,y,b,NULL);
return CLI_cmd_OK;
}
return  CLI_cmd_INVALID_PARAMETERS;
}



cmd_retval_t cli_boot_cmd_handler       (char *cmd, u16 cmdlen, char *response, u16 *response_len)
{
u32 *z = (u32*)BOOT_SIGN_ADR;
*z = BOOT_SIGN;
NVIC_SystemReset();
return CLI_cmd_OK;
}