#include "../hdr/crc16.h"

Crc16::Crc16(uint16_t init, uint16_t poly, uint16_t xor_out)
{
    _init = init;
    _poly = poly;
    _xor_out = xor_out;
}

Crc16::~Crc16()
{
}

uint16_t Crc16::_prepare_data(uint8_t *input_data, uint16_t crc)
{
    uint8_t byte_buffer = *input_data;
    uint16_t temp_crc = crc;
    uint16_t two_byte_buffer =  (uint16_t)byte_buffer;
    temp_crc = temp_crc ^ (two_byte_buffer << 8);
    temp_crc = temp_crc ^ (two_byte_buffer << 8);
    return ;
}

uint16_t Crc16::_binary_division(uint16_t data_byte)
{
    /*
        Description: 
        Evaluate if a bit is a 1 or 0. 
        If its a 1 division is possible, the data is shifted by 1 and the rest of the bits are Xored with the Polynomial.
        If its a 0 division is not possible, the data is shifted by 1 so the next bit can be evaluated.
    */
    //printf("BEGIN: evaluate_crc_from_msb ------------------------------------\n"); // DBG
    uint16_t crc_data_byte = data_byte;
    uint16_t check_msb = crc_data_byte & _TOP_BIT_MASK;
    if (check_msb)
    {
        crc_data_byte = crc_data_byte << 1;
        crc_data_byte = crc_data_byte ^ _poly; 
    }
    else
    {
        crc_data_byte = crc_data_byte << 1;
    }
    //printf("END: evaluate_crc_from_msb ------------------------------------\n\n"); // DBG

    return crc_data_byte;   
}

uint16_t Crc16::checksum(uint16_t data_byte)
{
    /*
        Description: Evaluates each bit of the byte provide.
    */
    //printf("\nBEGIN: calc_crc_from_one_byte ------------------------------------\n"); // DBG
    uint16_t temp = data_byte;
    uint16_t temp_crc = 0x0000; // Initialise temporary variable
    
    for(unsigned char bit = 0; bit < 8; bit++)
    {
        //printf("Bit %d:\n", bit); // DBG
        temp = _binary_division(temp);
    }
    temp_crc = temp;
    // printf("END: calc_crc_from_one_byte ------------------------------------\n\n"); // DBG
    return temp_crc;
}

uint16_t Crc16::multi_byte_checksum(uint8_t *data, int nbytes)
{
    /*
        Description: Process each byte of the multi-byte data
    */

    //printf("\n------------------------------------ BEGIN: crc16_multi_byte ------------------------------------\n"); // DBG
    uint16_t crc_result = 0x0000;
    uint16_t temp_crc = _init;  // Init CRC value (0x0000 or 0xFFFF depending on CRC)
    //uint16_t xor_out = _xor_out; // XorOut value (0x0000 or 0xFFFF depending on CRC)

    for(int byte = 0; byte < nbytes; byte++)
    {
        uint16_t data_16 =  (uint16_t)data[byte];
        temp_crc = temp_crc ^ (data_16 << 8);

        temp_crc = checksum(temp_crc);
    }
    crc_result = temp_crc ^ _xor_out;
    //printf("------------------------------------ END: crc16_multi_byte ------------------------------------\n\n");  // DBG

    return crc_result;
}

bool Crc16::verify(uint16_t crc)
{
    /*
        Function Description: Returns true if data has not been corrupted.
    */
    return (crc == 0)? true: false;
}