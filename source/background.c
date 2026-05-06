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
#include "tilemap.h"


/* ===== Data ===== */

/** Four background layers each with an upper and lower layer. */
typedef enum BackgroundId {
    BACKGROUND_LAYER_4L,
    BACKGROUND_LAYER_4H,
    BACKGROUND_LAYER_3L,
    BACKGROUND_LAYER_3H,
    BACKGROUND_LAYER_2L,
    BACKGROUND_LAYER_2H,
    BACKGROUND_LAYER_1L,
    BACKGROUND_LAYER_1H,
    // Keep this one last
    NUM_BACKGROUNDS,
} BackgroundId;

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
 * @brief Get the direct pixel buffer for the given background layer.
 */
static uint32_t* get_layer_pixel_buffer(size_t layer_id)
{
    return (uint32_t*)(backgrounds[layer_id].image.tex->data);
}


/** @brief Get background 1 tilemap address. */
inline uint32_t get_bg1_tilemap(const BGnSC_t* bgnsc)
{
    return bgnsc[0].tilemap << 9;
}

/** @brief Get background 2 tilemap address. */
inline uint32_t get_bg2_tilemap(const BGnSC_t* bgnsc)
{
    return bgnsc[1].tilemap << 9;
}

/** @brief Get background 3 tilemap address. */
inline uint32_t get_bg3_tilemap(const BGnSC_t* bgnsc)
{
    return bgnsc[2].tilemap << 9;
}

/** @brief Get background 4 tilemap address. */
inline uint32_t get_bg4_tilemap(const BGnSC_t* bgnsc)
{
    return bgnsc[3].tilemap << 9;
}

/** @brief Get background 1 tileset address. */
inline uint32_t get_bg1_tileset(const BG12NBA_t reg)
{
    return (uint32_t)(reg.bg1_tileset) << 11;
}

/** @brief Get background 2 tileset address. */
inline uint32_t get_bg2_tileset(const BG12NBA_t reg)
{
    return (uint32_t)(reg.bg2_tileset) << 11;
}

/** @brief Get background 3 tileset address. */
inline uint32_t get_bg3_tileset(const BG34NBA_t reg)
{
    return (uint32_t)(reg.bg3_tileset) << 11;
}

/** @brief Get background 4 tileset address. */
inline uint32_t get_bg4_tileset(const BG34NBA_t reg)
{
    return (uint32_t)(reg.bg4_tileset) << 11;
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
    // TODO: Actually use the registers properly
    const size_t bg1_tilemap_offset = get_bg1_tilemap(bgnsc);
    const size_t bg2_tilemap_offset = get_bg2_tilemap(bgnsc);
    const Tilemap* bg1_tilemap = (const Tilemap*)(vram + bg1_tilemap_offset);
    const Tilemap* bg2_tilemap = (const Tilemap*)(vram + bg2_tilemap_offset);

    const size_t bg1_tileset_offset = get_bg1_tileset(bg12nba);
    const size_t bg2_tileset_offset = get_bg2_tileset(bg12nba);
    const EncodedTile* bg1_tileset = (const EncodedTile*)(vram + bg1_tileset_offset);
    const EncodedTile* bg2_tileset = (const EncodedTile*)(vram + bg2_tileset_offset);

    decode_tilemap_4bpp(
        get_layer_pixel_buffer(BACKGROUND_LAYER_2L),
        bg2_tilemap,
        bg2_tileset,
        cgram,
        false
    );
    decode_tilemap_4bpp(
        get_layer_pixel_buffer(BACKGROUND_LAYER_1L),
        bg1_tilemap,
        bg1_tileset,
        cgram,
        false
    );
    decode_tilemap_4bpp(
        get_layer_pixel_buffer(BACKGROUND_LAYER_2H),
        bg2_tilemap,
        bg2_tileset,
        cgram,
        true
    );
    decode_tilemap_4bpp(
        get_layer_pixel_buffer(BACKGROUND_LAYER_1H),
        bg1_tilemap,
        bg1_tileset,
        cgram,
        true
    );

    // TODO: Layer 3

    // TODO: Optimization: Don't do this every frame
    backgrounds[BACKGROUND_LAYER_4L].depth = 0.0f;
    backgrounds[BACKGROUND_LAYER_4H].depth = 0.0f;
    backgrounds[BACKGROUND_LAYER_3L].depth = 0.0f;
    backgrounds[BACKGROUND_LAYER_3H].depth = 0.1f;
    // Note: This does not match the documentation I have found but it
    // does render the layers in the correct order.
    backgrounds[BACKGROUND_LAYER_2L].depth = 0.2f;
    backgrounds[BACKGROUND_LAYER_1L].depth = 0.4f;
    backgrounds[BACKGROUND_LAYER_2H].depth = 0.6f;
    backgrounds[BACKGROUND_LAYER_1H].depth = 0.7f;

    // TODO: Layer 3
}


/* ----- Other Functions ----- */

/** Initialize the Background object with the default values. */
static void init_background(Background* bg_ptr)
{
    bg_ptr->x = 0.0f;
    bg_ptr->y = 0.0f;
    bg_ptr->depth = 0.0f;
    bg_ptr->x_scale = 1.0f;
    bg_ptr->y_scale = 1.0f;
}

/** Draw a Background object with the stored attributes. */
static void draw_background_ptr(const Background* bg_ptr)
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

/** Draw the background for the given ID. */
static void draw_background(const BackgroundId id)
{
    draw_background_ptr(&backgrounds[id]);
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

void draw_background_layer0()
{
    // Mode 0: 4L, 3L
    // Mode 1: 3L
    // Mode 2-6,8: 2L
    // Mode 7: None

    // TODO: Assume background mode 1
    draw_background(BACKGROUND_LAYER_3L);
}

void draw_background_layer1()
{
    // Mode 0: 4H, 3H
    // Mode 1: 3H
    // Mode 2-8: 1L

    // TODO: Assume background mode 1
    draw_background(BACKGROUND_LAYER_3H);
    // Note: this does not match the documentation I have found but it
    // does render the layers in the correct order.
    draw_background(BACKGROUND_LAYER_2L);
}

void draw_background_layer2()
{
    // Mode 0-1: 2L, 1L
    // Mode 2-6,8: 2H
    // Mode 7: None

    // TODO: Assume background mode 1
    draw_background(BACKGROUND_LAYER_1L);
}

void draw_background_layer3()
{
    // Mode 0-1: 1H, 2H
    // Mode 2-7: 1H
    // Mode 8: None

    // TODO: Assume background mode 1
    draw_background(BACKGROUND_LAYER_2H);
    draw_background(BACKGROUND_LAYER_1H);
}

void draw_background_layer4()
{
    // Mode 1: 3H
    // All other modes: None

    // TODO: Assume background mode 1
    // TODO: For Mode 1, 3H may change position based on priority
    // draw_background(BACKGROUND_LAYER_3H);
}

