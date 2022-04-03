#include <dicomcodecs/codec.hpp>

void roundTrip(const dicomcodecs::image& image, const std::string& codec, double maxAverageDiff = 0.0);
void readFileAndDecode(const std::string& fileName, const std::string& codec, dicomcodecs::image& image);

void readFileAndRoundTrip(const std::string& fileName, const std::string& codec, double maxAverageDiff = 0.0) {
    try {
        dicomcodecs::image image;
        readFileAndDecode(fileName, codec, image);
        roundTrip(image, codec, maxAverageDiff);
    }
    catch(const char* err) {
        printf("EXCEPTION: %s\n", err);
    }
}
