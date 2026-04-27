/**
 * Read and write to CGRAM.
 *
 * Not currently used -- TODO
 */

#ifndef SNES_3DS_CGRAM_H_
#define SNES_3DS_CGRAM_H_


/* ===== Includes ===== */

#include "palette.h"
#include "stddef.h"
#include "stdint.h"


/* ===== Functions ===== */

/**
 * @brief Initialize CGRAM
 */
void cgram_init(CGRAM* cgram_in);

/**
 * @brief Set CGADD (offset to CGRAM write)
 */
void set_cgadd(uint8_t cgadd_in);

/**
 * @brief Write to CGRAM
 */
void cgram_write(uint16_t val);


#endif  // SNES_3DS_CGRAM_H_
