#include "../../extern/libjpeg-turbo/turbojpeg.h"
#include "../../include/dicomcodecs/exception.hpp"
#include "../../include/dicomcodecs/image.hpp"
#include <stdio.h>

using namespace dicomcodecs;
using namespace std;

void libjpegturbo8encoder(const image &sourceImage,
                          vector<uint8_t> &encodedBytes) {
  // HACK: presize the encoded buffer to the decoded size to make sure we have
  // enough space for the resulting image
  encodedBytes.resize(sourceImage.rawBytes.size());
  tjhandle tjInstance = nullptr;
  if ((tjInstance = tjInitCompress()) == nullptr) {
    const char *error = tjGetErrorStr2(tjInstance);
    throw dicom_codec_exception("libjpegturbo", error);
  }

  int progressive_(1);
  int quality_(100);
  int pixelFormat = sourceImage.componentCount == 1 ? TJPF_GRAY : TJPF_RGB;
  int outSubsamp = sourceImage.componentCount == 1 ? TJSAMP_GRAY : TJSAMP_444;
  int flags = 0;
  unsigned char *jpegBuf = nullptr;
  unsigned long jpegSize = 0;

  if (progressive_) {
    flags |= TJFLAG_PROGRESSIVE;
  }

  flags |= TJFLAG_NOREALLOC;
  jpegBuf = encodedBytes.data();
  jpegSize = encodedBytes.size();

  if (tjCompress2(tjInstance, sourceImage.rawBytes.data(), sourceImage.width, 0,
                  sourceImage.height, pixelFormat, &jpegBuf, &jpegSize,
                  outSubsamp, quality_, flags) < 0) {
    const char *error = tjGetErrorStr2(tjInstance);
    throw dicom_codec_exception("libjpegturbo", error);
  }

  encodedBytes.resize(jpegSize);

  tjDestroy(tjInstance);
  tjInstance = nullptr;
}
