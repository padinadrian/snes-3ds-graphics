#include <gtest/gtest.h>

extern "C" {
    #include "cgram.h"
}

namespace {

TEST(CgramTest, CgramSize)
{
    ASSERT_EQ(sizeof(CGRAM), 512);
}

TEST(CgramTest, CgramWrite)
{
    CGRAM cgram;
    cgram_init(&cgram);

    set_cgadd(127);

    cgram_write(0x11);
    cgram_write(0x22);
    cgram_write(0x33);
    cgram_write(0x44);
    cgram_write(0x55);
    cgram_write(0x66);

    const uint16_t* colors_ptr = cgram.palettes[0].colors;

    EXPECT_EQ(colors_ptr[127], 0x2211);
    EXPECT_EQ(colors_ptr[128], 0x4433);
    EXPECT_EQ(colors_ptr[129], 0x6655);
}

}   // namespace
