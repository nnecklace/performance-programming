#include <iostream>
#include <vector>
#include "VByteArray.h"
#include <fstream>
#include <string>

using namespace std;

int main() {
    int k;
    cin >> k;

    if (k > 63) {
        cout << "Max K 63" << endl;
        return 0;
    }

    vector<ull> v;

    ifstream file("data/F0", ios::binary);

    ull number;

    while(file.read(reinterpret_cast<char*>(&number), sizeof(number))) {
        v.push_back(number);
    }

    VByteArray vb(k, v);

}