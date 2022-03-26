#include <vector>
#include "image.hpp"
#include "types.hpp"
#include <string>

void decode(const std::vector<uint8_t> & encodedBytes, dicomcodecs::image& targetImage, const std::string& codec);
