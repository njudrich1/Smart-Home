#ifndef RING_BUFFER_H
#define RING_BUFFER_H

void init_cbuffer(void);
void cbuffer_write(unsigned char data);
unsigned char cbuffer_read(void);
bool cbuffer_data_available(void);

#endif