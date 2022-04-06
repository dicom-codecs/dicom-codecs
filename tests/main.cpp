#include <dicomcodecs/codec.hpp>

void readFile(std::string fileName, std::vector<uint8_t>& vec);
void printImage(const dicomcodecs::image& image);
void readFileAndDecode(const std::string& fileName, const std::string& codec, dicomcodecs::image& image);
void roundTrip(const dicomcodecs::image& image, const std::string& codec, double maxAverageDiff = 0.0);
void readFileAndRoundTrip(const std::string& fileName, const std::string& codec, double maxAverageDiff = 0.0);
void benchmark(dicomcodecs::image& image, const std::string& comment, const std::string& codec, size_t iterations);
std::string makeRawFileName(const char* name, const dicomcodecs::image& image);
void writeFile(const std::string fileName, const std::vector<uint8_t>& vec);
std::string readRawFile(const std::string filePath, dicomcodecs::image& image);
void makeRawImages();
void readRawFileAndRoundTrip(const std::string& fileName, const std::string& codec, double maxAverageDiff = 0.0);

void roundTripTests() {
    //readRawFileAndRoundTrip("extern/test-data/raw/CT1-512-512-1-16-0.raw", "charls");
    //readRawFileAndRoundTrip("extern/test-data/raw/CT1-512-512-1-16-0.raw", "openjpeg");
    
    readRawFileAndRoundTrip("extern/test-data/raw/jpeg400jfif-600-800-1-8-0.raw", "charls");
    readRawFileAndRoundTrip("extern/test-data/raw/jpeg400jfif-600-800-1-8-0.raw", "openjpeg");
    readRawFileAndRoundTrip("extern/test-data/raw/jpeg400jfif-600-800-1-8-0.raw", "libjpeg-turbo",2.0);
    readRawFileAndRoundTrip("extern/test-data/raw/jpeg400jfif-600-800-1-8-0.raw", "ijg12", 2.0);

    //readFileAndRoundTrip("extern/test-data/jpeg2000/CT1.j2k", "openjpeg");
    //readFileAndRoundTrip("extern/test-data/jpeglossy8bit/jpeg400jfif.jpg", "libjpeg-turbo", 2.0);
    //readFileAndRoundTrip("extern/test-data/jpeglossy12bit/test12.jpg", "ijg12", 2.0);
}

void benchmarkRawFilePath(const std::string& filePath, const std::string& codec, size_t iterations) {
    dicomcodecs::image image;
    std::string fileName = readRawFile(filePath, image);
    benchmark(image, fileName, codec, iterations);
}

void benchmarkTests() {
    printf("** Running Benchmark Tests **\n");
    dicomcodecs::image image;
    std::string codec("charls");
    size_t iterations = 1;
    benchmarkRawFilePath("extern/test-data/raw/CT1-512-512-1-16-0.raw", codec, iterations);
    benchmarkRawFilePath("extern/test-data/raw/CT2-512-512-1-16-0.raw", codec, iterations);
    benchmarkRawFilePath("extern/test-data/raw/MG1-3064-4664-1-12-0.raw", codec, iterations);
    benchmarkRawFilePath("extern/test-data/raw/MR2-1024-1024-1-12-0.raw", codec, iterations);
    benchmarkRawFilePath("extern/test-data/raw/MR3-512-512-1-16-0.raw", codec, iterations);
    benchmarkRawFilePath("extern/test-data/raw/MR4-512-512-1-12-0.raw", codec, iterations);
    benchmarkRawFilePath("extern/test-data/raw/NM1-256-1024-1-16-0.raw", codec, iterations);
    benchmarkRawFilePath("extern/test-data/raw/RG1-1841-1955-1-15-0.raw", codec, iterations);
    benchmarkRawFilePath("extern/test-data/raw/RG2-1760-2140-1-10-0.raw", codec, iterations);
    benchmarkRawFilePath("extern/test-data/raw/RG3-1760-1760-1-10-0.raw", codec, iterations);
    benchmarkRawFilePath("extern/test-data/raw/SC1-2048-2487-1-12-0.raw", codec, iterations);
}


int main(int argc, char** argv) 
{
    init();

    //makeRawImages();
    roundTripTests();
    //benchmarkTests();

    //readFileAndDecode("../../chafey/openjphjs/test/fixtures/j2c/CT1.j2c", "openjpeg", image); // HTJ2K


    return 0;
}