//
// Created by remza on 28.12.2025.
//

#include "include/png_codec.hpp"
#include "include/constants.hpp"

#include <cmath>
#include <memory>
#include <utility>
#include <zlib.h>

inline void skip_chunk(ImageCursor cursor)
{
    const auto length = read_numerical_value<uint32_t, std::endian::big>(cursor);
    cursor += length + 8;
}


inline uint32_t get_bits_per_pixel(const uint8_t color_type, const uint8_t bit_depth)
{
    uint32_t channels = 0;

    switch (color_type)
    {
        case 0:
            channels = 1;
            break; // Grayscale
        case 2:
            channels = 3;
            break; // RGB
        case 3:
            channels = 1;
            break; // Palette / Indexed
        case 4:
            channels = 2;
            break; // Grayscale + Alpha
        case 6:
            channels = 4;
            break; // RGBA
        default:
            return 0;
    }

    return bit_depth * channels;
}


PngCodec::PngCodec(ImageCursor cursor) : zero_cursor(cursor), signature(cursor), ihdr(cursor)
{
    while (true)
    {
        // read just chunk name
        // +4 cause 00h-03h is chunk length, and must skip those bytes
        const auto begin = reinterpret_cast<char *>(cursor + 4);
        const std::string_view chunk_name = {begin, 4};

        if (chunk_name == ChunkType::SRGB)
            skip_chunk(cursor);
        else if (chunk_name == ChunkType::PHYS)
            this->phys = Phys(cursor);
        else if (chunk_name == ChunkType::IDAT)
            this->idat = Idat(cursor);
        else if (chunk_name == ChunkType::GAMA)
            this->gama = Gama(cursor);
        else if (chunk_name == ChunkType::IEND)
            break;
        else
            break;
    }

    this->bytes_per_pixel = get_bits_per_pixel(this->ihdr.color_type, this->ihdr.bit_depth) / 8;
}
auto PngCodec::decode_to_rgb8() -> std::vector<RGB8>
{
    return {};
}
auto PngCodec::decode_to_rgba8() -> std::vector<RGBA8>
{
    return {};
}
auto PngCodec::decode_to_rgb16() -> std::vector<RGB16>
{
    return {};
}
auto PngCodec::decode_to_rgba16() -> std::vector<RGBA16>
{
    return {};
}


auto PngCodec::transform_2d_to_1d(const uint32_t x, const uint32_t y) const -> uint32_t
{
    const auto width = this->ihdr.width * bytes_per_pixel + 1;

    return width * y + x;
}

enum class FilterType
{
    None = 0,
    Sub = 1,
    Up = 2,
    Average = 3,
    Paeth = 4
};

auto PngCodec::decode_raw() const -> std::vector<std::byte>
{
    const auto bytes_per_pixel = get_bits_per_pixel(this->ihdr.color_type, this->ihdr.bit_depth) / 8;

    const auto rgb_space = this->ihdr.width * this->ihdr.height * bytes_per_pixel;
    const auto filter_space = this->ihdr.height;

    uLongf vector_length = rgb_space + filter_space;

    std::vector<std::byte> decoded_data(vector_length);

    const auto result = uncompress(reinterpret_cast<Bytef *>(decoded_data.data()), &vector_length,
                                   reinterpret_cast<Bytef *>(idat.compressed_data.data()), idat.compressed_data.size());

    if (result != Z_OK)
    {
        const std::string error_message = "ZLIB compression failed" + std::to_string(result);
        throw std::domain_error(error_message);
    }

    std::vector<std::byte> pixels;
    pixels.reserve(this->ihdr.width * bytes_per_pixel * this->ihdr.height);

    for (int row = 0; row < this->ihdr.height; row++)
    {
        const auto filter_type = static_cast<FilterType>(decoded_data[transform_2d_to_1d(0, row)]);

        const std::span row_data{&decoded_data[transform_2d_to_1d(1, row)], this->ihdr.width * bytes_per_pixel};

        switch (filter_type)
        {
            case FilterType::None:
                break;
            case FilterType::Sub:
                this->filter_sub(row_data);
                break;
            case FilterType::Up:
                //    Up(x) + Prior(x)

                {
                    const std::span upper_row{&pixels[transform_2d_to_1d(1, row - 1)],
                                              this->ihdr.width * bytes_per_pixel};
                    this->filter_up(row_data, upper_row);
                }
            case FilterType::Average:
                //  Average(x) + floor( (Raw(x-bpp) + Prior(x))/2 )

                {
                    const std::span upper_row{&pixels[transform_2d_to_1d(1, row - 1)],
                                              this->ihdr.width * bytes_per_pixel};

                    for (auto i = bytes_per_pixel; i < row_data.size(); i++)
                    {
                        const auto x = std::to_integer<uint8_t>(row_data[i]);
                        const auto a = std::to_integer<uint8_t>(row_data[i - bytes_per_pixel]);
                        const auto b = std::to_integer<uint8_t>(upper_row[i]);

                        row_data[i] = static_cast<std::byte>(x + std::floor(a + b / 2));
                    }
                    break;
                }
            case FilterType::Paeth:
                break;
        }

        pixels.insert(pixels.end(), row_data.begin(), row_data.end());
    }

    return pixels;
}

auto PngCodec::filter_sub(std::span<std::byte> row) const -> void
{
    for (size_t i = bytes_per_pixel; i < row.size(); i++)
    {
        const auto x = std::to_integer<uint8_t>(row[i]);
        const auto a = std::to_integer<uint8_t>(row[i - bytes_per_pixel]);

        row[i] = static_cast<std::byte>((x + a) % 256);
    }
}
auto PngCodec::filter_up(std::span<std::byte> row, const std::span<std::byte> up_row) const -> void
{
    for (auto i = 0; i < row.size(); i++)
    {
        const auto x = std::to_integer<uint8_t>(row[i]);
        const auto a = std::to_integer<uint8_t>(up_row[i]);

        row[i] = static_cast<std::byte>((x + a) % 256);
    }
}
auto PngCodec::filter_average(std::span<std::byte> row, std::span<std::byte> up_row) const -> void
{}
auto PngCodec::filter_paeth(std::span<std::byte> row, std::span<std::byte> up_row) const -> void
{}
