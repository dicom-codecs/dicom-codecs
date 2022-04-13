#include <dicomcodecs/codec.hpp>
#include <dicomcodecs/image.hpp>

using namespace dicomcodecs;
using namespace std;

void printImage(const image &image);

int getPixel(const image &image, int pixelNum) {
  if(image.bitsPerSample <= 8) {
    return image.rawBytes[pixelNum];
  } else {
    if(image.isSigned) {
      return *(short*)(image.rawBytes.data() + pixelNum * 2);
    } else {
      return *(unsigned short*)(image.rawBytes.data() + pixelNum * 2);
    }
  }
}

void roundTrip(const image &image, const string &codec,
               double maxAverageDiff = 0.0) {
  vector<uint8_t> encodedBytes;
  encode(image, encodedBytes, codec);
  dicomcodecs::image decodedImage;
  decode(encodedBytes, decodedImage, codec);
  // printImage(decodedImage);

  size_t numDifferences = 0;
  float delta = 0.0f;
  const int numPixels = image.width * image.height * image.componentCount;
  for (size_t i = 0; i < numPixels; i++) {
    int originalPixel = getPixel(image, i);
    int decodedPixel = getPixel(decodedImage, i);
    if(originalPixel != decodedPixel) {
      //printf("diff at pixel %d, %d != %d\n", i, originalPixel, decodedPixel);
      delta += abs(originalPixel - decodedPixel);
      numDifferences++;
    }
  }

  const double averageDifference = numDifferences ? delta / numDifferences : 0;
  const double percent =
      (double)numDifferences / (double)numPixels * 100.0;

  printf("%s num differences = %zu/%zu (%0.2f %%), average diff = %0.2f\n", codec.c_str(),
          numDifferences, decodedImage.rawBytes.size(), percent,
          averageDifference);
  if (averageDifference > maxAverageDiff) {
    throw("roundtrip did not produce original image");
  }
}
