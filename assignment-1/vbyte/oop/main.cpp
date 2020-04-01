#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <ctime>
#include <experimental/filesystem>
#include "codec.h"

using namespace std;
using namespace codec;
using namespace file;
using namespace chrono;

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

// Operations
// DIR and FILE are reserved
const string F = "--file";
const string D = "--all";

const string OUTDIR = "--output";

// This is only needed for macos 
namespace fs = std::experimental::filesystem;

string getExt(string& cmd) {
  if (cmd == ENCODE) return ".vb";
  if (cmd == DECODE) return ".dec";
  if (cmd == SORT) return ".sorted.vb";

  return "";
}

string getOutDir(char** args) {
  int i = 0;
  while (args[i]) {
    if (args[i] == OUTDIR) return args[i+1];
    i++;
  }

  return "";
}

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
void each(string& inDir, string& outDir, string& cmd, Codec& codec) {
  for (auto file : fs::directory_iterator(inDir)) {
    if (!fs::is_regular_file(fs::status(file))) continue;

    string path = file.path().string();
    string outputFile = outDir+"/"+file.path().filename().string()+getExt(cmd);

    File input (path, ios::in);
    File output(outputFile, ios::out);

    if (cmd == ENCODE) encode<T>(input, output, codec);
    if (cmd == DECODE) decode<T>(input, output, codec);
    if (cmd == SORT)   sort<T>(input, output, codec);

    output.close();
    input.close();
  }
}

// ./main --encode --all input --output output 
// ./main --encode --file F0 --output output
int main([[maybe_unused]]int argc, char** args) {
  if (!args[1] || !args[2]) {
    cout << "Please provide a command and an operation"  << endl;

    return 1;
  } 

  string cmd = args[1];

  if (cmd != ENCODE && cmd != DECODE && cmd != SORT) {
    cout << "Please provide a valid command" << endl;

    return 1;
  }

  if (!args[3]) {
    cout << "Please provide a file or directory" << endl;

    return 1;
  }

  string operation = args[2];
  Codec codec;

  if (operation == D) {
    string inDir = args[3];
    string outDir = getOutDir(args);

    if (fs::is_directory(fs::status(inDir))) {
      auto ws = system_clock::now();
      time_t ss = time(NULL);
      each<ll>(inDir, outDir, cmd, codec);
      time_t se = time(NULL);
      auto we = system_clock::now();

      auto wd = we-ws;
      cout << "Wallclock time was = " << wd.count() << endl;
      cout << "System time was = " << se-ss << endl;
      cout << "Memory usage = " << getMemoryUsage() << endl;

      return 0;
    } else {
      cout << inDir << " is not a directory!" << endl;
      return 1;
    }
  }

  string fileArg = args[3];
  string inputFile = fs::path(fileArg).filename();
  string outputFile = inputFile+getExt(cmd);

  File input(inputFile, ios::in);
  File output(outputFile, ios::out);

  auto ws = system_clock::now();
  time_t ss = time(NULL);
  if (cmd == ENCODE) encode<ll>(input, output, codec);
  if (cmd == DECODE) decode<ll>(input, output, codec);
  if (cmd == SORT)   sort<ll>(input, output, codec);
  time_t se = time(NULL);
  auto we = system_clock::now();

  auto wd = we-ws;
  cout << "Wallclock time was = " << wd.count() << endl;
  cout << "System time was = " << se-ss << endl;
  cout << "Memory usage = " << getMemoryUsage() << endl;

  output.close();
  input.close();
}

