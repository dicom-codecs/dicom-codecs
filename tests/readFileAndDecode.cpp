#include <string>
#include <vector>
#include <dicomcodecs/image.hpp>
#include <dicomcodecs/codec.hpp>

void readFile(std::string fileName, std::vector<uint8_t>& vec);
void printImage(const dicomcodecs::image& image);

void readFileAndDecode(const std::string& fileName, const std::string& codec, dicomcodecs::image& image)
{
    // initialize image to defaults
    image = dicomcodecs::image();
    try {
        std::vector<uint8_t> encodedBytes;
        readFile(fileName, encodedBytes);
        printf("------------------\n");
        printf("Decoding file %s with codec %s\n", fileName.c_str(), codec.c_str());
        decode(encodedBytes, image, codec);
        printImage(image);
    }
    catch(const char* reason) {
        printf("EXCEPTION: %s\n", reason);
    }

}
