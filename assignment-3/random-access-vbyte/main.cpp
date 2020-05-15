#include <iostream>
#include <bitset>
#include <random>
#include <fstream>
#include <chrono>
#include "RandomAccessArray.h"

using namespace std;

// g++ -fsanitize=undefined main.cpp BitArray.cpp RandomAccessArray.cpp PackedInteger.cpp -o main -O2

int main() {
    RandomAccessArray<32> ranArray;

    ifstream file("data/F0", ios::binary);

    ull number;

    while(file.read(reinterpret_cast<char*>(&number), sizeof(number))) {
        ranArray.encodeAndPush(number);
    }

    file.close();

    ranArray.compact();

    vector<ull> randomNumbers;

    mt19937 random_value(random_device{}());
    int n = 100;
    uniform_int_distribution<unsigned long long> out_of_hat_pull(0, 49999999);
    for (int i = 0; i < n; ++i) {
        ull x = out_of_hat_pull(random_value);
        randomNumbers.push_back(x);
    }

    auto now = std::chrono::high_resolution_clock::now();
    for (int i : randomNumbers) {
        ull x = ranArray.accessScan(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto time = end-now;
    cout << "Time taken for array setting " << time.count() << " nanos" << endl;

    // size 781251*64 = 50 Mega * 5 layers = 250 megs
    // size 5468751*64 = 350 megs * 5 layers = 1750000320 megs

    // vector bytes size 24
    // Total size = 1750000320+250+8+24 = ~~1.8 Gigabytes
}
