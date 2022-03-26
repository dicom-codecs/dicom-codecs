// Copyright (c) Chris Hafey.
// SPDX-License-Identifier: MIT

#pragma once

#include "image.hpp"
#include <vector>

namespace dicomcodecs
{
    /// <summary>
    /// Abstract base class for an image frame encoder
    /// </summary>
    class encoder
    {
        public:
            /// <summary>
            /// Encodes <sourceImage> into <encodedBytes>
            /// </summary>
            virtual void encode(const image& sourceImage, std::vector<uint8_t> & encodedBytes) = 0;
    };
}
