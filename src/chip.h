#ifndef CHIP_H
#define CHIP_H

#include <stdint.h>
#define DEBUG 0 

typedef struct {
	uint8_t data:4;
} uint4_t;

struct t_chip
{
	uint8_t RP[8]; // REGISTER/PAIRS

	uint8_t ROM[4096]; // ROM
	uint16_t RAM[512]; // RAM
	uint8_t RAMSTATUS[256]; // STATUS
	
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
	return addr * 4;
}

static int getDataRAMStatus(uint8_t bank, 
			 uint8_t chip, uint8_t reg)
{
	int addr = ( bank * 16 ) + ( chip * 4 ) + reg;
	return addr * 2;
}

static void displayRamRegister(uint8_t b, 
			 uint8_t c, uint8_t r)
{
	int baseaddr = getDataRAMAddr(b, c, r);
	printf("(bank %d:chip %d:reg %d)\n---\n0123456789012345\n",b,c,r);
	for (int i = 0; i < 4; i++)
	{
		if (chip.RAM[baseaddr + i] == 0x0)
		{
			printf("0000");
		} else
		{
			printf("%X",chip.RAM[baseaddr + i]);
		}
	}
	printf("\n---\n");
}

static void setRamRegisterCharacter(
	uint8_t bnk, uint8_t chp, uint8_t reg, 
	uint8_t character, uint8_t val)
{
	assert(bnk >= 0 && bnk < 8);
	assert(chp >= 0 && reg < 4);
	assert(chp >= 0 && reg < 4);
	assert(character >= 0 && character < 16);
	assert(val >= 0x0 && val <= 0xF);

	int baseaddr = getDataRAMAddr(bnk, chp, reg);
	int addr = baseaddr + ( character / 4 );
	int bit = character - (addr - baseaddr) * 4;

	uint16_t tempval = (0b0000000000000000 | val << (bit * 4));
	chip.RAM[addr] &= ~(0b0000000000000000 | 0xF << (bit * 4));
	chip.RAM[addr] |= tempval;

#if DEBUG
	printf("base %d addr %d bit %d %x\n", baseaddr, addr, bit, chip.RAM[addr]);
#endif
	return;
}

static void setRamRegisterStatusCharacter(
	uint8_t bnk, uint8_t chp, uint8_t reg, 
	uint8_t character, uint8_t val)
{
	assert(bnk >= 0 && bnk < 8);
	assert(chp >= 0 && reg < 4);
	assert(chp >= 0 && reg < 4);
	assert(character >= 0 && character < 4);
	assert(val >= 0x0 && val <= 0xF);

	int baseaddr = getDataRAMStatus(bnk, chp, reg);
	int addr = baseaddr + ( character / 4 );
	int bit = character - (addr - baseaddr) * 4;

	uint16_t tempval = (0b0000000000000000 | val << (bit * 4));
	chip.RAM[addr] &= ~(0b0000000000000000 | 0xF << (bit * 4));
	chip.RAM[addr] |= tempval;

#if DEBUG
	printf("base %d addr %d bit %d %x\n", baseaddr, addr, bit, chip.RAM[addr]);
#endif
	return;
}


#endif // CHIP_H
