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


// Cartridge *CartridgeCreate(const char* filePath){
//     printf("CartridgeCreateSS");
//     FILE *fd = fopen(filePath, "rb");
//     if (fd == NULL) {
//         fprintf(stderr, "Error opening '%s' file\n", filePath);
//         exit(1);
//     }
    
//     NesHeader sHeader = {0};
   
//     size_t ifile = fread(&sHeader, sizeof(NesHeader), 1, fd);
//     Cartridge *cartridge = (Cartridge*)malloc(sizeof(Cartridge));

//     cartridge->bImageValid = 0;

//     if (!cartridge) { 
//         fprintf(stderr, "Could't allocate space for the cartridge\n");
//         fclose(fd);
//         exit(1);
//     }
    
//     cartridge->nMapperID = ((sHeader.mapper2 >> 4) << 4) | (sHeader.mapper1 >> 4);
//     cartridge->mirror = (sHeader.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;

//     uint8_t fileType = 1;

//     if (fileType == 1) {
//             printf("CartridgeCreateMM1");
//         mapper->nPrgBanks = sHeader.prgRomChunks;

//         cartridge->vPRGMemory = (uint8_t*)malloc(16384*sHeader.prgRomChunks);
//         printf("CartridgeCreateMallocS");
//         cartridge->vPRGMemorySize = 0;
//         cartridge->vPRGMemoryCapacity = 16384*sHeader.prgRomChunks;
//         printf("CartridgeCreateMM2");
//         ifile = fread(cartridge->vPRGMemory, sizeof(uint8_t*), 16384*sHeader.prgRomChunks, fd);
//         printf("CartridgeCreateMM3");
//         mapper->nChrBanks = sHeader.chrRomChunks;

//         cartridge->vCHRMemory = (uint8_t*)malloc(8192*sHeader.chrRomChunks);
//         cartridge->vCHRMemorySize = 0;
//         cartridge->vCHRMemoryCapacity = 16384*sHeader.prgRomChunks;
//         ifile = fread(cartridge->vPRGMemory, sizeof(uint8_t*), 8192*sHeader.prgRomChunks, fd);
        
        
//     }
//     if (fclose(fd)) { 
//         fprintf(stdout, "Error closing the file\n");
//         exit(1);
//     }
//     ;
//     switch (cartridge->nMapperID) {
//         case 0: cartridge->pMapper = Mapper_Create(mapper->nPrgBanks, mapper->nChrBanks); break;
//         default: assert(cartridge->nMapperID && "Mapper ID not implemented"); break;
//     }
//     printf("CartridgeCreateEE");
//     return cartridge;
    
// }

Cartridge* CartridgeCreate(const char* filePath) {
    printf("CartridgeCreateSS");
    FILE* fd = fopen(filePath, "rb");
    if (fd == NULL) {
        perror("Error opening file");
        exit(1);
    }

    NesHeader sHeader = { 0 };
    if (fread(&sHeader, sizeof(NesHeader), 1, fd) != 1) {
        fprintf(stderr, "Error reading header\n");
        fclose(fd);
        exit(1);
    }

    Cartridge* cartridge = (Cartridge*)malloc(sizeof(Cartridge));
    if (!cartridge) {
        perror("Could't allocate space for the cartridge");
        fclose(fd);
        exit(1);
    }

    cartridge->bImageValid = 0;
    cartridge->nMapperID = ((sHeader.mapper2 >> 4) << 4) | (sHeader.mapper1 & 0x0F); // Corrected mask
    cartridge->mirror = (sHeader.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;

    cartridge->nPRGBanks = sHeader.prgRomChunks;
    cartridge->vPRGMemoryCapacity = 16384 * cartridge->nPRGBanks;
    cartridge->vPRGMemory = (uint8_t*)malloc(cartridge->vPRGMemoryCapacity);
    if (!cartridge->vPRGMemory) {
        perror("Failed to allocate PRG memory");
        exit(1);
    }
    cartridge->vPRGMemorySize = fread(cartridge->vPRGMemory, sizeof(uint8_t), cartridge->vPRGMemoryCapacity, fd);

    cartridge->nCHRBanks = sHeader.chrRomChunks;
    cartridge->vCHRMemoryCapacity = 8192 * cartridge->nCHRBanks;
    cartridge->vCHRMemory = (uint8_t*)malloc(cartridge->vCHRMemoryCapacity);
    if (!cartridge->vCHRMemory) {
        perror("Failed to allocate CHR memory");
        exit(1);
    }
    cartridge->vCHRMemorySize = fread(cartridge->vCHRMemory, sizeof(uint8_t), cartridge->vCHRMemoryCapacity, fd);

    if (fclose(fd) != 0) {
        perror("Error closing the file");
        exit(1);
    }
    cartridge->pMapper = Mapper_000_Create(cartridge->nPRGBanks, cartridge->nCHRBanks);
    // switch (cartridge->nMapperID) {
    // case 0:  break;
    // default: assert(0 && "Mapper ID not implemented"); break; // Added message
    // }

    printf("CartridgeCreateEE\n");
    return cartridge;
}

void CartridgeReset(Cartridge *cartridge){
    if (cartridge->pMapper != NULL)
		mapperReset();
}

int CartridgeCpuRead(Cartridge *cartridge, uint16_t addr, uint8_t *data)
{
    // printf("CartridgeCpuRead\n");
    if (!cartridge) {
        printf("Error: cartridge is NULL.\n");
        //return;
    }
	uint32_t mapped_addr = 0;
    Mapper *map = cartridge->pMapper;
	if (map->cpuMapRead( addr, &mapped_addr))
	{
        
		*data = cartridge->vPRGMemory[mapped_addr];
        // printf("CartridgeCpuReadE1\n");
		return 1;
	}
	else{
        // printf("CartridgeCpuReadE2\n");
		return 0;
    }
}

int CartridgeCpuWrite(Cartridge *cartridge, uint16_t addr, uint8_t data)
{
	uint32_t mapped_addr = 0;
    Mapper *map = cartridge->pMapper;
	if (map->cpuMapWrite( addr, &mapped_addr))
	{
        cartridge->vPRGMemory[mapped_addr] = data;
        return 1;
        
	}
	else
		return 0;
}


int CartridgePpuRead(Cartridge* cartridge, uint16_t addr, uint8_t* data)
{
    //printf("CartridgePpuReadS\n");
	uint32_t mapped_addr = 0;
    Mapper *map = cartridge->pMapper;
	if (map->ppuMapRead(addr, &mapped_addr))
	{
		*data = cartridge->vCHRMemory[mapped_addr];
        //printf("CartridgePpuReadE1\n");
		return 1;
	}
	else{
        //printf("CartridgePpuReadE2\n");
		return 0;
    }
}
int CartridgePpuWrite(Cartridge* cartridge, uint16_t addr, uint8_t data)
{
	uint32_t mapped_addr = 0;
    Mapper *map = cartridge->pMapper;
	if (map->ppuMapWrite( addr, &mapped_addr))
	{
		cartridge->vCHRMemory[mapped_addr] = data;
		return 1;
	}
	else
		return 0;
}