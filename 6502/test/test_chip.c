#include <stdio.h>

#include "../src/chip.h"

void* instructions_func[3] = {
	opcode_lda,
	opcode_clc,
        opcode_test
};

int main(int argc, char* argv[])
{
    // testing the zero page memory ranges
    uint8_t val = getZeroPage(1);
    assert (val == 0);

    // testing the stack
    // [x,x,x,x,x,x]
    for (int i = 0; i < 256; i++)
    {
        pushToStack(i);
    }

    for (int i = 255; i > 0; i--)
    {
        val = popFromStack();
        assert ( val == i );
    }

    // TESTING LDA
    // void opcode_lda(uint8_t mode, uint16_t opr)
    void (*lda)(uint8_t, uint16_t) = instructions_func[0];
    setZeroPage(82, 64);
    // mode: zeropage,X | LDA $80,X
    CHIP.reg_x = 2;
    (*lda)(12, 80);
    assert(CHIP.accumulator == 64);
    // mode: absolute | LDA opr | LDA $3010
    CHIP.RAM[3010] = 34;
    (*lda)(2, 3010);
    assert(CHIP.accumulator == 34);
    // mode: absolute, X | LDA opr, X | LDA $3120,X
    (*lda)(3,3008); // addres should be 3008 + 2 = 3010
    assert(CHIP.accumulator == 34);
    // mode: immediate | LDA #opr
    (*lda)(5,2);
    assert(CHIP.accumulator == 0);
    // mode: indirect, X
    CHIP.reg_y = 0x10;
    setZeroPage(0x70, 0x43);
    setZeroPage(0x71, 0x35);
    (*lda)(9,0x70);
    // mode: (indirect), X
    CHIP.reg_x = 0x05;
    setZeroPage(0x75,0x23);
    setZeroPage(0x76,0x30);
    CHIP.RAM[0x3023] = 0xA5;
    (*lda)(8,0x70);
    assert(CHIP.accumulator == 0xA5);


}
