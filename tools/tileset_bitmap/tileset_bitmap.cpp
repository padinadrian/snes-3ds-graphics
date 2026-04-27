//! Utility to convert a CGRAM palette to a bitmap image file.

#include <cassert>

#include "palette_bitmap.hpp"

extern "C" {
    #include "oam.h"
    #include "palette.h"
    #include "tile.h"
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
        bitmap_image& image,
        const uint8_t tile[],
        const CGRAM& cgram,
        const size_t x_offset,
        const size_t y_offset,
        const int bpp
)
{
    const PaletteColor* palette = cgram.palettes[12].colors;

    int bit_offset = 8 - bpp;
    for (size_t y = 0; y < 8; y += 1)
    {
        for (size_t x = 0; x < 8; x += 1)
        {
            const size_t x_final = x_offset + x;
            const size_t y_final = y_offset + y;

            const size_t tile_index = (y * 8) + x;
            const uint8_t color_index = tile[tile_index];
            const PaletteColorBits color = *(PaletteColorBits*)(&palette[color_index]);

            const uint8_t red = palette_color_get_red(color) << 3;
            const uint8_t green = palette_color_get_green(color) << 3;
            const uint8_t blue = palette_color_get_blue(color) << 3;

            image.set_pixel(x_final, y_final, red, green, blue);
            printf("index: %d; red: %d; green: %d; blue: %d\n", color_index, red, green, blue);
        }
    }
}

/**
 * @brief Generate a bitmap image representing the given VRAM state.
 *
 * @note For now this is hardcoded to work for 4bpp. Other modes will be
 * supported in the future.
 */
void generate_tileset_bitmap(
        const uint16_t* vram,
        const CGRAM& cgram,
        int bpp,
        const char* output_file
)
{
    // Settings
    size_t vram_step = 0;
    void (*tile_decode_fn)(Tile*, const EncodedTile*);

    if (bpp == 2)
    {
        vram_step = 8;
        tile_decode_fn = tile_decode_2bpp;
    }
    else if (bpp == 4)
    {
        vram_step = 16;
        tile_decode_fn = tile_decode_4bpp;
    }
    else if (bpp == 8)
    {
        vram_step = 32;
        tile_decode_fn = tile_decode_8bpp;
    }
    else
    {
        printf("Invalid bpp: %d\n", bpp);
        return;
    }

    const uint32_t width = 128;   // 16 columns of 8x8 tiles = 128 pixels wide
    const uint32_t height = VRAM_COUNT / 8 / bpp; // Number of pixels tall
    bitmap_image image{width, height};

    Tile tile = {};

    // Each tile is 8x8 pixels
    size_t vram_offset = 0;
    for (size_t y = 0; y < height; y += 8)
    {
        for (size_t x = 0; x < width; x += 8)
        {
            printf("vram offset; 0x%08lx (%lu)\n", vram_offset, vram_offset);
            tile_decode_fn(&tile, &vram[vram_offset]);
            copy_to_bitmap(image, tile.pixels, cgram, x, y, bpp);
            memset(&tile, 0, sizeof(tile));
            vram_offset += vram_step;
        }
    }
    image.save_image(output_file);
}


/**
 * Arguments:
 * [1] - <output_bmp> (string)
 * [2] - <input_vram> (string)
 * [3] - <input_cgram> (string)
 * [4] - <bits_per_pixel> (int)
 * [5] - <bits_per_pixel> (int)
 */
int main(int argc, char** argv)
{
    if (argc < 3)
    {
        printf("2 arguments required: [input_file] [output_file]\n");
        return 1;
    }

    int bpp = 4;
    if (argc > 4)
    {
        bpp = argv[4][0] - '0';
        if (bpp != 2 && bpp != 4 && bpp != 8)
        {
            printf("Invalid bpp: %s\n", argv[4]);
            return 1;
        }
    }

    uint16_t vram[VRAM_COUNT];
    size_t result = read_vram(vram, argv[1]);
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

    CGRAM cgram;
    result = read_cgram(&cgram, argv[3]);
    if (result < 1)
    {
        printf("Read failed; file not found.\n");
    }

    generate_tileset_bitmap(vram, cgram, bpp, argv[2]);
}
