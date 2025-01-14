#ifndef BUS_H
#define BUS_H

#include <stdint.h>
#include "Mapper0.h"
#include "cartridge.h"



// CPU State
typedef struct {
    Cartridge *cartridge;
    uint8_t ram[2048]; // 2KB Memory Space
    uint32_t nSystemClockCounter;
} BUS;





// Function prototypes
void bus_init(BUS *bus);
void write(BUS *bus, uint16_t addr, uint8_t data);
uint8_t read(BUS *bus, uint16_t addr, int bReadOnly);
void insertCartridge(BUS *bus, const char* filePath);
void BusClock();
#endif