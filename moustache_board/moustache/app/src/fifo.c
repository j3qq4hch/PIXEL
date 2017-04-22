#include "fifo.h"
#ifdef GENERIC_FIFO
#include <string.h>
#endif



u8 fifo_pop(fifo_t* f)
{
	if(f->counter == 0) return 0;
	s16 p = f->head - f->counter;
	if (p < 0) {p += f->buflen;}
	f->counter--;
	return f->buf[p];
}

u16 fifo_pop_buf(fifo_t* f, u8* buf, u16 len)
{
  u16 bytes_read = 0;
  while(len--)
  {
    if(f->counter == 0) break;
    s16 p = f->head - f->counter;
    if (p < 0) {p += f->buflen;}
    f->counter--;
    bytes_read++;
    *buf++ = f->buf[p];  
  }
return bytes_read;
}

void fifo_init(fifo_t* f, u8* data, u16 data_len)
{
f->buf		= data;
f->buflen	= data_len;
f->counter	= 0;
f->head		= 0;
}


//void fifo_push(fifo_t* f, u8 c)
//{
//    f->buf[f->head++] = c;
//    if(f->head == f->buflen) f->head = 0;
//    if(f->counter < f->buflen) f->counter++;
//}


#ifdef GENERIC_FIFO
void generic_fifo_init	(generic_fifo_t* f, u8* data, u16 elem_size, u16 elem_num)
{
f->buf			= data;
f->len			= elem_num;
f->element_size = elem_size;
f->counter		= 0;
f->head			= 0;
}

void generic_fifo_push	(generic_fifo_t* f, void* element)
{
	memcpy(&f->buf[f->head],element,f->element_size);
	f->head += f->element_size;
	if(f->counter < f->len) f->counter++;
	if(f->head == f->len * f->element_size) f->head = 0;
}

void   generic_fifo_pop	(generic_fifo_t* f, void* element)
{       
	if(f->counter == 0) return;
	s16 p = f->head - (f->counter*f->element_size);
	if (p < 0) {p += f->len * f->element_size;}
	f->counter--;
	memcpy(element,&f->buf[p],f->element_size);
}

#endif
