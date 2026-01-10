//
// Created by remza on 28.12.2025.
//

#pragma once

#include <array>
#include <cstdint>
#include <cstring>
#include <string_view>
#include <vector>

#include "colors.hpp"
#include "constants.hpp"
#include "logger.hpp"
#include "utils.h"

using ImageCursor = std::byte *&;

enum class BitsPerPixel : uint16_t
{
    monochrome = 1,
    palette_4 = 4,
    palette_8 = 8,
    palette_16 = 16,
    palette_24 = 24
};

enum class CompressionType : uint32_t
{
    RGB = 0,
    RLE8 = 1,
    RLE4 = 2,
};

struct FileHeader
{
    std::array<char, 2> signature{};
    uint32_t file_size{};
    std::array<std::byte, 4> reserved{};
    uint32_t offset{};

    explicit FileHeader(ImageCursor cursor)
    {
        this->signature = read_array_value<std::array<char, 2>>(cursor);
        this->file_size = read_numerical_value<uint32_t>(cursor);
        cursor += sizeof(this->file_size);
        this->offset = read_numerical_value<uint32_t>(cursor);
    }
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

    explicit InfoHeader(ImageCursor cursor)
    {
        this->size = read_numerical_value<uint32_t>(cursor);
        this->width = read_numerical_value<uint32_t>(cursor);
        this->height = read_numerical_value<uint32_t>(cursor);
        this->planes = read_numerical_value<uint16_t>(cursor);
        this->bits_per_pixel = read_numerical_value<BitsPerPixel>(cursor);
        this->compression = read_numerical_value<CompressionType>(cursor);
        this->image_size = read_numerical_value<uint32_t>(cursor);
        this->x_pixels_per_m = read_numerical_value<uint32_t>(cursor);
        this->y_pixels_per_m = read_numerical_value<uint32_t>(cursor);
        this->color_used = read_numerical_value<uint32_t>(cursor);
        this->important_colors = read_numerical_value<uint32_t>(cursor);
    }
};

struct ColorTable
{
    std::span<RGBA8> colors;

    explicit ColorTable(ImageCursor cursor)
    {
        constexpr uint32_t COLOR_TABLE_SIZE = 256 * 4;

        this->colors = std::span(reinterpret_cast<RGBA8*>(cursor) , COLOR_TABLE_SIZE);
        cursor += COLOR_TABLE_SIZE;
    }

    ColorTable() = default;
};

struct PixelData
{

};

