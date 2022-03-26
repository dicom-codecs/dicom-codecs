// Copyright (c) Chris Hafey.
// SPDX-License-Identifier: MIT

#pragma once

#include "image.hpp"
#include <vector>

namespace dicomcodecs
{
    /// <summary>
    /// Abstract base class for an image frame decoder
    /// </summary>
    class decoder
    {
        public:
            /// <summary>
            /// Decodes <encodedBytes> into <targetImage>
            /// </summary>
            virtual void decode(const std::vector<uint8_t> & encodedBytes, image& targetImage ) = 0;
    };
}