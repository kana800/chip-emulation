#include <stdio.h>

#include "chip.h"

int main(int argc, char* argv[])
{

	struct chip chip8;
	chip8.I = 4095;
	printf("chippy %d", chip8.I);
	return 0;
}
