//
// Created by debilian on 31.12.2025.
//

#pragma once

#include <cstdint>
#include "png_codec.hpp"

class Image
{
public:
    uint32_t width;
    uint32_t height;
    // std::vector<T> data;
    Codec &codec;
};

class PNG : public Image
{};
