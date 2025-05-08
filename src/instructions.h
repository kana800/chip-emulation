#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#define INS_SIZE 46

const char* instructions[INS_SIZE] = {
    "ADD", "ADM", 
    "BBL", 
    "CLB", "CLC", "CMC", "CMA",
    "DAA", "DAC", "DCL", 
    "FIM", "FIN", 
    "IAC", "INC", "ISZ", 
    "JCN", "JIN", "JMS", "JUN", 
    "KBP", 
    "LDM", "LD", 
    "NOP", 
    "RAL", "RAR", "RD0", "RD1", "RD2","RD3", "RDM", "RDR", 
    "SBM", "SRC", "STC", "SUB",
    "TCC", "TCS", 
    "WR0", "WR1", "WR2", "WR3", "WMP","WPM", "WRM", "WRR", 
    "XCH"
};

#endif // INSTRUCTION_H
