#include <stdio.h>
#include <assert.h>

#include "../src/chip.h"


int main(int argc, char* argv[])
{
	
	int idx = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
				idx = getDataRAMAddr(i, j, k); 
		}
	}
}
