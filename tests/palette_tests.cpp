#include <gtest/gtest.h>

extern "C" {
    #include "palette.h"
}


#include <cstdio>

namespace {

TEST(PaletteTest, PaletteObjectSizes)
{
    EXPECT_EQ(sizeof(PaletteColor), 2);
}

TEST(PaletteTest, PaletteColorGetColor)
{
    PaletteColor color;
    *((uint16_t*)&color) = 0x5555;
    EXPECT_EQ(palette_color_get_red(color), 21);
    EXPECT_EQ(palette_color_get_green(color), 10);
    EXPECT_EQ(palette_color_get_blue(color), 21);
}

TEST(PaletteTest, PaletteColorGetColor2)
{
    PaletteColor color;
    *((uint16_t*)&color) = 0b0111001011000110;
    EXPECT_EQ(palette_color_get_red(color), 6);
    EXPECT_EQ(palette_color_get_green(color), 22);
    EXPECT_EQ(palette_color_get_blue(color), 28);
}

}   // namespace
