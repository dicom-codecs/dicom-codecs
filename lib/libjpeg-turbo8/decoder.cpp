#include "../../extern/libjpeg-turbo/turbojpeg.h"
#include "../../include/dicomcodecs/exception.hpp"
#include "../../include/dicomcodecs/image.hpp"
#include <stdio.h>

using namespace dicomcodecs;
using namespace std;

void libjpegturbo8decoder(const vector<uint8_t> &encodedBytes,
                          image &targetImage) {
  tjhandle tjInstance = nullptr;
  if ((tjInstance = tjInitDecompress()) == nullptr) {
    const char *error = tjGetErrorStr2(tjInstance);
    throw dicom_codec_exception("libjpegturbo", error);
  }

  int width, height, inSubsamp, inColorspace;
  unsigned long jpegSize = (unsigned long)encodedBytes.size();

  int result = tjDecompressHeader3(tjInstance, encodedBytes.data(), jpegSize,
                                   &width, &height, &inSubsamp, &inColorspace);
  if (result < 0) {
    const char *error = tjGetErrorStr2(tjInstance);
    throw dicom_codec_exception("libjpegturbo", error);
  }

  targetImage.width = width;
  targetImage.height = height;
  targetImage.bitsPerSample = 8;
  targetImage.isSigned = false;
  targetImage.componentCount = inColorspace == 2 ? 1 : 3;

  int pixelFormat = (targetImage.componentCount == 1) ? TJPF_GRAY : TJPF_RGB;

  const size_t destinationSize =
      targetImage.width * targetImage.height * tjPixelSize[pixelFormat];
  targetImage.rawBytes.resize(destinationSize);

  if (tjDecompress2(tjInstance, encodedBytes.data(), encodedBytes.size(),
                    targetImage.rawBytes.data(), targetImage.width, 0,
                    targetImage.height, pixelFormat, 0) < 0) {
    tjDestroy(tjInstance);
    const char *error = tjGetErrorStr2(tjInstance);
    throw dicom_codec_exception("libjpegturbo", error);
  }

  tjDestroy(tjInstance);
}
