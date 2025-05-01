#include <stdio.h>
#include <string.h>

#define LINE 256

void parseLine(char* buffer, int line)
{
	// NOTE: ALL instruction has four 
	// separate distinct part or fields: 
	// LABEL, CODE OPERAND /COMMENT
	
	char temp_buffer[LINE]; 
	char temp_token[LINE];

	strcpy(temp_buffer, buffer);

	// temp variables to copy
	// maybe change to a struct
	// later
	char temp_label[50];
	char temp_code[10];
	char temp_operand[15];

	// CHECKING IF WE HAVE LABEL
	const char* delim_one = ",";
	char* token = strtok(buffer, delim_one);
	short token_count = 0;
	// LABEL PRESENT
	if (strcmp(temp_buffer, token) != 0)
	{
		int len = strlen(token);
		memcpy(temp_label,token,len);
		temp_label[len] = '\0';
		token = strtok(NULL, delim_one);
		token_count++;
	}

	strcpy(temp_token, token);

	// CHECKING IF WE HAVE COMMENTS
	const char* delim_two = "/";
	char* token_code = strtok(token,delim_two);
	printf("token %s %d\n", token_code);
	return;
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

	const unsigned int len = LINE;
	char buffer[len];
	
	int line = 1;
	
	while (fgets(buffer, len, fptr))
	{
		printf("%d %s",line,buffer);
		parseLine(buffer, line);
		line++;
	}

	fclose(fptr);
}
