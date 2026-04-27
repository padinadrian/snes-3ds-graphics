/**
 * Functions for working with 3DS textures
 */

#ifndef SNES_3DS_TEXTURE_H_
#define SNES_3DS_TEXTURE_H_


/* ===== Includes ===== */

#include <stdint.h>

#include "palette.h"
#include "tile.h"


/* ===== Functions ===== */

/**
 * @brief Helper function to decode a SNES tile/color into a 3DS texture.
 *
 * @param[out] pixel_buffer Output buffer to write the raw pixel values
 * @param[in] vram_ptr Pointer to vram with the tile data
 * @param[in] palette Pointer to start of palette to use to color the tile
 */
void decode_tile_to_texture_4bpp(
    uint32_t* pixel_buffer,
    const EncodedTile* vram_ptr,
    const PaletteColor* palette // Start of palette (8 colors)
);


#endif  // SNES_3DS_TEXTURE_H_
