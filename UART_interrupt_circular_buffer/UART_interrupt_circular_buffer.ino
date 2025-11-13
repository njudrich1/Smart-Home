#define FOSC 16000000 // Clock Speed
#define BAUD 9600



volatile int head = 0; // write
volatile int tail = 0; // read
const int max_buffer_size = 32;
volatile unsigned char cbuffer[max_buffer_size];

void buffer_write(unsigned char data)
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

ISR(USART0_RX_vect)
{
  // UART0 Recieve Complete - executes when data is recieved.
  buffer_write((unsigned char)UDR0);
}

void init_cbuffer(void)
{
  head = 0;
  tail = 0;
}

unsigned char buffer_read(void)
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

bool buffer_data_available(void)
{
  return ((head != tail)?true:false);
}

void init_uart0(unsigned char ubrr)
{
	/* Set the baud rate*/
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
 /* Enable receiver and transmitter and Recieve Interrupt*/
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
 /* Set frame format: 8data, 1stop bit DEFAULT*/ 
}

void USART_Transmit( unsigned char data )
 {
 /* Wait for empty transmit buffer */
 	while ( !( UCSR0A & (1<<UDRE0)) );
 /* Put data into buffer, sends the data */
 	UDR0 = data;
 }

 unsigned char USART_Receive( void )
 {
 /* Wait for data to be received */
 	while ( !(UCSR0A & (1<<RXC0)) );
 /* Get and return received data from buffer */
 	return UDR0;
 }

int main(void) 
{
	DDRB |= 1 << 7;  // Debug LED
	unsigned int MYUBRR = (FOSC/16/BAUD-1);  // Baud Rate calculation: 0x67
	init_uart0(MYUBRR); 
  sei();  // Enable Global Interrupts, SREG
  PORTB &= ~(1<<7);  // Debug LED
  unsigned char temp;
  init_cbuffer();

  while(1)
  {
    if (buffer_data_available())
    {
      PORTB ^= (1<<7); // Debug LED - Toggle
      temp = buffer_read();
      USART_Transmit(temp);  // Loopback
    }
    _delay_ms(100);
  }
  return 0;
}
