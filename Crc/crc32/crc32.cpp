#include "crc32.h"

Crc32::Crc32(uint32_t init, bool ref_in, bool ref_out, uint32_t poly, uint32_t xor_out, uint32_t residue)
{
    _init = init;
    _ref_in = ref_in;
    _ref_out = ref_out;
    _poly = poly;
    _xor_out = xor_out;
    _residue = residue;
    //printf("Residue: %x\n", _residue);  // DBG
}

uint32_t Crc32::get_init(void)
{
    //printf("Init: %x\n", _init);  // DBG
    return _init;
}

uint32_t Crc32::encode(uint8_t data, uint32_t crc, uint8_t num_bytes)
{
    //printf("\nBEGIN: encode ------------------------------------\n"); // DBG
    uint8_t temp_data = data;
    uint8_t temp_num_bytes = num_bytes; 
    uint32_t temp_crc = crc; // Initialise temporary variable

    uint32_t crc_result = 0x00000000; // Initialise temporary variable

    //printf("Byte Counter: %d\n", _byte_counter); //DBG

    uint32_t temp = _checksum(temp_data, temp_crc);

    _byte_counter++;
    //printf("Crc = %x\n\n", temp);
    if (_byte_counter < temp_num_bytes)
    {
        crc_result = temp;
    }
    else 
    {
        crc_result = _process_encode_result(temp);
        _byte_counter = 0x00;
    }
    //printf("END: encode ------------------------------------\n\n"); // DBG
    return crc_result;
}

uint32_t Crc32::decode(uint8_t data, uint32_t crc, uint8_t num_bytes)
{
    //printf("\nBEGIN: decode ------------------------------------\n"); // DBG
    uint8_t temp_data = data;
    uint8_t temp_num_bytes = num_bytes; 
    uint32_t temp_crc = crc; // Initialise temporary variable

    uint32_t crc_result = 0x00000000; // Initialise temporary variable

    //printf("Byte Counter: %d\n", _byte_counter); //DBG

    uint32_t temp = _checksum(temp_data, temp_crc);

    _byte_counter++;
    //printf("Crc = %x\n\n", temp);
    if (_byte_counter < temp_num_bytes)
    {
        crc_result = temp;
    }
    else 
    {
        crc_result = _process_decode_result(temp);
        _byte_counter = 0x00;
    }
    //printf("END: decode ------------------------------------\n\n"); // DBG
    return crc_result;
}

bool Crc32::verify(uint32_t crc)
{
    /*
        Function Description: Returns true if data has not been corrupted.
    */
    
    return (crc == _residue)? true: false;
}

uint32_t Crc32::_checksum(uint8_t data, uint32_t crc)
{
    /*
        Description: Evaluates each bit of the byte provide.
    */
    //printf("\nBEGIN: _checksum ------------------------------------\n"); // DBG
    uint8_t temp_data = data; 
    uint32_t temp_crc = crc; // Initialise temporary variable

    if (_ref_in)
    {
        temp_data = _reflection_input(temp_data, 8);
    }

    uint32_t temp = _prepare_data(temp_data, temp_crc);
    
    for(unsigned char bit = 0; bit < 8; bit++)
    {
        //printf("Bit %d:\n", bit+1); // DBG
        temp = _binary_division(temp);
    }

    //printf("Checksum = %x\n\n", temp);
    //printf("END: _checksum ------------------------------------\n\n"); // DBG
    return temp;
}

uint8_t Crc32::_reflection_input(uint8_t data, uint8_t nbits)
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
    //printf("Reflect in Data Output: %x\n", reflected_data);
    //printf("END: --------------------- _reflection_input ----------------------\n");
    
    return reflected_data;
}

uint32_t Crc32::_prepare_data(uint8_t data, uint32_t crc)
{
    uint8_t byte_buffer = data;
    uint32_t temp_crc = crc;
    uint32_t two_byte_buffer =  (uint32_t)byte_buffer;
    //printf("init crc: %08x\n", temp_crc);
    //printf("byte_buffer: %x\n", byte_buffer);
    //printf("two_byte_buffer: %08x\n", two_byte_buffer);

    temp_crc = temp_crc ^ (two_byte_buffer << 24);  

    //temp_crc = temp_crc ^ two_byte_buffer; // DBG try and remove the appended zeros ???
    //printf("Shifted two_byte_buffer: %08x\n", two_byte_buffer << 24);
    //printf("crc ^ Shifted two_byte_buffer: %08x\n", temp_crc);
    return temp_crc;
}

uint32_t Crc32::_binary_division(uint32_t data_byte)
{
    /*
        Description: 
        Evaluate if a bit is a 1 or 0. 
        If its a 1 division is possible, the data is shifted by 1 and the rest of the bits are Xored with the Polynomial.
        If its a 0 division is not possible, the data is shifted by 1 so the next bit can be evaluated.
    */
    //printf("BEGIN: _binary_division ------------------------------------\n"); // DBG
    uint32_t crc_data_byte = data_byte;
    uint32_t check_msb = crc_data_byte & _TOP_BIT_MASK;
    //printf("Before: %04x\n", crc_data_byte);
    if (check_msb)
    {
        crc_data_byte = crc_data_byte << 1;
        crc_data_byte = crc_data_byte ^ _poly; 
    }
    else
    {
        crc_data_byte = crc_data_byte << 1;
    }
    //printf("After: %04x\n", crc_data_byte);
    //printf("END: _binary_division ------------------------------------\n\n"); // DBG
    //printf("_binary_division: %04x\n", crc_data_byte);
    return crc_data_byte;   
}

uint32_t Crc32::_process_encode_result(uint32_t crc)
{
    //printf("\nBEGIN: _process_encode_result ------------------------------------\n"); // DBG
    uint32_t temp_crc = crc;

    if (_ref_out)
    {
        temp_crc = _reflection_output(temp_crc, 32); // This was 16
    }
    
    temp_crc = temp_crc ^ _xor_out;
    //printf("Final Encoded Crc with Xor: %x\n", temp_crc);
    //printf("END: _process_encode_result ------------------------------------\n\n"); // DBG
    return temp_crc;
}

uint32_t Crc32::_process_decode_result(uint32_t crc)
{
    //printf("\nBEGIN: _process_decode_result ------------------------------------\n"); // DBG
    uint32_t temp_crc = crc;

    if (_ref_out)
    {
        temp_crc = _reflection_output(temp_crc, 32);  // This was 16
    }

    if (_xor_out == 0xFFFFFFFF) 
    {
        temp_crc = temp_crc;
        //printf("Final Decoded Crc without Xor: %x\n", temp_crc);
    }
    else
    {
        temp_crc = temp_crc ^ _xor_out;
        //printf("Final Decoded Crc with Xor: %x\n", temp_crc);
    }
    
    //printf("Final Decoded Crc: %x\n", temp_crc);
    //printf("END: _process_decode_result ------------------------------------\n\n"); // DBG
    return temp_crc; // ^ _xor_out;
}

uint32_t Crc32::_reflection_output(uint32_t data, uint8_t nbits)
{
    uint32_t reflected_data = 0x00000000;
    uint32_t temp_data = data;
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
    //printf("Refout Output: %x\n", reflected_data);  //DBG
    //printf("END: ---------------------_reflection_output ----------------------\n");
    return reflected_data;
}


Crc32::~Crc32()
{
}