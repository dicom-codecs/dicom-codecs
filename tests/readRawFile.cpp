#include <dicomcodecs/image.hpp>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dicomcodecs;
using namespace std;

void readFile(string fileName, vector<uint8_t> &vec);

string parseToken(const string str, size_t &start) {
  size_t end = str.find("-", start);
  if (end == string::npos) {
    end = str.length();
  }
  string token = str.substr(start, end - start);
  start = end + 1; // skip over the delimeter
  return token;
}

string readRawFile(const string filePath, image &image) {
  size_t fileNameEnd = filePath.rfind("."); // find extension
  size_t fileNameStart =
      filePath.rfind("/") + 1; // find last directory separator
  string fileName = filePath.substr(fileNameStart, fileNameEnd - fileNameStart);
  size_t start = 0;
  string name = parseToken(fileName, start);
  image.width = stoi(parseToken(fileName, start));
  image.height = stoi(parseToken(fileName, start));
  image.componentCount = stoi(parseToken(fileName, start));
  image.bitsPerSample = stoi(parseToken(fileName, start));
  image.isSigned = stoi(parseToken(fileName, start));
  readFile(filePath, image.rawBytes);
  
  return fileName;
}
