#include "../../include/dicomcodecs/exception.hpp"
#include "../../include/dicomcodecs/image.hpp"
#include "../../include/dicomcodecs/point.hpp"
#include "../../include/dicomcodecs/size.hpp"
#include <limits.h>
// Kakadu core includes
#include "kdu_elementary.h"
#include "kdu_messaging.h"
#include "kdu_params.h"
#include "kdu_compressed.h"
#include "kdu_sample_processing.h"
#include "kdu_block_coding.h" // So we can display capabilities with -version
#include "kdu_arch.h"
// Application includes
#include "kdu_file_io.h"
#include "jpx.h"

#include "kdu_utils.h" // Access `kdu_memsafe_mul' etc. for safe mem calcs

#include "jp2.h"
#include "jpx.h"
#include <iostream>

#include "kdu_stripe_decompressor.h"

using namespace dicomcodecs;
using namespace std;
using namespace kdu_supp; // Also includes the `kdu_core' namespace
class kdu_stream_message : public kdu_thread_safe_message
{
public: // Member classes
    kdu_stream_message(std::ostream *stream)
    {
        this->stream = stream;
    }
    void put_text(const char *string)
    {
        (*stream) << string;
    }
    void flush(bool end_of_message = false)
    {
        stream->flush();
        kdu_thread_safe_message::flush(end_of_message);
    }

private: // Data
    std::ostream *stream;
};

static kdu_stream_message cout_message(&std::cout);
static kdu_stream_message cerr_message(&std::cerr);
static kdu_message_formatter pretty_cout(&cout_message);
static kdu_message_formatter pretty_cerr(&cerr_message);

void decode_(kdu_core::kdu_codestream &codestream, kdu_core::kdu_compressed_source_buffered &input, size_t decompositionLevel, image &targetImage)
{
    kdu_core::siz_params *siz = codestream.access_siz();
    kdu_core::kdu_params *cod = siz->access_cluster(COD_params);
    // printf("cod=%p\n", cod);
    // cod->get(Clevels, 0, 0, (int &)numDecompositions_);
    // cod->get(Corder, 0, 0, (int &)progressionOrder_);
    // cod->get(Creversible, 0, 0, isReversible_);
    // cod->get(Cblk, 0, 0, (int &)blockDimensions_.height);
    // cod->get(Cblk, 0, 1, (int &)blockDimensions_.width);

    // isHTEnabled_ = codestream.get_ht_usage();
    const size_t bytesPerPixel = (targetImage.bitsPerSample + 8 - 1) / 8;
    // printf("targetImage.bitsPerSample = %d, bytesPerPixel = %d\n", targetImage.bitsPerSample, bytesPerPixel);
    //  Now decompress the image in one hit, using `kdu_stripe_decompressor'
    size_t num_samples = kdu_core::kdu_memsafe_mul(targetImage.componentCount,
                                                   kdu_core::kdu_memsafe_mul(targetImage.width,
                                                                             targetImage.height));
    targetImage.rawBytes.resize(num_samples * bytesPerPixel);
    // printf("targetImage.rawBytes.size = %d\n", targetImage.rawBytes.size());
    kdu_core::kdu_byte *buffer = targetImage.rawBytes.data();
    kdu_supp::kdu_stripe_decompressor decompressor;
    decompressor.start(codestream);
    const int stripe_heights[] = {targetImage.height, targetImage.height, targetImage.height};
    const bool is_signed[3] = {targetImage.isSigned, targetImage.isSigned, targetImage.isSigned};
    const int precisions[3] = {targetImage.bitsPerSample, targetImage.bitsPerSample, targetImage.bitsPerSample};
    if (bytesPerPixel == 1)
    {
        decompressor.pull_stripe((kdu_core::kdu_byte *)buffer, stripe_heights);
    }
    else
    {
        auto result = decompressor.pull_stripe(
            (kdu_core::kdu_int16 *)buffer,
            (const int *)stripe_heights,
            (const int *)NULL,        // sample_offsets
            (const int *)NULL,        // sample_gaps
            (const int *)NULL,        // row_gaps
            (const int *)&precisions, // precisions
            (const bool *)&is_signed, //
            (const int *)NULL,        // pad_flags
            0);                       // vectorized_store_prefs
    }
    decompressor.finish();
}

void kakadu_init()
{
    kdu_core::kdu_customize_warnings(&pretty_cout);
    kdu_core::kdu_customize_errors(&pretty_cerr);
}

void kakadu_decoder(const vector<uint8_t> &jphBytes, image &targetImage)
{
    kakadu_init();

    kdu_compressed_source_buffered source((kdu_byte *)jphBytes.data(), jphBytes.size());
    jp2_family_src jp2_ultimate_src;
    jp2_ultimate_src.open(&source);
    jpx_source jpx_in;
    if (jpx_in.open(&jp2_ultimate_src, true) < 0)
    {
        throw "Not compatible with JP2 or JPX";
    }

    int jpx_layer_idx = 0;
    jpx_layer_source jpx_layer = jpx_in.access_layer(jpx_layer_idx);
    if (!jpx_layer)
    {
        throw "Unable to find the composing layer";
    }
    kdu_compressed_source *input = &source; // jpx_stream.open_stream();

    // Create the codestream object.
    kdu_core::kdu_codestream codestream;
    codestream.create(input);

    // Determine number of components to decompress
    kdu_core::kdu_dims dims;
    codestream.get_dims(0, dims);

    int num_components = codestream.get_num_components();
    if (num_components == 2)
        num_components = 1;
    else if (num_components >= 3)
    { // Check that components have consistent dimensions
        num_components = 3;
        kdu_core::kdu_dims dims1;
        codestream.get_dims(1, dims1);
        kdu_core::kdu_dims dims2;
        codestream.get_dims(2, dims2);
        if ((dims1 != dims) || (dims2 != dims))
            num_components = 1;
    }
    codestream.apply_input_restrictions(0, num_components, 0, 0, NULL);
    targetImage.width = dims.size.x;
    targetImage.height = dims.size.y;
    targetImage.componentCount = num_components;
    targetImage.bitsPerSample = codestream.get_bit_depth(0);
    targetImage.isSigned = codestream.get_signed(0);

    decode_(codestream, source, 0, targetImage);
    codestream.destroy();
    input->close();
}
