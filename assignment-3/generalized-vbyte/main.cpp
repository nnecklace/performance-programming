#include <iostream>
#include "VByteArray.h"
#include <vector>
#include <bitset>

using namespace std;

int main() {
    vector<ull> v;
    v.push_back(500);
    v.push_back(14);
    v.push_back(322);
    v.push_back(401);
    v.push_back(250);
    v.push_back(309);

    VByteArray vb(63U, v);

    std::cout << vb.accessScan(1) << std::endl;
    std::cout << vb.accessScan(2) << std::endl;
    std::cout << vb.accessScan(3) << std::endl;
    std::cout << vb.accessScan(4) << std::endl;
    std::cout << vb.accessScan(5) << std::endl;
    std::cout << vb.accessScan(6) << std::endl;
}