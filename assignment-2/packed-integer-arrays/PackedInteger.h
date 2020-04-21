#include <algorithm>
#include <iostream>

typedef unsigned long long ull;

namespace PackedInteger 
{
    const unsigned char BITS = 8;
    template<typename T> using EnableIfUnsigned = typename std::enable_if<std::is_unsigned<T>::value>::type;
    template<typename T, typename = typename std::enable_if<std::is_unsigned<T>::value>::type> class Array;
    template<typename T> class Array<T, EnableIfUnsigned<T>>
    {
    private:
        ull* sequence;
        T* width;
        const ull padding_size;
        const unsigned char BLOCK_SIZE = 64;
    public:
        Array(ull n);
        ~Array();
        ull get(ull nth) const;
        void set(ull nth, ull i);
        ull check(ull block);
    };

    template<typename T>
    Array<T, EnableIfUnsigned<T>>::Array(ull n) : padding_size(sizeof(*width)*BITS)
    {
        ull size = n*padding_size/BLOCK_SIZE+1;
        sequence = new ull[size];
        std::fill(sequence, sequence+size, 0);
    }

    template<typename T>
    Array<T, EnableIfUnsigned<T>>::~Array()
    {
        delete []sequence;
    }

    template<typename T>
    ull Array<T, EnableIfUnsigned<T>>::get(ull nth) const
    {
        // need a unsigned base value incase of overflow
        ull i = 1;
        ull k = padding_size*nth;
        ull max_value = padding_size == 64 ? ~(i-1) : (i<<padding_size)-1;
        return max_value & (sequence[k/BLOCK_SIZE]>>(k & (BLOCK_SIZE-1)));
    }

    template<typename T>
    void Array<T, EnableIfUnsigned<T>>::set(ull nth, ull i)
    {
        ull k = padding_size*nth;
        sequence[k/BLOCK_SIZE] |= i<<((BLOCK_SIZE-1)&k);
    }

    template<typename T>
    ull Array<T, EnableIfUnsigned<T>>::check(ull block) 
    {
        return sequence[block];
    }

    template<unsigned int T> class List
    {

    static_assert(T <= 64, "Integer width cannot be larger than 64");
    private:
        ull* sequence;
        const ull padding_size = T;
        const unsigned char BLOCK_SIZE = 64;
    public:
        List(ull n);
        ~List();
        ull get(ull nth) const;
        void set(ull nth, ull i);
        ull check(ull block) const;
    };


    template<unsigned int T>
    List<T>::List(ull n)
    {
        ull size = n*padding_size/BLOCK_SIZE+1;
        sequence = new ull[size];
        std::fill(sequence, sequence+size, 0);
    }

    template<unsigned int T>
    List<T>::~List() 
    {
        delete []sequence;
    }

    template<unsigned int T>
    ull List<T>::get(ull nth) const
    {
        ull i = 1;
        ull start = padding_size*nth;
        ull end = start+padding_size;
        ull modulus_value = BLOCK_SIZE-1;
        ull max_value = padding_size == 64 ? ~(i-1) : (i<<padding_size)-1;

        // find start of value might be whole value
        ull value_a = max_value & (sequence[start/BLOCK_SIZE]>>(start & modulus_value));

        // start and end are not in same block
        if ((start & modulus_value) > (end & modulus_value)) {
            ull value_b = sequence[end/BLOCK_SIZE] & ((i<<(end & modulus_value))-1);
            value_a |= value_b<<(BLOCK_SIZE-(start & modulus_value));
        }

        return value_a;
    }

    template<unsigned int T>
    void List<T>::set(ull nth, ull i) 
    {
        ull start = padding_size*nth;
        ull block = start/BLOCK_SIZE;
        ull pos_in_block = start & (BLOCK_SIZE - 1);
        sequence[block] |= i<<(pos_in_block);
        if (pos_in_block+padding_size > BLOCK_SIZE) {
            ull bits_left = BLOCK_SIZE-pos_in_block;
            i >>= bits_left;
            sequence[block+1] |= i;
        }
    }

    template<unsigned int T>
    ull List<T>::check(ull block) const
    {
        return sequence[block];
    }
}
