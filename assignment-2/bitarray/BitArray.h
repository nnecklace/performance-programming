#include <iostream>

typedef unsigned long long ull;

// TODO: Create template for bit sizes
class BitArray {
    private:
        ull* seq;
    public:
        BitArray(int n);
        ~BitArray();
        ull get(ull nth) const;
        void set(ull nth, ull i);
        ull check(ull block);
        ull sum (const int n) const;
};

BitArray::BitArray(int n) {
    seq = new ull[((n+63)/64)*64];
}

BitArray::~BitArray() {
    delete []seq;
}

ull BitArray::get(ull nth) const {
    return ((seq[nth/64]>>(nth%64)) & 1);
}

void BitArray::set(ull nth, ull i) {
    seq[nth/64] |= (i<<(nth%64));
}

// TODO: Remove! Only used for debugging
ull BitArray::check(ull block) {
    return seq[block];
}