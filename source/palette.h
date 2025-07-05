
#pragma once

#include <stdint.h>

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
    PaletteColor colors[256];
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



