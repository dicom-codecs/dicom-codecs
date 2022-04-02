// Copyright (c) Chris Hafey.
// SPDX-License-Identifier: MIT

#pragma once

#include <vector>

namespace dicomcodecs
{
    struct image {
        /// <summary>
        /// Width of the image, range [1, 65535].
        /// </summary>
        uint16_t width;

        /// <summary>
        /// Height of the image, range [1, 65535].
        /// </summary>
        uint16_t height;

        /// <summary>
        /// Number of bits per sample, range [2, 16]
        /// </summary>
        uint8_t bitsPerSample;

        /// <summary>
        /// Number of components contained in the frame, range [1, 255]
        /// </summary>
        uint8_t componentCount;

        /// <summary>
        /// true if signed, false if unsigned
        /// </summary>
        bool isSigned;

        /// <summary>
        /// the raw bytes for the underlying pixel data
        /// </summary>
        std::vector<uint8_t> rawBytes;

        /// <summary>
        /// equal operator
        /// </summary>
        inline bool operator== (const image& rhs) const { 
            return (width == rhs.width &&
                height == rhs.height &&
                width == rhs.width &&
                bitsPerSample == rhs.bitsPerSample &&
                componentCount == rhs.componentCount &&
                isSigned == rhs.isSigned &&
                rawBytes == rhs.rawBytes
            );
        }

        /// <summary>
        /// not equal operator
        /// </summary>
        inline bool operator!= (const image& rhs) const { 
            return !(*this == rhs);
        }

    };

}