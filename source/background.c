
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>


void translate_tile(uint32_t* output, uint8_t* input, size_t length, uint32_t bit_depth) {
    // TODO: Check bounds?

    switch (bit_depth) {
        case 2: {   // 2bpp, 4 colors
            // Alternate bytes: first LSB, then MSB
            for (size_t i = 0; i < length;) {
                size_t row = i << 2; // i * 4;

                uint8_t lsb = input[i++];
                for (size_t n = 0; n < 8; ++n) {
                    output[row + 7 - n] = lsb & 1;
                    lsb >>= 1;
                }

                uint8_t msb = input[i++];
                for (size_t n = 0; n < 8; ++n) {
                    output[row + 7 - n] |= ((msb & 1) << 1);
                    msb >>= 1;
                }
            }
        }
        case 4: {   // 4bpp, 16 colors

        }
        case 8: {   // 8bpp, direct color, 8-bit (or 11-bit?) color

        }
    }


    for (size_t i = 0; i < bit_depth; ++i) {

    }
}
