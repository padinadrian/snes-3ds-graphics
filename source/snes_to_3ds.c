#include <stdint.h>
#include <3ds.h>
#include <citro3d.h>
#include <citro2d.h>
#include "oam.h"
#include "snes_to_3ds.h"

static C3D_Tex c3d_sprite_tex[SNES_MAX_OBJECTS];
static C2D_Sprite c2d_sprites[SNES_MAX_OBJECTS];

static Palette default_palette;

const Tex3DS_SubTexture subtex_sprite = {
    8, 8,     // width, height
    0.0f, 1.0f, 1.0f, 0.0f  // left, top, right, bottom (??)
};

const size_t index_lookup[64] = {
    0,  1,  4,  5,  16, 17, 20, 21,
    2,  3,  6,  7,  18, 19, 22, 23,
    8,  9,  12, 13, 24, 25, 28, 29,
    10, 11, 14, 15, 26, 27, 30, 31,
    32, 33, 36, 37, 48, 49, 52, 53,
    34, 35, 38, 39, 50, 51, 54, 55,
    40, 41, 44, 45, 56, 57, 60, 61,
    42, 43, 46, 47, 58, 59, 62, 63
};

static bool dirty_flags[SNES_MAX_OBJECTS] = {};


void init_default_palette() {
    default_palette.colors[0] = 0x00000000; // black
    default_palette.colors[1] = 0xFF0000FF; // red
    default_palette.colors[2] = 0x00FF00FF; // green
    default_palette.colors[3] = 0x0000FFFF; // blue
    default_palette.colors[4] = 0xFFFF00FF; // yellow
    default_palette.colors[5] = 0x00FFFFFF; // teal
    default_palette.colors[6] = 0xFF00FFFF; // magenta
    default_palette.colors[7] = 0xFFFFFFFF; // white
}

void init_snes_sprites() {
    for (size_t i = 0; i < SNES_MAX_OBJECTS; ++i) {
        // Init texture
        C3D_Tex* texture_ptr = &(c3d_sprite_tex[i]);
        C3D_TexInitVRAM(c3d_sprite_tex + i, 8, 8, GPU_RGBA8);
        C2D_Image img_sprite = {texture_ptr, &subtex_sprite};

        // Create sprite from texture
        C2D_Sprite* sprite_ptr = &(c2d_sprites[i]);
        C2D_SpriteFromImage(sprite_ptr, img_sprite);
        C2D_SpriteSetCenter(sprite_ptr, 0.f, 0.f);
        C2D_SpriteSetPos(sprite_ptr, 255, 255); // init offscreen
        C2D_SpriteSetRotation(sprite_ptr, C3D_Angle(0.f));
        C2D_SpriteSetDepth(sprite_ptr, 0.f);
    }

    // Draw every sprite at least once on the first frame
    memset(dirty_flags, true, SNES_MAX_OBJECTS);
}

/** Set the sprite tile. */
void snes_set_sprite_tile(ObjectAttributeMemory* oam, uint32_t sprite_id, uint16_t tile_id) {
    oam->low_table[sprite_id].tile_id = tile_id;
    dirty_flags[sprite_id] = true;
}

/** Set the sprite position. */
void snes_update_sprite_pos(
        ObjectAttributeMemory* oam,
        uint32_t sprite_id,
        uint16_t x_pos, uint16_t y_pos) {
    // X position high bit
    uint32_t high_attr_index = sprite_id >> 2;
    uint8_t offset = (sprite_id & 3) << 1;  // 0, 2, 4, or 6
    uint8_t x_pos_msb = 1 << offset;
    if (x_pos > 0xFF) {
        oam->high_table[high_attr_index] |= x_pos_msb;
    }
    else {
        oam->high_table[high_attr_index] &= ~x_pos_msb;
    }

    oam->low_table[sprite_id].x_pos = x_pos & 0xFF;
    oam->low_table[sprite_id].y_pos = y_pos & 0xFF;

    dirty_flags[sprite_id] = true;
}

/** Set horizontal flip on or off. */
void snes_sprite_set_h_flip(ObjectAttributeMemory* oam, uint32_t sprite_id, bool flip) {
    oam->low_table[sprite_id].h_flip = flip;
    dirty_flags[sprite_id] = true;
}

/** Set vertical flip on or off. */
void snes_sprite_set_v_flip(ObjectAttributeMemory* oam, uint32_t sprite_id, bool flip) {
    oam->low_table[sprite_id].v_flip = flip;
    dirty_flags[sprite_id] = true;
}

/** Set vertical flip on or off. */
void snes_sprite_set_priority(ObjectAttributeMemory* oam, uint32_t sprite_id, uint8_t priority) {
    oam->low_table[sprite_id].priority = (priority & 3); // lower 2 bits only
    dirty_flags[sprite_id] = true;
}

/** Update C2D sprites to reflect changes in the SNES objects. */
void update_snes_sprites(ObjectAttributeMemory* oam, Tile* tileset) {
    Object snes_object;
    C2D_Sprite* sprite_ptr = c2d_sprites;

    for (size_t i = 0; i < SNES_MAX_OBJECTS; ++i) {
        if (dirty_flags[i]) {
            read_object_from_oam(&snes_object, oam, i);

            // TODO: Would be nice to have a way to not copy all of
            // this data every single time, and instead only update
            // sprites that have actually changed (dirty bit?)

            // TODO: Use a standard palette for now
            // TODO: Assume texture is 8x8
            // TODO: Check sprite size
            uint32_t* pixel_buffer = (uint32_t*)sprite_ptr->image.tex->data;
            Tile* tile_ptr = &(tileset[snes_object.tile_id]);
            for (size_t i = 0; i < 64; ++i) {
                // Rearrange tile into texture format
                size_t color_id = tile_ptr->pixels[i];
                size_t pixel_id = index_lookup[i];
                pixel_buffer[pixel_id] = default_palette.colors[color_id];
            }

            // Set position
            C2D_SpriteSetPos(sprite_ptr, snes_object.x_pos, snes_object.y_pos);

            // Set scale (flip x/y)
            float x_scale = snes_object.h_flip ? -1.f : 1.f;
            float y_scale = snes_object.v_flip ? -1.f : 1.f;
            C2D_SpriteSetScale(sprite_ptr, x_scale, y_scale);

            // Set depth: priority can be 0-3, scale to range 0.f-1.f
            float depth = snes_object.priority / 3.f;
            C2D_SpriteSetDepth(sprite_ptr, depth);

            if (i == 0)
                printf("\x1b[11;1H sprite:  %d, depth: %6.2f\x1b[K", i, depth);

            sprite_ptr++;
        }
    }
    memset(dirty_flags, false, SNES_MAX_OBJECTS);
}

void draw_snes_sprites() {
    for (
        C2D_Sprite* sprite_ptr = c2d_sprites;
        sprite_ptr < (c2d_sprites + SNES_MAX_OBJECTS);
        sprite_ptr++
    ) {
        if (sprite_ptr->params.pos.x < 256) {
            C2D_DrawSprite(sprite_ptr);
        }
    }
}