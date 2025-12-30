#pragma once

#include <array>

namespace TheKoder::PNG::Constants {
     constexpr std::array
    SIGNATURE = {
            std::byte{0x89}, std::byte{0x50}, std::byte{0x4E}, std::byte{0x47},
            std::byte{0x0D}, std::byte{0x0A}, std::byte{0x1A}, std::byte{0x0A}
    };


}

namespace ChunkType {
    constexpr auto IEND = "IEND";
    constexpr auto IHDR = "IHDR";
    constexpr auto SRGB = "sRGB";
    constexpr auto PNG = "‰PNG";
    constexpr auto GAMA = "gAMA";
    constexpr auto PHYS = "pHYs";
    constexpr auto IDAT = "IDAT";
};