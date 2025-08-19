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
	uint8_t bytesread = 0;
	
	char hexval[2]; 
	uint8_t hexval_int;

	printf("hex %s\n", buffer);

	uint8_t sizeofbyte = 3;
	
	for (int i = 0; i < len;)
	{
		memcpy(hexval, buffer + i, 2);
		hexval_int = strtol(hexval, NULL, 16);
		const char* instruction = instruction_set[hexval_int];
		const char address_mode = instruction[4];

		printf("%s - %d %s %d %c\n", 
			hexval, hexval_int, 
			instruction, strlen(instruction), address_mode);

		switch (address_mode)
		{
			case 'A': //accumulator
				bytesread = 1;
				break;
			case 'a': //absolute
				bytesread = 3 * sizeofbyte;
				break;
			case 'x': //absolute, X-indexed
				bytesread = 3;
				break;
			case 'y': //absolute, Y-indexed
				break;
			case '#': //immediate
				bytesread = 2 * sizeofbyte;
				break;
			case 'i': //implied
				bytesread = 1;
				// byte length is 1 byte
				break;
			case 'n': //indirect
				bytesread = 3;
				break;
			case 'X': //X-indexed, indirect
				bytesread = 2;
				break;
			case 'Y': //indirect, Y-indexed
				bytesread = 2;
				break;
			case 'r': //relative
				bytesread = 2;
				break;
			case 'z': //zeropage
				bytesread = 2;
				break;
			case 'p': //zeropage, X-indexed
				bytesread = 2;
				break;
			case 'g': //zeropage, Y-indexed
				bytesread = 2;
				break;
		}

		i += bytesread;

	}
	
	return 0;
}
