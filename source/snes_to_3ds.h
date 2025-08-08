// snes_to_3ds.h

#ifndef SNES_TO_3DS_H
#define SNES_TO_3DS_H

#include <3ds.h>
#include <citro3d.h>
#include <citro2d.h>
#include "oam.h"
#include "palette.h"

void init_default_palette();
void init_snes_sprites();
void update_snes_sprites(ObjectAttributeMemory* oam, Tile* tileset, CGRAM* cgram);
void draw_snes_sprites();
void snes_set_sprite_tile(ObjectAttributeMemory* oam, uint32_t sprite_id, uint16_t tile_id);
void snes_update_sprite_pos(ObjectAttributeMemory* oam,uint32_t sprite_id, uint16_t x_pos, uint16_t y_pos);
void snes_sprite_set_h_flip(ObjectAttributeMemory* oam, uint32_t sprite_id, bool flip);
void snes_sprite_set_v_flip(ObjectAttributeMemory* oam, uint32_t sprite_id, bool flip);
void snes_sprite_set_priority(ObjectAttributeMemory* oam, uint32_t sprite_id, uint8_t priority);

#endif  // SNES_TO_3DS_H
