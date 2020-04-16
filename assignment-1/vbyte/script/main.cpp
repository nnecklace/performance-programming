#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <experimental/filesystem>

using namespace std;

// only needed for c++14
namespace fs = std::experimental::filesystem;

typedef long long ll;
typedef void(*commandLineFunction)(string& path);

string getFilename(string& filename) {
  return filename.substr(filename.find_last_of("/")+1);
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

ll vByteDecode(vector<uint8_t>& numberStream) {
  ll numberBytes = 0;
  if (numberStream.size() == 1) {
    numberBytes = numberStream[0]-128;
    return numberBytes;
  }

  numberBytes += numberStream[0];
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
  ofstream output("output/"+getFilename(filename)+".vb", ios::binary);

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
  ofstream output("output/"+getFilename(filename)+".dec", ios::binary | ios::app);

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
  ofstream output("output/"+getFilename(filename)+".sorted.vb", ios::binary);

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

commandLineFunction getCmdFunction(string& cmd) {
  if (cmd == "--encode") return encode;
  else if (cmd == "--decode") return decode;
  else if (cmd == "--sort") return sortFile;

  throw invalid_argument("Unknown command passed");
}

int main([[maybe_unused]]int argc, char** args) {
  if (!args[1] || !args[2]) {
    cout << "Provide a file and a command" << endl;
  }

  string cmd = args[1];
  string filename = args[2];

  commandLineFunction cmdFn; 

  try {
    cmdFn = getCmdFunction(cmd);
  } catch (invalid_argument& excep) {
    cout << excep.what() << endl;
    return 0;
  }

  if (fs::is_directory(fs::status(filename))) {
    for (auto file : fs::directory_iterator(filename)) {
      if (!fs::is_regular_file(fs::status(file))) continue;
      string path = file.path();
      cmdFn(path);
    }

    return 0;
  }

  cmdFn(filename);
}

