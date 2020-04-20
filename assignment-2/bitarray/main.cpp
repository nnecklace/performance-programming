#include <iostream>
#include <vector>
#include <random>
#include <chrono>

#include "BitArray.h"

// TODO: Remove! Only used for debugging
#include <bitset>

using namespace std;

void test1() {
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

void test2() {
    unsigned long long n, m;
    vector<int> bits;

    cin >> n;

    BitArray* ba = new BitArray(n);

    cin >> m;

    bits.resize(m);

    mt19937 random_value(random_device{}());
    uniform_int_distribution<int> hat(0,n);
    for (unsigned long long i = 0; i < m; ++i) {
        bits[i] = hat(random_value);
    }

    for (size_t i = 0; i < bits.size(); ++i) {
        ba->set(bits[i], 1);
    }

    ba->compact();

    //ba->check();
    auto now = std::chrono::high_resolution_clock::now();
    cout << "Sum up to 1 " << ba->sum(1) << endl;
    cout << "Sum up to 5 " << ba->sum(5) << endl;
    cout << "Sum up to 11 " << ba->sum(11) << endl;
    cout << "Sum up to 20 " << ba->sum(20) << endl;
    cout << "Sum up to 50 " << ba->sum(50) << endl;
    cout << "Sum up to 64 " << ba->sum(64) << endl;
    cout << "Sum up to 75 " << ba->sum(75) << endl;
    cout << "Sum up to 100 " << ba->sum(100) << endl;
    cout << "Sum up to 128 " << ba->sum(128) << endl;
    cout << "Sum up to 200 " << ba->sum(200) << endl;
    cout << "Sum up to 256 " << ba->sum(256) << endl;
    cout << "Sum up to 512 " << ba->sum(512) << endl;
    cout << "Sum up to 1024 " << ba->sum(1024) << endl;
    cout << "Sum up to 2048 " << ba->sum(2048) << endl;
    cout << "Sum up to 4096 " << ba->sum(4096) << endl;
    cout << "Sum up to " << n << " " << ba->sum(n-1) << endl;
    auto end = std::chrono::high_resolution_clock::now();
    auto time = end-now;
    cout << "Time taken for sum queries " << time.count() << " nanos" << endl;
    // Cleanup
    delete ba;
}

int main() {
    test2();
}