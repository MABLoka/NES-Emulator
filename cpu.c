#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include "cpu.h"


static CPU cpu = {0};

void cpu_init(BUS* bus){

    // Registers
    cpu.A = 0;
    cpu.X = 0;
    cpu.Y = 0;
    // Status register
    cpu.F = 0;
    //stack pointer
    cpu.SP = 0xFD;
    // Program counter
    cpu.PC = 0xFFFC;
   
	cpu.bus = bus; // Connects the cpu to the bus
    cpu.fetched = 0x00;
    cpu.addr_abs = 0x0000;
    cpu.opcode = 0x00;
    cpu.cycles = 0;
    cpu.clock_count = 0;
}

// return the cpu to connect to the bus
CPU* GetCpu(){
	return &cpu;
}

//Instruction Lookup Table
struct Instruction {
    const char* name;
    uint8_t (*operation)(void);
    uint8_t (*addressingMode)(void);
    int cycles;
};
struct Instruction lookup[] = 
	{
		{ "BRK", BRK, IMM, 7 },{ "ORA", ORA, IZX, 6 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "???", NOP, IMP, 3 },{ "ORA", ORA, ZP0, 3 },{ "ASL", ASL, ZP0, 5 },{ "???", XXX, IMP, 5 },{ "PHP", PHP, IMP, 3 },{ "ORA", ORA, IMM, 2 },{ "ASL", ASL, IMP, 2 },{ "???", XXX, IMP, 2 },{ "???", NOP, IMP, 4 },{ "ORA", ORA, ABS, 4 },{ "ASL", ASL, ABS, 6 },{ "???", XXX, IMP, 6 },
		{ "BPL", BPL, REL, 2 },{ "ORA", ORA, IZY, 5 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "???", NOP, IMP, 4 },{ "ORA", ORA, ZPX, 4 },{ "ASL", ASL, ZPX, 6 },{ "???", XXX, IMP, 6 },{ "CLC", CLC, IMP, 2 },{ "ORA", ORA, ABY, 4 },{ "???", NOP, IMP, 2 },{ "???", XXX, IMP, 7 },{ "???", NOP, IMP, 4 },{ "ORA", ORA, ABX, 4 },{ "ASL", ASL, ABX, 7 },{ "???", XXX, IMP, 7 },
		{ "JSR", JSR, ABS, 6 },{ "AND", AND, IZX, 6 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "BIT", BIT, ZP0, 3 },{ "AND", AND, ZP0, 3 },{ "ROL", ROL, ZP0, 5 },{ "???", XXX, IMP, 5 },{ "PLP", PLP, IMP, 4 },{ "AND", AND, IMM, 2 },{ "ROL", ROL, IMP, 2 },{ "???", XXX, IMP, 2 },{ "BIT", BIT, ABS, 4 },{ "AND", AND, ABS, 4 },{ "ROL", ROL, ABS, 6 },{ "???", XXX, IMP, 6 },
		{ "BMI", BMI, REL, 2 },{ "AND", AND, IZY, 5 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "???", NOP, IMP, 4 },{ "AND", AND, ZPX, 4 },{ "ROL", ROL, ZPX, 6 },{ "???", XXX, IMP, 6 },{ "SEC", SEC, IMP, 2 },{ "AND", AND, ABY, 4 },{ "???", NOP, IMP, 2 },{ "???", XXX, IMP, 7 },{ "???", NOP, IMP, 4 },{ "AND", AND, ABX, 4 },{ "ROL", ROL, ABX, 7 },{ "???", XXX, IMP, 7 },
		{ "RTI", RTI, IMP, 6 },{ "EOR", EOR, IZX, 6 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "???", NOP, IMP, 3 },{ "EOR", EOR, ZP0, 3 },{ "LSR", LSR, ZP0, 5 },{ "???", XXX, IMP, 5 },{ "PHA", PHA, IMP, 3 },{ "EOR", EOR, IMM, 2 },{ "LSR", LSR, IMP, 2 },{ "???", XXX, IMP, 2 },{ "JMP", JMP, ABS, 3 },{ "EOR", EOR, ABS, 4 },{ "LSR", LSR, ABS, 6 },{ "???", XXX, IMP, 6 },
		{ "BVC", BVC, REL, 2 },{ "EOR", EOR, IZY, 5 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "???", NOP, IMP, 4 },{ "EOR", EOR, ZPX, 4 },{ "LSR", LSR, ZPX, 6 },{ "???", XXX, IMP, 6 },{ "CLI", CLI, IMP, 2 },{ "EOR", EOR, ABY, 4 },{ "???", NOP, IMP, 2 },{ "???", XXX, IMP, 7 },{ "???", NOP, IMP, 4 },{ "EOR", EOR, ABX, 4 },{ "LSR", LSR, ABX, 7 },{ "???", XXX, IMP, 7 },
		{ "RTS", RTS, IMP, 6 },{ "ADC", ADC, IZX, 6 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "???", NOP, IMP, 3 },{ "ADC", ADC, ZP0, 3 },{ "ROR", ROR, ZP0, 5 },{ "???", XXX, IMP, 5 },{ "PLA", PLA, IMP, 4 },{ "ADC", ADC, IMM, 2 },{ "ROR", ROR, IMP, 2 },{ "???", XXX, IMP, 2 },{ "JMP", JMP, IND, 5 },{ "ADC", ADC, ABS, 4 },{ "ROR", ROR, ABS, 6 },{ "???", XXX, IMP, 6 },
		{ "BVS", BVS, REL, 2 },{ "ADC", ADC, IZY, 5 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "???", NOP, IMP, 4 },{ "ADC", ADC, ZPX, 4 },{ "ROR", ROR, ZPX, 6 },{ "???", XXX, IMP, 6 },{ "SEI", SEI, IMP, 2 },{ "ADC", ADC, ABY, 4 },{ "???", NOP, IMP, 2 },{ "???", XXX, IMP, 7 },{ "???", NOP, IMP, 4 },{ "ADC", ADC, ABX, 4 },{ "ROR", ROR, ABX, 7 },{ "???", XXX, IMP, 7 },
		{ "???", NOP, IMP, 2 },{ "STA", STA, IZX, 6 },{ "???", NOP, IMP, 2 },{ "???", XXX, IMP, 6 },{ "STY", STY, ZP0, 3 },{ "STA", STA, ZP0, 3 },{ "STX", STX, ZP0, 3 },{ "???", XXX, IMP, 3 },{ "DEY", DEY, IMP, 2 },{ "???", NOP, IMP, 2 },{ "TXA", TXA, IMP, 2 },{ "???", XXX, IMP, 2 },{ "STY", STY, ABS, 4 },{ "STA", STA, ABS, 4 },{ "STX", STX, ABS, 4 },{ "???", XXX, IMP, 4 },
		{ "BCC", BCC, REL, 2 },{ "STA", STA, IZY, 6 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 6 },{ "STY", STY, ZPX, 4 },{ "STA", STA, ZPX, 4 },{ "STX", STX, ZPY, 4 },{ "???", XXX, IMP, 4 },{ "TYA", TYA, IMP, 2 },{ "STA", STA, ABY, 5 },{ "TXS", TXS, IMP, 2 },{ "???", XXX, IMP, 5 },{ "???", NOP, IMP, 5 },{ "STA", STA, ABX, 5 },{ "???", XXX, IMP, 5 },{ "???", XXX, IMP, 5 },
		{ "LDY", LDY, IMM, 2 },{ "LDA", LDA, IZX, 6 },{ "LDX", LDX, IMM, 2 },{ "???", XXX, IMP, 6 },{ "LDY", LDY, ZP0, 3 },{ "LDA", LDA, ZP0, 3 },{ "LDX", LDX, ZP0, 3 },{ "???", XXX, IMP, 3 },{ "TAY", TAY, IMP, 2 },{ "LDA", LDA, IMM, 2 },{ "TAX", TAX, IMP, 2 },{ "???", XXX, IMP, 2 },{ "LDY", LDY, ABS, 4 },{ "LDA", LDA, ABS, 4 },{ "LDX", LDX, ABS, 4 },{ "???", XXX, IMP, 4 },
		{ "BCS", BCS, REL, 2 },{ "LDA", LDA, IZY, 5 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 5 },{ "LDY", LDY, ZPX, 4 },{ "LDA", LDA, ZPX, 4 },{ "LDX", LDX, ZPY, 4 },{ "???", XXX, IMP, 4 },{ "CLV", CLV, IMP, 2 },{ "LDA", LDA, ABY, 4 },{ "TSX", TSX, IMP, 2 },{ "???", XXX, IMP, 4 },{ "LDY", LDY, ABX, 4 },{ "LDA", LDA, ABX, 4 },{ "LDX", LDX, ABY, 4 },{ "???", XXX, IMP, 4 },
		{ "CPY", CPY, IMM, 2 },{ "CMP", CMP, IZX, 6 },{ "???", NOP, IMP, 2 },{ "???", XXX, IMP, 8 },{ "CPY", CPY, ZP0, 3 },{ "CMP", CMP, ZP0, 3 },{ "DEC", DEC, ZP0, 5 },{ "???", XXX, IMP, 5 },{ "INY", INY, IMP, 2 },{ "CMP", CMP, IMM, 2 },{ "DEX", DEX, IMP, 2 },{ "???", XXX, IMP, 2 },{ "CPY", CPY, ABS, 4 },{ "CMP", CMP, ABS, 4 },{ "DEC", DEC, ABS, 6 },{ "???", XXX, IMP, 6 },
		{ "BNE", BNE, REL, 2 },{ "CMP", CMP, IZY, 5 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "???", NOP, IMP, 4 },{ "CMP", CMP, ZPX, 4 },{ "DEC", DEC, ZPX, 6 },{ "???", XXX, IMP, 6 },{ "CLD", CLD, IMP, 2 },{ "CMP", CMP, ABY, 4 },{ "NOP", NOP, IMP, 2 },{ "???", XXX, IMP, 7 },{ "???", NOP, IMP, 4 },{ "CMP", CMP, ABX, 4 },{ "DEC", DEC, ABX, 7 },{ "???", XXX, IMP, 7 },
		{ "CPX", CPX, IMM, 2 },{ "SBC", SBC, IZX, 6 },{ "???", NOP, IMP, 2 },{ "???", XXX, IMP, 8 },{ "CPX", CPX, ZP0, 3 },{ "SBC", SBC, ZP0, 3 },{ "INC", INC, ZP0, 5 },{ "???", XXX, IMP, 5 },{ "INX", INX, IMP, 2 },{ "SBC", SBC, IMM, 2 },{ "NOP", NOP, IMP, 2 },{ "???", SBC, IMP, 2 },{ "CPX", CPX, ABS, 4 },{ "SBC", SBC, ABS, 4 },{ "INC", INC, ABS, 6 },{ "???", XXX, IMP, 6 },
		{ "BEQ", BEQ, REL, 2 },{ "SBC", SBC, IZY, 5 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "???", NOP, IMP, 4 },{ "SBC", SBC, ZPX, 4 },{ "INC", INC, ZPX, 6 },{ "???", XXX, IMP, 6 },{ "SED", SED, IMP, 2 },{ "SBC", SBC, ABY, 4 },{ "NOP", NOP, IMP, 2 },{ "???", XXX, IMP, 7 },{ "???", NOP, IMP, 4 },{ "SBC", SBC, ABX, 4 },{ "INC", INC, ABX, 7 },{ "???", XXX, IMP, 7 },
	};

uint8_t cpu_read( uint16_t a)
{
	return read(cpu.bus, a, 0);
}
void cpu_write( uint16_t a, uint16_t d)
{
	write(cpu.bus, a, d);
}


void SetFlag( Flags f, int state){
    if(state){
        cpu.F |= f;
    }
    else{
        cpu.F &= ~f;
    }
}
uint8_t GetFlag( Flags f){
    
    return ((cpu.F & f) > 0) ? 1 : 0;
    
}

void cpuClock()
{
	
	if (cpu.cycles == 0)
	{
		
		cpu.opcode = cpu_read(cpu.PC);
		
		SetFlag(U, 1);
		
		cpu.PC++;
        uint8_t additional_cycle1 = (lookup[cpu.opcode].addressingMode)();

		// Perform operation
		uint8_t additional_cycle2 = (lookup[cpu.opcode].operation)();
		cpu.cycles = lookup[cpu.opcode].cycles;

        cpu.cycles += (additional_cycle1 & additional_cycle2);
		
		SetFlag(U, 1);
	}
	
	cpu.clock_count++;

	cpu.cycles--;
}

void reset()
{
	
	cpu.addr_abs = 0xFFFC;
	uint16_t lo = cpu_read(cpu.addr_abs + 0);
	uint16_t hi = cpu_read(cpu.addr_abs + 1);

	cpu.PC = (hi << 8) | lo;

	cpu.A = 0;
	cpu.X = 0;
	cpu.Y = 0;
	cpu.SP = 0xFD;
	cpu.F = 0x00 | U;

	cpu.addr_rel = 0x0000;
	cpu.addr_abs = 0x0000;
	cpu.fetched = 0x00;

	cpu.cycles = 8;
}

void nmi()
{
	cpu_write(0x0100 + cpu.SP, (cpu.PC >> 8) & 0x00FF);
	cpu.SP--;
	cpu_write(0x0100 + cpu.SP, cpu.PC & 0x00FF);
	cpu.SP--;

	SetFlag(B, 0);
	SetFlag(U, 1);
	SetFlag(I, 1);
	cpu_write(0x0100 + cpu.SP, cpu.F);
	cpu.SP--;

	cpu.addr_abs = 0xFFFA;
	uint16_t lo = cpu_read(cpu.addr_abs + 0);
	uint16_t hi = cpu_read(cpu.addr_abs + 1);
	cpu.PC = (hi << 8) | lo;

	cpu.cycles = 8;
}

void irq(){
	if(GetFlag(I) == 0){
		
		cpu_write(0x0100 + cpu.SP, (cpu.PC >> 8) & 0x00FF);
		cpu.SP--;
		cpu_write(0x0100 + cpu.SP, cpu.PC & 0x00FF);
		cpu.SP--;

		
		SetFlag(B, 0);
		SetFlag(U, 1);
		SetFlag(I, 1);
		cpu_write(0x0100 + cpu.SP, cpu.F);
		cpu.SP--;

		
		cpu.addr_abs = 0xFFFE;
		uint16_t lo = cpu_read(cpu.addr_abs + 0);
		uint16_t hi = cpu_read(cpu.addr_abs + 1);
		cpu.PC = (hi << 8) | lo;

		
		cpu.cycles = 7;
	}
}

uint8_t ADC()
{
	
	fetch();
	
	uint16_t temp = (uint16_t)cpu.A + (uint16_t)cpu.fetched + (uint16_t)GetFlag(C);
	
	SetFlag(C, temp > 255);
	
	SetFlag(Z, (temp & 0x00FF) == 0);
	
	SetFlag(V, (~((uint16_t)cpu.A ^ (uint16_t)cpu.fetched) & ((uint16_t)cpu.A ^ (uint16_t)temp)) & 0x0080);
	
	SetFlag(N, temp & 0x80);
	
	cpu.A = temp & 0x00FF;
	
	return 1;
}


uint8_t SBC()
{
	fetch();

	
	uint16_t value = ((uint16_t)cpu.fetched) ^ 0x00FF;

	uint16_t temp = (uint16_t)cpu.A + value + (uint16_t)GetFlag(C);
	SetFlag(C, temp & 0xFF00);
	SetFlag(Z, ((temp & 0x00FF) == 0));
	SetFlag(V, (temp ^ (uint16_t)cpu.A) & (temp ^ value) & 0x0080);
	SetFlag(N, temp & 0x0080);
	cpu.A = temp & 0x00FF;
	return 1;
}



// Bitwise Logic AND

uint8_t AND()
{
	fetch();
	cpu.A = cpu.A & cpu.fetched;
	SetFlag(Z, cpu.A == 0x00);
	SetFlag(N, cpu.A & 0x80);
	return 1;
}


// Arithmetic Shift Left

uint8_t ASL()
{
	fetch();
	uint16_t temp = (uint16_t)cpu.fetched << 1;
	SetFlag(C, (temp & 0xFF00) > 0);
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, temp & 0x80);
	if (lookup[cpu.opcode].addressingMode == &IMP)
		cpu.A = temp & 0x00FF;
	else
		write(cpu.bus, cpu.addr_abs, temp & 0x00FF);
	return 0;
}


// Branch if Carry Clear

uint8_t BCC()
{
	if (GetFlag(C) == 0)
	{
		cpu.cycles++;
		cpu.addr_abs = cpu.PC + cpu.addr_rel;
		
		if((cpu.addr_abs & 0xFF00) != (cpu.PC & 0xFF00))
			cpu.cycles++;
		
		cpu.PC = cpu.addr_abs;
	}
	return 0;
}


// Branch if Carry Set

uint8_t BCS()
{
	if (GetFlag(C) == 1)
	{
		cpu.cycles++;
		cpu.addr_abs = cpu.PC + cpu.addr_rel;

		if ((cpu.addr_abs & 0xFF00) != (cpu.PC & 0xFF00))
			cpu.cycles++;

		cpu.PC = cpu.addr_abs;
	}
	return 0;
}


// Branch if Equal

uint8_t BEQ()
{
	if (GetFlag(Z) == 1)
	{
		cpu.cycles++;
		cpu.addr_abs = cpu.PC + cpu.addr_rel;

		if ((cpu.addr_abs & 0xFF00) != (cpu.PC & 0xFF00))
			cpu.cycles++;

		cpu.PC = cpu.addr_abs;
	}
	return 0;
}

uint8_t BIT()
{
	fetch();
	uint16_t temp = cpu.A & cpu.fetched;
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, cpu.fetched & (1 << 7));
	SetFlag(V, cpu.fetched & (1 << 6));
	return 0;
}


// Branch if Negative

uint8_t BMI()
{
	if (GetFlag(N) == 1)
	{
		cpu.cycles++;
		cpu.addr_abs = cpu.PC + cpu.addr_rel;

		if ((cpu.addr_abs & 0xFF00) != (cpu.PC & 0xFF00))
			cpu.cycles++;

		cpu.PC = cpu.addr_abs;
	}
	return 0;
}


// Branch if Not Equal

uint8_t BNE()
{
	if (GetFlag(Z) == 0)
	{
		cpu.cycles++;
		cpu.addr_abs = cpu.PC + cpu.addr_rel;

		if ((cpu.addr_abs & 0xFF00) != (cpu.PC & 0xFF00))
			cpu.cycles++;

		cpu.PC = cpu.addr_abs;
	}
	return 0;
}


// Branch if Positive

uint8_t BPL()
{
	if (GetFlag(N) == 0)
	{
		cpu.cycles++;
		cpu.addr_abs = cpu.PC + cpu.addr_rel;

		if ((cpu.addr_abs & 0xFF00) != (cpu.PC & 0xFF00))
			cpu.cycles++;

		cpu.PC = cpu.addr_abs;
	}
	return 0;
}

// Break

uint8_t BRK()
{
	cpu.PC++;
	
	SetFlag(I, 1);
	write(cpu.bus, 0x0100 + cpu.SP, (cpu.PC >> 8) & 0x00FF);
	cpu.SP--;
	write(cpu.bus, 0x0100 + cpu.SP, cpu.PC & 0x00FF);
	cpu.SP--;

	SetFlag(B, 1);
	write(cpu.bus, 0x0100 + cpu.SP, cpu.F);
	cpu.SP--;
	SetFlag(B, 0);

	cpu.PC = (uint16_t)cpu_read(0xFFFE) | ((uint16_t)cpu_read(0xFFFF) << 8);
	return 0;
}


// Branch if Overflow Clear

uint8_t BVC()
{
	if (GetFlag(V) == 0)
	{
		cpu.cycles++;
		cpu.addr_abs = cpu.PC + cpu.addr_rel;

		if ((cpu.addr_abs & 0xFF00) != (cpu.PC & 0xFF00))
			cpu.cycles++;

		cpu.PC = cpu.addr_abs;
	}
	return 0;
}


// Branch if Overflow Set

uint8_t BVS()
{
	if (GetFlag(V) == 1)
	{
		cpu.cycles++;
		cpu.addr_abs = cpu.PC + cpu.addr_rel;

		if ((cpu.addr_abs & 0xFF00) != (cpu.PC & 0xFF00))
			cpu.cycles++;

		cpu.PC = cpu.addr_abs;
	}
	return 0;
}


// Clear Carry Flag

uint8_t CLC()
{
	SetFlag(C, 0);
	return 0;
}


// Clear Decimal Flag

uint8_t CLD()
{
	SetFlag(D, 0);
	return 0;
}


// Disable Interrupts / Clear Interrupt Flag

uint8_t CLI()
{
	SetFlag(I, 0);
	return 0;
}


// Clear Overflow Flag

uint8_t CLV()
{
	SetFlag(V, 0);
	return 0;
}

// Compare Accumulator


uint8_t CMP()
{
	fetch();
	uint16_t temp = (uint16_t)cpu.A - (uint16_t)cpu.fetched;
	SetFlag(C, cpu.A >= cpu.fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 1;
}


// Compare X Register


uint8_t CPX()
{
	fetch();
	uint16_t temp = (uint16_t)cpu.X - (uint16_t)cpu.fetched;
	SetFlag(C, cpu.X >= cpu.fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}


// Compare Y Register


uint8_t CPY()
{
	fetch();
	uint16_t temp = (uint16_t)cpu.Y - (uint16_t)cpu.fetched;
	SetFlag(C, cpu.Y >= cpu.fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}


// Decrement Value at Memory Location


uint8_t DEC()
{
	fetch();
	uint16_t temp = cpu.fetched - 1;
	write(cpu.bus, cpu.addr_abs, temp & 0x00FF);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}


// Decrement X Register


uint8_t DEX()
{
	cpu.X--;
	SetFlag(Z, cpu.X == 0x00);
	SetFlag(N, cpu.X & 0x80);
	return 0;
}


// Decrement Y Register


uint8_t DEY()
{
	cpu.Y--;
	SetFlag(Z, cpu.Y == 0x00);
	SetFlag(N, cpu.Y & 0x80);
	return 0;
}


// Bitwise Logic XOR


uint8_t EOR()
{
	fetch();
	cpu.A = cpu.A ^ cpu.fetched;	
	SetFlag(Z, cpu.A == 0x00);
	SetFlag(N, cpu.A & 0x80);
	return 1;
}


// Increment Value at Memory Location


uint8_t INC()
{
	fetch();
	uint16_t temp = cpu.fetched + 1;
	write(cpu.bus, cpu.addr_abs, temp & 0x00FF);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}


// Increment X Register


uint8_t INX()
{
	cpu.X++;
	SetFlag(Z, cpu.X == 0x00);
	SetFlag(N, cpu.X & 0x80);
	return 0;
}


// Increment Y Register


uint8_t INY()
{
	cpu.Y++;
	SetFlag(Z, cpu.Y == 0x00);
	SetFlag(N, cpu.Y & 0x80);
	return 0;
}


// Jump To Location

uint8_t JMP()
{
	cpu.PC = cpu.addr_abs;
	return 0;
}


// Jump To Sub-Routine

uint8_t JSR()
{
	cpu.PC--;

	write(cpu.bus, 0x0100 + cpu.SP, (cpu.PC >> 8) & 0x00FF);
	cpu.SP--;
	write(cpu.bus, 0x0100 + cpu.SP, cpu.PC & 0x00FF);
	cpu.SP--;

	cpu.PC = cpu.addr_abs;
	return 0;
}


// Load The Accumulator


uint8_t LDA()
{
	fetch();
	cpu.A = cpu.fetched;
	SetFlag(Z, cpu.A == 0x00);
	SetFlag(N, cpu.A & 0x80);
	return 1;
}


// Load The X Register


uint8_t LDX()
{
	fetch();
	cpu.X = cpu.fetched;
	SetFlag(Z, cpu.X == 0x00);
	SetFlag(N, cpu.X & 0x80);
	return 1;
}


// Load The Y Register


uint8_t LDY()
{
	fetch();
	cpu.Y = cpu.fetched;
	SetFlag(Z, cpu.Y == 0x00);
	SetFlag(N, cpu.Y & 0x80);
	return 1;
}

uint8_t LSR()
{
	fetch();
	SetFlag(C, cpu.fetched & 0x0001);
	uint16_t temp = cpu.fetched >> 1;	
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	if (lookup[cpu.opcode].addressingMode == &IMP)
		cpu.A = temp & 0x00FF;
	else
		write(cpu.bus, cpu.addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t NOP()
{

	switch (cpu.opcode) {
	case 0x1C:
	case 0x3C:
	case 0x5C:
	case 0x7C:
	case 0xDC:
	case 0xFC:
		return 1;
		break;
	}
	return 0;
}


// Bitwise Logic OR


uint8_t ORA()
{
	fetch();
	cpu.A = cpu.A | cpu.fetched;
	SetFlag(Z, cpu.A == 0x00);
	SetFlag(N, cpu.A & 0x80);
	return 1;
}


// Push Accumulator to Stack

uint8_t PHA()
{
	write(cpu.bus, 0x0100 + cpu.SP, cpu.A);
	cpu.SP--;
	return 0;
}


// Push Status Register to Stack

uint8_t PHP()
{
	write(cpu.bus, 0x0100 + cpu.SP, cpu.F| B | U);
	SetFlag(B, 0);
	SetFlag(U, 0);
	cpu.SP--;
	return 0;
}


// Pop Accumulator off Stack


uint8_t PLA()
{
	cpu.SP++;
	cpu.A = cpu_read(0x0100 + cpu.SP);
	SetFlag(Z, cpu.A == 0x00);
	SetFlag(N, cpu.A & 0x80);
	return 0;
}


// Pop Status Register off Stack

uint8_t PLP()
{
	cpu.SP++;
	cpu.F= cpu_read(0x0100 + cpu.SP);
	SetFlag(U, 1);
	return 0;
}

uint8_t ROL()
{
	fetch();
	uint16_t temp = (uint16_t)(cpu.fetched << 1) | GetFlag(C);
	SetFlag(C, temp & 0xFF00);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	if (lookup[cpu.opcode].addressingMode == &IMP)
		cpu.A = temp & 0x00FF;
	else
		write(cpu.bus, cpu.addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t ROR()
{
	fetch();
	uint16_t temp = (uint16_t)(GetFlag(C) << 7) | (cpu.fetched >> 1);
	SetFlag(C, cpu.fetched & 0x01);
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, temp & 0x0080);
	if (lookup[cpu.opcode].addressingMode == &IMP)
		cpu.A = temp & 0x00FF;
	else
		write(cpu.bus, cpu.addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t RTI()
{
	cpu.SP++;
	cpu.F= cpu_read(0x0100 + cpu.SP);
	cpu.F&= ~B;
	cpu.F&= ~U;

	cpu.SP++;
	cpu.PC = (uint16_t)cpu_read(0x0100 + cpu.SP);
	cpu.SP++;
	cpu.PC |= (uint16_t)cpu_read(0x0100 + cpu.SP) << 8;
	return 0;
}

uint8_t RTS()
{
	cpu.SP++;
	cpu.PC = (uint16_t)cpu_read(0x0100 + cpu.SP);
	cpu.SP++;
	cpu.PC |= (uint16_t)cpu_read(0x0100 + cpu.SP) << 8;
	
	cpu.PC++;
	return 0;
}




// Set Carry Flag

uint8_t SEC()
{
	SetFlag(C, 1);
	return 0;
}


// Set Decimal Flag

uint8_t SED()
{
	SetFlag(D, 1);
	return 0;
}


// Set Interrupt Flag / Enable Interrupts

uint8_t SEI()
{
	SetFlag(I, 1);
	return 0;
}


// Store Accumulator at Address

uint8_t STA()
{
	write(cpu.bus, cpu.addr_abs, cpu.A);
	return 0;
}


// Store X Register at Address

uint8_t STX()
{
	write(cpu.bus, cpu.addr_abs, cpu.X);
	return 0;
}


// Store Y Register at Address

uint8_t STY()
{
	write(cpu.bus, cpu.addr_abs, cpu.Y);
	return 0;
}


// Transfer Accumulator to X Register

uint8_t TAX()
{
	cpu.X = cpu.A;
	SetFlag(Z, cpu.X == 0x00);
	SetFlag(N, cpu.X & 0x80);
	return 0;
}


// Transfer Accumulator to Y Register

uint8_t TAY()
{
	cpu.Y = cpu.A;
	SetFlag(Z, cpu.Y == 0x00);
	SetFlag(N, cpu.Y & 0x80);
	return 0;
}


// Transfer Stack Pointer to X Register

uint8_t TSX()
{
	cpu.X = cpu.SP;
	SetFlag(Z, cpu.X == 0x00);
	SetFlag(N, cpu.X & 0x80);
	return 0;
}


// Transfer X Register to Accumulator

uint8_t TXA()
{
	cpu.A = cpu.X;
	SetFlag(Z, cpu.A == 0x00);
	SetFlag(N, cpu.A & 0x80);
	return 0;
}


// Transfer X Register to Stack Pointer

uint8_t TXS()
{
	cpu.SP = cpu.X;
	return 0;
}


//Transfer Y Register to Accumulator

uint8_t TYA()
{
	cpu.A = cpu.Y;
	SetFlag(Z, cpu.A == 0x00);
	SetFlag(N, cpu.A & 0x80);
	return 0;
}


// illegal opcodes
uint8_t XXX()
{
	return 0;
}








//Addressing modes
// Immidiate
uint8_t IMM(){

    cpu.addr_abs = cpu.PC++;
    return 0;

}

// Relative
uint8_t REL(){

    cpu.addr_rel = cpu_read(cpu.PC);
    cpu.PC++;
    if(cpu.addr_rel & 0x80){
        cpu.addr_rel |= 0xFF00;
    }
    return 0;

}

// Zero Page
uint8_t ZP0(){

    cpu.addr_abs = cpu_read(cpu.PC);
    cpu.PC++;
    cpu.addr_abs &= 0x00FF;
    return 0;

}

// Zero Page X
uint8_t ZPX(){

    cpu.addr_abs = cpu_read(cpu.PC) + cpu.X;
    cpu.PC++;
    cpu.addr_abs &= 0x00FF;
    return 0;

}

// Zero Page Y
uint8_t ZPY(){

    cpu.addr_abs = cpu_read(cpu.PC) + cpu.Y;
    cpu.PC++;
    cpu.addr_abs &= 0x00FF;
    return 0;

}

// Absoulte
uint8_t ABS(){

    uint16_t lo = cpu_read(cpu.PC);
	cpu.PC++;
	uint16_t hi = cpu_read(cpu.PC);
	cpu.PC++;

	cpu.addr_abs = (hi << 8) | lo;

	return 0;

}

// Absolute with X offset
uint8_t ABX(){

    uint16_t lo = cpu_read(cpu.PC);
	cpu.PC++;
	uint16_t hi = cpu_read(cpu.PC);
	cpu.PC++;

	cpu.addr_abs = (hi << 8) | lo;
    cpu.addr_abs += cpu.X;
    if ((cpu.addr_abs & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;	
	
}

// Absolute with Y offset
uint8_t ABY(){

    uint16_t lo = cpu_read(cpu.PC);
	cpu.PC++;
	uint16_t hi = cpu_read(cpu.PC);
	cpu.PC++;

	cpu.addr_abs = (hi << 8) | lo;
    cpu.addr_abs += cpu.Y;
    if ((cpu.addr_abs & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;

}

// Indirect
uint8_t IND(){

    uint16_t ptr_lo = cpu_read(cpu.PC);
	cpu.PC++;
	uint16_t ptr_hi = cpu_read(cpu.PC);
	cpu.PC++;

	uint16_t ptr = (ptr_hi << 8) | ptr_lo;

	if (ptr_lo == 0x00FF) // Simulate page boundary hardware bug
	{
		cpu.addr_abs = (cpu_read(ptr & 0xFF00) << 8) | cpu_read(ptr + 0);
	}
	else // Behave normally
	{
		cpu.addr_abs = (cpu_read(ptr + 1) << 8) | cpu_read(ptr + 0);
	}
	
	return 0;

}

// Indirect X
uint8_t IZX(){

    uint16_t t = cpu_read(cpu.PC);
	cpu.PC++;

	uint16_t lo = cpu_read((uint16_t)(t + (uint16_t)cpu.X) & 0x00FF);
	uint16_t hi = cpu_read((uint16_t)(t + (uint16_t)cpu.X + 1) & 0x00FF);

	cpu.addr_abs = (hi << 8) | lo;
	
	return 0;

}

// Indirect Y
uint8_t IZY(){

    uint16_t t = cpu_read(cpu.PC);
	cpu.PC++;

	uint16_t lo = cpu_read(t & 0x00FF);
	uint16_t hi = cpu_read((t + 1) & 0x00FF);

	cpu.addr_abs = (hi << 8) | lo;
	cpu.addr_abs += cpu.Y;
	
	if ((cpu.addr_abs & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;

}

// Implied
uint8_t IMP(){
    cpu.fetched = cpu.A;
    return 0;
}

uint8_t fetch()
{
	if (lookup[cpu.opcode].addressingMode != &IMP)
		cpu.fetched = cpu_read(cpu.addr_abs);
	return cpu.fetched;
}