#include <iostream>
#include "VByteArray.h"
#include <vector>

using namespace std;

int main() {
    vector<ull> v;
    v.push_back(500);
    v.push_back(14);
    v.push_back(322);
    v.push_back(401);
    v.push_back(250);
    v.push_back(309);

    // max k 63

    VByteArray vb(4U, v);

    vb.accessScan(6);
}