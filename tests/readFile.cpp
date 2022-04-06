#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

void readFile(string fileName, vector<uint8_t> &vec) {
  // open the file:
  ifstream file(fileName, ios::binary);
  if (file.fail()) {
    throw runtime_error("file does not exist (" + fileName + ")");
  }
  // Stop eating new lines in binary mode!!!
  file.unsetf(ios::skipws);

  // get its size:
  streampos fileSize;
  file.seekg(0, ios::end);
  fileSize = file.tellg();
  file.seekg(0, ios::beg);

  // reserve capacity
  vec.reserve(fileSize);

  // read the data:
  vec.insert(vec.begin(), istream_iterator<uint8_t>(file),
             istream_iterator<uint8_t>());
}
