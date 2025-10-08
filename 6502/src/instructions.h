#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdint.h>

#define INS_SIZE 56

void * instructions_func[56];

const char* instructions[INS_SIZE] = {
    "ADC", // add with carry
    "AND", // and (with accumulator)
    "ASL", // arithmetic shift left
    "BCC", // branch on carry clear
    "BCS", // branch on carry set
    "BEQ", // branch on equal (zero set)
    "BIT", // bit test
    "BMI", // branch on minus (negative set)
    "BNE", // branch on not equal (zero clear)
    "BPL", // branch on plus (negative clear)
    "BRK", // break / interrupt
    "BVC", // branch on overflow clear
    "BVS", // branch on overflow set
    "CLC", // clear carry
    "CLD", // clear decimal
    "CLI", // clear interrupt disable
    "CLV", // clear overflow
    "CMP", // compare (with accumulator)
    "CPX", // compare with X
    "CPY", // compare with Y
    "DEC", // decrement
    "DEX", // decrement X
    "DEY", // decrement Y
    "EOR", // exclusive or (with accumulator)
    "INC", // increment
    "INX", // increment X
    "INY", // increment Y
    "JMP", // jump
    "JSR", // jump subroutine
    "LDA", // load accumulator
    "LDX", // load X
    "LDY", // load Y
    "LSR", // logical shift right
    "NOP", // no operation
    "ORA", // or with accumulator
    "PHA", // push accumulator
    "PHP", // push processor status (SR)
    "PLA", // pull accumulator
    "PLP", // pull processor status (SR)
    "ROL", // rotate left
    "ROR", // rotate right
    "RTI", // return from interrupt
    "RTS", // return from subroutine
    "SBC", // subtract with carry
    "SEC", // set carry
    "SED", // set decimal
    "SEI", // set interrupt disable
    "STA", // store accumulator
    "STX", // store X
    "STY", // store Y
    "TAX", // transfer accumulator to X
    "TAY", // transfer accumulator to Y
    "TSX", // transfer stack pointer to X
    "TXA", // transfer X to accumulator
    "TXS", // transfer X to stack pointer
    "TYA" // transfer Y to accumulator 
};

// addressing mode 
// A accumulator
// a absolute
// x absolute, X-indexed
// y absolute, Y-indexed
// # immediate
// i implied
// n indirect
// X X-indexed, indirect
// Y indirect, Y-indexed
// r relative
// z zeropage
// p zeropage, X-indexed
// g zeropage, Y-indexed
const char* instruction_set[256] = {
    "BRK i", "ORA X", "", "", "", "ORA z", "ASL z", "",
    "PHP i", "ORA #", "ASL A", "", "", "ORA a", "ASL a", "",
    "BPL r", "ORA Y", "", "", "", "ORA p", "ASL p", "",
    "CLC i", "ORA y", "", "", "", "ORA x", "ASL x", "",
    "JSR a", "AND X", "", "", "BIT z", "AND z", "ROL z", "",
    "PLP i", "AND #", "ROL A", "", "BIT a", "AND a", "ROL a", "",
    "BMI r", "AND Y", "", "", "", "AND p", "ROL p", "",
    "SEC i", "AND y", "", "", "", "AND x", "ROL x", "",
    "RTI i", "EOR X", "", "", "", "EOR z", "LSR z", "",
    "PHA i", "EOR #", "LSR A", "", "JMP a", "EOR a", "LSR a", "",
    "BVC r", "EOR Y", "", "", "", "EOR p", "LSR p", "",
    "CLI i", "EOR y", "", "", "", "EOR x", "LSR x", "",
    "RTS i", "ADC X", "", "", "", "ADC z", "ROR z", "",
    "PLA i", "ADC #", "ROR A", "", "JMP n", "ADC a", "ROR a", "",
    "BVS r", "ADC Y", "", "", "", "ADC p", "ROR p", "",
    "SEI i", "ADC y", "", "", "", "ADC x", "ROR x", "",
    "", "STA X", "", "", "STY z", "STA z", "STX z", "",
    "DEY i", "", "TXA i", "", "STY a", "STA a", "STX a", "",
    "BCC r", "STA Y", "", "", "STY p", "STA p", "STX g", "",
    "TYA i", "STA y", "TXS i", "", "", "STA x", "", "",
    "LDY #", "LDA X", "LDX #", "", "LDY z", "LDA z", "LDX z", "",
    "TAY i", "LDA #", "TAX i", "", "LDY a", "LDA a", "LDX a", "",
    "BCS r", "LDA Y", "", "", "LDY p", "LDA p", "LDX g", "",
    "CLV i", "LDA y", "TSX i", "", "LDY x", "LDA x", "LDX y", "",
    "CPY #", "CMP X", "", "", "CPY z", "CMP z", "DEC z", "",
    "INY i", "CMP #", "DEX i", "", "CPY a", "CMP a", "DEC a", "",
    "BNE r", "CMP Y", "", "", "", "CMP p", "DEC p", "",
    "CLD i", "CMP y", "", "", "", "CMP x", "DEC x", "",
    "CPX #", "SBC X", "", "", "CPX z", "SBC z", "INC z", "",
    "INX i", "SBC #", "NOP i", "", "CPX a", "SBC a", "INC a", "",
    "BEQ r", "SBC Y", "", "", "", "SBC p", "INC p", "",
    "SED i", "SBC y", "", "", "", "SBC x", "INC x", ""
};

#endif // INSTRUCTION_H
