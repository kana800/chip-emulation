// generate sample hex code for
// testing purposes
#include <stdio.h>
#include <stdint.h>


int main(int argc, char* argv[])
{

	uint8_t x, y, n;


	// testing opcode 0nnn
	// nnn is a 12 bit address
//	for (int i = 0; i < 4096; i++)
//	{
//		printf("%d - 0x0%x\n",i,i);
//		printf("%04X",i);
//	}

	// testing opcode Dxyn
	// x 4 bit value; lowest 4 bit of the high byte ins
	// y 4 bit value; lowest 4 bit of the low byte ins
	// n 4 bit value; lowest 4 bit of the ins
	for (int i = 0; i < 4096; i++)
	{
		printf("D%03X",i);
	}
}
