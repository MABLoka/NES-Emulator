#include "ppu.h"
uint32_t defN = (0xFF << 24);

static PPU ppu = {0};
static STATUS status = {0};
static MASK mask = {0};
static PPUCTRL control = {0};

void PpuInit() {
    ppu.palScreen[0x00] = (Pixel){defN, 84, 84, 84, 0xff};
    ppu.palScreen[0x00] = (Pixel){ defN, 84, 84, 84, 0xff};
    ppu.palScreen[0x01] = (Pixel){ defN, 0, 30, 116, 0xff};
    ppu.palScreen[0x02] = (Pixel){ defN, 8, 16, 144, 0xff};
    ppu.palScreen[0x03] = (Pixel){ defN, 48, 0, 136, 0xff};
    ppu.palScreen[0x04] = (Pixel){ defN, 68, 0, 100, 0xff};
    ppu.palScreen[0x05] = (Pixel){ defN, 92, 0, 48, 0xff};
    ppu.palScreen[0x06] = (Pixel){ defN, 84, 4, 0, 0xff};
    ppu.palScreen[0x07] = (Pixel){ defN, 60, 24, 0, 0xff};
    ppu.palScreen[0x08] = (Pixel){ defN, 32, 42, 0, 0xff};
    ppu.palScreen[0x09] = (Pixel){ defN, 8, 58, 0, 0xff};
    ppu.palScreen[0x0A] = (Pixel){ defN, 0, 64, 0, 0xff};
    ppu.palScreen[0x0B] = (Pixel){ defN, 0, 60, 0, 0xff};
    ppu.palScreen[0x0C] = (Pixel){ defN, 0, 50, 60, 0xff};
    ppu.palScreen[0x0D] = (Pixel){ defN, 0, 0, 0, 0xff};
    ppu.palScreen[0x0E] = (Pixel){ defN, 0, 0, 0, 0xff};
    ppu.palScreen[0x0F] = (Pixel){ defN, 0, 0, 0, 0xff};

    ppu.palScreen[0x10] = (Pixel){ defN, 152, 150, 152, 0xff};
    ppu.palScreen[0x11] = (Pixel){ defN, 8, 76, 196, 0xff};
    ppu.palScreen[0x12] = (Pixel){ defN, 48, 50, 236, 0xff};
    ppu.palScreen[0x13] = (Pixel){ defN, 92, 30, 228, 0xff};
    ppu.palScreen[0x14] = (Pixel){ defN, 136, 20, 176, 0xff};
    ppu.palScreen[0x15] = (Pixel){ defN, 160, 20, 100, 0xff};
    ppu.palScreen[0x16] = (Pixel){ defN, 152, 34, 32, 0xff};
    ppu.palScreen[0x17] = (Pixel){ defN, 120, 60, 0, 0xff};
    ppu.palScreen[0x18] = (Pixel){ defN, 84, 90, 0, 0xff};
    ppu.palScreen[0x19] = (Pixel){ defN, 40, 114, 0, 0xff};
    ppu.palScreen[0x1A] = (Pixel){ defN, 8, 124, 0, 0xff};
    ppu.palScreen[0x1B] = (Pixel){ defN, 0, 118, 40, 0xff};
    ppu.palScreen[0x1C] = (Pixel){ defN, 0, 102, 120, 0xff};

    ppu.palScreen[0x1D] = (Pixel){ defN, 0, 0, 0, 0xff};
    ppu.palScreen[0x1E] = (Pixel){ defN, 0, 0, 0, 0xff};
    ppu.palScreen[0x1F] = (Pixel){ defN, 0, 0, 0, 0xff};
    ppu.palScreen[0x20] = (Pixel){ defN, 236, 238, 236, 0xff};
    ppu.palScreen[0x21] = (Pixel){ defN, 76, 154, 236, 0xff};
    ppu.palScreen[0x22] = (Pixel){ defN, 120, 124, 236, 0xff};
    ppu.palScreen[0x23] = (Pixel){ defN, 176, 98, 236, 0xff};
    ppu.palScreen[0x24] = (Pixel){ defN, 228, 84, 236, 0xff};
    ppu.palScreen[0x25] = (Pixel){ defN, 236, 88, 180, 0xff};
    ppu.palScreen[0x26] = (Pixel){ defN, 236, 106, 100, 0xff};
    ppu.palScreen[0x27] = (Pixel){ defN, 212, 136, 32, 0xff};
    ppu.palScreen[0x28] = (Pixel){ defN, 160, 170, 0, 0xff};
    ppu.palScreen[0x29] = (Pixel){ defN, 116, 196, 0, 0xff};

    ppu.palScreen[0x2A] = (Pixel){ defN, 76, 208, 32, 0xff};
    ppu.palScreen[0x2B] = (Pixel){ defN, 56, 204, 108, 0xff};
    ppu.palScreen[0x2C] = (Pixel){ defN, 56, 180, 204, 0xff};
    ppu.palScreen[0x2D] = (Pixel){ defN, 60, 60, 60, 0xff};
    ppu.palScreen[0x2E] = (Pixel){ defN, 0, 0, 0, 0xff};
    ppu.palScreen[0x2F] = (Pixel){ defN, 0, 0, 0, 0xff};
    ppu.palScreen[0x30] = (Pixel){ defN, 236, 238, 236, 0xff};
    ppu.palScreen[0x31] = (Pixel){ defN, 168, 204, 236, 0xff};
    ppu.palScreen[0x32] = (Pixel){ defN, 188, 188, 236, 0xff};
    ppu.palScreen[0x33] = (Pixel){ defN, 212, 178, 236, 0xff};
    ppu.palScreen[0x34] = (Pixel){ defN, 236, 174, 236, 0xff};

    ppu.palScreen[0x35] = (Pixel){ defN, 236, 174, 212, 0xff};
    ppu.palScreen[0x36] = (Pixel){ defN, 236, 180, 176, 0xff};
    ppu.palScreen[0x37] = (Pixel){ defN, 228, 196, 144, 0xff};
    ppu.palScreen[0x38] = (Pixel){ defN, 204, 210, 120, 0xff};
    ppu.palScreen[0x39] = (Pixel){ defN, 180, 222, 120, 0xff};
    ppu.palScreen[0x3A] = (Pixel){ defN, 168, 226, 144, 0xff};
    ppu.palScreen[0x3B] = (Pixel){ defN, 152, 226, 180, 0xff};
    ppu.palScreen[0x3C] = (Pixel){ defN, 160, 214, 228, 0xff};
    ppu.palScreen[0x3D] = (Pixel){ defN, 160, 162, 160, 0xff};
    ppu.palScreen[0x3E] = (Pixel){ defN, 0, 0, 0, 0xff};
    ppu.palScreen[0x3F] = (Pixel){defN, .0, 0, 0, 0xff};

    ppu.sprScreen = SpriteCreate(256, 240);
    ppu.sprNameTable[0] = SpriteCreate(256, 240);
    ppu.sprNameTable[1] = SpriteCreate(256, 240);
    ppu.sprPatternTable[0] = SpriteCreate(128, 128);
    ppu.sprPatternTable[1] = SpriteCreate(128, 128);
}

Sprite *SpriteCreate(uint16_t width, uint16_t height) {
    Sprite *sprite = (Sprite*)malloc(sizeof(sprite));
    sprite->width = width;
    sprite->height = height;
    sprite->pixels = (Color*)malloc(width*height*sizeof(Color));
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            SpriteSetPixel(sprite, i, j, BLACK);
        }
    }
    return sprite;
}

Color SpriteGetPixel(Sprite *sprite, uint16_t x, uint16_t y) {
    return sprite->pixels[x*sprite->height + y];
}

bool SpriteSetPixel(Sprite *sprite, uint16_t x, uint16_t y, Color color) {
    if (x >= 0 && x < sprite->width && y >= 0 && y < sprite->height) {
        sprite->pixels[x*sprite->height + y] = color;
        return true;
    }
    return false;
}

Sprite* GetPatternTable(uint8_t i, uint8_t palette)
{
	
	for (uint16_t nTileY = 0; nTileY < 16; nTileY++)
	{
		for (uint16_t nTileX = 0; nTileX < 16; nTileX++)
		{
			
			uint16_t nOffset = nTileY * 256 + nTileX * 16;

			
			for (uint16_t row = 0; row < 8; row++)
			{
				
				uint8_t tile_lsb = ppuRead(i * 0x1000 + nOffset + row + 0x0000, 0);
				uint8_t tile_msb = ppuRead(i * 0x1000 + nOffset + row + 0x0008, 0);


				
				for (uint16_t col = 0; col < 8; col++)
				{
					uint8_t pixel = (tile_lsb & 0x01) + (tile_msb & 0x01);

					tile_lsb >>= 1; tile_msb >>= 1;

					ppu.sprPatternTable[i]->SetPixel
					(
						nTileX * 8 + (7 - col), 

						nTileY * 8 + row, 
						GetColourFromPaletteRam(palette, pixel)
					);
				}
			}
		}
	}

	return *ppu.sprPatternTable[i];
}

uint8_t ppuRead(uint16_t addr, int rdonly){
    uint8_t data = 0x00;
	addr &= 0x3FFF;

	if (CartridgePpuRead(ppu.cart, addr, &data))
	{

	}
	else if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		// If the cartridge cant map the address, have
		// a physical location ready here
		data = ppu.tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF];
	}
	else if (addr >= 0x2000 && addr <= 0x3EFF)
	{
		addr &= 0x0FFF;

		if (ppu.cart->mirror == VERTICAL)
		{
			// Vertical
			if (addr >= 0x0000 && addr <= 0x03FF)
				data = ppu.tblName[0][addr & 0x03FF];
			if (addr >= 0x0400 && addr <= 0x07FF)
				data = ppu.tblName[1][addr & 0x03FF];
			if (addr >= 0x0800 && addr <= 0x0BFF)
				data = ppu.tblName[0][addr & 0x03FF];
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				data = ppu.tblName[1][addr & 0x03FF];
		}
		else if (ppu.cart->mirror == HORIZONTAL)
		{
			// Horizontal
			if (addr >= 0x0000 && addr <= 0x03FF)
				data = ppu.tblName[0][addr & 0x03FF];
			if (addr >= 0x0400 && addr <= 0x07FF)
				data = ppu.tblName[0][addr & 0x03FF];
			if (addr >= 0x0800 && addr <= 0x0BFF)
				data = ppu.tblName[1][addr & 0x03FF];
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				data = ppu.tblName[1][addr & 0x03FF];
		}
	}
	else if (addr >= 0x3F00 && addr <= 0x3FFF)
	{
		addr &= 0x001F;
		if (addr == 0x0010) addr = 0x0000;
		if (addr == 0x0014) addr = 0x0004;
		if (addr == 0x0018) addr = 0x0008;
		if (addr == 0x001C) addr = 0x000C;
		data = ppu.tblPalette[addr] & (mask.grayscale ? 0x30 : 0x3F);
    }

	return data;
}

void ppuWrite(uint16_t addr, uint8_t data)
{
	addr &= 0x3FFF;

	if (CartridgePpuWrite(ppu.cart, addr, data))
	{

	}
	else if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		ppu.tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;
	}
	else if (addr >= 0x2000 && addr <= 0x3EFF)
	{
		addr &= 0x0FFF;
		if (ppu.cart->mirror == VERTICAL)
		{
			// Vertical
			if (addr >= 0x0000 && addr <= 0x03FF)
				ppu.tblName[0][addr & 0x03FF] = data;
			if (addr >= 0x0400 && addr <= 0x07FF)
				ppu.tblName[1][addr & 0x03FF] = data;
			if (addr >= 0x0800 && addr <= 0x0BFF)
				ppu.tblName[0][addr & 0x03FF] = data;
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				ppu.tblName[1][addr & 0x03FF] = data;
		}
		else if (ppu.cart->mirror == HORIZONTAL)
		{
			// Horizontal
			if (addr >= 0x0000 && addr <= 0x03FF)
				ppu.tblName[0][addr & 0x03FF] = data;
			if (addr >= 0x0400 && addr <= 0x07FF)
				ppu.tblName[0][addr & 0x03FF] = data;
			if (addr >= 0x0800 && addr <= 0x0BFF)
				ppu.tblName[1][addr & 0x03FF] = data;
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				ppu.tblName[1][addr & 0x03FF] = data;
		}
	}
	else if (addr >= 0x3F00 && addr <= 0x3FFF)
	{
		addr &= 0x001F;
		if (addr == 0x0010) addr = 0x0000;
		if (addr == 0x0014) addr = 0x0004;
		if (addr == 0x0018) addr = 0x0008;
		if (addr == 0x001C) addr = 0x000C;
		ppu.tblPalette[addr] = data;
	}
}

void ConnectCartridge(Cartridge* cartridge)
{
	ppu.cart = cartridge;
}

void reset()
{
	status.reg = 0x00;
	mask.reg = 0x00;
	control.reg = 0x00;
	ppu.vramAddr.reg = 0x0000;
	ppu.tramAddr.reg = 0x0000;
    
    ppu.fineX = 0x00;
	ppu.addressLatch = 0x00;
	ppu.ppuDataBuffer = 0x00;
	ppu.scanline = 0;
	ppu.cycle = 0;
	ppu.bgNextTile_id     = 0x00;
	ppu.bgNextTileAttrib = 0x00;
	ppu.bgNextTileLsb    = 0x00;
	ppu.bgNextTileMsb    = 0x00;
	ppu.bgShifterPatternLo = 0x0000;
	ppu.bgShifterPatternHi = 0x0000;
	ppu.bgShifterAttribLo  = 0x0000;
	ppu.bgShifterAttribHi  = 0x0000;
}