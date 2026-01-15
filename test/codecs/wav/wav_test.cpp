//
// Created by debilian on 15.01.2026.
//

#include <gtest/gtest.h>
#include <vector>
#include <array>
#include <cstdint>
#include "wav/wav_codec.hpp"

TEST(WavIntegrationTest, ParsesWholeGoldenFile) {
    std::vector<uint8_t> golden_file = {
        // RIFF Header
        'R', 'I', 'F', 'F',
        40, 0, 0, 0,            // ChunkSize: 36 + 4 bytes data = 40
        'W', 'A', 'V', 'E',

        // FMT Chunk
        'f', 'm', 't', ' ',
        16, 0, 0, 0,            // Subchunk1Size: 16
        1, 0,                   // AudioFormat: 1 (PCM)
        2, 0,                   // NumChannels: 2 (Stereo)
        0x44, 0xAC, 0, 0,       // SampleRate: 44100
        0x10, 0xB1, 0x02, 0,    // ByteRate: 176400 (44100 * 2 * 16/8)
        4, 0,                   // BlockAlign: 4
        16, 0,                  // BitsPerSample: 16

        // DATA Chunk
        'd', 'a', 't', 'a',
        4, 0, 0, 0,             // Subchunk2Size: 4 bytes

        // Payload (1 sample, stereo, 16-bit)
        0x01, 0x00,             // Left: 1
        0xFF, 0xFF              // Right: -1
    };

    auto* cursor = reinterpret_cast<std::byte*>(golden_file.data());

    TheKoder::WAV::WAV wav(cursor);

    // RIFF Assertions
    constexpr std::array expected_riff = {'R', 'I', 'F', 'F'};
    constexpr std::array expected_wave = {'W', 'A', 'V', 'E'};
    EXPECT_EQ(wav.riff.chunk_id, expected_riff);
    EXPECT_EQ(wav.riff.chunk_size, 40);
    EXPECT_EQ(wav.riff.format, expected_wave);

    // FMT Assertions
    constexpr std::array expected_fmt = {'f', 'm', 't', ' '};
    EXPECT_EQ(wav.fmt.chunk_id, expected_fmt);
    EXPECT_EQ(wav.fmt.format, 1);
    EXPECT_EQ(wav.fmt.channels, 2);
    EXPECT_EQ(wav.fmt.sample_rate, 44100);
    EXPECT_EQ(wav.fmt.byte_rate, 176400);
    EXPECT_EQ(wav.fmt.block_align, 4);
    EXPECT_EQ(static_cast<uint16_t>(wav.fmt.bits_per_sample), 16);

    // Data Assertions
    constexpr std::array expected_data = {'d', 'a', 't', 'a'};
    EXPECT_EQ(wav.data.chunk_id, expected_data);
    EXPECT_EQ(wav.data.size, 4);

    ASSERT_EQ(wav.data.data.size(), 4);
    // Verify payload content via span
    EXPECT_EQ(static_cast<uint8_t>(wav.data.data[0]), 0x01); // L low
    EXPECT_EQ(static_cast<uint8_t>(wav.data.data[1]), 0x00); // L high
    EXPECT_EQ(static_cast<uint8_t>(wav.data.data[2]), 0xFF); // R low
    EXPECT_EQ(static_cast<uint8_t>(wav.data.data[3]), 0xFF); // R high
}

TEST(WavIntegrationTest, Decodes16BitSignedAudio) {
    std::vector<uint8_t> raw_bytes = {
        // RIFF Header
        'R', 'I', 'F', 'F',
        44, 0, 0, 0,            // ChunkSize: 36 + 8 bytes data = 44
        'W', 'A', 'V', 'E',

        // FMT Chunk
        'f', 'm', 't', ' ',
        16, 0, 0, 0,            // Subchunk1Size: 16
        1, 0,                   // AudioFormat: 1 (PCM)
        1, 0,                   // NumChannels: 1 (Mono)
        0x44, 0xAC, 0, 0,       // SampleRate: 44100
        0x88, 0x58, 0x01, 0,    // ByteRate: 88200 (44100 * 1 * 2)
        2, 0,                   // BlockAlign: 2
        16, 0,                  // BitsPerSample: 16

        // DATA Chunk
        'd', 'a', 't', 'a',
        8, 0, 0, 0,             // Subchunk2Size: 8 bytes (4 samples)

        // Payload (Little Endian)
        0x00, 0x00,             // Sample 1: 0
        0xE8, 0x03,             // Sample 2: 1000  (0x03E8)
        0x18, 0xFC,             // Sample 3: -1000 (0xFC18)
        0x00, 0x80              // Sample 4: -32768 (Min int16, 0x8000)
    };

    auto* cursor = reinterpret_cast<std::byte*>(raw_bytes.data());

    TheKoder::WAV::WAV wav(cursor);

    // Assuming decode_16bit returns std::vector<int16_t>
    const auto samples = wav.decode_16bit();

    ASSERT_EQ(samples.size(), 4);
    EXPECT_EQ(samples[0], 0);
    EXPECT_EQ(samples[1], 1000);
    EXPECT_EQ(samples[2], -1000);
    EXPECT_EQ(samples[3], -32768);
}