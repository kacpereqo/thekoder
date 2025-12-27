#include <iostream>
#include <fstream>
#include <vector>
#include <cstddef>
#include <cstdint>
#include <array>
#include <bit>
#include <cstring>
#include <zlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

class Logger{

public:
    static std::ostream& debug() {
        return log(DEBUG);
    }
    static std::ostream& info() {
        return log(INFO);
    }
    static std::ostream& warning() {
        return log(WARNING);
    }
    static std::ostream& error() {
        return log(ERROR);
    }


private:
    enum Level {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    static std::ostream& log(const Level level) {
        std::ostream& output = std::cout;

        switch (level) {
            case DEBUG:
                output << "[DEBUG] ";
                break;
            case INFO:
                output << "[INFO] ";
                break;
            case WARNING:
                output << "[WARNING] ";
                break;
            case ERROR:
                output << "[ERROR] ";
                break;
        }
        return output;
    }
};

constexpr std::array png_signature = {
    std::byte{0x89}, std::byte{0x50}, std::byte{0x4E}, std::byte{0x47},
    std::byte{0x0D}, std::byte{0x0A}, std::byte{0x1A}, std::byte{0x0A}
};

bool is_png(std::byte*& data) {
    for (size_t i = 0; i < png_signature.size(); ++i) {
        if (data[i] != png_signature[i]) {
            return false;
        }
    }
    data += png_signature.size();

    return true;
}

void skip_bytes(std::byte*& cursor, const size_t n) {
    cursor += n;
}

struct Chunk {
    uint32_t length{};
    std::array<char, 4> chunk_type{};
    std::vector<std::byte> data;
    std::array<std::byte,4 > crc{};

    size_t size() {
        return sizeof(length) + sizeof(chunk_type) + data.size() + sizeof(crc);
    }

    [[nodiscard]] std::string_view chunk_type_str() const {
        return {chunk_type.data(), chunk_type.size()};
    }
};

template <typename T>
T read_numerical_value(std::byte*& cursor) {
    T value{};
    std::memcpy(&value, cursor, sizeof(T));

    cursor += sizeof(T);

    value = std::byteswap(value);

    return value;
}

template <typename T>
T read_array_value(std::byte*& cursor) {
    T value{};
    std::memcpy(&value, cursor, sizeof(T));

    cursor += sizeof(T);

    return value;
}

std::vector<std::byte> read_bytes(std::byte*& cursor, const size_t n) {
    std::vector<std::byte> result;
    result.resize(n);
    std::memcpy(result.data(), cursor, n);
    cursor += n;

    return result;
}

Chunk read_chunk(std::byte*& cursor) {
    const auto length = read_numerical_value<uint32_t>(cursor);
    const auto chunk_type = read_array_value<std::array<char,4>>(cursor);
    const auto data = read_bytes(cursor, length);
    const auto crc = read_array_value<std::array<std::byte,4>>(cursor);

    return {length, chunk_type, data, crc};
}

std::ostream &operator<<(std::ostream& os, const Chunk& chunk) {
    os << "Chunk Length: " << chunk.length << std::endl;
    os << "Chunk Type: " << std::string(chunk.chunk_type.data(), chunk.chunk_type.size()) << std::endl;
    os << "Chunk Data Size: " << chunk.data.size() << " bytes" << std::endl;
    os << "Chunk CRC: ";
    for (const auto& byte : chunk.crc) {
        os << std::hex << std::to_integer<int>(byte) << " ";
    }
    os << std::dec << std::endl;

    return os;
}


struct IHDRData {
    uint32_t width;
    uint32_t height;
    uint8_t bit_depth;
    uint8_t color_type;
    uint8_t compression_method;
    uint8_t filter_method;
    uint8_t interlace_method;

    IHDRData() = default;
    explicit IHDRData(Chunk&& chunk) {
        std::byte* cursor = chunk.data.data();

        width = read_numerical_value<uint32_t>(cursor);
        height = read_numerical_value<uint32_t>(cursor);
        bit_depth = read_numerical_value<uint8_t>(cursor);
        color_type = read_numerical_value<uint8_t>(cursor);
        compression_method = read_numerical_value<uint8_t>(cursor);
        filter_method = read_numerical_value<uint8_t>(cursor);
        interlace_method = read_numerical_value<uint8_t>(cursor);

        Logger::debug() << "IHDR Data:" << std::endl;
        Logger::debug() << "Width: " << width << std::endl;
        Logger::debug() << "Height: " << height << std::endl;
        Logger::debug() << "Bit Depth: " << static_cast<int>(bit_depth) << std::endl;
        Logger::debug() << "Color Type: " << static_cast<int>(color_type) << std::endl;
        Logger::debug() << "Compression Method: " << static_cast<int>(compression_method) << std::endl;
        Logger::debug() << "Filter Method: " << static_cast<int>(filter_method) << std::endl;
        Logger::debug() << "Interlace Method: " << static_cast<int>(interlace_method) << std::endl;
    }
};

struct SRGBData {

};

struct GAMAData {
    uint32_t gamma{};

    GAMAData() = default;
    explicit GAMAData(Chunk&& chunk) {
        std::byte* cursor = chunk.data.data();

        gamma = read_numerical_value<uint32_t>(cursor);

        Logger::debug() << "gAMA Data:" << std::endl;
        Logger::debug() << "Gamma: " << gamma << std::endl;
    }
};

struct PHYSData {
    uint32_t pixels_per_unit_x{};
    uint32_t pixels_per_unit_y{};
    uint8_t unit{};

    PHYSData() = default;
    explicit PHYSData(Chunk&& chunk) {
        std::byte* cursor = chunk.data.data();

        pixels_per_unit_x = read_numerical_value<uint32_t>(cursor);
        pixels_per_unit_y = read_numerical_value<uint32_t>(cursor);
        unit = read_numerical_value<uint8_t>(cursor);

        Logger::debug() << "pHYs Data:" << std::endl;
        Logger::debug() << "Pixels per unit X: " << pixels_per_unit_x << std::endl;
        Logger::debug() << "Pixels per unit Y: " << pixels_per_unit_y << std::endl;
        Logger::debug() << "Unit: " << static_cast<int>(unit) << std::endl;
    }
};

struct IDATData {
    std::vector<std::byte> compressed_data;
    IDATData() = default;
    IDATData(Chunk&& chunk) {
        compressed_data = std::move(chunk.data);
    }
};

namespace ChunkType {
    constexpr auto IEND = "IEND";
    constexpr auto IHDR = "IHDR";
    constexpr auto SRGB = "sRGB";
    constexpr auto PNG = "‰PNG";
    constexpr auto GAMA = "gAMA";
    constexpr auto PHYS = "pHYs";
    constexpr auto IDAT = "IDAT";
};

int main(int argc, char* argv[]) {
    const std::string filename = "../zmitac.png";

    std::ifstream filestream(filename, std::ios::binary | std::ios::ate);

    if (!filestream) {

        Logger::debug() << "Failed to open file " << filename;
        std::exit(EXIT_FAILURE);

    }

    std::streamsize size = filestream.tellg();

    filestream.seekg(0, std::ios::beg);

    std::vector<std::byte> image_data(size);
    auto image_data_cursor = image_data.data();

    if ( filestream.read(reinterpret_cast<char*>(image_data.data()), size)) {

        Logger::debug() << "Image data read from file: " << filename << std::endl;

    } else {

        Logger::debug() << "Failed to read image data from file: " << filename << std::endl;
        std::exit(EXIT_FAILURE);

    }

    if (is_png(image_data_cursor))
        Logger::debug() << "The file is a valid PNG image" << std::endl;
    else
        Logger::warning() << "The file is not a valid PNG image" << std::endl;

    Chunk chunk;
    IHDRData hdr_data{};
    GAMAData gama_data{};
    PHYSData phys_data{};
    IDATData idat_data{};

    while (chunk.chunk_type_str() != ChunkType::IEND) {
        chunk = read_chunk(image_data_cursor);

        const auto chunk_type = chunk.chunk_type_str();

        if (chunk_type == ChunkType::IHDR) hdr_data = IHDRData{std::move(chunk)};
        else if (chunk_type == ChunkType::SRGB) {}
        else if (chunk_type == ChunkType::GAMA) gama_data = GAMAData{std::move(chunk)};
        else if (chunk_type == ChunkType::PHYS) phys_data = PHYSData{std::move(chunk)};
        else if (chunk_type == ChunkType::IDAT) idat_data = IDATData{std::move(chunk)};
        else if (chunk_type == ChunkType::IEND) {}
        else                                    {}
    }

    uLongf vector_length = hdr_data.width * hdr_data.height * 3 + hdr_data.height;

    std::vector<uint8_t> pixels;
    pixels.reserve(vector_length);

    auto result = uncompress(pixels.data(),
        &vector_length,
    reinterpret_cast<Bytef*>(idat_data.compressed_data.data()),
idat_data.compressed_data.size());

    if (result == Z_OK) {
        Logger::debug() << "Uncompressed data read from file: " << filename << std::endl;
    } else {
        Logger::debug() << "Failed to uncompress data from file: " << filename << std::endl;
    }


    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    SDL_SetAppMetadata(filename.c_str(), "1.0.0", "PNG Viewer");

    if (!SDL_Init(SDL_INIT_VIDEO) != 0) {
        Logger::error() << "SDL_Init Error: " << SDL_GetError() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    if (!SDL_CreateWindowAndRenderer(filename.c_str(), hdr_data.width, hdr_data.height, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetRenderLogicalPresentation(renderer, hdr_data.width, hdr_data.height, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    SDL_FRect rect;
    int scale = 1;
    rect.w = scale;
    rect.h = scale;

    for (uint32_t y = 0; y < hdr_data.height; y++) {
        const uint8_t filter_type = pixels[y * (hdr_data.width * 3 + 1)];

        for (uint32_t x = 0; x < hdr_data.width; x++) {
            const uint8_t r = pixels[y * (hdr_data.width * 3 + 1) + 1 + x * 3 + 0];
            const uint8_t g = pixels[y * (hdr_data.width * 3 + 1) + 1 + x * 3 + 1];
            const uint8_t b = pixels[y * (hdr_data.width * 3 + 1) + 1 + x * 3 + 2];

            SDL_SetRenderDrawColor(renderer, r, g, b, 255);

            rect.x = x * scale; // position scaled
            rect.y = y * scale;
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    SDL_RenderPresent(renderer);

    for (;;) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return SDL_APP_SUCCESS;
            }
        }

    }

    return SDL_APP_CONTINUE;
}
