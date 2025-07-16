// this is kinda silly to do this
// test but doing it just in case
// also note that the accumulator
// reset on the next cycle after 
// carry bit is set to 1 
#include <stdio.h>
#include <assert.h>

#include "../src/chip.h"

int main(int argc, char* argv[])
{
	fprintf(stdout,"initial state carry:%d accumulator:%d\n", 
		chip.CARRYBIT, chip.ACCUMULATOR);
	assert ( chip.ACCUMULATOR == 0b0000 );
//	addToAccumulator(0b0001);
//	assert ( chip.ACCUMULATOR == 0b0001 );
//	assert ( chip.CARRYBIT == 0);
	for (int i = 0; i < 30; i++)
	{
		addToAccumulator(0b0001);
		fprintf(stdout,"(%d) state carry:%d accumulator:%d\n", 
			i + 1, chip.CARRYBIT, chip.ACCUMULATOR);
	}

//	assert ( chip.ACCUMULATOR == 0b0000 );
//	assert ( chip.CARRYBIT == 1);
//	addToAccumulator(0b0001);	
//	assert ( chip.ACCUMULATOR == 0b0001 );
//	assert ( chip.CARRYBIT == 0);
	
	printf("before:\n\tcarry:%d accumulator:%d sub:%d\n", 
		chip.CARRYBIT, chip.ACCUMULATOR, 0b0110);
	chip.ACCUMULATOR = 0b1010;
	subFromAccumulator(0b0110);
	assert(chip.ACCUMULATOR == 0b0100);
	assert(chip.CARRYBIT == 0b0001);
	printf("after:\n\tcarry:%d accumulator:%d\n", chip.CARRYBIT, chip.ACCUMULATOR);

	printf("before:\n\tcarry:%d accumulator:%d sub:%d\n",
		chip.CARRYBIT, chip.ACCUMULATOR, 0b0110);
	chip.ACCUMULATOR = 0b1010;
	subFromAccumulator(0b0110);
	assert(chip.ACCUMULATOR == 0b0011);
	assert(chip.CARRYBIT == 0b0001);
	printf("after:\n\tcarry:%d accumulator:%d\n", chip.CARRYBIT, chip.ACCUMULATOR);


	return 0;
}
