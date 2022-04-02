#include <dicomcodecs/codec.hpp>

void readFile(std::string fileName, std::vector<uint8_t>& vec);
void printImage(const dicomcodecs::image& image);
void readFileAndDecode(const std::string& fileName, const std::string& codec, dicomcodecs::image& image);

void roundTrip(const dicomcodecs::image& image, const std::string& codec) {
    std::vector<uint8_t> encodedBytes;
    encode(image, encodedBytes, codec);
    dicomcodecs::image decodedImage;
    decode(encodedBytes, decodedImage, codec);
    if(decodedImage != image) {
        throw("roundtrip did not produce original image");
    }
}

void readFileAndRoundTrip(const std::string& fileName, const std::string& codec) {
    try {
        dicomcodecs::image image;
        readFileAndDecode(fileName, codec, image);
        roundTrip(image, codec);
    }
    catch(const char* err) {
        printf("EXCEPTION: %s\n", err);
    }
}


int main(int argc, char** argv) 
{
    init();
    readFileAndRoundTrip("../../chafey/charls-js/test/fixtures/jls/CT1.JLS", "charls");
    readFileAndRoundTrip("../../chafey/openjpegjs/test/fixtures/j2k/CT1.j2k", "openjpeg");
    readFileAndRoundTrip("extern/libjpeg-turbo/testimages/testorig.jpg", "libjpeg-turbo");
    //readFileAndDecode("/Users/chafey/src/github/dicom-codecs/ijg-old/tests/test12.jpg", "ijg12", image);

    //readFileAndDecode("../../chafey/openjphjs/test/fixtures/j2c/CT1.j2c", "openjpeg", image); // HTJ2K


    return 0;
}