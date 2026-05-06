/**
 * Handle backgrounds.
 *
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

#include <citro2d.h>

#include "cgram.h"
#include "texture.h"
#include "tile.h"


/* ===== Structs ===== */

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

/// @brief Background object
typedef struct Background
{
    C2D_Image image;
    // X offset
    float x;
    // Y offset
    float y;
    // Depth between 0.0 and 1.0
    float depth;
    // Horizontal scale
    float x_scale;
    // Vertical scale
    float y_scale;
} Background;


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


// TODO: Should these be somewhere else?

/**
 * @brief Initialize backgrounds.
 *
 * Must be called at the start of the program before any backgrounds are drawn.
 */
void init_backgrounds();

/**
 * @brief Update all the background images.
 *
 * @param[in] vram Pointer to VRAM
 * @param[in] cgram Pointer to CGRAM
 * @param[in] bgnsc BGnSC registers
 * @param[in] bg12nba BG12NBA register
 * @param[in] bg34nba BG34NBA register
 */
void update_backgrounds(
    const uint16_t* vram,
    const CGRAM* cgram,
    const BGnSC_t bgnsc[4],
    const BG12NBA_t bg12nba,
    const BG34NBA_t bg34nba
);

/**
 * @brief Draw background layers.
 *
 * The specific layers rendered are dependent on the background mode.
 *
 * @{
 */
void draw_background_layer0();
void draw_background_layer1();
void draw_background_layer2();
void draw_background_layer3();
void draw_background_layer4();
/** @} */


#endif  // SNES_3DS_BACKGROUND_H_
