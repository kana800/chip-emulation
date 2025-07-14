#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#define INS_SIZE 46

#include <stdint.h>

const char* instructions[INS_SIZE] = {
    "ADD", "ADM", 
    "BBL", 
    "CLB", "CLC", "CMC", "CMA",
    "DAA", "DAC", "DCL", 
    "FIM", "FIN", 
    "IAC", "INC", "ISZ", 
    "JCN",  "JIN", "JMS", "JUN", 
    "KBP", 
    "LDM", "LD", 
    "NOP", 
    "RAL", "RAR", "RD0", "RD1", "RD2","RD3", "RDM", "RDR", 
    "SBM", "SRC", "STC", "SUB",
    "TCC", "TCS", 
    "WR0", "WR1", "WR2", "WR3", "WMP","WPM", "WRM", "WRR", 
    "XCH"
};

const uint8_t instructions_hex[INS_SIZE] = {
    0b10000000, 0b11101011, // ADD, ADM
    0b11000000, // BBL
    0b11110000, 0b11110001, 0b11110011, 0b11110100, // CLB, CLC, CMC, CMA
    0b11111011, 0b11111000, 0b11111101, // DAA, DAC, DCL
    0b00100000, 0b00110000, // FIM**, FIN
    0b11110010, 0b01100000, 0b01110000, // IAC, INC, ISZ**
    0b00010000, 0b00110001, 0b01010000, 0b01000000, // JCN**, JIN, JMS**, JUN**
    0b11111100, // KBP
    0b11010000, 0b10100000, // LDM, LD
    0b00000000, // NOP
    0b11110101, 0b11110110, 0b11101100, 0b11101101, 0b11101110, 0b11101111,
    0b11101001, 0b11101010,
    0b11101000, 0b00100001, 0b11111010, 0b10010000,
    0b11110111, 0b11111001, 
    0b11100100, 0b11100101, 0b11100110, 0b11100111, 
    0b11100001, 0b11100011, 0b11100000, 0b11100010,
    0b10110000
};



#endif // INSTRUCTION_H
