//
// Created by debilian on 14.01.2026.
//

#pragma once

#include <array>
#include "../enums.hpp"
#include "codec.hpp"
#include "utils/reading_value.hpp"

namespace TheKoder::WAV::Chunks{
struct Riff
{
    std::array<char,4> chunk_id{};
    uint32_t chunk_size;
    std::array<char,4> format{};

    explicit Riff(DataCursor cursor)
    {
        this->chunk_id = read_array_value<std::array<char,4>>(cursor);
        this->chunk_size = read_numerical_value<uint32_t>(cursor);
        this->format = read_array_value<std::array<char,4>>(cursor);
    }
};

struct Fmt
{
    std::array<char,4> chunk_id{};
    uint32_t chunk_size;
    uint16_t format;
    uint16_t channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    Enums::BitsPerSample bits_per_sample;

    explicit Fmt(DataCursor cursor)
    {
        this->chunk_id = read_array_value<std::array<char,4>>(cursor);
        this->chunk_size = read_numerical_value<uint32_t>(cursor);
        this->format = read_array_value<uint16_t>(cursor);
        this->channels = read_array_value<uint16_t>(cursor);
        this->sample_rate = read_numerical_value<uint32_t>(cursor);
        this->byte_rate = read_numerical_value<uint32_t>(cursor);
        this->block_align = read_numerical_value<uint16_t>(cursor);
        this->bits_per_sample = read_numerical_value<Enums::BitsPerSample>(cursor);
    }
};

struct Data
{
    std::array<char,4> chunk_id{};
    uint32_t size;
    std::span<std::byte> data;

    explicit Data(DataCursor cursor)
    {
        this->chunk_id = read_array_value<std::array<char,4>>(cursor);
        this->size = read_numerical_value<uint32_t>(cursor);
        this->data = std::span(cursor, size);
    }
};
}