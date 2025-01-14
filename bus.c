#include <stdint.h>
#include "bus.h"
#include <stdio.h>



void bus_init(BUS *bus){
    for (int i = 0; i < 2048; i++) {
        bus->ram[i] = 0;
    }
	bus->nSystemClockCounter = 0;
	//bus->cpu = GetCpu(); // Connect the bus to
}


// BUS ConnectBus(CPU *cpu){
// 	bus->cpu = cpu;
// 	return bus;
// }
// Write to the addresses connected
void write(BUS *bus, uint16_t addr, uint8_t data)
{
	//check if the address is in cartridege, cartridge is prioratized
	if(CartridgeCpuWrite(bus->cartridge, addr, data)){ //CartridgeCpuRead return 0 if writing in cartridge failed

	}
	else if (addr >= 0x0000 && addr <= 0x1FFF) // check if it can be written to ram
		bus->ram[addr & 0x07FF] = data;
}

// Insert the cartridge by calling this function	  
void insertCartridge(BUS *bus,const char* filePath){
	if (!filePath) {
        printf("Error: Invalid file path.\n");
        return;
    }

    bus->cartridge = CartridgeCreate(filePath);
    if (!bus->cartridge) {
        printf("Error: Failed to create cartridge from file: %s\n", filePath);
    }
}


uint8_t read(BUS *bus, uint16_t addr, int bReadOnly)
{
	uint8_t data = 0;
	if(CartridgeCpuRead(bus->cartridge, addr, &data)){ //CartridgeCpuRead return 0 if writing in cartridge failed

	}
		
	else if (addr >= 0x0000 && addr <= 0xFFFF)
		return bus->ram[addr];

	return data;
}
