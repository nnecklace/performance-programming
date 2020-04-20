#include <iostream>
#include "PackedInteger.h"

#include <bitset>

using namespace std;

int main() {
    PackedInteger::Array<uint8_t> packed_array(10);
    packed_array.set(0, 64);
    packed_array.set(1, 127);
    packed_array.set(2, 2);
    packed_array.set(3, 1);

    cout << bitset<64>(packed_array.check(0)) << endl;

    cout << packed_array.get(0) << endl;
    cout << packed_array.get(1) << endl;
    cout << packed_array.get(2) << endl;
    cout << packed_array.get(3) << endl;
    cout << packed_array.get(4) << endl;
    cout << packed_array.get(5) << endl;
}
