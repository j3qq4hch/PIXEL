#ifndef __FIFO
#define __FIFO

#include "types.h"

//#define GENERIC_FIFO	1

#define FIFO_SUCCESS            0x00
#define FIFO_TIMEOUT_ERROR      0x01   
#define FIFO_OVERFLOW_ERROR     0x02


typedef struct
{
	u8* buf;
	u16 buflen;
	volatile u16 counter;
	u16 head;
}
fifo_t;

void fifo_init		(fifo_t* f, u8* data, u16 data_len);
u8   fifo_pop		(fifo_t* f);
u16  fifo_pop_buf       (fifo_t* f, u8* buf, u16 len);



//void fifo_push(fifo_t* f, u8 c);

static inline void fifo_push(fifo_t* f, u8 c)
{
    f->buf[f->head++] = c;
    if(f->head == f->buflen) f->head = 0;
    if(f->counter < f->buflen) f->counter++;
}




#ifdef GENERIC_FIFO
typedef struct
{
	u8* buf;
	u16 len;
	u16 head;
	u16 counter;
	u8  element_size;
}
generic_fifo_t;


void generic_fifo_init	(generic_fifo_t* f, u8* data, u16 elem_size, u16 elem_num);
void generic_fifo_push	(generic_fifo_t* f, void* element);
void generic_fifo_pop	(generic_fifo_t* f, void* element);

#endif

#endif
