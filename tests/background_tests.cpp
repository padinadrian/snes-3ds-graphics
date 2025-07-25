#include <gtest/gtest.h>

extern "C" {
    #include "background.h"
}

namespace {

TEST(BackgroundTest, TranslateTileTest1)
{
    uint8_t input[16] = {
        0x7C, 0x00, 0xBA, 0x7C, 0x82, 0x7C, 0x7C, 0x00,
        0x10, 0x00, 0xD6, 0x00, 0x7C, 0x00, 0x38, 0x00
    };

    uint32_t expected[64] = {
        0, 1, 1, 1, 1, 1, 0, 0,
        1, 2, 3, 3, 3, 2, 1, 0,
        1, 2, 2, 2, 2, 2, 1, 0,
        0, 1, 1, 1, 1, 1, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0,
        1, 1, 0, 1, 0, 1, 1, 0,
        0, 1, 1, 1, 1, 1, 0, 0,
        0, 0, 1, 1, 1, 0, 0, 0,
    };

    uint32_t output[64] = {};

    translate_tile(output, input, 16, 2);

    for (size_t i = 0; i < 64; ++i) {
        ASSERT_EQ(expected[i], output[i]);
    }
}

TEST(TileTest, TileEncode2bpp)
{
    // A single tile is 8x8 pixels
    // 8 * 8 = 64 color IDs
    const uint8_t tile_buffer[64] = {
        0, 2, 0, 3, 1, 3, 0, 3,
        0, 2, 0, 3, 1, 3, 0, 3,
        0, 2, 0, 3, 1, 3, 0, 3,
        0, 2, 0, 3, 1, 3, 0, 3,
        0, 2, 0, 3, 1, 3, 0, 3,
        0, 2, 0, 3, 1, 3, 0, 3,
        0, 2, 0, 3, 1, 3, 0, 3,
        0, 2, 0, 3, 1, 3, 0, 3,
    };

    // Each row is 2 bytes
    // 16 bytes defines 1 tile
    uint16_t vram[8] = {};

    tile_encode_2bpp(vram, tile_buffer);
    for (size_t i = 0; i < 8; ++i) {
        EXPECT_EQ(vram[i], 0x551D);
    }
}

TEST(TileTest, TileEncode4bpp) {
    // A single tile is 8x8 pixels
    // 8 * 8 = 64 color IDs
    const uint8_t tile_buffer[] = {
        1, 3, 5, 7, 8, 10, 12, 14,
        1, 3, 5, 7, 8, 10, 12, 14,
        1, 3, 5, 7, 8, 10, 12, 14,
        1, 3, 5, 7, 8, 10, 12, 14,
        1, 3, 5, 7, 8, 10, 12, 14,
        1, 3, 5, 7, 8, 10, 12, 14,
        1, 3, 5, 7, 8, 10, 12, 14,
        1, 3, 5, 7, 8, 10, 12, 14,
    };

    // Each row is 4 bytes
    // 32 bytes defines 1 tile
    uint16_t vram[16] = {};

    //       1         3         5         7
    // 0 0 0 1   0 0 1 1   0 1 0 1   0 1 1 1

    //       8        10        12        14
    // 1 0 0 0   1 0 1 0   1 1 0 0   1 1 1 0

    // Bits 0: 1 1 1 1 0 0 0 0 = 0xF0
    // Bits 1: 0 1 0 1 0 1 0 1 = 0x55
    // Bits 2: 0 0 1 1 0 0 1 1 = 0x33
    // Bits 3: 0 0 0 0 1 1 1 1 = 0x0F

    tile_encode_4bpp(vram, tile_buffer);
    for (size_t i = 0; i < 8; ++i) {
        EXPECT_EQ(vram[i], 0x55F0);
    }
    for (size_t i = 8; i < 16; ++i) {
        EXPECT_EQ(vram[i], 0x0F33);
    }
}

TEST(TileTest, TileEncode8bpp) {
    // A single tile is 8x8 pixels
    // 8 * 8 = 64 color IDs
    const uint8_t tile_buffer[] = {
        0x21, 0x43, 0x65, 0x87, 0x98, 0xBA, 0xDC, 0xFE,
        0x21, 0x43, 0x65, 0x87, 0x98, 0xBA, 0xDC, 0xFE,
        0x21, 0x43, 0x65, 0x87, 0x98, 0xBA, 0xDC, 0xFE,
        0x21, 0x43, 0x65, 0x87, 0x98, 0xBA, 0xDC, 0xFE,
        0x21, 0x43, 0x65, 0x87, 0x98, 0xBA, 0xDC, 0xFE,
        0x21, 0x43, 0x65, 0x87, 0x98, 0xBA, 0xDC, 0xFE,
        0x21, 0x43, 0x65, 0x87, 0x98, 0xBA, 0xDC, 0xFE,
        0x21, 0x43, 0x65, 0x87, 0x98, 0xBA, 0xDC, 0xFE,
    };

    // Each row is 8 bytes
    // 1 tile is 64 bytes
    uint16_t vram[32] = {};

    //       0x21        0x43        0x65        0x87
    //  0010 0001   0100 0011   0110 0101   1000 0111

    //       0x98        0xBA        0xDC        0xFE
    //  1001 1000   1011 1010   1101 1100   1111 1110

    // Bits 0: 11110000 = 0xF0
    // Bits 1: 01010101 = 0x55
    // Bits 2: 00110011 = 0x33
    // Bits 3: 00001111 = 0x0F
    // Bits 4: 00001111 = 0x0F
    // Bits 5: 10100101 = 0xA5
    // Bits 6: 01100011 = 0x63
    // Bits 7: 00011111 = 0x1F

    tile_encode_8bpp(vram, tile_buffer);
    for (size_t i = 0; i < 8; ++i) {
        EXPECT_EQ(vram[i], 0x55F0);
    }
    for (size_t i = 8; i < 16; ++i) {
        EXPECT_EQ(vram[i], 0x0F33);
    }
    for (size_t i = 16; i < 24; ++i) {
        EXPECT_EQ(vram[i], 0xA50F);
    }
    for (size_t i = 24; i < 32; ++i) {
        EXPECT_EQ(vram[i], 0x1F63);
    }
}

TEST(TileTest, TileDecode2bpp)
{
    // Input VRAM: 16 bytes (8 words) is one tile.
    const uint16_t vram[8] = {
        0x551D, 0x551D, 0x551D, 0x551D, 0x551D, 0x551D, 0x551D, 0x551D
    };

    // A single tile is 8x8 pixels
    // 8 * 8 = 64 color IDs
    const uint8_t expected_tiles[64] = {
        0, 2, 0, 3, 1, 3, 0, 3,
        0, 2, 0, 3, 1, 3, 0, 3,
        0, 2, 0, 3, 1, 3, 0, 3,
        0, 2, 0, 3, 1, 3, 0, 3,
        0, 2, 0, 3, 1, 3, 0, 3,
        0, 2, 0, 3, 1, 3, 0, 3,
        0, 2, 0, 3, 1, 3, 0, 3,
        0, 2, 0, 3, 1, 3, 0, 3,
    };

    uint8_t tile_buffer[64] = {};

    tile_decode_2bpp(tile_buffer, vram);
    for (size_t i = 0; i < 64; ++i) {
        EXPECT_EQ(tile_buffer[i], expected_tiles[i]);
    }
}

TEST(TileTest, TileDecode4bpp)
{
    // Input VRAM: 32 bytes (16 words) is one tile.
    const uint16_t vram[16] = {
        0x55F0, 0x55F0, 0x55F0, 0x55F0, 0x55F0, 0x55F0, 0x55F0, 0x55F0,
        0x0F33, 0x0F33, 0x0F33, 0x0F33, 0x0F33, 0x0F33, 0x0F33, 0x0F33,
    };

    // A single tile is 8x8 pixels
    // 8 * 8 = 64 color IDs
    const uint8_t expected_tiles[64] = {
        1, 3, 5, 7, 8, 10, 12, 14,
        1, 3, 5, 7, 8, 10, 12, 14,
        1, 3, 5, 7, 8, 10, 12, 14,
        1, 3, 5, 7, 8, 10, 12, 14,
        1, 3, 5, 7, 8, 10, 12, 14,
        1, 3, 5, 7, 8, 10, 12, 14,
        1, 3, 5, 7, 8, 10, 12, 14,
        1, 3, 5, 7, 8, 10, 12, 14,
    };

    uint8_t tile_buffer[64] = {};

    tile_decode_4bpp(tile_buffer, vram);

    for (size_t i = 0; i < 64; ++i) {
        EXPECT_EQ(tile_buffer[i], expected_tiles[i]);
    }
}

TEST(TileTest, TileDecode8bpp)
{
    // Input VRAM: 64 bytes (32 words) is one tile.
    const uint16_t vram[32] = {
        0x55F0, 0x55F0, 0x55F0, 0x55F0, 0x55F0, 0x55F0, 0x55F0, 0x55F0,
        0x0F33, 0x0F33, 0x0F33, 0x0F33, 0x0F33, 0x0F33, 0x0F33, 0x0F33,
        0xA50F, 0xA50F, 0xA50F, 0xA50F, 0xA50F, 0xA50F, 0xA50F, 0xA50F,
        0x1F63, 0x1F63, 0x1F63, 0x1F63, 0x1F63, 0x1F63, 0x1F63, 0x1F63,
    };

    // A single tile is 8x8 pixels
    // 8 * 8 = 64 color IDs
    const uint8_t expected_tiles[64] = {
        0x21, 0x43, 0x65, 0x87, 0x98, 0xBA, 0xDC, 0xFE,
        0x21, 0x43, 0x65, 0x87, 0x98, 0xBA, 0xDC, 0xFE,
        0x21, 0x43, 0x65, 0x87, 0x98, 0xBA, 0xDC, 0xFE,
        0x21, 0x43, 0x65, 0x87, 0x98, 0xBA, 0xDC, 0xFE,
        0x21, 0x43, 0x65, 0x87, 0x98, 0xBA, 0xDC, 0xFE,
        0x21, 0x43, 0x65, 0x87, 0x98, 0xBA, 0xDC, 0xFE,
        0x21, 0x43, 0x65, 0x87, 0x98, 0xBA, 0xDC, 0xFE,
        0x21, 0x43, 0x65, 0x87, 0x98, 0xBA, 0xDC, 0xFE,
    };

    uint8_t tile_buffer[64] = {};

    tile_decode_8bpp(tile_buffer, vram);

    for (size_t i = 0; i < 64; ++i) {
        EXPECT_EQ(tile_buffer[i], expected_tiles[i]);
    }
}

TEST(TileTest, TileEncodeDecode2bpp)
{
    // A single tile is 8x8 pixels
    // 8 * 8 = 64 color IDs
    const uint8_t expected_tiles[64] = {
        0, 1, 2, 3, 0, 1, 2, 3,
        1, 2, 3, 0, 1, 2, 3, 0,
        2, 3, 0, 1, 2, 3, 0, 1,
        3, 0, 1, 2, 3, 0, 1, 2,
        3, 2, 1, 0, 3, 2, 1, 0,
        2, 1, 0, 3, 2, 1, 0, 3,
        1, 0, 3, 2, 1, 0, 3, 2,
        0, 3, 2, 1, 0, 3, 2, 1,
    };

    // Encode tiles into VRAM
    uint16_t vram[8] = {};
    tile_encode_2bpp(vram, expected_tiles);

    // Decode tiles from VRAM and check that results are the same
    uint8_t tile_buffer[64] = {};
    tile_decode_2bpp(tile_buffer, vram);
    for (size_t i = 0; i < 64; ++i) {
        EXPECT_EQ(expected_tiles[i], tile_buffer[i]);
    }
}

TEST(TileTest, TileEncodeDecode4bpp)
{
    // A single tile is 8x8 pixels
    // 8 * 8 = 64 color IDs
    const uint8_t expected_tiles[64] = {
        10, 7, 0, 6, 1, 6, 1, 7,
        12, 4, 15, 13, 5, 11, 8, 3,
        9, 12, 13, 9, 1, 1, 8, 4,
        9, 13, 2, 3, 6, 7, 3, 5,
        10, 0, 7, 8, 13, 4, 6, 13,
        1, 12, 2, 1, 12, 3, 11, 10,
        13, 11, 11, 15, 10, 8, 11, 8,
        15, 0, 1, 9, 3, 8, 9, 13
    };

    // Encode tiles into VRAM
    uint16_t vram[16] = {};
    tile_encode_4bpp(vram, expected_tiles);

    // Decode tiles from VRAM and check that results are the same
    uint8_t tile_buffer[64] = {};
    tile_decode_4bpp(tile_buffer, vram);

    for (size_t i = 0; i < 64; ++i) {
        EXPECT_EQ(expected_tiles[i], tile_buffer[i]);
    }
}

TEST(TileTest, TileEncodeDecode8bpp)
{
    // A single tile is 8x8 pixels
    // 8 * 8 = 64 color IDs
    const uint8_t expected_tiles[64] = {
        73, 117, 171, 49, 2, 130, 58, 177,
        6, 187, 135, 63, 61, 147, 112, 152,
        128, 116, 241, 201, 105, 13, 124, 52,
        69, 175, 13, 228, 64, 1, 94, 140,
        173, 45, 115, 26, 59, 87, 171, 108,
        159, 128, 130, 211, 66, 68, 48, 24,
        157, 90, 191, 9, 245, 97, 226, 132,
        125, 109, 132, 170, 137, 50, 121, 160
    };

    // Encode tiles into VRAM
    uint16_t vram[32] = {};
    tile_encode_8bpp(vram, expected_tiles);

    // Decode tiles from VRAM and check that results are the same
    uint8_t tile_buffer[64] = {};
    tile_decode_8bpp(tile_buffer, vram);
    for (size_t i = 0; i < 64; ++i) {
        EXPECT_EQ(expected_tiles[i], tile_buffer[i]);
    }
}

}   // namespace
