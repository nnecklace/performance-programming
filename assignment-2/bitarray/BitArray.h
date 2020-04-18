#include <iostream>

typedef unsigned long long ull;

// TODO: Create template for bit sizes
class BitArray {
    private:
        ull* sequence;
        uint8_t* sum_table;
        const size_t size;
    public:
        BitArray(ull n);
        BitArray();
        ~BitArray();
        ull get(const ull nth) const;
        void set(ull nth, ull i);
        ull check(const ull block); // TODO: Remove!
        ull sum (const ull i) const;
        void compact();
        friend std::istream& operator>>(std::istream& input, const BitArray& b);
        friend std::ostream& operator<<(std::ostream& output, const BitArray& b);
};

BitArray::BitArray(ull n) : size(((n+63)/64)+1)
{
    sequence = new ull[size];
    sum_table = new uint8_t[size];
    std::fill(sequence, sequence+size, 0);
    std::fill(sum_table, sum_table+size, 0);
}

BitArray::~BitArray()
{
    delete []sequence;
    delete []sum_table;
}

ull BitArray::get(ull nth) const
{
    return sequence[nth/64]>>(nth&63) & 1;
}

void BitArray::set(ull nth, ull i)
{
    sequence[nth/64] |= i<<(nth&63);
}

ull BitArray::sum(const ull nth) const
{
    // incase we try to access illegal bits
    if (nth >= size) {
        return sum_table[size-1];
    }

    if (nth < 63) {
        return __builtin_popcountl(sequence[0]<<(63-nth));
    }

    // ull incase we want to search for the sum up to some insane number, like 69 000 000
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

std::istream& operator>>(std::istream& input, const BitArray& b)
{
    return input;
}

std::ostream& operator<<(std::ostream& output, const BitArray& b)
{
    return output;
}

// TODO: Remove! Only used for debugging
ull BitArray::check(ull block)
{
    return sequence[block];
}