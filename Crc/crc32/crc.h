#ifndef CRC_H
#define CRC_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

class Crc  // Crc Base Class
{
public:
    virtual uint32_t get_init(void) = 0;

    virtual uint32_t encode(uint8_t data, uint32_t crc,  uint8_t num_byte) = 0;

    virtual uint32_t decode(uint8_t data, uint32_t crc,  uint8_t num_byte) = 0;

    virtual bool verify(uint32_t crc) = 0;

};

#endif