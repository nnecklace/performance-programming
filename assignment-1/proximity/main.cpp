#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <cstring>
#include <chrono>
#include <ctime>

typedef uint8_t ui8; 
typedef unsigned int ui;

using namespace std;

const string DIR = "data";

// cin cannot read multiple digits if the vars are a type uint8_t since uint is typedef for char
ui upper, lower;
// array values will be set to false when allocated on the heap
bool dt[256];

void loadFile(ui code, vector<ui8>& buffer) {
  string filename = DIR+"/"+"F"+to_string(code)+".vb";
  fstream input(filename, ios::in | ios::binary);
  ui8 byteSize;
  // not sure if buggy or not
  memset(dt, 0, 256);

  while (input.read(reinterpret_cast<char*>(&byteSize), sizeof(byteSize))) {
    if (dt[byteSize]) continue;
    buffer.push_back(byteSize);
    dt[byteSize] = true;
  }

  input.close();
}

ui bruteForce(vector<ui8>& setA, vector<ui8>& setB) {
  ui size = 0;
  ui alloc = (*setB.rbegin())+1;
  // we could assume the size is at max 256
  vector<bool> visited;
  visited.resize(alloc);
  for (ui8 z : setA) {
    int x = z-upper;
    x = max(x, 0);
    int y = z+lower;
    y = min(y, 255);
    for (ui8 u : setB) {
      if (u >= x && u <= y && !visited[u]) {
        size++;
        visited[u] = true;
      }
    }
  }
  return size;
}

// time complexity O(n(m log m))
// where 
// n = number of pairs
// m = size of data in files
// 1 <= m <= 255
// Since m is almost guaranteed to be 255 
// we can assume a time complexity of O(n)
ui logarithmic(vector<ui8>& setA, vector<ui8>& setB) {
    ui size = 0;

    sort(setB.begin(), setB.end());
    sort(setA.begin(), setA.end());

    vector<ui8>::iterator currentLowest = setB.begin();
    // find lowest possible k, such that k is in range of b
    int k = *currentLowest-lower;
    k = max(k, 0);
    vector<ui8>::iterator currentIndex = lower_bound(setA.begin(), setA.end(), k);

    while (currentLowest != setB.end() && currentIndex != setA.end()) {
      ui8 z = *currentIndex;

      int x = z-upper;
      x = max(x, (int)*currentLowest);
      int y = z+lower;
      y = min(y, 255);

      auto lowerR = lower_bound(currentLowest, setB.end(), x);
      auto upperR = lower_bound(currentLowest, setB.end(), y);

      if (lowerR == setB.end()) break;
      if (upperR == setB.end()) upperR = setB.end()-1;

      if (*upperR > y) upperR--; // we have gone past the upper limit

      size += 1+upperR-lowerR;

      currentLowest = upperR+1;

      // skip numbers lower than currentLowest
      k = *currentLowest-lower;
      k = max(k, 0);
      currentIndex = lower_bound(currentIndex, setA.end(), k);
    }

    return size;
}

int main() {
  vector<pair<ui,ui>> v;
  // final result
  vector<ui> log, bf;
  
  ui pr, pl;

  cin >> upper >> lower;

  while(cin >> pr >> pl) v.push_back({pr, pl});

  auto ws = std::chrono::system_clock::now();
  time_t ss = time(NULL);
  for ([[maybe_unused]]pair<ui,ui> entry : v) {
    vector<ui8> setA, setB;
    loadFile(entry.first, setA);
    loadFile(entry.second, setB);

    log.push_back(logarithmic(setA, setB));
    //bf.push_back(bruteForce(setA, setB));
  }
  time_t se = time(NULL);
  auto we = std::chrono::system_clock::now();
  auto wd = we-ws;

  cout << "Wallclock time was = " << wd.count() << endl;
  cout << "System time was = " << se-ss << endl;

  for (size_t i = 0; i < v.size(); ++i) {
    cout << "Intersection size for pairs " << v[i].first << " " <<  v[i].second << " is " << log[i] << endl;
  }
}
