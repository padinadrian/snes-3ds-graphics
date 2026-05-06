/**
 * Handles background layers.
 */

/* ===== Includes ===== */

#include <stdint.h>
#include <string.h>

#include "cgram.h"
#include "texture.h"
#include "tilemap.h"


/* ===== Enums ===== */

enum { TILEMAP_WIDTH = 32 };
enum { TILEMAP_HEIGHT = 32 };
enum { TILEMAP_SIZE = (32 * 32) };  // 1024
enum { PIXELS_PER_TILE = 64 };


/* ===== Functions ===== */

/** Decode a tilemap into a background image. */
void decode_tilemap_4bpp(
    uint32_t* output_buf,
    const Tilemap* tilemap,
    const EncodedTile* tileset,
    const CGRAM* cgram,
    const bool priority
)
{
    // Each tilemap is a 32x32 tile region
    // 32*32 = 1024 tiles
    for (size_t i = 0; i < TILEMAP_SIZE; i += 1)
    {
        if (priority == tilemap[i].priority)
        {
            const uint32_t tile_id = tilemap[i].tile_id << 4;
            const uint32_t palette_id = tilemap[i].palette_id;
            decode_tile_to_texture_4bpp(
                output_buf,
                &tileset[tile_id],
                cgram->palettes[palette_id].colors,
                tilemap[i].h_flip,
                tilemap[i].v_flip
            );
        }
        else
        {
            memset(output_buf, 0, PIXELS_PER_TILE);
        }
        output_buf += PIXELS_PER_TILE;
    }
}

