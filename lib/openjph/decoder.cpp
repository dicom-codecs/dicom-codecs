#include "../../include/dicomcodecs/exception.hpp"
#include "../../include/dicomcodecs/image.hpp"
#include "../../include/dicomcodecs/point.hpp"
#include "../../include/dicomcodecs/size.hpp"
#include <ojph_arch.h>
#include <ojph_file.h>
#include <ojph_mem.h>
#include <ojph_params.h>
#include <ojph_codestream.h>
#include <limits.h>

using namespace dicomcodecs;
using namespace std;

static dicomcodecs::size calculateSizeAtDecompositionLevel(const dicomcodecs::image& image, int decompositionLevel)
  {
    dicomcodecs::size result(image.width, image.height);
    while (decompositionLevel > 0)
    {
      result.width = ojph_div_ceil(result.width, 2);
      result.height = ojph_div_ceil(result.height, 2);
      decompositionLevel--;
    }
    return result;
  }



void openjph_decoder(const vector<uint8_t> &encodedBytes, image &targetImage) {
    ojph::codestream codestream;
    ojph::mem_infile mem_file;
    mem_file.open(encodedBytes.data(), encodedBytes.size());

    codestream.enable_resilience();
    codestream.read_headers(&mem_file);
    ojph::param_siz siz = codestream.access_siz();
    targetImage.width = siz.get_image_extent().x - siz.get_image_offset().x;
    targetImage.height = siz.get_image_extent().y - siz.get_image_offset().y;
    targetImage.componentCount = siz.get_num_components();
    targetImage.bitsPerSample = siz.get_bit_depth(0);
    targetImage.isSigned = siz.is_signed(0);
    std::vector<dicomcodecs::point> downSamples_;
    downSamples_.resize(targetImage.componentCount);
    for (size_t i = 0; i < targetImage.componentCount; i++)
    {
      downSamples_[i].x = siz.get_downsampling(i).x;
      downSamples_[i].y = siz.get_downsampling(i).y;
    }
    dicomcodecs::point imageOffset_;
    dicomcodecs::size tileSize_;
    dicomcodecs::point tileOffset_;
    dicomcodecs::size blockDimensions_;
    std::vector<dicomcodecs::size> precincts_;
    int32_t numLayers_;
    bool isUsingColorTransform_;
    imageOffset_.x = siz.get_image_offset().x;
    imageOffset_.y = siz.get_image_offset().y;
    tileSize_.width = siz.get_tile_size().w;
    tileSize_.height = siz.get_tile_size().h;

    tileOffset_.x = siz.get_tile_offset().x;
    tileOffset_.y = siz.get_tile_offset().y;

    ojph::param_cod cod = codestream.access_cod();
    size_t numDecompositions_ = cod.get_num_decompositions();
    bool isReversible_ = cod.is_reversible();
    size_t progressionOrder_ = cod.get_progression_order();
    blockDimensions_.width = cod.get_block_dims().w;
    blockDimensions_.height = cod.get_block_dims().h;
    precincts_.resize(numDecompositions_);
    for (size_t i = 0; i < numDecompositions_; i++)
    {
      precincts_[i].width = cod.get_precinct_size(i).w;
      precincts_[i].height = cod.get_precinct_size(i).h;
    }
    numLayers_ = cod.get_num_layers();
    isUsingColorTransform_ = cod.is_using_color_transform();

  // calculate the resolution at the requested decomposition level and
    // allocate destination buffer
    int decompositionLevel = 0;
    dicomcodecs::size sizeAtDecompositionLevel = calculateSizeAtDecompositionLevel(targetImage, decompositionLevel);
    int resolutionLevel = numDecompositions_ - decompositionLevel;
    const size_t bytesPerPixel = (targetImage.bitsPerSample + 8 - 1) / 8;
    const size_t destinationSize = sizeAtDecompositionLevel.width * sizeAtDecompositionLevel.height * targetImage.componentCount * bytesPerPixel;
    targetImage.rawBytes.resize(destinationSize);

    // set the level to read to and reconstruction level to the specified decompositionLevel
    codestream.restrict_input_resolution(decompositionLevel, decompositionLevel);

    // parse it
    if (targetImage.componentCount == 1)
    {
      codestream.set_planar(true);
    }
    else
    {
      if (isUsingColorTransform_)
      {
        codestream.set_planar(false);
      }
      else
      {
        // for color images without a color transform,
        // calling set_planar(true) invokes an optimization
        // https://github.com/aous72/OpenJPH/issues/34
        codestream.set_planar(true);
      }
    }
    codestream.create();

    // Extract the data line by line...
    // NOTE: All values must be clamped https://github.com/aous72/OpenJPH/issues/35
    ojph::ui32 comp_num;
    for (int y = 0; y < sizeAtDecompositionLevel.height; y++)
    {
      size_t lineStart = y * sizeAtDecompositionLevel.width * targetImage.componentCount * bytesPerPixel;
      if (targetImage.componentCount == 1)
      {
        ojph::line_buf *line = codestream.pull(comp_num);
        if (targetImage.bitsPerSample <= 8)
        {
          unsigned char *pOut = (unsigned char *)&targetImage.rawBytes[lineStart];
          for (size_t x = 0; x < sizeAtDecompositionLevel.width; x++)
          {
            int val = line->i32[x];
            pOut[x] = std::max(0, std::min(val, UCHAR_MAX));
          }
        }
        else
        {
          if (targetImage.isSigned)
          {
            short *pOut = (short *)&targetImage.rawBytes[lineStart];
            for (size_t x = 0; x < sizeAtDecompositionLevel.width; x++)
            {
              int val = line->i32[x];
              pOut[x] = std::max(SHRT_MIN, std::min(val, SHRT_MAX));
            }
          }
          else
          {
            unsigned short *pOut = (unsigned short *)&targetImage.rawBytes[lineStart];
            for (size_t x = 0; x < sizeAtDecompositionLevel.width; x++)
            {
              int val = line->i32[x];
              pOut[x] = std::max(0, std::min(val, USHRT_MAX));
            }
          }
        }
      }
      else
      {
        for (int c = 0; c < targetImage.componentCount; c++)
        {
          ojph::line_buf *line = codestream.pull(comp_num);
          if (targetImage.bitsPerSample <= 8)
          {
            uint8_t *pOut = &targetImage.rawBytes[lineStart] + c;
            for (size_t x = 0; x < sizeAtDecompositionLevel.width; x++)
            {
              int val = line->i32[x];
              pOut[x * targetImage.componentCount] = std::max(0, std::min(val, UCHAR_MAX));
            }
          }
          else
          {
            // This should work but has not been tested yet
            if (targetImage.isSigned)
            {
              short *pOut = (short *)&targetImage.rawBytes[lineStart] + c;
              for (size_t x = 0; x < sizeAtDecompositionLevel.width; x++)
              {
                int val = line->i32[x];
                pOut[x * targetImage.componentCount] = std::max(SHRT_MIN, std::min(val, SHRT_MAX));
              }
            }
            else
            {
              unsigned short *pOut = (unsigned short *)&targetImage.rawBytes[lineStart] + c;
              for (size_t x = 0; x < sizeAtDecompositionLevel.width; x++)
              {
                int val = line->i32[x];
                pOut[x * targetImage.componentCount] = std::max(0, std::min(val, USHRT_MAX));
              }
            }
          }
        }
      }
    }
}