//! Utility to convert a CGRAM palette to a bitmap image file.

#include "palette_bitmap.hpp"

extern "C" {
    #include "palette.h"
}


int main(int argc, char** argv)
{
    if (argc < 3)
    {
        printf("2 arguments required: [input_file] [output_file]\n");
        return 1;
    }

    CGRAM cgram;
    read_cgram(&cgram, argv[1]);
    generate_palette_bitmap(&cgram, argv[2]);
}
