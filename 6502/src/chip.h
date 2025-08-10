#ifndef CHIP_H
#define CHIP_H

#include <stdio.h>
#include <stdint.h>

typedef struct __chip__
{
    uint16_t programcounter;
    uint8_t accumulator;
    uint8_t reg_x;
    uint8_t status_reg; // NV-BDIZC
    uint8_t stack_ptr;

    uint8_t stack[256];
} chip;


// sample
int opcode_adc();


#endif // CHIP_H
