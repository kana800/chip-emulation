#include <stdio.h>
#include <assert.h>

#include "../src/chip.h"

int main(int argc, char* argv[])
{
	
	// ram index
	int idx = 0;
	// ram status index
	int stat = 0;

	// 8 ram banks
	for (int i = 0; i < 8; i++)
	{
		// 4 chips per bank
		for (int j = 0; j < 4; j++)
		{
			// 4  registers per bank
			for (int k = 0; k < 4; k++)
			{
				idx = getDataRAMAddr(i, j, k); 
				printf("Bank:%d Chip:%d Reg:%d -> ", i, j, k);
				printf("R-Index %d S-Index %d\n", idx, idx);
			}
		}
	}

	setRamRegisterCharacter(0,0,0,0,0xF);
	setRamRegisterCharacter(0,0,0,1,0xC);
	setRamRegisterCharacter(0,0,0,2,0x1);
	setRamRegisterCharacter(0,0,0,3,1);
	setRamRegisterCharacter(0,0,0,4,1);
	setRamRegisterCharacter(0,0,0,5,1);
	setRamRegisterCharacter(0,0,0,6,0xE);
	setRamRegisterCharacter(0,0,0,7,1);
	setRamRegisterCharacter(0,0,0,8,0x0);
//	setRamRegisterBit(0, 0, 0, 0, 1);
//	assert(chip.RAM[0] == 0x01);
//	setRamRegisterBit(0, 0, 0, 1, 2);
//	assert(chip.RAM[0] == 0x21);
//	setRamRegisterBit(0, 0, 0, 0, 0);
//	assert(chip.RAM[0] == 0x20);
	displayRamRegister(0,0,0);
}
