#include <dicomcodecs/codec.hpp>
#include <dicomcodecs/exception.hpp>
#include <dicomcodecs/image.hpp>
#include <string>
#include <vector>

using namespace dicomcodecs;
using namespace std;

void readFile(string fileName, vector<uint8_t> &vec);
void printImage(const image &image);

void readFileAndDecode(const string &fileName, const string &codec,
                       image &image) {
  try {
    // initialize image to defaults
    image = dicomcodecs::image();
    vector<uint8_t> encodedBytes;
    readFile(fileName, encodedBytes);
    decode(encodedBytes, image, codec);
  } catch (dicom_codec_exception &ex) {
    printf("EXCEPTION: %s from codec %s while decoding file %s", ex.reason(),
           ex.codec(), fileName.c_str());
  } catch (const char *reason) {
    printf("Decoding file %s with codec %s\n", fileName.c_str(), codec.c_str());
    printImage(image);
    printf("EXCEPTION: %s\n", reason);
  }
}
