#include <iostream>
#include <bitset>
#include <random>
#include <fstream>
#include "RandomAccessArray.h"

using namespace std;

// g++ -fsanitize=undefined main.cpp BitArray.cpp RandomAccessArray.cpp PackedInteger.cpp -o main -O2

int main() {
    RandomAccessArray<7> ranArray;
    vector<ull> comparison(50000000);

    ifstream file("data/F0", ios::binary);

    ull number;
    int count = 0;

    while(file.read(reinterpret_cast<char*>(&number), sizeof(number))) {
        comparison[count++] = number;
        ranArray.encodeAndPush(number);
    }

    file.close();

    ranArray.compact();

    cout << "Layers " << ranArray.getLayers().size() << endl;

    vector<ull> randomNumbers;

    mt19937 random_value(random_device{}());
    int n = 1000;
    uniform_int_distribution<unsigned long long> out_of_hat_pull(0, 49999999);
    for (int i = 0; i < n; ++i) {
        ull x = out_of_hat_pull(random_value);
        randomNumbers.push_back(x);
    }

    cout << "Checking matches" << endl;

    count = 0;
    for (int i : randomNumbers) {
        if (ranArray.accessScan(i) != comparison[i]) {
            count++;
            cout << ranArray.accessScan(i) << " " << comparison[i] << endl;
        }
    }

    cout << "Mis match count " << count << endl;

    // size 781251*64 = 50 Mega * 5 layers = 250 megs
    // size 5468751*64 = 350 megs * 5 layers = 1750000320 megs

    // vector bytes size 24
    // Total size = 1750000320+250+8+24 = ~~1.8 Gigabytes

}
