#include <vector>
#include <dicomcodecs/codec.hpp>
#include <string>

void charlsdecoder(const std::vector<uint8_t> & encodedBytes, dicomcodecs::image& targetImage);
void openjpegdecoder(const std::vector<uint8_t> & encodedBytes, dicomcodecs::image& targetImage);
void libjpegturbodecoder(const std::vector<uint8_t> & encodedBytes, dicomcodecs::image& targetImage);
void ijg12_decode(const std::vector<uint8_t> & encodedBytes, dicomcodecs::image& targetImage);

void decode(const std::vector<uint8_t> & encodedBytes, dicomcodecs::image& targetImage, const std::string& codec) {
    if(codec == "charls") {
        return charlsdecoder(encodedBytes, targetImage);
    } else if(codec == "openjpeg") {
        return openjpegdecoder(encodedBytes, targetImage);
    } else if(codec == "libjpeg-turbo") {
        return libjpegturbodecoder(encodedBytes, targetImage);
    } else if(codec == "ijg") {
        return ijg12_decode(encodedBytes, targetImage);
    }

    throw "Unknown codec";
}