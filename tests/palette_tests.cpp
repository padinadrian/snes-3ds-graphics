#include <gtest/gtest.h>

extern "C" {
    #include "palette.h"
}

namespace {

TEST(PaletteTest, PaletteObjectSizes)
{
    ASSERT_EQ(sizeof(PaletteColor), 2);
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

TEST(PaletteTest, ConvertPaletteToRgba1)
{
    // Blue = 31
    // Green = 31
    // Red = 31
    uint16_t color = 0b0111111111111111;
    EXPECT_EQ(convert_palette_to_rgba(color), 0xF8F8F8FF);
}

TEST(PaletteTest, ConvertPaletteToRgba2)
{
    // Blue = 5
    // Green = 10
    // Red = 15
    uint16_t color = 0b0001010101001111;
    EXPECT_EQ(convert_palette_to_rgba(color), 0x785028FF);
}

}   // namespace
