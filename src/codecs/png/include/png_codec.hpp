//
// Created by remza on 28.12.2025.
//

#pragma once

#include "chunks.hpp"
#include "codec.hpp"
#include "colors.hpp"
#include "constants.hpp"

class PngCodec
{
public:
    auto decode_to_rgb8() -> std::vector<RGB8>;
    auto decode_to_rgba8() -> std::vector<RGBA8>;
    auto decode_to_rgb16() -> std::vector<RGB16>;
    auto decode_to_rgba16() -> std::vector<RGBA16>;
    [[nodiscard]] auto decode_raw() const -> std::vector<std::byte>;

    explicit PngCodec(ImageCursor cursor);

    std::byte *zero_cursor;
    PNGSignature signature;
    Ihdr ihdr;
    Gama gama{};
    Phys phys{};
    Idat idat{};

    uint8_t bytes_per_pixel{};

private:
    auto transform_2d_to_1d(uint32_t x, uint32_t y) const -> uint32_t;

    auto filter_sub(std::span<std::byte> row) const -> void;
    auto filter_up(std::span<std::byte> row, std::span<std::byte> up_row) const -> void;
    auto filter_average(std::span<std::byte> row, std::span<std::byte> up_row) const -> void;
    auto filter_paeth(std::span<std::byte> row, std::span<std::byte> up_row) const -> void;

    // static auto apply_filter_sub(std::span<ColorRGB> &row) -> void;
    // auto apply_filter_up(std::span<ColorRGB> &row, std::span<ColorRGB> &up_row) const -> void;
    // auto apply_filter_avg(std::span<ColorRGB> &row, std::span<ColorRGB> &up_row, std::span<ColorRGB> &down_row) const
    // -> void;
    // auto apply_filter_paeth(std::span<ColorRGB> &row, std::span<ColorRGB> &up_row, std::span<ColorRGB> &down_row)
    // const
    // -> void;
};
