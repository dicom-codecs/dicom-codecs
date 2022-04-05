#include <dicomcodecs/codec.hpp>

using namespace dicomcodecs;
using namespace std;

void readFileAndDecode(const string &fileName, const string &codec,
                       image &image);
string makeRawFileName(const char *name, const image &image);
void writeFile(const string fileName, const vector<uint8_t> &vec);

void makeRawImagesFromJPEGLS() {
  image image;
  string fileName;
  readFileAndDecode("extern/test-data/jpegls/CT1.JLS", "charls", image);
  fileName = makeRawFileName("extern/test-data/raw/CT1", image);
  printf("%s\n", fileName.c_str());
  writeFile(fileName, image.rawBytes);

  readFileAndDecode("extern/test-data/jpegls/CT2.JLS", "charls", image);
  fileName = makeRawFileName("extern/test-data/raw/CT2", image);
  printf("%s\n", fileName.c_str());
  writeFile(fileName, image.rawBytes);

  readFileAndDecode("extern/test-data/jpegls/MG1.JLS", "charls", image);
  fileName = makeRawFileName("extern/test-data/raw/MG1", image);
  printf("%s\n", fileName.c_str());
  writeFile(fileName, image.rawBytes);

  /*    readFileAndDecode("extern/test-data/jpegls/MR1.JLS", "charls", image);
      fileName = makeRawFileName("extern/test-data/raw/MR1", image);
      printf("%s\n", fileName.c_str());
      writeFile(fileName, image.rawBytes);
  */
  readFileAndDecode("extern/test-data/jpegls/MR2.JLS", "charls", image);
  fileName = makeRawFileName("extern/test-data/raw/MR2", image);
  printf("%s\n", fileName.c_str());
  writeFile(fileName, image.rawBytes);

  readFileAndDecode("extern/test-data/jpegls/MR3.JLS", "charls", image);
  fileName = makeRawFileName("extern/test-data/raw/MR3", image);
  printf("%s\n", fileName.c_str());
  writeFile(fileName, image.rawBytes);

  readFileAndDecode("extern/test-data/jpegls/MR4.JLS", "charls", image);
  fileName = makeRawFileName("extern/test-data/raw/MR4", image);
  printf("%s\n", fileName.c_str());
  writeFile(fileName, image.rawBytes);

  readFileAndDecode("extern/test-data/jpegls/NM1.JLS", "charls", image);
  fileName = makeRawFileName("extern/test-data/raw/NM1", image);
  printf("%s\n", fileName.c_str());
  writeFile(fileName, image.rawBytes);

  readFileAndDecode("extern/test-data/jpegls/RG1.JLS", "charls", image);
  fileName = makeRawFileName("extern/test-data/raw/RG1", image);
  printf("%s\n", fileName.c_str());
  writeFile(fileName, image.rawBytes);

  readFileAndDecode("extern/test-data/jpegls/RG2.JLS", "charls", image);
  fileName = makeRawFileName("extern/test-data/raw/RG2", image);
  printf("%s\n", fileName.c_str());
  writeFile(fileName, image.rawBytes);

  readFileAndDecode("extern/test-data/jpegls/RG3.JLS", "charls", image);
  fileName = makeRawFileName("extern/test-data/raw/RG3", image);
  printf("%s\n", fileName.c_str());
  writeFile(fileName, image.rawBytes);

  readFileAndDecode("extern/test-data/jpegls/SC1.JLS", "charls", image);
  fileName = makeRawFileName("extern/test-data/raw/SC1", image);
  printf("%s\n", fileName.c_str());
  writeFile(fileName, image.rawBytes);

  readFileAndDecode("extern/test-data/jpegls/XA1.JLS", "charls", image);
  fileName = makeRawFileName("extern/test-data/raw/XA1", image);
  printf("%s\n", fileName.c_str());
  writeFile(fileName, image.rawBytes);
}

void makeRawImagesFromJ2K() {
  image image;
  string fileName;
  string baseDir("extern/test-data/jpeg2000/");

  readFileAndDecode(baseDir + "MR1.j2k", "openjpeg", image);
  fileName = makeRawFileName("../test-data/raw/MR1", image);
  printf("%s\n", fileName.c_str());
  writeFile(fileName, image.rawBytes);

  readFileAndDecode(baseDir + "US1.j2k", "openjpeg", image);
  fileName = makeRawFileName("../test-data/raw/US1", image);
  printf("%s\n", fileName.c_str());
  writeFile(fileName, image.rawBytes);

  readFileAndDecode(baseDir + "VL1.j2k", "openjpeg", image);
  fileName = makeRawFileName("../test-data/raw/VL1", image);
  printf("%s\n", fileName.c_str());
  writeFile(fileName, image.rawBytes);

  readFileAndDecode(baseDir + "VL2.j2k", "openjpeg", image);
  fileName = makeRawFileName("../test-data/raw/VL2", image);
  printf("%s\n", fileName.c_str());
  writeFile(fileName, image.rawBytes);

  readFileAndDecode(baseDir + "VL3.j2k", "openjpeg", image);
  fileName = makeRawFileName("../test-data/raw/VL3", image);
  printf("%s\n", fileName.c_str());
  writeFile(fileName, image.rawBytes);

  readFileAndDecode(baseDir + "VL4.j2k", "openjpeg", image);
  fileName = makeRawFileName("../test-data/raw/VL4", image);
  printf("%s\n", fileName.c_str());
  writeFile(fileName, image.rawBytes);

  readFileAndDecode(baseDir + "VL5.j2k", "openjpeg", image);
  fileName = makeRawFileName("../test-data/raw/VL5", image);
  printf("%s\n", fileName.c_str());
  writeFile(fileName, image.rawBytes);

  readFileAndDecode(baseDir + "VL6.j2k", "openjpeg", image);
  fileName = makeRawFileName("../test-data/raw/VL6", image);
  printf("%s\n", fileName.c_str());
  writeFile(fileName, image.rawBytes);
}

void makeRawImagesFromJPEGLossy8Bit() {
  image image;
  string fileName;
  string baseDir("extern/test-data/jpeglossy8bit/");

  readFileAndDecode(baseDir + "jpeg400jfif.jpg", "libjpeg-turbo", image);
  fileName = makeRawFileName("../test-data/raw/jpeg400jfif", image);
  printf("%s\n", fileName.c_str());
  writeFile(fileName, image.rawBytes);
}

void makeRawImagesFromJPEGLossy12Bit() {
  image image;
  string fileName;
  string baseDir("extern/test-data/jpeglossy12bit/");

  readFileAndDecode(baseDir + "test12.jpg", "ijg12", image);
  fileName = makeRawFileName("../test-data/raw/test12", image);
  printf("%s\n", fileName.c_str());
  writeFile(fileName, image.rawBytes);
}

void makeRawImages() {
  printf("** Creating Raw Images **\n");
  // makeRawImagesFromJPEGLS();
  // makeRawImagesFromJ2K();
  // makeRawImagesFromJPEGLossy8Bit();
  makeRawImagesFromJPEGLossy12Bit();
}
