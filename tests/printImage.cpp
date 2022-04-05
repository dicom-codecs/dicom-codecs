#include <dicomcodecs/image.hpp>
#include <stdio.h>

void printImage(const dicomcodecs::image& image) {
    printf("width = %d\n", image.width);
    printf("height = %d\n", image.height);
    printf("bitsPerSample = %d\n", image.bitsPerSample);
    printf("componentCount = %d\n", image.componentCount);
    printf("isSigned = %d\n", image.isSigned);
    printf("rawBytes.size() = %lu\n", image.rawBytes.size());
}
