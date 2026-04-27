/**
 * SNES backgrounds consist of one or more layers of tilemaps.
 *
 * There are 8 available background modes, which can be
 * changed at any time via BGMODE, even mid-screen (TODO).
 *
 * Each mode has 1-4 layers, consisting of tiles that are 2bpp (4-color),
 * 4bpp (16-color), or 8bpp (256-color).
 *
 * Each of the four background layers has several properties:
 *   - Tilemap VRAM address: BGnSC
 *   - Tileset (CHR) VRAM address: BG12NBA, BG34NBA
 *   - Multiple tilemaps in 1x1, 2x1, 1x2, 2x2 formats: BGnSC
 *   - Tile size of 8x8 (or 16x8) or 16x16: BGMODE
 *   - Scroll position: BGnHOFS, BGnVOFS
 *   - Windows (TODO)
 *   - Mosaic: MOSAIC (TODO)
 *
 * Each of the four background layers can also be independently
 * activated
 *
 *
 */

#ifndef SNES_3DS_BACKGROUND_H_
#define SNES_3DS_BACKGROUND_H_


/* ===== Includes ===== */

#include <stdint.h>
#include <stdio.h>

#include "cgram.h"
#include "texture.h"
#include "tile.h"


/* ===== Enums ===== */

enum { TILEMAP_WIDTH = 32 };
enum { TILEMAP_HEIGHT = 32 };
enum { TILEMAP_SIZE = (32 * 32) };  // 1024


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

/// @brief BG1-4 tilemap address and size ($2107-$210A)
typedef struct BGnSC
{
    /// @brief Horizontal tilemap count (0 = 1 tilemap, 1 = 2 tilemaps)
    uint8_t x: 1;
    /// @brief Vertical tilemap count (0 = 1 tilemap, 1 = 2 tilemaps)
    uint8_t y: 1;
    /// @brief Tilemap VRAM address (word address = AAAAAA << 10)
    uint8_t address: 6;
} BGnSC_t;

/// @brief CHR word base addresses for BG1 and BG2 ($210B)
typedef struct BG12NBA
{
    /// @brief BG1 CHR word base address (word address = AAAA << 12)
    uint8_t bg1_address : 4;
    /// @brief BG2 CHR word base address (word address = BBBB << 12)
    uint8_t bg2_address : 4;
} BG12NBA_t;

/// @brief CHR word base addresses for BG3 and BG4 ($210C)
typedef struct BG34NBA
{
    /// @brief BG3 CHR word base address (word address = CCCC << 12)
    uint8_t bg3_address : 4;
    /// @brief BG4 CHR word base address (word address = DDDD << 12)
    uint8_t bg4_address : 4;
} BG34NBA_t;


/* ===== Functions ===== */

/** @brief Get background 1 start address. */
inline uint32_t get_bg1_address(const BG12NBA_t reg)
{
    return (uint32_t)(reg.bg1_address) << 12;
}

/** @brief Get background 2 start address. */
inline uint32_t get_bg2_address(const BG12NBA_t reg)
{
    return (uint32_t)(reg.bg2_address) << 12;
}

/** @brief Get background 3 start address. */
inline uint32_t get_bg3_address(const BG34NBA_t reg)
{
    return (uint32_t)(reg.bg3_address) << 12;
}

/** @brief Get background 4 start address. */
inline uint32_t get_bg4_address(const BG34NBA_t reg)
{
    return (uint32_t)(reg.bg4_address) << 12;
}

/**
 * @brief Background mode 1
 *
 * BG1 = 4bpp (16 color)
 * BG2 = 4bpp (16 color)
 * BG3 = 2bpp (4 color)
 *
 * Priority (front to back):
 * 3H S3 1H 2H S2 1L 2L S1 3H S0 3L
 *
 * @param[out] output_buf Output buffer to write pixel data to
 * @param[in] vram Pointer to VRAM
 * @param[in] cgram Pointer to CGRAM
 * @param[in] bgnsc BGnSC register value
 * @param[in] bg12nba BG12NBA register value
 * @param[in] bg34nba BG34NBA register value
 */
inline void background_mode1(
    uint32_t* output_buf,
    const uint16_t* vram,
    const CGRAM* cgram,
    const BGnSC_t bgnsc[4],
    const BG12NBA_t bg12nba,
    const BG34NBA_t bg34nba
)
{
    // const Tilemap* bg1_address = (const Tilemap*)(bgnsc[0].address << 9);
    // const Tilemap* bg2_tilemap = (const Tilemap*)(bgnsc[1].address << 9);
    const Tilemap* bg2_tilemap = (const Tilemap*)vram;
    // const uint16_t* bg2_tileset_addr = vram + (get_bg2_address(bg12nba) >> 1);
    const uint16_t* bg2_tileset_addr = vram + 0x2000;

    // Each tilemap is a 32x32 tile region
    // 32*32 = 1024 tiles
    for (size_t i = 0; i < TILEMAP_SIZE; i += 1)
    {
        const uint32_t tile_id = bg2_tilemap[i].tile_id;
        const uint32_t palette_id = bg2_tilemap[i].palette_id;
        decode_tile_to_texture_4bpp(
            output_buf,
            &bg2_tileset_addr[tile_id << 4],
            cgram->palettes[palette_id].colors,
            bg2_tilemap[i].h_flip,
            bg2_tilemap[i].v_flip
        );
        if ((i % 32) == 31)
        {
            output_buf += (33 * 64);
        }
        else
        {
            output_buf += 64;
        }
    }
}


#endif  // SNES_3DS_BACKGROUND_H_
