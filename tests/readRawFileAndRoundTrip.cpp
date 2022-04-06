#include <dicomcodecs/codec.hpp>

using namespace dicomcodecs;
using namespace std;

void roundTrip(const image &image, const string &codec,
               double maxAverageDiff = 0.0);
string readRawFile(const string filePath, image &image);

void readRawFileAndRoundTrip(const string &fileName, const string &codec,
                             double maxAverageDiff = 0.0) {
  try {
    printf("------BEGIN\n");
    image image;
    readRawFile(fileName, image);
    roundTrip(image, codec, maxAverageDiff);
    printf("------END\n");
  } catch (const char *err) {
    printf("EXCEPTION: %s\n", err);
  }
}
