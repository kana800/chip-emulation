#ifndef CHIP_H
#define CHIP_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

struct t_chip
{
	uint8_t RP[8]; // REGISTER PAIRS
	uint8_t ROM[4096]; // ROM
	uint8_t RAM[2048]; // RAM
	uint8_t RAMSTATUS[512]; // STATUS
	uint8_t ACCUMULATOR;

	uint16_t STACK[3]; // stack ( 3 -> 12-bit register )
	uint8_t stackpointer;

	uint8_t IOPORTS; // 4 bit input (upper) or output ports (lower) 
};


struct t_chip* initChip()
{
	struct t_chip* temp = 
		calloc(0, sizeof(struct t_chip));
	return temp;
}

void printChipSpec()
{
}

void printRegister(struct t_chip* chip)
{
	uint8_t upper = 0;
	uint8_t lower = 0;
	uint8_t temp  = 0;
	for (int i = 0; i < 8; i++)
	{
		temp = chip->RP[i];
		upper = temp & 0b11110000;
		lower = temp & 0b00001111;
		fprintf(stdout,"R%d %d %d\n", i, upper,lower);
	}
	return;
}

#endif // CHIP_H
