#include <stdint.h>
#include "bus.h"
#include <stdio.h>
#include "cpu.h"


void bus_init(BUS *bus){
	
    for (int i = 0; i < 2048; i++) {
        bus->ram[i] = 0;
    }
	bus->nSystemClockCounter = 0;
	//bus->cpu = GetCpu(); // Connect the bus to
}
void reset(BUS *bus)
{
	printf("reset S");
	//CartridgeReset(bus->cartridge);
	
	cpuReset();
	printf("reset E");
	// ppuReset();
	bus->nSystemClockCounter = 0;
	printf("reset E");
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
	else if(addr >= 0x2000 && addr <= 0x3FFF)
		ppuCpuWrite(addr, data);
	else if (addr >= 0x4016 && addr <= 0x4017) {
	bus->controller_state[addr & 0x0001] = bus->controller[addr & 0x0001];
	}

}

// Insert the cartridge by calling this function	  
void insertCartridge(BUS *bus,Cartridge* cartridge){
    bus->cartridge = cartridge;
}


uint8_t read(BUS *bus, uint16_t addr, int bReadOnly)
{
	// printf("busread\n");
	uint8_t data = 0;
	if(CartridgeCpuRead(bus->cartridge, addr, &data)){ //CartridgeCpuRead return 0 if writing in cartridge failed
		
	}
	else if (addr >= 0x0000 && addr <= 0x1FFF)
	
		return bus->ram[addr];

	else if(addr >= 0x2000 && addr <= 0x3FFF){
		
		return ppuCpuRead(addr, 0);
	}

	else if (addr >= 0x4016 && addr <= 0x4017) {
        data = (bus->controller_state[addr & 0x0001] & 0x80) > 0;
        bus->controller_state[addr & 0x0001] <<= 1;
    }	
	return data;
}

void busClock(BUS *bus)
{
	// Clocking. The heart and soul of an emulator. The running
	// frequency is controlled by whatever calls this function.
	// So here we "divide" the clock as necessary and call
	// the peripheral devices clock() function at the correct
	// times.

	// The fastest clock frequency the digital system cares
	// about is equivalent to the PPU clock. So the PPU is clocked
	// each time this function is called.
	ppuClock();

	// The CPU runs 3 times slower than the PPU so we only call its
	// clock() function every 3 times this function is called. We
	// have a global counter to keep track of this.
	if (bus->nSystemClockCounter % 3 == 0)
	{
		cpuClock();
	}

	// The PPU is capable of emitting an interrupt to indicate the
	// vertical blanking period has been entered. If it has, we need
	// to send that irq to the CPU.
	if (bus->ppu.nmi)
	{
		bus->ppu.nmi = false;
		NMI();
	}

	bus->nSystemClockCounter++;
}
