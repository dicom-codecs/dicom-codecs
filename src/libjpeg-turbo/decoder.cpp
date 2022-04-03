#include <stdio.h>
#include "../../extern/libjpeg-turbo/turbojpeg.h"

#include <dicomcodecs/image.hpp>
#include <dicomcodecs/exception.hpp>

void libjpegturbodecoder(const std::vector<uint8_t> & encodedBytes, dicomcodecs::image& targetImage) {
    tjhandle tjInstance = NULL;
    if ((tjInstance = tjInitDecompress()) == NULL) {
        const char* error = tjGetErrorStr2(tjInstance);
        //printf("tjInitDecompress() failed: %s\n", error);
        throw dicomcodecs::exception("libjpegturbo", error);
    }
    
    int width, height, inSubsamp, inColorspace;
    unsigned long jpegSize = (unsigned long)encodedBytes.size();

    int result = tjDecompressHeader3(tjInstance, encodedBytes.data(), jpegSize, &width, &height,
                            &inSubsamp, &inColorspace);
    if(result < 0) {
        const char* error = tjGetErrorStr2(tjInstance);
        throw dicomcodecs::exception("libjpegturbo", error);
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
        const char* error = tjGetErrorStr2(tjInstance);
        throw dicomcodecs::exception("libjpegturbo", error);
    }

    tjDestroy(tjInstance);
 }
    
