#include <dicomcodecs/codec.hpp>

void roundTrip(const dicomcodecs::image& image, const std::string& codec, double maxAverageDiff = 0.0);
std::string readRawFile(const std::string filePath, dicomcodecs::image& image);

void readRawFileAndRoundTrip(const std::string& fileName, const std::string& codec, double maxAverageDiff = 0.0) {
    try {
        printf("------BEGIN\n");
        dicomcodecs::image image;
        readRawFile(fileName, image);
        roundTrip(image, codec, maxAverageDiff);
        printf("------END\n");
    }
    catch(const char* err) {
        printf("EXCEPTION: %s\n", err);
    }
}
