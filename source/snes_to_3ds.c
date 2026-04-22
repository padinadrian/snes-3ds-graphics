#include <stdint.h>
#include <3ds.h>
#include <citro3d.h>
#include <citro2d.h>
#include "oam.h"
#include "snes_to_3ds.h"
#include "palette.h"
#include "tile.h"

static C3D_Tex c3d_sprite_tex[SNES_MAX_OBJECTS];
static C2D_Sprite c2d_sprites[SNES_MAX_OBJECTS];

static Palette default_palette;

const Tex3DS_SubTexture subtex_sprite = {
    16, 16,       // width, height
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


void init_default_palette()
{
    default_palette.colors[0] = 0x00000000; // black
    default_palette.colors[1] = 0xFF0000FF; // red
    default_palette.colors[2] = 0x00FF00FF; // green
    default_palette.colors[3] = 0x0000FFFF; // blue
    default_palette.colors[4] = 0xFFFF00FF; // yellow
    default_palette.colors[5] = 0x00FFFFFF; // teal
    default_palette.colors[6] = 0xFF00FFFF; // magenta
    default_palette.colors[7] = 0xFFFFFFFF; // white
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

/**
 * @brief Helper function to decode a SNES tile/color into a 3DS texture.
 *
 * @param[out] pixel_buffer Output buffer to write the raw pixel values
 * @param[in] vram_ptr Pointer to vram with the tile data
 * @param[in] palette Pointer to start of palette to use to color the tile
 */
void decode_tile_to_texture(
        uint32_t* pixel_buffer,
        const uint16_t* vram_ptr,
        const PaletteColor* palette // Start of palette (8 colors)
)
{
    // Each pixel is a 4-bit index (0-15).
    Tile tile = {};
    tile_decode_4bpp(tile.pixels, vram_ptr);

    // All sprites use 4bpp 16-color tiles. Each sprite selects one of
    // 8 palettes from the last half of CGRAM.
    uint16_t* palette_u16 = (uint16_t*)(palette);   // Array of 16 colors

    // Assume texture is 8x8 = 64 pixels.
    // Weird decoding pattern...
    for (size_t i = 0; i < 64; i += 1)
    {
        const uint8_t color_id = tile.pixels[i];
        const size_t pixel_id = index_lookup[i];
        if (color_id == 0)
        {
            pixel_buffer[pixel_id] = 0;
        }
        else
        {
            const uint16_t color = palette_u16[color_id];
            pixel_buffer[pixel_id] = convert_palette_to_rgba(color);
        }
    }
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
            const uint8_t palette_start = (snes_object.palette_id + 8) * 16;

            // Tiles are spaced 32 bytes apart in memory
            // However, vram is uint16_t so divide by 2 bytes
            uint32_t tile_id_offset = snes_object.tile_id << 4;

            // Tile page offset
            // TODO: The actual page offset should come from OBJSEL.
            // TODO: For now we assume pages are contiguous (next page is 1)
            tile_id_offset += (snes_object.tile_page << 12);

            // if ((SNES_MAX_OBJECTS - i) == 0)
            // {
            //     tile_id_offset = 0x420;
            // }

            decode_tile_to_texture(
                pixel_buffer,
                &vram_ptr[tile_id_offset],
                &(cgram->colors[palette_start])
            );

            // TODO: The actual sprite size is controlled by OBJSEL
            // TODO: For now assume size bit set means 16x16
            if (snes_object.obj_size)
            {
                decode_tile_to_texture(
                    pixel_buffer + 64,
                    &vram_ptr[tile_id_offset + 0x10],
                    &(cgram->colors[palette_start])
                );
                decode_tile_to_texture(
                    pixel_buffer + (2 * 64),
                    &vram_ptr[tile_id_offset + 0x100],
                    &(cgram->colors[palette_start])
                );
                decode_tile_to_texture(
                    pixel_buffer + (3 * 64),
                    &vram_ptr[tile_id_offset + 0x110],
                    &(cgram->colors[palette_start])
                );
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