#include <iostream>

typedef unsigned long long ull;

// TODO: Create template for bit sizes
class BitArray 
{
private:
    ull* sequence;
    ull* sum_table;
    const size_t size;
public:
    BitArray(ull n);
    BitArray(); // FIXME: Can lead to undefined behaviour
    ~BitArray();
    ull get(const ull nth) const;
    void set(ull nth, ull i);
    ull check(const ull block); // TODO: Remove!
    void check(); // TODO: Remove!
    ull sum (const ull i) const;
    void compact();
    friend std::istream& operator>>(std::istream& input, const BitArray& b);
    friend std::ostream& operator<<(std::ostream& output, const BitArray& b);
};

BitArray::BitArray(ull n) : size(((n+63)/64)+1)
{
    unsigned int sum_size = (n+63)/128+1;
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
    return sequence[nth/64]>>(nth&63) & 1;
}

void BitArray::set(ull nth, ull i)
{
    sequence[nth/64] |= i<<(nth&63);
}

ull BitArray::sum(const ull nth) const
{
    if (nth < 64) {
        return __builtin_popcountl(sequence[0]<<(63-nth));
    }

    if (nth >= 64 && nth < 127) {
        return __builtin_popcountl(sequence[0])+__builtin_popcountl(sequence[1]<<(127-nth));
    }

    // ull incase we want to search for the sum up to some insane number, like MAX_INT
    ull block = nth>>7;
    ull section = nth>>6;
    return sum_table[block-1]+__builtin_popcountl(sequence[section]<<(63-(nth&63)));
}

void BitArray::compact()
{
    sum_table[0] = __builtin_popcountl(sequence[0]);
    for (size_t i = 1; i < size/2; ++i) {
        sum_table[i] = __builtin_popcountl(sequence[i+(i-1)])+__builtin_popcountl(sequence[i+i])+sum_table[i-1];
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

void BitArray::check() 
{
    std::cout << "size " << size << std::endl;
    for (size_t i = 0; i < size/2; ++i) {
        std::cout << "Checking [" << i+1 << "] " << (int)sum_table[i] << std::endl;
    }
}