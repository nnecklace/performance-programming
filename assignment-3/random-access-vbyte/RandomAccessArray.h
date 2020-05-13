#include <cstdint>
#include <vector>
#include "PackedInteger.h"
#include "BitArray.h"
#include "shared.h"

template<unsigned int T> class RandomAccessArray 
{
static_assert(T <= 64, "Integer width cannot be larger than 64");
private:
    uint8_t max_layer = 0;
    const uint8_t width = T;
    std::vector<std::pair<BitArray*, PackedIntegerArray*>> layers;
public:
    ~RandomAccessArray();
    void encodeAndPush(ull number);
    uint8_t getMaxLayerCount() { return max_layer; }
    std::vector<std::pair<BitArray*, PackedIntegerArray*>> getLayers();
};