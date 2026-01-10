//
// Created by debilian on 10.01.2026.
//

#pragma once

#include <cstdint>

namespace TheKoder::BMP::Enums
{
    enum class BitsPerPixel : uint16_t
    {
        monochrome = 1,
        palette_4 = 4,
        palette_8 = 8,
        rgb_16 = 16,
        rgb_24 = 24,
        rgb_32 = 32
    };

    enum class CompressionType : uint32_t
    {
        RGB = 0,
        RLE8 = 1,
        RLE4 = 2,
        BITFIELDS = 3
    };
} // namespace TheKoder::BMP::Enums
