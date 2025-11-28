#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <stdlib.h>

int main(void)
{
    uint8_t rx[32];
    uint8_t msg_length;
    rx[0] = 0x31;
    uint8_t mask = ~((1 << 4) | (1 << 5) | (1 << 6) | (1 << 7)); 
    msg_length = mask & rx[0]; // Clear MSB nibble.

    //msg_length = atoi(msg_length);

    printf("answer: %x\n", msg_length);
    printf("mask: %x", mask);

    return 0;
}