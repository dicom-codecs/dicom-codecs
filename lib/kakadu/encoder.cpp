#include "../../include/dicomcodecs/exception.hpp"
#include "../../include/dicomcodecs/image.hpp"
#include "../../include/dicomcodecs/point.hpp"
#include "../../include/dicomcodecs/size.hpp"
#include "kdu_elementary.h"
#include "kdu_messaging.h"
#include "kdu_params.h"
#include "kdu_compressed.h"
#include "kdu_sample_processing.h"
#include "kdu_utils.h"
#include "jp2.h"

// Application level includes
#include "kdu_stripe_compressor.h"
using namespace dicomcodecs;
using namespace std;

using namespace kdu_supp; // Also includes the `kdu_core' namespace

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

class kdu_buffer_target : public kdu_core::kdu_compressed_target
{
public: // Member functions
    kdu_buffer_target(std::vector<uint8_t> &encoded) : encoded_(encoded)
    {
        encoded_.resize(0);
    }
    ~kdu_buffer_target() { return; } // Destructor must be virtual
    int get_capabilities() { return KDU_TARGET_CAP_CACHED; }
    bool write(const kdu_core::kdu_byte *buf, int num_bytes)
    {
        const size_t size = encoded_.size();
        encoded_.resize(size + num_bytes);
        memcpy(encoded_.data() + size, buf, num_bytes);
        return true;
    }

private: // Data
    std::vector<uint8_t> &encoded_;
};

void kakadu_encoder(const image &sourceImage, vector<uint8_t> &encodedBytes)
{
    char *ofname = NULL;
    kdu_supp::kdu_membroker *membroker = NULL;

    // resize the encoded buffer so we don't have to keep resizing it
    const size_t bytesPerPixel = (sourceImage.bitsPerSample + 8 - 1) / 8;
    encodedBytes.reserve(sourceImage.width * sourceImage.height * sourceImage.componentCount * bytesPerPixel);
    kdu_buffer_target target(encodedBytes);

    kdu_supp::kdu_compressed_target *output = NULL;
    kdu_supp::jp2_family_tgt jp2_ultimate_tgt;
    kdu_supp::jp2_target jp2_out;
    output = &jp2_out;
    // output = &target;
    //   jp2_ultimate_tgt.open(ofname, membroker);
    jp2_ultimate_tgt.open(&target, membroker);
    jp2_out.open(&jp2_ultimate_tgt);

    siz_params siz;
    // Mark the codestream as one that needs to conform to Part-15, which
    // then alters some defaults -- for example, `Cmodes' then defaults to
    // `Cmodes_HT' (the Part-15 HT block coder).  Part-15 conformance
    // does not prevent simultaneous conformance to other parts, including
    // conformance to Part-1 and Part-2.
    siz.set(Scap, 0, 0, Scap_P15);

    siz.set(Sdims, 0, 0, sourceImage.height);
    siz.set(Sdims, 0, 1, sourceImage.width);
    siz.set(Ssigned, 0, 0, sourceImage.isSigned);
    siz.set(Sprecision, 0, 0, sourceImage.bitsPerSample);
    siz.set(Scomponents, 0, 0, sourceImage.componentCount);
    siz.finalize_all();

    // Construct the `kdu_codestream' object and parse all remaining args
    kdu_core::kdu_codestream codestream;
    codestream.create(&siz, output);
    // Set up any specific coding parameters and finalize them.
    // codestream.access_siz()->parse_string("Cmodes=HT");
    char param[32];
    codestream.access_siz()->parse_string("Creversible=yes");
    codestream.access_siz()->parse_string("Corder=RPCL");
    size_t decompositions = calculateDecompositions(sourceImage);
    snprintf(param, 32, "Clevels=%zu", decompositions);
    codestream.access_siz()->parse_string(param);
    snprintf(param, 32, "Cblk={64,64}");
    codestream.access_siz()->parse_string(param);
    codestream.access_siz()->finalize_all();

    // Do minimal JP2 file initialization, for demonstration purposes
    jp2_dimensions dimensions = jp2_out.access_dimensions();
    dimensions.init(codestream.access_siz());
    dimensions.finalize_compatibility(codestream.access_siz());
    jp2_colour colour = jp2_out.access_colour();
    colour.init((sourceImage.componentCount >= 3) ? JP2_sRGB_SPACE : JP2_sLUM_SPACE);
    kdu_uint32 brand = jp2_out.get_brand();
    jp2_out.write_header();
    jp2_out.open_codestream(true);

    // Now compress the image in one hit, using `kdu_stripe_compressor'
    kdu_supp::kdu_stripe_compressor compressor;
    compressor.mem_configure(membroker);
    compressor.start(codestream);
    int stripe_heights[3] = {sourceImage.height, sourceImage.height, sourceImage.height};
    if (sourceImage.bitsPerSample <= 8)
    {
        compressor.push_stripe(
            (kdu_core::kdu_byte *)sourceImage.rawBytes.data(),
            stripe_heights);
    }
    else
    {
        bool is_signed[3] = {sourceImage.isSigned, sourceImage.isSigned, sourceImage.isSigned};
        int precisions[3] = {sourceImage.bitsPerSample, sourceImage.bitsPerSample, sourceImage.bitsPerSample};
        compressor.push_stripe(
            (kdu_core::kdu_int16 *)sourceImage.rawBytes.data(),
            stripe_heights,
            NULL, // samples_offsets
            NULL, // sample_gaps
            NULL, // row_gaps
            precisions,
            is_signed);
    }

    compressor.finish();

    // Clean up resources
    codestream.destroy();
    output->close();
    jp2_ultimate_tgt.close();
    compressor.reset();
}
