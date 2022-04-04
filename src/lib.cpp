#include <vector>
#include <dicomcodecs/codec.hpp>
#include <string>
#include <map>

void charlsdecoder(const std::vector<uint8_t> & encodedBytes, dicomcodecs::image& targetImage);
void charlsencoder(const dicomcodecs::image& sourceImage, std::vector<uint8_t> & encodedBytes);

void openjpegdecoder(const std::vector<uint8_t> & encodedBytes, dicomcodecs::image& targetImage);
void openjpegencoder(const dicomcodecs::image& sourceImage, std::vector<uint8_t> & encodedBytes);

void libjpegturbodecoder(const std::vector<uint8_t> & encodedBytes, dicomcodecs::image& targetImage);
void libjpegturboencoder(const dicomcodecs::image& sourceImage, std::vector<uint8_t> & encodedBytes);

void ijg12_decode(const std::vector<uint8_t> & encodedBytes, dicomcodecs::image& targetImage);
void ijg12_encode(const dicomcodecs::image& sourceImage, std::vector<uint8_t> & encodedBytes);

std::map<std::string, decoder_ptr> decoders;
std::map<std::string, encoder_ptr> encoders;

int registerDecoder(const std::string codec, decoder_ptr decoder) {
    printf("decoder registered for %s\n", codec.c_str());
    decoders[codec] = decoder;
    return 0;
}

int registerEncoder(const std::string codec, encoder_ptr encoder) {
    printf("encoder registered for %s\n", codec.c_str());
    encoders[codec] = encoder;
    return 0;
}

void init() {

#ifdef DICOM_CODECS_BUILD_CHARLS
    registerDecoder("charls", charlsdecoder);
    registerEncoder("charls", charlsencoder);
#endif
#ifdef DICOM_CODECS_BUILD_OPENJPEG
    registerDecoder("openjpeg", openjpegdecoder);
    registerEncoder("openjpeg", openjpegencoder);
#endif
#ifdef DICOM_CODECS_BUILD_LIBJPEGTURBO
    registerDecoder("libjpeg-turbo", libjpegturbodecoder);
    registerEncoder("libjpeg-turbo", libjpegturboencoder);
#endif
#ifdef DICOM_CODECS_BUILD_IJG12
    registerDecoder("ijg12", ijg12_decode);
    registerEncoder("ijg12", ijg12_encode);
#endif
}

void decode(const std::vector<uint8_t> & encodedBytes, dicomcodecs::image& targetImage, const std::string& codec) {
    decoder_ptr decoder = decoders[codec];
    if(decoder == 0) {
        throw "Unknown codec";
    }
    return (*decoder)(encodedBytes, targetImage);
}

void encode(const dicomcodecs::image& sourceImage, std::vector<uint8_t> & encodedBytes, const std::string& codec) {
    encoder_ptr encoder = encoders[codec];
    if(encoder == 0) {
        throw "Unknown codec";
    }
    return (*encoder)(sourceImage, encodedBytes);
}