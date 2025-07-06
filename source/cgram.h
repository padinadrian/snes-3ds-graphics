/// Read and write to CGRAM

#pragma once

#include "palette.h"
#include "stddef.h"
#include "stdint.h"

// Static pointer to CGRAM
static CGRAM* g_cgram = NULL;

// Static value of CGADD
static uint8_t g_cgadd = 0;

// Keeps track of the state of the CGRAM write (high or low byte)
static unsigned int g_write_offset = 0;

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
