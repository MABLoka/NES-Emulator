#include <stdint.h>
#include <stdlib.h>
#include "Mapper.h"



Mapper *Mapper_Create(uint8_t prgBanks, uint8_t chrBanks) {
    Mapper *mapper = (Mapper*)malloc(sizeof(Mapper));
    mapper->nPrgBanks = prgBanks;
    mapper->nChrBanks = chrBanks;
    mapper->cpuMapRead = NULL;
    mapper->cpuMapWrite = NULL;
    mapper->ppuMapRead = NULL;
    mapper->ppuMapWrite = NULL;
    return mapper;
}


