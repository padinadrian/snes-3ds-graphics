/**
 * Read and write to CGRAM.
 *
 * Not currently used - TODO
 */


/* ===== Includes ===== */

#include "cgram.h"

#include <string.h>


/* ===== Data ===== */

// Static pointer to CGRAM
static CGRAM* g_cgram = NULL;

// Static value of CGADD
static uint8_t g_cgadd = 0;

// Keeps track of the state of the CGRAM write (high or low byte)
static unsigned int g_write_offset = 0;


/* ===== Functions ===== */

/**
 * @brief Initialize CGRAM
 */
void cgram_init(CGRAM* cgram_in)
{
    g_cgram = cgram_in;
    memset((void*)g_cgram, 0, sizeof(CGRAM));
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
    uint16_t* cgram_as_u16 = (uint16_t*)&g_cgram;
    uint16_t* color_ptr = &cgram_as_u16[g_cgadd];
    if (g_write_offset == 0) {
        // First write
        *color_ptr = val;
    }
    else {
        // Second write - increment CGADD
        *color_ptr |= val << 8;
        g_cgadd += 1;
    }
    g_write_offset = !g_write_offset;
}
