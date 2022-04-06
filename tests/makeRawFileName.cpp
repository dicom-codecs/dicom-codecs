#include <dicomcodecs/image.hpp>
#include <exception>
#include <string>
#include <vector>

using namespace dicomcodecs;
using namespace std;

string makeRawFileName(const char *name, const image &image) {
  char fileName[128];
  sprintf(fileName, "%s-%d-%d-%d-%d-%d.raw", name, image.width, image.height,
          image.componentCount, image.bitsPerSample, image.isSigned);
  
  return fileName;
}
