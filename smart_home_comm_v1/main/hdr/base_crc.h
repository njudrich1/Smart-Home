#ifndef BASE_CRC_H
#define BASE_CRC_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

class BaseCRC
{
    public:
        virtual uint8_t checksum(uint8_t data, uint8_t crc,  uint8_t num_byte) = 0;
        virtual bool verify(uint8_t crc) = 0;
};

#endif