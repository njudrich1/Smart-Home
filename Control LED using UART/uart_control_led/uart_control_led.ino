#define FOSC 16000000 // Clock Speed
#define BAUD 9600

void init_uart0(unsigned char ubrr)
{
	/* Set the baud rate*/
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
 /* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
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
  PORTB &= ~(1<<7);  // Debug LED

  while(1)
  {
		//PORTB &= ~(1<<7);  // Debug LED

		unsigned char data = USART_Receive();
		if (data == 'o')
		{
			USART_Transmit(data);
			PORTB |= 1 << 7;  // Debug LED
		}
    
    if (data == 'f')
    {
      USART_Transmit(data);
			PORTB &= ~(1<<7);
    }
		_delay_ms(100);
  }
  return 0;
}