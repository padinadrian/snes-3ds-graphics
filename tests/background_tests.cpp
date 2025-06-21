#include <gtest/gtest.h>

extern "C" {
    #include "background.h"
}

namespace {

TEST(BackgroundTest, TranslateTileTest1) {

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

}