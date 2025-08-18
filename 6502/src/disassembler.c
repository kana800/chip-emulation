#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "instructions.h"

#define MAXSIZE 1024

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		fprintf(stdout, "usage: disassembler <object-file>\n");
		return -1;
	}

	FILE* fptr = fopen(argv[1], "rb");
	if (fptr == NULL)
	{
		fprintf(stderr, "invalid filename '%s' \n", argv[1]);
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

	uint8_t opc = 0;
	uint8_t cycles = 0;
	uint16_t bytesread = 0;
	
	char hexval[2]; 
	uint8_t hexval_int;

	for (int i = 0; i < len; i += 3)
	{
		memcpy(hexval, buffer + i, 2);
		hexval_int = strtol(hexval, NULL, 16);
		const char* instruction = instruction_set[hexval_int];
		const char address_mode = instruction[4];
//		assert(strlen(instruction) != 5);

		printf("%s - %d %s %d %c\n", 
			hexval, hexval_int, 
			instruction, strlen(instruction), address_mode);

		switch (address_mode)
		{
			case 'A': //accumulator
				break;
			case 'a': //absolute
				break;
			case 'x': //absolute, X-indexed
				break;
			case 'y': //absolute, Y-indexed
				break;
			case '#': //immediate
				break;
			case 'i': //implied
				break;
			case 'n': //indirect
				break;
			case 'X': //X-indexed, indirect
				break;
			case 'Y': //indirect, Y-indexed
				break;
			case 'r': //relative
				break;
			case 'z': //zeropage
				break;
			case 'p': //zeropage, X-indexed
				break;
			case 'g': //zeropage, Y-indexed
				break;
		}

	}
	
	return 0;
}
