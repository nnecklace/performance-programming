#include <iostream>

using namespace std;


int main() {
  int n, lower, upper;

  cin >> lower >> upper;

  cin >> n;

  cout << lower << " " << upper << endl;

  for (int i = 0; i < n; ++i) {
    int first = std::rand() % 246;
    int second = std::rand() % 246;
    if (first == second) {
      --i;
      continue;
    }
    cout << first << " " << second << endl;
  }
}
