#include <cstdio>

extern "C" {
    #include "oam.h"
}

/**
 * @brief Read OAM from file.
 */
size_t read_oam(ObjectAttributeMemory* oam, const char* filename)
{
    FILE* fp = fopen(filename, "rb");
    if (!fp)
    {
        printf("File not found: %s\n", filename);
        return 0;
    }
    size_t read_result = fread(
        &oam->low_table,
        sizeof(ObjectAttributesLower),
        SNES_OAM_LOW_TABLE_LEN,
        fp
    );
    read_result = fread(
        &oam->high_table,
        sizeof(ObjectAttributesUpper),
        SNES_OAM_HIGH_TABLE_LEN,
        fp
    );
    return 544;
}


int main(int argc, const char* argv[])
{
    const char* filename = argv[1];

    ObjectAttributeMemory oam;
    init_oam(&oam);
    size_t result = read_oam(&oam, filename);
    if (result != 544)
    {
        printf("Failed to read file.\n");
        return 1;
    }

    Object snes_object{};
    for (uint32_t i = 0; i < SNES_MAX_OBJECTS; ++i)
    {
        read_object_from_oam(&snes_object, &oam, i);
        const uint32_t tile = 0x8000 + (snes_object.tile_id * 0x20);
        printf(
            "i: %02x, tile: %04x, page: %d, x: %d, y: %d, v: %d, h: %d, prio: %d, pal: %d, size: %d\n",
            i,
            tile,
            snes_object.tile_page,
            snes_object.x_pos,
            snes_object.y_pos,
            snes_object.v_flip,
            snes_object.h_flip,
            snes_object.priority,
            snes_object.palette_id,
            snes_object.obj_size
        );
    }

    return 0;
}