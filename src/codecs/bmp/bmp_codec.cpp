#include <utility>

#include "bmp_codec.hpp"

using namespace TheKoder::BMP;
using namespace TheKoder::BMP::Chunks;

BMP::BMP(ImageCursor cursor) :  header(cursor), info_header(cursor)
{
    if (const auto bits_per_pixel = this->info_header.bits_per_pixel; bits_per_pixel == BitsPerPixel::monochrome || bits_per_pixel == BitsPerPixel::palette_4 || bits_per_pixel == BitsPerPixel::palette_8)
    {
        this->color_table = ColorTable{cursor};
    }

    this->data = std::span(cursor, this->info_header.size);
}

auto BMP::decode_to_rgb8() const -> std::vector<RGB8>
{
    return this->decode<RGB8>();
}

auto BMP::decode_to_rgba8() const -> std::vector<RGBA8>
{
    return this->decode<RGBA8>();
}

auto BMP::decode_to_rgb16() const -> std::vector<RGB16>
{
    return this->decode<RGB16>();
}

auto BMP::decode_to_rgba16() const -> std::vector<RGBA16>
{
    return this->decode<RGBA16>();
}

template<typename T>
auto BMP::decode() const ->  std::vector<T>
{
    const auto image_size = this->info_header.width * this->info_header.height;
    std::vector<T> pixels(image_size);

    for (int i = 0; i < image_size; i++)
    {
        switch (this->info_header.bits_per_pixel)
        {
            case BitsPerPixel::monochrome: break;
            case BitsPerPixel::palette_4: break;
            case BitsPerPixel::palette_8:
            {
                RGBA8 pixel {this->color_table.colors[std::to_underlying(this->data[i])]};
                pixel.a = 255;

                pixels[i] = static_cast<T>(pixel);
                break;
            }
            case BitsPerPixel::rgb_16: break;
            case BitsPerPixel::rgb_24: break;
            case BitsPerPixel::rgb_32: break;
        }
    }

    return pixels;
}
auto BMP::is_signature_valid() const -> bool
{
    if (this->header.signature[0] != 'B' || this->header.signature[1] != 'M')
        return false;
    return true;
}

auto BMP::get_width() const -> std::size_t
{
    return this->info_header.width;
}

auto BMP::get_height() const -> std::size_t
{
    return this->info_header.height;
}
