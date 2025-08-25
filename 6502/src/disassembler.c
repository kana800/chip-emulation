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
	
	char hexval[3]; 
	uint8_t hexval_int;

	printf("file input (hex dump): %s\n", buffer);

	uint8_t sizeofbyte = 3;
	
	// NOTE:experimentation
	static char twobyte[2];
	static char fourbyte[4];

	for (int i = 0; i < len;)
	{
		memcpy(hexval, buffer + i, 2);
		hexval[2] = '\0';

		hexval_int = strtol(hexval, NULL, 16);
		const char* instruction_mode = instruction_set[hexval_int];
		const char address_mode = instruction_mode[4];
		
		char instruction[3]; 
		memcpy(instruction, instruction_mode, 3);

		uint16_t operand = 0x00;
		//printf("hex:%s int:%d ins:%s len:%d mode:%c\n", 
		//	hexval, hexval_int, 
		//	instruction, 
		//	strlen(instruction), 
		//	address_mode);

		switch (address_mode)
		{
			case 'A': //accumulator
				printf("%s", instruction);
				bytesread = 1 * sizeofbyte;
				break;
			case 'a': //absolute
				memcpy(fourbyte, buffer + i + 6, 2);
				memcpy(fourbyte + 2, buffer + i + 3, 2);
				fourbyte[4] = '\0';
				printf("%s $%s\n", instruction, fourbyte);
				bytesread = 3 * sizeofbyte;
				break;
			case 'x': //absolute, X-indexed
				memcpy(twobyte, buffer + i + 3, 2);
				printf("%s $%s,X\n", instruction, twobyte);
				bytesread = 3 * sizeofbyte;
				break;
			case 'y': //absolute, Y-indexed
				memcpy(twobyte, buffer + i + 3, 2);
				printf("%s $%s,Y\n", instruction, twobyte);
				bytesread = 3 * sizeofbyte;
				break;
			case '#': //immediate
				memcpy(twobyte, buffer + i + 3, 2);
				printf("%s #$%s\n", instruction, twobyte);
				bytesread = 2 * sizeofbyte;
				break;
			case 'i': //implied
				printf("%s\n", instruction);
				bytesread = 1 * sizeofbyte;
				break;
			case 'n': //indirect
				memcpy(fourbyte, buffer + i + 6, 2);
				memcpy(fourbyte + 2, buffer + i + 3, 2);
				fourbyte[4] = '\0';
				printf("%s ($%s)\n", instruction, fourbyte);
				bytesread = 3 * sizeofbyte;
				break;
			case 'X': //X-indexed, indirect
				memcpy(twobyte, buffer + i + 3, 2);
				printf("%s ($%s,X)\n", instruction, twobyte);
				bytesread = 2 * sizeofbyte;
				break;
			case 'Y': //indirect, Y-indexed
				memcpy(twobyte, buffer + i + 3, 2);
				printf("%s ($%s,Y)\n", instruction, twobyte);
				bytesread = 2 * sizeofbyte;
				break;
			case 'r': //relative
				memcpy(twobyte, buffer + i + 3, 2);
				printf("%s $%s\n", instruction, twobyte);
				bytesread = 2 * sizeofbyte;
				break;
			case 'z': //zeropage
				memcpy(twobyte, buffer + i + 3, 2);
				printf("%s $%s\n", instruction, twobyte);
				bytesread = 2 * sizeofbyte;
				break;
			case 'p': //zeropage, X-indexed
				memcpy(twobyte, buffer + i + 3, 2);
				printf("%s $%s,X\n", instruction, twobyte);
				bytesread = 2 * sizeofbyte;
				break;
			case 'g': //zeropage, Y-indexed
				memcpy(twobyte, buffer + i + 3, 2);
				printf("%s $%s,Y\n", instruction, twobyte);
				bytesread = 2 * sizeofbyte;
				break;
		}
		i += bytesread;
	}
	
	return 0;
}
