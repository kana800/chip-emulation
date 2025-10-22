#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "chip.h"


void printmnemonic(uint16_t hexval)
{
#ifdef DEBUG
	printf("CLS\n");
#endif
}

int main(int argc, char* argv[])
{

	if (argc != 2)
	{
		fprintf(stdout, "usage: chip8 <obj>\n");
		return -1;
	}

	FILE* fptr = fopen(argv[1], "rb");
	if (fptr == NULL)
	{
		fprintf(stderr, "file '%s' doesn't exist \n", argv[1]);
		return -1;
	}

	fseek(fptr, 0L, SEEK_END);
	int len = ftell(fptr);

	unsigned char* buffer = malloc(sizeof(unsigned char) * len);
	fseek(fptr, 0L, SEEK_SET);

	size_t bytesread_f = fread(buffer, sizeof(unsigned char), len, fptr);

	if (bytesread_f != len)
	{
		fprintf(stderr, 
			"short read of '%s': expected %d bytes\n", argv[1], len);
		return -1;
	}

	struct chip chip8;

	char hexval[4];

	uint16_t hex_val = 0x00;

	uint16_t tempblock = 0x00;
	uint8_t kk = 0x00;
	uint8_t x = 0x00;
	uint8_t y = 0x00;

	for (int i = 0; i < len; i += 4)
	{
		memcpy(hexval, buffer + i, 4);
		hex_val = strtol(hexval, NULL, 16);
		uint8_t opcode = hex_val >> 12;
#ifdef DEBUG
		printf("(i) %d (hexstr) %s (hexval) %x %x\n", 
			i, hexval, hex_val, opcode);
#endif
		switch (opcode)
		{
			case 0x0:
				switch (hex_val)
				{
					case 0x00E0: // CLS; clear the display
						break;
					case 0x00EE: // RET return from subroutine
						break;
				}
				break;
			case 0x1:
				tempblock = hex_val & 0x0FFF;
				printf("JP %x\n",tempblock);
				break;
			case 0x2:
				tempblock = hex_val & 0x0FFF;
				printf("CALL %x\n",tempblock);
				break;
			case 0x3:
				tempblock = hex_val & 0x0FFF;
				x = (tempblock & 0x0F00) >> 8;
				kk = tempblock & 0x00FF;
				printf("SE V%x, %x\n", x, kk);
				break;
			case 0x4:
				tempblock = hex_val & 0x0FFF;
				x = (tempblock & 0x0F00) >> 8;
				kk = tempblock & 0x00FF;
				printf("SNE V%x, %x\n", x, kk);
				break;
			case 0x5:
				tempblock = hex_val & 0x0FFF;
				x = (tempblock & 0x00F0) >> 4;
				y = tempblock & 0x000F;
				printf("SE V%x, V%x\n", x, y);
				break;
			case 0x6:
				tempblock = hex_val & 0x0FFF;
				x = (tempblock & 0x00F0) >> 4;
				kk = tempblock & 0x00FF;
				printf("LD V%x, V%x\n", x, kk);
				break;
			case 0x7:
				tempblock = hex_val & 0x0FFF;
				x = (tempblock & 0x00F0) >> 4;
				kk = tempblock & 0x00FF;
				printf("ADD V%x, V%x\n", x, kk);
				break;
			case 0x8:
				tempblock = hex_val & 0x0FFF;
				x = (tempblock & 0x0F00) >> 8;
				y = (tempblock & 0x00F0) >> 4;
				uint8_t lb = tempblock & 0x000F;
				switch (lb)
				{
					case 0x0:
						printf("LD V%x,V%x\n",x,y);
						break;
					case 0x1:
						printf("OR V%x,V%x\n",x,y);
						break;
					case 0x2:
						printf("AND V%x,V%x\n",x,y);
						break;
					case 0x3:
						printf("XOR V%x,V%x\n",x,y);
						break;
					case 0x4:
						printf("ADD V%x,V%x\n",x,y);
						break;
					case 0x5:
						printf("SUB V%x,V%x\n",x,y);
						break;
					case 0x6:
						printf("SHR V%x,{V%x}\n",x,y);
						break;
					case 0x7:
						printf("SUBN V%x,V%x\n",x,y);
						break;
					case 0xE:
						printf("SHL V%x,{V%x}\n",x,y);
						break;
				}
				break;
			case 0x9:
				tempblock = hex_val & 0x0FFF;
				x = (tempblock & 0x00F0) >> 4;
				y = tempblock & 0x000F;
				printf("SNE V%x, V%x\n", x, y);
				break;
			case 0xA:
				tempblock = hex_val & 0x0FFF;
				printf("LD I,%x\n",tempblock);
				break;
			case 0xB:
				tempblock = hex_val & 0x0FFF;
				printf("JP V0,%x\n",tempblock);
				break;
		}
	}


	return 0;
}
