//
// Created by remza on 11.01.2026
//

// https://www.w3.org/Graphics/GIF/spec-gif89a.txt

#pragma once

#include <array>
#include <cstdint>

#include "colors.hpp"
#include "utils/reading_value.hpp"

struct Signature
{
    std::array<char, 3> signature;
    std::array<char, 3> version;
};

struct LogicalScreenDescriptor
{
    uint16_t width;
    uint16_t height;
    // <Packed Fields>  =      Global Color Table Flag       1 Bit
    //                         Color Resolution              3 Bits
    //                         Sort Flag                     1 Bit
    //                         Size of Global Color Table    3 Bits
    std::byte packed_field;

    uint8_t background_color_index;
    uint8_t pixel_aspect_ratio;
};

struct GlobalColorTable
{
    std::span<RGB8> color_table;
};

struct ImageDescriptor
{
    uint8_t image_separator;
    uint32_t image_left_position;
    uint32_t image_top_position;
    uint32_t width;
    uint32_t height;
    std::byte packed_field;
    // <Packed Fields>  =      Local Color Table Flag        1 Bit
    //                         Interlace Flag                1 Bit
    //                         Sort Flag                     1 Bit
    //                         Reserved                      2 Bits
    //                         Size of Local Color Table     3 Bits
};

struct LocalColorTable
{
    std::span<RGB8> color_table;
};

struct TableBasedImageData
{
    uint8_t lzw_minimum_code_size;
    std::span<std::byte> image_data;
};

struct GraphicControlExtension
{
    uint8_t extension_introducer;
    uint8_t graphic_control_label;
    uint8_t block_size;
    std::byte packed_field;
    // <Packed Fields>  =     Reserved                      3 Bits
    //                      Disposal Method               3 Bits
    //                      User Input Flag               1 Bit
    //                      Transparent Color Flag        1 Bit
    uint8_t delay_time;
    uint8_t block_terminator;
};

