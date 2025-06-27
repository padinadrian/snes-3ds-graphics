//! Functions for encoding and decoding tiles in VRAM.

#pragma once

/* ========== Includes ========== */
#include <stddef.h>
#include <stdint.h>


/* ========== Functions ========== */

/**
 * @brief Encode a single tile into a tile buffer.
 *
 * @param output_vram Pointer to destination buffer in VRAM
 * @param input_buf Pointer to input buffer of palette IDs
 */
void tile_encode_2bpp(uint16_t* output_vram, const uint8_t* palette_ids);

/**
 * @brief Encode a single tile into a tile buffer in 4bpp format.
 *
 * @param output_vram Pointer to destination buffer in VRAM
 * @param input_buf Pointer to input buffer of palette IDs
 */
void tile_encode_4bpp(uint16_t* output_vram, const uint8_t* palette_ids);

/**
 * @brief Encode a single tile into a tile buffer in 8bpp format.
 *
 * @param output_vram Pointer to destination buffer in VRAM
 * @param input_buf Pointer to input buffer of palette IDs
 */
void tile_encode_8bpp(uint16_t* output_vram, const uint8_t* palette_ids);

// TODO: Encode functions

void translate_tile(uint32_t* output, uint8_t* input, size_t length, uint32_t bit_depth);
