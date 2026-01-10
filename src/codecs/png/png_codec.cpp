//
// Created by remza on 28.12.2025.
//

#include "include/png_codec.hpp"
#include "include/constants.hpp"

#include <memory>
#include <zlib.h>

struct ScanLine
{
    uint8_t filter;
    uint8_t *r;
    uint8_t *g;
    uint8_t *b;
};


auto PngCodec::decode(std::byte *&cursor) -> std::vector<std::byte>
{
    Chunk chunk{};

    while (chunk.chunk_type_str() != ChunkType::IEND)
    {
        chunk = read_chunk(cursor);

        Logger::debug() << chunk << std::endl;

        const auto chunk_type = chunk.chunk_type_str();

        if (chunk_type == ChunkType::IHDR)
            this->ihdr = IHDRData{std::move(chunk)};
        else if (chunk_type == ChunkType::SRGB)
        {
        } else if (chunk_type == ChunkType::GAMA)
            this->gama = GAMAData{std::move(chunk)};
        else if (chunk_type == ChunkType::PHYS)
            this->phys = PHYSData{std::move(chunk)};
        else if (chunk_type == ChunkType::IDAT)
            this->idat = IDATData{std::move(chunk)};
        else if (chunk_type == ChunkType::IEND)
        {
        } else
        {
        }
    }

    const auto rgb_space = this->ihdr.width * this->ihdr.height * 3;
    const auto filter_space = this->ihdr.height;

    uLongf vector_length = rgb_space + filter_space;

    std::vector<std::byte> pixels(vector_length);

    const auto result = uncompress(reinterpret_cast<Bytef *>(pixels.data()), &vector_length,
                                   reinterpret_cast<Bytef *>(idat.compressed_data.data()), idat.compressed_data.size());

    if (result != Z_OK)
    {
        Logger::error() << result << std::endl;
    }

    return std::move(pixels);
}

namespace Filters
{
    constexpr auto NONE{std::byte{0x01}};
    constexpr auto SUB{std::byte{0x02}};
    constexpr auto AVG{std::byte{0x03}};
    constexpr auto PAETH{std::byte{0x04}};
} // namespace Filters

auto PngCodec::decode_to_rgb(std::byte *&cursor) -> std::vector<ColorRGB>
{
    const std::vector<std::byte> decoded_data = std::move(this->decode(cursor));

    const auto size = this->ihdr.width * this->ihdr.height;
    std::vector<ColorRGB> pixels(size);

    for (int row = 0; row < this->ihdr.height; row++)
    {
        constexpr auto COLOR_SIZE = sizeof(ColorRGB);

        for (int column = 0; column < this->ihdr.height; column++)
        {
            const auto decoded_data_idx = (row * this->ihdr.width + column) * COLOR_SIZE + (row + 1);
            const auto pixels_idx = row * ihdr.width + column;
            const ColorRGB color{decoded_data[decoded_data_idx + 0], decoded_data[decoded_data_idx + 1],
                                 decoded_data[decoded_data_idx + 2]};


            pixels[pixels_idx] = color;
        }

        const auto filter_idx = row * this->ihdr.width * COLOR_SIZE + row;
        const auto filter = decoded_data[filter_idx];

        switch (filter)
        {
            case Filters::NONE:
                break;
            case Filters::SUB:
                break;
            case Filters::AVG:
                break;
            case Filters::PAETH:
                break;
        }
    }

    return std::move(pixels);
}

auto PngCodec::apply_filter_sub(std::span<ColorRGB> &row) -> void
{}

auto PngCodec::apply_filter_up(std::span<ColorRGB> &row, std::span<ColorRGB> &up_row) const -> void
{}

auto PngCodec::apply_filter_avg(std::span<ColorRGB> &row, std::span<ColorRGB> &up_row,
                                std::span<ColorRGB> &down_row) const -> void
{}

auto PngCodec::apply_filter_paeth(std::span<ColorRGB> &row, std::span<ColorRGB> &up_row,
                                  std::span<ColorRGB> &down_row) const -> void
{}

auto PngCodec::is_signature_valid(std::byte *&cursor) -> bool
{
    for (size_t i = 0; i < TheKoder::PNG::Constants::SIGNATURE.size(); ++i)
    {
        if (cursor[i] != TheKoder::PNG::Constants::SIGNATURE[i])
        {
            return false;
        }
    }

    cursor += TheKoder::PNG::Constants::SIGNATURE.size();

    return true;
}
