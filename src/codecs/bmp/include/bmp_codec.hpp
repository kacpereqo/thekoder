//
// Created by remza on 28.12.2025.
//

#pragma once

#include "chunks.hpp"
#include "codec.hpp"
#include "colors.hpp"

namespace TheKoder::BMP
{

    class BMP
    {

    public:
        explicit BMP(ImageCursor cursor);

        [[nodiscard]] auto is_signature_valid() const -> bool;

        [[nodiscard]] auto decode_to_rgb8()   const -> std::vector<RGB8>;
        [[nodiscard]] auto decode_to_rgba8()  const -> std::vector<RGBA8>;
        [[nodiscard]] auto decode_to_rgb16()  const -> std::vector<RGB16>;
        [[nodiscard]] auto decode_to_rgba16() const -> std::vector<RGBA16>;

        [[nodiscard]] auto get_width() const -> std::size_t;
        [[nodiscard]] auto get_height() const -> std::size_t;

    private:
        Chunks::FileHeader header;
        Chunks::InfoHeader info_header;
        Chunks::ColorTable color_table;
        std::span<std::byte> data;

        template<typename T>
        auto decode() const ->  std::vector<T>;

    };
}