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


// Addressing Mode: 1 -> A – OPC A
// Addressing Mode: 2 -> abs – OPC $LLHH
// Addressing Mode: 3 -> abs,X – OPC $LLHH,X
// Addressing Mode: 4 -> abs,Y – OPC $LLHH,Y
// Addressing Mode: 5 -> # – OPC #$BB
// Addressing Mode: 6 -> impl – OPC
// Addressing Mode: 7 -> ind – OPC ($LLHH)
// Addressing Mode: 8 -> X,ind – OPC ($LL,X)
// Addressing Mode: 9 -> ind,Y – OPC ($LL),Y
// Addressing Mode: 10 -> rel – OPC $BB
// Addressing Mode: 11 -> zpg – OPC $LL
// Addressing Mode: 12 -> zpg,X – OPC $LL,X
// Addressing Mode: 13 -> zpg,Y – OPC $LL,Y

// add memory to accumulator with carry
void opcode_adc(uint8_t mode, uint8_t opr)
{

};

//  LDA Load Accumulator with Memory
//  STATUS REG:
//      N Z C I D V
//      + + - - - -
void opcode_lda(uint8_t mode, uint16_t opr)
{
    uint8_t data = 0x0;
    uint8_t b1 = 0x0;
    uint8_t b2 = 0x0;
    // lua stands for look up address :)
    uint16_t lua = 0x0;
    // Addressing Mode: 2 -> abs – OPC $LLHH
    // Addressing Mode: 3 -> abs,X – OPC $LLHH,X
    // Addressing Mode: 4 -> abs,Y – OPC $LLHH,Y
    // Addressing Mode: 5 -> # – OPC #$BB
    // Addressing Mode: 7 -> ind – OPC ($LLHH)
    // Addressing Mode: 8 -> X,ind – OPC ($LL,X)
    // Addressing Mode: 9 -> ind,Y – OPC ($LL),Y
    // Addressing Mode: 11 -> zpg – OPC $LL
    // Addressing Mode: 12 -> zpg,X – OPC $LL,X
    switch (mode)
    {
        case 5: // immediate | LDA #opr
            data = CHIP.RAM[opr];
            break;
        case 11: // zeropage | LDA opr
            data = getZeroPage(opr);
            break;
        case 12: // zeropage,X | LDA opr, X
            data = getZeroPage(opr + CHIP.reg_x);
            break;
        case 2: // absolute | LDA opr
            data = CHIP.RAM[opr];
            break;
        case 3: // absolute, X | LDA opr, X
            data = CHIP.RAM[opr + CHIP.reg_x];
            break;
        case 4: // absolute, Y | LDA opr, Y
            data = CHIP.RAM[opr + CHIP.reg_y];
            break;
        case 8: // indirect,X | LDA (opr,X)
            b1 = getZeroPage(opr + CHIP.reg_x);
            b2 = getZeroPage(opr + CHIP.reg_x + 0x1);
            lua = ((b2 << 8) | b1 );
            data = CHIP.RAM[lua];
            break;
        case 9: // indirect, Y | LDA (opr),Y
            b1 = getZeroPage(opr);
            b2 = getZeroPage(opr + 0x1);
            lua = ((b2 << 8) | b1 ) + CHIP.reg_y;
            data = CHIP.RAM[lua];
            break;
    }
    CHIP.accumulator = data;
    CHIP.status_register_flags &= 0b11000000;
    return;
};

//  AND Memory With Accumulator
//  A AND M -> A
//  STATUS REG:
//      N Z C I D V
//      + + - - - -
void opcode_and(uint8_t mode, uint16_t opr)
{
    uint8_t data = 0x0;
    uint8_t b1 = 0x0;
    uint8_t b2 = 0x0;
    // lua stands for look up address :)
    uint16_t lua = 0x0;
    // Addressing Mode: 2 -> abs – OPC $LLHH
    // Addressing Mode: 3 -> abs,X – OPC $LLHH,X
    // Addressing Mode: 4 -> abs,Y – OPC $LLHH,Y
    // Addressing Mode: 5 -> # – OPC #$BB
    // Addressing Mode: 7 -> ind – OPC ($LLHH)
    // Addressing Mode: 8 -> X,ind – OPC ($LL,X)
    // Addressing Mode: 9 -> ind,Y – OPC ($LL),Y
    // Addressing Mode: 11 -> zpg – OPC $LL
    // Addressing Mode: 12 -> zpg,X – OPC $LL,X
    switch (mode)
    {
        case 5: // immediate | AND #opr
            data = CHIP.RAM[opr];
            break;
        case 11: // zeropage | AND opr
            data = getZeroPage(opr);
            break;
        case 12: // zeropage,X | AND opr, X
            data = getZeroPage(opr + CHIP.reg_x);
            break;
        case 2: // absolute | AND opr
            data = CHIP.RAM[opr];
            break;
        case 3: // absolute, X | AND opr, X
            data = CHIP.RAM[opr + CHIP.reg_x];
            break;
        case 4: // absolute, Y | AND opr, Y
            data = CHIP.RAM[opr + CHIP.reg_y];
            break;
        case 8: // indirect,X | AND (opr,X)
            b1 = getZeroPage(opr + CHIP.reg_x);
            b2 = getZeroPage(opr + CHIP.reg_x + 0x1);
            lua = ((b2 << 8) | b1 );
            data = CHIP.RAM[lua];
            break;
        case 9: // indirect, Y | AND (opr),Y
            b1 = getZeroPage(opr);
            b2 = getZeroPage(opr + 0x1);
            lua = ((b2 << 8) | b1 ) + CHIP.reg_y;
            data = CHIP.RAM[lua];
            break;
    }
    CHIP.accumulator &= CHIP.RAM[data];
    CHIP.status_register_flags &= 0b00110000;
    return;
};

//  CLC Clear Carry Flag
//  STATUS REG
//      N Z C I D V
//      - - 0 - - -
void opcode_clc(uint8_t mode, uint16_t opr)
{
    assert(mode == 6);
    CHIP.status_register_flags &= 0b00110111;
    return;
}

// CLD Clear Decimal Mode
//  STATUS REG
//      N Z C I D V
//      - - - - 0 -
void opcode_cld(uint8_t mode, uint16_t opr)
{
    assert(mode == 6);
    CHIP.status_register_flags &= 0b00111101;
    return;
}

// INX Increment Index X by One
void opcode_inx(uint8_t mode, uint16_t opr)
{
    assert(mode == 6);
    CHIP.reg_x += 1;
    CHIP.status_register_flags &= 0b00110000;
    return;
}

// INY Increment Index Y by One
void opcode_iny(uint8_t mode, uint16_t opr)
{
    assert(mode == 6);
    CHIP.reg_y += 1;
    CHIP.status_register_flags &= 0b00110000;
    return;
}

void opcode_nop(uint8_t mode, uint16_t opr)
{
    return;
}

// SEC Set Carry Flag
void opcode_sec(uint8_t mode)
{
    CHIP.status_register_flags &= 0b00001000;
    return;
}


void opcode_test(uint8_t mode, uint16_t opr)
{
    printf("test function %d %d\n", mode, opr);
    return;
};

#endif // CHIP_H
