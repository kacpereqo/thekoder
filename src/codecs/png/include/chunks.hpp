//
// Created by remza on 28.12.2025.
//

#pragma once
#include <cstdint>
#include <string_view>
#include <array>
#include <vector>

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