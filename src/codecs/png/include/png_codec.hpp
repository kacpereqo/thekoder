//
// Created by remza on 28.12.2025.
//

#pragma once

#include "codec.hpp"
#include "chunks.hpp"
#include "constants.hpp"

struct ColorRGB {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    ColorRGB() = default;
    ColorRGB(const std::byte r, const std::byte g,const std::byte b) :
    r(std::to_integer<uint8_t>(r)),
    g(std::to_integer<uint8_t>(g)),
    b(std::to_integer<uint8_t>(b))
    {}

    friend std::ostream& operator<< (std::ostream& str, const ColorRGB& c) {
        str << "RGB( r=" << static_cast<int>(c.r) << ", g=" << static_cast<int>(c.g)<< ", b=" << static_cast<int>(c.b) << ")";
        return str;
    }
};

class PngCodec  {
public:
    auto decode_to_rgb(std::byte* &cursor) -> std::vector<ColorRGB>;
    auto decode(std::byte* &cursor) -> std::vector<std::byte>;

[[nodiscard]] IHDRData get_ihdr() const {
        return ihdr;
    }

    [[nodiscard]] GAMAData get_gama() const {
        return gama;
    }

    [[nodiscard]] PHYSData get_phys() const {
        return phys;
    }

    [[nodiscard]] IDATData get_idat() const {
        return idat;
    }

    static auto is_signature_valid(std::byte* &cursor) -> bool;

private:

    IHDRData ihdr{};
    GAMAData gama{};
    PHYSData phys{};
    IDATData idat{};

};
