#ifndef CARTRIDGE_H
#define CARTRIDGE_H

// #include <stdint.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <fstream>
// #include <stddef.h>
// #include <inttypes.h>
// #include "Mapper.h"
// #include <inttypes.h>

#include "Mapper0.h"

typedef enum 
	{
		HORIZONTAL,
		VERTICAL,
		ONESCREEN_LO,
		ONESCREEN_HI,
} MIRROR;

typedef struct {
    
    MIRROR mirror;
    int bImageValid;


    uint8_t nMapperID;

    uint8_t* vPRGMemory; 
    size_t vPRGMemorySize;
    size_t vPRGMemoryCapacity;
    uint8_t* vCHRMemory; 
    size_t vCHRMemorySize;
    size_t vCHRMemoryCapacity;

    uint8_t nPRGBanks;
	uint8_t nCHRBanks;

    Mapper* pMapper; 
} Cartridge;

Cartridge *CartridgeCreate(const char* filePath);


int CartridgeCpuRead(Cartridge* cartridge, uint16_t addr, uint8_t* data);
int CartridgeCpuWrite(Cartridge* cartridge, uint16_t addr, uint8_t data);

int CartridgePpuRead(Cartridge* cartridge, uint16_t addr, uint8_t* data);
int CartridgePpuWrite(Cartridge* cartridge, uint16_t addr, uint8_t data);

void CartridgeReset();
#endif  // CARTRIDGE_H