
#pragma once

#include <stdint.h>

enum { NUM_CGRAM_COLORS = 256 };
enum { CGRAM_SIZE = NUM_CGRAM_COLORS * 2 };

enum {
    PALETTE_RED_MASK   = 0b11111,
    PALETTE_GREEN_MASK = PALETTE_RED_MASK << 5,
    PALETTE_BLUE_MASK  = PALETTE_GREEN_MASK << 5,
};

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
} PaletteColor;

typedef struct {
    // List of colors in CGRAM
    PaletteColor colors[NUM_CGRAM_COLORS];
} CGRAM;

/**
 * @brief Get the red component of the color.
 */
static inline uint8_t palette_color_get_red(PaletteColor color) {
    return color.red;
}

/**
 * @brief Get the green component of the color.
 */
static inline uint8_t palette_color_get_green(PaletteColor color) {
    return (color.green_h << 3) | color.green_l;
}

/**
 * @brief Get the blue component of the color.
 */
static inline uint8_t palette_color_get_blue(PaletteColor color) {
    return color.blue;
}

/**
 * @brief Convert a 5-bit RGB color to an 8-bit RGBA color.
 */
static inline uint32_t convert_palette_to_rgba(uint16_t color)
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
