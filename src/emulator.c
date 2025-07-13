#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "chip.h"

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		fprintf(stdout, "usage: emulator <object-file>\n");
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
			"short read of '%s': expected %d bytes\n", 
			argv[1], len);
		return -1;
	}

	char hexcode[2];
	int bytesloaded = 0;
	// loading the program into the ROM
	for (int i = 0; i < len; bytesloaded += 1, i += 3)
	{
		memcpy(hexcode, buffer + i, 2);
		int hexval = strtol(hexcode, NULL, 16);
		chip.ROM[bytesloaded] = hexval;
	}
	bytesloaded -= 1;

	uint8_t opr = 0; // operand
	int idx = 0;   // ram index
	uint8_t addr = 0; // temp 8 bit address 
	uint8_t r = 0; // register
	uint8_t c = 0; // data chip
	uint8_t b = 0; // bank
	uint8_t chr = 0; // data character
	uint16_t programcounter = 0;
	uint8_t hexval = 0; // hex value of the operand or byteread
	bool run = true;
	

	while (run) 
	{
		opr = 0;
		uint8_t accumulator = 0;
		
		if (programcounter >= bytesloaded) break;

		hexval = chip.ROM[programcounter];
		switch (hexval)
		{
			case 16: // 0x10
			case 17: // 0x11
			case 18: // 0x12
			case 19: // 0x13
			case 20: // 0x14
			case 21: // 0x15
			case 22: // 0x16
			case 23: // 0x17
			case 24: // 0x18
			case 25: // 0x19
			case 26: // 0x1A
			case 27: // 0x1B
			case 28: // 0x1C
			case 29: // 0x1D
			case 30: // 0x1E
			case 31: // 0x1F
				opr = hexval & 0b00001111;
				hexval = chip.ROM[programcounter + 1];
				printf("JCN %d\n", hexval + opr ); // CN = 15
				programcounter += 2;
				break;
			case 32: // 0x20
			case 34: // 0x22
			case 36: // 0x24
			case 38: // 0x26
			case 40: // 0x28
			case 42: // 0x2A
			case 44: // 0x2C
			case 46: // 0x2E
				opr = (hexval >> 1) & 0b00000111;
				hexval = chip.ROM[programcounter + 1];
				printf("FIM %dP %d\n", opr, hexval);
				addToRegisterPair(opr, hexval);
				programcounter += 2;
				break;
			case 33: // 0x21
			case 35: // 0x23
			case 37: // 0x25
			case 39: // 0x27
			case 41: // 0x29
			case 43: // 0x2B
			case 45: // 0x2D
			case 47: // 0x2F
				// SRC SEND REGISTER CONTROL
				// 8 bits in the register pair
				// is used by an address.
				// Address in the RP can be
				// 1. DATA RAM DATA CHARACTER
				// 2. DATA RAM STAUS CHARACTER
				// 3. RAM OUTPUT PORT
				// 4. ROM I/O PORT
				opr = (hexval >> 1) & 0b00000111; 
				printf("SRC %dP\n", opr);
				chip.SRCADDRREG = opr;
				programcounter += 1;
				break;
			case 48: // 0x30
			case 50: // 0x32
			case 52: // 0x34
			case 54: // 0x36
			case 56: // 0x38
			case 58: // 0x3A
			case 60: // 0x3C
			case 62: // 0x3E
				opr = (hexval >> 1) & 0b00000111; 
				uint16_t regval =  hexval << 8 | 
					chip.RP[0] << 4 | chip.RP[1];
				printf("FIN %dP\n", opr);
				chip.RP[opr] = buffer[regval];
				programcounter += 1;
				break;
			case 49: // 0x31
			case 51: // 0x33
			case 53: // 0x35
			case 55: // 0x37
			case 57: // 0x39
			case 59: // 0x3B
			case 61: // 0x3D
				opr = (hexval >> 1) & 0b00000111; 
				printf("JIN %dP\n", opr);
				programcounter += 1;
				break;
			case 63: // 0x3F
			case 64: // 0x40
			case 65: // 0x41
			case 66: // 0x42
			case 67: // 0x43
			case 68: // 0x44
			case 69: // 0x45
			case 70: // 0x46
			case 71: // 0x47
			case 72: // 0x48
			case 73: // 0x49
			case 74: // 0x4A
			case 75: // 0x4B
			case 76: // 0x4C
			case 77: // 0x4D
			case 78: // 0x4E
			case 79: // 0x4F
				opr = hexval & 0b00001111;
				hexval = chip.ROM[programcounter + 1];
				printf("JUN %d\n", hexval + opr ); // CN = 15
				programcounter += 2;
				break;
			case 80: // 0x50
			case 81: // 0x51
			case 82: // 0x52
			case 83: // 0x53
			case 84: // 0x54
			case 85: // 0x55
			case 86: // 0x56
			case 87: // 0x57
			case 88: // 0x58
			case 89: // 0x59
			case 90: // 0x5A
			case 91: // 0x5B
			case 92: // 0x5C
			case 93: // 0x5D
			case 94: // 0x5E
			case 95: // 0x5F
				// JMS 
				// the address of the instruction
				// immeditately following JMS is written
				// to the addres of the stack
				opr = hexval & 0b00001111;
				hexval = chip.ROM[programcounter + 1];
				printf("JMS %d\n", hexval + opr ); // CN = 15
				programcounter += 2;
				chip.STACK[chip.stackpointer] = programcounter;
				programcounter = hexval + opr;
				break;
			case 96: // 0x60
			case 97: // 0x61
			case 98: // 0x62
			case 99: // 0x63
			case 100: // 0x64
			case 101: // 0x65
			case 102: // 0x66
			case 103: // 0x67
			case 104: // 0x68
			case 105: // 0x69
			case 106: // 0x6A
			case 107: // 0x6B
			case 108: // 0x6C
			case 109: // 0x6D
			case 110: // 0x6E
			case 111: // 0x6F
				// INC INCREMENT REGISTER
				// Index register indicated by REG
				// is increment by one;
				opr = hexval & 0b00001111;
				printf("INC %d\n", opr);
				assert(15 > opr >= 0);
				chip.RP[opr] += 1;
				programcounter += 1;
				break;
			case 112: // 0x70
			case 113: // 0x71
			case 114: // 0x72
			case 115: // 0x73
			case 116: // 0x74
			case 117: // 0x75
			case 118: // 0x76
			case 119: // 0x77
			case 120: // 0x78
			case 121: // 0x79
			case 122: // 0x7A
			case 123: // 0x7B
			case 124: // 0x7C
			case 125: // 0x7D
			case 126: // 0x7E
			case 127: // 0x7F
				// ISZ INCREMENT AND SKIP IF ZERO
				// index register specified by REG is
				// incremented by one. 
				// If the result is 0000B, program execution
				// continues
				// If the result is not equal to 0000B, the
				// 8 bits specified by ADDR replace the lowest
				// 8 bits of the program counter.
				// highest 4 bits remains unchanged.
				opr = hexval & 0b00001111;
				hexval = chip.ROM[programcounter + 1];
				printf("ISZ %dP %d\n", opr, hexval);
				programcounter += 2;
				break;
			case 128: // 0x80
			case 129: // 0x81
			case 130: // 0x82
			case 131: // 0x83
			case 132: // 0x84
			case 133: // 0x85
			case 134: // 0x86
			case 135: // 0x87
			case 136: // 0x88
			case 137: // 0x89
			case 138: // 0x8A
			case 139: // 0x8B
			case 140: // 0x8C
			case 141: // 0x8D
			case 142: // 0x8E
			case 143: // 0x8F
				opr = hexval & 0b00001111;
				printf("ADD %d\n", opr);
				addToAccumulator(chip.RP[opr]);
				programcounter += 1;
				break;
			case 144: // 0x90
			case 145: // 0x91
			case 146: // 0x92
			case 147: // 0x93
			case 148: // 0x94
			case 149: // 0x95
			case 150: // 0x96
			case 151: // 0x97
			case 152: // 0x98
			case 153: // 0x99
			case 154: // 0x9A
			case 155: // 0x9B
			case 156: // 0x9C
			case 157: // 0x9D
			case 158: // 0x9E
			case 159: // 0x9F
				opr = hexval & 0b00001111;
				printf("SUB %d\n", opr);
				programcounter += 1;
				break;
			case 160: // 0xA0
			case 161: // 0xA1
			case 162: // 0xA2
			case 163: // 0xA3
			case 164: // 0xA4
			case 165: // 0xA5
			case 166: // 0xA6
			case 167: // 0xA7
			case 168: // 0xA8
			case 169: // 0xA9
			case 170: // 0xAA
			case 171: // 0xAB
			case 172: // 0xAC
			case 173: // 0xAD
			case 174: // 0xAE
			case 175: // 0xAF
				// LD LOAD ACCUMULATOR
				// The contens of the Register is
				// store into the accumulator
				opr = hexval & 0b00001111;
				chip.ACCUMULATOR = chip.RP[opr];
				programcounter += 1;
				break;
			case 176: // 0xB0
			case 177: // 0xB1
			case 178: // 0xB2
			case 179: // 0xB3
			case 180: // 0xB4
			case 181: // 0xB5
			case 182: // 0xB6
			case 183: // 0xB7
			case 184: // 0xB8
			case 185: // 0xB9
			case 186: // 0xBA
			case 187: // 0xBB
			case 188: // 0xBC
			case 189: // 0xBD
			case 190: // 0xBE
			case 191: // 0xBF
				// XCH EXCHANGE REGISTER AND
				// ACCUMULATOR
				// content of register is swapped
				// with the accumulator
				opr = hexval & 0b00001111;
				printf("XCH R%d\n", opr);
				accumulator = chip.ACCUMULATOR;
				chip.ACCUMULATOR = chip.RP[opr];
				chip.RP[opr] = accumulator;
				programcounter += 1;
				break;
			case 192: // 0xC0
			case 193: // 0xC1
			case 194: // 0xC2
			case 195: // 0xC3
			case 196: // 0xC4
			case 197: // 0xC5
			case 198: // 0xC6
			case 199: // 0xC7
			case 200: // 0xC8
			case 201: // 0xC9
			case 202: // 0xCA
			case 203: // 0xCB
			case 204: // 0xCC
			case 205: // 0xCD
			case 206: // 0xCE
			case 207: // 0xCF
				// BBL branch back and load
				printf("BBL\n");
				opr = hexval & 0b00001111;
				chip.ACCUMULATOR = opr;
				programcounter = chip.STACK[chip.stackpointer];
				break;
			case 208: // 0xD0
			case 209: // 0xD1
			case 210: // 0xD2
			case 211: // 0xD3
			case 212: // 0xD4
			case 213: // 0xD5
			case 214: // 0xD6
			case 215: // 0xD7
			case 216: // 0xD8
			case 217: // 0xD9
			case 218: // 0xDA
			case 219: // 0xDB
			case 220: // 0xDC
			case 221: // 0xDD
			case 222: // 0xDE
			case 223: // 0xDF
				// LDM LOAD ACCUMULATOR IMMEDIATE
				// 4 bits of immediate data are loaded
				// into the accumulator
				printf("LDM\n");
				opr = hexval & 0b00001111;
				chip.ACCUMULATOR = opr;
				programcounter += 1;
				break;
			case 224: // 0xE0
				// WRM WRITE DATA RAM CHARACTER
				// The contents of the accumulator are
				// written into the DATA RAM character
				// specified by the last SRC instruction
				// [xx|xx|xxxx]
				//   ^ data ram chip
				//	^ register
				//	   ^ character
				addr = chip.RP[chip.SRCADDRREG];
				chr = (addr & 0b00001111);
				r = (addr >> 4) & (0b00000011);
				c = (addr >> 6) & (0b00000011);
				b = chip.DATARAMSELECTED;
				setRamRegisterCharacter(b, c, r, chr, chip.ACCUMULATOR);
				programcounter += 1;
				break;
			case 225: // 0xE1
				// WMP WRITE RAM PORT
				// The contents of the accumulator are
				// written to the output port associated
				// with the DATA RAM chip selected by last 
				// SRC insturction. This value will stay
				// at the output port until over written
				addr = chip.RP[chip.SRCADDRREG];
				r = (addr >> 4) & 0b00000011;
				chip.RAM_OUTPUT[chip.DATARAMSELECTED + r] =
					chip.ACCUMULATOR;
				programcounter += 1;
				break;
			case 226: // 0xE2
				// WRR WRITE ROM PORT
				// The contents of the accumulator are 
				// written to the output port associated
				// with the ROM selected by the last SRC instruction
				addr = chip.RP[chip.SRCADDRREG];
				r = (addr >> 4) & 0b00001111;
				chip.ROM_IO = chip.ACCUMULATOR;
				programcounter += 1;
				break;
			case 227: // 0xE3
				// WPM WRITE PROGRAM RAM
				// write the contents of the accumulator into a 
				// half byte of program RAM, or read the contents
				// of a half bytes of program RAM into ROM input
				// port where it can be accessed by a program
				printf("WPM\n");
				chip.RAM_OUTPUT[chip.DATARAMSELECTED] = 
					chip.ACCUMULATOR;
				programcounter += 1;
				break;
			case 228: // 0xE4
			case 229: // 0xE5
			case 230: // 0xE6
			case 231: // 0xE7
				// WRn WRITE DATA RAM STATUS CHARACTER
				// Data RAM status characters whoe number
				// from 0 to 3 is specified by field by n
				// DATA RAM register specified by the last 
				// SRC instruction, ar replaced by the contents
				// of the accumulator.
				// [xx|xx|xxxx]
				//  ^ data ram chip 
				//	^ register
				addr = chip.RP[chip.SRCADDRREG];
				chr =  hexval - 228;
				r = (addr >> 4) & (0b00000011);
				c = (addr >> 6) & (0b00000011);
				b = chip.DATARAMSELECTED;
				setRamRegisterCharacter(b, c, r, chr, chip.ACCUMULATOR);
				programcounter += 1;
				break;
			case 232: // 0xE8
				printf("SBM\n");
				programcounter += 1;
				break;
			case 233: // 0xE9
				// RDM READ DATA RAM DATA CHAR
				// The DATA RAM data character specified
				// by the last SRC instruction is loaded into
				// the accumulator.
				printf("RDM\n");
				addr = chip.RP[chip.SRCADDRREG];
				r = (addr & 0b00001111);
				c = (addr >> 4) & (0b00000011);
				b = (addr >> 6) & (0b00000011);
				idx = getDataRAMAddr(b, c, r);
				chip.ACCUMULATOR = chip.RAM[idx];
				programcounter += 1;
				break;
			case 234: // 0xEA
				printf("RDR\n");
				programcounter += 1;
				break;
			case 235: // 0xEB
				// ADM ADD DATA RAM TO ACCUMULATOR WITH CARRY
				// DATA RAM character specified by last SRC
				// instruction plus carry bit are added into
				// the accumulator
				addr = chip.RP[chip.SRCADDRREG];
				r = (addr & 0b00001111);
				c = (addr >> 4) & (0b00000011);
				b = (addr >> 6) & (0b00000011);
				idx = getDataRAMAddr(b, c, r);
				addToAccumulator(chip.RAM[idx]);
				programcounter += 1;
				break;
			case 236: // 0xEC
				// READ DATA RAM STATUS CHARACTER
				// RDR 0 - 3
				addr = chip.RP[chip.SRCADDRREG];
				chr =  hexval - 236;
				r = (addr >> 4) & (0b00000011);
				c = (addr >> 6) & (0b00000011);
				b = chip.DATARAMSELECTED;
				setRamRegisterCharacter(b, c, r, chr, chip.ACCUMULATOR);
				programcounter += 1;
				break;
			case 237: // 0xED
				printf("RD1\n");
				programcounter += 1;
				break;
			case 238: // 0xEE
				printf("RD2\n");
				programcounter += 1;
				break;
			case 239: // 0xEF
				printf("RD3\n");
				programcounter += 1;
				break;
			case 240: // 0xF0
				// CLB CLEAR BOTH
				// accumulator is set to 0000B and
				// carry bit is reset;
				chip.ACCUMULATOR = 0;
				chip.CARRYBIT = 0;
				programcounter += 1;
				break;
			case 241: // 0xF1
				// CLC CLEAR CARRY
				// carry bit is set to zero
				chip.CARRYBIT = 0;
				programcounter += 1;
				break;
			case 242: // 0xF2
				// IAC INCREMENT ACCUMULATOR
				// contents of the accumulator are
				// incremented by one; the carry bit
				// is set if there is a carry out of 
				// high order bit position, and reset 
				// if there is no carry.
				addToAccumulator(0b0001);
				programcounter += 1;
				break;
			case 243: // 0xF3
				// CMC COMPLEMENT CARRY
				// if carry bit is zero set to one
				// if carry bit is one set to zero
				chip.CARRYBIT = ~chip.CARRYBIT;
				programcounter += 1;
				break;
			case 244: // 0xF4
				// CMA COMPLEMENT ACCUMULATOR
				// Each bit of the contents of
				// accumulator is complemented
				// ( one's complement ) carry
				// bit not affected
				chip.ACCUMULATOR |= 0b1111;
				programcounter += 1;
				break;
			case 245: // 0xF5
				// RAL ROTATE ACCUMULATOR LEFT 
				// THROUGH CARRY
				// The contents of the accumulator 
				// are rotated one bit position to the left
				// high order bit of the accumulator replaces
				// carry bit while the low order bit of the 
				// the accumulator
				accumulator = chip.ACCUMULATOR >> 3;
				chip.ACCUMULATOR = 
					( chip.ACCUMULATOR << 1 ) | chip.CARRYBIT;
				chip.CARRYBIT = accumulator;
				programcounter += 1;
				break;
			case 246: // 0xF6
				// RAR ROTATE ACCUMULATOR RIGHT
				// THROUGH CARRY
				// opposite of RAL
				accumulator = chip.ACCUMULATOR << 3;
				chip.ACCUMULATOR = 
					( chip.ACCUMULATOR >> 1 ) | chip.CARRYBIT;
				chip.CARRYBIT = accumulator;
				programcounter += 1;
				break;
			case 247: // 0xF7
				// TCC TRANSMIT CARRY AND CLEAR
				// If carry bit = 0, the accumulator is set 0000B
				// If carry bit = 1, the accumulator is set 0001B
				// carry bit is reset;
				chip.ACCUMULATOR = 0b0001;
				if (chip.CARRYBIT = 0) 
				{
					chip.ACCUMULATOR = 0b0000;
				}
				chip.CARRYBIT = 0;
				programcounter += 1;
				break;
			case 248: // 0xF8
				// DAC DECREMENT ACCUMULATOR
				// the contents of the accumulator is decremented
				// by one. The carry bit is set if there is no
				// borrow out of the high order bit position, and 
				// rest if there is a borrow
				chip.ACCUMULATOR += 0b1111;
				if (chip.ACCUMULATOR >= 15) 
				{
					chip.CARRYBIT = 0;
				}
				chip.ACCUMULATOR += 0b1111;
				programcounter += 1;
				break;
			case 249: // 0xF9
				// TCS TRANSFER CARRY SUBTRACT
				// if carry bit is 0, the 
				// accumulator is set to 9. 
				// if carry bit is 1, the 
				// accumulator is set to 10
				// carry bit is reset
				chip.ACCUMULATOR = 9;
				if (chip.CARRYBIT == 1)
				{
					chip.ACCUMULATOR += 1;
				}
				programcounter += 1;
				break;
			case 250: // 0xFA
				// STC SET CARRY
				// set carry bit to 1
				chip.CARRYBIT = 1;
				programcounter += 1;
				break;
			case 251: // 0xFB
				// DAA DECIMAL ADJUST ACCUMULATOR
				// contents of the accumulator are greater
				// than 9, or if the carry bit = 1, accumulator
				// is incremented by 6;
				if ((chip.ACCUMULATOR > 9) 
					|| (chip.CARRYBIT))
				{
					addToAccumulator(6);
				}
				programcounter += 1;
				break;
			case 252: // 0xFC
				// KBP KEYBOARD PROCESS 
				// if accumulator contains 0000b; remains unchanged
				// one bit of accumulator is set; the accumulator
				// set a number from 1 to 4 indicating which bit 
				// was set
				switch (chip.ACCUMULATOR)
				{
					case 0b0000:
					case 0b0001:
					case 0b0010:
					case 0b0100:
					case 0b1000:
						break;
					default:
						chip.ACCUMULATOR = 0b1111;
						break;
				}
				programcounter += 1;
				break;
			case 253: // 0xFD
				// DCL DESGINATE COMMAND LINE
				// using rightmost 3 bits of accumulator
				// to determine which of the 4 data banks
				// will be referenced.
				chip.DATARAMSELECTED = 
					chip.ACCUMULATOR & 0b0111;
				programcounter += 1;
				break;
		}
	}
	return 0;
}
