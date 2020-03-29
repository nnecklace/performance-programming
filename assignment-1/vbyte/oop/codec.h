#include <vector>
#include <algorithm>
#include <type_traits>
#include "file.h"

using std::vector;
using std::pair;
using file::File;
using std::enable_if_t;
using std::is_unsigned;

typedef unsigned long long ll;

namespace codec
{
  class Codec 
  {
    private:
      vector<uint8_t> encodeVByte(ll number);
      template<typename T, enable_if_t<is_unsigned<T>::value>* = nullptr>
      T decodeVByte(vector<uint8_t>& encodedNumber);
    public:
      template<typename T, enable_if_t<is_unsigned<T>::value>* = nullptr>
      vector<pair<vector<uint8_t>, size_t>> encode(File& file);
      template<typename T, enable_if_t<is_unsigned<T>::value>* = nullptr>
      vector<pair<vector<uint8_t>, size_t>> sort(File& file);
      template<typename T, enable_if_t<is_unsigned<T>::value>* = nullptr>
      vector<T> decode(File& file);
  };
}

vector<uint8_t> codec::Codec::encodeVByte(ll number) 
{
  vector<uint8_t> encodedStream;
  while (true) {
    encodedStream.push_back(number % 128);
    if (number < 128) {
      encodedStream[encodedStream.size()-1] += 128;
      break;
    }

    number /= 128;
  }
  return encodedStream;
}

template<typename T, enable_if_t<is_unsigned<T>::value>*>
T codec::Codec::decodeVByte(vector<uint8_t>& numberStream) 
{
  if (numberStream.size() == 1) {
    return numberStream[0]-128;
  }

  T decodedNumber = 0;

  decodedNumber += numberStream[0];
  // length of number in bytes
  T length = sizeof(T);

  for (size_t i = 1; i < numberStream.size(); ++i) {
    T portion = 0;
    T currentVByte = numberStream[i];
 
    if (currentVByte < 128) {
      portion = currentVByte<<(length-1);
    } else {
      // remove stop bit
      portion = (currentVByte-128)<<(length-1);
    }
    decodedNumber += portion;
    // move seven bits forward to remove stop bit
    length += 7; 
  }

  return decodedNumber;
}

// T is used to describe what kind of number we want to encode (32bit, 64bit, 24bit) 
template<typename T, enable_if_t<is_unsigned<T>::value>*>
vector<pair<vector<uint8_t>, size_t>> codec::Codec::encode(File& file) 
{
  T number;
  vector<T> numberBuffer;
  vector<pair<vector<uint8_t>, size_t>> encodedNumbers;
  file.readAndPopulate<T>(number, sizeof(number), numberBuffer);

  for (size_t i = 0; i < numberBuffer.size(); ++i) {
    vector<uint8_t> encodedNumber = encodeVByte(numberBuffer[i]);
    encodedNumbers.push_back({encodedNumber, encodedNumber.size()});
  }

  return encodedNumbers;
}

template<typename T, enable_if_t<is_unsigned<T>::value>*>
vector<pair<vector<uint8_t>, size_t>> codec::Codec::sort(File& file) 
{
  T number;
  vector<T> numberBuffer;
  vector<pair<vector<uint8_t>, size_t>> sortedEncodedNumbers;
  file.readAndPopulate<ll>(number, sizeof(number), numberBuffer); 

  std::sort(numberBuffer.begin(), numberBuffer.end());
  
  for (size_t i = 1; i < numberBuffer.size(); ++i) {
    ll difference = numberBuffer[i] - numberBuffer[i-1];
    vector<uint8_t> result = encodeVByte(difference);
    sortedEncodedNumbers.push_back({result, result.size()});
  }

  return sortedEncodedNumbers;
}

template<typename T, enable_if_t<is_unsigned<T>::value>*>
vector<T> codec::Codec::decode(File& file) 
{
  uint8_t numberBytes;
  vector<uint8_t> encodedVByteStream;
  vector<T> decodedNumbers;

  while(file.read(numberBytes, sizeof(numberBytes))) {
    encodedVByteStream.push_back(numberBytes);
    if ((numberBytes & 128) == 0) continue; // stop bit not set
    decodedNumbers.push_back(decodeVByte<T>(encodedVByteStream));
    encodedVByteStream.clear();
  }

  return decodedNumbers;
}

