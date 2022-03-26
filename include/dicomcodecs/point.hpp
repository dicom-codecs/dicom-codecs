// Copyright (c) Chris Hafey.
// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>

namespace dicomcodecs
{
    struct point {
        point() : x(0), y(0) {}
        point(uint32_t x, uint32_t y) : x(x), y(y) {}

        uint32_t x;
        uint32_t y;
    };
}