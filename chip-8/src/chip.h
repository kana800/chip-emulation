#ifndef CHIP_H
#define CHIP_H


#include <stdint.h>

struct chip {
	uint8_t V[16]; // V Register
	uint8_t memory[4096]; // Memory
	
	uint16_t I; // Address Register

	uint8_t delay_timer;
	uint8_t sound_timer;

	uint16_t stack[16];
	uint16_t sp; // stack pointer
	uint16_t pc; // program counter
};

#endif
