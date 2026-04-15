//! Utility to convert a CGRAM palette to a bitmap image file.

#include <cassert>

#include "palette_bitmap.hpp"

extern "C" {
    #include "background.h"
    #include "oam.h"
    #include "palette.h"
}

const size_t VRAM_SIZE = 65536;
const size_t VRAM_COUNT = VRAM_SIZE / 2;


/**
 * @brief Read VRAM from file.
 */
size_t read_vram(uint16_t* vram, const char* vram_file)
{
    size_t read_result = 0;
    FILE* fp = fopen(vram_file, "rb");
    if (!fp)
    {
        return 0;
    }

    // Read 512 bytes at a time.
    printf("fp: %p\n", fp);
    for (size_t i = 0; i < 128; ++i) {
        read_result += fread(
            vram + (i * 256),
            2,
            256,
            fp
        );
    }
    printf("read_result: %lu\n", read_result);
    return read_result;
}

/**
 * @brief Copy a single 8x8 tile to the image at the given pixel offset.
 */
void copy_to_bitmap(
        uint8_t tile[],
        bitmap_image& image,
        size_t x_offset,
        size_t y_offset
)
{
    for (size_t y = 0; y < 8; y += 1)
    {
        for (size_t x = 0; x < 8; x += 1)
        {
            const size_t index = (y * 8) + x;
            size_t x_final = x_offset + x;
            size_t y_final = y_offset + y;
            printf("tile: x: %lu;\ty: %lu;\tindex: %lu\n", x_final, y_final, index);
            image.set_pixel(
                x_final,
                y_final,
                tile[index] << 4,
                tile[index] << 4,
                tile[index] << 4
            );
        }
    }
}

/**
 * @brief Generate a bitmap image representing the given VRAM state.
 *
 * @note For now this is hardcoded to work for 4bpp. Other modes will be
 * supported in the future.
 */
void generate_tileset_bitmap(uint16_t* vram, const char* output_file)
{
    // Tile* tileset = (Tile*)(&vram[0]);

    const size_t width = 128;   // 16 columns of 8x8 tiles = 128 pixels wide
    const size_t height = 1024; // 64 rows of 8x8 tiles = 512 pixels tall
    bitmap_image image{width, height};

    uint8_t tile[64] = {};

    // Each tile is 8x8 pixels
    // printf("vram: x: %lu;\ty: %lu;\tindex: %lu\n", x, y, index);
    size_t vram_offset = 0;
    for (size_t y = 0; y < height; y += 8)
    {
        for (size_t x = 0; x < width; x += 8)
        {
            printf("vram offset; 0x%08lx (%lu)\n", vram_offset, vram_offset);
            tile_decode_4bpp(tile, &vram[vram_offset]);
            copy_to_bitmap(tile, image, x, y);
            memset(tile, 0, 64);
            vram_offset += 16;
        }
    }

    image.save_image(output_file);
}


int main(int argc, char** argv)
{
    if (argc < 3)
    {
        printf("2 arguments required: [input_file] [output_file]\n");
        return 1;
    }

    uint16_t vram[VRAM_COUNT];
    const size_t result = read_vram(vram, argv[1]);
    if (result < 1)
    {
        printf("Read failed; file not found.\n");
        return 1;
    }
    if (result != VRAM_COUNT)
    {
        printf("Read failed; file incorrect size.\n");
        return 1;
    }

    generate_tileset_bitmap(vram, argv[2]);
}
