#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

struct t_chip
{
	uint8_t RP; // REGISTER
	uint8_t ROM[4096]; // ROM
	uint8_t RAM[2048]; // RAM
	uint8_t RAMSTATUS[512]; // STATUS
	uint8_t STACK[4]; // stack
	uint8_t ACCUMULATOR;
};

#endif // MEMORY_H
