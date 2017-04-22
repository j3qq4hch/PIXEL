#include <string.h>

#include "settings.h"
#include "pwm_app.h"
#include "modbus_app.h"
#include "flash_access.h"


static u16     setting_serialize_stream(const setting_t * s, u8 id, void(*write_byte)(u8 b));
static const setting_t * setting_deserialize_stream(u8 (*get_byte) (u8*byte), const setting_t *map, u8 maplen);

u16 get_crc(u8* data, u16 len);

extern pwm_master_param_t pwm_param[];
modbus_app_param_t modbus_app_param = {0};
//OK, let array index of setting be its unique ID, by whitch it should be recognised if there is no readable string


extern u16 pwm_period_ticks;
static const u16 dflt_pwm_res = 1024;

static const modbus_app_param_t dflt_modbus_app_param = 
{
0x01,
115200,
"8N1"
};

static const pwm_master_param_t dflt_pwm_param = 
{
  NULL,
  1,
  1,
  100
};


static const setting_t settings[] =
{
{"pwmres",      TYPE_U16, &pwm_period_ticks,                  &dflt_pwm_res},

{"step0",       TYPE_U8,  &pwm_param[0].step,                 &dflt_pwm_param.step},
{"per0",        TYPE_U8,  &pwm_param[0].period,               &dflt_pwm_param.period},
{"val0",        TYPE_U16, &pwm_param[0].desired_value,        &dflt_pwm_param.desired_value},

{"step1",       TYPE_U8,  &pwm_param[1].step,                 &dflt_pwm_param.step},
{"per1",        TYPE_U8,  &pwm_param[1].period,               &dflt_pwm_param.period},
{"val1",        TYPE_U16, &pwm_param[1].desired_value,        &dflt_pwm_param.desired_value},

{"step2",       TYPE_U8,  &pwm_param[2].step,                 &dflt_pwm_param.step},
{"per2",        TYPE_U8,  &pwm_param[2].period,               &dflt_pwm_param.period},
{"val2",        TYPE_U16, &pwm_param[2].desired_value,        &dflt_pwm_param.desired_value},

{"step3",       TYPE_U8,  &pwm_param[3].step,                 &dflt_pwm_param.step},
{"per3",        TYPE_U8,  &pwm_param[3].period,               &dflt_pwm_param.period},
{"val3",        TYPE_U16, &pwm_param[3].desired_value,        &dflt_pwm_param.desired_value},

{"step4",       TYPE_U8,  &pwm_param[4].step,                 &dflt_pwm_param.step},
{"per4",        TYPE_U8,  &pwm_param[4].period,               &dflt_pwm_param.period},
{"val4",        TYPE_U16, &pwm_param[4].desired_value,        &dflt_pwm_param.desired_value},

{"step5",       TYPE_U8,  &pwm_param[5].step,                 &dflt_pwm_param.step},
{"per5",        TYPE_U8,  &pwm_param[5].period,               &dflt_pwm_param.period},
{"val5",        TYPE_U16, &pwm_param[5].desired_value,        &dflt_pwm_param.desired_value},

{"adr",         TYPE_U8,                                           &modbus_app_param.adr,       &dflt_modbus_app_param.adr},
{"baud",        TYPE_U32,                                          &modbus_app_param.baudrate,  &dflt_modbus_app_param.baudrate},
{"format",      TYPE_BYTE_ARRAY | sizeof(modbus_app_param.format), &modbus_app_param.format,    &dflt_modbus_app_param.format}
};

////////////////////////////////////////////////////////////////////////////////

void settings_load_dflt(void)
{
  for(u8 i = 0; i < ARRAY_SIZE(settings); i++)
  {
    switch(settings[i].type & 0xFF00)
    {
    case TYPE_U8: 
      {
      *(u8*)settings[i].val_p = 0;
      if(settings[i].default_val_p) *(u8*)settings[i].val_p = *(u8*)settings[i].default_val_p;
      break;
      }
    case TYPE_U16:
        {
          *(u16*)settings[i].val_p = 0;
          if(settings[i].default_val_p) *(u16*)settings[i].val_p = *(u16*)settings[i].default_val_p;
          break;
        }
    case TYPE_U32:
        {
          *(u32*)settings[i].val_p = 0;
          *(u32*)settings[i].val_p = *(u32*)settings[i].default_val_p;
          break;
        }
    case TYPE_BYTE_ARRAY:
        {
          u8 len = settings[i].type & 0xFF;
          memset(settings[i].val_p, 0x00, len); //make it GO AWAY later
          if(settings[i].default_val_p) memcpy(settings[i].val_p, settings[i].default_val_p, len);
          break;
        }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void settings_load(void)
{ 
  if(get_crc((u8*)SETTINGS_START_ADR,FLASH_PAGE_SIZE - 2) != *(u16*)(SETTINGS_START_ADR + FLASH_PAGE_SIZE - 2))
  {
    settings_load_dflt();
    return;
  }
  
  flash_init(SETTINGS_START_ADR);
  while(1)
  {
    if(setting_deserialize_stream(flash_read_byte, settings, ARRAY_SIZE(settings))== NULL) break;
  }
}

void settings_save(void)
{
  DISABLE_SYSTICK();
  flash_init(SETTINGS_START_ADR);
  FLASH_Unlock();
  FLASH_ErasePage(SETTINGS_START_ADR);
  
  for(u8 i = 0; i < ARRAY_SIZE(settings); i++)
  {
    setting_serialize_stream(&settings[i], i, flash_write_byte);
  }
  flash_finalize();
  FLASH_ProgramHalfWord( SETTINGS_START_ADR + FLASH_PAGE_SIZE - 2, get_crc((u8*)SETTINGS_START_ADR,FLASH_PAGE_SIZE - 2) );
  FLASH_Lock();
  ENABLE_SYSTICK();
}


static u16 setting_serialize_stream(const setting_t * s, u8 id, void(*write_byte)(u8 b))
{
 write_byte(id);   
 write_byte(s->type >> 8);
 write_byte(s->type & 0xFF);
 
 u16 c = 3;
 switch (s->type & 0xFF00)
  {
  case TYPE_U8:
    {
    write_byte(*(u8*)s->val_p);
    c++;
    break;
    } 
    
  case TYPE_U16:
    {
    write_byte(*(u16*)s->val_p >> 8);
    write_byte(*(u16*)s->val_p & 0xFF);      
    c+=2;
    break;
    }
    
  case TYPE_U32:
    {
    write_byte(*(u32*)s->val_p >> 24); 
    write_byte(*(u32*)s->val_p >> 16);
    write_byte(*(u32*)s->val_p >> 8);
    write_byte(*(u32*)s->val_p & 0xFF);      
    c+=4;
    break;
    }
  case TYPE_BYTE_ARRAY:
    {
    u8 len = s->type & 0xFF;
    u8* byte = (u8*)s->val_p;
    for(u8 i = 0; i < len; i++)
      {
      write_byte(*byte++);
      }
  c += len;
  break;
    }
  default:
    {
    c = 0xFFFF;
    break;
    }
  }
return c;
}


////////////////////////////////////////////////////////////////////////////////


// reads some bytes and then tries to find a place for them in map. 
// If success - returns pointer to modified setting emelent in map
// If something goes wrong - returns NULL
//
// u8 (*get_byte) (u8* byte) is a pointer to the function that must read byte from some stream 
// and put it to u8* byte ptr
// if OK - return 0 
// if failed to read - return 1
static const setting_t * setting_deserialize_stream(u8 (*get_byte) (u8*byte), const setting_t *map, u8 maplen)
{
 u8 id = 0;
 if(get_byte(&id)) return NULL;
 if(id > maplen) return NULL;           //invalid ID
 const setting_t * s = &map[id];
 
 u16 type = 0;
 if(get_byte((u8*)&type+1)) return NULL;
 if(get_byte((u8*)&type))   return NULL;
  
 if(type != s->type) return NULL;  //type mismatch
  
 switch (type & 0xFF00)
  {
  case TYPE_U8:
    {
    if(get_byte((u8*)s->val_p)) return NULL;
    break;
    } 
    
  case TYPE_U16:
    {
    if(get_byte((u8*)s->val_p + 1))     return NULL;
    if(get_byte((u8*)s->val_p))         return NULL;
    break;
    }
    
  case TYPE_U32:
    {
    if(get_byte((u8*)s->val_p + 3))     return NULL;
    if(get_byte((u8*)s->val_p + 2))     return NULL;
    if(get_byte((u8*)s->val_p + 1))     return NULL;
    if(get_byte((u8*)s->val_p))         return NULL;
    break;
    }
  case TYPE_BYTE_ARRAY:
    {
      u8 len = s->type & 0xFF;
  
      for(u8 i = 0; i < len; i++)
        {
          if(get_byte((u8*)s->val_p + i))     return NULL;
        }
  break;
    }
  default:
    {
      return NULL; // invalid type
      break;
    }
  }
return s;
}

////////////////////////////////////////////////////////////////////////////////
