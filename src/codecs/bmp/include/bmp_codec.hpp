//
// Created by remza on 28.12.2025.
//

#pragma once

#include "chunks.hpp"
#include "codec.hpp"
#include "constants.hpp"
#include "colors.hpp"

class BMP
{
public:
    explicit BMP(ImageCursor cursor);

    FileHeader header;
    InfoHeader info_header;
    ColorTable color_table;
    std::span<std::byte> data;

    auto decode_to_rgb8() -> std::vector<RGB8>;
    auto decode_to_rgba8() -> std::vector<RGBA8>;
    auto decode_to_rgb16() -> std::vector<RGB16>;
    auto decode_to_rgba16() -> std::vector<RGBA16>;

private:
    template<typename T>
    auto decode() -> std::vector<T>;

};
