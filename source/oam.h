/**
 * Object attribute memory.
 */

#ifndef SNES_3DS_OAM_H_
#define SNES_3DS_OAM_H_


/* ===== Includes ===== */

#include <stdint.h>


/* ===== Data ===== */

/** Max number of objects that can be stored in OAM. */
#define SNES_MAX_OBJECTS 128

/** Length of the OAM low table array. */
#define SNES_OAM_LOW_TABLE_LEN (SNES_MAX_OBJECTS)

/** Length of the OAM high table array. */
#define SNES_OAM_HIGH_TABLE_LEN (SNES_MAX_OBJECTS >> 2)

/** Complete size of OAM in bytes. */
#define SNES_OAM_SIZE 544


/* ===== Structs ===== */

/**
 * Size: 34 bits
 */
typedef struct Object {
    uint8_t tile_id : 8;
    uint8_t tile_page : 1;
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
 * lower table.
 */
typedef struct ObjectAttributesLower {
    // Lower 8 bits of the object X position
    uint8_t x_pos : 8;
    // Object Y position.
    uint8_t y_pos : 8;
    // Lower 8 bits of the tile ID
    uint8_t tile_id : 8;
    // MSB of the tile ID (also known as tile page)
    uint8_t tile_page: 1;
    // Color palette ID
    uint8_t palette_id : 3;
    // Priority (0-3)
    uint8_t priority : 2;
    // Horizontal flip
    uint8_t h_flip : 1;
    // Vertical flip
    uint8_t v_flip : 1;
} ObjectAttributesLower;

/**
 * Stores the object attributes stored in the upper table.
 */
typedef struct ObjectAttributesUpper {
    // MSB of X position
    uint8_t x_pos_msb : 1;
    // Object size (8x8 or 16x16)
    uint8_t obj_size : 1;
} ObjectAttributesUpper;

/**
 * Object containing both upper and lower tables of OAM.
 */
 typedef struct ObjectAttributeMemory {
    // Object attributes lower table
    ObjectAttributesLower low_table[SNES_OAM_LOW_TABLE_LEN];
    // Object attributes upper table
    uint8_t high_table[SNES_OAM_HIGH_TABLE_LEN];
} ObjectAttributeMemory;


/* ===== Functions ===== */

/**
 * @brief Initialize OAM memory.
 */
void init_oam(ObjectAttributeMemory* oam);

/**
 * @brief Read a single object from OAM.
 */
void read_object_from_oam(
    Object* object_ptr,
    const ObjectAttributeMemory* oam,
    const uint16_t index
);

#endif  // SNES_3DS_OAM_H_
