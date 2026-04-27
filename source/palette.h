/**
 * Definitions and functions for updating CGRAM and color palettes.
 */

#ifndef SNES_3DS_PALETTE_H_
#define SNES_3DS_PALETTE_H_


/* ===== Includes ===== */

#include <stdint.h>


/* ===== Enums ===== */

enum { COLORS_PER_PALETTE = 16 };
enum { NUM_PALETTES = 16 };
enum { NUM_CGRAM_COLORS = NUM_PALETTES * COLORS_PER_PALETTE }; // 256
enum { CGRAM_SIZE = NUM_CGRAM_COLORS * 2 }; // 512

enum {
    PALETTE_RED_MASK   = 0b11111,
    PALETTE_GREEN_MASK = PALETTE_RED_MASK << 5,
    PALETTE_BLUE_MASK  = PALETTE_GREEN_MASK << 5,
};


/* ===== Structs ===== */

/// Alias for a single color in a palette.
typedef uint16_t PaletteColor;

// Layout of a single color in CGRAM.
typedef struct {
    // Red component of color
    uint8_t red : 5;
    // Lower 3 bits of green component of color
    uint8_t green_l : 3;
    // Higher 2 bits of green component of color
    uint8_t green_h : 2;
    // Blue component of color
    uint8_t blue : 5;
    // Unused
    uint8_t _unused : 1;
} PaletteColorBits;

// A palette consists of one row of 16 colors.
typedef struct Palette {
    PaletteColor colors[COLORS_PER_PALETTE];
} Palette;

// Collection of palletes contained in CGRAM.
typedef struct {
    Palette palettes[NUM_PALETTES];
} CGRAM;


/* ===== Functions ===== */

/**
 * @brief Get the red component of the color.
 */
static inline uint8_t palette_color_get_red(PaletteColorBits color) {
    return color.red;
}

/**
 * @brief Get the green component of the color.
 */
static inline uint8_t palette_color_get_green(PaletteColorBits color) {
    return (color.green_h << 3) | color.green_l;
}

/**
 * @brief Get the blue component of the color.
 */
static inline uint8_t palette_color_get_blue(PaletteColorBits color) {
    return color.blue;
}

/**
 * @brief Convert a 5-bit RGB color to an 8-bit RGBA color.
 */
inline uint32_t convert_palette_to_rgba(PaletteColor color)
{
    uint32_t color_int = (uint32_t)color;
    uint32_t red_int   = color_int & PALETTE_RED_MASK;
    uint32_t green_int = color_int & PALETTE_GREEN_MASK;
    uint32_t blue_int  = color_int & PALETTE_BLUE_MASK;

    // Red:   color >> 0 << 24 << 3 = color << 27
    // Green: color >> 5 << 16 << 3 = color << 14
    // Blue:  color >> 10 << 8 << 3 = color << 1

    // Return as RGBA format
    return (red_int << 27) | (green_int << 14) | (blue_int << 1) | 0xFF;
}

/**
 * @brief Convert an 8-bit RGBA color to a 5-bit RGB color.
 */
inline PaletteColor encode_rgba_to_palette(uint32_t color)
{
    const PaletteColor red = (color >> 27) & PALETTE_RED_MASK;
    const PaletteColor green = (color >> 14) & PALETTE_GREEN_MASK;
    const PaletteColor blue = (color >> 1) & PALETTE_BLUE_MASK;
    return red | green | blue;
}


#endif  // SNES_3DS_PALETTE_H_
