//
// Created by remza on 28.12.2025.
//

#include "include/png_codec.hpp"

#include <cassert>

#include "include/constants.hpp"

#include <cmath>
#include <memory>
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


PngCodec::PngCodec(ImageCursor cursor) :  signature(cursor), ihdr(cursor)
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
    }

    this->bytes_per_pixel = get_bits_per_pixel(this->ihdr.color_type, this->ihdr.bit_depth) / 8;
    this->image_size = this->ihdr.width * this->ihdr.height;
    this->image_space = image_size * bytes_per_pixel;
}
auto PngCodec::decode_to_rgb8() -> std::vector<RGB8>
{
    // Validation
    if (ihdr.bit_depth != 8) throw std::runtime_error("Image is not 8-bit");
    if (ihdr.color_type != 2) throw std::runtime_error("Image is not RGB"); // Type 2 is RGB

    const std::vector<std::byte> raw = decode_raw();
    std::vector<RGB8> output;
    output.reserve(this->image_size);

    for (size_t i = 0; i < raw.size(); i += 3)
    {
        output.emplace_back(
            std::to_integer<uint8_t>(raw[i]),
            std::to_integer<uint8_t>(raw[i+1]),
            std::to_integer<uint8_t>(raw[i+2])
        );
    }

    return output;
}

auto PngCodec::decode_to_rgba8() const -> std::vector<RGBA8>
{
    if (ihdr.bit_depth != 8) throw std::runtime_error("Image is not 8-bit");

    const std::vector<std::byte> raw = decode_raw();
    std::vector<RGBA8> output;
    output.reserve(this->image_size);

    if (ihdr.color_type == 6) // Truecolor with Alpha (R, G, B, A)
    {
        for (size_t i = 0; i < raw.size(); i += 4)
        {
            output.emplace_back(
                std::to_integer<uint8_t>(raw[i]),
                std::to_integer<uint8_t>(raw[i+1]),
                std::to_integer<uint8_t>(raw[i+2]),
                std::to_integer<uint8_t>(raw[i+3])
            );
        }
    }
    else if (ihdr.color_type == 2) // Truecolor (R, G, B) -> Add opaque Alpha
    {
        for (size_t i = 0; i < raw.size(); i += 3)
        {
            output.emplace_back(
                std::to_integer<uint8_t>(raw[i]),
                std::to_integer<uint8_t>(raw[i+1]),
                std::to_integer<uint8_t>(raw[i+2]),
                255
            );
        }
    }
    else
    {
        throw std::runtime_error("Unsupported color type for RGBA8 conversion");
    }

    return output;
}

auto PngCodec::decode_to_rgb16() const -> std::vector<RGB16>
{
    if (ihdr.bit_depth != 16) throw std::runtime_error("Image is not 16-bit");
    if (ihdr.color_type != 2) throw std::runtime_error("Image is not RGB");

    const std::vector<std::byte> raw = decode_raw();
    std::vector<RGB16> output;
    output.reserve(this->image_size);

    for (size_t i = 0; i < raw.size(); i += 6)
    {
        uint16_t r = (std::to_integer<uint8_t>(raw[i]) << 8) | std::to_integer<uint8_t>(raw[i+1]);
        uint16_t g = (std::to_integer<uint8_t>(raw[i+2]) << 8) | std::to_integer<uint8_t>(raw[i+3]);
        uint16_t b = (std::to_integer<uint8_t>(raw[i+4]) << 8) | std::to_integer<uint8_t>(raw[i+5]);

        output.emplace_back(r, g, b);
    }

    return output;
}

auto PngCodec::decode_to_rgba16() const -> std::vector<RGBA16>
{
    if (ihdr.bit_depth != 16) throw std::runtime_error("Image is not 16-bit");
    if (ihdr.color_type != 6) throw std::runtime_error("Image is not RGBA");

    const std::vector<std::byte> raw = decode_raw();
    std::vector<RGBA16> output;
    output.reserve(this->image_size);

    for (size_t i = 0; i < raw.size(); i += this->bytes_per_pixel)
    {
        uint16_t r = (std::to_integer<uint8_t>(raw[i])   << 8) | std::to_integer<uint8_t>(raw[i+1]);
        uint16_t g = (std::to_integer<uint8_t>(raw[i+2]) << 8) | std::to_integer<uint8_t>(raw[i+3]);
        uint16_t b = (std::to_integer<uint8_t>(raw[i+4]) << 8) | std::to_integer<uint8_t>(raw[i+5]);
        uint16_t a = (std::to_integer<uint8_t>(raw[i+6]) << 8) | std::to_integer<uint8_t>(raw[i+7]);

        output.emplace_back(r, g, b, a);
    }

    return output;
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

auto PngCodec::decode_idat() const -> std::vector<std::byte>
{
    size_t vector_length = this->image_space + this->ihdr.height;

    std::vector<std::byte> decoded_data(vector_length);

    const auto result = uncompress(reinterpret_cast<Bytef *>(decoded_data.data()), &vector_length,
                                   reinterpret_cast<Bytef *>(idat.compressed_data.data()), idat.compressed_data.size());

    if (result != Z_OK)
    {
        const std::string error_message = "ZLIB compression failed" + std::to_string(result);
        throw std::runtime_error(error_message);
    }

    return decoded_data;
}

auto PngCodec::decode_raw() const -> std::vector<std::byte>
{

    auto decoded_data = this->decode_idat();

    std::vector<std::byte> pixels;
    pixels.reserve(this->image_space);

    for (int row = 0; row < this->ihdr.height; row++)
    {
        const auto filter_type = static_cast<FilterType>(decoded_data[transform_2d_to_1d(0, row)]);

        const std::span row_data{&decoded_data[transform_2d_to_1d(1, row)], this->ihdr.width * bytes_per_pixel};

        switch (filter_type)
        {
            case FilterType::Sub:
            {
                this->filter_sub(row_data);
                break;
            }
            case FilterType::Up:
                //    Up(x) + Prior(x)
            {
                assert(row == 0);
                const std::span upper_row{&pixels[transform_2d_to_1d(1, row - 1)],
                                          this->ihdr.width * bytes_per_pixel};
                this->filter_up(row_data, upper_row);
                break;
            }
                case FilterType::Average:
                //  Average(x) + floor( (Raw(x-bpp) + Prior(x))/2 )
            {
                assert(row == 0);
                const std::span upper_row{&pixels[transform_2d_to_1d(1, row - 1)],
                                          this->ihdr.width * bytes_per_pixel};

                this->filter_average(row_data, upper_row);
                break;
            }
                case FilterType::Paeth:
                //    Paeth(x) + PaethPredictor(Raw(x-bpp), Prior(x), Prior(x-bpp))
            {
                assert(row == 0);
                const std::span upper_row{&pixels[transform_2d_to_1d(1, row - 1)],
                                              this->ihdr.width * bytes_per_pixel};
                this->filter_paeth(row_data, upper_row);
                break;
            }
                case FilterType::None:
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
    assert(row.size() == up_row.size());

    for (size_t i = 0; i < row.size(); i++)
    {
        const auto x = std::to_integer<uint8_t>(row[i]);
        const auto a = std::to_integer<uint8_t>(up_row[i]);

        row[i] = static_cast<std::byte>((x + a) % 256);
    }
}
auto PngCodec::filter_average(std::span<std::byte> row, const std::span<std::byte> up_row) const -> void
{
    assert(row.size() == up_row.size());

    for (size_t i = 0; i < row.size(); i++)
    {
        const auto x = std::to_integer<uint8_t>(row[i]);

        const auto a = (i <= this->bytes_per_pixel)
        ? std::to_integer<uint8_t>(row[i - bytes_per_pixel])
        : 0;

        const auto b = std::to_integer<uint8_t>(up_row[i]);

        row[i] = static_cast<std::byte>(x + std::floor(a + b / 2));
    }
}

inline auto paeth_predcitor(const uint8_t a, const uint8_t b, const uint8_t c) -> uint8_t
{
    // https://www.w3.org/TR/PNG-Filters.html
    // 6.6. Filter type 4: Paeth

    const int32_t p = a + b - c;

    const int32_t pa = std::abs(p - a);
    const int32_t pb = std::abs(p - b);
    const int32_t pc = std::abs(p - c);

    if (pa <= pb && pb <= pc) return a;
    if (pb <= pc) return b;
    return c;
}

auto PngCodec::filter_paeth(std::span<std::byte> row, const std::span<std::byte> up_row) const -> void
{
    assert(row.size() == up_row.size());

    for (size_t i = 0; i < row.size(); i++)
    {
        const auto x = std::to_integer<uint8_t>(row[i]);

        const uint8_t a = (i >= bytes_per_pixel)
                    ? std::to_integer<uint8_t>(row[i - bytes_per_pixel])
                    : 0;
        const auto b = std::to_integer<uint8_t>(up_row[i]);

        const uint8_t c = (i >= bytes_per_pixel)
            ? std::to_integer<uint8_t>(up_row[i - bytes_per_pixel])
            : 0;

        const auto pa = paeth_predcitor(a, b, c);
        row[i] = static_cast<std::byte>((x + pa) % 256);
    }
}
