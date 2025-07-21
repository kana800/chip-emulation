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
				stat = getDataRAMStatus(i,j,k);
				printf("Bank:%d Chip:%d Reg:%d -> ", i, j, k);
				printf("R-Index %d S-Index %d\n", idx, stat);
			}
		}
	}

	// RAM DATA
	setRamRegisterCharacter(0,0,0,0,0xF);
	setRamRegisterCharacter(0,0,0,1,0xC);
	setRamRegisterCharacter(0,0,0,2,0x1);
	setRamRegisterCharacter(0,0,0,3,1);
	setRamRegisterCharacter(0,0,0,4,1);
	setRamRegisterCharacter(0,0,0,5,1);
	setRamRegisterCharacter(0,0,0,6,0xE);
	setRamRegisterCharacter(0,0,0,7,1);
	setRamRegisterCharacter(0,0,0,8,0x0);
	
	assert(chip.RAM[0] == 0x11CF);
	assert(chip.RAM[1] == 0x1E11);
	assert(chip.RAM[2] == 0x0000);
	assert(chip.RAM[4] == 0x0000);

	uint16_t temp = 0x0;
	temp = getDataRamCharacter(0,0,0,0);
	assert(temp == 0xF);
	temp = getDataRamCharacter(0,0,0,1);
	assert(temp == 0xC);
	temp = getDataRamCharacter(0,0,0,2);
	assert(temp == 0x1);
	temp = getDataRamCharacter(0,0,0,3);
	assert(temp == 0x1);
	temp = getDataRamCharacter(0,0,0,4);
	temp = getDataRamCharacter(0,0,0,5);
	temp = getDataRamCharacter(0,0,0,6);
	temp = getDataRamCharacter(0,0,0,7);
	temp = getDataRamCharacter(0,0,0,8);


	displayRamRegister(0,0,0);

	// RAM STATUS
	// bank, chip, reg, chr, val
	// REG 1 -> DBAF
	setDataRamStatusCharacter(0,0,0,0,0xF);
	setDataRamStatusCharacter(0,0,0,1,0xA);
	setDataRamStatusCharacter(0,0,0,2,0xB);
	setDataRamStatusCharacter(0,0,0,3,0xD);

	assert(chip.RAMSTATUS[0] == 0xAF);
	assert(chip.RAMSTATUS[1] == 0xDB);

	setDataRamStatusCharacter(0,0,1,0,0xF);
	setDataRamStatusCharacter(0,0,1,1,0xF);
	setDataRamStatusCharacter(0,0,1,2,0xF);
	setDataRamStatusCharacter(0,0,1,3,0xF);

	assert(chip.RAMSTATUS[2] == 0xFF);
	assert(chip.RAMSTATUS[3] == 0xFF);

	setDataRamStatusCharacter(7,3,3,0,0xF);
	setDataRamStatusCharacter(7,3,3,1,0xA);
	setDataRamStatusCharacter(7,3,3,2,0xF);
	setDataRamStatusCharacter(7,3,3,3,0xD);

	assert(chip.RAMSTATUS[254] == 0xAF);
	assert(chip.RAMSTATUS[255] == 0xDF);

	// RAM STATUS
	// bank, chip, reg, character
	assert(getDataRamStatusValue(0,0,0,1) == 0xA);
	assert(getDataRamStatusValue(0,0,0,3) == 0xD);
	assert(getDataRamStatusValue(7,3,3,2) == 0xF);
	assert(getDataRamStatusValue(4,2,3,1) == 0x0);
}
