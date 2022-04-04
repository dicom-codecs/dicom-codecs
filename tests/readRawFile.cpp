#include <string>
#include <vector>
#include <stdexcept>
#include <dicomcodecs/image.hpp>

void readFile(std::string fileName, std::vector<uint8_t>& vec);

std::string parseToken(const std::string str, size_t& start) {
    size_t end = str.find("-", start);
    if(end == std::string::npos) {
        end = str.length();
    }
    std::string token = str.substr(start, end - start); 
    start = end + 1; // skip over the delimeter
    return token;
}

std::string readRawFile(const std::string filePath, dicomcodecs::image& image) {
    size_t fileNameEnd = filePath.rfind("."); // find extension
    size_t fileNameStart = filePath.rfind("/") + 1; // find last directory separator
    std::string fileName = filePath.substr(fileNameStart, fileNameEnd-fileNameStart);
    size_t start = 0;
    std::string name = parseToken(fileName, start);
    image.width = stoi(parseToken(fileName, start));
    image.height = stoi(parseToken(fileName, start));
    image.componentCount = stoi(parseToken(fileName, start));
    image.bitsPerSample = stoi(parseToken(fileName, start));
    image.isSigned = stoi(parseToken(fileName, start));
    readFile(filePath, image.rawBytes);
    return fileName;
}
