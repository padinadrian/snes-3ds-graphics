

#pragma once


/**
 * Source: https://snes.nesdev.org/wiki/PPU_registers
 */
typedef enum {
    INIDISP = 0x2100,   // Forced blanking (F), screen brightness (B)
    OBJSEL  = 0x2101,   // OBJ sprite size (S), name secondary select (N), name base address (B)
    OAMADDL = 0x2102,   // OAM word address (A) lower byte
    OAMADDH = 0x2103,   // Priority rotation (P), address high bit (B)
    OAMDATA = 0x2104,   // OAM data write byte (2x for word) (D), increments OAMADD byte
    BGMODE  = 0x2105,   // Tilemap tile size (#), BG3 priority (P), BG mode (M)
    MOSAIC  = 0x2106,   // Mosaic size (S), mosaic BG enable (#)
    BG1SC   = 0x2107,   // Tilemap VRAM address (A), vertical/horizontal tilemap count (Y/X)
    BG2SC   = 0x2108,
    BC3SC   = 0x2109,
    BG4SC   = 0x210A,
    BG12NBA = 0x210B,   // BG2 CHR base address (B), BG1 CHR base address (A)
    BG34NBA = 0x210C,   // BG4 CHR base address (D), BG3 CHR base address (C)
    BG1HOFS = 0x210D,   // BG1 horizontal scroll (X), Mode 7 horizontal scroll
    BG1VOFS = 0x210E,   // BG1 vertical scroll (Y), Mode 7 vertical scroll
    BG2HOFS = 0x210F,   // BG2 horizontal scroll (X)
    BG2VOFS = 0x2110,   // BG2 vertical scroll (Y)
    BG3HOFS = 0x2111,   // BG3 horizontal scroll (X)
    BG3VOFS = 0x2112,   // BG3 vertical scroll (Y)
    BG4HOFS = 0x2113,   // BG4 horizontal scroll (X)
    BG4VOFS = 0x2114,   // BG4 vertical scroll (Y)
    VMAIN   = 0x2115,   // VRAM address increment mode (M), remapping (R), increment size (I)
    VMADDL  = 0x2116,   // VRAM word address lower byte
    VMADDH  = 0x2117,   // VRAM word address upper byte
    VMDATAL = 0x2118,   // VRAM data write. Increments VMADD after write according to VMAIN setting
    VMDATAH = 0x2119,   //
} MmemoryMapRegister;
