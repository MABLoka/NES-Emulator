#ifndef MAPPER_H
#define MAPPER_H

#include <stdint.h>
#include <stdlib.h>

// Define the Mapper struct
typedef struct {

    // Additional data specific to Mapper
    uint8_t nPrgBanks;
    uint8_t nChrBanks;
    
    int (*cpuMapRead)( uint16_t addr, uint32_t* mapped_addr);
    int (*cpuMapWrite)( uint16_t addr, uint32_t* mapped_addr);
    int (*ppuMapRead)( uint16_t addr, uint32_t* mapped_addr);
    int (*ppuMapWrite)( uint16_t addr, uint32_t* mapped_addr);
} Mapper;

// Function prototypes for creating and destroying Mapper instances
Mapper* Mapper_Create(uint8_t prgBanks, uint8_t chrBanks);




#endif // MAPPER_H