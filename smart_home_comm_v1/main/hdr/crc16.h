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

  NOTE: Only when RefIn and RefOut are True, when verifying a data frame, the CRC needs to be transmitted as Big Endian.
              For Example: for the data 0x31 the CRC, with reflection true, is 0x200A. This should be stored in the data frame 
                           as 31_0a_20.    */
public:
    Crc16(uint16_t init, bool ref_in, bool ref_out, uint16_t poly, uint16_t xor_out);
    uint16_t checksum(uint8_t data, uint16_t crc, uint8_t num_bytes);
    uint16_t get_init(void);
    uint16_t get_xor_out(void);
    bool verify(uint16_t crc);
    ~Crc16();

private:
    uint16_t _init;
    bool _ref_in;
    bool _ref_out;
    uint16_t _poly;
    uint16_t _xor_out;

    uint8_t _byte_counter = 0x00;

    const uint16_t _TOP_BIT_MASK = 0x8000;
    
    uint8_t _reflection_input(uint8_t data, uint8_t nbits);
    uint16_t _prepare_data(uint8_t data, uint16_t crc);
    uint16_t _binary_division(uint16_t crc);
    uint16_t _process_final_crc(uint16_t crc);
    uint16_t _reflection_output(uint16_t data, uint8_t nbits);
};


#endif