#include <fstream>
#include <iostream>
#include <vector>
#include <iterator>

void writeFile(const std::string fileName, const std::vector<uint8_t>& vec) {
    std::ofstream outFile(fileName);
    outFile.write(reinterpret_cast<const char*>(&vec[0]), vec.size());
}
