#include "../../include/dicomcodecs/exception.hpp"
#include "../../include/dicomcodecs/image.hpp"
#include "../../include/dicomcodecs/point.hpp"
#include "../../include/dicomcodecs/size.hpp"
#include "encodedBuffer.hpp"
#include <ojph_arch.h>
#include <ojph_codestream.h>
#include <ojph_file.h>
#include <ojph_mem.h>
#include <ojph_params.h>

using namespace dicomcodecs;
using namespace std;

static size_t calculateDecompositions(const image &sourceImage)
{
  float width = sourceImage.width;
  float height = sourceImage.height;
  size_t decompositions = 0;
  while (width > 64.0f && height > 64.0f)
  {
    decompositions++;
    width = ceil(width / 2);
    height = ceil(height / 2);
  }

  return decompositions;
}

void openjph_encoder(const image &sourceImage, vector<uint8_t> &encodedBytes)
{
  EncodedBuffer encoded_;
  encoded_.open();
  std::vector<dicomcodecs::point> downSamples_;
  downSamples_.resize(sourceImage.componentCount);
  for (int c = 0; c < sourceImage.componentCount; ++c)
  {
    downSamples_[c].x = 1;
    downSamples_[c].y = 1;
  }

  // Setup image size parameters
  ojph::codestream codestream;
  ojph::param_siz siz = codestream.access_siz();
  siz.set_image_extent(ojph::point(sourceImage.width, sourceImage.height));
  int num_comps = sourceImage.componentCount;
  siz.set_num_components(num_comps);

  size_t decompositions_ = calculateDecompositions(sourceImage);
  bool lossless_(true);
  float quantizationStep_(-1.0);
  size_t progressionOrder_(2);
  dicomcodecs::point imageOffset_;
  dicomcodecs::size tileSize_;
  dicomcodecs::point tileOffset_;
  dicomcodecs::size blockDimensions_(64, 64);
  for (int c = 0; c < num_comps; ++c)
  {
    siz.set_component(c, ojph::point(downSamples_[c].x, downSamples_[c].y),
                      sourceImage.bitsPerSample, sourceImage.isSigned);
  }
  siz.set_image_offset(ojph::point(imageOffset_.x, imageOffset_.y));
  siz.set_tile_size(ojph::size(tileSize_.width, tileSize_.height));
  siz.set_tile_offset(ojph::point(tileOffset_.x, tileOffset_.y));

  // Setup encoding parameters
  ojph::param_cod cod = codestream.access_cod();
  cod.set_num_decomposition(decompositions_);
  cod.set_block_dims(blockDimensions_.width, blockDimensions_.height);
  std::vector<ojph::size> precincts;
  std::vector<dicomcodecs::size> precincts_;
  precincts.resize(precincts_.size());
  for (size_t i = 0; i < precincts_.size(); i++)
  {
    precincts[i].w = precincts_[i].width;
    precincts[i].h = precincts_[i].height;
  }
  cod.set_precinct_size(precincts_.size(), precincts.data());
  bool isUsingColorTransform_ = false;

  const char *progOrders[] = {"LRCP", "RLCP", "RPCL", "PCRL", "CPRL"};
  cod.set_progression_order(progOrders[progressionOrder_]);
  cod.set_color_transform(isUsingColorTransform_);
  cod.set_reversible(lossless_);
  if (!lossless_)
  {
    codestream.access_qcd().set_irrev_quant(quantizationStep_);
  }
  codestream.set_planar(isUsingColorTransform_ == false);
  codestream.write_headers(&encoded_);

  // Encode the image
  const size_t bytesPerPixel = (sourceImage.bitsPerSample + 8 - 1) / 8;
  ojph::ui32 next_comp;
  ojph::line_buf *cur_line = codestream.exchange(NULL, next_comp);
  siz = codestream.access_siz();
  int height = siz.get_image_extent().y - siz.get_image_offset().y;
  for (size_t y = 0; y < height; y++)
  {
    for (size_t c = 0; c < siz.get_num_components(); c++)
    {
      int *dp = cur_line->i32;
      if (sourceImage.bitsPerSample <= 8)
      {
        uint8_t *pIn = (uint8_t *)(sourceImage.rawBytes.data() +
                                   (y * sourceImage.width * bytesPerPixel *
                                    siz.get_num_components()) +
                                   c);
        for (size_t x = 0; x < sourceImage.width; x++)
        {
          *dp++ = *pIn;
          pIn += siz.get_num_components();
        }
      }
      else
      {
        if (sourceImage.isSigned)
        {
          int16_t *pIn = (int16_t *)(sourceImage.rawBytes.data() +
                                     (y * sourceImage.width * bytesPerPixel));
          for (size_t x = 0; x < sourceImage.width; x++)
          {
            *dp++ = *pIn++;
          }
        }
        else
        {
          uint16_t *pIn = (uint16_t *)(sourceImage.rawBytes.data() +
                                       (y * sourceImage.width * bytesPerPixel));
          for (size_t x = 0; x < sourceImage.width; x++)
          {
            *dp++ = *pIn++;
          }
        }
      }
      cur_line = codestream.exchange(cur_line, next_comp);
    }
  }

  // cleanup
  codestream.flush();
  codestream.close();

  encodedBytes = encoded_.getBuffer();
}
