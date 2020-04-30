#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include "PackedInteger.h"

using namespace std;

int main() {
    int n = 2480000;
    vector<uint64_t> v;
    PackedInteger::List<11> packed_list(n);
    PackedInteger::Array<uint16_t> packed_array(n);

    mt19937 random_value(random_device{}());
    uniform_int_distribution<unsigned long long> out_of_hat_pull(0, 2047);
    for (int i = 0; i < n; ++i) {
        unsigned long long x = out_of_hat_pull(random_value);
        v.push_back(x);
    }

    unsigned int k = 0;
    auto now = std::chrono::high_resolution_clock::now();
    for (uint64_t x : v) {
        packed_array.set(k, x);
        k++;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto time = end-now;
    cout << "Time taken for array setting " << time.count() << " nanos" << endl;

    k = 0;
    now = std::chrono::high_resolution_clock::now();
    for (uint64_t x : v) {
        packed_list.set(k, x);
        k++;
    }
    end = std::chrono::high_resolution_clock::now();
    time = end-now;
    cout << "Time taken for list setting " << time.count() << " nanos" << endl;

    now = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; ++i) {
        uint64_t val = packed_array.get(i);
    }
    end = std::chrono::high_resolution_clock::now();
    time = end-now;
    cout << "Time taken for array getting " << time.count() << " nanos" << endl;

    now = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; ++i) {
        uint64_t val = packed_list.get(i);
    }
    end = std::chrono::high_resolution_clock::now();
    time = end-now;
    cout << "Time taken for list getting " << time.count() << " nanos" << endl;
}
