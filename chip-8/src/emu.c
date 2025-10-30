#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <ncurses.h>

#include "chip.h"

#define DEBUG

static struct chip chip8;


void displayChipRegInfo()
{
	for (int i = 0; i < 16; i++)
	{
		printf("V[%d]  = %03X\n", i , chip8.V[i]);
	}
}

void updateDisplay(uint8_t sprite)
{
	assert(sprite < 16);
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

void clearDisplay()
{
	return;
}

void loadIndexRegister(uint16_t addr)
{
	assert(addr < 4096);
	chip8.I = addr;
}

void loadDataToRegister(uint8_t reg, uint8_t value)
{
	assert(reg < 16);
	assert(addr < 4096);
	chip8.V[reg] = addr;
}

void addToRegister(uint8_t reg, uint8_t value)
{
	assert(reg < 16);
	assert(addr < 4096);
	chip8.V[reg] += addr;
}

void subFromRegister(uint8_t reg, uint8_t value)
{
	assert(reg < 16);
	assert(addr < 4096);
	chip8.V[reg] -= addr;
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

	void (*funcmem[512])();

	// copy the memory into the chip ROM
	printf("size of the loaded program %ld bytes\n", bytesread_f);
	memcpy(chip8.memory + 512, buffer, bytesread_f);

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

//	initscr();

//	updateDisplay(0);

//	for (int i = 0; i < 16; i++)
//	{
//		clear();
//		updateDisplay(i);
//		refresh();
//		getch();
//	}

	int inscount = 0; // keep a record of elements in the funcmem array

	// dis-assembling the rom; i want to do something
	// experimental, where i make a function array
	// and the function calls will be pushed to the array
	// and executed all the once, instead of decoding and
	// executing
	for (int i = 512; i < (bytesread_f + 512); i += 2)
	{

		memcpy(hexval, chip8.memory + i, sizeof(uint8_t) * 2);
		hex_val = hexval[0] << 8 | hexval[1];
		opcode = hexval[0] >> 4;
#ifdef DEBUG
		printf("mem[%d] -> %x %x = %x %x = %X opcode %x\n",
			i, chip8.memory[i], 
			chip8.memory[i + 1], 
			hexval[0], hexval[1], hex_val, opcode);
#endif
		switch (opcode)
		{
			case 0x0:
				switch (hex_val)
				{
					case 0x00E0: // CLS; clear the display
						printf("CLS\n");
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
				break;
			case 0x2:
				tempblock = hex_val & 0x0FFF;
				printf("CALL %x\n",tempblock);
				break;
			case 0x3:
				tempblock = hex_val & 0x0FFF;
				x = (tempblock & 0x0F00) >> 8;
				kk = tempblock & 0x00FF;
				printf("SE V%x, %x\n", x, kk);
				break;
			case 0x4:
				tempblock = hex_val & 0x0FFF;
				x = (tempblock & 0x0F00) >> 8;
				kk = tempblock & 0x00FF;
				printf("SNE V%x, %x\n", x, kk);
				break;
			case 0x5:
				tempblock = hex_val & 0x0FFF;
				x = (tempblock & 0x00F0) >> 4;
				y = tempblock & 0x000F;
				printf("SE V%x, V%x\n", x, y);
				break;
			case 0x6:
				tempblock = hex_val & 0x0FFF;
				x = hexval[0] & 0b00001111;
				kk = hexval[1];
				printf("LD V%x, %x\n", x, kk);
				loadDataToRegister(x, kk);
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
						chip8.V[x] = chip8.V[y];
						printf("LD V%x,V%x\n",x,y);
						break;
					case 0x1:
						// bit wise OR; Vx = Vx | Vy
						chip8.V[x] |= chip8.V[y];
						printf("OR V%x,V%x\n",x,y);
						break;
					case 0x2:
						// bit wise AND; Vx = Vx & Vy
						chip8.V[x] &= chip8.V[y];
						printf("AND V%x,V%x\n",x,y);
						break;
					case 0x3:
						// bit wise XOR; Vx = Vx ^ Vy
						printf("XOR V%x,V%x\n",x,y);
						chip8.V[x] ^= chip8.V[y];
						break;
					case 0x4:
						printf("ADD V%x,V%x\n",x,y);
						temp = chip8.V[x] + chip8.V[y];
						if (chip8.V[x] > 255) 
							chip8.V[15] = 0x01;
						chip8.V[x] = temp;
						break;
					case 0x5:
						printf("SUB V%x,V%x\n",x,y);
						temp = chip8.V[x] + chip8.V[y];
						if (chip8.V[x] < 0) 
							chip8.V[15] = 0x01;
						chip8.V[x] = temp;
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
		printf("mem_r\n");
		displayChipRegInfo();
	}

//	endwin();

	return 0;
}
