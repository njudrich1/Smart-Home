#ifndef CRC16_H
#define CRC16_H

#include "crc.h"

enum crc_type {
    e_XMODEM,
    e_IBM3740,
    e_GSM,
    e_GENIBUS,
    e_KERMIT,
    e_MCRF4XX,
    e_IBMSDLC
};

class Crc16: public Crc
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
                           as 31_0a_20.
    */
public:
    Crc16(uint16_t init, bool ref_in, bool ref_out, uint16_t poly, uint16_t xor_out, uint16_t residue=0);
    uint16_t get_init(void);
    uint16_t encode(uint8_t data, uint16_t crc,  uint8_t num_byte);
    uint16_t decode(uint8_t data, uint16_t crc,  uint8_t num_byte);
    bool verify(uint16_t crc);
    ~Crc16();

private:
    uint16_t _init;
    bool _ref_in;
    bool _ref_out;
    uint16_t _poly;
    uint16_t _xor_out; // If this is 0xFFFF, the CRC algorithm will produce a residue, when decoder, instead of a zero value.
    uint16_t _residue;

    uint8_t _byte_counter = 0x00;
    const uint16_t _TOP_BIT_MASK = 0x8000;

    uint16_t _checksum(uint8_t data, uint16_t crc); 
    uint8_t _reflection_input(uint8_t data, uint8_t nbits);
    uint16_t _prepare_data(uint8_t data, uint16_t crc);
    uint16_t _binary_division(uint16_t crc);
    uint16_t _process_encode_result(uint16_t crc);
    uint16_t _process_decode_result(uint16_t crc);
    uint16_t _reflection_output(uint16_t data, uint8_t nbits);
};


#endif