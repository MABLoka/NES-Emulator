#ifndef MAPPER0_H
#define MAPPER0_H

#include <stdint.h>
#include "Mapper.h"




extern Mapper *mapper;
// Function prototypes for creating and destroying Mapper_000 instances
Mapper* Mapper_000_Create(uint8_t prgBanks, uint8_t chrBanks);

#endif //MAPPER0_H