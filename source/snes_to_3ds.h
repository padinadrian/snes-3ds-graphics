/**
 * Master file (TODO)
 */

#ifndef SNES_TO_3DS_H_
#define SNES_TO_3DS_H_


/* ===== Includes ===== */

#include <3ds.h>
#include <citro3d.h>
#include <citro2d.h>

#include "oam.h"
#include "palette.h"


/* ===== Functions ===== */

void init_snes_sprites();
void update_snes_sprites(ObjectAttributeMemory* oam, const uint16_t* vram_ptr, const CGRAM* cgram);
void draw_snes_sprites();


#endif  // SNES_TO_3DS_H_
