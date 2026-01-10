#include <utility>

#include "bmp_codec.hpp"

BMP::BMP(ImageCursor cursor) :  header(cursor), info_header(cursor)
{
    if (const auto bits_per_pixel = this->info_header.bits_per_pixel; bits_per_pixel == BitsPerPixel::monochrome || bits_per_pixel == BitsPerPixel::palette_4 || bits_per_pixel == BitsPerPixel::palette_8)
    {
        this->color_table = ColorTable{cursor};
    }

    this->data = std::span(cursor, this->info_header.size);
}

auto BMP::decode_to_rgb8() -> std::vector<RGB8>
{
    return this->decode<RGB8>();
}

auto BMP::decode_to_rgba8() -> std::vector<RGBA8>
{
    return this->decode<RGBA8>();
}

auto BMP::decode_to_rgb16() -> std::vector<RGB16>
{
    return this->decode<RGB16>();
}

auto BMP::decode_to_rgba16() -> std::vector<RGBA16>
{
    return this->decode<RGBA16>();
}

template<typename T>
auto BMP::decode() -> std::vector<T>
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
            case BitsPerPixel::palette_16: break;
            case BitsPerPixel::palette_24: break;
        }
    }

    return pixels;
}
