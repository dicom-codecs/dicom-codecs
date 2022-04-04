#include <string>
#include <vector>
#include <exception>
#include <dicomcodecs/image.hpp>

std::string makeRawFileName(const char* name, const dicomcodecs::image& image) {

    char fileName[128];
    sprintf(fileName, "%s-%d-%d-%d-%d-%d.raw", name, image.width, image.height, image.componentCount, image.bitsPerSample, image.isSigned);
    return fileName;
}