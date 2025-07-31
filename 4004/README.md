> None of these components are fully built, its mostly experimentation; 

There are 3 components
- Assembler
- Disassembler
- Emulator

All the functions and structure related to the chip can be found on [`chip.h`](src/chip.h)

To build and run the assembler:
```
make assembler
assembler <sample.asm>
```

To build and run the diassembler:
```
make disasm
disasm <sample.obj>
```

To build and run the emulator
```
make emu
emu <sample.obj>
```

### references

- massive inspiration -> [`lpg2709` emulator-intel-4004](https://github.com/lpg2709/emulator-Intel-4004)
- [e4004 online emulator](http://e4004.szyc.org/)
- [pyntel4004](https://pyntel4004.readthedocs.io/en/latest/intro/4001.html)
