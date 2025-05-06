#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define LINE 256

struct t_label 
{
	char name[10];
	int listing;
};

// EXPERIMENT: labelarray[0] is
// used to track the total count
// of labels in the array;
static struct t_label labelarray[50];

void parseToken(char* buffer, int line)
{
	printf("%d: token %s\n", line, buffer);
	char temp_code[10];
	char temp_operand[15];

	for (int i = 0; i <= strlen(buffer); i++)
	{
		printf("\t%d -> %c\n", i, buffer[i]);
	}

	return;
}


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

	// CHECKING IF WE HAVE LABEL
	const char* delim_one = ",";
	char* token = strtok(buffer, delim_one);
	short label_present = 0;
	// LABEL PRESENT
	if (strcmp(temp_buffer, token) != 0)
	{
		int len = strlen(token);
		memcpy(temp_label,token,len);
		temp_label[len] = '\0';
		token = strtok(NULL, delim_one);
		label_present = 1;

		int count = labelarray[0].listing;
		printf("count %d\n", count);
		memcpy(&labelarray[count].name,temp_label, len);
		labelarray[count].name[len] = '\0';
		labelarray[0].listing += 1;
	}

	// CHECKING IF WE HAVE COMMENTS
	const char* delim_two = "/";
	char* token_code = strtok(token,delim_two);
	int j = 0;
	// SANITIZE THE STRING;
	for (int i = 0; i < strlen(token_code); i++)
	{
		if (!isspace(token_code[i]))
		{
			temp_token[j] = token_code[i];
			j++;
		}
	}
	temp_token[j] = '\0';
	if (j > 0)
	{
		if (label_present == 1)
		{
			printf("%d: label: %s token: %s\n", 
				line, temp_label, temp_token);
		}
		else
		{
			printf("%d: token: %s\n", 
				line, temp_token);
		}
	}
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
	
	labelarray[0].listing = 1;

	int line = 1;
	while (fgets(buffer, len, fptr))
	{
//		printf("%d %s",line,buffer);
		parseLine(buffer, line);
		line++;
	}


	int labelcount = labelarray[0].listing;
	for (int i = 1; i <= labelcount; i++)
	{
		struct t_label temp_label = labelarray[i];
		printf("label array %d %s\n", temp_label.listing, temp_label.name);
	}


	fclose(fptr);
}
