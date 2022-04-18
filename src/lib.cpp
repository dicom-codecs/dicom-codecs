#include <map>
#include <string>
#include <vector>

#include <dicomcodecs/codec.hpp>
#include <dicomcodecs/exception.hpp>

using namespace dicomcodecs;
using namespace std;

void charlsdecoder(const vector<uint8_t> &encodedBytes, image &targetImage);
void charlsencoder(const image &sourceImage, vector<uint8_t> &encodedBytes);

void openjpegdecoder(const vector<uint8_t> &encodedBytes, image &targetImage);
void openjpegencoder(const image &sourceImage, vector<uint8_t> &encodedBytes);

void libjpegturbo8decoder(const vector<uint8_t> &encodedBytes,
                          image &targetImage);
void libjpegturbo8encoder(const image &sourceImage,
                          vector<uint8_t> &encodedBytes);

void ijg12_decode(const vector<uint8_t> &encodedBytes, image &targetImage);
void ijg12_encode(const image &sourceImage, vector<uint8_t> &encodedBytes);

void gdcm_decode(const vector<uint8_t> &encodedBytes, image &targetImage);
void gdcm_encode(const image &sourceImage, vector<uint8_t> &encodedBytes);

void rledecoder(const vector<uint8_t> &encodedBytes, image &targetImage);
void rleencoder(const image &sourceImage, vector<uint8_t> &encodedBytes);

void ljpeg6b16_decode(const vector<uint8_t> &encodedBytes, image &targetImage);
void ljpeg6b16_encode(const image &sourceImage, vector<uint8_t> &encodedBytes);

void gdcmjpeg16decoder(const vector<uint8_t> &encodedBytes, image &targetImage);
void gdcmjpeg16encoder(const image &sourceImage, vector<uint8_t> &encodedBytes);

void openjph_decoder(const vector<uint8_t> &encodedBytes, image &targetImage);
void openjph_encoder(const image &sourceImage, vector<uint8_t> &encodedBytes);

map<string, decoder_ptr> decoders;
map<string, encoder_ptr> encoders;

int registerDecoder(const string codec, decoder_ptr decoder) {
  printf("decoder registered for %s\n", codec.c_str());
  decoders[codec] = decoder;
  return 0;
}

int registerEncoder(const string codec, encoder_ptr encoder) {
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
#ifdef DICOM_CODECS_BUILD_LIBJPEGTURBO8
  registerDecoder("libjpeg-turbo8", libjpegturbo8decoder);
  registerEncoder("libjpeg-turbo8", libjpegturbo8encoder);
#endif
#ifdef DICOM_CODECS_BUILD_IJG12
  registerDecoder("ijg12", ijg12_decode);
  registerEncoder("ijg12", ijg12_encode);
#endif
#ifdef DICOM_CODECS_BUILD_LJPEG6B16
  registerDecoder("ljpeg6b16", ljpeg6b16_decode);
  registerEncoder("ljpeg6b16", ljpeg6b16_encode);
#endif
#ifdef DICOM_CODECS_BUILD_RLE
  registerDecoder("rle", rledecoder);
  registerEncoder("rle", rleencoder);
#endif
#ifdef DICOM_CODECS_GDCM_JPEG16
  registerDecoder("gdcm-jpeg16", gdcmjpeg16decoder);
  registerEncoder("gdcm-jpeg16", gdcmjpeg16encoder);
#endif

#ifdef DICOM_CODECS_OPENJPH
  registerDecoder("openjph", openjph_decoder);
  registerEncoder("openjph", openjph_encoder);
#endif
}

void decode(const vector<uint8_t> &encodedBytes, image &targetImage,
            const string &codec) {
  decoder_ptr decoder = decoders[codec];
  if (decoder == 0) {
    throw dicom_codec_exception(codec.c_str(), "Unknown codec");
  }

  return (*decoder)(encodedBytes, targetImage);
}

void encode(const image &sourceImage, vector<uint8_t> &encodedBytes,
            const string &codec) {
  encoder_ptr encoder = encoders[codec];
  if (encoder == 0) {
    throw dicom_codec_exception(codec.c_str(), "Unknown codec");
  }

  return (*encoder)(sourceImage, encodedBytes);
}
