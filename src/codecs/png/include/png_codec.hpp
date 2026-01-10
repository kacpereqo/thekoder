//
// Created by remza on 28.12.2025.
//

#pragma once

#include "chunks.hpp"
#include "codec.hpp"
#include "constants.hpp"

struct ColorRGB
{
    uint8_t r;
    uint8_t g;
    uint8_t b;

    ColorRGB() = default;

    ColorRGB(const uint8_t r, const uint8_t g, const uint8_t b) : r(r), g(g), b(b)
    {}

    ColorRGB(const std::byte r, const std::byte g, const std::byte b) :
        r(std::to_integer<uint8_t>(r)), g(std::to_integer<uint8_t>(g)), b(std::to_integer<uint8_t>(b))
    {}

    ColorRGB operator-(const ColorRGB rhs) const
    {
        const uint8_t new_r = this->r - rhs.r;
        const uint8_t new_g = this->b - rhs.g;
        const uint8_t new_b = this->b - rhs.b;

        return {new_r, new_g, new_b};
    }

    friend std::ostream &operator<<(std::ostream &str, const ColorRGB &c)
    {
        str << "RGB( r=" << static_cast<int>(c.r) << ", g=" << static_cast<int>(c.g) << ", b=" << static_cast<int>(c.b)
            << ")";
        return str;
    }
};

class PngCodec
{
public:
    auto decode_to_rgb(std::byte *&cursor) -> std::vector<ColorRGB>;
    auto decode(std::byte *&cursor) -> std::vector<std::byte>;

    [[nodiscard]] IHDRData get_ihdr() const
    {
        return ihdr;
    }

    [[nodiscard]] GAMAData get_gama() const
    {
        return gama;
    }

    [[nodiscard]] PHYSData get_phys() const
    {
        return phys;
    }

    [[nodiscard]] IDATData get_idat() const
    {
        return idat;
    }

    static auto is_signature_valid(std::byte *&cursor) -> bool;

private:
    IHDRData ihdr{};
    GAMAData gama{};
    PHYSData phys{};
    IDATData idat{};

    static auto apply_filter_sub(std::span<ColorRGB> &row) -> void;
    auto apply_filter_up(std::span<ColorRGB> &row, std::span<ColorRGB> &up_row) const -> void;
    auto apply_filter_avg(std::span<ColorRGB> &row, std::span<ColorRGB> &up_row, std::span<ColorRGB> &down_row) const
            -> void;
    auto apply_filter_paeth(std::span<ColorRGB> &row, std::span<ColorRGB> &up_row, std::span<ColorRGB> &down_row) const
            -> void;
};
