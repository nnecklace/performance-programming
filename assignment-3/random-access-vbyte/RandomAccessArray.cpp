#include "RandomAccessArray.h"

template<unsigned int T>
RandomAccessArray<T>::~RandomAccessArray() 
{
   for (int i = 0; i < layers.size(); ++i) {
        std::pair<BitArray*, PackedIntegerArray*> current_layer = layers[i];
        delete current_layer.first;
        delete current_layer.second;
   }
}

template<unsigned int T>
void RandomAccessArray<T>::encodeAndPush(ull number) 
{
    ull base = 0;
    ull encoded = 0;
    ull divider = 1ULL<<width;

    while (true) {
        encoded |= ((number % divider)<<base);
        if (number < divider) {
            encoded |= divider<<base;
            break;
        }

        number /= divider;
        base += width+1;
    }

    uint8_t length = 64 - __builtin_clzl(encoded);
    uint8_t parts = 0;
    ull data_block_mask = (1<<width)-1;
    uint8_t layer = 1;

    while (true) {
        if (length <= 0) break;
        uint8_t part = encoded>>((width+1)*parts);
        std::pair<BitArray*, PackedIntegerArray*> current_layer;

        if (layer <= layers.size()) {
            current_layer = layers[layer-1];
        } else {
            current_layer = {new BitArray(50000000), new PackedIntegerArray(50000000, width)};
        }

        if ((part & divider) != 0) {
            current_layer.first->setNext(1);
        } else {
            current_layer.first->setNext(0);
        }

        current_layer.second->setNext(part & data_block_mask);

        if (layer > layers.size()) {
            layers.emplace_back(current_layer);
        }

        layer++;
        parts++;
        length -= width+1;
    }
    max_layer = std::max(max_layer, layer);
}

template<unsigned int T>
std::vector<std::pair<BitArray*, PackedIntegerArray*>> RandomAccessArray<T>::getLayers() 
{
    return layers;
}

// There is apparently no other way to do this ://
// https://stackoverflow.com/questions/6397657/instantiating-multiple-templates

// There might be a better way: https://stackoverflow.com/questions/25202250/c-template-instantiation-avoiding-long-switches
template class RandomAccessArray<1>;
template class RandomAccessArray<2>;
template class RandomAccessArray<3>;
template class RandomAccessArray<4>;
template class RandomAccessArray<5>;
template class RandomAccessArray<6>;
template class RandomAccessArray<7>;
template class RandomAccessArray<8>;
template class RandomAccessArray<9>;
template class RandomAccessArray<10>;
template class RandomAccessArray<11>;
template class RandomAccessArray<12>;
template class RandomAccessArray<13>;
template class RandomAccessArray<14>;
template class RandomAccessArray<15>;
template class RandomAccessArray<16>;
template class RandomAccessArray<17>;
template class RandomAccessArray<18>;
template class RandomAccessArray<19>;
template class RandomAccessArray<20>;
template class RandomAccessArray<21>;
template class RandomAccessArray<22>;
template class RandomAccessArray<23>;
template class RandomAccessArray<24>;
template class RandomAccessArray<25>;
template class RandomAccessArray<26>;
template class RandomAccessArray<27>;
template class RandomAccessArray<28>;
template class RandomAccessArray<29>;
template class RandomAccessArray<30>;
template class RandomAccessArray<31>;
template class RandomAccessArray<32>;
template class RandomAccessArray<33>;
template class RandomAccessArray<34>;
template class RandomAccessArray<35>;
template class RandomAccessArray<36>;
template class RandomAccessArray<37>;
template class RandomAccessArray<38>;
template class RandomAccessArray<39>;
template class RandomAccessArray<40>;
template class RandomAccessArray<41>;
template class RandomAccessArray<42>;
template class RandomAccessArray<43>;
template class RandomAccessArray<44>;
template class RandomAccessArray<45>;
template class RandomAccessArray<46>;
template class RandomAccessArray<47>;
template class RandomAccessArray<48>;
template class RandomAccessArray<49>;
template class RandomAccessArray<50>;
template class RandomAccessArray<51>;
template class RandomAccessArray<52>;
template class RandomAccessArray<53>;
template class RandomAccessArray<54>;
template class RandomAccessArray<55>;
template class RandomAccessArray<56>;
template class RandomAccessArray<57>;
template class RandomAccessArray<58>;
template class RandomAccessArray<59>;
template class RandomAccessArray<60>;
template class RandomAccessArray<61>;
template class RandomAccessArray<62>;
template class RandomAccessArray<63>;
template class RandomAccessArray<64>;