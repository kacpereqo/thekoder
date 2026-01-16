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
    auto decode_to_rgba8() const -> std::vector<RGBA8>;
    auto decode_to_rgb16() const -> std::vector<RGB16>;
    auto decode_to_rgba16() const -> std::vector<RGBA16>;
    [[nodiscard]] auto decode_raw() const -> std::vector<std::byte>;

    explicit PngCodec(ImageCursor cursor);

    // Physical Chunks
    PNGSignature signature;
    Ihdr ihdr;
    Gama gama{};
    Phys phys{};
    Idat idat{};

    // Utility data
    uint8_t bytes_per_pixel{};
    uint32_t image_size{};
    uint32_t image_space{};

private:
    auto decode_idat() const -> std::vector<std::byte>;
    auto transform_2d_to_1d(uint32_t x, uint32_t y) const -> uint32_t;

    auto filter_sub(std::span<std::byte> row) const -> void;
    auto filter_up(std::span<std::byte> row, std::span<std::byte> up_row) const -> void;
    auto filter_average(std::span<std::byte> row, std::span<std::byte> up_row) const -> void;
    auto filter_paeth(std::span<std::byte> row, std::span<std::byte> up_row) const -> void;
};
