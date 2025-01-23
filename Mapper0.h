#ifndef MAPPER0_H
#define MAPPER0_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "Mapper.h"




// Function prototypes for creating and destroying Mapper_000 instances
Mapper* Mapper_000_Create(uint8_t prgBanks, uint8_t chrBanks);
void mapperReset();
int cpuMapRead( uint16_t addr, uint32_t* mapped_addr);
int cpuMapWrite( uint16_t addr, uint32_t* mapped_addr);
int ppuMapRead(  uint16_t addr, uint32_t* mapped_addr);
int ppuMapWrite( uint16_t addr, uint32_t* mapped_addr);
#endif //MAPPER0_H