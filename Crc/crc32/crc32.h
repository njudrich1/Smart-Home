#ifndef CRC32_H
#define CRC32_H

#include "crc.h"

enum crc32_type {
    e_BZIP_TWO,
    e_CKSUM,
    e_ISOHDLC,
    e_JAMCRC,
    e_MPEG_TWO
};

class Crc32 : public Crc
{
    /*  TODO
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
    Crc32(uint32_t init, bool ref_in, bool ref_out, uint32_t poly, uint32_t xor_out, uint32_t residue=0);
    uint32_t get_init(void);
    uint32_t encode(uint8_t data, uint32_t crc,  uint8_t num_byte);
    uint32_t decode(uint8_t data, uint32_t crc,  uint8_t num_byte);
    bool verify(uint32_t crc);
    ~Crc32();

private:
    uint32_t _init;
    bool _ref_in;
    bool _ref_out;
    uint32_t _poly;
    uint32_t _xor_out; // If this is 0xFFFF, the CRC algorithm will produce a residue, when decoder, instead of a zero value.
    uint32_t _residue;

    uint8_t _byte_counter = 0x00;
    const uint32_t _TOP_BIT_MASK = 0x80000000;

    uint32_t _checksum(uint8_t data, uint32_t crc); 
    uint8_t _reflection_input(uint8_t data, uint8_t nbits);
    uint32_t _prepare_data(uint8_t data, uint32_t crc);
    uint32_t _binary_division(uint32_t crc);
    uint32_t _process_encode_result(uint32_t crc);
    uint32_t _process_decode_result(uint32_t crc);
    uint32_t _reflection_output(uint32_t data, uint8_t nbits);
};


#endif
