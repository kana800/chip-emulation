#include <stdio.h>


// all instruction has four separate 
// distinct part or or fields: 
// LABEL CODE OPERAND COMMENT
void parseLine()
{

}


int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "error: not enough input\nusage: <assembler> .asm\n");
		return -1;
	}


	FILE* fptr = fopen(argv[1], "rb");
	if (!fptr)
	{
		fprintf(stderr, "error: invalid '%s' filepath\nusage: <assember> .asm\n", argv[1]);
		return -1;
	}

	const unsigned int len = 256;
	char buffer[len];
	
	int line = 1;
	
	while (fgets(buffer, len, fptr))
	{
		printf("%d %s",line,buffer);
		line++;
	}

	fclose(fptr);
}
