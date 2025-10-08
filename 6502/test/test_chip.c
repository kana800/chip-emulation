#include <stdio.h>

#include "../src/chip.h"

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


}
