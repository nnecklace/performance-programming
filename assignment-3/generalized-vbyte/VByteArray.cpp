#include "VByteArray.h"
#include <iostream>
#include <bitset>

VByteArray::VByteArray(uint8_t k, std::vector<ull>& array) : width(k), padding_size(2ULL<<k), size(2)
{
    ull next = 0;
    ull current_size = size;
    sequence = new ull[size];
    std::fill(sequence, sequence+2, 0);

    for (int i = 0; i < array.size(); ++i) {
        ull encoded = this->encode(array[i]);
        ull length = __builtin_clzl(encoded);
        ull position = next%BLOCK_SIZE;

        sequence[next/BLOCK_SIZE] |= encoded<<position;

        if (length+position > BLOCK_SIZE) {
            encoded >>= BLOCK_SIZE-position;
            sequence[next/BLOCK_SIZE+1] |= encoded;
        }

        if (position >= (BLOCK_SIZE-length+next)%BLOCK_SIZE) {
            current_size++; 
            if (current_size > size) increaseSize();
        }

        next += BLOCK_SIZE-length;
    }
}

VByteArray::~VByteArray() 
{
    delete[] sequence;
}

ull VByteArray::accessScan(const ull nth) const 
{
    ull padding = width+1;
    ull stop_bit = 1<<width;
    ull position = 0;
    ull current_nth = 0;
    
    ull block_number = 0;
    ull block = sequence[block_number];
    uint8_t overflow_bits = 0;

    while (true) {
        block >>= overflow_bits;

        uint8_t bits_in_block = BLOCK_SIZE-overflow_bits;
        uint8_t chunks_in_block = bits_in_block/padding;
        uint8_t remainder_bits = bits_in_block%padding;
        
        ull remainder_mask = (1<<remainder_bits)-1;
        ull overflow_mask = (1<<(padding-remainder_bits))-1;

        overflow_bits = padding-remainder_bits;

        bool found = false;

        for (int i = 1; i <= chunks_in_block; ++i) {
            if (current_nth == nth-1) {
                found = true;
                break;
            }
            if ((block & stop_bit) != 0) current_nth++;
            block >>= padding;
            bits_in_block -= padding;
            position += padding;
        }

        if (found || current_nth == nth-1) break;

        if (remainder_bits > 0) {
            ull temp = (block & remainder_mask);
            temp |= (sequence[block_number+1] & overflow_mask)<<remainder_bits;
            if ((temp & stop_bit) != 0) current_nth++;
            position += padding;
        }

        block = sequence[++block_number];
    }

    return this->decode(position);
}

int VByteArray::getSizeInBytes() const
{
    return size*sizeof(sequence)+sizeof(width)+sizeof(BLOCK_SIZE)+sizeof(padding_size)+sizeof(size);
}