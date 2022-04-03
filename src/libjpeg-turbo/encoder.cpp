#include <stdio.h>
#include "../../extern/libjpeg-turbo/turbojpeg.h"
#include <dicomcodecs/image.hpp>

void libjpegturboencoder(const dicomcodecs::image& sourceImage, std::vector<uint8_t> & encodedBytes) {
    // HACK: presize the encoded buffer to the decoded size to make sure we have
    // enough space for the resulting image
    encodedBytes.resize(sourceImage.rawBytes.size());
    tjhandle tjInstance = NULL;
    if ((tjInstance = tjInitCompress()) == NULL) {
        const char* error = tjGetErrorStr2(tjInstance);
        printf("tjInitCompress() failed: %s\n", error);
        throw("initializing compressor");
    }

    int progressive_(1);
    int quality_(100);
    int pixelFormat = sourceImage.componentCount == 1 ? TJPF_GRAY : TJPF_RGB;
    int outSubsamp = sourceImage.componentCount == 1 ? TJSAMP_GRAY : TJSAMP_444;
    int flags = 0;
    unsigned char* jpegBuf = NULL;
    unsigned long jpegSize = 0;

    
    if(progressive_) {
        flags |= TJFLAG_PROGRESSIVE;
    }

    flags |= TJFLAG_NOREALLOC;
    jpegBuf = encodedBytes.data();
    jpegSize = encodedBytes.size();

    if (tjCompress2(tjInstance, sourceImage.rawBytes.data(), sourceImage.width, 0, sourceImage.height, pixelFormat,
                    &jpegBuf, &jpegSize, outSubsamp, quality_, flags) < 0) {
        const char* error = tjGetErrorStr2(tjInstance);
        printf("tjCompress2() failed: %s\n", error);

      throw("compressing image");
    }

    encodedBytes.resize(jpegSize);

    tjDestroy(tjInstance);  tjInstance = NULL;
  
  }