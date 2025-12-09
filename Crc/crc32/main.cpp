
#include "crc32.h"

#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// --------------- Tests --------------- //

#include <cassert>

// ----- Choose one of the msg lengths. -----
//#define MSG_LEN_ONE
#define MSG_LEN_NINE

// ----- Choose operation -----
//#define ENCODE
#define DECODE

// ----- Choose one of the CRC-16. -----
//#define BZIP_TWO // Has a Residue
//#define CKSUM // Residue 
//#define ISOHDLC  // Has a Residue 
#define JAMCRC
//#define MPEG2

// Helper Function
Crc* get_crc(crc32_type type)
{
    Crc *crc = nullptr;

    switch (type)
    {
    case crc32_type::e_BZIP_TWO:
        crc = new Crc32(0xFFFFFFFF, false, false, 0x04C11DB7, 0xFFFFFFFF, 0xc704dd7b); // Residue 
        break;
    case crc32_type::e_CKSUM:
        crc = new Crc32(0x00000000, false, false, 0x04C11DB7, 0xFFFFFFFF, 0xc704dd7b); // Residue 
        break;
    case crc32_type::e_ISOHDLC:
        crc = new Crc32(0xFFFFFFFF, true, true, 0x04C11DB7, 0xFFFFFFFF, 0xdebb20e3); // Residue 
        break;
    case crc32_type::e_JAMCRC:
        crc = new Crc32(0xFFFFFFFF, true, true, 0x04C11DB7, 0x00000000);
        break;
    case crc32_type::e_MPEG_TWO:
        crc = new Crc32(0xFFFFFFFF, false, false, 0x04C11DB7, 0x00000000);
        break;

    default:
        crc = new Crc32(0xFFFFFFFF, false, false, 0x04C11DB7, 0xFFFFFFFF, 0xc704dd7b);  // e_BZIP2
        break;
    }
    
    return crc;
}

int main(void)
{
#ifdef BZIP_TWO
crc32_type algo = crc32_type::e_BZIP_TWO;

#ifdef MSG_LEN_ONE
    uint32_t EXPECTED_CRC = 0x6104306C;
#endif
#ifdef MSG_LEN_NINE
    uint32_t EXPECTED_CRC = 0xFC891918;
#endif

    uint32_t EXPECTED_REMAINDER = 0xc704dd7b;
#endif

#ifdef CKSUM
crc32_type algo = crc32_type::e_CKSUM;

#ifdef MSG_LEN_ONE
    uint32_t EXPECTED_CRC = 0x2F0C8FD8;
#endif
#ifdef MSG_LEN_NINE
    uint32_t EXPECTED_CRC = 0x765E7680;
#endif
    uint32_t EXPECTED_REMAINDER = 0xc704dd7b;
#endif

#ifdef ISOHDLC  
crc32_type algo = crc32_type::e_ISOHDLC;

#ifdef MSG_LEN_ONE
    uint32_t EXPECTED_CRC = 0x83DCEFB7;
#endif
#ifdef MSG_LEN_NINE
    uint32_t EXPECTED_CRC = 0xCBF43926;
#endif
    uint32_t EXPECTED_REMAINDER = 0xdebb20e3;
#endif

#ifdef JAMCRC  
crc32_type algo = crc32_type::e_JAMCRC;

#ifdef MSG_LEN_ONE
    uint32_t EXPECTED_CRC = 0x7C231048;
#endif
#ifdef MSG_LEN_NINE
    uint32_t EXPECTED_CRC = 0x340BC6D9;
#endif
    uint32_t EXPECTED_REMAINDER = 0x00000000;
#endif

#ifdef MPEG2
crc32_type algo = crc32_type::e_MPEG_TWO;

#ifdef MSG_LEN_ONE
    uint32_t EXPECTED_CRC = 0x9EFBCF93;
#endif
#ifdef MSG_LEN_NINE
    uint32_t EXPECTED_CRC = 0x0376E6E7;
#endif
    uint32_t EXPECTED_REMAINDER = 0x00000000;
#endif

    printf("Test:\n");
    Crc *ptr = get_crc(algo);


    uint32_t temp_crc = ptr->get_init(); // init
    uint32_t temp_crc2 = ptr->get_init(); // init
    //uint32_t temp_crc = 0x00000000; // init
    //uint32_t temp_crc2 = 0x00000000; // init
    uint32_t crc_result = 0x00000000; // init
    uint32_t crc_result2 = 0x00000000; // init

    // ------- Test Data Frame --------
#ifdef MSG_LEN_ONE
    uint8_t data_buffer[1] = {0x31};
    int MSG_SIZE = 1;// DBG
#endif

#ifdef MSG_LEN_NINE
    uint8_t data_buffer[9] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
    int MSG_SIZE = 9;// DBG
#endif

#ifdef ENCODE

    printf("-------------------------------Encode Data:-------------------------------\n");
    for (int i = 0; i < MSG_SIZE; i++)
    {
        temp_crc = ptr->encode(data_buffer[i], temp_crc, MSG_SIZE);
        //printf("%d: %x\n", i, temp_crc);
    }
    crc_result = temp_crc;
    printf("Crc Result: %x\n", crc_result); 

    assert((crc_result == EXPECTED_CRC && "Incorrect CRC calculated"));

    printf("END -------------------------------\n");
#endif


#ifdef DECODE

    printf("\n-------------------------------Decode Data:-------------------------------\n");
    // ------------ Non-Reflection Tests (refin=false and refout=false) ----------------
#ifdef BZIP_TWO
    // BZIP2 - Tested with init=0xFFFFFFFF and xor_out=0xFFFFFFFF and residue=0xc704dd7b
#ifdef MSG_LEN_ONE
    uint8_t data_buffer2[1+4] = {0x31, 0x61, 0x04, 0x30, 0x6C};
#endif
#ifdef MSG_LEN_NINE
    uint8_t data_buffer2[9+4] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0xFC, 0x89, 0x19, 0x18};
#endif

#endif

#ifdef CKSUM
    // CKSUM - Tested with init=0x00000000 and xor_out=0xFFFFFFFF and residue=0xc704dd7b
#ifdef MSG_LEN_ONE
    uint8_t data_buffer2[1+4] = {0x31, 0x2F, 0x0C, 0x8F, 0xD8};
#endif
#ifdef MSG_LEN_NINE
    uint8_t data_buffer2[9+4] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x76, 0x5E, 0x76, 0x80};
#endif

#endif

#ifdef MPEG2
    // MPEG2 - Tested with init=0xFFFFFFFF and xor_out=0x00000000
#ifdef MSG_LEN_ONE
    uint8_t data_buffer2[1+4] = {0x31, 0x9E, 0xFB, 0xCF, 0x93}; 
#endif
#ifdef MSG_LEN_NINE
    uint8_t data_buffer2[9+4] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x03, 0x76, 0xE6, 0xE7};
#endif  // 0x0376E6E7

#endif


    // ------------ Reflection Tests (refin=true and refout=true) ----------------- 
#ifdef ISOHDLC
    // ISOHDLC - Tested with init=0xFFFFFFFF and xor_out=0xFFFFFFFF and residue=0xdebb20e3
#ifdef MSG_LEN_ONE
    uint8_t data_buffer2[1+4] = {0x31, 0xB7, 0xEF, 0xDC, 0x83};  // NOTE: the CRC 0x83DCEFB7 is reflected in the recieved data.
#endif
#ifdef MSG_LEN_NINE
    uint8_t data_buffer2[9+4] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x26, 0x39, 0xF4, 0xCB};  // NOTE: the CRC 0xCBF43926 is reflected in the recieved data.
#endif

#endif

#ifdef JAMCRC
    // JAMCRC - Tested with init=0xFFFFFFFF and xor_out=0x00000000
#ifdef MSG_LEN_ONE
    uint8_t data_buffer2[1+4] = {0x31, 0x48, 0x10, 0x23, 0x7C}; // NOTE: the CRC 0x7C231048 is reflected in the recieved data.
#endif
#ifdef MSG_LEN_NINE
    uint8_t data_buffer2[9+4] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0xD9, 0xC6, 0x0B, 0x34};  // NOTE: the CRC 0x340BC6D9 is reflected in the recieved data.
#endif

#endif

    for (int i = 0; i < MSG_SIZE+4; i++)
    {
        temp_crc2 = ptr->decode(data_buffer2[i], temp_crc2, MSG_SIZE+4);
    }

    crc_result2 = temp_crc2;
    printf("Remainder: %x", crc_result2); 

    if (ptr->verify(crc_result2))
    {
        printf("\nCRC valid\n");
    }
    else
    {
        printf("\nCRC Invalid\n");
    }

    assert((crc_result2 == EXPECTED_REMAINDER && "Unexpected remainder"));

    printf("END -------------------------------\n");
#endif

    return 0;
}