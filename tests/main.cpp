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

void testCodecs(const char *filePath, const std::vector<std::string> &codecs, double maxAverageDiff, size_t iterations)
{
    try
    {
        image image;
        readRawFile(filePath, image);
        // printImage(image);

        for (std::string codec : codecs)
        {
            // roundTrip(image, codec, maxAverageDiff);
        }
        for (std::string codec : codecs)
        {
            benchmarkDecode(image, "", codec, iterations);
        }
        for (std::string codec : codecs)
        {
            // benchmarkEncode(image, "", codec, iterations);
        }
    }
    catch (dicom_codec_exception &ex)
    {
        printf("%s\n", ex.what());
    }
    catch (const char *pFoo)
    {
        printf("%s\n", pFoo);
    }
    catch (...)
    {
        printf("caught unknown exception..\n");
    }
}

void testAllCodecs8BitColorUS(size_t iterations = 5)
{
    printf("** Testing all codecs 8 bit color US**\n");
    std::vector<std::string> codecs{"libjpeg-turbo8", "ljpeg6b16", "ijg12", "gdcm-jpeg16", "charls", "openjpeg", "openjph", "rle", "kakadu"};
    testCodecs("extern/test-data/raw/US1-640-480-3-8-0.raw", codecs, 6.0, iterations);
}

void testAllCodecs12BitGrayMG(size_t iterations = 5)
{
    printf("** Testing all codecs 12 bit gray MG**\n");
    std::vector<std::string> codecs{"gdcm-jpeg16", "charls", "openjpeg", "openjph", "rle", "kakadu"};
    testCodecs("extern/test-data/raw/MG1-3064-4664-1-12-0.raw", codecs, 0.0, iterations);
}

void testAllCodecs12BitGrayMR(size_t iterations = 5)
{
    printf("** Testing all codecs 12 bit gray MR**\n");
    std::vector<std::string> codecs{"charls", "gdcm-jpeg16", "openjpeg", "openjph", "rle", "kakadu"};
    testCodecs("extern/test-data/raw/MR4-512-512-1-12-0.raw", codecs, 0.0, iterations);
}

void testAllCodecs16BitGrayCT(size_t iterations = 5)
{
    printf("** Testing all codecs 16 bit gray CT**\n");
    // gdcm-jpeg16 broken for signed?
    // std::vector<std::string> codecs{"gdcm-jpeg16"};
    // std::vector<std::string> codecs{"ljpeg6b16"};
    std::vector<std::string> codecs{"openjpeg", "openjph", "rle", "kakadu", "charls"};
    testCodecs("extern/test-data/raw/CT1-512-512-1-16-1.raw", codecs, 0.0, iterations);
}

int main(int argc, char **argv)
{
    init();
    /*
    std::vector<unsigned char> encodedBytes;
    readFile("extern/test-data/jpeglossy12bit/test12.jpg", encodedBytes);
    image image;
    decode(encodedBytes, image, "ljpeg6b16");
    printImage(image);

    image image;
    readRawFile("extern/test-data/raw/MR4-512-512-1-12-0.raw", image);
    printImage(image);
    std::vector<unsigned char> encodedBytes;
    encode(image, encodedBytes, "kakadu");
    writeFile("kakadu.jph", encodedBytes);
    */

    const size_t iterations = 1;
    testAllCodecs8BitColorUS(iterations);
    testAllCodecs12BitGrayMR(iterations);
    testAllCodecs16BitGrayCT(iterations); // gdcm-jpeg16 broken
    testAllCodecs12BitGrayMG(iterations);
    return 0;
}
