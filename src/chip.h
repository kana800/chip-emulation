#ifndef CHIP_H
#define CHIP_H

#include <stdint.h>

struct t_chip
{
	uint8_t RP[8]; // REGISTER PAIRS
	uint8_t ROM[4096]; // ROM
	uint8_t RAM[2048]; // RAM
	uint8_t RAMSTATUS[512]; // STATUS
	
	uint8_t ACCUMULATOR;
	uint8_t CARRYBIT:1;

	uint16_t STACK[3]; // stack is 3 12-bit register)
	uint8_t stackpointer:3;

	uint8_t IOPORTS; // 4 bit input (upper) or output ports (lower) 
};

#endif // CHIP_H
