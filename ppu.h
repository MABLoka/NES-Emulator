#ifndef PPU_H
#define PPU_H

#include "Cartridge.h"
#include "raylib.h"



typedef struct{
    uint8_t     tblName[2][1024];
	uint8_t     tblPattern[2][4096];
	uint8_t		tblPalette[32];
} PPU;

typedef union
	{
		struct
		{
			uint8_t unused : 5;
			uint8_t spriteOverflow : 1;
			uint8_t spriteZeroHit : 1;
			uint8_t verticalBlank : 1;
		};

		uint8_t reg;
} STATUS;

typedef union
	{
		struct
		{
			uint8_t grayscale : 1;
			uint8_t render_background_left : 1;
			uint8_t render_sprites_left : 1;
			uint8_t render_background : 1;
			uint8_t render_sprites : 1;
			uint8_t enhance_red : 1;
			uint8_t enhance_green : 1;
			uint8_t enhance_blue : 1;
		};

		uint8_t reg;
} MASK;

typedef union 
{
	struct
	{
		uint8_t nametable_x : 1;
		uint8_t nametable_y : 1;
		uint8_t increment_mode : 1;
		uint8_t pattern_sprite : 1;
		uint8_t pattern_background : 1;
		uint8_t sprite_size : 1;
		uint8_t slave_mode : 1; // unused
		uint8_t enable_nmi : 1;
	} BITS;

	uint8_t reg;
} PPUCTRL;

union loopyRegister
{
	// Credit to Loopy for working this out :D
	struct
	{

		uint16_t coarse_x : 5;
		uint16_t coarse_y : 5;
		uint16_t nametable_x : 1;
		uint16_t nametable_y : 1;
		uint16_t fine_y : 3;
		uint16_t unused : 1;
	};

	uint16_t reg = 0x0000;
};

typedef struct{
	Cartridge *cart;
    uint8_t     tblName[2][1024];
	uint8_t     tblPattern[2][4096];
	uint8_t		tblPalette[32];
	Pixel  palScreen[0x40];

	Sprite* sprScreen;
	Sprite* sprNameTable[2];
	Sprite* sprPatternTable[2];

	loopyRegister vramAddr; 
	loopyRegister tramAddr; 

	// Pixel offset horizontally
	uint8_t fineX = 0x00;

	// Internal communications
	uint8_t addressLatch = 0x00;
	uint8_t ppuDataBuffer = 0x00;

	// Pixel "dot" position information
	int16_t scanline = 0;
	int16_t cycle = 0;

	// Background rendering
	uint8_t bgNextTile_id     = 0x00;
	uint8_t bgNextTileAttrib = 0x00;
	uint8_t bgNextTileLsb    = 0x00;
	uint8_t bgNextTileMsb    = 0x00;
	uint16_t bgShifterPatternLo = 0x0000;
	uint16_t bgShifterPatternHi = 0x0000;
	uint16_t bgShifterAttribLo  = 0x0000;
	uint16_t bgShifterAttribHi  = 0x0000;

} PPU;
int CartridgeCpuRead(Cartridge* cartridge, uint16_t addr, int rdonly = 0);
int	CartridgeCpuWrite(Cartridge* cartridge, uint16_t addr, uint8_t  data);

// Communications with PPU Bus
int PpuRead( uint16_t addr, int rdonly = 0);
int PpuWrite( uint16_t addr, uint8_t data);

void ConnectCartridge(Cartridge* cartridge);
void clock();
void reset();

int nmi = 0;

typedef struct
{
	uint32_t n = (0xFF << 24);
	uint8_t r; uint8_t g; uint8_t b; uint8_t a;
} Pixel;

typedef struct
{	
	uint32_t width;
	uint32_t height;
	enum Mode { NORMAL, PERIODIC, CLAMP };
	enum Flip { NONE = 0, HORIZ = 1, VERT = 2 };
	Color *pixels;
} Sprite;

Sprite *SpriteCreate(uint32_t width, uint32_t height);
Color SpriteGetPixel(Sprite *sprite, uint32_t x, uint32_t y);
int SpriteSetPixel(Sprite *sprite, uint32_t x, uint32_t y, Color color);


#endif