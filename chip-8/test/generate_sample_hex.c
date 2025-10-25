// generate sample hex code for
// testing purposes
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define DEBUG 1

int main(int argc, char* argv[])
{

	if (argc != 2)
	{
		printf("usage: genhex <opcode>\n\tgenhex 0 -> generate SYS addr (0x0nnn)\n");
//		printf(
//		    "\t0x0NNN -> SYS addr\n"
//		    "\t0x1NNN -> JP addr\n"
//		    "\t0x2NNN -> CALL addr\n"
//		    "\t0x3XKK -> SE Vx, byte\n"
//		    "\t0x4XKK -> SNE Vx, byte\n"
//		    "\t0x5XY0 -> SE Vx, Vy\n"
//		    "\t0x6XKK -> LD Vx, byte\n"
//		    "\t0x7XKK -> ADD Vx, byte\n"
//		    "\t0x8XY0 -> LD Vx, Vy\n"
//		    "\t0x8XY1 -> OR Vx, Vy\n"
//		    "\t0x8XY2 -> AND Vx, Vy\n"
//		    "\t0x8XY3 -> XOR Vx, Vy\n"
//		    "\t0x8XY4 -> ADD Vx, Vy\n"
//		    "\t0x8XY5 -> SUB Vx, Vy\n"
//		    "\t0x8XY6 -> SHR Vx {, Vy}\n"
//		    "\t0x8XY7 -> SUBN Vx, Vy\n"
//		    "\t0x8XYE -> SHL Vx {, Vy}\n"
//		    "\t0x9XY0 -> SNE Vx, Vy\n"
//		    "\t0xANNN -> LD I, addr\n"
//		    "\t0xANNN -> LD I, addr\n"
//		    "\t0xBNNN -> JP V0, addr\n"
//		    "\t0xCXKK -> RND Vx, byte\n"
//		    "\t0xDXYN -> DRW Vx, Vy, nibble\n"
//		    "\t0xEX9E -> SKP Vx\n"
//		    "\t0xEXA1 -> SKNP Vx\n"
//		    "\t0xFX07 -> LD Vx, DT\n"
//		    "\t0xFX0A -> LD Vx, K\n"
//		    "\t0xFX15 -> LD DT, Vx\n"
//		    "\t0xFX18 -> LD ST, Vx\n"
//		    "\t0xFX1E -> ADD I, Vx\n"
//		    "\t0xFX29 -> LD F, Vx\n"
//		    "\t0xFX33 -> LD B, Vx\n"
//		    "\t0xFX55 -> LD [I], Vx\n"
//		    "\t0xFX65 -> LD Vx, [I]\n"
//		);
		return -1;
	}

	int opcode = strtol(argv[1], NULL, 16);
	uint8_t x, y, n;
	// nnn or addr - A 12-bit value, the lowest 12 bits of the instruction  
	// n or nibble - A 4-bit value, the lowest 4 bits of the instruction  
	// x - A 4-bit value, the lower 4 bits of the high byte of the instruction  
	// y - A 4-bit value, the upper 4 bits of the low byte of the instruction  
	// kk or byte - An 8-bit value, the lowest 8 bits of the instruction

	switch (opcode)
	{
		case 0x0: // opcode 0nnn
		case 0x1: // opcode 1nnn
		case 0x2: // opcode 2nnn
		case 0x3: // opcode 3xkk
		case 0x4: // opcode 4xkk
		case 0x6: // opcode 6xkk
		case 0x7: // opcode 7xkk
		case 0xc: // opcode cxkk
		case 0x8:
			for (int i = 0; i < 4096; i++)
			{
				printf("%X%03X",opcode,i);
			}
			break;
	}
}
