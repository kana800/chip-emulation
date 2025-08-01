#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


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
	bool isoperand = false;
	bool isembed = false;
	uint8_t opr = 0;

	for (int i = 0; i < len; i += 3)
	{
		memcpy(hexcode, buffer + i, 2);
		int hexval = strtol(hexcode, NULL, 16);
//		printf("%d %s %d\n", i, hexcode, hexval);
		if (isoperand)
		{
			if (isembed)
			{
				printf("%d\n", hexval + opr);
				isembed = false;
			}
			else
			{
				printf("%d\n", hexval);
			}
			isoperand = false;
			continue;
		}
		
		opr = 0;

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
				printf("JCN "); // CN = 15
				opr = hexval & 0b00001111;
				isoperand = true;
				isembed = true;
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
				printf("FIM %dP ", opr);
				isoperand = true;
				break;
			case 33: // 0x21
			case 35: // 0x23
			case 37: // 0x25
			case 39: // 0x27
			case 41: // 0x29
			case 43: // 0x2B
			case 45: // 0x2D
			case 47: // 0x2F
				opr = (hexval >> 1) & 0b00000111; 
				printf("SRC %dP\n", opr);
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
				printf("FIN %dP\n", opr);
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
				printf("JUN ");
				opr = hexval & 0b00001111;
				isoperand = true;
				isembed = true;
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
				printf("JMS ");
				opr = hexval & 0b00001111;
				isoperand = true;
				isembed = true;
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
				opr = hexval & 0b00001111;
				printf("ISZ %dP ");
				isoperand = true;
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
				printf("ISZ\n");
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
				opr = hexval & 0b00001111;
				printf("LD R%d\n");
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
				opr = hexval & 0b00001111;
				printf("XCH R%d\n", opr);
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
				printf("BBL\n");
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
				printf("LDM\n");
				break;
			case 224: // 0xE0
				printf("WRM\n");
				break;
			case 225: // 0xE1
				printf("WMP\n");
				break;
			case 226: // 0xE2
				printf("WRR\n");
				break;
			case 227: // 0xE3
				printf("WPM\n");
				break;
			case 228: // 0xE4
				printf("WR0\n");
				break;
			case 229: // 0xE5
				printf("WR1\n");
				break;
			case 230: // 0xE6
				printf("WR2\n");
				break;
			case 231: // 0xE7
				printf("WR3\n");
				break;
			case 232: // 0xE8
				printf("SBM\n");
				break;
			case 233: // 0xE9
				printf("RDM\n");
				break;
			case 234: // 0xEA
				printf("RDR\n");
				break;
			case 235: // 0xEB
				printf("ADM\n");
				break;
			case 236: // 0xEC
				printf("RD0\n");
				break;
			case 237: // 0xED
				printf("RD1\n");
				break;
			case 238: // 0xEE
				printf("RD2\n");
				break;
			case 239: // 0xEF
				printf("RD3\n");
				break;
			case 240: // 0xF0
				printf("CLB\n");
				break;
			case 241: // 0xF1
				printf("CLC\n");
				break;
			case 242: // 0xF2
				printf("IAC\n");
				break;
			case 243: // 0xF3
				printf("CMC\n");
				break;
			case 244: // 0xF4
				printf("CMA\n");
				break;
			case 245: // 0xF5
				printf("RAL\n");
				break;
			case 246: // 0xF6
				printf("RAR\n");
				break;
			case 247: // 0xF7
				printf("TCC\n");
				break;
			case 248: // 0xF8
				printf("DAC\n");
				break;
			case 249: // 0xF9
				printf("TCS\n");
				break;
			case 250: // 0xFA
				printf("STC\n");
				break;
			case 251: // 0xFB
				printf("DAA\n");
				break;
			case 252: // 0xFC
				printf("KBP\n");
				break;
			case 253: // 0xFD
				printf("DCL\n");
				break;
		}
	}

	return 0;
}
