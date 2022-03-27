#include <dicomcodecs/codec.hpp>

void readFile(std::string fileName, std::vector<uint8_t>& vec);
void printImage(const dicomcodecs::image& image);
void readFileAndDecode(const std::string& fileName, const std::string& codec, dicomcodecs::image& image);

int main(int argc, char** argv) 
{
    dicomcodecs::image image;

    readFileAndDecode("extern/test-data/jpegls/CT1.JLS", "charls", image);
    readFileAndDecode("extern/test-data/jpeg2000/CT1.j2k", "openjpeg", image);
    //readFileAndDecode("extern/test-data/j2c/CT1.j2c", "openjpeg", image);
    //readFileAndDecode("extern/test-data/jpeglossy8bit/test8.jpg", "libjpeg-turbo", image);
    readFileAndDecode("extern/test-data/jpeglossy8bit/jpeg400jfif.jpg", "libjpeg-turbo", image);

    //readFileAndDecode("/Users/chafey/src/github/dicom-codecs/ijg-old/tests/test12.jpg", "ijg", image); // does not work due to symbol collision with libturbo-jpeg :(

    return 0;
}