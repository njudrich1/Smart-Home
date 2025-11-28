#define FOSC 16000000 // Clock Speed
#define BAUD 9600

#include "hdr/ring_buffer.h"
#include "hdr/crc16.h"

/* 
  Data Frame
  | HEADER | Message Length | Message | CRC (2 bytes) | 
  
  Communication protocol used is UART. TODO: maybe use Ethernet instead - UDP or IP. 
  TODO: Add Source and Destination address sections to the data frame.

  Header: 
  One byte in length. 
  Header is 0x73, ASCII character 's'.
  Message Length:
  One byte in length. - TODO: Make atleast 2 bytes. Edit ASCII to int conversion code.
  Message: 
  9 bytes in length. - TODO: Depended on Msg Length SEE TODO in Msg Length
  CRC:
  Two bytes in length.
  CRC-16 is used. TODO: Decide what suitable CRC I should used. Maybe edit to use a CRC-32.

*/

ISR(USART0_RX_vect)
{
  // UART0 Recieve Complete - executes when data is recieved.
  cbuffer_write((unsigned char)UDR0);
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
  /* Debug LED*/
	DDRB |= 1 << 7;    // Initialise Debug LED
  PORTB &= ~(1<<7);  // Set to off

  /* UART */
	unsigned int MYUBRR = (FOSC/16/BAUD-1);  // Baud Rate calculation: 0x67 = 9600
	init_uart0(MYUBRR);                     // Initialise UART and UART RX interrupt
  sei();                                  // Enable Global Interrupts, SREG

  /* Circular Buffer */
  init_cbuffer(); // Initialise Circular Buffer to recieve data

  /* State Machine */
  enum comm_proto {                   // State Machine states - prefix 's_' indicates that the item in the enum is a state.
    s_wait_0x73,
    s_read_msg_length,
    s_read_msg
  };
  enum comm_proto state = s_wait_0x73;  // Initial state assigned to wait_0x73
  const uint8_t FRAME_HEADER = 0x73;

  /* Handling Receieve Data */
  // Buffers
  uint8_t rx[1];  // Temporary Data Input Buffer
  uint8_t data_buffer[32]; // Data Buffer to store recieved data
  // Counters
  uint8_t cnt = 1;  // The zeroth element in the buffer is never used.
  uint8_t msg_length = 0;  // Recieved Message Length
  // Bitwise Mask
  uint8_t mask = ~((1 << 4) | (1 << 5) | (1 << 6) | (1 << 7));  // Mask used to convert recieved msg length from ASCII to int

  /* CRC */
  // Constants
  const uint16_t INIT_CRC = 0x0000; // Initial CRC value - possible values 0x0000 or 0xFFFF depending on CRC type
  const uint16_t XOR_OUT = 0x0000;
  const uint8_t CRC_NUM_BYTES = 2;
  // CRC Objects
  Crc16 crc(0x0000, 0x1021, 0x0000);  // Declare the CRC object
  // Buffers
  uint16_t tmp_buffer_16 = 0x0000;    // Initialise temporary buffer, used to store the received data when converted from 8 bits to 16 bits
  uint16_t temp_crc = INIT_CRC;  // Acts an the Initial CRC value and a temporary buffer for the CRC
  uint16_t crc_result = 0x0000; // Temporary buffer to store the final CRC value

  while(1)
  {
    if (cbuffer_data_available())
    {
      rx[0] = cbuffer_read();
      switch (state) 
      {
        case s_wait_0x73: 
          if (rx[0] == FRAME_HEADER)
          {
            state = s_read_msg_length;
            PORTB ^= (1<<7); 
            //USART_Transmit('s');
          }
          break;

        case s_read_msg_length:
          msg_length = mask & rx[0];  // Convert ASCII value to integer representative.
          state = s_read_msg;
          break;

        case s_read_msg:
          data_buffer[cnt] = rx[0];
          USART_Transmit(data_buffer[cnt]);

          // Calculate CRC
          tmp_buffer_16 =  (uint16_t)data_buffer[cnt];
          temp_crc = temp_crc ^ (tmp_buffer_16 << 8);
          temp_crc = crc.checksum(temp_crc);
          
          if (cnt == msg_length + CRC_NUM_BYTES)
          {
            crc_result = temp_crc ^ XOR_OUT;

            if (crc.verify(crc_result))
            {
              PORTB ^= (1<<7); // DBG - TODO: CRC SUCCESS RESPONSE?
            }
            else {
              USART_Transmit('x');    // Data corrupted - TODO: CRC FAIL RESPONSE?
            }

            cnt = 1;
            temp_crc = INIT_CRC;
            state = s_wait_0x73;
          }
          else {
            cnt++;
          }
          break;
      }

    }
    _delay_ms(100);
  }
  return 0;
}
