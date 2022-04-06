// Copyright (c) Chris Hafey.
// SPDX-License-Identifier: MIT

#pragma once

#include <string>
#include <vector>

#include "image.hpp"
#include "types.hpp"

typedef void (*decoder_ptr)(const std::vector<uint8_t> &encodedBytes,
                            dicomcodecs::image &targetImage);
typedef void (*encoder_ptr)(const dicomcodecs::image &sourcerImage,
                            std::vector<uint8_t> &encodedBytes);

void init();
int registerDecoder(const std::string codec, decoder_ptr);
int registerEncoder(const std::string codec, encoder_ptr);
void decode(const std::vector<uint8_t> &encodedBytes,
            dicomcodecs::image &targetImage, const std::string &codec);
void encode(const dicomcodecs::image &sourceImage,
            std::vector<uint8_t> &encodedBytes, const std::string &codec);
