/// Read and write to CGRAM

#include "cgram.h"

#include <string.h>

/**
 * @brief Initialize CGRAM
 */
void cgram_init(CGRAM* cgram_in)
{
    g_cgram = cgram_in;
    memset(g_cgram->colors, 0, NUM_CGRAM_COLORS * sizeof(PaletteColor));
}

/**
 * @brief Set CGADD (offset to CGRAM write)
 */
void set_cgadd(uint8_t cgadd_in)
{
    g_cgadd = cgadd_in;
}

/**
 * @brief Write to CGRAM
 */
void cgram_write(uint16_t val)
{
    PaletteColor* color_ptr = &g_cgram->colors[g_cgadd];
    if (g_write_offset == 0) {
        // First write
        *(uint16_t*)color_ptr = val;
    }
    else {
        // Second write - increment CGADD
        *(uint16_t*)color_ptr |= val << 8;
        g_cgadd += 1;
    }
    g_write_offset = !g_write_offset;
}
