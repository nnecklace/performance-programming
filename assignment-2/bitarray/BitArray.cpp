#include "BitArray.h"

BitArray::BitArray(ull n) : size(n/64+1), sum_size(n/128+1)
{
    sequence = new ull[size];
    sum_table = new ull[sum_size];
    std::fill(sequence, sequence+size, 0);
    std::fill(sum_table, sum_table+sum_size, 0);
}

BitArray::~BitArray()
{
    delete []sequence;
    delete []sum_table;
}

ull BitArray::get(ull nth) const
{
    return sequence[nth>>6]>>(nth&63) & 1;
}

void BitArray::set(ull nth, ull i)
{
    sequence[nth>>6] |= i<<(nth&63);
}

ull BitArray::sum(const ull nth) const
{
    if (nth < 64) {
        return __builtin_popcountl(sequence[0]<<(63-nth));
    }

    // ull incase we want to search for the sum up to some insane number, like MAX_INT
    ull block = nth>>6;
    return sum_table[block-1]+__builtin_popcountl(sequence[block]<<(63-(nth&63)));
}

void BitArray::compact()
{
    sum_table[0] = __builtin_popcountl(sequence[0]);
    for (size_t i = 1; i < size; ++i) {
        sum_table[i] = __builtin_popcountl(sequence[i])+sum_table[i-1];
    }
}
