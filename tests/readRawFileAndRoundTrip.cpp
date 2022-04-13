#include <dicomcodecs/codec.hpp>

using namespace dicomcodecs;
using namespace std;

void roundTrip(const image &image, const string &codec,
               double maxAverageDiff = 0.0);
string readRawFile(const string filePath, image &image);

void readRawFileAndRoundTrip(const string &fileName, const string &codec,
                             double maxAverageDiff = 0.0)
{
  try
  {
    printf("BEGIN readRawFileAndRoundTrip(%s, %s)\n", fileName.c_str(), codec.c_str());
    image image;
    readRawFile(fileName, image);
    roundTrip(image, codec, maxAverageDiff);
    printf("END readRawFileAndRoundTrip(%s, %s)\n", fileName.c_str(), codec.c_str());
  }
  catch (const char *err)
  {
    printf("EXCEPTION: %s\n", err);
  }
}
