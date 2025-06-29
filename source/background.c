//! Functions for encoding and decoding tiles in VRAM.

/* ========== Includes ========== */
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "utility.h"

/* ========== Functions ========== */

/**
 * @brief Helper function for encoding one row of a tile into a tile buffer.
 *
 * @param output_vram Pointer to destination buffer in VRAM
 * @param input_buf Pointer to input buffer of palette IDs
 * @param shift Amount to shift the palette ID down before reading the value
 */
void tile_encode_helper(
        uint16_t* output_vram,
        const uint8_t* palette_ids,
        const int shift
)
{
    uint16_t upper = 0, lower = 0;
    for (size_t i = 0; i < 8; ++i, lower <<= 1, upper <<= 1) {
        lower |= (palette_ids[i] >> shift) & 1;
        upper |= ((palette_ids[i] >> shift) & 2) >> 1;
    }
    *output_vram = (upper << 7) | (lower >> 1);
}

/**
 * @brief Encode a single tile into a tile buffer in 2bpp format.
 *
 * @param output_vram Pointer to destination buffer in VRAM
 * @param input_buf Pointer to input buffer of palette IDs
 */
void tile_encode_2bpp(uint16_t* output_vram, const uint8_t* palette_ids)
{
    for (size_t i = 0; i < 8; ++i) {
        tile_encode_helper(output_vram + i, palette_ids + (i * 8), 0);
    }
}

/**
 * @brief Encode a single tile into a tile buffer in 4bpp format.
 *
 * @param output_vram Pointer to destination buffer in VRAM
 * @param input_buf Pointer to input buffer of palette IDs
 */
void tile_encode_4bpp(uint16_t* output_vram, const uint8_t* palette_ids)
{
    // First 16 bytes are bit planes 0 and 1
    for (size_t i = 0; i < 8; ++i) {
        tile_encode_helper(output_vram + i, palette_ids, 0);
    }
    // Second 16 bytes are bit planes 2 and 3
    for (size_t i = 8; i < 16; ++i) {
        tile_encode_helper(output_vram + i, palette_ids, 2);
    }
}

/**
 * @brief Encode a single tile into a tile buffer in 8bpp format.
 *
 * @param output_vram Pointer to destination buffer in VRAM
 * @param input_buf Pointer to input buffer of palette IDs
 */
void tile_encode_8bpp(uint16_t* output_vram, const uint8_t* palette_ids)
{
    // First 16 bytes are bit planes 0 and 1
    for (size_t i = 0; i < 8; ++i) {
        tile_encode_helper(output_vram + i, palette_ids, 0);
    }
    // Second 16 bytes are bit planes 2 and 3
    for (size_t i = 8; i < 16; ++i) {
        tile_encode_helper(output_vram + i, palette_ids, 2);
    }
    // Third 16 bytes are bit planes 4 and 5
    for (size_t i = 16; i < 24; ++i) {
        tile_encode_helper(output_vram + i, palette_ids, 4);
    }
    // Fourth 16 bytes are bit planes 6 and 7
    for (size_t i = 24; i < 32; ++i) {
        tile_encode_helper(output_vram + i, palette_ids, 6);
    }
}

/**
 * @brief Helper function to decode a single row of a tile from VRAM.
 *
 * @param output_buf Pointer to destination buffer of palette IDs
 * @param input_vram Pointer to input buffer of tile data in VRAM
 * @param shift Amount to shift the palette ID down before reading the value
 */
void tile_decode_helper(
        uint8_t* output_buf,
        uint16_t input_vram,
        const int shift
)
{
    uint8_t temp_buf[8];

    // Lower byte is bit plane 0
    for (size_t i = 0; i < 8; ++i) {
        temp_buf[i] = (input_vram & 1) << shift;
        input_vram >>= 1;
    }
    // Upper byte is bit plane 1
    for (size_t i = 0; i < 8; ++i) {
        temp_buf[i] |= ((input_vram & 1) << (shift + 1));
        input_vram >>= 1;
    }

    // Copy bytes to output buffer (use OR to preserve results of previous calls)
    reverse_bytes(temp_buf, 8);
    for (size_t i = 0; i < 8; i++) {
        output_buf[i] |= temp_buf[i];
    }
}

/**
 * @brief Decode a single tile from a tile buffer in 2bpp format.
 *
 * @param output_buf Pointer to destination buffer of palette IDs
 * @param input_vram Pointer to input buffer of tile data in VRAM
 */
void tile_decode_2bpp(uint8_t* output_buf, const uint16_t* input_vram)
{
    // One tile is 8 rows of 2 bytes each.
    for (size_t i = 0; i < 8; ++i) {
        tile_decode_helper(output_buf + (i * 8), input_vram[i], 0);
    }
}

/**
 * @brief Decode a single tile from a tile buffer in 4bpp format.
 *
 * @param output_buf Pointer to destination buffer of palette IDs
 * @param input_vram Pointer to input buffer of tile data in VRAM
 */
void tile_decode_4bpp(uint8_t* output_buf, const uint16_t* input_vram)
{
    // First 16 bytes are bit planes 0 and 1
    for (size_t i = 0; i < 8; ++i) {
        tile_decode_helper(output_buf + (i * 8), input_vram[i], 0);
    }
    // Second 16 bytes are bit planes 2 and 3
    for (size_t i = 0; i < 8; ++i) {
        tile_decode_helper(output_buf + (i * 8), input_vram[i + 8], 2);
    }
}

/**
 * @brief Decode a single tile from a tile buffer in 8bpp format.
 *
 * @param output_buf Pointer to destination buffer of palette IDs
 * @param input_vram Pointer to input buffer of tile data in VRAM
 */
void tile_decode_8bpp(uint8_t* output_buf, const uint16_t* input_vram)
{
    // First 16 bytes are bit planes 0 and 1
    for (size_t i = 0; i < 8; ++i) {
        tile_decode_helper(output_buf + (i * 8), input_vram[i], 0);
    }
    // First 16 bytes are bit planes 2 and 3
    for (size_t i = 8; i < 16; ++i) {
        tile_decode_helper(output_buf + (i * 8), input_vram[i], 2);
    }
    // First 16 bytes are bit planes 4 and 5
    for (size_t i = 16; i < 24; ++i) {
        tile_decode_helper(output_buf + (i * 8), input_vram[i], 4);
    }
    // First 16 bytes are bit planes 6 and 7
    for (size_t i = 24; i < 32; ++i) {
        tile_decode_helper(output_buf + (i * 8), input_vram[i], 6);
    }
}




void translate_tile(uint32_t* output, uint8_t* input, size_t length, uint32_t bit_depth) {
    // TODO: Check bounds?

    switch (bit_depth) {
        case 2: {   // 2bpp, 4 colors
            // Alternate bytes: first LSB, then MSB
            for (size_t i = 0; i < length;) {
                size_t row = i << 2; // i * 4;

                uint8_t lsb = input[i++];
                for (size_t n = 0; n < 8; ++n) {
                    output[row + 7 - n] = lsb & 1;
                    lsb >>= 1;
                }

                uint8_t msb = input[i++];
                for (size_t n = 0; n < 8; ++n) {
                    output[row + 7 - n] |= ((msb & 1) << 1);
                    msb >>= 1;
                }
            }
        }
        case 4: {   // 4bpp, 16 colors

        }
        case 8: {   // 8bpp, direct color, 8-bit (or 11-bit?) color

        }
    }


    for (size_t i = 0; i < bit_depth; ++i) {

    }
}
