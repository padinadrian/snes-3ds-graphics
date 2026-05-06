/**
 * Handle tilemaps.
 */

#ifndef SNES_3DS_TILEMAP_H_
#define SNES_3DS_TILEMAP_H_


/* ===== Includes ===== */

#include <stdint.h>

#include "cgram.h"
#include "texture.h"
#include "tile.h"


/* ===== Structs ===== */

/**
 * Tilemap object; contains info on a single tile in a background.
 *
 * Size: 16 bits (2 bytes)
 */
typedef struct Tilemap {
    /// @brief Tile index: Select one of 1024 from VRAM relative to base address
    uint16_t tile_id : 10;
    /// @brief Palette selection: 0-7 selects one of up to 8 palettes from CGRAM
    uint8_t palette_id : 3;
    /// @brief Priority: Background = 0, foreground = 1
    uint8_t priority : 1;
    /// @brief Flip horizontal
    uint8_t h_flip : 1;
    /// @brief Flip vertical
    uint8_t v_flip : 1;
} Tilemap;


/* ===== Functions ===== */

/**
 * @brief Decode a tilemap into a background image.
 *
 * @param[out] output_buf Output pixel buffer
 * @param[in] tilemap Pointer to tilemap with tile and color info
 * @param[in] tileset Pointer to vram section containing tile data
 * @param[in] cgram Color palette
 */
void decode_tilemap_4bpp(
    uint32_t* output_buf,
    const Tilemap* tilemap,
    const EncodedTile* tileset,
    const CGRAM* cgram,
    const bool priority
);


#endif  // SNES_3DS_TILEMAP_H_
