//
// Created by debilian on 10.01.2026.
//

#pragma once
#include <array>
#include <cstdint>
#include <span>

#include "codec.hpp"
#include "utils/reading_value.hpp"

enum class BitsPerSample : uint16_t
{
    bits_8 = 8,
    bits_16 = 16
};

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
    BitsPerSample bits_per_sample;

    explicit Fmt(DataCursor cursor)
    {
        this->chunk_id = read_array_value<std::array<char,4>>(cursor);
        this->chunk_size = read_numerical_value<uint32_t>(cursor);
        this->format = read_array_value<uint16_t>(cursor);
        this->channels = read_array_value<uint16_t>(cursor);
        this->sample_rate = read_numerical_value<uint32_t>(cursor);
        this->byte_rate = read_numerical_value<uint32_t>(cursor);
        this->block_align = read_numerical_value<uint16_t>(cursor);
        this->bits_per_sample = read_numerical_value<BitsPerSample>(cursor);
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

class WAV
{
public:
    Riff riff;
    Fmt fmt;
    Data data;

    auto decode_8bit() -> std::vector<uint8_t>;
    auto decode_16bit() -> std::vector<uint16_t>;

    explicit WAV(DataCursor cursor) : riff(cursor), fmt(cursor), data(cursor)
    {}

private:
    template<typename T>
    auto decode() -> std::vector<T>;
};