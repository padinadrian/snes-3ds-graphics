/**
 * Master file for managing SNES backgrounds and sprites.
 */

/* ===== Includes ===== */

#include "snes_to_3ds.h"

#include <stdint.h>

#include <3ds.h>
#include <citro3d.h>
#include <citro2d.h>

#include "oam.h"
#include "palette.h"
#include "texture.h"
#include "tile.h"


/* ===== Data ===== */

static C3D_Tex c3d_sprite_tex[SNES_MAX_OBJECTS];
static C2D_Sprite c2d_sprites[SNES_MAX_OBJECTS];

static Palette default_palette;

const Tex3DS_SubTexture subtex_sprite = {
    16, 16,       // width, height
    0.0f, 1.0f, 1.0f, 0.0f  // left, top, right, bottom (??)
};

static bool dirty_flags[SNES_MAX_OBJECTS] = {};


/* ===== Functions ===== */

void init_default_palette()
{
    default_palette.colors[0] = encode_rgba_to_palette(0x00000000); // black
    default_palette.colors[1] = encode_rgba_to_palette(0xFF0000FF); // red
    default_palette.colors[2] = encode_rgba_to_palette(0x00FF00FF); // green
    default_palette.colors[3] = encode_rgba_to_palette(0x0000FFFF); // blue
    default_palette.colors[4] = encode_rgba_to_palette(0xFFFF00FF); // yellow
    default_palette.colors[5] = encode_rgba_to_palette(0x00FFFFFF); // teal
    default_palette.colors[6] = encode_rgba_to_palette(0xFF00FFFF); // magenta
    default_palette.colors[7] = encode_rgba_to_palette(0xFFFFFFFF); // white
}

void init_snes_sprites()
{
    for (size_t i = 0; i < SNES_MAX_OBJECTS; ++i)
    {
        // Init texture
        C3D_Tex* texture_ptr = &(c3d_sprite_tex[i]);
        C3D_TexInitVRAM(texture_ptr, 16, 16, GPU_RGBA8);
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
    refresh_all_sprites();
}

void refresh_all_sprites()
{
    memset(dirty_flags, true, SNES_MAX_OBJECTS);
}

/** Set the sprite tile. */
void snes_set_sprite_tile(
        ObjectAttributeMemory* oam,
        uint32_t sprite_id,
        uint16_t tile_id
)
{
    oam->low_table[sprite_id].tile_id = tile_id;
    dirty_flags[sprite_id] = true;
}

/** Set the sprite position. */
void snes_update_sprite_pos(
        ObjectAttributeMemory* oam,
        uint32_t sprite_id,
        uint16_t x_pos, uint16_t y_pos
)
{
    // X position high bit
    uint32_t high_attr_index = sprite_id >> 2;
    uint8_t offset = (sprite_id & 3) << 1;  // 0, 2, 4, or 6
    uint8_t x_pos_msb = 1 << offset;
    if (x_pos > 0xFF)
    {
        oam->high_table[high_attr_index] |= x_pos_msb;
    }
    else
    {
        oam->high_table[high_attr_index] &= ~x_pos_msb;
    }

    oam->low_table[sprite_id].x_pos = x_pos & 0xFF;
    oam->low_table[sprite_id].y_pos = y_pos & 0xFF;

    dirty_flags[sprite_id] = true;
}

/** Set horizontal flip on or off. */
void snes_sprite_set_h_flip(
        ObjectAttributeMemory* oam,
        uint32_t sprite_id,
        bool flip
)
{
    oam->low_table[sprite_id].h_flip = flip;
    dirty_flags[sprite_id] = true;
}

/** Set vertical flip on or off. */
void snes_sprite_set_v_flip(
        ObjectAttributeMemory* oam,
        uint32_t sprite_id,
        bool flip
)
{
    oam->low_table[sprite_id].v_flip = flip;
    dirty_flags[sprite_id] = true;
}

/** Set vertical flip on or off. */
void snes_sprite_set_priority(
        ObjectAttributeMemory* oam,
        uint32_t sprite_id,
        uint8_t priority
)
{
    oam->low_table[sprite_id].priority = (priority & 3); // lower 2 bits only
    dirty_flags[sprite_id] = true;
}


/** Update C2D sprites to reflect changes in the SNES objects. */
void update_snes_sprites(
        ObjectAttributeMemory* oam,
        const uint16_t* vram_ptr,
        const CGRAM* cgram
)
{
    Object snes_object;
    C2D_Sprite* sprite_ptr = c2d_sprites;

    for (size_t i = 0; i < SNES_MAX_OBJECTS; ++i)
    {
        if (dirty_flags[i])
        {
            // Update objects in reverse order
            // This ensures that the lowest objects are drawn last
            // This should draw them on top
            read_object_from_oam(&snes_object, oam, SNES_MAX_OBJECTS - i);

            // TODO: Would be nice to have a way to not copy all of
            // this data every single time, and instead only update
            // sprites that have actually changed (dirty bit?)

            // TODO: Assume texture is 8x8
            // TODO: Check sprite size
            uint32_t* pixel_buffer = (uint32_t*)sprite_ptr->image.tex->data;

            // All sprites use 4bpp 16-color tiles. Each sprite selects one of
            // 8 palettes from the last half of CGRAM.
            const uint8_t palette_start = (snes_object.palette_id + 8);

            // Tiles are spaced 32 bytes apart in memory
            // However, vram is uint16_t so divide by 2 bytes
            uint32_t tile_id_offset = snes_object.tile_id << 4;

            // Tile page offset
            // TODO: The actual page offset should come from OBJSEL.
            // TODO: For now we assume pages are contiguous (next page is 1)
            tile_id_offset += (snes_object.tile_page << 12);

            decode_tile_to_texture_4bpp(
                pixel_buffer,
                &vram_ptr[tile_id_offset],
                cgram->palettes[palette_start].colors,
                false,
                false
            );

            // TODO: The actual sprite size is controlled by OBJSEL
            // TODO: For now assume size bit set means 16x16
            if (snes_object.obj_size)
            {
                decode_tile_to_texture_4bpp(
                    pixel_buffer + 64,
                    &vram_ptr[tile_id_offset + 0x10],
                    cgram->palettes[palette_start].colors,
                    false,
                    false
                );
                decode_tile_to_texture_4bpp(
                    pixel_buffer + (2 * 64),
                    &vram_ptr[tile_id_offset + 0x100],
                    cgram->palettes[palette_start].colors,
                    false,
                    false
                );
                decode_tile_to_texture_4bpp(
                    pixel_buffer + (3 * 64),
                    &vram_ptr[tile_id_offset + 0x110],
                    cgram->palettes[palette_start].colors,
                    false,
                    false
                );
            }
            else
            {
                if (snes_object.h_flip)
                {
                    snes_object.x_pos -= 8;
                }
                if (snes_object.v_flip)
                {
                    snes_object.y_pos -= 8;
                }
            }

            // Set position
            C2D_SpriteSetPos(sprite_ptr, snes_object.x_pos, snes_object.y_pos);

            // Set scale (flip x/y)
            const float x_scale = snes_object.h_flip ? -1.f : 1.f;
            const float y_scale = snes_object.v_flip ? -1.f : 1.f;
            C2D_SpriteSetScale(sprite_ptr, x_scale, y_scale);

            // Priority can be 0-3. This determines depth relative to background.
            // Do not worry about sprite depth relative to each other.
            const float depth = (float)(snes_object.priority) / 4.0f;
            C2D_SpriteSetDepth(sprite_ptr, depth);

            sprite_ptr++;
        }
    }
    memset(dirty_flags, false, SNES_MAX_OBJECTS);
}

void draw_snes_sprites()
{
    enum { MAX_DISPLAY_X_POS = 256 };

    for (
        C2D_Sprite* sprite_ptr = c2d_sprites;
        sprite_ptr < (c2d_sprites + SNES_MAX_OBJECTS);
        sprite_ptr++
    )
    {
        if (sprite_ptr->params.pos.x < MAX_DISPLAY_X_POS)
        {
            C2D_DrawSprite(sprite_ptr);
        }
    }
}