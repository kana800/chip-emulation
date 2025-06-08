#ifndef CHIP_H
#define CHIP_H

#include <stdint.h>

struct t_chip
{
	uint8_t RP[8]; // REGISTER PAIRS
	uint8_t ROM[4096]; // ROM
	uint8_t RAM[2048]; // RAM
	uint8_t RAMSTATUS[512]; // STATUS
	
	uint8_t ACCUMULATOR:4;
	uint8_t CARRYBIT:1;

	uint16_t STACK[3]; // stack is 3 12-bit register)
	uint8_t stackpointer:3;

	uint8_t IOPORTS; // 4 bit input (upper) or output ports (lower) 
};

static struct t_chip chip; 

static void resetAccumulator()
{
	chip.ACCUMULATOR = 0;
	chip.CARRYBIT = 0;
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

#endif // CHIP_H
