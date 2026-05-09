/**
 * Functions for working with 3DS textures
 */

#ifndef SNES_3DS_TEXTURE_H_
#define SNES_3DS_TEXTURE_H_


/* ===== Includes ===== */

#include <stdint.h>
#include <stdbool.h>

#include "palette.h"
#include "tile.h"


/* ===== Functions ===== */

/**
 * @brief Decode a 2bpp SNES tile/color into a 3DS texture.
 *
 * @param[out] pixel_buffer Output buffer to write the raw pixel values
 * @param[in] vram_ptr Pointer to vram with the tile data
 * @param[in] palette Pointer to start of palette to use to color the tile
 * @param[in] flip_horizontal If true, flip the tile horizontally
 * @param[in] flip_vertical If true, flip the tile vertically
 */
void decode_tile_to_texture_2bpp(
    uint32_t* pixel_buffer,
    const EncodedTile* vram_ptr,
    const PaletteColor* palette,
    const bool flip_horizontal,
    const bool flip_vertical
);

/**
 * @brief Decode a 4bpp SNES tile/color into a 3DS texture.
 *
 * @param[out] pixel_buffer Output buffer to write the raw pixel values
 * @param[in] vram_ptr Pointer to vram with the tile data
 * @param[in] palette Pointer to start of palette to use to color the tile
 * @param[in] flip_horizontal If true, flip the tile horizontally
 * @param[in] flip_vertical If true, flip the tile vertically
 */
void decode_tile_to_texture_4bpp(
    uint32_t* pixel_buffer,
    const EncodedTile* vram_ptr,
    const PaletteColor* palette,
    const bool flip_horizontal,
    const bool flip_vertical
);

/**
 * @brief Decode a 8bpp SNES tile/color into a 3DS texture.
 *
 * @param[out] pixel_buffer Output buffer to write the raw pixel values
 * @param[in] vram_ptr Pointer to vram with the tile data
 * @param[in] palette Pointer to start of palette to use to color the tile
 * @param[in] flip_horizontal If true, flip the tile horizontally
 * @param[in] flip_vertical If true, flip the tile vertically
 */
void decode_tile_to_texture_8bpp(
    uint32_t* pixel_buffer,
    const EncodedTile* vram_ptr,
    const PaletteColor* palette,
    const bool flip_horizontal,
    const bool flip_vertical
);


#endif  // SNES_3DS_TEXTURE_H_
