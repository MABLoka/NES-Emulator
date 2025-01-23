
#include "Mapper0.h"



static Mapper mapper = {0};
Mapper* Mapper_000_Create(uint8_t prgBanks, uint8_t chrBanks){
	Mapper *mapper = (Mapper*)malloc(sizeof(Mapper));
	mapper = Mapper_Create(prgBanks, chrBanks);
	mapper->cpuMapRead = cpuMapRead;
	mapper->cpuMapWrite = cpuMapWrite;
	mapper->ppuMapRead = ppuMapRead;
	mapper->ppuMapWrite = ppuMapWrite;

    return mapper;
}

void mapperReset(){

}
int cpuMapRead( uint16_t addr, uint32_t* mapped_addr){
	// printf("cpuMapRead S\n");
    if (addr >= 0x8000 && addr <= 0xFFFF)
	{
		*mapped_addr = addr & (mapper.nPrgBanks > 1 ? 0x7FFF : 0x3FFF);
		return 1;
	}
	return 0;
}

int cpuMapWrite( uint16_t addr, uint32_t* mapped_addr){
    if (addr >= 0x8000 && addr <= 0xFFFF)
	{
		*mapped_addr = addr & (mapper.nPrgBanks > 1 ? 0x7FFF : 0x3FFF);
		return 1;
	}

	return 0;
}

int ppuMapRead( uint16_t addr, uint32_t* mapped_addr){
	//printf("ppuMapReadS\n");
    if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		*mapped_addr = addr;
		//printf("ppuMapReadE\n");
		return 1;
	}
	//printf("ppuMapReadE\n");
	return 0;
}

int ppuMapWrite(uint16_t addr, uint32_t* mapped_addr){
    // if (addr >= 0x0000 && addr <= 0x1FFF)
	// {
	// 	if (mapper.nChrBanks == 0)
	// 	{
	// 		*mapped_addr = addr;
	// 		return 1;
	// 	}
	// }

	return 0;
}
