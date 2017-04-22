
#include "modbus.h"
#include "modbus_handlers.h"


u16 get_crc(u8* data, u16 len);


static u8 write_single_reg_handler (u8 * data, u8 * resp, u16 * l);
static u8 write_mult_reg_handler   (u8 * data, u8 * resp, u16 * l);
static u8 read_holding_reg_handler (u8 * data, u8 * resp, u16 * l);


static u8 access_holding_reg(u16 adr, u8 mode, u16 * val);

static const modbus_code_map_t supported_func_codes[] = 
{
#ifdef WRITE_SINGLE_REGISTER
  {WRITE_SINGLE_REGISTER,       write_single_reg_handler},
#endif
#ifdef WRITE_MULTIPLE_REGISTERS
  {WRITE_MULTIPLE_REGISTERS,    write_mult_reg_handler},
#endif 
#ifdef READ_HOLDING_REGISTERS
  {READ_HOLDING_REGISTERS,      read_holding_reg_handler}
#endif
};

static const reg_map_t holding_register_map[] = 
{ 
  {1,                   1,              version_access},
  {2,                   1,              pwm_period_access},
  {PWM_START_ADR,       PWM_END_ADR,    app_pwm_access},
  {STEP_START_ADR,      STEP_END_ADR,   app_step_access},
  {PERIOD_START_ADR,    PERIOD_END_ADR, app_period_access},
  {50,                  50,             serial_settings_access},
  {51,                  51,             modbus_adr_acceess},
  {53,                  53,             adc_access},
  {ID_START_ADR,        ID_END_ADR,     id_access}
};


static u8 modbus_adr = 0;

void modbus_set_adr(u8 m)
{
modbus_adr = m;
}

u8 modbus_get_adr(void)
{
return modbus_adr;
}

//returns length of response data part if everything is OK 
//otherwise returns 0
u16 modbus_process_request(u8 * data, u16 datalen, u8 * response, u16 response_maxlen)
{
    
  u16 crc = (*(data + datalen-2)<<8) + *(data + datalen-1);
  if(crc != get_crc(data, datalen - 2)) 
  {
    return 0; //CRC ERROR
  }
  
  response[0] = *data++;   //address
  response[1] = *data++;   //function code
  
  if((response[0])&&(response[0] != modbus_adr)) //Address mismatch
    { 
      return 0;
    }
  
  u16 resplen = 2;
  u8 error = ILLEGAL_FUNCTION;
  for(u8 i = 0; i < ARRAY_SIZE(supported_func_codes); i++)
  {
    if(response[1] == supported_func_codes[i].code) 
      {
       error = supported_func_codes[i].handler(data, &response[2], &resplen);
       break;
      }
  }
  if(error)
    {
    response[1] |= 0x80;
    response[2]  = error;
    resplen++;
    }
  
  crc = get_crc(response, resplen); 
  response[resplen++] = (crc & 0xFF00) >> 8;
  response[resplen++] = crc & 0x00FF;
  
  return resplen;
}


// Compute the MODBUS RTU CRC
u16 get_crc(u8* buf, u16 len)
{
  u16 crc = 0xFFFF;
  
  for (u16 pos = 0; pos < len; pos++) {
    crc ^= (u16)buf[pos];          // XOR byte into least sig. byte of crc
  
    for (int i = 8; i != 0; i--) // Loop over each bit
    {    
      if ((crc & 0x0001) != 0)  // If the LSB is set
      {      
        crc >>= 1;                    // Shift right and XOR 0xA001
        crc ^= 0xA001;
      }
      else                            // Else LSB is not set
        crc >>= 1;                    // Just shift right
    }
  }
  // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
  u8 a = crc & 0xFF;
  crc >>= 8;
  crc |= a<<8;
  return crc;  
}

/////////////////////////////////////////////////

static u8 access_holding_reg(u16 adr, u8 mode, u16 * val)
{
  for(u8 i = 0; i < ARRAY_SIZE(holding_register_map); i++)
  {
    if((adr >= holding_register_map[i].start_adr) && (adr <= holding_register_map[i].end_adr))
    {
    return holding_register_map[i].reg_access_func(mode, adr, val);
    }
  }
  return ILLEGAL_DATA_ADDRESS;
}

/////////////////////////////////////////////////
#ifdef WRITE_SINGLE_REGISTER
static u8 write_single_reg_handler(u8 * data, u8 * resp, u16 * l)
{
  u16 adr = ((*data++) << 8);// 
      adr +=  *(data++);
  u16 val = *data++ << 8;
      val+= *data++;

  *resp++ = (adr & 0xFF00) >>8;
  *resp++ =  adr & 0x00FF;
  
  *resp++ = (val & 0xFF00) >>8;
  *resp++ =  val & 0x00FF;
  *l += 4;
  return  access_holding_reg(adr++, REG_WRITE, &val);
}
#endif


#ifdef WRITE_MULTIPLE_REGISTERS
static u8 write_mult_reg_handler  (u8 * data, u8 * resp, u16 * l)
{ 
  
  u16 adr = ((*data++) << 8);
      adr +=  *(data++);
  u16 n   = ((*data++) << 8);
      n   +=  *(data++);
  
  *resp++ = (adr & 0xFF00) >>8;
  *resp++ =  adr & 0x00FF;
  *resp++ = (n & 0xFF00) >>8;;
  *resp++ =  n & 0x00FF;
 
  *l += 4;
  if(n * 2 != *(data++)) return ILLEGAL_DATA_VALUE;
  u8 error = 0;
  u16 val = 0;
  
  while(n--)
  {
  val = ((*data++) << 8);
  val+= *(data++);
  error = access_holding_reg(adr++, REG_WRITE, &val);
  if(error) {break;}
  }
  return error;
}
#endif

#ifdef READ_HOLDING_REGISTERS

static u8 read_holding_reg_handler(u8 * data, u8 * resp, u16 * l)
{ 
  u8 error = 0;
  u16 adr = ((*data++) << 8); // start address
      adr +=  *(data++);
  u16 n   = ((*data++) << 8); //number of regisgters
      n   +=  *(data++);
  
   *resp++ = n * 2; //byte count
   *l += 1;
   u16 val = 0;
  
  while(n--)
  {
  error = access_holding_reg(adr++, REG_READ, &val);
  if(!error) 
    {
    *resp++ = (val & 0xFF00) >>8;
    *resp++ = val & 0xFF;
    *l += 2;
    }
  else
    {
      break;
    }
  }
  return error;
}
#endif