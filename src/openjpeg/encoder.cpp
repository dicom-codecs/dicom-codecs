#include <exception>
#include <limits.h>
#include <memory>

#include <openjpeg.h>
#include <stdlib.h>
#include <string.h>
#define J2K_MAGIC_NUMBER 0x51FF4FFF

#include "buffer.hpp"
#include <dicomcodecs/image.hpp>
#include <dicomcodecs/point.hpp>
#include <dicomcodecs/size.hpp>

using namespace dicomcodecs;
using namespace std;

static void error_callback(const char *msg, void *client_data) {
  (void)client_data;
  fprintf(stdout, "[ERROR] %s", msg);
}
/**
sample warning debug callback expecting no client object
*/
static void warning_callback(const char *msg, void *client_data) {
  (void)client_data;
  fprintf(stdout, "[WARNING] %s", msg);
}
/**
sample debug callback expecting no client object
*/
static void info_callback(const char *msg, void *client_data) {
  (void)client_data;
  fprintf(stdout, "[INFO] %s", msg);
}

void openjpegencoder(const image &sourceImage, vector<uint8_t> &encodedBytes) {
  size_t decompositions_ = 5;
  bool lossless_ = true;
  vector<float> layerCompressionRatios_;
  size_t progressionOrder_ = 2; // RPCL

  vector<point> downSamples_;
  point imageOffset_;
  dicomcodecs::size tileSize_;
  point tileOffset_;
  dicomcodecs::size blockDimensions_(64, 64);
  vector<dicomcodecs::size> precincts_;

  opj_cparameters_t parameters; /* compression parameters */
  opj_stream_t *l_stream = 00;
  opj_codec_t *l_codec = 00;
  opj_image_t *image = nullptr;

  OPJ_COLOR_SPACE color_space =
      sourceImage.componentCount > 1 ? OPJ_CLRSPC_SRGB : OPJ_CLRSPC_GRAY;

  vector<opj_image_cmptparm_t> cmptparm;
  cmptparm.resize(sourceImage.componentCount);
  /* initialize image components */
  for (int i = 0; i < sourceImage.componentCount; i++) {
    cmptparm[i].prec = (OPJ_UINT32)sourceImage.bitsPerSample;
    cmptparm[i].sgnd = (OPJ_UINT32)sourceImage.isSigned;
    cmptparm[i].dx = 1; //(OPJ_UINT32)(subsampling_dx * raw_cp->rawComps[i].dx);
    cmptparm[i].dy = 1; //(OPJ_UINT32)(subsampling_dy * raw_cp->rawComps[i].dy);
    cmptparm[i].w = (OPJ_UINT32)sourceImage.width;
    cmptparm[i].h = (OPJ_UINT32)sourceImage.height;
  }
  image = opj_image_create((OPJ_UINT32)sourceImage.componentCount,
                           cmptparm.data(), color_space);

  /* set image offset and reference grid */
  image->x0 = (OPJ_UINT32)imageOffset_.x;
  image->y0 = (OPJ_UINT32)imageOffset_.y;
  image->x1 =
      (OPJ_UINT32)sourceImage
          .width; // TODO: revisit logic in terms of subsampling and offsets?
  image->y1 =
      (OPJ_UINT32)sourceImage
          .height; // TODO: revisit logic in terms of subsampling and offsets?

  if (sourceImage.bitsPerSample <= 8) {
    if (sourceImage.componentCount == 1) {
      copy((uint8_t *)sourceImage.rawBytes.data(),
           (uint8_t *)(sourceImage.rawBytes.data() +
                       sourceImage.rawBytes.size()),
           image->comps[0].data);
    } else {
      for (size_t compno = 0; compno < sourceImage.componentCount; compno++) {
        for (size_t i = 0; i < sourceImage.width * sourceImage.height; i++) {
          image->comps[compno].data[i] =
              sourceImage.rawBytes
                  .data()[(i * sourceImage.componentCount) + compno];
        }
      }
    }
  } else if (sourceImage.bitsPerSample <= 16) {
    if (sourceImage.isSigned) {
      copy((short *)sourceImage.rawBytes.data(),
           (short *)(sourceImage.rawBytes.data() + sourceImage.rawBytes.size()),
           image->comps[0].data);
    } else {
      copy((unsigned short *)sourceImage.rawBytes.data(),
           (unsigned short *)(sourceImage.rawBytes.data() +
                              sourceImage.rawBytes.size()),
           image->comps[0].data);
    }
  }

  /* set encoding parameters to default values */
  opj_set_default_encoder_parameters(&parameters);
  parameters.tcp_mct =
      (char)sourceImage.componentCount > 1
          ? 1
          : 0; // disable for grayscale: TODO - set this properly for color
  parameters.prog_order = (OPJ_PROG_ORDER)progressionOrder_;
  parameters.numresolution = decompositions_ + 1;
  parameters.irreversible = !lossless_;

  parameters.tcp_numlayers = layerCompressionRatios_.size();
  for (size_t layer = 0; layer < layerCompressionRatios_.size(); layer++) {
    parameters.tcp_rates[layer] = layerCompressionRatios_[layer];
  }
  parameters.cp_disto_alloc = 1;

  // TODO: add support for JP2 encoding via config parameter
  l_codec = opj_create_compress(OPJ_CODEC_J2K);

  /* catch events using our callbacks and give a local context */
  // opj_set_info_handler(l_codec, info_callback, 00);
  opj_set_warning_handler(l_codec, warning_callback, 00);
  opj_set_error_handler(l_codec, error_callback, 00);

  // TODO: Add support for using tiles?

  if (!opj_setup_encoder(l_codec, &parameters, image)) {
    fprintf(stderr, "failed to encode image: opj_setup_encoder\n");
    opj_destroy_codec(l_codec);
    opj_image_destroy(image);
    return; // TODO: implement error handling
  }

  // HACK: For now - make encoded buffer the same size as decoded so we can
  // avoid messing with BufferStream malloc/free stuff
  encodedBytes.resize(sourceImage.rawBytes.size());

  /* open a byte stream for writing and allocate memory for all tiles */
  opj_buffer_info_t buffer_info;
  buffer_info.buf = encodedBytes.data();
  buffer_info.cur = encodedBytes.data();
  buffer_info.len = encodedBytes.size();
  l_stream = opj_stream_create_buffer_stream(&buffer_info, OPJ_FALSE);

  /* encode the image */
  if (!opj_start_compress(l_codec, image, l_stream)) {
    fprintf(stderr, "failed to encode image: opj_start_compress\n");
    return; // todo: error handling
  }

  if (!opj_encode(l_codec, l_stream)) {
    fprintf(stderr, "failed to encode image: opj_encode\n");
    return; // todo: error handling
  }

  if (!opj_end_compress(l_codec, l_stream)) {
    fprintf(stderr, "failed to encode image: opj_end_compress\n");
    return; // todo: error handling
  }

  encodedBytes.resize(buffer_info.cur - buffer_info.buf);
}
