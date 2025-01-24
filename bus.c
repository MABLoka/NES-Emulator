#include <stdint.h>
#include "bus.h"
#include <stdio.h>
#include "cpu.h"


void bus_init(BUS *bus){
	
    for (int i = 0; i < 2048; i++) {
        bus->ram[i] = 0;
    }
	bus->nSystemClockCounter = 0;
	bus->dmaPage = 0x00;
	bus->dmaAddr = 0x00;
	bus->dmaData = 0x00;

	bus->dmaDummy = 1;
	bus->dmaTransfer = 0;
	//bus->cpu = GetCpu(); // Connect the bus to
}
void reset(BUS *bus)
{
	printf("reset S");
	//CartridgeReset(bus->cartridge);
	
	cpuReset();
	// ppuReset();
	bus->nSystemClockCounter = 0;
	bus->dmaPage = 0x00;
	bus->dmaAddr = 0x00;
	bus->dmaData = 0x00;

	bus->dmaDummy = 1;
	bus->dmaTransfer = 0;
	printf("reset E");
}

// BUS ConnectBus(CPU *cpu){
// 	bus->cpu = cpu;
// 	return bus;
// }
// Read the addresses connected
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

	else if(addr == 0x4014){
		bus->dmaPage = data;
		bus->dmaAddr = 0x00;
		bus->dmaTransfer = 1;	
	}
	else if (addr >= 0x4016 && addr <= 0x4017) {
		
		bus->controller_state[addr & 0x0001] = bus->controller[addr & 0x0001];
	}

}

// Insert the cartridge by calling this function	  
void insertCartridge(BUS *bus,Cartridge* cartridge){
    bus->cartridge = cartridge;
}




void busClock(BUS *bus)
{
	ppuClock();

	if (bus->nSystemClockCounter % 3 == 0)
	{
		if (bus->dmaTransfer)
		{
			if (bus->dmaDummy)
			{
				if (bus->nSystemClockCounter % 2 == 1)
				{
					bus->dmaDummy = false;
				}
			}
			else
			{

				if (bus->nSystemClockCounter % 2 == 0)
				{
					bus->dmaData = cpuRead(bus->dmaPage << 8 | bus->dmaAddr);
				}
				else
				{
					bus->ppu->pOAM[bus->dmaAddr] = bus->dmaData = 0x00;;

					bus->dmaAddr++;

					if (bus->dmaAddr == 0x00)
					{
						bus->dmaTransfer = 0;
						bus->dmaDummy = 1;
					}
				}
			}
		}
		else
			cpuClock();
	}

	
	if (bus->ppu->nmi)
	{
		bus->ppu->nmi = false;
		NMI();
	}

	bus->nSystemClockCounter++;
}
