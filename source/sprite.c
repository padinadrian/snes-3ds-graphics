/**
 * Handles SNES sprites
 */

/* ===== Includes ===== */

#include <stdbool.h>
#include <string.h>

#include "oam.h"
#include "palette.h"


/* ===== Data ===== */

static bool dirty_flags[SNES_MAX_OBJECTS] = {};


/* ===== Functions ===== */

/** Force a redraw of all sprites. */
void refresh_all_sprites()
{
    memset(dirty_flags, true, SNES_MAX_OBJECTS);
}

/** Set all dirty flags to false. */
void clean_all_sprites()
{
    memset(dirty_flags, false, SNES_MAX_OBJECTS);
}

/** Check if the sprite needs to be updated. */
bool snes_sprite_is_dirty(const uint32_t sprite_id)
{
    return dirty_flags[sprite_id];
}

/** Set the sprite tile. */
void snes_set_sprite_tile(
    ObjectAttributeMemory* oam,
    const uint32_t sprite_id,
    const uint16_t tile_id
)
{
    oam->low_table[sprite_id].tile_id = tile_id;
    dirty_flags[sprite_id] = true;
}

/** Set the sprite position. */
void snes_update_sprite_pos(
    ObjectAttributeMemory* oam,
    const uint32_t sprite_id,
    const uint16_t x_pos,
    const uint16_t y_pos
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

/** Set the sprite horizontal flip on or off. */
void snes_sprite_set_h_flip(
    ObjectAttributeMemory* oam,
    const uint32_t sprite_id,
    const bool flip
)
{
    oam->low_table[sprite_id].h_flip = flip;
    dirty_flags[sprite_id] = true;
}

/** Set the sprite vertical flip on or off. */
void snes_sprite_set_v_flip(
    ObjectAttributeMemory* oam,
    const uint32_t sprite_id,
    const bool flip
)
{
    oam->low_table[sprite_id].v_flip = flip;
    dirty_flags[sprite_id] = true;
}

/** Set the sprite priority relative to the background. */
void snes_sprite_set_priority(
    ObjectAttributeMemory* oam,
    const uint32_t sprite_id,
    const uint8_t priority
)
{
    oam->low_table[sprite_id].priority = (priority & 3); // lower 2 bits only
    dirty_flags[sprite_id] = true;
}

