// Header file for input output functions
#include <stdio.h>
#include <stdint.h>
#include <string.h>

uint16_t crc16(uint8_t data, int nbytes)
{
    uint16_t crc = 0;
    uint16_t data_16 =  (uint16_t)data;
    uint16_t check_msb = 0;

    for(int byte = 0; byte < nbytes; byte++)
    {
        printf("Round: %d:\n", byte);
        printf("1: %x\n", crc);
        printf("1.5: %x\n", (data_16 << 8)); // 0x3100 (11000100000000)

        crc = crc ^ (data_16 << 8);

        printf("2: %x\n", crc); // 0x3100 (11000100000000)

        for(unsigned char bit = 0; bit < 8; bit++)
        {
            printf("Bit %d:\n", bit);
            check_msb = crc & 0x8000;
            printf("3: %#x\n", check_msb);
            if (check_msb)
            {
                crc = crc << 1;
                printf("4: %x\n", crc);
                crc = crc ^ 0x1021; //
                printf("4.5: %x\n", crc);
            }
            else
            {
                printf("5: %x\n", crc << 1); // 0110001000000000 (0x6200)
                crc = crc << 1;
            }
            
        }
    }
    return crc;
}

// Main function: entry point for execution
int main() 
{
    // crc = 0001000000100001 (0x1021)
    // data = 00110001 (0x31) (ASCII character '1')

    // 2)
    uint8_t y = 0x32;
    int byte_num = 1;
    uint16_t result = 0;
    printf("Data in: %#x\n", y);

    result = crc16(y, byte_num);

    printf("Data out: %#x\n", result);
    printf("Size of y: %d\n", (int)sizeof(y));
    printf("Size of result: %d\n", (int)sizeof(result));

    return 0;
}