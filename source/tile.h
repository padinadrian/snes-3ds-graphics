/**
 * Functions for encoding and decoding tiles in VRAM.
 */

#ifndef SNES_3DS_TILE_H_
#define SNES_3DS_TILE_H_


/* ========== Includes ========== */

#include <stddef.h>
#include <stdint.h>


/* ========== Structs ========== */

/** @brief Encoded tile data in VRAM. */
typedef uint16_t EncodedTile;

/** @brief An 8x8 tile containing 64 pixel IDs. */
typedef struct Tile {
    /// @brief Pixel IDs that make up the 8x8 tile
    uint8_t pixels[64];
} Tile;


/* ========== Functions ========== */

/**
 * @brief Encode a single tile into a tile buffer.
 *
 * @param output_vram Pointer to destination buffer in VRAM
 * @param input_buf Pointer to input buffer of palette IDs
 */
void tile_encode_2bpp(EncodedTile* output_vram, const Tile* tile);

/**
 * @brief Encode a single tile into a tile buffer in 4bpp format.
 *
 * @param output_vram Pointer to destination buffer in VRAM
 * @param input_buf Pointer to input buffer of palette IDs
 */
void tile_encode_4bpp(EncodedTile* output_vram, const Tile* tile);

/**
 * @brief Encode a single tile into a tile buffer in 8bpp format.
 *
 * @param output_vram Pointer to destination buffer in VRAM
 * @param input_buf Pointer to input buffer of palette IDs
 */
void tile_encode_8bpp(EncodedTile* output_vram, const Tile* tile);

/**
 * @brief Decode a single tile from a tile buffer in 2bpp format.
 *
 * @param output_buf Pointer to destination buffer of palette IDs
 * @param input_vram Pointer to input buffer of tile data in VRAM
 */
void tile_decode_2bpp(Tile* tile, const EncodedTile* input_vram);

/**
 * @brief Decode a single tile from a tile buffer in 4bpp format.
 *
 * @param output_buf Pointer to destination buffer of palette IDs (size 64)
 * @param input_vram Pointer to input buffer of tile data in VRAM (size 16)
 */
void tile_decode_4bpp(Tile* tile, const EncodedTile* input_vram);

/**
 * @brief Decode a single tile from a tile buffer in 8bpp format.
 *
 * @param output_buf Pointer to destination buffer of palette IDs
 * @param input_vram Pointer to input buffer of tile data in VRAM
 */
void tile_decode_8bpp(Tile* tile, const EncodedTile* input_vram);


/* ===== Unused Stuff ===== */

void translate_tile(uint32_t* output, uint8_t* input, size_t length, uint32_t bit_depth);


#endif  // SNES_3DS_TILE_H_
