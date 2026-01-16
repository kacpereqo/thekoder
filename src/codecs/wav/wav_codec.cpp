//
// Created by debilian on 10.01.2026.
//

#include "include/wav/wav_codec.hpp"
auto TheKoder::WAV::WAV::decode_8bit() const -> std::vector<int8_t>
{
    return this->decode<int8_t>();
}

auto TheKoder::WAV::WAV::decode_16bit() const -> std::vector<int16_t>
{
    return this->decode<int16_t>();
}

template<typename T>
auto TheKoder::WAV::WAV::decode() const -> std::vector<T>
{
    const auto sample_count = this->data.size / sizeof(T);
    std::vector<T> sounds(sample_count);

    std::memcpy(sounds.data(), this->data.data.data(), data.data.size());

    return sounds;
}
