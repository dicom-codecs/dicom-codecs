#include <charls/charls.h>
#include <dicomcodecs/image.hpp>

using namespace charls;
using namespace dicomcodecs;
using namespace std;

void charlsdecoder(const vector<uint8_t> &encodedBytes, image &targetImage) {
  jpegls_decoder decoder;
  decoder.source(encodedBytes);
  decoder.read_header();
  int32_t nearLossless_ = decoder.near_lossless();
  interleave_mode interleaveMode_ = decoder.interleave_mode();
  frame_info frameInfo = decoder.frame_info();
  targetImage.width = frameInfo.width;
  targetImage.height = frameInfo.height;
  targetImage.bitsPerSample = frameInfo.bits_per_sample;
  targetImage.componentCount = frameInfo.component_count;
  const size_t destination_size{decoder.destination_size()};
  targetImage.rawBytes.resize(destination_size);
  decoder.decode(targetImage.rawBytes);
}
