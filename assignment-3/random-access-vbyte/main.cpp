#include <iostream>
#include <bitset>
#include <fstream>
#include "RandomAccessArray.h"

using namespace std;

int main() {
    RandomAccessArray<63> ranArray;

    ifstream file("data/F0", ios::binary);

    ull number;

    while(file.read(reinterpret_cast<char*>(&number), sizeof(number))) {
        ranArray.encodeAndPush(number);
    }

    file.close();

    ranArray.compact();
}
