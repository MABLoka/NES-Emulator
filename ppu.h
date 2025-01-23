#ifndef PPU_H
#define PPU_H

#include "Cartridge.h"
#include "raylib.h"

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
		uint8_t nametableX : 1;
		uint8_t nametableY : 1;
		uint8_t incrementMode : 1;
		uint8_t patternSprite : 1;
		uint8_t patternBackground : 1;
		uint8_t spriteSize : 1;
		uint8_t slaveMode : 1; // unused
		uint8_t enable_nmi : 1;
		};

	uint8_t reg;
} PPUCTRL;

typedef union 
{
	
	struct 
		{
		uint16_t coarseX : 5;
		uint16_t coarseY : 5;
		uint16_t nametableX : 1;
		uint16_t nametableY : 1;
		uint16_t fineY : 3;
		uint16_t unused : 1;
	};

	uint16_t reg;
}loopyRegister;


typedef struct
{	
	uint32_t width;
	uint32_t height;
	Color *pixels;
} Sprite;


typedef struct{
	Cartridge *cart;
    uint8_t     tblName[2][1024];
	uint8_t     tblPattern[2][4096];
	uint8_t		tblPalette[32];
	Color  palScreen[0x40];

	Sprite* sprScreen;
	Sprite* sprNameTable[2];
	Sprite* sprPatternTable[2];

	loopyRegister vramAddr; 
	loopyRegister tramAddr; 

	
	uint8_t fineX;

	
	uint8_t addressLatch;
	uint8_t ppuDataBuffer;

	
	int16_t scanline;
	int16_t cycle;

	int frame_complete;

	uint8_t bgNextTileId;
	uint8_t bgNextTileAttrib;
	uint8_t bgNextTileLsb;
	uint8_t bgNextTileMsb;
	uint16_t bgShifterPatternLo;
	uint16_t bgShifterPatternHi;
	uint16_t bgShifterAttribLo;
	uint16_t bgShifterAttribHi;
	int nmi;
} PPU;


// Communications with PPU Bus
uint8_t ppuRead( uint16_t addr, int rdonly);
void ppuWrite( uint16_t addr, uint8_t data);

void ConnectCartridgePpu(Cartridge* cartridge);
void ppuClock();
void ppuReset();
PPU* GetPpu();
void PpuInit();
uint8_t ppuCpuRead(uint16_t addr, int rdonly);
void ppuCpuWrite(uint16_t addr, uint8_t data);

Sprite *SpriteCreate(uint32_t width, uint32_t height);
Color SpriteGetPixel(Sprite *sprite, uint32_t x, uint32_t y);
int SpriteSetPixel(Sprite *sprite, uint32_t x, uint32_t y, Color color);
Color GetColorFromPaletteRam(uint8_t palette, uint8_t pixel);
Sprite* GetPatternTable(uint8_t i, uint8_t palette);

void IncrementScrollX();
void IncrementScrollY();
void TransferAddressX();
void TransferAddressY();
void LoadBackgroundShifters();
void UpdateShifters();
#endif //PPU_H