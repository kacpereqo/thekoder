#include <gtest/gtest.h>
#include <vector>
#include <array>
#include <cstdint>
#include "wav/chunks.hpp"

using namespace TheKoder::WAV::Chunks;

TEST(WavChunksTest, RiffChunkParsesCorrectly) {
    std::vector<uint8_t> bytes = {
        'R', 'I', 'F', 'F',
        0x24, 0x00, 0x00, 0x00,
        'W', 'A', 'V', 'E'
    };

    auto* cursor = reinterpret_cast<std::byte*>(bytes.data());

    const Riff riff(cursor);

    constexpr std::array expected_id = {'R', 'I', 'F', 'F'};
    constexpr std::array expected_format = {'W', 'A', 'V', 'E'};

    EXPECT_EQ(riff.chunk_id, expected_id);
    EXPECT_EQ(riff.chunk_size, 36);
    EXPECT_EQ(riff.format, expected_format);
}

TEST(WavChunksTest, FmtChunkParsesCorrectly) {
    std::vector<uint8_t> bytes = {
        'f', 'm', 't', ' ',
        16, 0, 0, 0,
        1, 0,
        2, 0,
        0x44, 0xAC, 0, 0,
        0x10, 0xB1, 0x02, 0,
        4, 0,
        16, 0
    };

    auto* cursor = reinterpret_cast<std::byte*>(bytes.data());

    Fmt fmt(cursor);

    constexpr std::array expected_id = {'f', 'm', 't', ' '};

    EXPECT_EQ(fmt.chunk_id, expected_id);
    EXPECT_EQ(fmt.chunk_size, 16);
    EXPECT_EQ(fmt.format, 1);
    EXPECT_EQ(fmt.channels, 2);
    EXPECT_EQ(fmt.sample_rate, 44100);
    EXPECT_EQ(fmt.byte_rate, 176400);
    EXPECT_EQ(fmt.block_align, 4);
    EXPECT_EQ(static_cast<uint16_t>(fmt.bits_per_sample), 16);
}

TEST(WavChunksTest, DataChunkParsesCorrectly) {
    std::vector<uint8_t> bytes = {
        'd', 'a', 't', 'a',
        4, 0, 0, 0,
        0xDE, 0xAD, 0xBE, 0xEF
    };

    auto* cursor = reinterpret_cast<std::byte*>(bytes.data());

    Data data(cursor);

    constexpr std::array expected_id = {'d', 'a', 't', 'a'};

    EXPECT_EQ(data.chunk_id, expected_id);
    EXPECT_EQ(data.size, 4);
    ASSERT_EQ(data.data.size(), 4);
    EXPECT_EQ(static_cast<uint8_t>(data.data[0]), 0xDE);
    EXPECT_EQ(static_cast<uint8_t>(data.data[3]), 0xEF);
}

