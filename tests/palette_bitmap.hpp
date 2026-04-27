/**
 * Helper functions for palette bitmap tests.
 */

#ifndef SNES_3DS_TESTS_PALETTE_BITMAPP_HPP_
#define SNES_3DS_TESTS_PALETTE_BITMAPP_HPP_


/* ===== Includes ===== */

#include <stdio.h>

#include "bitmap_image.hpp"

extern "C" {
    #include "palette.h"
}


/* ===== Functions ===== */

/**
 * @brief Read CGRAM from a binary file.
 */
inline size_t read_cgram(CGRAM* cgram, const char* cgram_file)
{
    FILE* fp = fopen(cgram_file, "rb");
    if (!fp)
    {
        return 0;
    }
    return fread(cgram->palettes[0].colors, 2, NUM_CGRAM_COLORS, fp);
}

/**
 * @brief Convert a CGRAM palette into a bitmap image and write it to file.
 */
inline void generate_palette_bitmap(CGRAM* cgram, const char* output_file)
{
    bitmap_image image{16, 16};
    PaletteColorBits* colors_ptr = (PaletteColorBits*)(cgram->palettes[0].colors);
    for (int y = 0; y < 16; y += 1)
    {
        for (int x = 0; x < 16; x += 1)
        {
            size_t index = y * 16 + x;
            image.set_pixel(
                x, y,
                palette_color_get_red(colors_ptr[index]) << 3,
                palette_color_get_green(colors_ptr[index]) << 3,
                palette_color_get_blue(colors_ptr[index]) << 3
            );
        }
    }
    image.save_image(output_file);
}


#endif  // SNES_3DS_TESTS_PALETTE_BITMAPP_HPP_
