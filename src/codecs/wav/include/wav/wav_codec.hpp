//
// Created by debilian on 10.01.2026.
//

#pragma once
#include <cstdint>
#include <vector>
#include "chunks.hpp"


namespace TheKoder::WAV
{
    class WAV
    {
    public:
        Chunks::Riff riff;
        Chunks::Fmt fmt;
        Chunks::Data data;

        [[nodiscard]] auto decode_8bit() const -> std::vector<int8_t>;
        [[nodiscard]] auto decode_16bit() const -> std::vector<int16_t>;

        explicit WAV(DataCursor cursor) : riff(cursor), fmt(cursor), data(cursor)
        {}
    private:
        template<typename T>
        [[nodiscard]] auto decode() const -> std::vector<T>;
    };
}