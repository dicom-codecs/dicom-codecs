#include <dicomcodecs/codec.hpp>

void readFile(std::string fileName, std::vector<uint8_t>& vec);
void printImage(const dicomcodecs::image& image);
void readFileAndDecode(const std::string& fileName, const std::string& codec, dicomcodecs::image& image);
void roundTrip(const dicomcodecs::image& image, const std::string& codec, double maxAverageDiff = 0.0);
void readFileAndRoundTrip(const std::string& fileName, const std::string& codec, double maxAverageDiff = 0.0);


int main(int argc, char** argv) 
{
    init();
    readFileAndRoundTrip("extern/test-data/jpegls/CT1.JLS", "charls");
    readFileAndRoundTrip("extern/test-data/jpeg2000/CT1.j2k", "openjpeg");
    readFileAndRoundTrip("extern/test-data/jpeglossy8bit/jpeg400jfif.jpg", "libjpeg-turbo", 2.0);
    readFileAndRoundTrip("extern/test-data/jpeglossy12bit/test12.jpg", "ijg12", 2.0);



    //readFileAndDecode("../../chafey/openjphjs/test/fixtures/j2c/CT1.j2c", "openjpeg", image); // HTJ2K


    return 0;
}