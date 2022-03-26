#include <charls/charls.h>
#include "dicomcodecs/image.hpp"

void charlsdecoder(const std::vector<uint8_t> & encodedBytes, dicomcodecs::image& targetImage) {
    charls::jpegls_decoder decoder;
    decoder.source(encodedBytes);
    
    decoder.read_header();

    int32_t nearLossless_ = decoder.near_lossless();
    charls::interleave_mode interleaveMode_ = decoder.interleave_mode();
    charls::frame_info frameInfo = decoder.frame_info();
    targetImage.width = frameInfo.width;
    targetImage.height = frameInfo.height;
    targetImage.bitsPerSample = frameInfo.bits_per_sample;
    targetImage.componentCount = frameInfo.component_count;
    const size_t destination_size{decoder.destination_size()};
    targetImage.rawBytes.resize(destination_size);
    decoder.decode(targetImage.rawBytes);
}
    
