#include <stdio.h>
#include <stdint.h>

#include "instructions.h"


int main(int argc, char* argv[])
{

	uint8_t opc;
	switch (opc)
	{

		case 0x2A: // accumulator	ROL A
		case 0x26: // zeropage		ROL operand
		case 0x36: // zeropage,X	ROL operand,X
		case 0x2E: // absolute		ROL operand
		case 0x3E: // absolute,X	ROL operand,X
			break;

		case 0x84: // zeropage		STY operand
		case 0x94: // zeropage, X
		case 0x8C: // absolute
			break;
		case 0x8A: // implied		TAX
			break;
		case 0x9A: // implied		TXS
			break;
		case 0x98: // implied		TYA
			break;
	}




	return 0;
}
