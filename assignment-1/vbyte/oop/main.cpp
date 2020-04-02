#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <ctime>
#include <stdexcept>
#include <experimental/filesystem>
#include "codec.h"

using namespace std;
using namespace codec;
using namespace file;
using namespace chrono;

typedef void(*commandLineFunction)(File& in, File& ou, Codec& c);

// Found on stackoverflow: https://stackoverflow.com/questions/1543157/how-can-i-find-out-how-much-memory-my-c-app-is-using-on-the-mac
long getMemoryUsage() {
  struct rusage usage;
  if(0 == getrusage(RUSAGE_SELF, &usage)) {
    return usage.ru_maxrss; // bytes
  } else {
    return 0;
  }
}

// Commands
const string ENCODE = "--encode";
const string DECODE = "--decode";
const string SORT = "--sort";
const string OUTDIR = "--output";

// This is only needed for macos 
namespace fs = std::experimental::filesystem;

template<typename T, enable_if_t<is_unsigned<T>::value>* = nullptr>
void encode(File& inFile, File& outFile, Codec& codec) {
  vector<pair<vector<uint8_t>, size_t>> encodedOutput = codec.encode<T>(inFile);

  outFile.save(encodedOutput);
}

template<typename T, enable_if_t<is_unsigned<T>::value>* = nullptr>
void decode(File& inFile, File& outFile, Codec& codec) {
  vector<T> decodedOutput = codec.decode<T>(inFile);

  outFile.save(decodedOutput);
}

template<typename T, enable_if_t<is_unsigned<T>::value>* = nullptr>
void sort(File& inFile, File& outFile, Codec& codec) {
  vector<pair<vector<uint8_t>, size_t>> encodedOutput = codec.sort<T>(inFile);

  outFile.save(encodedOutput);
}

template<typename T, enable_if_t<is_unsigned<T>::value>* = nullptr>
void each(string& inDir, string& outDir, string& ext, commandLineFunction& cmdFn, Codec& codec) {
  for (auto file : fs::directory_iterator(inDir)) {
    if (!fs::is_regular_file(fs::status(file))) continue;

    string inPath = file.path().string();
    string outPath = outDir+"/"+file.path().filename().string()+ext;

    File input (inPath, ios::in);
    File output(outPath, ios::out);

    cmdFn(input, output, codec);

    output.close();
    input.close();
  }
}

// Helpers
string getOutDir(char** args) {
  int i = 0;
  while (args[i]) {
    if (args[i] == OUTDIR) return args[i+1];
    i++;
  }

  return "";
}

commandLineFunction getCmdFunction(string& cmd) {
  if (cmd == ENCODE) return encode<ll>;
  if (cmd == DECODE) return decode<ll>;
  if (cmd == SORT) return sort<ll>;

  throw invalid_argument("Unknown command passed");
}

string getExt(string& cmd) {
  if (cmd == ENCODE) return ".vb";
  if (cmd == DECODE) return ".dec";
  if (cmd == SORT) return ".sorted.vb";

  return "";
}

// ./main --encode input --output output 
// ./main --encode F0 --output output
int main([[maybe_unused]]int argc, char** args) {
  if (!args[1] || !args[2]) {
    cout << "Please provide a command and a file or folder"  << endl;

    return 0;
  } 

  string cmd = args[1];
  string inDir = args[2];
  string outDir = getOutDir(args);
  string ext;

  commandLineFunction cmdFn;
  Codec codec;

  try {
    cmdFn = getCmdFunction(cmd);
    ext = getExt(cmd);
  } catch (invalid_argument& excep) {
    cout << excep.what() << endl;
    return 0;
  }

  if (fs::is_directory(fs::status(inDir))) {
    auto ws = system_clock::now();
    time_t ss = time(NULL);
    each<ll>(inDir, outDir, ext, cmdFn, codec);
    time_t se = time(NULL);
    auto we = system_clock::now();

    auto wd = we-ws;
    cout << "Wallclock time was = " << wd.count() << endl;
    cout << "System time was = " << se-ss << endl;
    cout << "Memory usage = " << getMemoryUsage() << endl;

    return 0;
  }

  string filename = fs::path(inDir).filename();
  string inPath = fs::path(inDir).string();
  string outPath = outDir+filename+ext;

  File input(inPath, ios::in);
  File output(outPath, ios::out);

  auto ws = system_clock::now();
  time_t ss = time(NULL);
  cmdFn(input, output, codec);
  time_t se = time(NULL);
  auto we = system_clock::now();

  auto wd = we-ws;
  cout << "Wallclock time was = " << wd.count() << endl;
  cout << "System time was = " << se-ss << endl;
  cout << "Memory usage = " << getMemoryUsage() << endl;

  output.close();
  input.close();
}

