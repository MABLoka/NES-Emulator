#include "Cartridge.h"
#include <stdint.h>
#include <stdio.h>
#include <assert.h>



typedef struct
{
    char name[4];
    uint8_t prgRomChunks;
    uint8_t chrRomChunks;
    uint8_t mapper1;
    uint8_t mapper2;
    uint8_t prgRamSize;
    uint8_t tvSystem1;
    uint8_t tvSystem2;
    char unused[5];
} NesHeader; 


Cartridge *CartridgeCreate(const char* filePath){
    
    FILE *fd = fopen(filePath, "rb");
    if (fd == NULL) {
        fprintf(stderr, "Error opening '%s' file\n", filePath);
        exit(1);
    }
    
    NesHeader sHeader = {0};
   
    size_t ifile = fread(&sHeader, sizeof(NesHeader), 1, fd);
    Cartridge *cartridge = (Cartridge*)malloc(sizeof(Cartridge));

    cartridge->bImageValid = 0;

    if (!cartridge) { 
        fprintf(stderr, "Could't allocate space for the cartridge\n");
        fclose(fd);
        exit(1);
    }
    
    cartridge->nMapperID = ((sHeader.mapper2 >> 4) << 4) | (sHeader.mapper1 >> 4);
    cartridge->mirror = (sHeader.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;

    uint8_t fileType = 1;

    if (fileType == 1) {
        
        mapper->nPrgBanks = sHeader.prgRomChunks;

        cartridge->vPRGMemory = (uint8_t*)malloc(16384*sHeader.prgRomChunks);
        cartridge->vPRGMemorySize = 0;
        cartridge->vPRGMemoryCapacity = 16384*sHeader.prgRomChunks;
        ifile = fread(cartridge->vPRGMemory, sizeof(uint8_t*), 16384*sHeader.prgRomChunks, fd);
        
        mapper->nChrBanks = sHeader.chrRomChunks;

        cartridge->vCHRMemory = (uint8_t*)malloc(8192*sHeader.chrRomChunks);
        cartridge->vCHRMemorySize = 0;
        cartridge->vCHRMemoryCapacity = 16384*sHeader.prgRomChunks;
        ifile = fread(cartridge->vPRGMemory, sizeof(uint8_t*), 8192*sHeader.prgRomChunks, fd);
        
        
    }
    if (fclose(fd)) { 
        fprintf(stdout, "Error closing the file\n");
        exit(1);
    }
    ;
    switch (cartridge->nMapperID) {
        case 0: cartridge->pMapper = Mapper_Create(mapper->nPrgBanks, mapper->nChrBanks); break;
        default: assert(cartridge->nMapperID && "Mapper ID not implemented"); break;
    }
    return cartridge;
}


int CartridgeCpuRead(Cartridge *cartridge, uint16_t addr, uint8_t *data)
{
	uint32_t mapped_addr = 0;
	if (cartridge->pMapper->cpuMapRead(addr, &mapped_addr))
	{
		*data = cartridge->vPRGMemory[mapped_addr];
		return 1;
	}
	else
		return 0;
}

int CartridgeCpuWrite(Cartridge *cartridge, uint16_t addr, uint8_t data)
{
	uint32_t mapped_addr = 0;
	if (cartridge->pMapper->cpuMapRead(addr, &mapped_addr))
	{
        cartridge->vPRGMemory[mapped_addr] = data;
        return 1;
        
	}
	else
		return 0;
}


int CartridgePpuRead(Cartridge* cartridge, uint16_t addr, uint8_t* data)
{
	
}
int CartridgePpuWrite(Cartridge* cartridge, uint16_t addr, uint8_t data)
{
	
}