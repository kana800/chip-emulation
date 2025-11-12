#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <ncurses.h>
#include <errno.h>

#include "chip.h"

#define DEBUG

typedef struct _funcdetails_
{
	void (*func)(uint8_t, uint8_t);
	uint8_t args[2];
	uint8_t argcount;
} funcdetails;

static struct chip CHIP8;
static funcdetails FUNCMEM[512];
static int INSCOUNT = 0;

void pushToStack(uint16_t val)
{
	CHIP8.stack[CHIP8.sp] = val;
	CHIP8.sp += 1;
	return;
}

void popFromStack() { CHIP8.sp -= 1; }

void addToFuncMem(
	int ic, void* func, int ac, uint16_t args)
{
	FUNCMEM[ic].func = func;
	switch (ac)
	{
		case 1:
			FUNCMEM[ic].args[0] = args;
			FUNCMEM[ic].argcount = 1;
			break;
		case 2:
			FUNCMEM[ic].args[0] = (args & 0x00FF);
			FUNCMEM[ic].args[1] = (args & 0xFF00) >> 8;
			FUNCMEM[ic].argcount = 2;
			break;
		case 3:
			FUNCMEM[ic].args[0] = (args >> 8);
			FUNCMEM[ic].args[1] = (args & 0x00FF);
			FUNCMEM[ic].argcount = 2;
			break;
		default:
			FUNCMEM[ic].argcount = 0;
			break;
	}
	INSCOUNT++;
}

// Dxyn -> DRW Vx,Vy, nib 
// Display N-Byte Sprite @ (Vx,Vy) 
// V[16] = 1; if collisions
// NOTE: 12 bytes are packed into
// 16 bytes ( 2 x 8 bytes );
//	Vx ( 4 bits ) - 8 bytes
//	Vy ( 4 bits ) - 8 bytes
//	n  ( 4 bits ) - (1 to 16)
void updateDisplay(uint8_t r1, uint8_t r2)
{
	uint8_t vx = (r1 & 0b11110000) >> 4;
	uint8_t vy = (r1 & 0b00001111);
	uint8_t sprite = r2;

	printf("%x %x %x\n", vx, vy, sprite);

	assert(vx < 16 && vy < 16 && sprite < 16);

	for(int i = 0; i < 64 * 32; i++)
	{
		if (i % 64 == 0)
		{
			printw("\n");
		}
		printw("%c",predefined_sprites[sprite][i]);
	}
	printw("\n");
}

void clearDisplay(uint8_t a1, uint8_t a2)
{
	return;
}

void loadIndexRegister(uint8_t a1, uint8_t a2)
{
	assert(a1 < 4096);
	CHIP8.I = a1;
}

void jumpToAddr(uint8_t a1, uint8_t a2)
{
	uint16_t addr = a1 << 8 | a2;
	assert(addr < 4096);
	CHIP8.pc = addr + CHIP8.V[0];
}

void loadDataToRegister(uint8_t reg, uint8_t value)
{
	assert(reg < 16);
	assert(value < 4096);
	printf("exec %d %d\n", reg, value);
	CHIP8.V[reg] = value;
}

void addToRegister(uint8_t reg, uint8_t value)
{
	assert(reg < 16);
	assert(value < 4096);
	CHIP8.V[reg] += value;
}

void subFromRegister(uint8_t reg, uint8_t value)
{
	assert(reg < 16);
	assert(value < 4096);
	CHIP8.V[reg] -= value;
}

void bitwiseORRegister(uint8_t r1, uint8_t r2)
{
	assert(r1 < 16 && r2 < 16);
	CHIP8.V[r1] |= CHIP8.V[r2];
}

void bitwiseXORRegister(uint8_t r1, uint8_t r2)
{
	assert(r1 < 16 && r2 < 16);
	CHIP8.V[r1] ^= CHIP8.V[r2];
}

void bitwiseANDRegister(uint8_t r1, uint8_t r2)
{
	assert(r1 < 16 && r2 < 16);
	CHIP8.V[r1] &= CHIP8.V[r2];
}

void copyRegisterData(uint8_t r1, uint8_t r2)
{
	assert(r1 < 16 && r2 < 16);
	CHIP8.V[r1] = CHIP8.V[r2];
}

void addRegWithCarry(uint8_t r1, uint8_t r2)
{
	assert(r1 < 16 && r2 < 16);
	uint8_t temp = CHIP8.V[r1] + CHIP8.V[r2];
	if (CHIP8.V[r1] > 255) 
		CHIP8.V[15] = 0x01;
	CHIP8.V[r1] = temp;
}

void subRegWithCarry(uint8_t r1, uint8_t r2)
{
	assert(r1 < 16 && r2 < 16);
	if (CHIP8.V[r1] > CHIP8.V[r2])
		CHIP8.V[15] = 0x01;
	CHIP8.V[r1] -= CHIP8.V[r2];
}

void shiftLeftRegister(uint8_t r1, uint8_t r2)
{
	assert(r1 < 16 && r2 < 8);
	uint8_t temp = CHIP8.V[r1] >> r1;
	CHIP8.V[r1] = temp;
}

void shiftRightRegister(uint8_t r1, uint8_t r2)
{
	assert(r1 < 16 && r2 < 8);
	uint8_t temp = CHIP8.V[r1] << r2;
	CHIP8.V[r1] = temp;
}

// compare registers and skip instructions
// the two registers are packed
// into r1 and r2 contains the opcode
// if r2
//	- 0x5 -> skip next instruction 
//	if registers are equal
//	- 0x9 -> skip next instruction
//	if register are not equal
void compareRegisters(uint8_t r1, uint8_t r2)
{
	uint8_t reg1 = (r1 & 0b11110000) >> 4;
	uint8_t reg2 = r1 >> 4; 

	assert(reg1 < 16 && reg2 < 16);
	assert(r2 != 0x5 | r2 != 0x9);

	switch (r2)
	{
		case 0x5:
			if (CHIP8.V[reg1] == CHIP8.V[reg2]) 
			{
				CHIP8.pc += 0x02;
				return;
			}
			break;
		case 0x9:
			if (CHIP8.V[reg1] != CHIP8.V[reg2]) 
			{
				CHIP8.pc += 0x02;
				return;
			}
			break;
	}
}

// compare register value with byte provided
// registers and the opcode are packed ( lower )
// into r1 and r2 contains the address
// if r1
//	- 0x3 -> skip next instruction 
//	if register == kk
//	- 0x4 -> skip next instruction
//	if register != kk
void compareRegisterValues(uint8_t r1, uint8_t r2)
{
	uint8_t reg1 = (r1 & 0b11110000) >> 4;
	uint8_t opcode = r1 & 0b00001111;

	assert(reg1 < 16);
	assert(opcode != 0x3 | opcode != 0x4);

	switch (opcode)
	{
		case 0x3:
			if (CHIP8.V[reg1] == r2) 
			{
				CHIP8.pc += 0x02;
				return;
			}
			break;
		case 0x4:
			if (CHIP8.V[reg1] != r2) 
			{
				CHIP8.pc += 0x02;
				return;
			}
			break;
	}
}

void setRandomByte(uint8_t r1, uint8_t r2)
{
	// set V[r1] = random byte AND kk
	// TODO: random byte instead of 0x00
	CHIP8.V[r1] = 0x00 & r2;
}

// NOTE: Call Subroutine at nnn
// pushes current PC to stack then jumps
// 2nnn | CALL addr ( addr is 12 bits )
void callSubroutine(uint8_t r1, uint8_t r2)
{
	uint16_t addr = r1 << 8 | r2;
	pushToStack(addr);
	CHIP8.pc = addr;
}

// NOTE: return from subroutine; set the
// PC to top of the stack and decrement
// SP
void returnFromSubroutine(uint8_t r1, uint8_t r2)
{
	CHIP8.pc = CHIP8.stack[CHIP8.sp];
	popFromStack();
}

void reversedSubstraction(uint8_t r1, uint8_t r2)
{

}

// get or set the delay timer and sound timer;
// the mode is configured by arg r2
// r2:
//	- r2 0x01 -> set the delay timer
//	- r2 0x02 -> get the delay timer
//	- r2 0x03 -> set the sound timer
// NOTE: "get" as in loading the value to the
// register mentioned in r1
void getSetTimers(uint8_t r1, uint8_t r2)
{
	assert(r1 < 16);
	assert(r2 < 0x05);
	switch (r2)
	{
		case 0x01:
			CHIP8.delay_timer = CHIP8.V[r1];
			break;
		case 0x02:
			CHIP8.V[r1] = CHIP8.delay_timer;
			break;
		case 0x03:
			CHIP8.sound_timer = CHIP8.V[r1];
			break;
	}
	return;
}

void addToIndexRegister(uint8_t r1, uint8_t r2)
{
	assert(r1 < 16);
	CHIP8.I += CHIP8.V[r1];
}


void setFontCharacterAddress(uint8_t r1, uint8_t r2)
{

}

// pause and wait for user input
// NOTE: user input is a hex value
void pauseForKeyPress(uint8_t r1, uint8_t r2)
{
	char input[3];
	assert(r1 < 16);
	// can make this better; need to do 
	// some checking for the inputs
	fgets(input, sizeof(input), stdin);
	errno = 0;
	uint16_t hex = strtol(input, NULL, 16);
	assert(errno == EINVAL);
	// if (errno == EINVAL) {
        //        exit(EXIT_FAILURE);
        // }
	CHIP8.V[r1] = hex;
	return;
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

	if (bytesread_f > (4096 - 512))
	{
		fprintf(stderr, "ROM IS TOO LARGE\n");
		return -1;
	}

//	void (*funcmem[512])();

	// copy the memory into the chip ROM
	printf("size of the loaded program %ld bytes\n", bytesread_f);
	memcpy(CHIP8.memory + 512, buffer, bytesread_f);

	uint8_t hexval[2];
	uint16_t hex_val = 0x00;
	uint8_t opcode = 0x00;

	uint16_t tempblock = 0x00;
	uint8_t temp = 0x00;
	uint8_t kk = 0x00;
	uint8_t x = 0x00;
	uint8_t y = 0x00;
	uint8_t n = 0x00;
	uint8_t lb = 0x00;

	// dis-assembling the rom; i want to do something
	// experimental, where i make a function array
	// and the function calls will be pushed to the array
	// and executed all the once, instead of decoding and
	// executing
	for (int i = 512; i < (bytesread_f + 512); i += 2)
	{

		memcpy(hexval, CHIP8.memory + i, sizeof(uint8_t) * 2);
		hex_val = hexval[0] << 8 | hexval[1];
		opcode = hexval[0] >> 4;
#ifdef DEBUG
		printf("mem[%d] -> %x %x = %x %x = %X opcode %x\n",
			i, CHIP8.memory[i], 
			CHIP8.memory[i + 1], 
			hexval[0], hexval[1], hex_val, opcode);
#endif
		switch (opcode)
		{
			case 0x0:
				switch (hex_val)
				{
					case 0x00E0: // CLS; clear the display
						printf("CLS\n");
//						FUNCMEM[inscount].func = clearDisplay;
//						FUNCMEM[inscount].argcount = 0;
//						inscount++;
						addToFuncMem(
							INSCOUNT, 
							&clearDisplay, 0, 0);
						break;
					case 0x00EE: // RET return from subroutine
						break;
					default:
						tempblock = hex_val & 0x0FFF;
						printf("SYS %03x\n", tempblock);
						break;
				}
				break;
			case 0x1:
				tempblock = hex_val & 0x0FFF;
				printf("JP %x\n",tempblock);
				addToFuncMem(INSCOUNT, 
					&jumpToAddr, 
					2, tempblock);
				break;
			case 0x2:
				tempblock = hex_val & 0x0FFF;
				addToFuncMem(INSCOUNT, 
					&callSubroutine, 
					2, tempblock);
				printf("CALL %x\n",tempblock);
				break;
			case 0x3:
				tempblock = hex_val & 0x0FFF;
				x = (tempblock & 0x0F00) >> 8;
				kk = tempblock & 0x00FF;
				printf("SE V%x, %x\n", x, kk);
				addToFuncMem(INSCOUNT, 
					&compareRegisterValues, 
					x << 4 | 0x3, kk);
				break;
			case 0x4:
				tempblock = hex_val & 0x0FFF;
				x = (tempblock & 0x0F00) >> 8;
				kk = tempblock & 0x00FF;
				printf("SNE V%x, %x\n", x, kk);
				addToFuncMem(INSCOUNT, 
					&compareRegisterValues, 
					x << 4 | 0x4, kk);
				break;
			case 0x5:
				tempblock = hex_val & 0x0FFF;
				x = (tempblock & 0x00F0) >> 4;
				y = tempblock & 0x000F;
				addToFuncMem(
					INSCOUNT, &compareRegisters, 
					x << 4 | y, 0x9);
				printf("SE V%x, V%x\n", x, y);
				break;
			case 0x6:
				tempblock = hex_val & 0x0FFF;
				x = hexval[0] & 0b00001111;
				kk = hexval[1];
				printf("LD V%x, %x\n", x, kk);
				addToFuncMem(INSCOUNT, 
					loadDataToRegister, 
					2, kk << 8 | x);
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
				lb = tempblock & 0x000F;
				switch (lb)
				{
					case 0x0:
						// copies Vy into Vx
						printf("LD V%x,V%x\n",x,y);
						addToFuncMem(INSCOUNT,
							&copyRegisterData, 
							2, y << 8 | x);
						break;
					case 0x1:
						// bit wise OR; Vx = Vx | Vy
						printf("OR V%x,V%x\n",x,y);
						addToFuncMem(INSCOUNT,
							&bitwiseORRegister, 
							2, y << 8 | x);
						break;
					case 0x2:
						// bit wise AND; Vx = Vx & Vy
						printf("AND V%x,V%x\n",x,y);
						addToFuncMem(INSCOUNT,
							&bitwiseANDRegister, 
							2, y << 8 | x);
						break;
					case 0x3:
						// bit wise XOR; Vx = Vx ^ Vy
						printf("XOR V%x,V%x\n",x,y);
						addToFuncMem(INSCOUNT,
							&bitwiseXORRegister, 
							2, y << 8 | x);
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
				addToFuncMem(
					INSCOUNT, &compareRegisters, 
					x << 4 | y, 0x9);
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
			case 0xC:
				tempblock = hex_val & 0x0FFF;
				x = (tempblock & 0x00F0) >> 4;
				kk = tempblock & 0x00FF;
				printf("RND V%x, %x\n", x, kk);
				break;
			case 0xD:
				tempblock = hex_val & 0x0FFF;
				x = (tempblock & 0x0F00) >> 8;
				y = (tempblock & 0x00F0) >> 4;
				n = (tempblock & 0x000F);

				assert( x < 16 && y < 16 && n < 16);


				addToFuncMem(INSCOUNT,
					&updateDisplay, 
					3,  (x << 4 | y ) << 8 | n);
				printf("DRW V%x, V%x, %x\n", x, y, n);
				break;
			case 0xE:
				tempblock = hex_val & 0x0FFF;
				lb = tempblock & 0x00FF;
				x = (tempblock & 0xF00) >> 4;
				switch (lb)
				{
					case 0x9E:
						printf("SKP V%x\n", x);
						break;
					case 0xA1:
						printf("SKNP V%x\n", x);
						break;
				}
				break;
			case 0xF:
				x = hexval[0] & 0b00001111;
				lb = hexval[1];
				switch (lb)
				{
					case 0x07:
						printf("LD V%x,DT\n", x);
						break;
					case 0x0A:
						printf("LD V%x,K\n", x);
						break;
					case 0x15:
						printf("LD DT,V%x\n", x);
						break;
					case 0x18:
						printf("LD ST,V%x\n", x);
						break;
					case 0x1E:
						printf("ADD I,V%x\n", x);
						break;
					case 0x29:
						printf("LD F,V%x\n", x);
						break;
					case 0x33:
						printf("LD B,V%x\n", x);
						break;
					case 0x55:
						printf("LD [I],V%x\n", x);
						break;
					case 0x65:
						printf("LD V%x,[I]\n", x);
						break;
				}
				break;
		}
	}

	for (int i = 0; i < INSCOUNT; i++)
	{
		void (*f)(uint8_t, uint8_t) = FUNCMEM[i].func;
		(*f)(FUNCMEM[i].args[0], FUNCMEM[i].args[1]);
	}

//	endwin();

	return 0;
}
