#ifndef CHIP_H
#define CHIP_H

#include <stdint.h>

typedef struct {
	uint8_t data:4;
} uint4_t;

struct t_chip
{
	uint8_t RP[8]; // REGISTER/PAIRS

	uint8_t ROM[4096]; // ROM
	uint16_t RAM[128]; // RAM
	uint8_t RAMSTATUS[512]; // STATUS
	
	uint8_t DATARAMSELECTED:3; 

	// stores the register pair 
	// of the last occured 'SRC'
	// instruction
	uint8_t SRCADDRREG;

	uint8_t ACCUMULATOR:4;
	uint8_t CARRYBIT:1;

	uint16_t STACK[3]; // stack is 3 12-bit register
	uint8_t stackpointer:3;

//	uint8_t IOPORTS; // 4 bit input (upper) or output ports (lower) 
	uint8_t RAM_OUTPUT[16]; // 4 bit per output per chip; 4 chips per 1 bank
	uint8_t ROM_IO; // 4 bit per output; and have 4 RAM banks
};

static struct t_chip chip; 

static void resetAccumulator()
{
	chip.ACCUMULATOR = 0;
	chip.CARRYBIT = 0;
}

static void addToRegisterPair(
	uint8_t reg, uint8_t value) 
{
	assert(8 > reg >= 0);
	chip.RP[reg] = value;
}

static uint8_t getRegisterPairValue(uint8_t reg)
{
	assert(8 > reg >= 0);
	return chip.RP[reg];
}

static uint8_t getRegisterValue(uint8_t reg)
{
	assert(16 > reg >= 0);
	uint8_t rp = reg / 2;
	// RP 5 [10 | 11] 
	if (reg % 2 == 0)
	{
		return (chip.RP[rp] & 0b11110000) >> 4;
	}
	return (chip.RP[rp] & 0b00001111);
}

static void addToAccumulator(uint8_t value)
{
	if (chip.CARRYBIT == 1) resetAccumulator();
	if (chip.ACCUMULATOR == 15) 
	{
		chip.CARRYBIT = 1;
	}
	chip.ACCUMULATOR += value;
}

static int getDataRAMAddr(uint8_t bank, 
			 uint8_t chip, uint8_t reg)
{
	int addr = ( bank * 16 ) + ( chip * 4 ) + reg;
//	printf("bank %d chip %d reg %d -> %d\n", bank, chip, reg, addr);
	return addr;
}

#endif // CHIP_H
