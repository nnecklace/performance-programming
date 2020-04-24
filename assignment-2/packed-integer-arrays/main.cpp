#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include "PackedInteger.h"

#include <bitset>

using namespace std;

int main() {
    int n = 100;
    vector<uint64_t> v(n);
    PackedInteger::List<45> packed_array(n);
    //PackedInteger::Array<uint64_t> packed_array(n);

    mt19937 random_value(random_device{}());
    uniform_int_distribution<unsigned long long> out_of_hat_pull(0, 35184372088831);
    for (int i = 0; i < n; ++i) {
        unsigned long long x = out_of_hat_pull(random_value);
        v[i] = x;
        packed_array.set(i, x);
    }

    for (int i = 0; i < n; ++i) {
        cout << v[i] << " - " << packed_array.get(i) << endl;
    }
}
