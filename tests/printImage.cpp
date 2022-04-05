#include <dicomcodecs/image.hpp>
#include <stdio.h>

using namespace dicomcodecs;

void printImage(const image &image) {
  printf("width = %d\n", image.width);
  printf("height = %d\n", image.height);
  printf("bitsPerSample = %d\n", image.bitsPerSample);
  printf("componentCount = %d\n", image.componentCount);
  printf("isSigned = %d\n", image.isSigned);
  printf("rawBytes.size() = %lu\n", image.rawBytes.size());
}
