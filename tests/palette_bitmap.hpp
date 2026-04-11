//! Helper functions for palette bitmap tests.

#pragma once

#include <stdio.h>

#include "bitmap_image.hpp"

extern "C" {
    #include "palette.h"
}


/**
 * @brief Read CGRAM from a binary file.
 */
inline size_t read_cgram(CGRAM* cgram, const char* cgram_file)
{
    FILE* fp = fopen(cgram_file, "rb");
    const size_t result = fread(cgram->colors, 2, NUM_CGRAM_COLORS, fp);
    return result;
}

/**
 * @brief Convert a CGRAM palette into a bitmap image and write it to file.
 */
inline void generate_palette_bitmap(CGRAM* cgram, const char* output_file)
{
    bitmap_image image{16, 16};
    for (int y = 0; y < 16; y += 1)
    {
        for (int x = 0; x < 16; x += 1)
        {
            size_t index = y * 16 + x;
            image.set_pixel(
                x, y,
                palette_color_get_red(cgram->colors[index]) << 3,
                palette_color_get_green(cgram->colors[index]) << 3,
                palette_color_get_blue(cgram->colors[index]) << 3
            );
        }
    }
    image.save_image(output_file);
}