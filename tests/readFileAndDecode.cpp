#include <string>
#include <vector>
#include <dicomcodecs/image.hpp>
#include <dicomcodecs/codec.hpp>
#include <dicomcodecs/exception.hpp>

void readFile(std::string fileName, std::vector<uint8_t>& vec);
void printImage(const dicomcodecs::image& image);

void readFileAndDecode(const std::string& fileName, const std::string& codec, dicomcodecs::image& image)
{
    try {
        // initialize image to defaults
        image = dicomcodecs::image();
        std::vector<uint8_t> encodedBytes;
        readFile(fileName, encodedBytes);
        decode(encodedBytes, image, codec);
    }
    catch(dicomcodecs::exception& ex) {
        printf("EXCEPTION: %s from codec %s while decoding file %s", ex.reason(), ex.codec(), fileName.c_str());
    }
    catch(const char* reason) {
        printf("Decoding file %s with codec %s\n", fileName.c_str(), codec.c_str());
        printImage(image);
        printf("EXCEPTION: %s\n", reason);
    }
}
