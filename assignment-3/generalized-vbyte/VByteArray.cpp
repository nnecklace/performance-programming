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
    uint8_t bits_read = 0;
    uint8_t bits_left = BLOCK_SIZE;
    uint8_t chunks = BLOCK_SIZE/(width+1);
    uint8_t rem = BLOCK_SIZE%(width+1);
    uint32_t stop = 1<<(width+1);
    uint32_t block = 0;
    uint32_t current = 0;

    ull value = sequence[block];

    // find value
    while (true) {
        value >>= bits_read;

        bits_left = BLOCK_SIZE-bits_read;
        chunks = bits_left/(width+1);
        rem = bits_left%(width+1);
        bits_read = 0;
        bool found = false;

        for (int i = 1; i <= chunks; ++i) {
            if (current == nth-1) {
                found = true;
                break;
            }
            if ((value & stop) != 0) current++;
            value >>= width+1;
            bits_left -= width+1;
        }

        if (found || current == nth-1) break;

        // handle remainder
        if (rem > 0) {
            ull temp = value>>rem;
            uint32_t overflow_mask = (1<<(width-rem+1))-1;
            temp |= (sequence[block+1] & overflow_mask)<<(width-rem+rem);
            // bits read from next block
            bits_read = width-rem+rem;
            if ((temp & stop) != 0) current++;
        }

        value = sequence[++block];
    }

    // start decoding
    ull decoded = 0;
    ull mask = (1<<(width+1))-1;
    ull shift_value = 0;

    if (width+1 > bits_left) {
        uint32_t remaining_bit_mask = (1<<(bits_left+1))-1;
        decoded |= (value & remaining_bit_mask);
        value = sequence[block+1];
        ull remaining = width-bits_left;
        uint32_t overflow_mask = 1<<((1<<(remaining+1))-1);
        decoded |= (value & overflow_mask)<<(bits_left+1);
        value >>= remaining+1;
    }

    while (true) {
        bool found = false;
        ull next = value>>(width+1); 
        if ((next & stop) == stop) {
            next /= (1<<(width+1));
            found = true;
        }
        next &= mask;
        decoded |= next<<(shift_value-1);
        shift_value += width;
        if (found) break;
    }

    return decoded;
}

int VByteArray::getSizeInBytes() const
{
    return size*sizeof(sequence)+sizeof(width)+sizeof(BLOCK_SIZE)+sizeof(padding_size)+sizeof(size);
}