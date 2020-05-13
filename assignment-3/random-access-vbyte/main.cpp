#include <iostream>
#include <bitset>
#include "RandomAccessArray.h"
#include <fstream>

using namespace std;

int main() {
    RandomAccessArray<7> ranArray;

    ifstream file("data/F0", ios::binary);

    ull number;

    while(file.read(reinterpret_cast<char*>(&number), sizeof(number))) {
        ranArray.encodeAndPush(number);
    }
}
