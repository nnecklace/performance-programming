#include "PackedInteger.h"

PackedIntegerArray::PackedIntegerArray(ull n, uint8_t k) : padding_size(k)
{
    ull size = n*padding_size/BLOCK_SIZE+1;
    sequence = new ull[size];
    std::fill(sequence, sequence+size, 0);
}

PackedIntegerArray::~PackedIntegerArray() 
{
    delete []sequence;
}

ull PackedIntegerArray::get(ull nth) const
{
    ull i = 1;
    ull start = padding_size*nth;
    ull end = start+padding_size;
    ull modulus_value = BLOCK_SIZE-1;
    ull max_value = padding_size == 64 ? ~(i-1) : (i<<padding_size)-1;

    // find start of value might be whole value
    ull value_a = max_value & (sequence[start/BLOCK_SIZE]>>(start & modulus_value));

    // start and end are not in same block
    if ((start & modulus_value) > (end & modulus_value)) {
        ull value_b = sequence[end/BLOCK_SIZE] & ((i<<(end & modulus_value))-1);
        value_a |= value_b<<(BLOCK_SIZE-(start & modulus_value));
    }

    return value_a;
}

void PackedIntegerArray::setNext(ull i) 
{
    ull start = padding_size*next;
    ull block = start/BLOCK_SIZE;
    ull pos_in_block = start & (BLOCK_SIZE - 1);
    sequence[block] |= i<<(pos_in_block);
    if (pos_in_block+padding_size > BLOCK_SIZE) {
        ull bits_left = BLOCK_SIZE-pos_in_block;
        i >>= bits_left;
        sequence[block+1] |= i;
    }
    next++;
}
