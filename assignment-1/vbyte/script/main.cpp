#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <experimental/filesystem>

typedef long long ll;

using namespace std;
using namespace chrono;

// only needed for macos
namespace fs = std::experimental::filesystem;

// Found on stackoverflow: https://stackoverflow.com/questions/1543157/how-can-i-find-out-how-much-memory-my-c-app-is-using-on-the-mac
long getMemoryUsage() {
  struct rusage usage;
  if(0 == getrusage(RUSAGE_SELF, &usage))
    return usage.ru_maxrss; // bytes
  else
    return 0;
}

vector<uint8_t> vByteEncode(ll numberBlock) {
  vector<uint8_t> bytes;
  while (true) {
    bytes.push_back(numberBlock % 128);
    if (numberBlock < 128) {
      bytes[bytes.size()-1] += 128;
      break;
    }

    numberBlock /= 128;
  }
  return bytes;
}

ll vByteDecode(vector<uint8_t> numberStream) {
  ll numberBytes = 0;
  if (numberStream.size() == 1) {
    numberBytes = numberStream[0]-128;
    return numberBytes;
  }

  numberBytes += numberStream[0];
  // might lead to bug?
  ll length = 8;

  for (size_t i = 1; i < numberStream.size(); ++i) {
    ll n = 0;
    ll number = numberStream[i];
 
    if (number < 128) {
      n = number<<(length-1);
    } else {
      // remove stop bit
      n = (number-128)<<(length-1);
    }
    numberBytes += n;
    length += 7;
  }

  return numberBytes;
}

void encode(string& filename) {
  ifstream input(filename, ios::binary);
  ofstream output("output/"+(filename.substr(filename.find_last_of("/")+1))+".vb", ios::binary);
  ll numberBytes;
  while(input.read(reinterpret_cast<char*>(&numberBytes), sizeof(numberBytes))) {
    vector<uint8_t> result = vByteEncode(numberBytes);
    output.write((char*)&result[0], result.size());
  }
  
  input.close();
  output.close();
}

void decode(string& filename) {
  vector<uint8_t> byteStream;
  uint8_t numberBytes;

  ifstream input(filename, ios::binary);
  ofstream output("output/"+(filename.substr(filename.find_last_of("/")+1))+".dec", ios::binary | ios::app);

  while(input.read(reinterpret_cast<char*>(&numberBytes), sizeof(numberBytes))) {
    byteStream.push_back(numberBytes);
    if ((numberBytes & 128) == 0) continue; // stop bit not set
    ll result = vByteDecode(byteStream);
    output.write((char*)&result, sizeof(ll));
    byteStream.clear();
  }

  input.close();
  output.close();
}

void sortFile(string& filename) {
  ifstream input(filename, ios::binary);
  ofstream output("output/"+(filename.substr(filename.find_last_of("/")+1))+".sorted.vb", ios::binary);

  ll numberBytes;
  vector<ll> v;

  while(input.read(reinterpret_cast<char*>(&numberBytes), sizeof(numberBytes))) {
    v.push_back(numberBytes);
  }

  sort(v.begin(), v.end());
  
  for (size_t i = 1; i < v.size(); ++i) {
    ll difference = v[i] - v[i-1];
    vector<uint8_t> result = vByteEncode(difference);
    output.write((char*)&result[0], result.size());
  }

  input.close();
  output.close();
}

int main([[maybe_unused]]int argc, char** args) {
  if (!args[1] || !args[2]) {
    cout << "Provide a file and a command" << endl;
  }

  string cmd = args[1];
  string filename = args[2];

  if (fs::is_directory(fs::status(filename))) {
    auto ws = system_clock::now();
    time_t ss = time(NULL);

    for (auto file : fs::directory_iterator(filename)) {
      if (!fs::is_regular_file(fs::status(file))) continue;

      string path = file.path();

      if (cmd == "encode") encode(path);
      if (cmd == "decode") decode(path);
      if (cmd == "sort") sortFile(path);
    }

    time_t se = time(NULL);
    auto we = system_clock::now();

    auto wd = we-ws;
    cout << "Wallclock time was = " << wd.count() << endl;
    cout << "System time was = " << se-ss << endl;
    cout << "Memory usage = " << getMemoryUsage() << endl;

    return 0;
  }

  auto ws = system_clock::now();
  time_t ss = time(NULL);
  if (cmd == "encode") encode(filename);
  if (cmd == "decode") decode(filename);
  if (cmd == "sort") sortFile(filename);
  time_t se = time(NULL);
  auto we = system_clock::now();

  auto wd = we-ws;
  cout << "Wallclock time was = " << wd.count() << endl;
  cout << "System time was = " << se-ss << endl;
  cout << "Memory usage = " << getMemoryUsage() << endl;
}

