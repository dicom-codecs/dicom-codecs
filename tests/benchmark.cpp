#include "timer.hpp"
#include <chrono>
#include <dicomcodecs/codec.hpp>
#include <dicomcodecs/image.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace dicomcodecs;
using namespace std;

void benchmarkEncode(const image &image, const string &comment, const string &codec,
                     size_t iterations) {
  Timer timer;
  vector<uint8_t> encodedBytes;
  encode(image, encodedBytes, codec); // warm up encoder
  timer.start();
  for (size_t e = 0; e < iterations; e++) {
    encode(image, encodedBytes, codec);
  }
  double totalMS = timer.end();
  double averageMS = totalMS / iterations;
  printf("Encode %s with %s - average time of %0.2f ms (%zu iterations)\n",
         codec.c_str(), comment.c_str(), averageMS, iterations);
  //printf("Compression Ratio: %0.2f:1\n", (float)image.rawBytes.size() / (float)encodedBytes.size());
}

void benchmarkDecode(const image &image, const string &comment, const string &codec,
                     size_t iterations) {
  Timer timer;

  vector<uint8_t> encodedBytes;
  encode(image, encodedBytes, codec);

  // decode
  dicomcodecs::image imagedec;
  decode(encodedBytes, imagedec, codec); // warm up the decoder
  timer.start();
  for (size_t i = 0; i < iterations; i++) {
    decode(encodedBytes, imagedec, codec);
  }
  double totalMS = timer.end();
  double averageMS = totalMS / iterations;
  printf("Decode %s with %s - average time of %0.2f ms(%zu iterations)\n",
         codec.c_str(), comment.c_str(), averageMS, iterations);
}

void benchmark(image &image, const string &comment, const string &codec,
               size_t iterations) {
  benchmarkEncode(image, comment, codec, iterations);
  benchmarkDecode(image, comment, codec, iterations);
}
