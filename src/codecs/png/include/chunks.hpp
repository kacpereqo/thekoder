//
// Created by remza on 28.12.2025.
//

#pragma once

#include <array>
#include <cstdint>
#include <string_view>
#include "codec.hpp"
#include "utils/reading_value.hpp"

struct ChunkBegin
{
    uint32_t length{};
    std::array<char, 4> name{};

    ChunkBegin() = default;
    explicit ChunkBegin(ImageCursor cursor)
    {
        this->length = read_numerical_value<uint32_t, std::endian::big>(cursor);
        this->name = read_array_value<std::array<char, 4>>(cursor);
    }

    auto name_as_str_view() -> std::string_view
    {
        return std::string_view{name};
    }
};

struct PNGSignature
{
    std::array<std::byte, 8> signature{};

    explicit PNGSignature(ImageCursor cursor)
    {
        this->signature = read_array_value<std::array<std::byte, 8>>(cursor);
    }
};

struct ChunkCrc
{
    uint32_t crc{};
};


struct Ihdr : ChunkBegin, ChunkCrc
{
    uint32_t width{};
    uint32_t height{};
    uint8_t bit_depth{};
    uint8_t color_type{};
    uint8_t compression_method{};
    uint8_t filter_method{};
    uint8_t interlace_method{};

    explicit Ihdr(ImageCursor cursor) : ChunkBegin(cursor), ChunkCrc()
    {
        this->width = read_numerical_value<uint32_t, std::endian::big>(cursor);
        this->height = read_numerical_value<uint32_t, std::endian::big>(cursor);
        this->bit_depth = read_numerical_value<uint8_t, std::endian::big>(cursor);
        this->color_type = read_numerical_value<uint8_t, std::endian::big>(cursor);
        this->compression_method = read_numerical_value<uint8_t, std::endian::big>(cursor);
        this->filter_method = read_numerical_value<uint8_t, std::endian::big>(cursor);
        this->interlace_method = read_numerical_value<uint8_t, std::endian::big>(cursor);
        this->crc = read_numerical_value<uint32_t, std::endian::big>(cursor);
    }
};

struct SRGBData
{};

struct Gama : ChunkBegin, ChunkCrc
{
    uint32_t gamma{};

    Gama() = default;
    explicit Gama(ImageCursor cursor) : ChunkBegin(cursor), ChunkCrc()
    {
        this->gamma = read_numerical_value<uint32_t, std::endian::big>(cursor);
        this->crc = read_numerical_value<uint32_t, std::endian::big>(cursor);
    }
};

struct Phys : ChunkBegin, ChunkCrc
{
    uint32_t pixels_per_unit_x{};
    uint32_t pixels_per_unit_y{};
    uint8_t unit{};

    Phys() = default;
    explicit Phys(ImageCursor cursor) : ChunkBegin(cursor), ChunkCrc()
    {
        this->pixels_per_unit_x = read_numerical_value<uint32_t, std::endian::big>(cursor);
        this->pixels_per_unit_y = read_numerical_value<uint32_t, std::endian::big>(cursor);
        this->unit = read_numerical_value<uint8_t, std::endian::big>(cursor);
        this->crc = read_numerical_value<uint32_t, std::endian::big>(cursor);
    }
};

struct Idat : ChunkBegin, ChunkCrc
{
    std::span<std::byte> compressed_data{};

    Idat() = default;
    explicit Idat(ImageCursor cursor) : ChunkBegin(cursor), ChunkCrc()
    {
        this->compressed_data = {cursor, this->length};
        this->crc = read_numerical_value<uint32_t, std::endian::big>(cursor);
    }
};