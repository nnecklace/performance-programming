#include <vector>
#include <algorithm>
typedef unsigned long long ull;

class VByteArray 
{
private:
    const uint8_t width;
    const uint8_t BLOCK_SIZE = 64;
    const ull padding_size;
    ull size;
    ull* sequence;
    ull encode(ull input) const 
    {
        ull base = 0;
        ull encoded = 0;
        ull divider = 1<<width;
        while (true) {
            encoded |= ((input % divider)<<base);
            if (input < divider) {
                encoded |= divider<<base;
                break;
            }
            input /= divider;
            base += width+1;
        }

        return encoded;
    }
    ull decode(const ull nth) const 
    {
        return 0;
    }
    void increaseSize() 
    {
        ull* temp = new ull[2*size];
        std::copy(sequence, sequence+size, temp);
        delete[] sequence;
        sequence = temp;
        size *= 2;
    }
public:
    VByteArray(uint8_t k, ull* array, int length);
    VByteArray(uint8_t k, std::vector<ull>& array);
    ~VByteArray();
    // linear search until we reach nth-1 stop bits
    ull accessScan(const ull nth) const;
    int getSizeInBytes() const;
};