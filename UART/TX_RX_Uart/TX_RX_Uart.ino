/*typedef enum {DIRECTION_TX, DIRECTION_RX, DIRECTION_TX_RX} TransferDirection_Mode;
typedef enum {DATAWIDTH_8B, DATAWIDTH_9B} DataSize_Mode;
typedef enum {SYNCHRONOUS, ASYNCHRONOUS} Mode;
typedef enum {ONEBIT, TWOBIT} StopBit_Mode;
typedef enum {NONE, EVEN, ODD} Parity_Mode;

typedef struct
{
	TransferDirection_Mode UartDirection;
	DataSize_Mode UartDataWidth;
	Mode UartMode;
	StopBit_Mode UartStopBitSize;
	Parity_Mode UartParity;
	int BaudRate;

} UARTTypeDef; */

//#define FOSC 1843200 // Clock Speed
#define FOSC 16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

void init_uart0(unsigned int ubrr)
{
	/* Set the baud rate*/
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
 /* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
 /* Set frame format: 8data, 1stop bit */
	UCSR0C = (0<<USBS0)|(3<<UCSZ01);
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
	DDRB |= 1 << 7;
	init_uart0(MYUBRR);
 /*	UARTTypeDef uartObj = {0};
	  uartObj.UartDirection = DIRECTION_TX_RX;
	  uartObj.UartDataWidth = DATAWIDTH_8B;
	  uartObj.UartMode = ASYNCHRONOUS;
	  uartObj.UartStopBitSize = ONEBIT;
	  uartObj.UartParity = NONE;
	  uartObj.BaudRate = 9600; */

  while(1)
  {
		unsigned char t0 = 'c';
		unsigned char t1 = 'b';
		//unsigned char t2 = 0x43;
		PORTB |= 1 << 7;
    _delay_ms(1000);
		//USART_Transmit(t0);
		if (USART_Receive() == t0)
		{
			USART_Transmit(t1);
		 	PORTB &= ~(1<<7);
		 	_delay_ms(2000);
		}
  }
  return 0;
}
