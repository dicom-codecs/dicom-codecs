#include <dicomcodecs/codec.hpp>
#include <dicomcodecs/exception.hpp>
using namespace dicomcodecs;
using namespace std;

void readFile(string fileName, vector<uint8_t> &vec);
void printImage(const image &image);
void readFileAndDecode(const string &fileName, const string &codec,
                       image &image);
void roundTrip(const image &image, const string &codec,
               double maxAverageDiff = 0.0);
void readFileAndRoundTrip(const string &fileName, const string &codec,
                          double maxAverageDiff = 0.0);
void benchmark(image &image, const string &comment, const string &codec,
               size_t iterations);
void benchmarkDecode(const image &image, const string &comment, const string &codec,
        size_t iterations);
void benchmarkEncode(const image &image, const string &comment, const string &codec,
                     size_t iterations);

string makeRawFileName(const char *name, const image &image);
void writeFile(const string fileName, const vector<uint8_t> &vec);
string readRawFile(const string filePath, image &image);
void makeRawImages();
void readRawFileAndRoundTrip(const string &fileName, const string &codec,
                             double maxAverageDiff = 0.0);

void roundTripTests()
{
    // readRawFileAndRoundTrip("extern/test-data/raw/CT1-512-512-1-16-0.raw",
    // "charls");
    // readRawFileAndRoundTrip("extern/test-data/raw/CT1-512-512-1-16-0.raw",
    //                       "openjpeg");

    // readRawFileAndRoundTrip("extern/test-data/raw/jpeg400jfif-600-800-1-8-0.raw",
    // "charls");
    // readRawFileAndRoundTrip("extern/test-data/raw/jpeg400jfif-600-800-1-8-0.raw",
    // "openjpeg");
    // readRawFileAndRoundTrip("extern/test-data/raw/jpeg400jfif-600-800-1-8-0.raw",
    // "libjpeg-turbo",2.0);
    // readRawFileAndRoundTrip("extern/test-data/raw/jpeg400jfif-600-800-1-8-0.raw",
    //                        "ijg12", 2.0);
    readRawFileAndRoundTrip("extern/test-data/raw/jpeg400jfif-600-800-1-8-0.raw",
                            "libjpeg", 2.0);

    // readFileAndRoundTrip("extern/test-data/jpeg2000/CT1.j2k", "openjpeg");
    // readFileAndRoundTrip("extern/test-data/jpeglossy8bit/jpeg400jfif.jpg",
    // "libjpeg-turbo", 2.0);
    // readFileAndRoundTrip("extern/test-data/jpeglossy12bit/test12.jpg",
    // "ijg12", 2.0);
}

void benchmarkRawFilePath(const string &filePath, const string &codec,
                          size_t iterations)
{
    image image;
    string fileName = readRawFile(filePath, image);
    benchmark(image, fileName, codec, iterations);
}

void benchmarkTests()
{
    printf("** Running Benchmark Tests **\n");
    image image;
    string codec("charls");
    size_t iterations = 1;
    benchmarkRawFilePath("extern/test-data/raw/CT1-512-512-1-16-0.raw", codec,
                         iterations);
    benchmarkRawFilePath("extern/test-data/raw/CT2-512-512-1-16-0.raw", codec,
                         iterations);
    benchmarkRawFilePath("extern/test-data/raw/MG1-3064-4664-1-12-0.raw", codec,
                         iterations);
    benchmarkRawFilePath("extern/test-data/raw/MR2-1024-1024-1-12-0.raw", codec,
                         iterations);
    benchmarkRawFilePath("extern/test-data/raw/MR3-512-512-1-16-0.raw", codec,
                         iterations);
    benchmarkRawFilePath("extern/test-data/raw/MR4-512-512-1-12-0.raw", codec,
                         iterations);
    benchmarkRawFilePath("extern/test-data/raw/NM1-256-1024-1-16-0.raw", codec,
                         iterations);
    benchmarkRawFilePath("extern/test-data/raw/RG1-1841-1955-1-15-0.raw", codec,
                         iterations);
    benchmarkRawFilePath("extern/test-data/raw/RG2-1760-2140-1-10-0.raw", codec,
                         iterations);
    benchmarkRawFilePath("extern/test-data/raw/RG3-1760-1760-1-10-0.raw", codec,
                         iterations);
    benchmarkRawFilePath("extern/test-data/raw/SC1-2048-2487-1-12-0.raw", codec,
                         iterations);
}

void decodeTests()
{
    image image;
    readFileAndDecode("extern/test-data/jpeglossless/CT1.JLL", "ljpeg6b", image);
    printImage(image);
    readFileAndDecode("extern/test-data/jpeglossless/CT1.JLL", "gdcm-jpeg16", image);
    printImage(image);
    /*    std::vector<unsigned char> encodedBytes;
        encode(image, encodedBytes, "charls");
        writeFile("foo.jls", encodedBytes);
    */
}

void testAllCodecs() {
    printf("** Testing all codecs **\n");
    try {
            
        image image;
        readFileAndDecode("extern/test-data/jpeglossy8bit/jpeg400jfif.jpg", "libjpeg-turbo8", image);

        printImage(image);

        roundTrip(image, "libjpeg-turbo8", 2.0);
        roundTrip(image, "ljpeg6b16");
        roundTrip(image, "ijg12", 2.0);
        roundTrip(image, "gdcm-jpeg16", 2.0);
        roundTrip(image, "charls");
        roundTrip(image, "openjpeg");
        //roundTrip(image, "rle");

        size_t iterations = 5;
        benchmarkDecode(image, "", "libjpeg-turbo8", iterations);
        benchmarkDecode(image, "", "ljpeg6b16", iterations);
        benchmarkDecode(image, "", "ijg12", iterations);
        benchmarkDecode(image, "", "gdcm-jpeg16", iterations);
        benchmarkDecode(image, "", "charls", iterations);
        benchmarkDecode(image, "", "openjpeg", iterations);
        //benchmarkDecode(image, "", "rle", iterations);

        benchmarkEncode(image, "", "libjpeg-turbo8", iterations);
        benchmarkEncode(image, "", "ljpeg6b16", iterations);
        benchmarkEncode(image, "", "ijg12", iterations);
        benchmarkEncode(image, "", "gdcm-jpeg16", iterations);
        benchmarkEncode(image, "", "charls", iterations);
        benchmarkEncode(image, "", "openjpeg", iterations);
        //benchmarkEncode(image, "", "rle", iterations);
    }
    catch(dicom_codec_exception& ex) {
        printf("%s\n", ex.what());
    }
    catch(const char*pFoo) {
        printf("%s\n", pFoo);
    }
}

int main(int argc, char **argv)
{
    init();

    // makeRawImages();
    // roundTripTests();
    // benchmarkTests();
    testAllCodecs();
    //decodeTests();
    // readFileAndDecode("../../chafey/openjphjs/test/fixtures/j2c/CT1.j2c",
    // "openjpeg", image); // HTJ2K

    return 0;
}
