#include <stdio.h>
#include <assert.h>

#include "../src/chip.h"



int main(int argc, char* argv[])
{

//	SBM
//	FLOW OF SBM
//		addr = chip.RP[chip.SRCADDRREG];
//		chr = (addr & 0b00001111);
//		r = (addr >> 4) & (0b00000011);
//		c = (addr >> 6) & (0b00000011);
//		b = chip.DATARAMSELECTED;
//		uint16_t val = getDataRamCharacter(b,c,r,chr);
//		subFromAccumulator(val);
//		programcounter += 1;
	
	chip.ACCUMULATOR = 0b0111; // 5
	chip.CARRYBIT = 1;
	
	setRamRegisterCharacter(0,0,0,0,0b0101); 
	assert(chip.RAM[0] == 0b0101);
	
	uint16_t val = getDataRamCharacter(0,0,0,0);
	assert(val == 0b0101);

	subFromAccumulator(val);
	assert(chip.ACCUMULATOR == 0b0001);
	assert(chip.CARRYBIT == 0);

	chip.ACCUMULATOR = 0b0011; // 3
	chip.CARRYBIT = 0;
	assert(chip.RAM[0] == 0b0101);
	subFromAccumulator(val);
	assert(chip.ACCUMULATOR == 0b1110);
	assert(chip.CARRYBIT == 0);
	
}
