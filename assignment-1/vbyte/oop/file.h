#include <string>
#include <vector>
#include <iostream>
#include <fstream>

typedef unsigned long long ll;

using std::string;
using std::vector;
using std::fstream;
using std::ios;
using std::ios_base;
using std::pair;

namespace file 
{
  class File
  {
    private:
      fstream file;
    public:
      File(string& name, ios_base::openmode mode) {
        this->file = fstream(name, mode | ios::binary);
      }
      File(string& name, string& dir, ios_base::openmode mode) {
        string fullPath = dir+"/"+name;
        File(fullPath, mode);
      }
      template<typename T>
      void readAndPopulate(T& value, size_t size, vector<T>& bufferStream);
      bool read(uint8_t& chunck, size_t size);
      void save(vector<pair<vector<uint8_t>, size_t>>& contents);
      template<typename T>
      void save(vector<T>& contents);
      void close();
  };
}

template<typename T>
void file::File::readAndPopulate(T& value, size_t size, vector<T>& bufferStream) 
{
  while(file.read(reinterpret_cast<char*>(&value), size)) {
    bufferStream.push_back(value);
  }
}

bool file::File::read(uint8_t& chunck, size_t size) 
{
  file.read(reinterpret_cast<char*>(&chunck), size); 

  // eof bit is only set after we have read past the last char in the file
  // therefore we do the check like this.
  // don't know how to convert istream* to bool
  return !file.eof();
}

// used for saving encoded data
void file::File::save(vector<pair<vector<uint8_t>, size_t>>& contents)
{
  for (size_t i = 0; i < contents.size(); ++i) {
    pair<vector<uint8_t>, size_t> chunck = contents[i];
    file.write((char*)&chunck.first[0], chunck.second);
  }
}

// used for saving decoded data
template<typename T>
void file::File::save(vector<T>& contents) 
{
  for (size_t i = 0; i < contents.size(); ++i) {
    T value = contents[i];
    file.write((char*)&value, sizeof(T));
  } 
}

void file::File::close() 
{
  if (file.is_open()) {
    file.close();
  }
}

