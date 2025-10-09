#ifndef CHIP_H
#define CHIP_H

#include <stdio.h>
#include <stdint.h>
#include <assert.h>

typedef struct __chip__
{
    uint16_t programcounter;
    uint8_t accumulator;
    uint8_t reg_x;
    uint8_t reg_y;
    uint8_t status_register_flags; // NV-BDIZC
    uint8_t stack_ptr;

    uint8_t RAM[64*1024];

} chip_t;

static chip_t CHIP;

// set zero page address value
uint8_t getZeroPage(uint8_t addr)
{
    assert(addr >= 0 && addr < 256);
    return CHIP.RAM[addr];
};

void setZeroPage(uint8_t addr, uint8_t val)
{
    assert(addr >= 0 && addr < 256);
    CHIP.RAM[addr] = val;
    return;
};

uint8_t popFromStack()
{
    CHIP.stack_ptr--;
    uint16_t addr = CHIP.stack_ptr + 0x0100;
    assert(addr >= 0x0100 && addr <= 0x01FF);
    return CHIP.RAM[addr];
};

void pushToStack(uint8_t val)
{
    uint16_t addr = CHIP.stack_ptr + 0x0100;
    assert(addr >= 0x0100 && addr <= 0x01FF);
    CHIP.RAM[addr] = val;
    CHIP.stack_ptr++;
    return;
};

// add memory to accumulator with carry
void opcode_adc(uint8_t mode, uint8_t opr)
{

};

// clear the carry flag
void opcode_clc()
{
};

// load accumulator with memory
// STATUS REG 
//  N Z C I D V
//  + + - - - -
void opcode_lda(uint8_t mode, uint8_t opr)
{
    switch (mode)
    {
        case 0: // immediate
        case 1: // zeropage
        case 2: // zeropage, X
        case 3: // zeropage, Y
        case 4: // absolute
        case 5: // absolute, X
    }
    CHIP.accumulator = CHIP.RAM[opr];
    CHIP.status_register_flags &= 0b11000000;
    return;
};


void opcode_test(uint8_t mode, uint8_t opr)
{
    printf("test function %d %d\n", mode, opr);
    return;
}

#endif // CHIP_H
