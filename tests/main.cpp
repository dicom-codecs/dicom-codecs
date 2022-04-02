#include <dicomcodecs/codec.hpp>

void readFile(std::string fileName, std::vector<uint8_t>& vec);
void printImage(const dicomcodecs::image& image);
void readFileAndDecode(const std::string& fileName, const std::string& codec, dicomcodecs::image& image);

int main(int argc, char** argv) 
{
    dicomcodecs::image image;
    init();

    readFileAndDecode("../../chafey/charls-js/test/fixtures/jls/CT1.JLS", "charls", image);
    std::vector<uint8_t> encodedBytes;
    encode(image, encodedBytes, "charls");
    printf("encoded buffer size = %lu bytes\n", encodedBytes.size());
    dicomcodecs::image decodedImage;
    decode(encodedBytes, decodedImage, "charls");
    if(decodedImage != image) {
        printf("ERROR: encoder did not produce original image");
    }


    //readFileAndDecode("../../chafey/openjpegjs/test/fixtures/j2k/CT1.j2k", "openjpeg", image);
    //readFileAndDecode("extern/libjpeg-turbo/testimages/testorig.jpg", "libjpeg-turbo", image);
    //readFileAndDecode("/Users/chafey/src/github/dicom-codecs/ijg-old/tests/test12.jpg", "ijg12", image);

    //readFileAndDecode("../../chafey/openjphjs/test/fixtures/j2c/CT1.j2c", "openjpeg", image); // HTJ2K


    return 0;
}