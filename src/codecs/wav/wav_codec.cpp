//
// Created by debilian on 10.01.2026.
//

#include "include/wav_codec.hpp"

auto WAV::decode_8bit() -> std::vector<uint8_t>
{}
auto WAV::decode_16bit() -> std::vector<uint16_t>
{
    const auto sample_count = this->data.size / sizeof(uint16_t);
    std::vector<uint16_t> sounds;
    sounds.resize(sample_count);

    const auto data_cursor = reinterpret_cast<uint16_t*>(this->data.data.data());

    for (int i = 0; i < sample_count; i++)
        sounds[i] = data_cursor[i];

    return sounds;
}

template<typename T>
auto WAV::decode() -> std::vector<T>
{
    // const auto sample_count = this->data.size / sizeof(T);
    // std::vector<T> sounds;
    // sounds.resize(sample_count);
    //
    // for (int i = 0; i < sample_count; ++i)
    // {
    //     sounds[i] =
    // }
}
