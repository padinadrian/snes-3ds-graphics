/**
 * Functions for working with 3DS textures
 */

/* ===== Includes ===== */

#include <stdbool.h>

#include "texture.h"

#include "tile.h"


/* ===== Data ===== */

/**
 * 3DS textures use a weird recursive pattern for tile pixels.
 *
 * This lookup table makes that mapping easier.
 */
const size_t pixel_index_lookup_normal[64] = {
    0,  1,  4,  5,  16, 17, 20, 21,
    2,  3,  6,  7,  18, 19, 22, 23,
    8,  9,  12, 13, 24, 25, 28, 29,
    10, 11, 14, 15, 26, 27, 30, 31,
    32, 33, 36, 37, 48, 49, 52, 53,
    34, 35, 38, 39, 50, 51, 54, 55,
    40, 41, 44, 45, 56, 57, 60, 61,
    42, 43, 46, 47, 58, 59, 62, 63,
};

const size_t pixel_index_lookup_h_flip[64] = {
    21, 20, 17, 16,  5,  4,  1,  0,
    23, 22, 19, 18,  7,  6,  3,  2,
    29, 28, 25, 24, 13, 12,  9,  8,
    31, 30, 27, 26, 15, 14, 11, 10,
    53, 52, 49, 48, 37, 36, 33, 32,
    55, 54, 51, 50, 39, 38, 35, 34,
    61, 60, 57, 56, 45, 44, 41, 40,
    63, 62, 59, 58, 47, 46, 43, 42,
};

const size_t pixel_index_lookup_v_flip[64] = {
    42, 43, 46, 47, 58, 59, 62, 63,
    40, 41, 44, 45, 56, 57, 60, 61,
    34, 35, 38, 39, 50, 51, 54, 55,
    32, 33, 36, 37, 48, 49, 52, 53,
    10, 11, 14, 15, 26, 27, 30, 31,
    8,  9,  12, 13, 24, 25, 28, 29,
    2,  3,  6,  7,  18, 19, 22, 23,
    0,  1,  4,  5,  16, 17, 20, 21,
};

const size_t pixel_index_lookup_flip_both[64] = {
    63, 62, 59, 58, 47, 46, 43, 42,
    61, 60, 57, 56, 45, 44, 41, 40,
    55, 54, 51, 50, 39, 38, 35, 34,
    53, 52, 49, 48, 37, 36, 33, 32,
    31, 30, 27, 26, 15, 14, 11, 10,
    29, 28, 25, 24, 13, 12,  9,  8,
    23, 22, 19, 18,  7,  6,  3,  2,
    21, 20, 17, 16,  5,  4,  1,  0,
};

/**
 * @brief Select the pixel pattern based on horizontal and vertical flip.
 */
inline const size_t* select_pixel_index_lookup(
    const bool flip_horizontal,
    const bool flip_vertical
)
{
    if (flip_horizontal && flip_vertical)
    {
        return pixel_index_lookup_flip_both;
    }
    if (flip_horizontal)
    {
        return pixel_index_lookup_h_flip;
    }
    if (flip_vertical)
    {
        return pixel_index_lookup_v_flip;
    }
    return pixel_index_lookup_normal;
}


/* ===== Functions ===== */

void decode_tile_to_texture_4bpp(
    uint32_t* pixel_buffer,
    const EncodedTile* tileset,
    const PaletteColor* palette, // Start of palette (8 colors)
    const bool flip_horizontal,
    const bool flip_vertical
)
{
    // Each pixel is a 4-bit index (0-15).
    Tile tile = {};
    tile_decode_4bpp(&tile, tileset);

    // All sprites use 4bpp 16-color tiles. Each sprite selects one of
    // 8 palettes from the last half of CGRAM.
    uint16_t* palette_u16 = (uint16_t*)(palette);   // Array of 16 colors

    const size_t* pixel_index_lookup = select_pixel_index_lookup(
        flip_horizontal,
        flip_vertical
    );

    // Assume texture is 8x8 = 64 pixels.
    // Weird decoding pattern...
    for (size_t i = 0; i < 64; i += 1)
    {
        const uint8_t color_id = tile.pixels[i];
        const size_t pixel_id = pixel_index_lookup[i];
        if (color_id == 0)
        {
            // pixel_buffer[pixel_id] = 0xFF80FFFF;
            pixel_buffer[pixel_id] = 0;
        }
        else
        {
            const uint16_t color = palette_u16[color_id];
            pixel_buffer[pixel_id] = convert_palette_to_rgba(color);
        }
    }
}
