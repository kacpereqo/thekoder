//
// Created by remza on 28.12.2025.
//

#pragma once

#include <vector>
#include "colors.hpp"

using ImageCursor = std::byte *&;
using DataCursor = std::byte *&;

class Codec
{
public:
    virtual ~Codec() = default;

    [[nodiscard]] auto is_signature_valid() const -> bool;

    [[nodiscard]] auto virtual decode_to_rgb8() const -> std::vector<RGB8>;
    [[nodiscard]] auto virtual decode_to_rgba8() const -> std::vector<RGBA8>;
    [[nodiscard]] auto virtual decode_to_rgb16() const -> std::vector<RGB16>;
    [[nodiscard]] auto virtual decode_to_rgba16() const -> std::vector<RGBA16>;

    [[nodiscard]] auto virtual get_width() const -> std::size_t;
    [[nodiscard]] auto virtual get_height() const -> std::size_t;
};