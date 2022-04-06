#include <dicomcodecs/codec.hpp>

using namespace dicomcodecs;
using namespace std;

void roundTrip(const image &image, const string &codec,
               double maxAverageDiff = 0.0);
void readFileAndDecode(const string &fileName, const string &codec,
                       image &image);

void readFileAndRoundTrip(const string &fileName, const string &codec,
                          double maxAverageDiff = 0.0) {
  try {
    image image;
    readFileAndDecode(fileName, codec, image);
    roundTrip(image, codec, maxAverageDiff);
  } catch (const char *err) {
    printf("EXCEPTION: %s\n", err);
  }
}
