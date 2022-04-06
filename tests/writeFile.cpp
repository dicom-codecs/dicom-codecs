#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

void writeFile(const string fileName, const vector<uint8_t> &vec) {
  ofstream outFile(fileName);
  outFile.write(reinterpret_cast<const char *>(&vec[0]), vec.size());
}
