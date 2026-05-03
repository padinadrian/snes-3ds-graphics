/**
 * Handles background layers.
 */

/* ===== Includes ===== */

#include "background.h"

#include <stdint.h>

#include <3ds.h>
#include <citro3d.h>
#include <citro2d.h>

#include "cgram.h"
#include "texture.h"
#include "tile.h"


/* ===== Data ===== */

enum { NUM_BACKGROUNDS = 4 };
enum {
    BG_TEX_WIDTH = 256,
    BG_TEX_HEIGHT = 256,
};

static C3D_Tex c3d_bg_textures[NUM_BACKGROUNDS] = {};
static Background backgrounds[NUM_BACKGROUNDS] = {};
static bool dirtyFlags[NUM_BACKGROUNDS] = {};

const Tex3DS_SubTexture bg_subtex = {
    BG_TEX_WIDTH, BG_TEX_HEIGHT,
    0.0f, 1.0f, 1.0f, 0.0f
};


/* ===== Functions ===== */

/* ----- Helper Functions ----- */

/**
 * @brief Decode a tilemap into a background image.
 *
 * @param[out] output_buf Output pixel buffer
 * @param[in] tilemap Pointer to tilemap with tile and color info
 * @param[in] tileset Pointer to vram section containing tile data
 * @param[in] cgram Color palette
 */
inline void decode_background_4bpp(
    uint32_t* output_buf,
    const Tilemap* tilemap,
    const EncodedTile* tileset,
    const CGRAM* cgram
)
{
    // Each tilemap is a 32x32 tile region
    // 32*32 = 1024 tiles
    for (size_t i = 0; i < TILEMAP_SIZE; i += 1)
    {
        const uint32_t tile_id = tilemap[i].tile_id;
        const uint32_t palette_id = tilemap[i].palette_id;
        decode_tile_to_texture_4bpp(
            output_buf,
            &tileset[tile_id << 4],
            cgram->palettes[palette_id].colors,
            tilemap[i].h_flip,
            tilemap[i].v_flip
        );
        output_buf += 64;
    }
}


/* ----- Background Modes ----- */

/**
 * @brief Background Mode 1
 *
 * BG1 = 4bpp (16 color)
 * BG2 = 4bpp (16 color)
 * BG3 = 2bpp (4 color)
 *
 * Priority (front to back):
 * 3H S3 1H 2H S2 1L 2L S1 3H S0 3L
 */
static void background_mode1(
    const uint16_t* vram,
    const CGRAM* cgram,
    const BGnSC_t bgnsc[4],
    const BG12NBA_t bg12nba,
    const BG34NBA_t bg34nba
)
{
    // const Tilemap* bg1_address = (const Tilemap*)(bgnsc[0].address << 9);
    // const Tilemap* bg2_tilemap = (const Tilemap*)(bgnsc[1].address << 9);
    const Tilemap* bg2_tilemap = (const Tilemap*)vram;
    // const uint16_t* bg2_tileset_addr = vram + (get_bg2_address(bg12nba) >> 1);
    const uint16_t* bg2_tileset_addr = vram + 0x2000;

    uint32_t* pixel_buffer = (uint32_t*)(backgrounds[1].image.tex->data);

    decode_background_4bpp(
        pixel_buffer,
        bg2_tilemap,
        bg2_tileset_addr,
        cgram
    );
}


/* ----- Other Functions ----- */

/** Initialize the Background object with the default values. */
static void init_background(Background* bg_ptr)
{
    bg_ptr->x = 0.0f;
    bg_ptr->y = -16.0f;
    bg_ptr->depth = 0.0f;
    bg_ptr->x_scale = 1.0f;
    bg_ptr->y_scale = 1.0f;
}

/** Draw a Background object with the stored attributes. */
static void draw_background(const Background* bg_ptr)
{
    C2D_DrawImageAt(
        bg_ptr->image,
        bg_ptr->x,
        bg_ptr->y,
        bg_ptr->depth,
        NULL,
        bg_ptr->x_scale,
        bg_ptr->y_scale
    );
}

/** Initialize all the backgrounds. */
void init_backgrounds()
{
    for (size_t i = 0; i < NUM_BACKGROUNDS; ++i)
    {
        // Init texture
        C3D_Tex* texture_ptr = &(c3d_bg_textures[i]);
        C3D_TexInitVRAM(texture_ptr, BG_TEX_WIDTH, BG_TEX_HEIGHT, GPU_RGBA8);
        C2D_Image bg_image = {texture_ptr, &bg_subtex};
        backgrounds[i].image = bg_image;

        dirtyFlags[i] = true;
        init_background(&backgrounds[i]);
    }
}

/** Update all the background images. */
void update_backgrounds(
    const uint16_t* vram,
    const CGRAM* cgram,
    const BGnSC_t bgnsc[4],
    const BG12NBA_t bg12nba,
    const BG34NBA_t bg34nba
)
{
    // TODO: Switch on background mode
    // For now assume mode 1
    if (dirtyFlags[0])
    {
        background_mode1(vram, cgram, bgnsc, bg12nba, bg34nba);
        dirtyFlags[0] = false;
    }
}

/** Draw all the backgrounds. */
void draw_backgrounds()
{
    for (size_t i = 0; i < NUM_BACKGROUNDS; ++i)
    {
        draw_background(&backgrounds[1]);
    }
}
