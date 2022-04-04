#include <dicomcodecs/codec.hpp>

void readFileAndDecode(const std::string& fileName, const std::string& codec, dicomcodecs::image& image);
std::string makeRawFileName(const char* name, const dicomcodecs::image& image);
void writeFile(const std::string fileName, const std::vector<uint8_t>& vec);

void makeRawImages() {
    printf("** Creating Raw Images **\n");
    dicomcodecs::image image;
    std::string fileName;
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
