#include <iostream>
#include <fstream>
#include <vector>
#include <cstddef>
#include <cstdint>
#include <array>
#include <bit>
#include <cstring>
#include <zlib.h>
//#include <SDL3/SDL.h>
//#include <SDL3/SDL_main.h>
#include "logger.hpp"
#include "codecs/png/include/constants.hpp"



//bool is_png(std::byte*& data) {
//    for (size_t i = 0; i < TheKoder::PNG::Constants::PNG_SIGNATURE.size(); ++i) {
//        if (data[i] != TheKoder::PNG::Constants::PNG_SIGNATURE[i]) {
//            return false;
//        }
//    }
//
//    data += TheKoder::PNG::Constants::PNG_SIGNATURE.size();
//    return true;
//}
//
//void skip_bytes(std::byte*& cursor, const size_t n) {
//    cursor += n;
//}
//
//
//template <typename T>
//T read_numerical_value(std::byte*& cursor) {
//    T value{};
//    std::memcpy(&value, cursor, sizeof(T));
//
//    cursor += sizeof(T);
//
//    value = std::byteswap(value);
//
//    return value;
//}
//
//template <typename T>
//T read_array_value(std::byte*& cursor) {
//    T value{};
//    std::memcpy(&value, cursor, sizeof(T));
//
//    cursor += sizeof(T);
//
//    return value;
//}
//
//std::vector<std::byte> read_bytes(std::byte*& cursor, const size_t n) {
//    std::vector<std::byte> result;
//    result.resize(n);
//    std::memcpy(result.data(), cursor, n);
//    cursor += n;
//
//    return result;
//}
//
//Chunk read_chunk(std::byte*& cursor) {
//    const auto length = read_numerical_value<uint32_t>(cursor);
//    const auto chunk_type = read_array_value<std::array<char,4>>(cursor);
//    const auto data = read_bytes(cursor, length);
//    const auto crc = read_array_value<std::array<std::byte,4>>(cursor);
//
//    return {length, chunk_type, data, crc};
//}
//
//std::ostream &operator<<(std::ostream& os, const Chunk& chunk) {
//    os << "Chunk Length: " << chunk.length << std::endl;
//    os << "Chunk Type: " << std::string(chunk.chunk_type.data(), chunk.chunk_type.size()) << std::endl;
//    os << "Chunk Data Size: " << chunk.data.size() << " bytes" << std::endl;
//    os << "Chunk CRC: ";
//    for (const auto& byte : chunk.crc) {
//        os << std::hex << std::to_integer<int>(byte) << " ";
//    }
//    os << std::dec << std::endl;
//
//    return os;
//}

int main(int argc, char* argv[]) {
    const std::string filename = "../zmitac.png";

    std::ifstream filestream(filename, std::ios::binary | std::ios::ate);

    if (!filestream) {

        Logger::debug() << "Failed to open file " << filename;
        std::exit(EXIT_FAILURE);

    }
    Logger::info() << "Successfully opened file " << filename;

    std::streamsize size = filestream.tellg();

    filestream.seekg(0, std::ios::beg);

    std::vector<std::byte> image_data(size);
    auto image_data_cursor = image_data.data();


    return 0;
}
