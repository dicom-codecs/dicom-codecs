#include <dicomcodecs/codec.hpp>

void readFile(std::string fileName, std::vector<uint8_t>& vec);
void printImage(const dicomcodecs::image& image);
void readFileAndDecode(const std::string& fileName, const std::string& codec, dicomcodecs::image& image);

int main(int argc, char** argv) 
{
    dicomcodecs::image image;
    std::vector<uint8_t> encodedBytes;

    readFileAndDecode("../../chafey/charls-js/test/fixtures/jls/CT1.JLS", "charls", image);
    readFileAndDecode("../../chafey/openjphjs/test/fixtures/j2c/CT1.j2c", "openjpeg", image);

    return 0;
}