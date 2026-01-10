//
// Created by debilian on 10.01.2026.
//

#include "include/chunks.hpp"

#include "utils/reading_value.hpp"
#include "logger.hpp"

using namespace TheKoder::BMP::Chunks;
using namespace TheKoder::BMP::Enums;

FileHeader::FileHeader(ImageCursor cursor)
{
    this->signature = read_array_value<std::array<char, 2>>(cursor);

    this->file_size = read_numerical_value<uint32_t>(cursor);

    this->reserved = read_array_value<std::array<std::byte, 4>>(cursor);

    this->offset = read_numerical_value<uint32_t>(cursor);
}

InfoHeader::InfoHeader(ImageCursor cursor)
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

ColorTable::ColorTable(ImageCursor cursor, uint32_t color_count)
{
    auto* palette_ptr = reinterpret_cast<RGBA8*>(cursor);

    this->colors = std::span(palette_ptr, color_count);

    cursor += (color_count * sizeof(RGBA8));
}