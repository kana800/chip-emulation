#include <stdio.h> 
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "instructions.h"

#include <stdlib.h>

#define LINE 256

struct t_label 
{
	char name[10];
	int listing;
};

struct t_infoblock
{
	char line[50];
	uint8_t isdata:1;
//	union {
//		uint8_t hex;
//		uint16_t hex; // includes data
//	};
	uint8_t hex;
	uint8_t data;
	// storing label information
	uint8_t s_size:2;
	char s_data[2][15];
	uint8_t bytesread;
};


// EXPERIMENT: labelarray[0] is
// used to track the total count
// of labels in the array;
static struct t_label labelarray[50];

static struct t_infoblock listingarray[LINE];

static uint8_t lacount;
static unsigned int line;

enum e_errortypes {
	REG_OUT_OF_BOUNDS,
	REG_P_OUT_OF_BOUNDS,
	INVALID_LABEL_FIELD,
	INVALID_OPERAND_TYPE,
	INVALID_OPERATION
};

static void handleInstructionError(char* buffer, int errcode)
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
		case 2:
			fprintf(stderr, 
				"INVALID LABEL FIELD @%d:%s\n", line, buffer);
			break;
		case 3:
			fprintf(stderr,
				"INVALID OPERAND TYPE LINE@%d:%s\n", line, buffer);
			break;
		case 4:
			fprintf(stderr,
				"INVALID OPERATION LINE %d : %s\n", line, buffer);
			break;
	}
}


// NOTE: 
// 1. first characters of a label must be a letter
// of the alphabet 
// 2. the remaining characters can be letter or 
// a decimal number
// 3. labels maybe any length, but should have
// unique first three characters
static uint8_t isLabel(char* buffer, uint8_t len, int line)
{
	uint8_t res = 0;
	if (isdigit(buffer[0]))
	{
		handleInstructionError(buffer, 2);
		return res;
	}

//	for (int i = 0; i < len; i++)
//	{
//		if (isdigit(buffer[i])) 
//		{
//			res = 1;
//			break;
//		}
//	}
	return res;
}


enum e_operandtype {
	invalid_type,
	integer,
	register_pair,
	special_character,
	label,
	arithmetic
};

static uint8_t operandType(
	char* buffer, uint8_t len, uint8_t paircount)
{
	// operand types can be
	// 0 -> invalid type
	// 1 -> int
	// 2 -> register pair
	// 3 -> special character '*'
	// 4 -> label (label existence not checked)
	// 5 -> arithmetic
	
	if (isdigit(buffer[0]))
	{
		// checking for register pair
		if ((buffer[1] == 'P') && (len == 2)) return register_pair;
		// checking for numbers 
		// and arithmetic operations
		for (int i = 0; i < len; i++)
		{
			char temp_c = buffer[i];
			switch (temp_c)
			{
				case '-':
				case '+':
					return arithmetic;
				default:
					if (isalpha(temp_c))
					{
						handleInstructionError(buffer, 3);
						return invalid_type;
					}
			}
		}
		return 1;
	}
	// check for special character
	if (buffer[0] == '*') return special_character;
	return label;
}


static uint8_t parseInstruction(char* buffer, int line)
{
	char temp_code[5];
	char temp_operand_1[15];

	char temp_operand[2][15];
	uint8_t opt_res[2] = {0,0};

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
//	printf("\t OPCODE '%s'\n",temp_code);

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

	int paircount = 0;
	int pairlength = 0;
	int temp_lacount = lacount;

	if (buffersize > 3)
	{
		memcpy(temp_operand_1, buffer + code_cpy_size + 1, buffersize + 1);
		temp_operand_1[buffersize + 1] = '\0';
		char* operand_pair = strtok(temp_operand_1, " ");
		while (operand_pair != NULL) {
			pairlength = strlen(operand_pair);
			memcpy(temp_operand[paircount], operand_pair, pairlength);
			temp_operand[paircount][pairlength] = '\0';
			operand_pair = strtok(NULL, " ");
			paircount += 1;
		}
	}

	assert(paircount <= 2);

	// two options for paircount; either one pair or two pairs
	// if one pair; there are three options
	// operand is decimal, label or special character '*'
	// if two pair
	for (int i = 0; i < paircount; i++)
	{
		opt_res[i] = operandType(temp_operand[i], 
		  strlen(temp_operand[i]), i);
		assert(opt_res[i] != 0);
	}
	
	uint8_t opa = instructions_hex[ins];
	uint8_t opr = 0x00;
	uint8_t opr2 = 0x00;
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
			opr = atoi(temp_operand[0]);
			if ((opr < 0) || (opr > 15))
			{
				handleInstructionError(buffer, 0);
				break;
			}
			hins = opa | opr; 
			listingarray[lacount].hex = hins;
			lacount += 1;
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
		case 22:  // "NOP"  
			listingarray[lacount].hex = opa;
			lacount += 1;
			break;
		case 11:  // "FIN"  
		case 16:  // "JIN"  
		case 32:  // "SRC"  
			opr = atoi(temp_operand[0]);
			if ((opr < 0) || (opr > 7))
			{
				handleInstructionError(buffer, 1);
				break;
			}
			hins = opa | opr << 1;
			listingarray[lacount].hex = hins;
			lacount += 1;
			break;
		case 10:  // "FIM"
			if ((paircount != 2) || 
				(opt_res[0] >= label) ||
				(opt_res[1] == register_pair))
			{
				handleInstructionError(buffer, INVALID_OPERATION);
				break;
			}

			uint8_t regpair = atoi(temp_operand[0]);
			if ((regpair > 14) && (regpair < 0))
			{
				handleInstructionError(buffer, INVALID_OPERATION);
				assert((regpair > 14) && (regpair < 0));
			}

			if (opt_res[0] == integer) regpair / 2; 
			hins = opa | regpair << 1;
			listingarray[lacount].hex = hins;
			lacount += 1;

			switch (opt_res[1])
			{
				case label:
					strcpy(listingarray[lacount].s_data[1], 
						temp_operand[1]);
					lacount += 1;
					break;
				case arithmetic:
					break;
				case integer:
					uint8_t opr_pair = atoi(temp_operand[1]);
					listingarray[lacount].hex = opr_pair;
					lacount += 1;
					break;
			}
			break;
		case 14:  // "ISZ"
			// valid operands are
			// REG ADDR
			// REG can be register_pair || integer
			// ADDR can be integer || label
			// i am sorry
			if (((opt_res[0] != register_pair) 
				&& (opt_res[0] != integer)) 
				|| ((opt_res[1] != integer) 
				&& (opt_res[1] != label))
			)
			{
				handleInstructionError(buffer, INVALID_OPERATION);
				break;
			} 

			uint8_t reg_pair = atoi(temp_operand[0]);
			hins = opa | reg_pair;
			listingarray[lacount].hex = hins;
			lacount += 1;

			switch (opt_res[1])
			{
				case label:
					strcpy(listingarray[lacount].s_data[1], 
						temp_operand[1]);
					lacount += 1;
					break;
				case integer:
					uint8_t opr_pair = atoi(temp_operand[1]);
					listingarray[lacount].hex = opr_pair;
					lacount += 1;
					break;
				default:
					handleInstructionError(buffer, 
							INVALID_OPERATION);
					break;
			}
			break;
		case 15:  // "JCN"
			if ((paircount != 2) || (opt_res[0] != integer) && 
				((opt_res[1] != integer) || (opt_res[1] != label)))
			{
				handleInstructionError(buffer, INVALID_OPERATION);
				break;
			}

			uint8_t condition = atoi(temp_operand[0]);
			hins = opa | condition;
			listingarray[lacount].hex = hins;
			lacount += 1;

			switch (opt_res[1])
			{
				case label:
					strcpy(listingarray[lacount].s_data[1], 
						temp_operand[1]);
					lacount += 1;
					break;
				case integer:
					uint8_t opr_pair = atoi(temp_operand[1]);
					listingarray[lacount].hex = opr_pair;
					lacount += 1;
					break;
				default:
					handleInstructionError(buffer, 
							INVALID_OPERATION);
					break;
			}
			listingarray[lacount].bytesread = 2;
			break;
		case 17:  // "JMS"  
		case 18:  // "JUN"  
			hins = opa;
	
			if (((opt_res[0] != integer) || (opt_res[0] != label)) 
				&& (paircount != 1))
			{
				handleInstructionError(buffer, 
						INVALID_OPERATION);
				break;
			}

			listingarray[lacount].hex = hins;
		
			switch (opt_res[0])
			{
				case label:
					listingarray[lacount].isdata = 1;
					listingarray[lacount].data = 0xFF;
					listingarray[lacount].s_size = paircount;
					listingarray[lacount].hex = hins;
					strcpy(listingarray[lacount].s_data[0], 
						temp_operand[0]);
					lacount += (paircount + 1);
					break;
				case integer:
					int opr_pair = atoi(temp_operand[0]);
					if (opr_pair > 256)
					listingarray[lacount].hex = hins | opr_pair >> 4;
					lacount += (paircount + 1);
					break;
				default:
					handleInstructionError(buffer, 
							INVALID_OPERATION);
					break;
			}
			listingarray[lacount].bytesread = 2;
			break;	
	}

	return lacount;
}


static void parseLine(char* buffer, int line)
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
	uint8_t temp_count = lacount;
	
	// LABEL PRESENT
	if (strcmp(temp_buffer, token) != 0)
	{
		int len = strlen(token);
		memcpy(temp_label,token,len);
		temp_label[len] = '\0';
		token = strtok(NULL, delim_one);
		uint8_t label_res = isLabel(temp_label, len, line);
//		assert(label_res == 1);
		label_present = 1;
		// copying the 'label name' to the
		// label array
		int count = labelarray[0].listing;
		memcpy(&labelarray[count].name,temp_label, len);
		labelarray[count].name[len] = '\0';
		labelarray[count].listing = lacount;
		labelarray[0].listing += 1;
	}

	// CHECKING IF WE HAVE COMMENTS
	const char* delim_two = "/";
	char* token_code = strtok(token,delim_two);
	int j = 0;
	short firstspace = 0;

	int sp = 0, ep = strlen(token_code) - 1;
	while (isspace(token_code[sp])) sp++;
	while (isspace(token_code[ep])) ep--;

	// SANITIZE THE STRING;
	for (int i = sp; i <= ep; i++)
	{
		switch (token_code[i])
		{
			case ' ':
			case '\t':
				if (firstspace == 1)
				{
					temp_token[j] = ' ';
					j++;
					firstspace = 0;
				}
				break;
			default:
				temp_token[j] = token_code[i];
				firstspace = 1;
				j++;
		}
	}
	temp_token[j] = '\0';

	if (j > 0)
	{
		int parsedhex = parseInstruction(temp_token, line);
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


	while (fgets(buffer, len, fptr))
	{
		parseLine(buffer, line);
		line++;
	}
	
	fclose(fptr);

	int labelcount = labelarray[0].listing;
	for (int i = 1; i < labelcount; i++)
	{
		struct t_label temp_label = labelarray[i];
		printf("label array %d %s\n", temp_label.listing, temp_label.name);
	}
	

	for (int i = 0; i < lacount; i++)
	{
		struct t_infoblock tempblock = listingarray[i];
		if (tempblock.isdata == 1)
		{
			printf("blockid %d: %X %X", i, 
				tempblock.hex, tempblock.data, tempblock.s_data[0]);
			for (int i = 1; i < labelcount; i++)
			{
				struct t_label temp_label = labelarray[i];
				if (strcmp(temp_label.name, 
					tempblock.s_data[0]) == 0)
				{
					printf(" %X\n", i);
				}

				if ((tempblock.s_size == 2) && 
				(strcmp(temp_label.name, tempblock.s_data[1]) == 0))
				{
					printf("matching block %s == %s\n", labelarray[i].name, tempblock.s_data[1]);
				}
			}
		} 
		else
		{
			printf("blockid %d: %X\n",i, tempblock.hex);
		}
	}





}
