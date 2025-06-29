//! Utility functions.

#pragma once

/* ========== Includes ========== */
#include <stddef.h>
#include <stdint.h>

/* ========== Functions ========== */

/**
 * @brief Helper function to swap two byte values.
 */
static inline void swap_u8(uint8_t* a, uint8_t* b)
{
    uint8_t temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * @brief Helper function to reverse an array of bytes.
 */
static inline void reverse_bytes(uint8_t* bytes, const size_t length)
{
    uint8_t* start_ptr = bytes;
    uint8_t* end_ptr = bytes + length - 1;
    while (end_ptr > start_ptr) {
        swap_u8(start_ptr++, end_ptr--);
    }
}
