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
    ull decode(const ull position) const 
    {
        ull decoded = 0;

        ull block_number = position/BLOCK_SIZE;
        ull block = sequence[block_number];
        ull stop_bit = 1<<width;

        uint8_t padding = width+1;
        uint8_t overflow_bits = position%BLOCK_SIZE;
        uint8_t shift_value = 0;

        ull chunk_mask = (1<<width)-1;

        while (true) {
            block >>= overflow_bits;
            uint8_t bits_in_block = BLOCK_SIZE-overflow_bits;
            uint8_t chunks_in_block = bits_in_block/padding;
            uint8_t remainder_bits = bits_in_block%padding;

            ull remainder_mask = (1<<remainder_bits)-1;
            ull overflow_mask = (1<<(padding-remainder_bits))-1;

            overflow_bits = padding-remainder_bits;

            bool complete = false;

            for (int i = 1; i <= chunks_in_block; ++i) {
                if (complete) break;
                if ((block & stop_bit) != 0) complete = true;
                decoded |= (block & chunk_mask)<<shift_value;
                shift_value += width;
                block >>= padding;
                bits_in_block -= padding;
            }

            if (complete) break;

            if (remainder_bits > 0) {
                ull temp = (block & remainder_mask);
                temp |= (sequence[block_number+1] & overflow_mask)<<remainder_bits;
                decoded |= temp<<shift_value;
                shift_value += width;
                if ((temp & stop_bit) != 0) break;
            }

            block = sequence[++block_number];
        }

        return decoded;
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