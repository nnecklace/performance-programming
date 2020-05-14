#include <vector>
#include "PackedInteger.h"
#include "BitArray.h"
#include "shared.h"

template<unsigned int T> class RandomAccessArray 
{
static_assert(T <= 64, "Integer width cannot be larger than 63");
private:
    const uint8_t width = T;
    std::vector<std::pair<BitArray*, PackedIntegerArray*>> layers;
public:
    ~RandomAccessArray();
    void encodeAndPush(ull number);
    ull accessScan(ull nth);
    void compact();
};