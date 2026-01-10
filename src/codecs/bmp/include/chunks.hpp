#pragma once

#include <array>
#include <cstddef> // for std::byte
#include <cstdint>
#include <span>

#include "colors.hpp"
#include "enums.hpp"

using ImageCursor = std::byte *&;
using namespace TheKoder::BMP::Enums;

namespace TheKoder::BMP::Chunks
{
    struct FileHeader
    {
        std::array<char, 2> signature{};
        uint32_t file_size{};
        std::array<std::byte, 4> reserved{};
        uint32_t offset{};

        explicit FileHeader(ImageCursor cursor);
    };

    struct InfoHeader
    {
        uint32_t size{};
        uint32_t width{};
        uint32_t height{};
        uint16_t planes{};
        BitsPerPixel bits_per_pixel{};
        CompressionType compression{};
        uint32_t image_size{};
        uint32_t x_pixels_per_m{};
        uint32_t y_pixels_per_m{};
        uint32_t color_used{};
        uint32_t important_colors{};

        explicit InfoHeader(ImageCursor cursor);
    };

    struct ColorTable
    {
        std::span<RGBA8> colors;
        explicit ColorTable(ImageCursor cursor, uint32_t color_count = 256);

        ColorTable() = default;
    };

} // namespace TheKoder::BMP::Chunks
