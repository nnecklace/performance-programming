#include <iostream>
#include <vector>
#include <random>
#include <chrono>

#include "BitArray.h"
#include <bitset>

using namespace std;

void test1(vector<int>& bits, int n, int m) {
    BitArray ba = BitArray(n);

    mt19937 random_value(random_device{}());
    uniform_int_distribution<int> hat(0,n);
    for (int i = 0; i < m; ++i) {
        bits.push_back(hat(random_value));
    }

    auto now = std::chrono::steady_clock::now();
    for (int i : bits) ba.set(i, 1);
    auto end = std::chrono::steady_clock::now();
    auto time = end-now;
    cout << "Time taken for setting " << time.count() << " nanos" << endl;

    now = std::chrono::steady_clock::now();
    for (int i : bits) {
        // we need to store the value somewhere, otherwise to compiler will remove the get call
        ull p = ba.get(i);
    }
    end = std::chrono::steady_clock::now();
    time = end-now;
    cout << "Time taken for getting " << time.count() << " nanos" << endl;

    bits.clear();
    for (int i = 0; i < m; ++i) {
        bits.push_back(hat(random_value));
    }

    now = std::chrono::steady_clock::now();
    for (int i : bits) {
        ull p = ba.get(i);
    }
    end = std::chrono::steady_clock::now();
    time = end-now;
    cout << "Time taken for getting " << time.count() << " nanos" << endl;
}

void test2(vector<int>& bits, int n, int m) {
    BitArray ba = BitArray(n);

    mt19937 random_value(random_device{}());
    uniform_int_distribution<int> hat(0,n);
    for (int i = 0; i < m; ++i) {
        bits.push_back(hat(random_value));
    }

    for (int i : bits) ba.set(i, 1);

    ba.compact();

    auto now = std::chrono::high_resolution_clock::now();
    for (int i : bits) {
        ull p = ba.sum(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto time = end-now;
    cout << "Time taken for sum queries " << time.count() << " nanos" << endl;

}

int main() {
    int n, m;
    vector<int> bits;
    cin >> n >> m;
    test2(bits,n,m);
}