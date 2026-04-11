//! Test for converting a CGRAM palette to a bitmap image.

#include <stdio.h>

#include <gtest/gtest.h>

#include "palette_bitmap.hpp"

extern "C" {
    #include "cgram.h"
}

namespace {

/**
 * @brief Check if two files are the same
 */
bool compare_files(const char* file1, const char* file2)
{
    FILE* fp1 = fopen(file1, "rb");
    FILE* fp2 = fopen(file2, "rb");

    constexpr size_t ARRAY_LENGTH = 8;
    uint32_t data1[ARRAY_LENGTH] = {};
    uint32_t data2[ARRAY_LENGTH] = {};

    while (true)
    {
        size_t res1 = fread(&data1, sizeof(uint32_t), ARRAY_LENGTH, fp1);
        size_t res2 = fread(&data2, sizeof(uint32_t), ARRAY_LENGTH, fp2);

        if (res1 == 0 && res2 == 0)
        {
            return true;
        }

        if (res1 != res2)
        {
            return false;
        }

        for (size_t i = 0; i < ARRAY_LENGTH; i += 1)
        {
            if (data1[i] != data2[i])
            {
                return false;
            }
        }
    }

    return true;
}


TEST(CgramBitmapTest, LozPalette)
{
    CGRAM cgram;
    const size_t result = read_cgram(&cgram, "tests/loz_cgram.bin");
    generate_palette_bitmap(&cgram, "tests/loz_cgram.bmp");
    EXPECT_TRUE(
        compare_files(
            "tests/loz_cgram.bmp",
            "tests/loz_cgram_expected.bmp"
        )
    );
}

}   // namespace
