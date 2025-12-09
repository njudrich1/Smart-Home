#ifndef CRC_H
#define CRC_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

class Crc  // Crc Base Class
{
public:
    virtual uint16_t get_init(void) = 0;

    virtual uint16_t encode(uint8_t data, uint16_t crc,  uint8_t num_byte) = 0;

    virtual uint16_t decode(uint8_t data, uint16_t crc,  uint8_t num_byte) = 0;

    virtual bool verify(uint16_t crc) = 0;

};

#endif