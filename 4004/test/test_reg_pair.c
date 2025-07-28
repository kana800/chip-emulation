#include <stdio.h>
#include <assert.h>

#include "../src/chip.h"

void printRegisterPairs()
{
	for (int i = 0; i < 16; i += 2)
	{
		printf("REG %d : %X %X\n", 
			i,
			getRegisterValue(i),
			getRegisterValue(i + 1));
	}
}

int main(int argc, char* argv[])
{
	printf("Initial State\n");
	// printRegisterPairs();
	printf("Adding 10\n");
	addToRegisterPair(5, 1000);
	addToRegisterPair(6, 10);
	printRegisterPairs();
	uint8_t retval = getRegisterPairValue(5);
	assert( 10 == retval );
	for (int i = 0; i < 16; i++)
	{
		retval = getRegisterValue(i);
	}
}
