// Header file for input output functions
#include <stdio.h>
#include <stdint.h>
#include <string.h>

uint16_t crc16(uint8_t data, int nbytes)
{
    /* Old Design*/

    uint16_t crc = 0;
    //uint16_t data_16 =  (uint16_t)data;
    uint16_t check_msb = 0;

    for(int byte = 0; byte < nbytes; byte++)
    {
        printf("Round: %d:\n", byte);
        printf("1: %x\n", crc);
        printf("Byte: %x\n", data);
        uint16_t data_16 =  (uint16_t)data;
        printf("1.5: %x\n", (data_16 << 8)); // 0x3100 (11000100000000)

        crc = crc ^ (data_16 << 8);

        printf("2: %x\n", crc); // 0x3100 (11000100000000)

        for(unsigned char bit = 0; bit < 8; bit++)
        {
            printf("Bit %d:\n", bit);
            check_msb = crc & 0x8000;
            printf("3: %#x\n", check_msb);
            if (check_msb)
            {
                crc = crc << 1;
                printf("4: %x\n", crc);
                crc = crc ^ 0x1021; //
                printf("4.5: %x\n", crc);
            }
            else
            {
                printf("5: %x\n", crc << 1); // 0110001000000000 (0x6200)
                crc = crc << 1;
            }
            
        }
    }
    return crc;
}

void print_msg(uint8_t *data, int length)  // DEBUG 
{
    printf("Message: ");
    int msg_len = length;
    for(int byte = 0; byte < msg_len; byte++)
    {
        printf("%x ", data[byte]);
    }
    printf("\n");
}

uint16_t evaluate_crc_from_msb(uint16_t crc)
{
    //printf("BEGIN: evaluate_crc_from_msb ------------------------------------\n"); // DBG
    uint16_t crc_data_byte = crc;
    uint16_t check_msb = crc_data_byte & 0x8000;
    //printf("MSB: %#x\n", check_msb); // DBG
    if (check_msb)
    {
        crc_data_byte = crc_data_byte << 1;
        //printf("1: %x\n", crc_data_byte); // DBG
        crc_data_byte = crc_data_byte ^ 0x1021; 
        //printf("1.2: %x\n", crc_data_byte); // DBG
    }
    else
    {
        //printf("2: %x\n", crc_data_byte << 1); // DBG
        crc_data_byte = crc_data_byte << 1;
    }
    //printf("END: evaluate_crc_from_msb ------------------------------------\n\n"); // DBG

    return crc_data_byte;   
}

uint16_t calc_crc_from_one_byte(uint16_t init_crc)
{
    //printf("\nBEGIN: calc_crc_from_one_byte ------------------------------------\n"); // DBG
    uint16_t data_byte = init_crc;
    uint16_t crc = 0x0000;
    
    for(unsigned char bit = 0; bit < 8; bit++)
    {
        //printf("Bit %d:\n", bit); // DBG
        data_byte = evaluate_crc_from_msb(data_byte);
    }

    crc = data_byte;
    // printf("END: calc_crc_from_one_byte ------------------------------------\n\n"); // DBG

    return crc;
}


uint16_t crc16_multi_byte(uint8_t *data, int nbytes)
{
    /* Refactored Design */

    printf("\n------------------------------------ BEGIN: crc16_multi_byte ------------------------------------\n");
    uint16_t crc_result = 0x0000;
    uint16_t temp_crc = 0x0000; // Init CRC value (0x0000 or 0xFFFF depending on CRC)
    uint16_t xor_out = 0x0000; // XorOut value (0x0000 or 0xFFFF depending on CRC)

    for(int byte = 0; byte < nbytes; byte++)
    {
        //printf("1:"); // DBG
        //printf("Byte Number: %d:\n", byte); // DBG
        //printf("Current CRC value: %x\n", temp_crc); // DBG
        //printf("Byte Value: %x\n", data[byte]); // DBG

        uint16_t data_16 =  (uint16_t)data[byte];

        //printf("1.5: %x\n", (data_16 << 8)); // DBG

        temp_crc = temp_crc ^ (data_16 << 8);

        //printf("2: %x\n", temp_crc); // DBG

        temp_crc = calc_crc_from_one_byte(temp_crc);
    }
    crc_result = temp_crc ^ xor_out;
    printf("------------------------------------ END: crc16_multi_byte ------------------------------------\n\n");

    return crc_result;
}

int crc_data_check(uint16_t crc)
{
    /*
        Function Description: Returns True if data has not been corrupted.
    */
    return (crc == 0)? 1: 0;
}

// USER TODO: Uncomment the desired operation to perform. Then uncomment a byte length
#define CALC_CRC
//#define CHECK_UCORRPT_MSG
//#define CHECK_CORRPT_MSG

// USER TODO: Select the byte msg length
//#define ONE_BYTE_MSG
#define FOUR_BYTE_MSG


int main() 
{

#ifdef CALC_CRC
    printf("Calculate Message CRC: ------------------------------------\n");
    #ifdef ONE_BYTE_MSG
        int message_len = 1;
    #else 
        int message_len = 4; // FOUR_BYTE_MSG
    #endif
#endif

#ifndef CALC_CRC // Checking msg and crc
    // msg length + crc
    #ifdef ONE_BYTE_MSG
        int message_len = 3; 
    #else 
        int message_len = 6; // FOUR_BYTE_MSG
    #endif
#endif


#ifdef CHECK_UCORRPT_MSG
    printf("Check Uncorrupt Message CRC: ------------------------------------\n");
#endif

#ifdef CHECK_CORRPT_MSG
    printf("Check Corrupt Message CRC: ------------------------------------\n");
#endif

#ifdef CALC_CRC
    // Calculate CRC of message //
#ifdef ONE_BYTE_MSG
    /*
       - message = 00110001 (0x31) (ASCII character '1')
       - Expected CRC = 0x2672 (0010_0110_0111_0010)
    */
    uint8_t message[1] = {0x31}; 
#endif

#ifdef FOUR_BYTE_MSG
    /*
       - message = 0x31, 0x32, 0x33, 0x34 (ASCII character '1', '2', '3', '4')
       - Expected CRC = 0xd789
    */
    uint8_t message[4] = {0x31, 0x32, 0x33, 0x34};
#endif

#endif

#ifdef CHECK_UCORRPT_MSG
    // Check data has not been corrupted //
    // Non Corrupted Data with Crc//

#ifdef ONE_BYTE_MSG
    // 1 byte message not corrupted with it's crc. Should return zero.
    uint8_t message[1+2] = {0x31, 0x26, 0x72}; 
#endif

#ifdef FOUR_BYTE_MSG
    // 4 byte message not corrupted with it's crc. Should return zero.
    uint8_t message[4+2] = {0x31, 0x32, 0x33, 0x34, 0xd7, 0x89}; 
#endif

#endif

#ifdef CHECK_CORRPT_MSG
    // Corrupted Data with Crc //
#ifdef ONE_BYTE_MSG
    // (1)
    /* 1 byte message corrupted 
       - With LSB bit flip (to 0).
       - 0x31 -> 0x30 (0011_0001 -> 0011_0000)
       - Output: 0011_0111_0011_0000 (0x3730) != 0

    */ 
    uint8_t message[1+2] = {0x30, 0x26, 0x72};
#endif

#ifdef FOUR_BYTE_MSG
    // (2)
    /* 4 byte message corrupted 
       - 3rd byte corrupted with 3rd bit flip (0 to 1)
       - 0x33 -> 0x37 (0011_0011 -> 0011_0111)
       - Crc with uncorrupt message is 1101_0111_1000_1001 (0xd789)
       - Output: 1100_1010_1111_0001 (0xcaf1) != 0
    */ 
    uint8_t message[4+2] = {0x31, 0x32, 0x37, 0x34, 0xd7, 0x89}; 
#endif
#endif

    uint16_t result = 0;
    // Poly = 0001000000100001 (0x1021)

    print_msg(message, message_len); //DEBUG

    //result = crc16(y, byte_num);  // Old crc calculating function
    result = crc16_multi_byte(message, message_len);

    printf("Data out: %#x\n", result);

#ifdef CHECK_CORRPT_MSG
    int corrupt_check = crc_data_check(result);
    printf("Corrupt Check: %d\n", corrupt_check);
    printf("Corrupt Check is 1 if the data is uncorrupted and 0 if it is corrupted");
#endif
#ifdef CHECK_UCORRPT_MSG
    int corrupt_check = crc_data_check(result);
    printf("Corrupt Check: %d\n", corrupt_check);
    printf("Corrupt Check is 1 if the data is uncorrupted and 0 if it is corrupted");
#endif
    return 0;
}