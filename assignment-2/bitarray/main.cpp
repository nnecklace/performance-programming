#include <iostream>
#include "BitArray.h"
#include <vector>
#include <random>
#include <chrono>

#include <bitset>

using namespace std;

int main() {
    int n, m;
    vector<int> bits;

    cin >> n;

    BitArray* ba = new BitArray(n);

    cin >> m;

    bits.resize(m);

    mt19937 random_value(random_device{}());
    uniform_int_distribution<int> hat(0,n);
    for (int i = 0; i < m; ++i) {
        bits[i] = hat(random_value);
    }

    auto now = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < bits.size(); ++i) {
        ba->set(bits[i], 1);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto time = end-now;
    cout << "Time taken for setting " << time.count() << "nanos" << endl;

    now = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < bits.size(); ++i) {
        ull p = ba->get(bits[i]);
    }
    end = std::chrono::high_resolution_clock::now();
    time = end-now;
    cout << "Time taken for getting " << time.count() << "nanos" << endl;

    // Cleanup
    delete ba;
}