#include "ppu.h"


static PPU ppu = {0};
static STATUS status = {0};
static MASK mask = {0};
static PPUCTRL control = {0};


void PpuInit() {
    ppu.palScreen[0x00] = (Color){84, 84, 84, 0xff};
    ppu.palScreen[0x01] = (Color){0, 30, 116, 0xff};
    ppu.palScreen[0x02] = (Color){8, 16, 144, 0xff};
    ppu.palScreen[0x03] = (Color){48, 0, 136, 0xff};
    ppu.palScreen[0x04] = (Color){68, 0, 100, 0xff};
    ppu.palScreen[0x05] = (Color){92, 0, 48, 0xff};
    ppu.palScreen[0x06] = (Color){84, 4, 0, 0xff};
    ppu.palScreen[0x07] = (Color){60, 24, 0, 0xff};
    ppu.palScreen[0x08] = (Color){32, 42, 0, 0xff};
    ppu.palScreen[0x09] = (Color){8, 58, 0, 0xff};
    ppu.palScreen[0x0A] = (Color){0, 64, 0, 0xff};
    ppu.palScreen[0x0B] = (Color){0, 60, 0, 0xff};
    ppu.palScreen[0x0C] = (Color){0, 50, 60, 0xff};
    ppu.palScreen[0x0D] = (Color){0, 0, 0, 0xff};
    ppu.palScreen[0x0E] = (Color){0, 0, 0, 0xff};
    ppu.palScreen[0x0F] = (Color){0, 0, 0, 0xff};

    ppu.palScreen[0x10] = (Color){152, 150, 152, 0xff};
    ppu.palScreen[0x11] = (Color){8, 76, 196, 0xff};
    ppu.palScreen[0x12] = (Color){48, 50, 236, 0xff};
    ppu.palScreen[0x13] = (Color){92, 30, 228, 0xff};
    ppu.palScreen[0x14] = (Color){136, 20, 176, 0xff};
    ppu.palScreen[0x15] = (Color){160, 20, 100, 0xff};
    ppu.palScreen[0x16] = (Color){152, 34, 32, 0xff};
    ppu.palScreen[0x17] = (Color){120, 60, 0, 0xff};
    ppu.palScreen[0x18] = (Color){84, 90, 0, 0xff};
    ppu.palScreen[0x19] = (Color){40, 114, 0, 0xff};
    ppu.palScreen[0x1A] = (Color){8, 124, 0, 0xff};
    ppu.palScreen[0x1B] = (Color){0, 118, 40, 0xff};
    ppu.palScreen[0x1C] = (Color){0, 102, 120, 0xff};

    ppu.palScreen[0x1D] = (Color){0, 0, 0, 0xff};
    ppu.palScreen[0x1E] = (Color){0, 0, 0, 0xff};
    ppu.palScreen[0x1F] = (Color){0, 0, 0, 0xff};
    ppu.palScreen[0x20] = (Color){236, 238, 236, 0xff};
    ppu.palScreen[0x21] = (Color){76, 154, 236, 0xff};
    ppu.palScreen[0x22] = (Color){120, 124, 236, 0xff};
    ppu.palScreen[0x23] = (Color){176, 98, 236, 0xff};
    ppu.palScreen[0x24] = (Color){228, 84, 236, 0xff};
    ppu.palScreen[0x25] = (Color){236, 88, 180, 0xff};
    ppu.palScreen[0x26] = (Color){236, 106, 100, 0xff};
    ppu.palScreen[0x27] = (Color){212, 136, 32, 0xff};
    ppu.palScreen[0x28] = (Color){160, 170, 0, 0xff};
    ppu.palScreen[0x29] = (Color){116, 196, 0, 0xff};

    ppu.palScreen[0x2A] = (Color){76, 208, 32, 0xff};
    ppu.palScreen[0x2B] = (Color){56, 204, 108, 0xff};
    ppu.palScreen[0x2C] = (Color){56, 180, 204, 0xff};
    ppu.palScreen[0x2D] = (Color){60, 60, 60, 0xff};
    ppu.palScreen[0x2E] = (Color){0, 0, 0, 0xff};
    ppu.palScreen[0x2F] = (Color){0, 0, 0, 0xff};
    ppu.palScreen[0x30] = (Color){236, 238, 236, 0xff};
    ppu.palScreen[0x31] = (Color){168, 204, 236, 0xff};
    ppu.palScreen[0x32] = (Color){188, 188, 236, 0xff};
    ppu.palScreen[0x33] = (Color){212, 178, 236, 0xff};
    ppu.palScreen[0x34] = (Color){236, 174, 236, 0xff};

    ppu.palScreen[0x35] = (Color){236, 174, 212, 0xff};
    ppu.palScreen[0x36] = (Color){236, 180, 176, 0xff};
    ppu.palScreen[0x37] = (Color){228, 196, 144, 0xff};
    ppu.palScreen[0x38] = (Color){204, 210, 120, 0xff};
    ppu.palScreen[0x39] = (Color){180, 222, 120, 0xff};
    ppu.palScreen[0x3A] = (Color){168, 226, 144, 0xff};
    ppu.palScreen[0x3B] = (Color){152, 226, 180, 0xff};
    ppu.palScreen[0x3C] = (Color){160, 214, 228, 0xff};
    ppu.palScreen[0x3D] = (Color){160, 162, 160, 0xff};
    ppu.palScreen[0x3E] = (Color){0, 0, 0, 0xff};
    ppu.palScreen[0x3F] = (Color){0, 0, 0, 0xff};

    ppu.sprScreen = SpriteCreate(256, 240);
    ppu.sprNameTable[0] = SpriteCreate(256, 240);
    ppu.sprNameTable[1] = SpriteCreate(256, 240);
    ppu.sprPatternTable[0] = SpriteCreate(128, 128);
    ppu.sprPatternTable[1] = SpriteCreate(128, 128);

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
	ppu.bgNextTileId     = 0x00;
	ppu.bgNextTileAttrib = 0x00;
	ppu.bgNextTileLsb    = 0x00;
	ppu.bgNextTileMsb    = 0x00;
	ppu.bgShifterPatternLo = 0x0000;
	ppu.bgShifterPatternHi = 0x0000;
	ppu.bgShifterAttribLo  = 0x0000;
	ppu.bgShifterAttribHi  = 0x0000;

	ppu.pOAM = (uint8_t*)malloc(sizeof(sObjectAttributeEntry)*64);
}

Sprite *SpriteCreate(uint32_t width, uint32_t height) {
    Sprite *sprite = (Sprite*)malloc(sizeof(Sprite));
    sprite->width = width;
    sprite->height = height;
    sprite->pixels = (Color*)malloc(width*height*sizeof(Color));
    for (uint32_t j = 0; j < height; j++) {
        for (uint32_t i = 0; i < width; i++) {
            SpriteSetPixel(sprite, i, j, BLACK);
        }
    }
    return sprite;
}

Color SpriteGetPixel(Sprite *sprite, uint32_t x, uint32_t y) {
	
    return sprite->pixels[x*sprite->height + y];
}

int SpriteSetPixel(Sprite *sprite, uint32_t x, uint32_t y, Color pixel) {
	//printf("SpriteSetPixelS\n");
    if ((x >= 0) && (x < sprite->width) && (y >= 0) && (y < sprite->height)) {
        sprite->pixels[x*sprite->height + y] = pixel;
        return 1;
    }
    return 0;
}

Sprite* GetPatternTable(uint8_t i, uint8_t palette)
{
	//printf("GetPatternTableS\n");
	for (uint16_t nTileY = 0; nTileY < 16; nTileY++)
	{
		//printf("nTileY\n");
		for (uint16_t nTileX = 0; nTileX < 16; nTileX++)
		{
			//printf("nTileX\n");
			uint16_t nOffset = nTileY * 256 + nTileX * 16;

			
			for (uint16_t row = 0; row < 8; row++)
			{
				//printf("row\n");
				uint8_t tile_lsb = ppuRead(i * 0x1000 + nOffset + row + 0x0000, 0);
				uint8_t tile_msb = ppuRead(i * 0x1000 + nOffset + row + 0x0008, 0);


				
				for (uint16_t col = 0; col < 8; col++)
				{
					uint8_t pixel = (tile_lsb & 0x01) + (tile_msb & 0x01);

					tile_lsb >>= 1; tile_msb >>= 1;

					SpriteSetPixel(ppu.sprPatternTable[i],
					nTileX * 8 + (7 - col),
					nTileY * 8 + row,
					 GetColorFromPaletteRam(palette, pixel)); 
				}
			}
		}
	}

	return ppu.sprPatternTable[i];
}

Color GetColorFromPaletteRam(uint8_t palette, uint8_t pixel)
{
	return ppu.palScreen[ppuRead(0x3F00 + (palette << 2) + pixel, 0) & 0x3F];
}

void ConnectCartridgePpu(Cartridge* cartridge)
{
	ppu.cart = cartridge;
}

void ppuReset()
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
	ppu.bgNextTileId     = 0x00;
	ppu.bgNextTileAttrib = 0x00;
	ppu.bgNextTileLsb    = 0x00;
	ppu.bgNextTileMsb    = 0x00;
	ppu.bgShifterPatternLo = 0x0000;
	ppu.bgShifterPatternHi = 0x0000;
	ppu.bgShifterAttribLo  = 0x0000;
	ppu.bgShifterAttribHi  = 0x0000;
}
PPU* GetPpu(){
	return &ppu;
}
uint8_t ppuCpuRead(uint16_t addr, int rdonly)
{
	uint8_t data = 0x00;
	addr &= 0x0007;
	if (rdonly)
	{

		switch (addr)
		{
		case 0x0000: // Control
			data = control.reg;
			break;
		case 0x0001: // Mask
			data = mask.reg;
			break;
		case 0x0002: // Status
			data = status.reg;
			break;
		case 0x0003: // OAM Address
			break;
		case 0x0004: // OAM Data
			break;
		case 0x0005: // Scroll
			break;
		case 0x0006: // PPU Address
			break;
		case 0x0007: // PPU Data
			break;
		}
	}
	else
	{
		
		switch (addr)
		{
			
			case 0x0000: break;
			
				
			case 0x0001: break;
			
				
			case 0x0002:
				// printf("case 2\n");
				// printf("ppuDataBuffer: %x\n", ppu.ppuDataBuffer);
				// printf("status.reg: %x\n", status.reg);
				
				data = (status.reg & 0xE0) | (ppu.ppuDataBuffer & 0x1F);
				
				
				status.verticalBlank = 0;

				
				ppu.addressLatch = 0;
				break;

				// OAM Address
			case 0x0003: break;

				// OAM Data
			case 0x0004: break;

				// Scroll - Not Readable
			case 0x0005: break;

				// PPU Address - Not Readable
			case 0x0006: break;

				// PPU Data
			case 0x0007: 
				// printf("case 7\n");
				data = ppu.ppuDataBuffer;
				// printf("Case  7: %x and %x\n", status.reg , ppu.ppuDataBuffer);
				// printf("Case  7: Vram: %x at Vram: %x\n",ppu.vramAddr.reg ,ppuRead(ppu.vramAddr.reg, 0));
				ppu.ppuDataBuffer = ppuRead(ppu.vramAddr.reg, 0);
				
				if (ppu.vramAddr.reg >= 0x3F00) 
					data = ppu.ppuDataBuffer;
				
				ppu.vramAddr.reg += (control.incrementMode ? 32 : 1);
				break;
		}
	}
	
	return data;
}

void ppuCpuWrite(uint16_t addr, uint8_t data)
{
	
	addr &= 0x0007;
	switch (addr)
	{
		case 0x0000: // Control
			control.reg = data;
			ppu.tramAddr.nametableX = control.nametableX;
			ppu.tramAddr.nametableY = control.nametableY;
			break;
		case 0x0001: // Mask
			mask.reg = data;
			break;
		case 0x0002: // Status
			break;
		case 0x0003: // OAM Address
			break;
		case 0x0004: // OAM Data
			break;
		case 0x0005: // Scroll
			if (ppu.addressLatch == 0)
			{
				
				ppu.fineX = data & 0x07;
				ppu.tramAddr.coarseX = data >> 3;
				ppu.addressLatch = 1;
			}
			else
			{
				
				ppu.tramAddr.fineY = data & 0x07;
				ppu.tramAddr.coarseY = data >> 3;
				ppu.addressLatch = 0;
			}
			break;
		case 0x0006: // PPU Address
			if (ppu.addressLatch == 0)
			{
				
				ppu.tramAddr.reg = (uint16_t)((data & 0x3F) << 8) | (ppu.tramAddr.reg & 0x00FF);
				ppu.addressLatch = 1;
			}
			else
			{
				
				ppu.tramAddr.reg = (ppu.tramAddr.reg & 0xFF00) | data;
				ppu.vramAddr = ppu.tramAddr;
				ppu.addressLatch = 0;
			}
			break;
		case 0x0007: // PPU Data
			ppuWrite(ppu.vramAddr.reg, data);
			
			ppu.vramAddr.reg += (control.incrementMode ? 32 : 1);
			break;
	}
	
}

uint8_t ppuRead(uint16_t addr, int rdonly)
{
	//printf("ppuReadS\n");
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
	//printf("ppuReadE\n");
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

void ppuClock()
{
	

	if (ppu.scanline >= -1 && ppu.scanline < 240)
	{		
		if (ppu.scanline == 0 && ppu.cycle == 0)
		{
			ppu.cycle = 1;
		}

		if (ppu.scanline == -1 && ppu.cycle == 1)
		{

			status.verticalBlank = 0;
		}


		if ((ppu.cycle >= 2 && ppu.cycle < 258) || (ppu.cycle >= 321 && ppu.cycle < 338))
		{
			UpdateShifters();
			
			

			switch ((ppu.cycle - 1) % 8)
			{
			case 0:

				LoadBackgroundShifters();

				ppu.bgNextTileId = ppuRead(0x2000 | (ppu.vramAddr.reg & 0x0FFF), 0);

		
				break;
			case 2:
								
				ppu.bgNextTileAttrib = ppuRead(0x23C0 | (ppu.vramAddr.nametableY << 11) 
					                                 | (ppu.vramAddr.nametableX << 10) 
					                                 | ((ppu.vramAddr.coarseY >> 2) << 3) 
					                                 | (ppu.vramAddr.coarseX >> 2), 0);
				
		
				if (ppu.vramAddr.coarseY & 0x02) ppu.bgNextTileAttrib >>= 4;
				if (ppu.vramAddr.coarseX & 0x02) ppu.bgNextTileAttrib >>= 2;
				ppu.bgNextTileAttrib &= 0x03;
				break;

			

			case 4: 
	
				ppu.bgNextTileLsb = ppuRead((control.patternBackground << 12) 
					                       + ((uint16_t)ppu.bgNextTileId << 4) 
					                       + (ppu.vramAddr.fineY) + 0, 0);

				break;
			case 6:
		
				ppu.bgNextTileMsb = ppuRead((control.patternBackground << 12)
					                       + ((uint16_t)ppu.bgNextTileId << 4)
					                       + (ppu.vramAddr.fineY) + 8, 0);
				break;
			case 7:
			
				IncrementScrollX();
				break;
			}
		}

		if (ppu.cycle == 256)
		{
			IncrementScrollY();
		}

		if (ppu.cycle == 257)
		{
			LoadBackgroundShifters();
			TransferAddressX();
		}

		if (ppu.cycle == 338 || ppu.cycle == 340)
		{
			ppu.bgNextTileId = ppuRead(0x2000 | (ppu.vramAddr.reg & 0x0FFF), 0);
		}

		if (ppu.scanline == -1 && ppu.cycle >= 280 && ppu.cycle < 305)
		{
			TransferAddressY();
		}
		if (ppu.cycle == 257 && ppu.scanline >= 0)
		{
			memset(ppu.spriteScanline, 0xFF, 8 * sizeof(sObjectAttributeEntry));

			
			ppu.spriteCount = 0;

			for (uint8_t i = 0; i < 8; i++)
			{
				ppu.spriteShifterPatternLo[i] = 0;
				ppu.spriteShifterPatternHi[i] = 0;
			}

			
			uint8_t nOAMEntry = 0;

			
			ppu.bSpriteZeroHitPossible = 0;

			while (nOAMEntry < 64 && ppu.spriteCount < 9)
			{
				// Note the conversion to signed numbers here
				int16_t diff = ((int16_t)ppu.scanline - (int16_t)ppu.OAM[nOAMEntry].y);

				
				if (diff >= 0 && diff < (control.spriteSize ? 16 : 8))
				{
					
					if (ppu.spriteCount < 8)
					{
						// Is this sprite sprite zero?
						if (nOAMEntry == 0)
						{
							
							ppu.bSpriteZeroHitPossible = 1;
						}

						memcpy(&ppu.spriteScanline[ppu.spriteCount], &ppu.OAM[nOAMEntry], sizeof(sObjectAttributeEntry));
						ppu.spriteCount++;
					}				
				}

				nOAMEntry++;
			} 

			// Set sprite overflow flag
			status.spriteOverflow = (ppu.spriteCount > 8);

			
		}

		if (ppu.cycle == 340)
				{

					for (uint8_t i = 0; i < ppu.spriteCount; i++)
					{
						

						uint8_t sprite_pattern_bits_lo, sprite_pattern_bits_hi;
						uint16_t sprite_pattern_addr_lo, sprite_pattern_addr_hi;


						if (!control.spriteSize)
						{
							// 8x8 Sprite Mode - The control register determines the pattern table
							if (!(ppu.spriteScanline[i].attribute & 0x80))
							{
								// Sprite is NOT flipped vertically, i.e. normal    
								sprite_pattern_addr_lo = 
								(control.patternSprite << 12  )
								| (ppu.spriteScanline[i].id   << 4   )  
								| (ppu.scanline - ppu.spriteScanline[i].y); 
														
							}
							else
							{
								// Sprite is flipped vertically, i.e. upside down
								sprite_pattern_addr_lo = 
								(control.patternSprite << 12  )  
								| (ppu.spriteScanline[i].id   << 4   ) 
								| (7 - (ppu.scanline - ppu.spriteScanline[i].y)); 
							}

						}
						else
						{
							// 8x16 Sprite Mode - The sprite attribute determines the pattern table
							if (!(ppu.spriteScanline[i].attribute & 0x80))
							{
								// Sprite is NOT flipped vertically, i.e. normal
								if (ppu.scanline - ppu.spriteScanline[i].y < 8)
								{
									// Reading Top half Tile
									sprite_pattern_addr_lo = 
									((ppu.spriteScanline[i].id & 0x01)      << 12) 
									| ((ppu.spriteScanline[i].id & 0xFE)      << 4 )  
									| ((ppu.scanline - ppu.spriteScanline[i].y) & 0x07 ); 
								}
								else
								{
									// Reading Bottom Half Tile
									sprite_pattern_addr_lo = 
									( (ppu.spriteScanline[i].id & 0x01)      << 12)  
									| (((ppu.spriteScanline[i].id & 0xFE) + 1) << 4 )  
									| ((ppu.scanline - ppu.spriteScanline[i].y) & 0x07  ); 
								}
							}
							else
							{
								// Sprite is flipped vertically, i.e. upside down
								if (ppu.scanline - ppu.spriteScanline[i].y < 8)
								{
									// Reading Top half Tile
									sprite_pattern_addr_lo = 
									( (ppu.spriteScanline[i].id & 0x01)      << 12)    
									| (((ppu.spriteScanline[i].id & 0xFE) + 1) << 4 )    
									| (7 - (ppu.scanline - ppu.spriteScanline[i].y) & 0x07); 
								}
								else
								{
									// Reading Bottom Half Tile
									sprite_pattern_addr_lo = 
									((ppu.spriteScanline[i].id & 0x01)       << 12)    
									| ((ppu.spriteScanline[i].id & 0xFE)       << 4 )    
									| (7 - (ppu.scanline - ppu.spriteScanline[i].y) & 0x07);
								}
							}
						}

						
						sprite_pattern_addr_hi = sprite_pattern_addr_lo + 8;

						
						sprite_pattern_bits_lo = ppuRead(sprite_pattern_addr_lo, 0);
						sprite_pattern_bits_hi = ppuRead(sprite_pattern_addr_hi, 0);

						if (ppu.spriteScanline[i].attribute & 0x40)
						{
							sprite_pattern_bits_lo = flipbyte(sprite_pattern_bits_lo);
							sprite_pattern_bits_hi = flipbyte(sprite_pattern_bits_hi);
						}

						
						ppu.spriteShifterPatternLo[i] = sprite_pattern_bits_lo;
						ppu.spriteShifterPatternHi[i] = sprite_pattern_bits_hi;
					}
				}
	
	}

	

	if(ppu.scanline == 240)
	{

	}

	if (ppu.scanline >= 241 && ppu.scanline < 261)
	{
		if (ppu.scanline == 241 && ppu.cycle == 1)
		{
			status.verticalBlank = 1;

			if (control.enable_nmi) 
				ppu.nmi = 1;
		}
	}

	uint8_t bg_pixel = 0x00;  
	uint8_t bg_palette = 0x00; 

	
	if(mask.reg & 0b00001000)
	{
		
		uint16_t bit_mux = 0x8000 >> ppu.fineX;
 
		uint8_t p0_pixel = (ppu.bgShifterPatternLo & bit_mux) > 0;
		uint8_t p1_pixel = (ppu.bgShifterPatternHi & bit_mux) > 0;

		
		bg_pixel = (p1_pixel << 1) | p0_pixel;

		
		uint8_t bg_pal0 = (ppu.bgShifterAttribLo & bit_mux) > 0;
		uint8_t bg_pal1 = (ppu.bgShifterAttribHi & bit_mux) > 0;
		bg_palette = (bg_pal1 << 1) | bg_pal0;
	}

	uint8_t fg_pixel = 0x00;   
	uint8_t fg_palette = 0x00; 
	uint8_t fg_priority = 0x00;
							   
	if (mask.reg & 0b00001000)
	{
	
		ppu.bSpriteZeroBeingRendered = 0;

		for (uint8_t i = 0; i < ppu.spriteCount; i++)
		{
			
			if (ppu.spriteScanline[i].x == 0) 
			{
				
				uint8_t fg_pixel_lo = (ppu.spriteShifterPatternLo[i] & 0x80) > 0;
				uint8_t fg_pixel_hi = (ppu.spriteShifterPatternHi[i] & 0x80) > 0;
				fg_pixel = (fg_pixel_hi << 1) | fg_pixel_lo;

				fg_palette = (ppu.spriteScanline[i].attribute & 0x03) + 0x04;
				fg_priority = (ppu.spriteScanline[i].attribute & 0x20) == 0;

				
				if (fg_pixel != 0)
				{
					if (i == 0) 
					{
						ppu.bSpriteZeroBeingRendered = 1;
					}

					break;
				}				
			}
		}		
	}

	uint8_t pixel = 0x00;  
	uint8_t palette = 0x00; 

	if (bg_pixel == 0 && fg_pixel == 0)
	{
		
		pixel = 0x00;
		palette = 0x00;
	}
	else if (bg_pixel == 0 && fg_pixel > 0)
	{
		
		pixel = fg_pixel;
		palette = fg_palette;
	}
	else if (bg_pixel > 0 && fg_pixel == 0)
	{
		
		pixel = bg_pixel;
		palette = bg_palette;
	}
	else if (bg_pixel > 0 && fg_pixel > 0)
	{
		
		if (fg_priority)
		{
		
			pixel = fg_pixel;
			palette = fg_palette;
		}
		else
		{
			
			pixel = bg_pixel;
			palette = bg_palette;
		}

		// Sprite Zero Hit detection
		if (ppu.bSpriteZeroHitPossible && ppu.bSpriteZeroBeingRendered)
		{
			
			if ((mask.reg & 0b00010000) & (mask.reg & 0b00001000))
			{
				
				if (~((mask.reg & 0b01000000) | (mask.reg & 0b00100000)))
				{
					if (ppu.cycle >= 9 && ppu.cycle < 258)
					{
						status.spriteZeroHit = 1;
					}
				}
				else
				{
					if (ppu.cycle >= 1 && ppu.cycle < 258)
					{
						status.spriteZeroHit = 1;
					}
				}
			}
		}
	}


	SpriteSetPixel(ppu.sprScreen, ppu.cycle - 1, ppu.scanline, GetColorFromPaletteRam(palette, pixel));

	
	ppu.cycle++;

	if (ppu.cycle >= 341)
	{
		ppu.cycle = 0;
		ppu.scanline++;
		if (ppu.scanline >= 261)
		{
			ppu.scanline = -1;
			ppu.frame_complete = 1;
		}
	}
}

void IncrementScrollX()
{
	

	if (mask.render_background || mask.render_sprites)
	{
		
		if (ppu.vramAddr.coarseX == 31)
		{
			
			ppu.vramAddr.coarseX = 0;
			
			ppu.vramAddr.nametableX = ~ppu.vramAddr.nametableX;
		}
		else
		{
			
			ppu.vramAddr.coarseX++;
		}
	}
}


void IncrementScrollY()
{

	if (mask.render_background || mask.render_sprites)
	{

		if (ppu.vramAddr.fineY < 7)
		{
			ppu.vramAddr.fineY++;
		}
		else
		{

			ppu.vramAddr.fineY = 0;

			if (ppu.vramAddr.coarseY == 29)
			{

				ppu.vramAddr.coarseY = 0;

				ppu.vramAddr.nametableY = ~ppu.vramAddr.nametableY;
			}
			else if (ppu.vramAddr.coarseY == 31)
			{

				ppu.vramAddr.coarseY = 0;
			}
			else
			{

				ppu.vramAddr.coarseY++;
			}
		}
	}
}


void TransferAddressX()
{

	if (mask.render_background || mask.render_sprites)
	{
		ppu.vramAddr.nametableX = ppu.tramAddr.nametableX;
		ppu.vramAddr.coarseX    = ppu.tramAddr.coarseX;
	}
}

void TransferAddressY()
{

	if (mask.render_background || mask.render_sprites)
	{
		ppu.vramAddr.fineY      = ppu.tramAddr.fineY;
		ppu.vramAddr.nametableY = ppu.tramAddr.nametableY;
		ppu.vramAddr.coarseY    = ppu.tramAddr.coarseY;
	}
}


void LoadBackgroundShifters()
{	

	ppu.bgShifterPatternLo = (ppu.bgShifterPatternLo & 0xFF00) | ppu.bgNextTileLsb;
	ppu.bgShifterPatternHi = (ppu.bgShifterPatternHi & 0xFF00) | ppu.bgNextTileMsb;

	ppu.bgShifterAttribLo  = (ppu.bgShifterAttribLo & 0xFF00) | ((ppu.bgNextTileAttrib & 0b01) ? 0xFF : 0x00);
	ppu.bgShifterAttribHi  = (ppu.bgShifterAttribHi & 0xFF00) | ((ppu.bgNextTileAttrib & 0b10) ? 0xFF : 0x00);
}


void UpdateShifters()
{
	if (mask.render_background)
	{

		ppu.bgShifterPatternLo <<= 1;
		ppu.bgShifterPatternHi <<= 1;


		ppu.bgShifterAttribLo <<= 1;
		ppu.bgShifterAttribHi <<= 1;
	}
}


uint8_t flipbyte(uint8_t b)
{
	b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
	b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
	b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
	return b;
}