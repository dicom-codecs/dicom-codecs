#include <exception>
#include <limits.h>
#include <memory>

#include <openjpeg.h>
#include <stdlib.h>
#include <string.h>
#define J2K_MAGIC_NUMBER 0x51FF4FFF

#include "buffer.hpp"
#include <dicomcodecs/image.hpp>
#include <dicomcodecs/size.hpp>

using namespace dicomcodecs;
using namespace std;

static void error_callback(const char *msg, void *client_data) {
  (void)client_data;
  printf("[ERROR] %s", msg);
}
static void warning_callback(const char *msg, void *client_data) {
  (void)client_data;
  printf("[WARNING] %s", msg);
}
static void info_callback(const char *msg, void *client_data) {
  (void)client_data;
  printf("[INFO] %s", msg);
}

dicomcodecs::size calculateSizeAtDecompositionLevel(const image &imageFrame,
                                       int decompositionLevel) {
  dicomcodecs::size result(imageFrame.width, imageFrame.height);
  while (decompositionLevel--) {
    result.width = (((result.width + 2) - 1) / 2);
    result.height = (((result.height + 2) - 1) / 2);
  }
  return result;
}

void openjpegdecoder(const vector<uint8_t> &encodedBytes, image &targetImage) {
  size_t decompositionLevel = 0;

  opj_dparameters_t parameters;
  opj_codec_t *l_codec = nullptr;
  opj_image_t *image = nullptr;
  opj_stream_t *l_stream = nullptr;

  // detect stream type
  // NOTE: DICOM only supports OPJ_CODEC_J2K, but not everyone follows this
  // and some DICOM images will have JP2 encoded bitstreams
  // http://dicom.nema.org/medical/dicom/2017e/output/chtml/part05/sect_A.4.4.html
  if (((OPJ_INT32 *)encodedBytes.data())[0] == J2K_MAGIC_NUMBER) {
    l_codec = opj_create_decompress(OPJ_CODEC_J2K);
  } else {
    l_codec = opj_create_decompress(OPJ_CODEC_JP2);
  }

  // opj_set_info_handler(l_codec, info_callback,00);
  opj_set_warning_handler(l_codec, warning_callback, 00);
  opj_set_error_handler(l_codec, error_callback, 00);

  opj_set_default_decoder_parameters(&parameters);
  parameters.cp_reduce = decompositionLevel;
  // parameters.cp_layer = decodeLayer_;

  // opj_set_decoded_resolution_factor(l_codec, 1);
  // set stream
  opj_buffer_info_t buffer_info;
  buffer_info.buf = (OPJ_BYTE *)encodedBytes.data();
  buffer_info.cur = (OPJ_BYTE *)encodedBytes.data();
  buffer_info.len = encodedBytes.size();
  l_stream = opj_stream_create_buffer_stream(&buffer_info, OPJ_TRUE);

  /* Setup the decoder decoding parameters using user parameters */
  if (!opj_setup_decoder(l_codec, &parameters)) {
    printf("[ERROR] opj_decompress: failed to setup the decoder\n");
    opj_stream_destroy(l_stream);
    opj_destroy_codec(l_codec);
    return;
  }
  // disable strict mode so we can partially decode J2K streams
  opj_decoder_set_strict_mode(l_codec, OPJ_FALSE);

  /* Read the main header of the codestream and if necessary the JP2 boxes*/
  if (!opj_read_header(l_stream, l_codec, &image)) {
    printf("[ERROR] opj_decompress: failed to read the header\n");
    opj_stream_destroy(l_stream);
    opj_destroy_codec(l_codec);
    opj_image_destroy(image);
    return;
  }

  /* decode the image */
  if (!opj_decode(l_codec, l_stream, image)) {
    printf("[ERROR] opj_decompress: failed to decode tile!\n");
    opj_destroy_codec(l_codec);
    opj_stream_destroy(l_stream);
    opj_image_destroy(image);
    return;
  }

  targetImage.width = image->x1;
  targetImage.height = image->y1;
  targetImage.componentCount = image->numcomps;
  targetImage.isSigned = image->comps[0].sgnd;
  targetImage.bitsPerSample = image->comps[0].prec;

  // colorSpace_ = image->color_space;
  // imageOffset_.x = image->x0;
  // imageOffset_.y = image->y0;
  // image->comps[0].factor always 0??

  opj_codestream_info_v2_t *cstr_info =
      opj_get_cstr_info(l_codec); /* Codestream information structure */
  // numLayers_ = cstr_info->m_default_tile_info.numlayers;
  // progressionOrder_ = cstr_info->m_default_tile_info.prg;
  // isReversible_ = cstr_info->m_default_tile_info.tccp_info->qmfbid == 1;
  // blockDimensions_.width = 1 <<
  // cstr_info->m_default_tile_info.tccp_info->cblkw; blockDimensions_.height = 1
  // << cstr_info->m_default_tile_info.tccp_info->cblkh; tileOffset_.x =
  // cstr_info->tx0; tileOffset_.y = cstr_info->ty0; tileSize_.width =
  // cstr_info->tdx; tileSize_.height = cstr_info->tdy; numDecompositions_ =
  // cstr_info->m_default_tile_info.tccp_info->numresolutions - 1;

  // calculate the resolution at the requested decomposition level and
  // allocate destination buffer
  dicomcodecs::size sizeAtDecompositionLevel =
      calculateSizeAtDecompositionLevel(targetImage, decompositionLevel);
  const size_t bytesPerPixel = (targetImage.bitsPerSample + 8 - 1) / 8;
  const size_t destinationSize = targetImage.width * targetImage.height *
                                 targetImage.componentCount * bytesPerPixel;
  targetImage.rawBytes.resize(destinationSize);

  // Convert from int32 to native size
  int comp_num;
  for (int y = 0; y < sizeAtDecompositionLevel.height; y++) {
    size_t lineStartPixel = y * sizeAtDecompositionLevel.width;
    size_t lineStart =
        lineStartPixel * targetImage.componentCount * bytesPerPixel;
    if (targetImage.componentCount == 1) {
      int *pIn =
          (int *)&(image->comps[0].data[y * sizeAtDecompositionLevel.width]);
      if (targetImage.bitsPerSample <= 8) {
        unsigned char *pOut = (unsigned char *)&targetImage.rawBytes[lineStart];
        for (size_t x = 0; x < sizeAtDecompositionLevel.width; x++) {
          int val = pIn[x];
          ;
          pOut[x] = max(0, min(val, UCHAR_MAX));
        }
      } else {
        if (targetImage.isSigned) {
          short *pOut = (short *)&targetImage.rawBytes[lineStart];
          for (size_t x = 0; x < sizeAtDecompositionLevel.width; x++) {
            int val = pIn[x];
            ;
            pOut[x] = max(SHRT_MIN, min(val, SHRT_MAX));
          }
        } else {
          unsigned short *pOut =
              (unsigned short *)&targetImage.rawBytes[lineStart];
          for (size_t x = 0; x < sizeAtDecompositionLevel.width; x++) {
            int val = pIn[x];
            ;
            pOut[x] = max(0, min(val, USHRT_MAX));
          }
        }
      }
    } else {
      if (targetImage.bitsPerSample <= 8) {
        uint8_t *pOut = &targetImage.rawBytes[lineStart];
        for (size_t x = 0; x < sizeAtDecompositionLevel.width; x++) {
          pOut[x * 3 + 0] = image->comps[0].data[lineStartPixel + x];
          pOut[x * 3 + 1] = image->comps[1].data[lineStartPixel + x];
          pOut[x * 3 + 2] = image->comps[2].data[lineStartPixel + x];
        }
      } /*else {
        // This should work but has not been tested yet
        if(frameInfo.isSigned) {
          short* pOut = (short*)&decoded_[lineStart] + c;
          for (size_t x = 0; x < sizeAtDecompositionLevel.width; x++) {
            int val = line->i32[x];
            pOut[x * frameInfo.componentCount] = max(SHRT_MIN, min(val,
      SHRT_MAX));
          }
        } else {
          unsigned short* pOut = (unsigned short*)&decoded_[lineStart] + c;
          for (size_t x = 0; x < sizeAtDecompositionLevel.width; x++) {
              int val = line->i32[x];
              pOut[x * frameInfo.componentCount] = max(0, min(val, USHRT_MAX));
          }
        }
      }*/
    }
  }

  opj_stream_destroy(l_stream);
  opj_destroy_codec(l_codec);
  opj_image_destroy(image);
}
