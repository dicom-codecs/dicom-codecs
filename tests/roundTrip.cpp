#include <dicomcodecs/codec.hpp>

void printImage(const dicomcodecs::image& image);

void roundTrip(const dicomcodecs::image& image, const std::string& codec, double maxAverageDiff = 0.0) {
    std::vector<uint8_t> encodedBytes;
    encode(image, encodedBytes, codec);
    dicomcodecs::image decodedImage;
    decode(encodedBytes, decodedImage, codec);
    //printImage(decodedImage);

    if(decodedImage != image) {
        size_t numDifferences = 0;
        float delta = 0.0f;
        for(size_t i=0; i < decodedImage.rawBytes.size(); i++) {
            if(decodedImage.rawBytes[i] != image.rawBytes[i]) {
                delta += abs(decodedImage.rawBytes[i] - image.rawBytes[i]);
                numDifferences++;
            }
        }

        const double averageDifference = delta / numDifferences;
        const double percent =  (double)numDifferences / (double)decodedImage.rawBytes.size() * 100.0;

        printf("num differences = %zu/%zu (%0.2f %%), average diff = %0.2f\n", numDifferences, decodedImage.rawBytes.size(), percent, averageDifference);
        if(averageDifference > maxAverageDiff) {
            throw("roundtrip did not produce original image");
        }
    }
}
