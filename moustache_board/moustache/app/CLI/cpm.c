//command process module is the main part of command line interface(CLI)

#include "cpm.h"


#ifdef CLI_RESP_INCLUDES_CMD

cmd_retval_t cli_cmd_process  (char *cmd,                 //command string
                               u16 cmd_len,               //command string length
                               char *response,            //char buffer for response
                               u16  *response_len,
                               const cli_map_t * map,     //cli command map
                               u16 map_size,              //cli command map length
                               u8 nest_flag)              //nesting flag. Is used if response must include command
{
  
u16 l       = 0;
u16 n       = 0;
u16 resplen = 0;
cmd_retval_t retval = CLI_cmd_UNKNOWN;

while((ISPRINTABLE(*cmd))&&(cmd_len))
{
  l++;
  cmd++;
  cmd_len--;
}               //find lenght of the first word
if(!l) 
{
*response_len = 0;
return CLI_cmd_UNKNOWN;
}
if(l)
{
while( ( !(ISPRINTABLE(*cmd)) )&&(cmd_len) )
{
  n++;
  cmd++;
  cmd_len--;
}               //find lenght of the first word


cmd -= l+n;       //go to the beginning


if(nest_flag)
{
*(response-1) = ' ';
}
memcpy(response,cmd,l);
response   += l;
*response++ = ':';

const cli_map_t *q = map;

 
while(map_size--)
{
  if(strncasecmp(q->cmd,cmd,l) == 0)
  {
    if(strlen(q->cmd) == l)
    { 
  retval = q->cmd_handler(cmd + l + n,cmd_len,response,&resplen);
  break;
    }
  }
  q++;
}
}
if(resplen == 0)
                {
                resplen = cli_retval_getmesage(retval,response);
                }

response += resplen;

if(!nest_flag)
{
*response++ = '\r';
*response++ = '\n';
resplen += 2;
}

resplen += (l + 1);
*response_len += resplen;
return retval;
}

#else

cmd_retval_t cli_cmd_process  (char *cmd,                 //command string
                               u16 cmd_len,               //command string length
                               char *response,            //char buffer for response
                               u16  *response_len,
                               const cli_map_t * map,     //cli command map
                               u16 map_size,              //cli command map length
                               u8 nest_flag)              //nesting flag. Is used if response must include command
{
while(!(ISUSABLECHAR(*cmd))&&(cmd_len)) //skip everything till first usable char
{
  cmd++;
  cmd_len--;
}
if(cmd_len == 0) return 0;
u16 l       = 0;
u16 resplen = 0;
while((ISUSABLECHAR(*cmd))&&(cmd_len)) //find lenght of the first word
{
  l++;
  cmd++;
  cmd_len--;
}       
cmd -= l;               //go to the beginning

const cli_map_t *n = &cli_map[0];
cmd_retval_t retval = CLI_cmd_UNKNOWN;

while(map_size--)
{
  if(strncasecmp(n->cmd,cmd,l))
  {
  retval = n->cmd_handler(cmd,response,&resplen);
  break;
  }
}
if(resplen == 0)
                {
                resplen = cli_retval_getmesage(retval,response);
                }
response += resplen;
*response++ = '\r';
*response++ = '\n';
return retval;
}
#endif


static const cli_retval_map_t retval_map[] = 
{       
  {CLI_cmd_OK,                  "OK"},
  {CLI_cmd_UNKNOWN,             "UNKNOWNCMD"},
  {CLI_cmd_INVALID_PARAMETERS,  "PARAMERROR"},
  {CLI_cmd_FAIL,                "CMDFAIL"},
  {CLI_cmd_INVALID_PASS,        "PASSERROR"}
};


//puts message to msg buf and returns its length

u16 cli_retval_getmesage(cmd_retval_t retval,char* msg)
{
const cli_retval_map_t *map = &retval_map[0];
u16 len = 0;
    for(char i=0;i<ARRAY_SIZE(retval_map);i++,map++)
    {
    if(retval == map->retval) 
    {
     len = strlen(map->msg);
     memcpy(msg,  map->msg, len);
     break;
    }
    }
    
    return len;
}

///////////////////////////////////////////

cmd_retval_t cli_help_cmd_handler(char *cmd, u16 cmdlen, char *response, u16 *response_len)
{
return CLI_cmd_OK;
}

///////////////////////////////////////////

cmd_retval_t cli_cli_cmd_handler(char *cmd, u16 cmdlen, char *response, u16 *response_len)
{
return CLI_cmd_OK;
}

