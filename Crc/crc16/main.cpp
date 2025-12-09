
#include "crc16.h"

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
#define ENCODE
//#define DECODE

// ----- Choose one of the CRC-16. -----
//#define XMODEM
//#define IBM3740
//#define GSM  // Has a Residue
//#define GENIBUS  // Has a Residue
//#define KERMIT
//#define MCRF4XX
#define IBMSDLC  // Has a Residue

// Helper Function
Crc* get_crc(crc_type type)
{
    Crc *crc = nullptr;

    switch (type)
    {
    case crc_type::e_XMODEM:
        crc = new Crc16(0x0000, false, false, 0x1021, 0x0000);
        break;
    case crc_type::e_IBM3740:
        crc = new Crc16(0xFFFF, false, false, 0x1021, 0x0000);
        break;
    case crc_type::e_GSM:
        crc = new Crc16(0x0000, false, false, 0x1021, 0xFFFF, 0x1D0F);
        break;
    case crc_type::e_GENIBUS:
        crc = new Crc16(0xFFFF, false, false, 0x1021, 0xFFFF, 0x1D0F);
        break;
    case crc_type::e_KERMIT:
        crc = new Crc16(0x0000, true, true, 0x1021, 0x0000);
        break;
    case crc_type::e_MCRF4XX:
        crc = new Crc16(0xFFFF, true, true, 0x1021, 0x0000);
        break;
    case crc_type::e_IBMSDLC:
        crc = new Crc16(0xFFFF, true, true, 0x1021, 0xFFFF, 0xF0B8);
        break;

    default:
        crc = new Crc16(0x0000, false, false, 0x1021, 0x0000);  // XMODEM
        break;
    }
    
    return crc;
}

int main(void)
{
#ifdef XMODEM
#ifdef MSG_LEN_ONE
    uint16_t EXPECTED_CRC = 0x2672;
#endif
#ifdef MSG_LEN_NINE
    uint16_t EXPECTED_CRC = 0x31C3;
#endif

    uint16_t EXPECTED_REMAINDER = 0x0000;
#endif

#ifdef IBM3740
#ifdef MSG_LEN_ONE
    uint16_t EXPECTED_CRC = 0xC782;
#endif
#ifdef MSG_LEN_NINE
    uint16_t EXPECTED_CRC = 0x29B1;
#endif

    uint16_t EXPECTED_REMAINDER = 0x0000;
#endif

#ifdef GSM  
#ifdef MSG_LEN_ONE
    uint16_t EXPECTED_CRC = 0xD98D;
#endif
#ifdef MSG_LEN_NINE
    uint16_t EXPECTED_CRC = 0xCE3C;
#endif
    uint16_t EXPECTED_REMAINDER = 0x1D0F;
#endif

#ifdef GENIBUS  
#ifdef MSG_LEN_ONE
    uint16_t EXPECTED_CRC = 0x387D;
#endif
#ifdef MSG_LEN_NINE
    uint16_t EXPECTED_CRC = 0xD64E;
#endif
    uint16_t EXPECTED_REMAINDER = 0x1D0F;

#endif

#ifdef KERMIT
#ifdef MSG_LEN_ONE
    uint16_t EXPECTED_CRC = 0x200A;
#endif
#ifdef MSG_LEN_NINE
    uint16_t EXPECTED_CRC = 0x2189;
#endif

    uint16_t EXPECTED_REMAINDER = 0x0000;
#endif

#ifdef MCRF4XX
#ifdef MSG_LEN_ONE
    uint16_t EXPECTED_CRC = 0x2F8D;
#endif
#ifdef MSG_LEN_NINE
    uint16_t EXPECTED_CRC = 0x6F91;
#endif
    
    uint16_t EXPECTED_REMAINDER = 0x0000;
#endif

#ifdef IBMSDLC
#ifdef MSG_LEN_ONE
    uint16_t EXPECTED_CRC = 0xD072;
#endif
#ifdef MSG_LEN_NINE
    uint16_t EXPECTED_CRC = 0x906E;
#endif

    uint16_t EXPECTED_REMAINDER = 0xF0B8;
#endif

    printf("Test:\n");

    Crc *ptr = get_crc(crc_type::e_IBMSDLC);

    uint16_t temp_crc = ptr->get_init(); // init
    uint16_t temp_crc2 = ptr->get_init(); // init
    uint16_t crc_result = 0x0000; // init
    uint16_t crc_result2 = 0x0000; // init


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
        //temp_crc = crc.checksum(data_buffer[i], temp_crc, MSG_SIZE);
        temp_crc = ptr->encode(data_buffer[i], temp_crc, MSG_SIZE);
        //printf("%d: %x\n", i, temp_crc);
    }
    //crc_result = crc.process_final_crc(temp_crc);
    crc_result = temp_crc;
    printf("Crc Result: %x\n", crc_result); 

    assert((crc_result == EXPECTED_CRC && "Incorrect CRC calculated"));

    printf("END -------------------------------\n");
#endif


#ifdef DECODE

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

    for (int i = 0; i < MSG_SIZE+2; i++)
    {
        temp_crc2 = ptr->decode(data_buffer2[i], temp_crc2, MSG_SIZE+2);
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