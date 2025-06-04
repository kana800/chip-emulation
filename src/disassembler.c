#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

	//const char* delim = " ";
	//char* tok = strtok(buffer, delim);
	//int i = 0;
	//while (tok != NULL)
	//{
	//	printf("%d %s\n", i, tok);
	//}
	char hexcode[2];
	for (int i = 0; i < len; i += 3)
	{
		memcpy(hexcode, buffer + i, 2);
		int hexval = strtol(hexcode, NULL, 16);
		printf("%d %s %d\n", i, hexcode, hexval);
	}

	return 0;
}
