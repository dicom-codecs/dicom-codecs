#include <charls/charls.h>
#include <dicomcodecs/image.hpp>

void charlsencoder(const dicomcodecs::image& sourceImage, std::vector<uint8_t> & encodedBytes) {
    charls::jpegls_encoder encoder;
    encoder.near_lossless(0);
    encoder.frame_info({sourceImage.width, sourceImage.height, sourceImage.bitsPerSample, sourceImage.componentCount})
        .interleave_mode(charls::interleave_mode::none);
    encodedBytes.resize(encoder.estimated_destination_size());
    encoder.destination(encodedBytes);
    const size_t bytes_written{encoder.encode(sourceImage.rawBytes)};
    encodedBytes.resize(bytes_written);
}
