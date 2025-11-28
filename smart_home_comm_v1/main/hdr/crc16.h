#ifndef CRC16_H
#define CRC16_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

class Crc16 
{
    /* 
        CRC-16/XMODEM 
        Poly: 0x1021
        Init: 0x0000
        RefIn: false
        RefOut: false
        XorOut: 0x0000
    */
private:
    uint16_t _init;
    uint16_t _xor_out;
    uint16_t _poly;
    const uint16_t _TOP_BIT_MASK = 0x8000;
    uint16_t _binary_division(uint16_t crc);
    uint16_t _prepare_data(uint8_t *input_data, uint16_t crc);
    //uint16_t _calc_crc_from_one_byte(uint16_t init_crc);
public:
    Crc16(uint16_t init, uint16_t poly, uint16_t xor_out);
    uint16_t multi_byte_checksum(uint8_t *data, int nbytes);
    uint16_t checksum(uint16_t init_crc);
    bool verify(uint16_t crc);
    ~Crc16();
};


#endif