#include <algorithm>
#include "shared.h"

class PackedIntegerArray
{
private:
    ull* sequence;
    const uint8_t padding_size;
    ull next = 0;
    const unsigned char BLOCK_SIZE = 64;
public:
    PackedIntegerArray(ull n, uint8_t k);
    ~PackedIntegerArray();
    ull get(ull nth) const;
    void setNext(ull i);
};
