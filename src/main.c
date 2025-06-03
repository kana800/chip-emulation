#include <stdio.h>

#include "chip.h"

static struct t_chip chip; 

int main(int argc, char* argv[])
{
	printf("%d", chip.RP[0]);
}
