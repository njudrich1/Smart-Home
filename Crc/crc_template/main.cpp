
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "crct.hpp"

//#include <template/crct.hpp>
// --------------- Tests --------------- //

#include <cassert>

// ----- Choose one of the CRCs. -----
//#define CRC16
#define CRC32

// ----- Choose one of the msg lengths. -----
//#define MSG_LEN_ONE
#define MSG_LEN_NINE

// ----- Choose operation -----
//#define ENCODE
#define DECODE

// ----- Choose one of the CRC-16. If CRC-16 is selected. -----
//#define XMODEM
//#define IBM3740
//#define GSM  // Has a Residue
//#define GENIBUS  // Has a Residue
//#define KERMIT
//#define MCRF4XX
//#define IBMSDLC  // Has a Residue

// ----- Choose one of the CRC-32. If CRC-32 is selected. -----
//#define BZIP_TWO // Has a Residue
#define CKSUM // Residue 
//#define ISOHDLC  // Has a Residue 
//#define JAMCRC
//#define MPEG2


int main(void)
{

// ---------- CRC-16 Algorithms ---------- 
#ifdef CRC16

#ifdef XMODEM
    printf("CRC16 - XMODEM\n");
    CrcT<uint16_t> crc(0x0000, false, false, 0x1021, 0x0000);
#ifdef MSG_LEN_ONE
    uint16_t EXPECTED_CRC = 0x2672;
#endif
#ifdef MSG_LEN_NINE
    uint16_t EXPECTED_CRC = 0x31C3;
#endif

    uint16_t EXPECTED_REMAINDER = 0x0000;
#endif

#ifdef IBM3740
    printf("CRC16 - IBM3740\n");
    CrcT<uint16_t> crc(0xFFFF, false, false, 0x1021, 0x0000);
#ifdef MSG_LEN_ONE
    uint16_t EXPECTED_CRC = 0xC782;
#endif
#ifdef MSG_LEN_NINE
    uint16_t EXPECTED_CRC = 0x29B1;
#endif

    uint16_t EXPECTED_REMAINDER = 0x0000;
#endif

#ifdef GSM  
    printf("CRC16 - GSM\n");
    CrcT<uint16_t> crc(0x0000, false, false, 0x1021, 0xFFFF, 0x1D0F);  // Residue
#ifdef MSG_LEN_ONE
    uint16_t EXPECTED_CRC = 0xD98D;
#endif
#ifdef MSG_LEN_NINE
    uint16_t EXPECTED_CRC = 0xCE3C;
#endif
    uint16_t EXPECTED_REMAINDER = 0x1D0F;
#endif

#ifdef GENIBUS  
    printf("CRC16 - GENIBUS\n");
    CrcT<uint16_t> crc(0xFFFF, false, false, 0x1021, 0xFFFF, 0x1D0F);  // Residue
#ifdef MSG_LEN_ONE
    uint16_t EXPECTED_CRC = 0x387D;
#endif
#ifdef MSG_LEN_NINE
    uint16_t EXPECTED_CRC = 0xD64E;
#endif
    uint16_t EXPECTED_REMAINDER = 0x1D0F;

#endif

#ifdef KERMIT
    printf("CRC16 - KERMIT\n");
    CrcT<uint16_t> crc(0x0000, true, true, 0x1021, 0x0000);
#ifdef MSG_LEN_ONE
    uint16_t EXPECTED_CRC = 0x200A;
#endif
#ifdef MSG_LEN_NINE
    uint16_t EXPECTED_CRC = 0x2189;
#endif

    uint16_t EXPECTED_REMAINDER = 0x0000;
#endif

#ifdef MCRF4XX
    printf("CRC16 - MCRF4XX\n");
    CrcT<uint16_t> crc(0xFFFF, true, true, 0x1021, 0x0000);
#ifdef MSG_LEN_ONE
    uint16_t EXPECTED_CRC = 0x2F8D;
#endif
#ifdef MSG_LEN_NINE
    uint16_t EXPECTED_CRC = 0x6F91;
#endif
    
    uint16_t EXPECTED_REMAINDER = 0x0000;
#endif

#ifdef IBMSDLC
    printf("CRC16 - IBMSDLC\n");
    CrcT<uint16_t> crc(0xFFFF, true, true, 0x1021, 0xFFFF, 0xF0B8); // Residue
#ifdef MSG_LEN_ONE
    uint16_t EXPECTED_CRC = 0xD072;
#endif
#ifdef MSG_LEN_NINE
    uint16_t EXPECTED_CRC = 0x906E;
#endif

    uint16_t EXPECTED_REMAINDER = 0xF0B8;
#endif

#endif

// ---------- CRC-32 Algorithms ---------- 
#ifdef CRC32

#ifdef BZIP_TWO
    printf("CRC32 - BZIP_TWO\n");
    CrcT<uint32_t> crc(0xFFFFFFFF, false, false, 0x04C11DB7, 0xFFFFFFFF, 0xc704dd7b); // Residue

#ifdef MSG_LEN_ONE
    uint32_t EXPECTED_CRC = 0x6104306C;
#endif
#ifdef MSG_LEN_NINE
    uint32_t EXPECTED_CRC = 0xFC891918;
#endif

    uint32_t EXPECTED_REMAINDER = 0xc704dd7b;
#endif

#ifdef CKSUM
    printf("CRC32 - CKSUM\n");
    CrcT<uint32_t> crc(0x00000000, false, false, 0x04C11DB7, 0xFFFFFFFF, 0xc704dd7b); // Residue 

#ifdef MSG_LEN_ONE
    uint32_t EXPECTED_CRC = 0x2F0C8FD8;
#endif
#ifdef MSG_LEN_NINE
    uint32_t EXPECTED_CRC = 0x765E7680;
#endif
    uint32_t EXPECTED_REMAINDER = 0xc704dd7b;
#endif

#ifdef ISOHDLC 
    printf("CRC32 - ISOHDLC\n");
    CrcT<uint32_t> crc(0xFFFFFFFF, true, true, 0x04C11DB7, 0xFFFFFFFF, 0xdebb20e3); // Residue

#ifdef MSG_LEN_ONE
    uint32_t EXPECTED_CRC = 0x83DCEFB7;
#endif
#ifdef MSG_LEN_NINE
    uint32_t EXPECTED_CRC = 0xCBF43926;
#endif
    uint32_t EXPECTED_REMAINDER = 0xdebb20e3;
#endif

#ifdef JAMCRC  
    printf("CRC32 - JAMCRC\n");
    CrcT<uint32_t> crc(0xFFFFFFFF, true, true, 0x04C11DB7, 0x00000000);

#ifdef MSG_LEN_ONE
    uint32_t EXPECTED_CRC = 0x7C231048;
#endif
#ifdef MSG_LEN_NINE
    uint32_t EXPECTED_CRC = 0x340BC6D9;
#endif
    uint32_t EXPECTED_REMAINDER = 0x00000000;
#endif

#ifdef MPEG2
    printf("CRC32 - MPEG2\n");
    CrcT<uint32_t> crc(0xFFFFFFFF, false, false, 0x04C11DB7, 0x00000000);

#ifdef MSG_LEN_ONE
    uint32_t EXPECTED_CRC = 0x9EFBCF93;
#endif
#ifdef MSG_LEN_NINE
    uint32_t EXPECTED_CRC = 0x0376E6E7;
#endif
    uint32_t EXPECTED_REMAINDER = 0x00000000;
#endif

#endif


    printf("Test:\n");

#ifdef CRC16
    int CRC_SIZE = 2;      
    uint16_t temp_crc = crc.get_init(); // init
    uint16_t temp_crc2 = crc.get_init(); // init
    uint16_t crc_result = 0x0000; // init
    uint16_t crc_result2 = 0x0000; // init
#endif

#ifdef CRC32
    int CRC_SIZE = 4;  
    uint32_t temp_crc = crc.get_init(); // init
    uint32_t temp_crc2 = crc.get_init(); // init  
    uint32_t crc_result = 0x00000000; // init
    uint32_t crc_result2 = 0x00000000; // init
#endif

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
        temp_crc = crc.encode(data_buffer[i], temp_crc, MSG_SIZE);
        //printf("%d: %x\n", i, temp_crc);
    }
    crc_result = temp_crc;
    printf("Crc Result: %x\n", crc_result); 

    assert((crc_result == EXPECTED_CRC && "Incorrect CRC calculated"));

    printf("END -------------------------------\n");
#endif

#ifdef DECODE

#ifdef CRC16
    printf("\n-------------------------------Decode Data:-------------------------------\n");
    // ------------ Non-Reflection Tests (refin=false and refout=false) ----------------
#ifdef XMODEM
    // XMODEM - Tested with init=0x0000 and xor_out=0x0000
#ifdef MSG_LEN_ONE
    uint8_t data_buffer2[3] = {0x31, 0x26, 0x72};
#endif
#ifdef MSG_LEN_NINE
    uint8_t data_buffer2[9+2] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x31, 0xC3};
#endif

#endif

#ifdef IBM3740
    // IBM-3740 - Tested with init=0xFFFF and xor_out=0x0000
#ifdef MSG_LEN_ONE
    uint8_t data_buffer2[3] = {0x31, 0xC7, 0x82};
#endif
#ifdef MSG_LEN_NINE
    uint8_t data_buffer2[9+2] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x29, 0xB1};
#endif

#endif

#ifdef GSM
    // GSM - Tested with init=0x0000 and xor_out=0xFFFF; Residue = 0x1D0F
#ifdef MSG_LEN_ONE
    uint8_t data_buffer2[3] = {0x31, 0xD9, 0x8D}; 
#endif
#ifdef MSG_LEN_NINE
    uint8_t data_buffer2[9+2] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0xCE, 0x3C};
#endif

#endif

#ifdef GENIBUS
    // GENIBUS - Tested with init=0xFFFF and xor_out=0xFFFF; Residue = 0x1D0F
#ifdef MSG_LEN_ONE
    uint8_t data_buffer2[3] = {0x31, 0x38, 0x7D};
#endif
#ifdef MSG_LEN_NINE
    uint8_t data_buffer2[9+2] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0xD6, 0x4E};
#endif

#endif

    // ------------ Reflection Tests (refin=true and refout=true) ----------------- 
#ifdef KERMIT
    // KERMIT - Tested with init=0x0000 and xor_out=0x0000 
#ifdef MSG_LEN_ONE
    uint8_t data_buffer2[3] = {0x31, 0x0a, 0x20};  // NOTE: the CRC 0x200A is reflected in the recieved data.
#endif
#ifdef MSG_LEN_NINE
    uint8_t data_buffer2[9+2] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x89, 0x21};  // NOTE: the CRC 0x2189 is reflected in the recieved data.
#endif

#endif

#ifdef MCRF4XX
    // MCRF4XX - Tested with init=0xFFFF and xor_out=0x0000
#ifdef MSG_LEN_ONE
    uint8_t data_buffer2[3] = {0x31, 0x8D, 0x2F}; // NOTE: the CRC 0x2F8D is reflected in the recieved data.
#endif
#ifdef MSG_LEN_NINE
    uint8_t data_buffer2[9+2] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x91, 0x6F};  // NOTE: the CRC 0x6F91 is reflected in the recieved data.
#endif

#endif

#ifdef IBMSDLC
    // IBM-SDLC - Tested with init=0xFFFF and xor_out=0xFFFF; Residue = 0xF0B8 (No CRC-16 of init=0x0000 and xor_out=0xFFFF)
#ifdef MSG_LEN_ONE
    uint8_t data_buffer2[3] = {0x31, 0x72, 0xD0}; // NOTE: the CRC 0xD072 is reflected in the recieved data.
#endif
#ifdef MSG_LEN_NINE
    uint8_t data_buffer2[9+2] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x6E, 0x90};  // NOTE: the CRC 0x906E is reflected in the recieved data.
#endif

#endif
#endif


#ifdef CRC32

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
#endif


    for (int i = 0; i < MSG_SIZE+CRC_SIZE; i++)
    {
        temp_crc2 = crc.decode(data_buffer2[i], temp_crc2, MSG_SIZE+CRC_SIZE);
    }

    crc_result2 = temp_crc2;
    printf("Remainder: %x", crc_result2); 

    if (crc.verify(crc_result2))
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