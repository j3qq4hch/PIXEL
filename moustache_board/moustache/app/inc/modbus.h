
#ifndef __MODBUS
#define __MODBUS

#include "types.h"


#define MODBUS_WRITE_

//modbus error codes
#define NO_ERROR                        00
#define ILLEGAL_FUNCTION                01 
#define ILLEGAL_DATA_ADDRESS            02 
#define ILLEGAL_DATA_VALUE              03 
#define SERVER_DEVICE_FAILURE           04
#define ACKNOWLEDGE                     05 
#define SERVER_DEVICE_BUSY              06 
#define MEMORY_PARITY_ERROR             08 

//#define READ_COILS                      1
//#define READ_DISCRETE_INPUTS            2
#define READ_HOLDING_REGISTERS          3
//#define READ_INPUT_REGISTERS            4
//#define WRITE_SINGLE_COIL               5
//#define WRITE_MULTIPLE_COILS            15
#define WRITE_SINGLE_REGISTER           6
#define WRITE_MULTIPLE_REGISTERS        16

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

typedef struct
{
u8 code;
u8(*handler)(u8 * data, u8 * resp, u16 * l);
}modbus_code_map_t;


#define REG_READ        0
#define REG_WRITE       1


typedef struct
{
u16 start_adr;        //register address
u16 end_adr;        //register address
u8 (*reg_access_func)(u8 mode, u16 adr, u16 * val); //callback function which is called to access registrer value.      
                                                    //value valid-check must be performed here. 
}reg_map_t;

u16 modbus_process_request(u8 * data, u16 datalen, u8 * response, u16 response_maxlen);
void modbus_set_adr(u8 m);
u8 modbus_get_adr(void);
#endif