#include <stdio.h>

#include "chip.h"


int main(int argc, char* argv[])
{
	struct t_chip* chip = initChip();
	printRegister(chip);
	free(chip);
	return 0;
}
