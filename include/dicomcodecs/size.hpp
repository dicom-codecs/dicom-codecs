// Copyright (c) Chris Hafey.
// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>

namespace dicomcodecs
{
    struct size {
        size() : width(0), height(0) {}
        size(uint32_t width, uint32_t height) : width(width), height(height) {}

        uint32_t width;
        uint32_t height;
    };
}