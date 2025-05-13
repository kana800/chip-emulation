#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "instructions.h"

#include <stdlib.h>

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

void handleInstructionError(char* buffer, int line, int errcode)
{
	switch (errcode)
	{
		case 0: // REGISTER OUT OF BOUNDS
			fprintf(stderr, 
				"REGISTER OUT OF BOUNDS @%d:%s\n", line, buffer);
			break;
		case 1: // REGISTER PAIRS OUT OF BOUNDS
			fprintf(stderr, 
				"REGISTER PAIRS OUT OF BOUNDS @%d:%s\n", line, buffer);
			break;
	}
}

void parseInstruction(char* buffer, int line)
{
//	printf("%d: token %s\n", line, buffer);
	char temp_code[5];
	char temp_operand[15];
	int buffersize = strlen(buffer);
	int code_cpy_size = 3;
	// CODE OPERAND
	// CODE contains three letter; 
	// **LD is two letters
	
	if ((buffer[0] == 'L') && 
		(buffer[1] == 'D') && 
		(buffer[2] != 'M'))
	{
		code_cpy_size = 2;
	}

	memcpy(temp_code, buffer, code_cpy_size);
	temp_code[code_cpy_size] = '\0';

	if (buffersize > 3)
	{
		memcpy(temp_operand, buffer + code_cpy_size, buffersize + 1);
		temp_operand[buffersize + 1] = '\0';

	}
	
	// THIS IS NOT EFFICIENT; 
	// I WILL USE HASHMAPS LATER
	int ins = -1;
	for (int i = 0; i < INS_SIZE; i++)
	{
		if (strcmp(temp_code, instructions[i]) == 0)
		{
			ins = i;
			break;
		}
	}

//	printf("\tline: %d parsed OPCODE %s %s %x %d\n",
//		line, temp_code, instructions[ins], 
//		instructions_hex[ins], instructions_hex[ins]);
//
//	printf("\tline: %d parsed OPERAND %s\n",
//		line, temp_operand);

	uint8_t opa = instructions_hex[ins];
	uint8_t opr = 0x00;
	uint8_t hins;

	switch (ins)
	{
		case 0: // "ADD" OPR -> 1000 OPA -> RRRR | LABEL, OP, REG
		case 2: // "BBL"
		case 13:  // "INC"  
		case 20:  // "LDM"  
		case 21:  // "LD"   
		case 34:  // "SUB"  
		case 45:  // "XCH"
			opr = atoi(temp_operand);
			if ((opr < 0) || (opr > 15))
			{
				handleInstructionError(buffer, line, 0);
				break;
			}
			hins = opa | opr; 
			printf("%X ", hins); 
			break;
		case 1: // "ADM"
		case 3: // "CLB"  
		case 4: // "CLC"  
		case 5: // "CMC"  
		case 6: // "CMA"  
		case 7: // "DAA"  
		case 8: // "DAC"  
		case 9: // "DCL"  
		case 12:  // "IAC"  
		case 19:  // "KBP"  
		case 23:  // "RAL"  
		case 24:  // "RAR"  
		case 25:  // "RD0"  
		case 26:  // "RD1"  
		case 27:  // "RD2"  
		case 28:  // "RD3"  
		case 29:  // "RDM"  
		case 30:  // "RDR"  
		case 31:  // "SBM"  
		case 33:  // "STC"  
		case 35:  // "TCC"  
		case 36:  // "TCS"  
		case 37:  // "WR0"  
		case 38:  // "WR1"  
		case 39:  // "WR2"  
		case 40:  // "WR3"  
		case 41:  // "WMP"  
		case 42:  // "WPM"  
		case 43:  // "WRM"  
		case 44:  // "WRR"  
			printf("%X ", instructions_hex[ins]);
			break;
		case 11:  // "FIN"  
		case 16:  // "JIN"  
		case 32:  // "SRC"  
			opr = atoi(temp_operand);
			if ((opr < 0) || (opr > 7))
			{
				handleInstructionError(buffer, line, 1);
				break;
			}
			hins = opa | opr << 1;
			printf("%X ", hins);
			break;
		case 10:  // "FIM"  
		case 14:  // "ISZ"  
		case 15:  // "JCN"  
		case 17:  // "JMS"  
		case 18:  // "JUN"  
		case 22:  // "NOP"  
			printf("\nline: %d parsed OPCODE %s %s %x %d\n",
				line, temp_code, instructions[ins], 
				instructions_hex[ins], instructions_hex[ins]);
			break;
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
//			printf("%d: label: %s token: %s\n", 
//				line, temp_label, temp_token);
		}
		else
		{
//			printf("%d: token: %s\n", 
//				line, temp_token);
		}
		parseInstruction(temp_token, line);
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
	for (int i = 1; i < labelcount; i++)
	{
		struct t_label temp_label = labelarray[i];
//		printf("label array %d %s\n", temp_label.listing, temp_label.name);
	}


	fclose(fptr);
}
