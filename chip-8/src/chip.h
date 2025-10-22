#ifndef CHIP_H
#define CHIP_H

#include <stdint.h>

static uint16_t display[64*32];

static uint16_t predefined_sprites[1][64*32] =
{
	{'*','*','*','*','*','*','*','*', [8 ... 64] = ' ', 
	 '*',' ',' ',' ',' ',' ',' ','*', [73 ... 129] = ' ',
	 '*',' ',' ',' ',' ',' ',' ','*', [136 ... 191] = ' ',
	 '*',' ',' ',' ',' ',' ',' ','*', [200 ... 264] = ' ',
	 '*','*','*','*','*','*','*','*', [272 ... 327] = ' ', 
	 [328 ... 2047] = ' '
	}
};

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

void updateDisplay()
{
	for(int i = 0; i <= 64 * 32; i++)
	{
		if (i % 64 == 0)
		{
			printf("_");
		} else
		{
			printf("%c", predefined_sprites[0][i]);
		}
	}

}


#endif
