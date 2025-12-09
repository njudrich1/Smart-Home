#include "../hdr/crc16.h"

Crc16::Crc16(uint16_t init, bool ref_in, bool ref_out, uint16_t poly, uint16_t xor_out)
{
    _init = init;
    _ref_in = ref_in;
    _ref_out = ref_out;
    _poly = poly;
    _xor_out = xor_out;
}

uint16_t Crc16::checksum(uint8_t data, uint16_t crc, uint8_t num_bytes)
{
    /*
        Description: Evaluates each bit of the byte provide.
    */
    //printf("\nBEGIN: calc_crc_from_one_byte ------------------------------------\n"); // DBG
    uint8_t temp_data = data; 
    uint8_t temp_num_bytes = num_bytes; 

    if (_ref_in)
    {
        temp_data = _reflection_input(temp_data, 8);
    }

    uint16_t temp = _prepare_data(temp_data, crc);
    uint16_t temp_crc = 0x0000; // Initialise temporary variable
    
    for(unsigned char bit = 0; bit < 8; bit++)
    {
        //printf("Bit %d:\n", bit); // DBG
        temp = _binary_division(temp);
    }

    _byte_counter++;
    //printf("Byte Counter: %d\n", _byte_counter);
    if (_byte_counter < temp_num_bytes)
    {
        temp_crc = temp;
    }
    else 
    {
        temp_crc = _process_final_crc(temp);
        _byte_counter = 0x00;
    }
    // printf("END: calc_crc_from_one_byte ------------------------------------\n\n"); // DBG
    return temp_crc;
}

uint16_t Crc16::get_xor_out(void)
{
    return _xor_out;
}

uint16_t Crc16::get_init(void)
{
    return _init;
}

bool Crc16::verify(uint16_t crc)
{
    /*
        Function Description: Returns true if data has not been corrupted.
    */
    return (crc == 0)? true: false;
}


uint16_t Crc16::_prepare_data(uint8_t data, uint16_t crc)
{
    uint8_t byte_buffer = data;
    uint16_t temp_crc = crc;
    uint16_t two_byte_buffer =  (uint16_t)byte_buffer;
    temp_crc = temp_crc ^ (two_byte_buffer << 8);
    return temp_crc;
}

uint8_t Crc16::_reflection_input(uint8_t data, uint8_t nbits)
{
    uint8_t reflected_data = 0x00;
    uint8_t temp_data = data;
    uint8_t temp_nbits = nbits;

    //printf("BEGIN: _reflection_input ----------------------\n");
    //printf("1) data in: %x\n", temp_data);  //DBG

    for (uint8_t i = 0; i < temp_nbits; i++)
    {   
        if ((temp_data & (1 << i)) != 0)
        {
            reflected_data |= (1 << (temp_nbits - 1) - i);
        }
    }

    //printf("END: --------------------- _reflection_input ----------------------\n");

    return reflected_data;
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

uint16_t Crc16::_process_final_crc(uint16_t crc)
{
    uint16_t temp_crc = crc;

    if (_ref_out)
    {
        temp_crc = _reflection_output(temp_crc, 16);
    }
    return temp_crc ^ _xor_out;
}

uint16_t Crc16::_reflection_output(uint16_t data, uint8_t nbits)
{
    uint16_t reflected_data = 0x0000;
    uint16_t temp_data = data;
    uint8_t temp_nbits = nbits;
    
    //printf("BEGIN: _reflection_output ----------------------\n");
    //printf("1) data in: %x\n", temp_data);  //DBG

    for (uint8_t i = 0; i < temp_nbits; i++)
    {   
        //printf("%d.\n", i); 
        if ((temp_data & (1 << i)) != 0)
        {
            reflected_data |= (1 << (temp_nbits - 1) - i);
        }
    }

    //printf("END: ---------------------_reflection_output ----------------------\n");
    return reflected_data;
}

Crc16::~Crc16()
{
}