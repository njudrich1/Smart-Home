#ifndef CRCT_HPP
#define CRCT_HPP

#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* #include "../crc.h" */


template <class T> 
class CrcT
{
public:
    CrcT(T init, bool ref_in, bool ref_out, T poly, T xor_out, T residue=0);
    T get_init(void);
    T encode(uint8_t data, T crc,  uint8_t num_byte);
    T decode(uint8_t data, T crc,  uint8_t num_byte);
    bool verify(T crc);
    ~CrcT();

private:
    T _init;
    bool _ref_in;
    bool _ref_out;
    T _poly;
    T _xor_out; // If this is 0xFFFF, the CRC algorithm will produce a residue, when decoder, instead of a zero value.
    T _residue;

    uint16_t _byte_counter = 0x0000;

    //const T _TOP_BIT_MASK = 0x80000000;  // TODO: CANT BE A CONSTANT ANYMORE??? 
    T _TOP_BIT_MASK;
    T _LOW; 
    T _HIGH;
    int _BIT_SHIFT;
    int _REFLECT_BITS;

    T _checksum(uint8_t data, T crc); 
    uint8_t _reflection_input(uint8_t data, uint8_t nbits);
    T _prepare_data(uint8_t data, T crc);
    T _binary_division(T crc);
    T _process_encode_result(T crc);
    T _process_decode_result(T crc);
    T _reflection_output(T data, uint8_t nbits);
};


template <class T> 
CrcT<T>::CrcT(T init, bool ref_in, bool ref_out, T poly, T xor_out, T residue)
{
    _init = init;
    _ref_in = ref_in;
    _ref_out = ref_out;
    _poly = poly;
    _xor_out = xor_out;
    _residue = residue;
    //printf("Residue: %x\n", _residue);  // DBG

    switch (sizeof(T))
    {
    case 2:
        _TOP_BIT_MASK = 0x8000;
        _LOW = 0x0000;
        _HIGH = 0xFFFF;
        _BIT_SHIFT = 8;
        _REFLECT_BITS = 16;
        break;
    case 4:
        _TOP_BIT_MASK = 0x80000000;
        _LOW = 0x00000000;
        _HIGH = 0xFFFFFFFF;
        _BIT_SHIFT = 24;
        _REFLECT_BITS = 32;
        break;
    }
}

template <class T> 
T CrcT<T>::get_init(void)
{
    //printf("Init: %x\n", _init);  // DBG
    return _init;
}

template <class T> 
T CrcT<T>::encode(uint8_t data, T crc, uint8_t num_bytes)
{
    //printf("\nBEGIN: encode ------------------------------------\n"); // DBG
    uint8_t temp_data = data;
    uint8_t temp_num_bytes = num_bytes; 
    T temp_crc = crc; // Initialise temporary variable

    //T crc_result = 0x00000000; // Initialise temporary variable (TODO: NEED TO UPDATE CANT BE FIXED??????????)
    T crc_result = _LOW; // Initialise temporary variable (TODO: NEED TO UPDATE CANT BE FIXED??????????)

    //printf("Byte Counter: %d\n", _byte_counter); //DBG

    T temp = _checksum(temp_data, temp_crc);

    _byte_counter++;
    //printf("Crc = %x\n\n", temp);
    if (_byte_counter < temp_num_bytes)
    {
        crc_result = temp;
    }
    else 
    {
        crc_result = _process_encode_result(temp);
        _byte_counter = 0x0000;
    }
    //printf("END: encode ------------------------------------\n\n"); // DBG
    return crc_result;
}

template <class T> 
T CrcT<T>::decode(uint8_t data, T crc, uint8_t num_bytes)
{
    //printf("\nBEGIN: decode ------------------------------------\n"); // DBG
    uint8_t temp_data = data;
    uint8_t temp_num_bytes = num_bytes; 
    T temp_crc = crc; // Initialise temporary variable

    //T crc_result = 0x00000000; // Initialise temporary variable (TODO: NEED TO UPDATE CANT BE FIXED??????????)
    T crc_result = _LOW; // Initialise temporary variable (TODO: NEED TO UPDATE CANT BE FIXED??????????)

    //printf("Byte Counter: %d\n", _byte_counter); //DBG

    T temp = _checksum(temp_data, temp_crc);

    _byte_counter++;
    //printf("Crc = %x\n\n", temp);
    if (_byte_counter < temp_num_bytes)
    {
        crc_result = temp;
    }
    else 
    {
        crc_result = _process_decode_result(temp);
        _byte_counter = 0x0000;
    }
    //printf("END: decode ------------------------------------\n\n"); // DBG
    return crc_result;
}

template <class T> 
bool CrcT<T>::verify(T crc)
{
    /*
        Function Description: Returns true if data has not been corrupted.
    */
    
    return (crc == _residue)? true: false;
}

template <class T> 
T CrcT<T>::_checksum(uint8_t data, T crc)
{
    /*
        Description: Evaluates each bit of the byte provide.
    */
    //printf("\nBEGIN: _checksum ------------------------------------\n"); // DBG
    uint8_t temp_data = data; 
    T temp_crc = crc; // Initialise temporary variable

    if (_ref_in)
    {
        temp_data = _reflection_input(temp_data, 8);
    }

    T temp = _prepare_data(temp_data, temp_crc);
    
    for(unsigned char bit = 0; bit < 8; bit++)
    {
        //printf("Bit %d:\n", bit+1); // DBG
        temp = _binary_division(temp);
    }

    //printf("Checksum = %x\n\n", temp);
    //printf("END: _checksum ------------------------------------\n\n"); // DBG
    return temp;
}

template <class T> 
uint8_t CrcT<T>::_reflection_input(uint8_t data, uint8_t nbits)
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

template <class T> 
T CrcT<T>::_prepare_data(uint8_t data, T crc)
{
    uint8_t byte_buffer = data;
    T temp_crc = crc;
    T two_byte_buffer =  (T)byte_buffer;
    //printf("init crc: %08x\n", temp_crc);
    //printf("byte_buffer: %x\n", byte_buffer);
    //printf("two_byte_buffer: %08x\n", two_byte_buffer);

    //temp_crc = temp_crc ^ (two_byte_buffer << 24);  // TODO: NEED TO UPDATE CANT BE FIXED ????? 24
    temp_crc = temp_crc ^ (two_byte_buffer << _BIT_SHIFT);  // TODO: NEED TO UPDATE CANT BE FIXED ????? 24

    //temp_crc = temp_crc ^ two_byte_buffer; // DBG try and remove the appended zeros ???
    //printf("Shifted two_byte_buffer: %08x\n", two_byte_buffer << 24);
    //printf("crc ^ Shifted two_byte_buffer: %08x\n", temp_crc);
    return temp_crc;
}

template <class T> 
T CrcT<T>::_binary_division(T data_byte)
{
    /*
        Description: 
        Evaluate if a bit is a 1 or 0. 
        If its a 1 division is possible, the data is shifted by 1 and the rest of the bits are Xored with the Polynomial.
        If its a 0 division is not possible, the data is shifted by 1 so the next bit can be evaluated.
    */
    //printf("BEGIN: _binary_division ------------------------------------\n"); // DBG
    T crc_data_byte = data_byte;
    T check_msb = crc_data_byte & _TOP_BIT_MASK;
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

template <class T> 
T CrcT<T>::_process_encode_result(T crc)
{
    //printf("\nBEGIN: _process_encode_result ------------------------------------\n"); // DBG
    T temp_crc = crc;

    if (_ref_out)
    {
        //temp_crc = _reflection_output(temp_crc, 32); // This was 16.  (TODO: NEED TO UPDATE CANT BE FIXED??????????)
        temp_crc = _reflection_output(temp_crc, _REFLECT_BITS); // This was 16.  (TODO: NEED TO UPDATE CANT BE FIXED??????????)
    }
    
    temp_crc = temp_crc ^ _xor_out;
    //printf("Final Encoded Crc with Xor: %x\n", temp_crc);
    //printf("END: _process_encode_result ------------------------------------\n\n"); // DBG
    return temp_crc;
}

template <class T> 
T CrcT<T>::_process_decode_result(T crc)
{
    //printf("\nBEGIN: _process_decode_result ------------------------------------\n"); // DBG
    T temp_crc = crc;

    if (_ref_out)
    {
        //temp_crc = _reflection_output(temp_crc, 32);  // This was 16. (TODO: NEED TO UPDATE CANT BE FIXED??????????)
        temp_crc = _reflection_output(temp_crc, _REFLECT_BITS);  // This was 16. (TODO: NEED TO UPDATE CANT BE FIXED??????????)
    }

//    if (_xor_out == 0xFFFFFFFF)  // (TODO: NEED TO UPDATE CANT BE FIXED??????????)
    if (_xor_out == _HIGH)  // (TODO: NEED TO UPDATE CANT BE FIXED??????????)
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

template <class T> 
T CrcT<T>::_reflection_output(T data, uint8_t nbits)
{
    //T reflected_data = 0x00000000;  // (TODO: NEED TO UPDATE CANT BE FIXED??????????)
    T reflected_data = _LOW;  // (TODO: NEED TO UPDATE CANT BE FIXED??????????)
    T temp_data = data;
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


template <class T> 
CrcT<T>::~CrcT()
{
}

#endif