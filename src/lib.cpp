#include <vector>
#include <dicomcodecs/codec.hpp>
#include <string>

void charlsdecoder(const std::vector<uint8_t> & encodedBytes, dicomcodecs::image& targetImage);
void openjpegdecoder(const std::vector<uint8_t> & encodedBytes, dicomcodecs::image& targetImage);
void libjpegturbodecoder(const std::vector<uint8_t> & encodedBytes, dicomcodecs::image& targetImage);
void ijg12_decode(const std::vector<uint8_t> & encodedBytes, dicomcodecs::image& targetImage);

void decode(const std::vector<uint8_t> & encodedBytes, dicomcodecs::image& targetImage, const std::string& codec) {

#ifdef DICOM_CODECS_BUILD_CHARLS
    if(codec == "charls") {
        return charlsdecoder(encodedBytes, targetImage);
    }
#endif
#ifdef DICOM_CODECS_BUILD_OPENJPEG
    if(codec == "openjpeg") {
        return openjpegdecoder(encodedBytes, targetImage);
    }
#endif
#ifdef DICOM_CODECS_BUILD_LIBJPEGTURBO
    if(codec == "libjpeg-turbo") {
        return libjpegturbodecoder(encodedBytes, targetImage);
    } 
#endif
#ifdef DICOM_CODECS_BUILD_IJG
    if(codec == "ijg12") {
        return ijg12_decode(encodedBytes, targetImage);
    }
#endif
    throw "Unknown codec";
}