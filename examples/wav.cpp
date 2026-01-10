#include <cstddef>
#include <fstream>
#include <iostream>
#include <vector>
#include <SDL3/SDL.h>

#include "codecs/wav/include/wav_codec.hpp"
#include "logger/include/logger.hpp"
#include "wav_codec.hpp"

auto read_file_data(const std::string_view filename)
{

    std::ifstream filestream(filename.data(), std::ios::binary | std::ios::ate);

    if (!filestream)
    {

        Logger::debug() << "Failed to open file " << filename;
        std::exit(EXIT_FAILURE);
    }
    Logger::info() << "Successfully opened file " << filename;

    const std::streamsize size = filestream.tellg();

    filestream.seekg(0, std::ios::beg);

    std::vector<std::byte> image_data(size);

    if (filestream.read(reinterpret_cast<char *>(image_data.data()), size))
    {
        Logger::debug() << "Image data read from file: " << filename << std::endl;
    } else
    {
        Logger::debug() << "Failed to read image data from file: " << filename << std::endl;
        std::exit(EXIT_FAILURE);
    }

    filestream.close();


    return image_data;
}

int main(int argc, char **argv)
{
    constexpr std::string_view filename = "../examples/beeb_boop.wav";

    auto image_data = read_file_data(filename);
    auto data_cursor = image_data.data();

    WAV wav(data_cursor);
    const auto sounds = wav.decode_16bit();

    if (!SDL_Init(SDL_INIT_AUDIO)) {
        std::cerr << "SDL Init Failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_AudioSpec spec;
    SDL_zero(spec);

    spec.freq = wav.fmt.sample_rate;
    spec.channels = wav.fmt.channels;
    spec.format = SDL_AUDIO_S16LE;

    SDL_AudioDeviceID device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (device == 0) {
        std::cerr << "Failed to open audio device: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_AudioStream* stream = SDL_CreateAudioStream(&spec, nullptr);

    if (!stream) {
        std::cerr << "Failed to create stream: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (!SDL_BindAudioStream(device, stream)) {
        std::cerr << "Failed to bind stream: " << SDL_GetError() << std::endl;
        return -1;
    }

    size_t byte_size = sounds.size() * sizeof(int16_t);

    if (!SDL_PutAudioStreamData(stream, sounds.data(), byte_size)) {
        std::cerr << "Failed to put data: " << SDL_GetError() << std::endl;
    }

    SDL_ResumeAudioDevice(device);

    while (SDL_GetAudioStreamAvailable(stream) > 0) {
        SDL_Delay(100);
    }

    SDL_DestroyAudioStream(stream);
    SDL_CloseAudioDevice(device);
    SDL_Quit();

    return 0;

}