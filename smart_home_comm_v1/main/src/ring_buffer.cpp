#include "../hdr/ring_buffer.h"

volatile int head = 0; // write
volatile int tail = 0; // read
const int max_buffer_size = 32;
volatile unsigned char cbuffer[max_buffer_size];

void init_cbuffer(void)
{
  head = 0;
  tail = 0;
}

void cbuffer_write(unsigned char data)
{
  int nextPos = head + 1;
  if (nextPos == max_buffer_size)  // On the next write the max buffer size is reached - Overwrite of the buffer is required
  {
    nextPos = 0;
    //PORTB ^= (1<<7);
  }
  if (nextPos != tail) // Buffer not full on next write
  {
    cbuffer[head] = data;
    head = nextPos;
  }
  else {
    // Buffer full on next write ((current head + 1) == current tail)
  }
}

unsigned char cbuffer_read(void)
{
  unsigned char temp = '\0';
  int nextPos = tail + 1;
  if (tail != head) // If the buffer is not empty
  {
      if (nextPos == max_buffer_size) // Prevent reading from outside buffer.
      {
        nextPos = 0;
      }
      temp = cbuffer[tail];
      tail = nextPos;
  }
  else {
    // Buffer is currently empty (current tail == current head)
  }
  return temp;
}

bool cbuffer_data_available(void)
{
  return ((head != tail)?true:false);
}