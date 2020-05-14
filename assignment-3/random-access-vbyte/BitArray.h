#include <algorithm>
#include "shared.h"

class BitArray 
{
private:
    ull* sequence;
    ull* sum_table;
    ull next = 0;
    const size_t size;
    const size_t sum_size;
public:
    BitArray(ull n);
    ~BitArray();
    ull get(const ull nth) const;
    void setNext(ull i);
    ull sum (const ull i) const;
    void compact();
};