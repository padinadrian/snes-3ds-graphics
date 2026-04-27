/**
 * Functions for working with 3DS textures
 */

/* ===== Includes ===== */

#include "texture.h"

#include "tile.h"


/* ===== Data ===== */

/**
 * 3DS textures use a weird recursive pattern for tile pixels.
 *
 * This lookup table makes that mapping easier.
 */
const size_t pixel_index_lookup[64] = {
    0,  1,  4,  5,  16, 17, 20, 21,
    2,  3,  6,  7,  18, 19, 22, 23,
    8,  9,  12, 13, 24, 25, 28, 29,
    10, 11, 14, 15, 26, 27, 30, 31,
    32, 33, 36, 37, 48, 49, 52, 53,
    34, 35, 38, 39, 50, 51, 54, 55,
    40, 41, 44, 45, 56, 57, 60, 61,
    42, 43, 46, 47, 58, 59, 62, 63
};


/* ===== Functions ===== */

void decode_tile_to_texture_4bpp(
    uint32_t* pixel_buffer,
    const EncodedTile* vram_ptr,
    const PaletteColor* palette // Start of palette (8 colors)
)
{
    // Each pixel is a 4-bit index (0-15).
    Tile tile = {};
    tile_decode_4bpp(&tile, vram_ptr);

    // All sprites use 4bpp 16-color tiles. Each sprite selects one of
    // 8 palettes from the last half of CGRAM.
    uint16_t* palette_u16 = (uint16_t*)(palette);   // Array of 16 colors

    // Assume texture is 8x8 = 64 pixels.
    // Weird decoding pattern...
    for (size_t i = 0; i < 64; i += 1)
    {
        const uint8_t color_id = tile.pixels[i];
        const size_t pixel_id = pixel_index_lookup[i];
        if (color_id == 0)
        {
            pixel_buffer[pixel_id] = 0;
        }
        else
        {
            const uint16_t color = palette_u16[color_id];
            pixel_buffer[pixel_id] = convert_palette_to_rgba(color);
        }
    }
}
