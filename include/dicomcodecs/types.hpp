// Copyright (c) Chris Hafey.
// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>

namespace dicomcodecs {
enum codec_t { JPEG2000, JPEGLS, JPEGLOSSLESS, RLE };

struct point {
  point() : x(0), y(0) {}
  point(uint32_t x, uint32_t y) : x(x), y(y) {}

  uint32_t x;
  uint32_t y;
};

struct size {
  size() : width(0), height(0) {}
  size(uint32_t width, uint32_t height) : width(width), height(height) {}

  uint32_t width;
  uint32_t height;
};

} // namespace dicomcodecs
