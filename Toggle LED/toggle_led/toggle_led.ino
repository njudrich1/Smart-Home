
// Toggles the in-built LED on the MCU
int main(void) 
{
  DDRB |= 1 << 7;
  while(1)
  {
    PORTB |= 1 << 7;
    _delay_ms(1000);
    PORTB &= ~(1<<7);
    _delay_ms(1000);
  }
  return 0;
};