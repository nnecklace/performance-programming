#include <algorithm>
#include <iostream>

typedef unsigned long long ull;

class BitArray 
{
private:
    ull* sequence;
    ull* sum_table;
    const size_t size;
    const size_t sum_size;
public:
    BitArray(ull n);
    ~BitArray();
    ull get(const ull nth) const;
    void set(ull nth, ull i);
    ull sum (const ull i) const;
    void compact();
};
