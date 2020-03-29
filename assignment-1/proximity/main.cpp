#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <cstring>

typedef uint8_t ui8; 
typedef unsigned int ui;

using namespace std;

const string DIR = "data";

// Notes use segment tree to count running sum in n log n algorithm

// cin cannot read multiple digits if the vars are a type uint8_t since uint is typedef for char
ui upper, lower, pr, pl;
vector<pair<ui,ui>> v;
vector<ui8> setA;
vector<ui8> setB;
// final result
vector<ui> result;
bool dt[256];

int naiveCount;
bool visited[256];

void loadFile(ui code, vector<ui8>& buffer) {
  string filename = DIR+"/"+"F"+to_string(code)+".vb";
  fstream input(filename, ios::in | ios::binary);
  ui8 byteSize;
  memset(dt, false, sizeof(dt));

  while (input.read(reinterpret_cast<char*>(&byteSize), sizeof(byteSize))) {
    if (dt[byteSize]) continue;
    buffer.push_back(byteSize);
    dt[byteSize] = true;
  }

  input.close();
}

void printV(vector<ui8>& w) {
  for (size_t i = 0; i < w.size(); ++i) {
    cout << (ui)w[i] << " ";
  }
  cout << endl;
}

void naiveSolution(vector<ui8>& a, vector<ui8>& b) {
  for (ui8 z : a) {
    int x = z-upper;
    x = max(x, 0);
    int y = z+lower;
    y = min(y, 255);
    for (ui8 u : b) {
      if (u >= x && u <= y && !visited[u]) {
        naiveCount++;
        visited[u] = true;
      }
    }
  }
}

void solution() {
    sort(setB.begin(), setB.end());
    sort(setA.begin(), setA.end());

    vector<ui8>::iterator currentLowest = setB.begin();
    vector<ui8>::iterator currentIndex = setA.begin();
    ui size = 0;

    while (currentLowest != setB.end() && currentIndex != setA.end()) {
      ui8 z = *currentIndex;
      currentIndex++;

      int x = z-upper;
      x = max(x, (int)*currentLowest);
      int y = z+lower;
      y = min(y, 255);

      auto lowerR = lower_bound(currentLowest, setB.end(), x);
      auto upperR = lower_bound(currentLowest, setB.end(), y);

      if (lowerR == setB.end()) break;
      if (upperR == setB.end()) upperR = setB.end()-1;

      if (*upperR > y) upperR--; // we have gone past the upper limit
      if (*lowerR > y || *upperR < x) continue; // no possible numbers within this range

      size += 1+upperR-lowerR;

      currentLowest = upperR+1;
      // skip numbers lower than currentLowest
      int k = *currentLowest-lower;
      k = max(k, 0);
      currentIndex = lower_bound(currentIndex, setA.end(), *currentLowest-lower);
    }

    result.push_back(size);
}

int main() {
  cin >> upper >> lower;

  while(cin >> pr >> pl) v.push_back({pr, pl});

  // time complexity O(n(m log m))
  // where 
  // n = number of pairs
  // m = size of data in files
  // 1 <= m <= 255
  // overall worst complexity n*255(log 255)
  for (pair<ui,ui> entry : v) {
    loadFile(entry.first, setA);
    loadFile(entry.second, setB);

    naiveSolution(setA, setB);
    solution();
  }

  for (size_t i = 0; i < v.size(); ++i) {
    cout << "Intersection size for files F" << v[i].first << " and F" << v[i].second << endl;
    cout << "Naive : " << naiveCount << " better " << result[i] << endl;
  }
}
