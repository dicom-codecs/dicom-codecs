#include <stdio.h>
#include "../../extern/libjpeg-turbo/turbojpeg.h"

#include <dicomcodecs/image.hpp>

void libjpegturbodecoder(const std::vector<uint8_t> & encodedBytes, dicomcodecs::image& targetImage) {
    tjhandle tjInstance = NULL;
    if ((tjInstance = tjInitDecompress()) == NULL) {
        throw("initializing decompressor\n");
    }
    
    int width, height, inSubsamp, inColorspace;
    unsigned long jpegSize = (unsigned long)encodedBytes.size();

    int result = tjDecompressHeader3(tjInstance, encodedBytes.data(), jpegSize, &width, &height,
                            &inSubsamp, &inColorspace);
    if(result < 0) {
        printf("tjDecompressHeader3() returned %d\n", result);
        throw("reading header\n");
    }

    targetImage.width = width;
    targetImage.height = height;
    targetImage.bitsPerSample = 8;
    targetImage.isSigned = false;
    targetImage.componentCount = inColorspace == 2 ? 1 : 3;

    int pixelFormat = (targetImage.componentCount == 1) ? TJPF_GRAY : TJPF_RGB;

    const size_t destinationSize = targetImage.width * targetImage.height * tjPixelSize[pixelFormat];
    targetImage.rawBytes.resize(destinationSize);

    if (tjDecompress2(tjInstance, encodedBytes.data(), encodedBytes.size(), targetImage.rawBytes.data(), 
        targetImage.width, 0, targetImage.height, pixelFormat, 0) < 0) {
        tjDestroy(tjInstance);
        throw("decompressing JPEG image\n");
    }

    tjDestroy(tjInstance);
 }
    
