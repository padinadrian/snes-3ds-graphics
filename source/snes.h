// snes.h

#ifndef TEST_3DS_SNES_H_
#define TEST_3DS_SNES_H_


#include <stdint.h>

/** Max number of objects that can be stored in OAM. */
#define SNES_MAX_OBJECTS 128

/** Length of the OAM low table array. */
#define SNES_OAM_LOW_TABLE_LEN (SNES_MAX_OBJECTS)

/** Length of the OAM high table array. */
#define SNES_OAM_HIGH_TABLE_LEN (SNES_MAX_OBJECTS >> 2)

/**
 * Tilemap object; contains info on a single tile in a background.
 *
 * Size: 16 bits (2 bytes)
 */
typedef struct Tilemap {
    uint8_t v_flip : 1;
    uint8_t h_flip : 1;
    uint8_t priority : 1;
    uint8_t palette_id : 3;
    uint16_t tile_id : 10;
} Tilemap;

/**
 * Size: 34 bits
 */
typedef struct Object {
    uint16_t tile_id : 9;
    uint16_t x_pos : 9;
    uint8_t y_pos : 8;
    uint8_t v_flip : 1;
    uint8_t h_flip : 1;
    uint8_t priority : 2;
    uint8_t palette_id : 3;
    uint8_t obj_size : 1;
} Object;

/**
 * Objects in OAM are split into two tables; This object stores the data in the
 * upper table.
 */
typedef struct ObjectAttributesLower {
    // Lower 8 bits of the object X position
    uint8_t x_pos : 8;
    // Object Y position.
    uint8_t y_pos : 8;
    // Lower 8 bits of the tile ID
    uint8_t tile_id : 8;
    // Vertical flip
    uint8_t v_flip : 1;
    // Horizontal flip
    uint8_t h_flip : 1;
    // Priority (0-3)
    uint8_t priority : 2;
    // Color palette ID
    uint8_t palette_id : 3;
    // MSB of the tile ID (also known as tile page)
    uint8_t tile_page: 1;
} ObjectAttributesLower;

/**
 * Stores the object attributes stored in Table 2.
 */
typedef struct ObjectAttributesUpper {
    // MSB of X position
    uint8_t x_pos_msb : 1;
    // Object size (8x8 or 16x16)
    uint8_t obj_size : 1;
} ObjectAttributesUpper;

/**
 *
 */
typedef struct ObjectAttributeMemory {
    // Object attributes lower table
    ObjectAttributesLower low_table[SNES_OAM_LOW_TABLE_LEN];
    // Object attributes upper table
    uint8_t high_table[SNES_OAM_HIGH_TABLE_LEN];
} ObjectAttributeMemory;


/**
 * TODO: I HAVE NO IDEA IF THIS IS THE CORRECT STRUCTURE
 */
typedef struct Palette {
    uint32_t colors[8];
} Palette;


/**
 * TODO: I HAVE NO IDEA IF THIS IS THE CORRECT STRUCTURE
 */
typedef struct Tile {
    /// 8x8 tile = 64 pixels
    uint8_t pixels[64];
} Tile;

/** Read a single object from OAM. */
void read_object_from_oam(Object* object_ptr, ObjectAttributeMemory* oam, uint16_t index);

/** Initialize OAM memory. */
void init_oam(ObjectAttributeMemory* oam);

#endif  // TEST_3DS_SNES_H_
