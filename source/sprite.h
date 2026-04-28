/**
 * Handles SNES sprites
 */

#ifndef SNES_3DS_SPRITE_H_
#define SNES_3DS_SPRITE_H_


/* ===== Includes ===== */

#include <stdbool.h>

#include "oam.h"
#include "palette.h"


/* ===== Functions ===== */

/**
 * @brief Force a redraw of all sprites.
 */
void refresh_all_sprites();

/**
 * @brief Set all dirty flags to false.
 */
void clean_all_sprites();

/**
 * @brief Check if the sprite needs to be updated.
 *
 * @param[in] sprite_id Sprite ID to update
 */
bool snes_sprite_is_dirty(const uint32_t sprite_id);

/**
 * @brief Set the sprite tile.
 *
 * @param[out] oam Pointer to object attribute memory
 * @param[in] sprite_id Sprite ID to update
 * @param[in] tile_id Tile ID to use for the sprite
 */
void snes_set_sprite_tile(
    ObjectAttributeMemory* oam,
    const uint32_t sprite_id,
    const uint16_t tile_id
);

/**
 * @brief Set the sprite position.
 *
 * @param oam Pointer to object attribute memory
 * @param sprite_id Sprite ID to update
 * @param x_pos X position
 * @param y_pos Y position
 */
void snes_update_sprite_pos(
    ObjectAttributeMemory* oam,
    const uint32_t sprite_id,
    const uint16_t x_pos,
    const uint16_t y_pos
);

/**
 * @brief Set the sprite horizontal flip on or off.
 *
 * @param oam Pointer to object attribute memory
 * @param sprite_id Sprite ID to update
 * @param flip true if the sprite should be flipped, otherwise false
 */
void snes_sprite_set_h_flip(
    ObjectAttributeMemory* oam,
    const uint32_t sprite_id,
    const bool flip
);

/**
 * @brief Set the sprite vertical flip on or off.
 *
 * @param oam Pointer to object attribute memory
 * @param sprite_id Sprite ID to update
 * @param flip true if the sprite should be flipped, otherwise false
 */
void snes_sprite_set_v_flip(
    ObjectAttributeMemory* oam,
    const uint32_t sprite_id,
    const bool flip
);

/**
 * @brief Set the sprite priority to the background.
 *
 * @param oam Pointer to object attribute memory
 * @param sprite_id Sprite ID to update
 * @param priority Priority relative to background (0-3)
 */
void snes_sprite_set_priority(
    ObjectAttributeMemory* oam,
    const uint32_t sprite_id,
    const uint8_t priority
);


#endif  // SNES_3DS_SPRITE_H_
