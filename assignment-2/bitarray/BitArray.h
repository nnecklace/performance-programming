#include <cstddef>
#include <cstdint>

typedef unsigned long long ull;

// TODO: Create template for bit sizes
class BitArray {
    private:
        ull* sequence;
        uint8_t* sum_table;
        const size_t size;
    public:
        BitArray(ull n);
        ~BitArray();
        ull get(const ull nth) const;
        void set(ull nth, ull i);
        ull check(const ull block);
        ull sum (const ull i) const;
        void compact();
};

BitArray::BitArray(ull n) : size(((n+63)/64)+1)
{
    sequence = new ull[size];
}

BitArray::~BitArray()
{
    delete []sequence;
    delete []sum_table;
}

ull BitArray::get(ull nth) const
{
    return ((sequence[nth/64]>>(nth%64)) & 1);
}

void BitArray::set(ull nth, ull i)
{
    sequence[nth/64] |= (i<<(nth%64));
}

ull BitArray::sum(const ull nth) const
{
    return sum_table[(nth/64)]+__builtin_popcountl(sequence[nth/64]<<(64-(nth%64)));
}

void BitArray::compact()
{
    sum_table = new uint8_t[size];
    sum_table[0] = __builtin_popcountl(sequence[0]);
    for (int i = 1; i < size; ++i) {
        sum_table[i] = __builtin_popcountl(sequence[i])+sum_table[i-1];
    }
}

// TODO: Remove! Only used for debugging
ull BitArray::check(ull block)
{
    return sequence[block];
}