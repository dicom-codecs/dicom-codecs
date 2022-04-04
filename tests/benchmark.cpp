#include <string>
#include <vector>
#include <dicomcodecs/image.hpp>
#include <dicomcodecs/codec.hpp>
#include <chrono>
#include <iostream>
#include "timer.hpp"

void benchmarkEncode(dicomcodecs::image& image, const std::string& comment, const std::string& codec, size_t iterations) {
    Timer timer;
    std::vector<uint8_t> encodedBytes;
    timer.start();
    for(size_t e=0; e < iterations; e++) {
        encode(image, encodedBytes, codec);
    }
    double totalMS = timer.end();
    double averageMS = totalMS / iterations;
    printf("Encode %s with %s - average time of %0.2f ms (%zu iterations)\n", codec.c_str(), comment.c_str(), averageMS, iterations);
}

void benchmarkDecode(dicomcodecs::image& image, const std::string& comment, const std::string& codec, size_t iterations) {
    Timer timer;

    std::vector<uint8_t> encodedBytes;
    encode(image, encodedBytes, codec);

    // decode
    timer.start();
    for(size_t i=0; i < iterations; i++) {
        decode(encodedBytes, image, codec);
    }
    double totalMS = timer.end();
    double averageMS = totalMS / iterations;
    printf("Decode %s with %s - average time of %0.2f ms(%zu iterations)\n", codec.c_str(), comment.c_str(), averageMS, iterations);
}

void benchmark(dicomcodecs::image& image, const std::string& comment, const std::string& codec, size_t iterations)
{
    benchmarkEncode(image, comment, codec, iterations);
    benchmarkDecode(image, comment, codec, iterations);
}
