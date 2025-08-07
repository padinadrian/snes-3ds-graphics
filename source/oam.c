// oam.c

#include <stdint.h>
#include <string.h>
#include "oam.h"


/** Initialize OAM */

void init_oam(ObjectAttributeMemory* oam) {
    // Set all X position MSB to hide all objects by default
    memset(oam->high_table, 0x55, SNES_OAM_HIGH_TABLE_LEN);
}

/** Read a single object from OAM. */
void read_object_from_oam(Object* object_ptr, ObjectAttributeMemory* oam, uint16_t index) {

    ObjectAttributesLower* low_attrs = oam->low_table + index;

    // Fetch attribute bits from high table
    size_t high_attr_index = index >> 2;
    uint8_t offset = (index & 3) << 1;
    uint8_t high_attrs = (oam->high_table[high_attr_index] >> offset) & 3;

    // Upper bit: Size select bit
    object_ptr->obj_size = (high_attrs >> 1);

    // Lower bit: Upper bit of X position
    uint16_t x_pos = low_attrs->x_pos;
    x_pos |= ((uint16_t)high_attrs & 1) << 8;
    object_ptr->x_pos = x_pos;

    // Copy rest of attributes from low table
    object_ptr->y_pos = low_attrs->y_pos;
    object_ptr->v_flip = low_attrs->v_flip;
    object_ptr->h_flip = low_attrs->h_flip;
    object_ptr->priority = low_attrs->priority;
    object_ptr->palette_id = low_attrs->palette_id;

    uint16_t tile_id = low_attrs->tile_id;
    tile_id |= ((uint16_t)high_attrs >> 1) << 8;
    object_ptr->tile_id = tile_id;

    tile_id |= ((uint16_t)high_attrs >> 1) << 8;
    object_ptr->tile_id = tile_id;
}

