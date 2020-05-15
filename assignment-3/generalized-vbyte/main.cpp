#include <iostream>
#include <vector>
#include "VByteArray.h"
#include <fstream>
#include <string>
#include <random>
#include <chrono>

using namespace std;

int main() {
    int k;
    cin >> k;

    if (k > 62) {
        cout << "Max K 62" << endl;
        return 0;
    }

    vector<ull> v;

    ifstream file("data/F0", ios::binary);

    ull number;

    while(file.read(reinterpret_cast<char*>(&number), sizeof(number))) {
        v.push_back(number);
    }

    vector<ull> randomNumbers;

    mt19937 random_value(random_device{}());
    int n = 100;
    uniform_int_distribution<unsigned long long> out_of_hat_pull(0, 49999999);
    for (int i = 0; i < n; ++i) {
        ull x = out_of_hat_pull(random_value);
        randomNumbers.push_back(x);
    }

    VByteArray vb(k, v);

    auto now = std::chrono::high_resolution_clock::now();
    for (int i : randomNumbers) {
        ull x = vb.accessScan(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto time = end-now;
    cout << "Time taken for array setting " << time.count() << " nanos" << endl;
    cout << "Total memory " << vb.getSizeInBytes() << endl;
}